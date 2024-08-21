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
    if (current_layer == LAYER_LSYM || current_layer == LAYER_RSYM) {
      del_mods(MOD_MASK_SHIFT);
      del_oneshot_mods(MOD_MASK_SHIFT);
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
      add_weak_mods(MOD_BIT(KC_LSFT));
      return true;

    // Keycodes that continue caps word without shifting.

    case KC_1 ... KC_0:
    case KC_BSPC:
    case KC_DEL:
    case KC_UNDS:
      return true;

    // Do not deactivate if symbol, num or nav layer keys are held down.

    case KC_RSYM:
    case KC_LSYM:
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
    case KC_LCAPS:
    case KC_RCAPS:
    case KC_LSYM:
    case KC_RSYM:
    case KC_ENT_NUM:
    case KC_SPC_NAV:
    case KC_F_FUNC:
    case KC_U_CTRL:
      return TAPPING_TERM_LAYER;
    default:
      return TAPPING_TERM;
  }
}
