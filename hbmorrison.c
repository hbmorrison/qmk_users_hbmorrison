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

// True if left- or right-shift tap dance keys have recently been single tapped.

bool td_lsft_single_tapped = false;
bool td_rsft_single_tapped = false;

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

  // Complete the oneshot action for the left- and right-shift keys.

  if (td_lsft_single_tapped && record->event.pressed && keycode != TD(TD_LSFT))
    clear_oneshot_layer_state(ONESHOT_PRESSED);

  if (td_rsft_single_tapped && record->event.pressed && keycode != TD(TD_RSFT))
    clear_oneshot_layer_state(ONESHOT_PRESSED);

  // Only allow left-hand modifiers to work with the right side of the keyboard
  // and vice versa. The goal here is to produce consistent, sensible behaviour.
  // Rather than stall any active mods when a "bad" keypress occurs, the mods are
  // removed for an unmodded same-side keypress then swapped back in afterwards,
  // where they will affect future keypresses as expected.

  uint8_t current_layer = get_highest_layer(layer_state);

  if (record->event.pressed && current_layer == LAYER_BASE) {

    uint16_t simple_keycode = keycode;

    // Convert any layer or mod keypresses into their alternative simple
    // keypresses.

    switch (keycode) {
      case KC_HR_LGUI:
        simple_keycode = KC_LGUI_ALT;
        break;
      case KC_HR_LALT:
        simple_keycode = KC_LALT_ALT;
        break;
      case KC_HR_LCTL:
        simple_keycode = KC_LCTL_ALT;
        break;
      case KC_HR_LCA:
        simple_keycode = KC_LCA_ALT;
        break;
      case KC_HR_RGUI:
        simple_keycode = KC_RGUI_ALT;
        break;
      case KC_HR_RALT:
        simple_keycode = KC_RALT_ALT;
        break;
      case KC_HR_RCTL:
        simple_keycode = KC_RCTL_ALT;
        break;
      case KC_HR_RCA:
        simple_keycode = KC_RCA_ALT;
        break;
    }

    // Check if any left-hand mods are present.

    uint8_t left_mods = get_mods() & MOD_BITS_LEFT;
    uint8_t left_oneshot_mods = get_oneshot_mods() & MOD_BITS_LEFT;

    if (left_mods || left_oneshot_mods) {

      // If there are, check whether this keypress is on the left-hand side of
      // the keyboard.

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

          // Remove all of the left-hand mods and tap the keypress.

          if (left_mods) { del_mods(left_mods); }
          if (left_oneshot_mods) { del_oneshot_mods(left_oneshot_mods); }

          tap_code(simple_keycode);

          // Add the non-oneshot mods back because they continue to be valid
          // while the mod key is being held down.

          if (left_mods) { add_mods(left_mods); }

          return false;
      }

    }

    // Check if any right-hand mods are present.

    uint8_t right_mods = get_mods() & MOD_BITS_RIGHT;
    uint8_t right_oneshot_mods = get_oneshot_mods() & MOD_BITS_RIGHT;

    if (right_mods || right_oneshot_mods) {

      // If there are, check whether this keypress is on the right-hand side of
      // the keyboard.

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

          // Remove all of the right-hand mods and tap the keypress.

          if (right_mods) { del_mods(right_mods); }
          if (right_oneshot_mods) { del_oneshot_mods(right_oneshot_mods); }

          tap_code(simple_keycode);

          // Add the non-oneshot mods back because they continue to be valid
          // while the mod key is being held down.

          if (right_mods) { add_mods(right_mods); }

          return false;
      }

    }
  }

  // Store current modifiers for shift-backspace action.

  hbm_mod_state = get_mods();

  // Process generic keycodes.

  switch (keycode) {

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
  switch (keycode) {

    // Keycodes that continue caps word with shift applied.

    case KC_A ... KC_Z:
      add_weak_mods(MOD_BIT(KC_LSFT));
      return true;

    // Keycodes that continue caps word without shifting.

    case KC_1 ... KC_0:
    case KC_BSPC:
    case KC_DEL:
    case KC_UNDS:
      return true;

    // Do not deactivate if symbol, num or nav layer keys are held down.

    case KC_NUM:
    case KC_NAV:
#ifdef HBM_HANDED
    case TD(TD_LSFT):
    case TD(TD_RSFT):
#else
    case KC_R_THUMB: // This will be the LSYM key in non-handed mode.
#endif
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
    case TD(TD_LSFT):
    case TD(TD_RSFT):
      return TAPPING_TERM_TAP_DANCE;
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

  // If the count is currently one then we could be heading towards a tap or a
  // hold. Default to hold and set the left-shift modifier. If it turns out to
  // be a tap after all, the modifier can be removed below.

  if (state->count == 1) {
    td_lsft_held = true;
    add_mods(MOD_BIT(KC_LSFT));
  }

}

void lsft_finished(tap_dance_state_t *state, void *user_data) {

  // If another key has been pressed during TAPPING_TERM_TAP_DANCE and the left
  // shift key is no longer pressed, remove the shift modifier and check for
  // taps.

  if (state->interrupted && !state->pressed) {

    td_lsft_held = false;
    del_mods(MOD_BIT(KC_LSFT));

    if (state->count == 1) {
      td_lsft_single_tapped = true;
      set_oneshot_layer(LAYER_RSYM, ONESHOT_START);
      return;
    }

    if (state->count == 2) {
      td_lsft_single_tapped = false;
      caps_word_toggle();
    }
  }

}

void lsft_reset(tap_dance_state_t *state, void *user_data) {

  // At the end of the tap dance, remove the left-shift modifier if the
  // left-shift key has been held down.

  if (td_lsft_held) {
    td_lsft_held = false;
    del_mods(MOD_BIT(KC_LSFT));
  }

}

// Tap dance on right-shift.

void rsft_tap(tap_dance_state_t *state, void *user_data) {

  // If the count is currently one then we could be heading towards a tap or a
  // hold. Default to hold and set the right-shift modifier. If it turns out to
  // be a tap after all, the modifier can be removed below.

  if (state->count == 1) {
    td_rsft_held = true;
    add_mods(MOD_BIT(KC_RSFT));
  }

}

void rsft_finished(tap_dance_state_t *state, void *user_data) {

  // If another key has been pressed during TAPPING_TERM_TAP_DANCE and the right
  // shift key is no longer pressed, remove the shift modifier and check for
  // taps.

  if (state->interrupted && !state->pressed) {

    td_rsft_held = false;
    del_mods(MOD_BIT(KC_RSFT));

    if (state->count == 1) {
      td_rsft_single_tapped = true;
      set_oneshot_layer(LAYER_LSYM, ONESHOT_START);
      return;
    }

    if (state->count == 2) {
      td_rsft_single_tapped = false;
      caps_word_toggle();
    }
  }

}

void rsft_reset(tap_dance_state_t *state, void *user_data) {

  // At the end of the tap dance, remove the right-shift modifier if the
  // right-shift key has been held down.

  if (td_rsft_held) {
    td_rsft_held = false;
    del_mods(MOD_BIT(KC_RSFT));
  }

}
