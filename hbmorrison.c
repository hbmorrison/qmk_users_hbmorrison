/*
Copyright 2023 Hannah Blythe Morrison

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "hbmorrison.h"

// State of the M_ALT_TAB macro - true if we are currently tabbing between
// windows.

static bool hbm_alt_tab_pressed = false;

// State of the shift modifiers - used to turn shift off for symbol layers.

static bool hbm_shift_pressed = false;
static bool hbm_os_shift_pressed = false;

// Tapdance for right shift key to add capsword.

typedef enum {
  TD_NONE,
  TD_UNKNOWN,
  TD_SINGLE_TAP,
  TD_DOUBLE_TAP,
  TD_HOLD
} td_state_t;

typedef struct {
  td_state_t state;
} td_tap_t;

static td_tap_t td_rsft = {
  .state = TD_NONE
};

td_state_t td_get_state(tap_dance_state_t *state);

void rsft_finished(tap_dance_state_t *state, void *user_data);
void rsft_reset(tap_dance_state_t *state, void *user_data);

tap_dance_action_t tap_dance_actions[] = {
    [TD_RSFT] = ACTION_TAP_DANCE_FN_ADVANCED(NULL, rsft_finished, rsft_reset)
};

// Process key presses.

bool process_record_user(uint16_t keycode, keyrecord_t *record) {

  // Stop pressing the KC_LALT key once M_ALT_TAB is no longer being pressed.

  if (keycode != M_ALT_TAB && hbm_alt_tab_pressed) {
    unregister_code(KC_LALT);
    hbm_alt_tab_pressed = false;
  }

  // Ensure that shift is not pressed when additional layers are active, aside
  // from a few exceptions. This ensures that symbol keypresses will always
  // produce the unshifted symbol, unless explicitly shifted with LSFT() in
  // code.

  uint8_t current_layer = get_highest_layer(layer_state);

  if (record->event.pressed) {
    if (current_layer > LAYER_BASE) {
      switch (keycode) {
        // Allow these keys in the extended layers to be shifted.
        case KC_TAB:
        case KC_Z:
        case KC_SLSH:
          break;
        // Remove the shift modifier for everything else.
        default:
          hbm_shift_pressed = get_mods() & MOD_BIT(KC_LSFT);
          hbm_os_shift_pressed = get_oneshot_mods() & MOD_BIT(KC_LSFT);
          del_mods(MOD_MASK_SHIFT);
          del_oneshot_mods(MOD_MASK_SHIFT);
      }
    }
  } else {
    if (hbm_shift_pressed) {
      add_mods(MOD_BIT(KC_LSFT));
      hbm_shift_pressed = false;
    }
    if (hbm_os_shift_pressed) {
      add_oneshot_mods(MOD_BIT(KC_LSFT));
      hbm_os_shift_pressed = false;
    }
  }

  // Only allow left-hand shift and crtl mods to work with the right side of the
  // keyboard and vice versa.

  if (record->event.pressed && current_layer == LAYER_BASE) {
    if (get_mods() & MOD_BITS_LEFT) {
      switch (keycode) {
        case KC_Q:
        case KC_W:
        case KC_F:
        case KC_P:
        case KC_B:
        case KC_A:
        case KC_R:
        case KC_S:
        case KC_T:
        case KC_G:
        case KC_Z_RSYM:
        case KC_X_GUI:
        case KC_C_ALT:
        case KC_D_CTL:
        case KC_V:
          clear_mods();
      }
    }
    if (get_mods() & MOD_BITS_RIGHT) {
      switch (keycode) {
        case KC_J:
        case KC_L:
        case KC_U:
        case KC_Y:
        case KC_BSPC:
        case KC_M:
        case KC_N:
        case KC_E:
        case KC_I:
        case KC_O:
        case KC_K:
        case KC_H_CTL:
        case KC_COMMA_ALT:
        case KC_DOT_GUI:
        case KC_SLSH_LSYM:
          clear_mods();
      }
    }
  }

  switch (keycode) {

    // Hold down KC_LALT persistantly to allow tabbing through windows.

    case M_ALT_TAB:
      if (record->event.pressed) {
        if (!hbm_alt_tab_pressed) {
          register_code(KC_LALT);
          hbm_alt_tab_pressed = true;
        }
        tap_code(KC_TAB);
      }
      break;

    // Switch between virtual desktops.

    case M_NDESK:
      if (record->event.pressed) {
        SEND_STRING(SS_DOWN(X_LCTL)SS_DOWN(X_LGUI));
        SEND_STRING(SS_TAP(X_RGHT));
        SEND_STRING(SS_UP(X_LGUI)SS_UP(X_LCTL));
      }
      break;
    case M_PDESK:
      if (record->event.pressed) {
        SEND_STRING(SS_DOWN(X_LCTL)SS_DOWN(X_LGUI));
        SEND_STRING(SS_TAP(X_LEFT));
        SEND_STRING(SS_UP(X_LGUI)SS_UP(X_LCTL));
      }
      break;

  }

  return true;
}

// Only capitalise alpha characters and remove the minus character so that
// typing '-' stops the caps word.

bool caps_word_press_user(uint16_t keycode) {
  switch (keycode) {
    // Keycodes that continue caps word with shift applied.
    case KC_A ... KC_Z:
      // Apply shift to next key only.
      add_weak_mods(MOD_BIT(KC_LSFT));
      return true;
    // Keycodes that continue caps word without shifting.
    case KC_1 ... KC_0:
    case KC_BSPC:
    case KC_DEL:
    case KC_UNDS:
      return true;
    // Do not deactivate if symbol, num or nav layer keys are held down.
    case KC_Z_RSYM:
    case KC_SLSH_LSYM:
    case KC_ENT_NUM:
    case KC_SPC_NAV:
      return true;
    // Deactivate caps word by default.
    default:
      return false;
  }
}

// Set the tapping terms for modifiers and layer keys.

uint16_t get_tapping_term(uint16_t keycode, keyrecord_t *record) {
  switch (keycode) {
    // Set the tapping term for the modifiers.
    case KC_D_CTL:
    case KC_C_ALT:
    case KC_X_GUI:
    case KC_H_CTL:
    case KC_COMMA_ALT:
    case KC_DOT_GUI:
    case KC_F7_CTL:
    case KC_F8_ALT:
    case KC_F9_GUI:
      return TAPPING_TERM_MODS;
    // Set the tapping term for layer keys.
    case KC_Z_RSYM:
    case KC_SLSH_LSYM:
    case KC_ENT_NUM:
    case KC_SPC_NAV:
      return TAPPING_TERM_LAYER;
    default:
      return TAPPING_TERM;
  }
}

// Disable permissive hold on the Windows mod tap keys.

bool get_permissive_hold(uint16_t keycode, keyrecord_t *record) {
  switch (keycode) {
    case KC_X_GUI:
    case KC_DOT_GUI:
      return false;
    default:
      return true;
  }
}

td_state_t td_get_state(tap_dance_state_t *state) {

  if (state->count == 1) {
    // If the keypress has been interrupted by another keypress or is no longer
    // held down by the end of the tap time, then we know it was just a single
    // tap.
    if (state->interrupted || !state->pressed)
      return TD_SINGLE_TAP;
    else
      // If it is still held at the end of the tap time, then it is a hold.
      return TD_HOLD;
  } else if (state->count > 1) {
    // In the case where there has been more than one tap in the tap time, we
    // have to make a judgement call. If the key is still held down we will
    // assume it was a flutter and call it a hold.
    if (state->pressed)
      return TD_HOLD;
    // If the keypress has finished or has been interrupted then we will assume
    // that there was some actual fast typing going on and issue two taps, which
    // will be the most common scenario.
    if (state->interrupted || !state->pressed)
      return TD_DOUBLE_TAP;
  }

  return TD_UNKNOWN;
}

// Enable capsword on right shift.

void rsft_finished(tap_dance_state_t *state, void *user_data) {
    td_rsft.state = td_get_state(state);
    switch (td_rsft.state) {
        case TD_SINGLE_TAP:
          add_oneshot_mods(MOD_BIT(KC_RSFT));
          break;
        case TD_DOUBLE_TAP:
          caps_word_toggle();
          break;
        case TD_HOLD:
          add_mods(MOD_BIT(KC_RSFT));
          break;
        default:
          break;
    }
}

void rsft_reset(tap_dance_state_t *state, void *user_data) {
    switch (td_rsft.state) {
        case TD_HOLD:
          del_mods(MOD_BIT(KC_RSFT));
        default:
          break;
    }
    td_rsft.state = TD_NONE;
}
