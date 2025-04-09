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
bool process_record_user_linux(uint16_t keycode, keyrecord_t *record);

// State of the M_ALT_TAB macro - true if we are currently tabbing between
// windows.

static bool hbm_alt_tab_pressed = false;

// State for managing shift backspace behaviour.

static bool hbm_del_registered = false;

// Used to temporarily store the state of the mod keys during shift backspace.

static uint8_t hbm_mod_state = 0;

// State of the shift modifiers - used to turn shift off for symbol layers.

static bool hbm_shift_pressed = false;
static bool hbm_os_shift_pressed = false;

// Process key presses.

bool process_record_user(uint16_t keycode, keyrecord_t *record) {

  // Stop pressing the KC_LALT key once M_ALT_TAB is no longer being pressed.

  if (keycode != M_ALT_TAB && hbm_alt_tab_pressed) {
    unregister_code(KC_LALT);
    hbm_alt_tab_pressed = false;
  }

  // Ensure that shift is not pressed when symbol layers are active. This
  // ensures that symbol keypresses will always produce the unshifted symbol,
  // unless explicitly shifted in code.

  uint8_t current_layer = get_highest_layer(layer_state);

  if (record->event.pressed) {
    if (current_layer == LAYER_LSYM || current_layer == LAYER_RSYM) {
      hbm_shift_pressed = get_mods() & MOD_BIT(KC_LSFT);
      hbm_os_shift_pressed = get_oneshot_mods() & MOD_BIT(KC_LSFT);
      del_mods(MOD_MASK_SHIFT);
      del_oneshot_mods(MOD_MASK_SHIFT);
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

  // Only allow left-hand modifiers to work with the right side of the keyboard
  // and vice versa.

  if (record->event.pressed && current_layer == LAYER_BASE) {
    if (get_mods() & MOD_BITS_LEFT || get_oneshot_mods() & MOD_BITS_LEFT) {
      switch (keycode) {
        case KC_Q:
        case KC_W:
        case KC_F_FUNC:
        case KC_P:
        case KC_B:
        case KC_R:
        case KC_S:
        case KC_T:
        case KC_G:
        case KC_A_RSYM:
        case KC_Z:
        case KC_X_GUI:
        case KC_C_ALT:
        case KC_D_CTL:
        case KC_V_CS:
          del_oneshot_mods(MOD_BITS_LEFT);
          return false;
      }
    }

    if (get_mods() & MOD_BITS_RIGHT || get_oneshot_mods() & MOD_BITS_RIGHT) {
      switch (keycode) {
        case KC_J:
        case KC_L:
        case KC_U_CTRL:
        case KC_Y:
        case KC_BSPC:
        case KC_M:
        case KC_N:
        case KC_E:
        case KC_I:
        case KC_O_LSYM:
        case KC_K_CS:
        case KC_H_CTL:
        case KC_COMMA_ALT:
        case KC_DOT_GUI:
        case KC_Z:
          del_oneshot_mods(MOD_BITS_RIGHT);
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

    // Esc then colon.

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

    // Equals + greater-than.

    case M_EQL_GT:
      if (record->event.pressed) {
        tap_code(KC_EQL);
        tap_code16(KC_GT);
      }
      break;

    // Swap between Windows and Linux shortcuts.

    case M_ISWINDOWS:
      if (record->event.pressed) {
        hbm_operatingsystem = OS_WINDOWS;
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

    case KC_ENT_NUM:
    case KC_SPC_NAV:
    case KC_A_RSYM:
    case KC_O_LSYM:
      return true;

    // Deactivate caps word by default.

    default:
      return false;
  }
}

// Set the tapping terms for layer keys.

uint16_t get_tapping_term(uint16_t keycode, keyrecord_t *record) {
  switch (keycode) {
    case KC_ENT_NUM:
    case KC_SPC_NAV:
    case KC_A_RSYM:
    case KC_O_LSYM:
    case KC_F_FUNC:
    case KC_U_CTRL:
      return TAPPING_TERM_LAYER;
    default:
      return TAPPING_TERM;
  }
}
