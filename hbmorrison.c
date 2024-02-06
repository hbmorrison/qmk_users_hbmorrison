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

// Used to temporarily store the state of the mod keys.
static uint8_t hbm_mod_state = 0;

// State for managing shift backspace behaviour.
static bool hbm_del_registered = false;

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
        // Allow keys in the extended layers to be shifted.
        case KC_A ... KC_Z:
        case KC_DOT:
        case KC_BSPC:
        // Allow tabs to be shifted.
        case KC_TAB:
          break;
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

  // Store current modifiers for shift-backspace action.
  hbm_mod_state = get_mods();

  switch (keycode) {

    // Shift-backspace produces delete.

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

    //Switch between virtual desktops.

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

    // Close current tab.

    case M_TCLOSE:
      if (record->event.pressed) {
        SEND_STRING(SS_DOWN(X_LCTL)SS_TAP(X_F4)SS_UP(X_LCTL));
      }
      break;

    // Minimise and restore window.

    case M_WMIN:
      if (record->event.pressed) {
        SEND_STRING(SS_DOWN(X_LSFT)SS_DOWN(X_LCTL)SS_DOWN(X_LALT));
        SEND_STRING(SS_TAP(X_M));
        SEND_STRING(SS_UP(X_LALT)SS_UP(X_LCTL)SS_UP(X_LSFT));
      }
      break;

    // Close window.

    case M_WKILL:
      if (record->event.pressed) {
        SEND_STRING(SS_DOWN(X_LSFT)SS_DOWN(X_LCTL)SS_DOWN(X_LALT));
        SEND_STRING(SS_TAP(X_K));
        SEND_STRING(SS_UP(X_LALT)SS_UP(X_LCTL)SS_UP(X_LSFT));
      }
      break;

    // Centre window and minimise all others.

    case M_WFOCUS:
      if (record->event.pressed) {
        SEND_STRING(SS_DOWN(X_LSFT)SS_DOWN(X_LCTL)SS_DOWN(X_LALT));
        SEND_STRING(SS_TAP(X_F));
        SEND_STRING(SS_UP(X_LALT)SS_UP(X_LCTL)SS_UP(X_LSFT));
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
    case KC_SYM:
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
    case KC_T_SFT:
    case KC_N_SFT:
    case KC_R_CTL:
    case KC_I_CTL:
    case KC_X_ALT:
    case KC_DOT_ALT:
    case KC_D_GUI:
    case KC_H_GUI:
    case KC_S_CS:
    case KC_E_CS:
    case KC_A_MEH:
    case KC_O_MEH:
      return TAPPING_TERM_MODS;
    // Set the tapping term for layer keys.
    case KC_SYM:
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
    case KC_D_GUI:
    case KC_H_GUI:
      return false;
    default:
      return true;
  }
}
