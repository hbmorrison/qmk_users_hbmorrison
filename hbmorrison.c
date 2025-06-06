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

uint16_t get_simple_keycode(uint16_t keycode);

// Defines whether to issue Windows or ChromeOS keypresses from macros - Windows
// by default.

static uint8_t hbm_operatingsystem = OS_WINDOWS;

// Declare operating specific keycode handling functions.

bool process_record_user_windows(uint16_t keycode, keyrecord_t *record);
bool process_record_user_chromeos(uint16_t keycode, keyrecord_t *record);
bool process_record_user_linux(uint16_t keycode, keyrecord_t *record);

// State of the M_ALT_TAB macro - true if we are currently tabbing between
// windows.

static bool hbm_alt_tab_pressed = false;

// State for managing shift backspace behaviour.

static bool hbm_del_registered = false;

// Used to temporarily store the state of the mod keys during shift backspace.

static uint8_t hbm_mod_state = 0;

// True if left- or right-shift tap dance keys are currently being held.

bool td_lsft_held = false;
bool td_rsft_held = false;

// Tapdance for multi-function shift keys.

void lsft_tap(tap_dance_state_t *state, void *user_data);
void lsft_finished(tap_dance_state_t *state, void *user_data);
void lsft_reset(tap_dance_state_t *state, void *user_data);
void rsft_tap(tap_dance_state_t *state, void *user_data);
void rsft_finished(tap_dance_state_t *state, void *user_data);
void rsft_reset(tap_dance_state_t *state, void *user_data);

tap_dance_action_t tap_dance_actions[] = {
  [TD_LSFT] = ACTION_TAP_DANCE_FN_ADVANCED(lsft_tap, lsft_finished, lsft_reset),
  [TD_RSFT] = ACTION_TAP_DANCE_FN_ADVANCED(rsft_tap, rsft_finished, rsft_reset),
};

// Process key presses.

bool process_record_user(uint16_t keycode, keyrecord_t *record) {

  // Stop pressing the KC_LALT key once M_ALT_TAB is no longer being pressed.

  if (keycode != M_ALT_TAB && hbm_alt_tab_pressed) {
    unregister_code(KC_LALT);
    hbm_alt_tab_pressed = false;
  }

  // Only allow left-hand modifiers to work with the right side of the keyboard
  // and vice versa.

  if (record->event.pressed && get_highest_layer(layer_state) == LAYER_BASE) {

    uint16_t simple_keycode = get_simple_keycode(keycode);

    // Check if any left-hand mods are present. If there are, ignore any
    // keypress on the left-hand side of the keyboard.

    uint8_t left_mods = get_mods() & MOD_BITS_LEFT;
    uint8_t left_oneshot_mods = get_oneshot_mods() & MOD_BITS_LEFT;

    if (left_mods || left_oneshot_mods) {
      switch (simple_keycode) {
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
        case KC_Z:
        case KC_X:
        case KC_C:
        case KC_D:
        case KC_V:
          del_oneshot_mods(left_oneshot_mods);
          return false;
      }
    }

    // Check if any right-hand mods are present. If there are, ignore any
    // keypress on the right-hand side of the keyboard.

    uint8_t right_mods = get_mods() & MOD_BITS_RIGHT;
    uint8_t right_oneshot_mods = get_oneshot_mods() & MOD_BITS_RIGHT;

    if (right_mods || right_oneshot_mods) {
      switch (simple_keycode) {
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
        case KC_H:
        case KC_COMMA:
        case KC_DOT:
        case KC_SLSH:
          del_oneshot_mods(right_oneshot_mods);
          return false;
      }
    }

  }

  // Store current modifiers for the shift + backspace funtionality.

  hbm_mod_state = get_mods();

  // Process generic keycodes.

  switch (keycode) {

    // Shift + backspace sends delete.

    case KC_BSPC:
      if (record->event.pressed) {
        if (hbm_mod_state & MOD_MASK_SHIFT) {
          del_mods(MOD_MASK_SHIFT);
          register_code(KC_DEL);
          hbm_del_registered = true;
          set_mods(hbm_mod_state);
          return false;
        }
      } else {
        if (hbm_del_registered) {
          unregister_code(KC_DEL);
          hbm_del_registered = false;
          return false;
        }
      }
      break;

    // Escape key followed by colon.

    case M_ESC_COLN:
      if (record->event.pressed) {
        tap_code(KC_ESC);
        SEND_STRING(SS_DELAY(100));
        tap_code16(KC_COLN);
      }
      break;

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

    // Swap between Windows, ChromeOS and Linux shortcuts.

    case M_ISWINDOWS:
      if (record->event.pressed) {
        hbm_operatingsystem = OS_WINDOWS;
      }
      break;

    case M_ISCHROMEOS:
      if (record->event.pressed) {
        hbm_operatingsystem = OS_CHROMEOS;
      }
      break;

    case M_ISLINUX:
      if (record->event.pressed) {
        hbm_operatingsystem = OS_LINUX;
      }
      break;

  }

  // Process operating system specific keycodes.

  switch (hbm_operatingsystem) {
    case OS_WINDOWS:
      return process_record_user_windows(keycode, record);
    case OS_CHROMEOS:
      return process_record_user_chromeos(keycode, record);
    case OS_LINUX:
      return process_record_user_linux(keycode, record);
  }

  return true;

}

// Convert any layer or mod keypresses into their alternative simple keypresses.

uint16_t get_simple_keycode(uint16_t keycode) {
  switch (keycode) {
    case KC_HR_LGUI:
      return KC_LGUI_KEY;
    case KC_HR_LALT:
      return KC_LALT_KEY;
    case KC_HR_LCTL:
      return KC_LCTL_KEY;
    case KC_HR_LCA:
      return KC_LCA_KEY;
    case KC_HR_RGUI:
      return KC_RGUI_KEY;
    case KC_HR_RALT:
      return KC_RALT_KEY;
    case KC_HR_RCTL:
      return KC_RCTL_KEY;
    case KC_HR_RCA:
      return KC_RCA_KEY;
    default:
      return keycode;
  }
}

bool process_record_user_windows(uint16_t keycode, keyrecord_t *record) {

  switch (keycode) {

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

    case M_OVERVIEW:
      if (record->event.pressed) {
        SEND_STRING(SS_DOWN(X_LGUI));
        SEND_STRING(SS_TAP(X_TAB));
        SEND_STRING(SS_UP(X_LGUI));
      }
      break;

    // Full screen.

    case M_FULLSCREEN:
      if (record->event.pressed) {
        SEND_STRING(SS_TAP(X_F11));
      }
      break;

    // Open the emoji window.

    case M_EMOJI:
      if (record->event.pressed) {
        SEND_STRING(SS_DOWN(X_LGUI)SS_TAP(X_SCLN)SS_UP(X_LGUI));
      }
      break;

  }

  return true;
}

bool process_record_user_chromeos(uint16_t keycode, keyrecord_t *record) {

  switch (keycode) {

    // Switch between virtual desktops.

    case M_NDESK:
      if (record->event.pressed) {
        SEND_STRING(SS_DOWN(X_LGUI));
        SEND_STRING(SS_TAP(X_RBRC));
        SEND_STRING(SS_UP(X_LGUI));
      }
      break;

    case M_PDESK:
      if (record->event.pressed) {
        SEND_STRING(SS_DOWN(X_LGUI));
        SEND_STRING(SS_TAP(X_LBRC));
        SEND_STRING(SS_UP(X_LGUI));
      }
      break;

    case M_OVERVIEW:
      if (record->event.pressed) {
        SEND_STRING(SS_TAP(X_F5));
      }
      break;

    // Open the emoji window.

    case M_EMOJI:
      if (record->event.pressed) {
        SEND_STRING(SS_DOWN(X_LSFT)SS_DOWN(X_LGUI));
        SEND_STRING(SS_TAP(X_SPC));
        SEND_STRING(SS_UP(X_LGUI)SS_UP(X_LSFT));
      }
      break;

  }

  return true;
}

bool process_record_user_linux(uint16_t keycode, keyrecord_t *record) {

  switch (keycode) {

    // Switch between virtual desktops.

    case M_NDESK:
      if (record->event.pressed) {
        SEND_STRING(SS_DOWN(X_LCTL)SS_DOWN(X_LALT));
        SEND_STRING(SS_TAP(X_RGHT));
        SEND_STRING(SS_UP(X_LALT)SS_UP(X_LCTL));
      }
      break;

    case M_PDESK:
      if (record->event.pressed) {
        SEND_STRING(SS_DOWN(X_LCTL)SS_DOWN(X_LALT));
        SEND_STRING(SS_TAP(X_LEFT));
        SEND_STRING(SS_UP(X_LALT)SS_UP(X_LCTL));
      }
      break;

    case M_OVERVIEW:
      if (record->event.pressed) {
        SEND_STRING(SS_DOWN(X_LCTL)SS_DOWN(X_LALT));
        SEND_STRING(SS_TAP(X_DOWN));
        SEND_STRING(SS_UP(X_LALT)SS_UP(X_LCTL));
      }
      break;
  }

  return true;
}

// Only capitalise alpha characters and remove the minus character so that
// typing '-' stops the caps word.

bool caps_word_press_user(uint16_t keycode) {

  switch (get_simple_keycode(keycode)) {

    // Keycodes that continue caps word with left- or right-shift applied.

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
    case KC_Z:
    case KC_X:
    case KC_C:
    case KC_D:
    case KC_V:
      add_weak_mods(MOD_BIT(KC_RSFT));
      return true;

    case KC_J:
    case KC_L:
    case KC_U:
    case KC_Y:
    case KC_M:
    case KC_N:
    case KC_E:
    case KC_I:
    case KC_O:
    case KC_K:
    case KC_H:
      add_weak_mods(MOD_BIT(KC_LSFT));
      return true;

    // Keys that continue caps word but are not shifted themselves.

    case KC_1 ... KC_0:
    case KC_BSPC:
    case KC_DEL:
    case KC_UNDS:
      return true;

    // Do not deactivate if sym, num or nav layer keys are pressed.

    case KC_NUM:
    case KC_NAV:
    case KC_TD_LSFT:
    case KC_TD_RSFT:
      return true;

    // Deactivate caps word by default.

    default:
      return false;

  }
}

// Set the tapping terms for layer keys.

uint16_t get_tapping_term(uint16_t keycode, keyrecord_t *record) {
  switch (keycode) {
    case KC_NUM:
    case KC_NAV:
    case KC_FUNC:
    case KC_CTLS:
      return TAPPING_TERM_LAYER;
    case KC_TD_LSFT:
    case KC_TD_RSFT:
      return TAPPING_TERM_SHIFT_TAP_DANCE;
    default:
      return TAPPING_TERM;
  }
}

// Set which keys get retro tapping.

bool get_retro_tapping(uint16_t keycode, keyrecord_t *record) {
  switch (keycode) {
    case KC_NUM:
    case KC_NAV:
      return true;
    default:
      return false;
  }
}

// Tap dance on left-shift.

void lsft_tap(tap_dance_state_t *state, void *user_data) {

  // If the tap dance count has just been incremented for the first time then we
  // could be heading towards a tap or a hold. We will default to hold and
  // activate the sym layer. If it turns out to be a tap after all, the layer
  // can be removed below.

  if (state->count == 1) {
    td_lsft_held = true;
    layer_on(LAYER_RSYM);
  }

}

void lsft_finished(tap_dance_state_t *state, void *user_data) {

  // If another key has been pressed during TAPPING_TERM_SHIFT_TAP_DANCE and the
  // tap dance key is no longer pressed then there has been a tap, so clear the
  // sym layer and figure out what to do with the taps.

  if (state->interrupted && !state->pressed) {

    td_lsft_held = false;
    layer_off(LAYER_RSYM);

    if (state->count == 1) {
      if (is_caps_word_on())
        caps_word_off();
      add_oneshot_mods(MOD_BIT(KC_LSFT));
    } else {
      caps_word_toggle();
    }

  }

}

void lsft_reset(tap_dance_state_t *state, void *user_data) {

  // If the tap dance key was held until this point then we have been in the sym
  // layer, so we can remove the sym layer now that the tap dance key has been
  // lifted.

  if (td_lsft_held) {
    td_lsft_held = false;
    layer_off(LAYER_RSYM);
  }

}

// Tap dance on right-shift.

void rsft_tap(tap_dance_state_t *state, void *user_data) {

  // If the tap dance count has just been incremented for the first time then we
  // could be heading towards a tap or a hold. We will default to hold and
  // activate the sym layer. If it turns out to be a tap after all, the layer
  // can be removed below.

  if (state->count == 1) {
    td_rsft_held = true;
    layer_on(LAYER_LSYM);
  }

}

void rsft_finished(tap_dance_state_t *state, void *user_data) {

  // If another key has been pressed during TAPPING_TERM_SHIFT_TAP_DANCE and the
  // tap dance key is no longer pressed then there has been a tap, so clear the
  // sym layer and figure out what to do with the taps.

  if (state->interrupted && !state->pressed) {

    td_rsft_held = false;
    layer_off(LAYER_LSYM);

    if (state->count == 1) {
      if (is_caps_word_on())
        caps_word_off();
      add_oneshot_mods(MOD_BIT(KC_RSFT));
    } else {
      caps_word_toggle();
    }

  }

}

void rsft_reset(tap_dance_state_t *state, void *user_data) {

  // If the tap dance key was held until this point then we have been in the sym
  // layer, so we can remove the sym layer now that the tap dance key has been
  // lifted.

  if (td_rsft_held) {
    td_rsft_held = false;
    layer_off(LAYER_LSYM);
  }

}
