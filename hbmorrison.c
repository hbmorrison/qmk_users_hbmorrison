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

bool process_homerow_mod(uint16_t tap, uint16_t hold, uint16_t second_hold, keyrecord_t *record);
bool process_rsft_mod(keyrecord_t *record);
bool process_record_user_windows(uint16_t keycode, keyrecord_t *record);
bool process_record_user_chromeos(uint16_t keycode, keyrecord_t *record);
bool process_record_user_linux(uint16_t keycode, keyrecord_t *record);

// Indicates whether to issue Windows, ChromeOS or Linux keypresses from macros
// with Windows selected by default.

static uint8_t selected_operating_system = OS_WINDOWS;

// True if the right hand versions of the modifier keys are currently in use.

bool rsft_held = false;
bool rctl_held = false;
bool ralt_held = false;
bool rgui_held = false;
bool rca_held = false;

// Stores the state of the shift keys when in layers higher than the base layer.

static uint8_t sym_layer_shift_mods = 0;

// True if we have just substituted shift-backspace with del.

static bool del_registered = false;

// True if we are currently tabbing between windows.

static bool alt_tab_state = false;

// Process keypresses.

bool process_record_user(uint16_t keycode, keyrecord_t *record) {

  // Get the current state that we need.

  uint8_t mod_state = get_mods();
  uint8_t highest_layer = get_highest_layer(layer_state);

  // Only allow left hand modifiers to work with the right hand side of the
  // keyboard and vice versa.

  if (highest_layer == LAYER_BASE && record->event.pressed) {

    bool right_hand_mods = rsft_held || rctl_held || ralt_held || rgui_held || rca_held;

    // Check if the current keypress is on the left hand side.

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
      case KC_Z:
      case KC_X:
      case KC_C:
      case KC_D:
      case KC_V:
      case HR_LCA:
      case HR_LGUI:
      case HR_LALT:
      case HR_LCTL:
      case LT_FUNC:

        // If a left shift oneshot modifier is active, clear it.

        if (get_oneshot_mods() & MOD_BIT(KC_LSFT))
          del_oneshot_mods(MOD_BIT(KC_LSFT));

        // If a left hand modifier is being held, remove the mods, tap the key
        // unmodded, then reinstate the mods.

        if (mod_state && ! right_hand_mods) {
          clear_mods();
          tap_code16(keycode);
          set_mods(mod_state);
          return false;
        }

    }

    // Check if the current keypress is on the right hand side.

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
      case KC_H:
      case KC_COMMA:
      case KC_DOT:
      case KC_SLSH:
      case HR_RCTL:
      case HR_RALT:
      case HR_RGUI:
      case HR_RCA:
      case LT_CTLS:

        // If a right shift oneshot modifier is active, clear it.

        if (get_oneshot_mods() & MOD_BIT(KC_RSFT))
          del_oneshot_mods(MOD_BIT(KC_RSFT));

        // If a right hand modifier is being held, remove the mods, tap the key
        // unmodded, then reinstate the mods.

        if (mod_state && right_hand_mods) {
          clear_mods();
          tap_code16(keycode);
          set_mods(mod_state);
          return false;
        }

    }

  }

  // Ensure that shift modifiers are not set when symbol layers are active. This
  // ensures that symbol keypresses will always produce the unshifted symbol,
  // unless explicitly shifted in code.

  if (highest_layer == LAYER_LSYM || highest_layer == LAYER_RSYM) {
    if (record->event.pressed) {

      sym_layer_shift_mods = get_mods() & MOD_MASK_SHIFT;
      del_mods(MOD_MASK_SHIFT);
      del_oneshot_mods(MOD_MASK_SHIFT);

    } else {

      if (sym_layer_shift_mods) {
        add_mods(sym_layer_shift_mods);
        sym_layer_shift_mods = 0;
      }

    }
  }

  // Stop pressing the KC_LALT key once M_ALT_TAB is no longer being pressed.

  if (keycode != M_ALT_TAB && alt_tab_state) {
    unregister_code(KC_LALT);
    alt_tab_state = false;
  }

  // Override each right hand homerow modifier key so that we can record when it
  // is being held. We avoid processing the homerow modifier keys if the tap
  // count is greater than one to allow QMK to handle key repeats.

  if (record->tap.count < 2) {
    switch (keycode) {

      case HR_RCTL:
        rctl_held = process_homerow_mod(HR_RCTL_KEY, KC_LCTL, 0, record);
        return false;

      case HR_RALT:
        ralt_held = process_homerow_mod(HR_RALT_KEY, KC_LALT, 0, record);
        return false;

      case HR_RGUI:
        rgui_held = process_homerow_mod(HR_RGUI_KEY, KC_LGUI, 0, record);
        return false;

      case HR_RCA:
        rca_held = process_homerow_mod(HR_RCA_KEY, KC_LCTL, KC_LALT, record);
        return false;

    }
  }

  // Handle the rest of the keypresses we are interested in.

  switch (keycode) {

    // The right shift modifier is handled separately from the homerow modifiers
    // above so that we can detect double taps for toggling caps word.

    case OSM_RSFT:
      rsft_held = process_rsft_mod(record);
      return false;

    // Shift-backspace produces delete.

    case KC_BSPC:
      if (record->event.pressed) {
        if (mod_state & MOD_BIT(KC_LSFT)) {
          del_mods(MOD_BIT(KC_LSFT));
          register_code(KC_DEL);
          add_mods(MOD_BIT(KC_LSFT));
          del_registered = true;
          return false;
        }
      } else {
        if (del_registered) {
          unregister_code(KC_DEL);
          del_registered = false;
          return false;
        }
      }
      break;

    // Hold down the ALT key persistently when tabbing through windows.

    case M_ALT_TAB:
      if (record->event.pressed) {
        if (! alt_tab_state) {
          register_code(KC_LALT);
          alt_tab_state = true;
        }
        tap_code(KC_TAB);
      }
      break;

    // Send Escape then Colon.

    case M_ESC_COLN:
      SEND_STRING(SS_TAP(X_ESC) SS_DELAY(100) ":");
      break;

    // Swap between Windows, ChromeOS and Linux shortcuts.

    case M_ISWINDOWS:
      if (record->event.pressed)
        selected_operating_system = OS_WINDOWS;
      break;

    case M_ISCHROMEOS:
      if (record->event.pressed)
        selected_operating_system = OS_CHROMEOS;
      break;

    case M_ISLINUX:
      if (record->event.pressed)
        selected_operating_system = OS_LINUX;
      break;

  }

  // Process operating system specific keycodes.

  switch (selected_operating_system) {
    case OS_WINDOWS:
      return process_record_user_windows(keycode, record);
    case OS_CHROMEOS:
      return process_record_user_chromeos(keycode, record);
    case OS_LINUX:
      return process_record_user_linux(keycode, record);
  }

  return true;

}

// Process a homerow modifier key and return true if the key is currently being
// held.

bool process_homerow_mod(uint16_t tap, uint16_t hold, uint16_t second_hold, keyrecord_t *record) {

  if (record->event.pressed) {

    if (record->tap.count) {
      if (is_caps_word_on() && ! caps_word_press_user(tap))
        caps_word_off();
      tap_code16(tap);
    } else {
      register_code(hold);
      if (second_hold)
        register_code(second_hold);
      return true;
    }

  } else {

    if (! record->tap.count) {
      if (second_hold)
        unregister_code(second_hold);
      unregister_code(hold);
    }

  }

  return false;
}

// Process the right shift modifier key and return true if the key is currently
// being held.

bool process_rsft_mod(keyrecord_t *record) {

  if (record->event.pressed) {

    register_code(KC_LSFT);
    return true;

  } else {

    unregister_code(KC_LSFT);

    if (record->tap.count == 1)
      add_oneshot_mods(MOD_BIT(KC_RSFT));

    if (record->tap.count > 1)
      caps_word_toggle();

  }

  return false;
}

// Process keypresses for Windows.

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

    // Minimise.

    case M_MINIMISE:
      if (record->event.pressed) {
        SEND_STRING(SS_DOWN(X_LGUI));
        SEND_STRING(SS_TAP(X_DOWN));
        SEND_STRING(SS_UP(X_LGUI));
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

// Process keypresses for ChromeOS.

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

    // Full screen.

    case M_FULLSCREEN:
      if (record->event.pressed) {
        SEND_STRING(SS_DOWN(X_LALT));
        SEND_STRING(SS_TAP(X_EQL));
        SEND_STRING(SS_UP(X_LALT));
      }
      break;

    // Minimise.

    case M_MINIMISE:
      if (record->event.pressed) {
        SEND_STRING(SS_DOWN(X_LALT));
        SEND_STRING(SS_TAP(X_MINS));
        SEND_STRING(SS_UP(X_LALT));
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

// Process keypresses for Linux.

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

    // Basic alpha keycodes continue caps word with shift applied.

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

    // Number keys, underscore, backspace and del continue caps word but are not
    // shifted themselves.

    case KC_1 ... KC_0:
    case KC_UNDS:
    case KC_BSPC:
    case KC_DEL:
      return true;

    // Allow tab for shell completion of variable names.

    case KC_TAB:
      return true;

    // Layer keys also continue caps word so that symbols, numbers and
    // navigation keys can be accessed.

    case LT_LSYM:
    case LT_RSYM:
    case LT_NUM:
    case LT_NAV:
      return true;

    // Finish caps word for any other keypress.

    default:
      return false;

  }
}

// Set the tapping terms for layer and tap dance keys.

uint16_t get_tapping_term(uint16_t keycode, keyrecord_t *record) {
  switch (keycode) {
    case LT_NUM:
    case LT_NAV:
    case LT_FUNC:
    case LT_CTLS:
      return TAPPING_TERM_LAYER;
    case HR_LCA:
    case HR_LALT:
    case HR_LCTL:
    case HR_RCTL:
    case HR_RALT:
    case HR_RCA:
      return TAPPING_TERM_HOMEROW;
    case HR_LGUI:
    case HR_RGUI:
      return TAPPING_TERM_HOMEROW_GUI;
    default:
      return TAPPING_TERM;
  }
}

bool get_permissive_hold(uint16_t keycode, keyrecord_t *record) {
  switch (keycode) {
    case HR_LGUI:
    case HR_LALT:
    case HR_RALT:
    case HR_RGUI:
      return false;
    default:
      return true;
  }
}

// Only the space, enter, alt and gui keys get retro tapping.

bool get_retro_tapping(uint16_t keycode, keyrecord_t *record) {
  switch (keycode) {
    case LT_NUM:
    case LT_NAV:
    case HR_LGUI:
    case HR_LALT:
    case HR_RALT:
    case HR_RGUI:
      return true;
    default:
      return false;
  }
}
