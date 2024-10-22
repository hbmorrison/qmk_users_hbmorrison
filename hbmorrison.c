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

static bool hbm_is_chromebook = false;

// State of the M_ALT_TAB macro - true if we are currently tabbing between
// windows.

static bool hbm_alt_tab_pressed = false;

// State for managing shift backspace behaviour.

static bool hbm_del_registered = false;

// Used to temporarily store the state of the mod keys during shift backspace.

static uint8_t hbm_mod_state = 0;

// Process key presses.

bool process_record_user(uint16_t keycode, keyrecord_t *record) {

  // Stop pressing the KC_LALT key once M_ALT_TAB is no longer being pressed.

  if (keycode != M_ALT_TAB && hbm_alt_tab_pressed) {
    unregister_code(KC_LALT);
    hbm_alt_tab_pressed = false;
  }

  // Ensure that shift is not pressed when symbol layers are active, aside from
  // a few exceptions. This ensures that symbol keypresses will always produce
  // the unshifted symbol, unless explicitly shifted in code.

  uint8_t current_layer = get_highest_layer(layer_state);

  if (record->event.pressed) {
    if (current_layer == LAYER_SYM) {
      del_mods(MOD_MASK_SHIFT);
      del_oneshot_mods(MOD_MASK_SHIFT);
    }
  }

  // Store current modifiers for shift-backspace action.

  hbm_mod_state = get_mods();

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

    // ESC + colon.

    case M_ESC_COLN:
      if (record->event.pressed) {
        tap_code(KC_ESC);
        SEND_STRING(SS_DELAY(100));
        tap_code16(KC_COLN);
      }
      break;

    // Switch between virtual desktops.

    case M_NDESK:
      if (record->event.pressed) {
        if (hbm_is_chromebook) {
          SEND_STRING(SS_DOWN(X_LGUI));
          SEND_STRING(SS_TAP(X_RBRC));
          SEND_STRING(SS_UP(X_LGUI));
        } else {
          SEND_STRING(SS_DOWN(X_LCTL)SS_DOWN(X_LGUI));
          SEND_STRING(SS_TAP(X_RGHT));
          SEND_STRING(SS_UP(X_LGUI)SS_UP(X_LCTL));
        }
      }
      break;

    case M_PDESK:
      if (record->event.pressed) {
        if (hbm_is_chromebook) {
          SEND_STRING(SS_DOWN(X_LGUI));
          SEND_STRING(SS_TAP(X_LBRC));
          SEND_STRING(SS_UP(X_LGUI));
        } else {
          SEND_STRING(SS_DOWN(X_LCTL)SS_DOWN(X_LGUI));
          SEND_STRING(SS_TAP(X_LEFT));
          SEND_STRING(SS_UP(X_LGUI)SS_UP(X_LCTL));
        }
      }
      break;

    case M_OVERVIEW:
      if (record->event.pressed) {
        if (hbm_is_chromebook) {
          SEND_STRING(SS_TAP(X_F5));
        } else {
          SEND_STRING(SS_DOWN(X_LGUI));
          SEND_STRING(SS_TAP(X_TAB));
          SEND_STRING(SS_UP(X_LGUI));
        }
      }
      break;

    // Open the emoji window.

    case M_EMOJI:
      if (record->event.pressed) {
        if (hbm_is_chromebook) {
          SEND_STRING(SS_DOWN(X_LSFT)SS_DOWN(X_LGUI));
          SEND_STRING(SS_TAP(X_SPC));
          SEND_STRING(SS_UP(X_LGUI)SS_UP(X_LSFT));
        } else {
          SEND_STRING(SS_DOWN(X_LGUI)SS_TAP(X_SCLN)SS_UP(X_LGUI));
        }
      }
      break;

    // Swap between Windows and ChromeOS macro keypresses.

    case M_ISCROS:
      if (record->event.pressed) {
        hbm_is_chromebook = true;
      }
      break;

    case M_ISWIN:
      if (record->event.pressed) {
        hbm_is_chromebook = false;
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

    case KC_OSL_SYM:
    case KC_ENT_NUM:
    case KC_SPC_NAV:
      return true;

    // Deactivate caps word by default.

    default:
      return false;
  }
}

// Set the tapping terms for layer keys.

uint16_t get_tapping_term(uint16_t keycode, keyrecord_t *record) {
  switch (keycode) {
    case KC_OSL_SYM:
    case KC_ENT_NUM:
    case KC_SPC_NAV:
    case KC_F_FUNC:
    case KC_U_CTRL:
      return TAPPING_TERM_LAYER;
    default:
      return TAPPING_TERM;
  }
}
