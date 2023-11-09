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
#include "hbmorrison.h"

#ifdef TAP_DANCE_ENABLE
typedef enum {
  TD_NONE,
  TD_UNKNOWN,
  TD_SINGLE_TAP,
  TD_DOUBLE_TAP,
  TD_POST_TAP,
  TD_HOLD,
  TD_HOLD_KEYPRESS
} td_state_t;

td_state_t td_get_state(tap_dance_state_t *state);

static td_state_t td_ent_state = TD_NONE;
static td_state_t td_spc_state = TD_NONE;
static td_state_t td_slsh_state = TD_NONE;
static td_state_t td_z_state = TD_NONE;

void td_ent_finished(tap_dance_state_t *state, void *user_data);
void td_ent_reset(tap_dance_state_t *state, void *user_data);
void td_spc_finished(tap_dance_state_t *state, void *user_data);
void td_spc_reset(tap_dance_state_t *state, void *user_data);
void td_slsh_finished(tap_dance_state_t *state, void *user_data);
void td_slsh_reset(tap_dance_state_t *state, void *user_data);
void td_z_finished(tap_dance_state_t *state, void *user_data);
void td_z_reset(tap_dance_state_t *state, void *user_data);

tap_dance_action_t tap_dance_actions[] = {
  [TD_ENT_LAYER] = ACTION_TAP_DANCE_FN_ADVANCED(NULL, td_ent_finished, td_ent_reset),
  [TD_SPC_LAYER] = ACTION_TAP_DANCE_FN_ADVANCED(NULL, td_spc_finished, td_spc_reset),
  [TD_SLSH_LAYER] = ACTION_TAP_DANCE_FN_ADVANCED(NULL, td_slsh_finished, td_slsh_reset),
  [TD_Z_LAYER] = ACTION_TAP_DANCE_FN_ADVANCED(NULL, td_z_finished, td_z_reset),
};
#endif // TAP_DANCE_ENABLE

// Issues HYPR-keypress for Windows and ALT-keypress for ChromeOS.

void hypr_or_alt(char *ss, bool pressed);

// Defines whether to issue Windows or ChromeOS keypresses from macros - Windows
// by default.

static bool hbm_is_chromebook = false;

// State of the M_ALT_TAB macro - true if we are currently tabbing between
// windows.

static bool hbm_alt_tab_pressed = false;

// State for managing shift-backspace behaviour.

static bool hbm_del_registered = false;
static uint8_t hbm_mod_state = 0;

// True if rand() has already been seeded using srand().

static bool hbm_srand_seeded = false;

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

  if (record->event.pressed && current_layer > LAYER_BASE) {
    switch (keycode) {
      // Allow tabs to be shifted.
      case KC_TAB:
        break;
      // Allow Z and / to be shifted if sidekeys are on.
#ifdef HBM_SIDEKEY_ENABLE
      case KC_Z:
      case KC_SLSH:
        break;
#endif // HBM_SIDEKEY_ENABLE
      default:
        del_mods(MOD_MASK_SHIFT);
        del_oneshot_mods(MOD_MASK_SHIFT);
    }
  }

  // Store the current state of modifiers for shift-backspace behaviour.

  hbm_mod_state = get_mods();

  switch (keycode) {

    // Shift-backspace issues delete.

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

    // Override the mod-tap functionality for the mod-keypress shortcuts in the
    // navigation layer so that the modified keypresses can be sent correctly as
    // 16-bit taps. By default the MOD_T() macros can only send 8-bit keypresses
    // on tap, so macros like LALT_T(LCTL(KC_C)) will not work. Each one has to
    // be overriden as below to issue the correct 16-bit tap.

    case KC_CTL_X_SFTALT:
      if (record->tap.count && record->event.pressed) {
          tap_code16(LCTL(KC_X)); // Send Ctrl-X on tap
          return false;
      }
      break;
    case KC_CTL_C_ALT:
      if (record->tap.count && record->event.pressed) {
          tap_code16(LCTL(KC_C)); // Send Ctrl-C on tap
          return false;
      }
      break;
    case KC_SFT_CTL_C_CTL:
      if (record->tap.count && record->event.pressed) {
          tap_code16(LSFT(LCTL(KC_C))); // Send Shift-Ctrl-C on tap
          return false;
      }
      break;
    case KC_CTL_V_SFTCTL:
      if (record->tap.count && record->event.pressed) {
          tap_code16(LCTL(KC_V)); // Send Ctrl-V on tap
          return false;
      }
      break;

    // Application switching macros.

    case M_APP1:
      hypr_or_alt(SS_TAP(X_1), record->event.pressed);
      break;
    case M_APP2:
      hypr_or_alt(SS_TAP(X_2), record->event.pressed);
      break;
    case M_APP3:
      hypr_or_alt(SS_TAP(X_3), record->event.pressed);
      break;
    case M_APP4:
      hypr_or_alt(SS_TAP(X_4), record->event.pressed);
      break;
    case M_APP5:
      hypr_or_alt(SS_TAP(X_5), record->event.pressed);
      break;
    case M_APP6:
      hypr_or_alt(SS_TAP(X_6), record->event.pressed);
      break;
    case M_APP7:
      hypr_or_alt(SS_TAP(X_7), record->event.pressed);
      break;
    case M_APP8:
      hypr_or_alt(SS_TAP(X_8), record->event.pressed);
      break;

    // Launch 1Password - in ChromeOS, switch to the browser before issuing
    // shortcut keypress.

    case M_1PASS:
      if (record->event.pressed) {
        if (hbm_is_chromebook) {
          SEND_STRING(SS_DOWN(X_LALT)SS_TAP(X_1)SS_UP(X_LALT));
          SEND_STRING(SS_DELAY(100));
          SEND_STRING(SS_DOWN(X_LSFT)SS_DOWN(X_LCTL));
          SEND_STRING(SS_TAP(X_X));
          SEND_STRING(SS_UP(X_LCTL)SS_UP(X_LSFT));
        } else {
          SEND_STRING(SS_DOWN(X_LSFT)SS_DOWN(X_LCTL));
          SEND_STRING(SS_TAP(X_SPC));
          SEND_STRING(SS_UP(X_LCTL)SS_UP(X_LSFT));
        }
      }
      break;

    // Window operations.

    case M_HIDE:
      if (record->event.pressed) {
        if (hbm_is_chromebook) {
          SEND_STRING(SS_DOWN(X_LALT));
          SEND_STRING(SS_TAP(X_MINS));
          SEND_STRING(SS_UP(X_LALT));
        } else {
          SEND_STRING(SS_DOWN(X_LSFT)SS_DOWN(X_LCTL)SS_DOWN(X_LALT)SS_DOWN(X_LGUI));
          SEND_STRING(SS_TAP(X_M));
          SEND_STRING(SS_UP(X_LGUI)SS_UP(X_LALT)SS_UP(X_LCTL)SS_UP(X_LSFT));
        }
      }
      break;
    case M_MAX:
      if (record->event.pressed) {
        if (hbm_is_chromebook) {
          SEND_STRING(SS_DOWN(X_LALT));
          SEND_STRING(SS_TAP(X_EQL));
          SEND_STRING(SS_UP(X_LALT));
        } else {
          SEND_STRING(SS_DOWN(X_LSFT)SS_DOWN(X_LCTL)SS_DOWN(X_LALT)SS_DOWN(X_LGUI));
          SEND_STRING(SS_TAP(X_A));
          SEND_STRING(SS_UP(X_LGUI)SS_UP(X_LALT)SS_UP(X_LCTL)SS_UP(X_LSFT));
        }
      }
      break;
    case M_CLOSE:
      if (record->event.pressed) {
        if (hbm_is_chromebook) {
          SEND_STRING(SS_DOWN(X_LCTL)SS_DOWN(X_LSFT));
          SEND_STRING(SS_TAP(X_W));
          SEND_STRING(SS_UP(X_LSFT)SS_UP(X_LCTL));
        } else {
          SEND_STRING(SS_DOWN(X_LSFT)SS_DOWN(X_LCTL)SS_DOWN(X_LALT)SS_DOWN(X_LGUI));
          SEND_STRING(SS_TAP(X_K));
          SEND_STRING(SS_UP(X_LGUI)SS_UP(X_LALT)SS_UP(X_LCTL)SS_UP(X_LSFT));
        }
      }
      break;

    // Open a new terminal window.

    case M_NTRM:
      if (record->event.pressed) {
        if (hbm_is_chromebook) {
          SEND_STRING(SS_DOWN(X_LCTL)SS_TAP(X_T)SS_UP(X_LCTL));
        } else {
          SEND_STRING(SS_DOWN(X_LSFT)SS_DOWN(X_LCTL));
          SEND_STRING(SS_TAP(X_1));
          SEND_STRING(SS_UP(X_LCTL)SS_UP(X_LSFT));
        }
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

    // Send dot dot slash.

    case M_DDS:
      if (record->event.pressed) {
        SEND_STRING("../");
      }
      break;

    // Sends Esc : for Vim.

    case M_ESC_COLN:
      if (record->event.pressed) {
        SEND_STRING(SS_TAP(X_ESC));
        SEND_STRING(SS_DOWN(X_LSFT)SS_TAP(X_SCLN)SS_UP(X_LSFT));
      }
      break;

    // Types out a four-digit pseudo-random number.

    case M_4RAND:
      if (record->event.pressed) {
        if (hbm_srand_seeded == false) {
#if defined(__AVR_ATmega32U4__)
          srand(TCNT0 + TCNT1 + TCNT3 + TCNT4);
#else
          srand(timer_read32());
#endif
          hbm_srand_seeded = true;
        }
        char rand_string[5];
        sprintf(rand_string, "%d", rand() % 10000 + 1000);
        SEND_STRING(rand_string);
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

#ifdef TAP_DANCE_ENABLE
void post_process_record_user(uint16_t keycode, keyrecord_t *record) {
  if (!record->event.pressed) {

    // Turn off tap layers after another keypress is registered. This emulates
    // the behaviour of oneshot layers.

    if (td_slsh_state == TD_POST_TAP && IS_LAYER_ON(LAYER_SLSH_TAP)) {
      layer_off(LAYER_SLSH_TAP);
      td_slsh_state = TD_NONE;
    }
    if (td_z_state == TD_POST_TAP && IS_LAYER_ON(LAYER_Z_TAP)) {
      layer_off(LAYER_Z_TAP);
      td_z_state = TD_NONE;
    }

    // If HOLD_KEYPRESS_ENABLE is enabled, holding and releasing a layer key
    // without pressing another key will send the original layer key keypress,
    // as if the hold tap dance behaviour was not in place. The following checks
    // are reached when another keypress has been detected, and switches the
    // appropriate tap dance state away from TD_HOLD, so that the tap dance code
    // knows what has happened and does not issue the original keypress when the
    // hold is released.

#ifdef HBM_TD_HOLD_ORIGINAL_KEYPRESS_ENABLE
    if (td_ent_state == TD_HOLD && IS_LAYER_ON(LAYER_ENT_HOLD)) {
      td_ent_state = TD_HOLD_KEYPRESS;
    }
    if (td_spc_state == TD_HOLD && IS_LAYER_ON(LAYER_SPC_HOLD)) {
      td_spc_state = TD_HOLD_KEYPRESS;
    }
    if (td_slsh_state == TD_HOLD && IS_LAYER_ON(LAYER_SLSH_HOLD)) {
      td_slsh_state = TD_HOLD_KEYPRESS;
    }
    if (td_z_state == TD_HOLD && IS_LAYER_ON(LAYER_Z_HOLD)) {
      td_z_state = TD_HOLD_KEYPRESS;
    }
#endif // HBM_TD_HOLD_ORIGINAL_KEYPRESS_ENABLE

  }
}
#endif // TAP_DANCE_ENABLE

uint16_t get_tapping_term(uint16_t keycode, keyrecord_t *record) {
  switch (keycode) {
    // Set the tapping term for the homerow mods.
#ifdef HBM_HOMEROW_ENABLE
    case KC_X_SFTALT:
    case KC_C_ALT:
    case KC_D_CTL:
    case KC_V_SFTCTL:
    case KC_G_GUI:
    case KC_M_GUI:
    case KC_K_SFTCTL:
    case KC_H_CTL:
    case KC_COMM_ALT:
    case KC_DOT_SFTALT:
    case KC_CTL_X_SFTALT:
    case KC_CTL_C_ALT:
    case KC_SFT_CTL_C_CTL:
    case KC_CTL_V_SFTCTL:
      return TAPPING_TERM_MODS;
#endif // HBM_HOMEROW_ENABLE
    // Set the tapping term for tapdance keys.
    case KC_ENT_LAYER:
    case KC_SPC_LAYER:
      return TAPPING_TERM_THUMBKEY_TD;
    case KC_SLSH_LAYER:
    case KC_Z_LAYER:
      return TAPPING_TERM_SIDEKEY_TD;
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
    // Do not deactivate if the one-shot sym layer key is held down.
    case KC_OS_SYM:
      return true;
    // Deactivate caps word by default.
    default:
      return false;
  }
}

// Issues HYPR-keycode for Windows for AutoHotkey to interpret, and ALT-keycode
// for ChromeOS, which launches the n'th app on the task bar.

void hypr_or_alt(char *ss, bool pressed) {
  if (pressed) {
    if (hbm_is_chromebook) {
      SEND_STRING(SS_DOWN(X_LALT));
      SEND_STRING(ss);
      SEND_STRING(SS_UP(X_LALT));
    } else {
      SEND_STRING(SS_DOWN(X_LSFT)SS_DOWN(X_LCTL)SS_DOWN(X_LALT)SS_DOWN(X_LGUI));
      SEND_STRING(ss);
      SEND_STRING(SS_UP(X_LALT));
      SEND_STRING(SS_UP(X_LGUI)SS_UP(X_LALT)SS_UP(X_LCTL)SS_UP(X_LSFT));
    }
  }
}

// Interpret the current state of a tap dance. This is called at the end of the
// tap dance's tap time so we can make a judgement about whether the tap dance
// key is e.g. still held down and interpret that as a hold, and so on.

#ifdef TAP_DANCE_ENABLE
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
    //  Potentially we could add code here to catch triple taps etc to interpret
    //  *really* fast typing, but we have to stop somewhere.
  }
  return TD_UNKNOWN;
}

void td_ent_finished(tap_dance_state_t *state, void *user_data) {
  td_ent_state = td_get_state(state);
  switch (td_ent_state) {
    case TD_SINGLE_TAP:
      register_code(KC_ENT);
      break;
    case TD_DOUBLE_TAP:
      register_code(KC_ENT);
      unregister_code(KC_ENT);
      register_code(KC_ENT);
      break;
    case TD_HOLD:
      layer_on(LAYER_ENT_HOLD);
      break;
    default:
      break;
  }
}

void td_ent_reset(tap_dance_state_t *state, void *user_data) {
  switch (td_ent_state) {
    case TD_SINGLE_TAP:
    case TD_DOUBLE_TAP:
      unregister_code(KC_ENT);
      td_ent_state = TD_NONE;
      break;
    case TD_HOLD:
      // If the tap dance state is still TD_HOLD here then we know that another
      // key has not been pressed since the hold started, and we can issue the
      // original keypress.
#ifdef HBM_TD_HOLD_ORIGINAL_KEYPRESS_ENABLE
      tap_code(KC_ENT);
#endif // HBM_TD_HOLD_ORIGINAL_KEYPRESS_ENABLE
    case TD_HOLD_KEYPRESS:
      layer_off(LAYER_ENT_HOLD);
      td_ent_state = TD_NONE;
      break;
    default:
      td_ent_state = TD_NONE;
      break;
  }
}

void td_spc_finished(tap_dance_state_t *state, void *user_data) {
  td_spc_state = td_get_state(state);
  switch (td_spc_state) {
    case TD_SINGLE_TAP:
      register_code(KC_SPC);
      break;
    case TD_DOUBLE_TAP:
      register_code(KC_SPC);
      unregister_code(KC_SPC);
      register_code(KC_SPC);
      break;
    case TD_HOLD:
      layer_on(LAYER_SPC_HOLD);
      break;
    default:
      break;
  }
}

void td_spc_reset(tap_dance_state_t *state, void *user_data) {
  switch (td_spc_state) {
    case TD_SINGLE_TAP:
    case TD_DOUBLE_TAP:
      unregister_code(KC_SPC);
      td_spc_state = TD_NONE;
      break;
    case TD_HOLD:
      // If the tap dance state is still TD_HOLD here then we know that another
      // key has not been pressed since the hold started, and we can issue the
      // original keypress.
#ifdef HBM_TD_HOLD_ORIGINAL_KEYPRESS_ENABLE
      tap_code(KC_SPC);
#endif // HBM_TD_HOLD_ORIGINAL_KEYPRESS_ENABLE
    case TD_HOLD_KEYPRESS:
      layer_off(LAYER_SPC_HOLD);
      td_spc_state = TD_NONE;
      break;
    default:
      td_spc_state = TD_NONE;
      break;
  }
}

void td_slsh_finished(tap_dance_state_t *state, void *user_data) {
  td_slsh_state = td_get_state(state);
  switch (td_slsh_state) {
    case TD_SINGLE_TAP:
      layer_on(LAYER_SLSH_TAP);
      break;
    case TD_DOUBLE_TAP:
      register_code(KC_SLSH);
      break;
    case TD_HOLD:
      layer_on(LAYER_SLSH_HOLD);
      break;
    default:
      break;
  }
}

void td_slsh_reset(tap_dance_state_t *state, void *user_data) {
  switch (td_slsh_state) {
    case TD_SINGLE_TAP:
      td_slsh_state = TD_POST_TAP;
      break;
    case TD_DOUBLE_TAP:
      unregister_code(KC_SLSH);
      td_slsh_state = TD_NONE;
      break;
    case TD_HOLD:
      // If the tap dance state is still TD_HOLD here then we know that another
      // key has not been pressed since the hold started, and we can issue the
      // original keypress.
#ifdef HBM_TD_HOLD_ORIGINAL_KEYPRESS_ENABLE
      tap_code(KC_SLSH);
#endif // HBM_TD_HOLD_ORIGINAL_KEYPRESS_ENABLE
    case TD_HOLD_KEYPRESS:
      layer_off(LAYER_SLSH_HOLD);
      td_slsh_state = TD_NONE;
      break;
    default:
      td_slsh_state = TD_NONE;
      break;
  }
}

void td_z_finished(tap_dance_state_t *state, void *user_data) {
  td_z_state = td_get_state(state);
  switch (td_z_state) {
    case TD_SINGLE_TAP:
      layer_on(LAYER_Z_TAP);
      break;
    case TD_DOUBLE_TAP:
      register_code(KC_Z);
      break;
    case TD_HOLD:
      layer_on(LAYER_Z_HOLD);
      break;
    default:
      break;
  }
}

void td_z_reset(tap_dance_state_t *state, void *user_data) {
  switch (td_z_state) {
    case TD_SINGLE_TAP:
      td_z_state = TD_POST_TAP;
      break;
    case TD_DOUBLE_TAP:
      unregister_code(KC_Z);
      td_z_state = TD_NONE;
      break;
    case TD_HOLD:
      // If the tap dance state is still TD_HOLD here then we know that another
      // key has not been pressed since the hold started, and we can issue the
      // original keypress.
#ifdef HBM_TD_HOLD_ORIGINAL_KEYPRESS_ENABLE
      tap_code(KC_Z);
#endif // HBM_TD_HOLD_ORIGINAL_KEYPRESS_ENABLE
    case TD_HOLD_KEYPRESS:
      layer_off(LAYER_Z_HOLD);
      td_z_state = TD_NONE;
      break;
    default:
      td_z_state = TD_NONE;
      break;
  }
}
#endif // TAP_DANCE_ENABLE

// Reduce the size of the compiled firmware.

uint16_t keycode_config(uint16_t keycode) {
    return keycode;
}

uint8_t mod_config(uint8_t mod) {
    return mod;
}
