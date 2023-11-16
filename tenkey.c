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

#include "stdio.h"
#include "tenkey.h"

// State of the M_ALT_TAB macro - true if we are currently tabbing between
// windows.

static bool tenkey_alt_tab_pressed = false;

bool process_record_user(uint16_t keycode, keyrecord_t *record) {

  // Stop pressing the KC_LALT key once M_ALT_TAB is no longer being pressed.

  if (keycode != M_ALT_TAB && tenkey_alt_tab_pressed) {
    unregister_code(KC_LALT);
    tenkey_alt_tab_pressed = false;
  }

  // Ensure that shift is not pressed when additional layers are active, aside
  // from a few exceptions. This ensures that symbol keypresses will always
  // produce the unshifted symbol, unless explicitly shifted with LSFT() in
  // code.

  uint8_t current_layer = get_highest_layer(layer_state);

  if (record->event.pressed && current_layer > LAYER_BASE) {
    switch (keycode) {
      // Allow tabs to be shifted.
      case KC_TAB:
        break;
      default:
        del_mods(MOD_MASK_SHIFT);
        del_oneshot_mods(MOD_MASK_SHIFT);
    }
  }

  switch (keycode) {

    // Hold down KC_LALT persistantly to allow tabbing through windows.

    case M_ALT_TAB:
      if (record->event.pressed) {
        if (!tenkey_alt_tab_pressed) {
          register_code(KC_LALT);
          tenkey_alt_tab_pressed = true;
        }
        tap_code(KC_TAB);
      }
      break;

  }

  return true;
}

uint16_t get_tapping_term(uint16_t keycode, keyrecord_t *record) {
  switch (keycode) {
    // Set the tapping term for the homerow mods.
    case KC_W_ALT:
    case KC_R_CTL:
    case KC_T_SFT:
    case KC_P_GUI:
    case KC_L_GUI:
    case KC_N_SFT:
    case KC_I_CTL:
    case KC_Y_ALT:
      return TAPPING_TERM_MODS;
    // Set the tapping term for layer keys.
    case KC_ENT_NUM_LAYER:
    case KC_SPC_NAV_LAYER:
#ifdef TENKEY_FILL_LAYER_ENABLE
    case KC_S_FILL_LEFT:
    case KC_E_FILL_RIGHT:
#endif
      return TAPPING_TERM_LAYER;
    default:
      return TAPPING_TERM;
  }
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
    // Do not deactivate if the layer keys are held down.
#ifdef TENKEY_FILL_LAYER_ENABLE
    case KC_S_FILL_LEFT:
    case KC_E_FILL_RIGHT:
#endif
#ifdef TENKEY_PINKY_ENABLE
    case KC_A_SYM_LEFT:
    case KC_O_SYM_RIGHT:
#else
    case KC_X_SYM_LEFT:
    case KC_DOT_SYM_RIGHT:
#endif
      return true;
    // Deactivate caps word by default.
    default:
      return false;
  }
}

// Reduce the size of the compiled firmware.

uint16_t keycode_config(uint16_t keycode) {
    return keycode;
}

uint8_t mod_config(uint8_t mod) {
    return mod;
}
