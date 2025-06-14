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

// Indicates whether to issue Windows, ChromeOS or Linux keypresses from macros
// with Windows selected by default.

static uint8_t selected_operating_system = OS_WINDOWS;

// Operating system specific keycode processing functions.

bool process_record_user_windows(uint16_t keycode, keyrecord_t *record);
bool process_record_user_chromeos(uint16_t keycode, keyrecord_t *record);
bool process_record_user_linux(uint16_t keycode, keyrecord_t *record);

// True if the right-hand versions of the modifier keys are currently in use.

bool hr_rsft_active = false;
bool hr_rctl_active = false;
bool hr_ralt_active = false;
bool hr_rgui_active = false;
bool hr_rca_active = false;

// Process a homerow mod key manually and return true if the key is being
// held.

bool process_homerow_mod(uint16_t tap, uint16_t hold, uint16_t second_hold, keyrecord_t *record);

// True if we are currently tabbing between windows.

static bool alt_tab_state = false;

// True if there has been a keypress in the left or right sym layers.

bool rsym_key_pressed = false;
bool lsym_key_pressed = false;

// Tap dances for the multi-function shift keys. Tap dance actions are
// decided early using the _press and _release functions rather than waiting for
// tap dance data to accumulate for the _finished and _reset functions to make
// decisions.
//
// This is because the homerow modifier keys - and all MT macros - will not work
// alongside the usual tap dance mechanics. Any MT keypress during a tap dance
// will finish the tap dance and set the tap dance state to interrupted, but
// only after the MT keypresses have been registered, too late for any _finished
// code to affect them. Using the _press and _release tap dance functions allow
// shift modifiers or caps word to be applied before any MT macros register
// keypresses.

void lsft_press(tap_dance_state_t *state, void *user_data);
void lsft_release(tap_dance_state_t *state, void *user_data);
void rsft_press(tap_dance_state_t *state, void *user_data);
void rsft_release(tap_dance_state_t *state, void *user_data);

tap_dance_action_t tap_dance_actions[] = {
  [TD_LSFT] = ACTION_TAP_DANCE_FN_ADVANCED_WITH_RELEASE(lsft_press, lsft_release, NULL, NULL),
  [TD_RSFT] = ACTION_TAP_DANCE_FN_ADVANCED_WITH_RELEASE(rsft_press, rsft_release, NULL, NULL),
};

// Process key presses.

bool process_record_user(uint16_t keycode, keyrecord_t *record) {

  uint8_t current_highest_layer = get_highest_layer(layer_state);

  // Only allow left-hand modifiers to work with the right-hand side of the
  // keyboard and vice versa.

  if (current_highest_layer == LAYER_BASE && record->event.pressed) {

    // Check if any mods are active and if any or the right-hand mod keys have
    // been held or tapped.

    uint8_t any_mods = get_mods() | get_oneshot_mods();
    bool right_hand_mods = hr_rctl_active || hr_ralt_active || hr_rgui_active || hr_rca_active || hr_rsft_active;

    // If a right-hand key has been tapped while a right-hand modifier is
    // active, clear the mods in question, tap the key unmodded, then reinstate
    // the cleared mods.

    if (any_mods && right_hand_mods) {
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
        case KC_HR_RCTL:
        case KC_HR_RALT:
        case KC_HR_RGUI:
        case KC_HR_RCA:
          uint8_t mod_state = get_mods();
          clear_mods();
          clear_oneshot_mods();
          // If it was active, the right side oneshot shift has just been cleared.
          hr_rsft_active = false;
          tap_code16(keycode);
          set_mods(mod_state);
          return false;
      }
    }

    // If a left-hand key has been tapped while a left-hand modifier is active,
    // clear the mods in question, tap the key unmodded, then reinstate the
    // cleared mods.

    if (any_mods && ! right_hand_mods) {
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
        case KC_HR_LCTL:
        case KC_HR_LALT:
        case KC_HR_LGUI:
        case KC_HR_LCA:
          uint8_t mod_state = get_mods();
          clear_mods();
          clear_oneshot_mods();
          tap_code16(keycode);
          set_mods(mod_state);
          return false;
      }
    }

  }

  // Record that a sym layer key has been pressed. This information is used by
  // the shift tap dances to decide whether or not to set a oneshot shift
  // modifier.

  if (record->event.pressed) {
    if (! lsym_key_pressed && current_highest_layer == LAYER_LSYM)
      lsym_key_pressed = true;
    if (! rsym_key_pressed && current_highest_layer == LAYER_RSYM)
      rsym_key_pressed = true;
  }

  // Stop pressing the KC_LALT key once M_ALT_TAB is no longer being pressed.

  if (keycode != M_ALT_TAB && alt_tab_state) {
    unregister_code(KC_LALT);
    alt_tab_state = false;
  }

  // Process macros and special handling for keycodes, but let QMK handle any
  // key repeats.

  if (record->tap.count < 2) {

    switch (keycode) {

      // Override the right-hand homerow mod keys to use left-hand modifiers but
      // record that the right-hand mod key is being held, so that handedness
      // can be enforced.

      case KC_HR_RCTL:
        hr_rctl_active = process_homerow_mod(KC_RCTL_KEY, KC_LCTL, 0, record);
        return false;

      case KC_HR_RALT:
        hr_ralt_active = process_homerow_mod(KC_RALT_KEY, KC_LALT, 0, record);
        return false;

      case KC_HR_RGUI:
        hr_rgui_active = process_homerow_mod(KC_RGUI_KEY, KC_LGUI, 0, record);
        return false;

      case KC_HR_RCA:
        hr_rca_active = process_homerow_mod(KC_RCA_KEY, KC_LCTL, KC_LALT, record);
        return false;

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

  }

  return true;
}

// Process a homerow mod key manually and return true if the key is being
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

    // Layer keys also continue caps word so that symbols, numbers and
    // navigation keys can be accessed.

    case KC_TD_LSFT:
    case KC_TD_RSFT:
    case KC_NUM:
    case KC_NAV:
      return true;

    // Finish caps word for any other keypress.

    default:
      return false;

  }
}

// Set the tapping terms for layer and tap dance keys.

uint16_t get_tapping_term(uint16_t keycode, keyrecord_t *record) {
  switch (keycode) {
    case KC_NUM:
    case KC_NAV:
    case KC_FUNC:
    case KC_CTLS:
      return TAPPING_TERM_LAYER;
    case KC_HR_LGUI:
    case KC_HR_LALT:
    case KC_HR_LCTL:
    case KC_HR_LCA:
    case KC_HR_RGUI:
    case KC_HR_RALT:
    case KC_HR_RCTL:
    case KC_HR_RCA:
      return TAPPING_TERM_HOMEROW;
    case KC_TD_LSFT:
    case KC_TD_RSFT:
      return TAPPING_TERM_TAP_DANCE_SHIFT;
    default:
      return TAPPING_TERM;
  }
}

// Only the space and enter keys get retro tapping.

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

void lsft_press(tap_dance_state_t *state, void *user_data) {

  // Assume a hold if the tap dance key has been pressed for the first time.

  if (state->count == 1) {
    rsym_key_pressed = false;
    layer_on(LAYER_RSYM);
  }

  // If there is a second tap dance key press then clear the unused oneshot
  // shift mod that was added on the first key release.

  if (state->count > 1)
    del_oneshot_mods(MOD_BIT(KC_LSFT));

}

void lsft_release(tap_dance_state_t *state, void *user_data) {

  if (state->count == 1) {

    // When the tap dance key has been released after its first press then we are
    // finished with the sym layer.

    layer_off(LAYER_RSYM);

    // If a symbol key was not pressed while the sym layer was on then the tap
    // dance must be a tap, so add a oneshot shift mod.

    if (! rsym_key_pressed)
      add_oneshot_mods(MOD_BIT(KC_LSFT));

  }

  // When the tap dance key is released for a second time without interruption
  // then toggle caps word.

  if (state->count > 1)
    caps_word_toggle();

}

// Tap dance on right-shift.

void rsft_press(tap_dance_state_t *state, void *user_data) {

  // This will be set to true if the right shift tap dance finishes with a
  // single tap so reset it here.

  hr_rsft_active = false;

  if (state->count == 1) {
    lsym_key_pressed = false;
    layer_on(LAYER_LSYM);
  }

  if (state->count > 1)
    del_oneshot_mods(MOD_BIT(KC_LSFT));

}

void rsft_release(tap_dance_state_t *state, void *user_data) {

  if (state->count == 1) {

    layer_off(LAYER_LSYM);

    // Record that right shift is now active.

    if (! lsym_key_pressed) {
      hr_rsft_active = true;
      add_oneshot_mods(MOD_BIT(KC_LSFT));
    }

  }

  if (state->count > 1)
    caps_word_toggle();

}
