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

typedef enum {
  TD_NONE,
  TD_UNKNOWN,
  TD_SINGLE_TAP,
  TD_DOUBLE_TAP,
  TD_HOLD
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

// Defines whether to issue Windows or ChromeOS keypresses from macros - Windows
// by default.

static bool hbm_is_chromebook = false;

// Issues HYPR-keypress for Windows and ALT-keypress for ChromeOS.

void hypr_or_alt(char *ss, bool pressed);

// State of the M_ALT_TAB macro - true if we are currently tabbing between
// windows.

static bool hbm_alt_tab_pressed = false;

// State for managing shift-backspace behaviour.

static bool hbm_del_registered = false;
static uint8_t hbm_mod_state = 0;

bool process_record_user(uint16_t keycode, keyrecord_t *record) {

  // Stop pressing the KC_LALT key once M_ALT_TAB is no longer being pressed.

  if (keycode != M_ALT_TAB && hbm_alt_tab_pressed) {
    unregister_code(KC_LALT);
    hbm_alt_tab_pressed = false;
  }

  // Ensure that shift is not pressed when additional layers are active, except
  // for tapdance layer keypresses.

  uint8_t current_layer = get_highest_layer(layer_state);

  if (current_layer > LAYER_BASE) {
    switch (keycode) {
      case KC_Z:
      case KC_SLSH:
        break;
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

    // Close the current tab.

    case M_XTAB:
      if (record->event.pressed) {
        if (hbm_is_chromebook) {
          SEND_STRING(SS_DOWN(X_LCTL)SS_TAP(X_W)SS_UP(X_LCTL));
        } else {
          SEND_STRING(SS_DOWN(X_LCTL)SS_TAP(X_F4)SS_UP(X_LCTL));
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

void post_process_record_user(uint16_t keycode, keyrecord_t *record) {

  // Ensure that tap layers are turned off after a single keypress - this
  // emulates the behaviour of oneshot layers.

  if (!record->event.pressed) {
    if (IS_LAYER_ON(LAYER_SLSH_TAP) && keycode != KC_SLSH_LAYER) {
      layer_off(LAYER_SLSH_TAP);
    }
    if (IS_LAYER_ON(LAYER_Z_TAP) && keycode != KC_Z_LAYER) {
      layer_off(LAYER_Z_TAP);
    }
  }

}

uint16_t get_tapping_term(uint16_t keycode, keyrecord_t *record) {
  switch (keycode) {
    // Set the tapping term for the homerow mods.
    case KC_X_CTL:
    case KC_C_ALT:
    case KC_D_GUI:
    case KC_H_GUI:
    case KC_COMM_ALT:
    case KC_DOT_CTL:
      return TAPPING_TERM_MODS;
    // Set the tapping term for tapdance keys.
    case KC_ENT_LAYER:
    case KC_SPC_LAYER:
    case KC_SLSH_LAYER:
    case KC_Z_LAYER:
      return TAPPING_TERM_TD;
    default:
      return TAPPING_TERM;
  }
}

// Issues HYPR-keycode for Windows and ALT-keycode for ChromeOS.

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
      break;
    case TD_HOLD:
      layer_off(LAYER_ENT_HOLD);
      break;
    default:
      break;
  }
  td_ent_state = TD_NONE;
}

void td_spc_finished(tap_dance_state_t *state, void *user_data) {
  td_spc_state = td_get_state(state);
  switch (td_spc_state) {
    case TD_SINGLE_TAP:
      register_code(KC_SPC);
      break;
    case TD_DOUBLE_TAP:
      register_code(KC_TAB);
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
      unregister_code(KC_SPC);
      break;
    case TD_DOUBLE_TAP:
      unregister_code(KC_TAB);
      break;
    case TD_HOLD:
      layer_off(LAYER_SPC_HOLD);
      break;
    default:
      break;
  }
  td_spc_state = TD_NONE;
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
      // LAYER_SLSH_TAP turned off in post_process_record_user()
      break;
    case TD_DOUBLE_TAP:
      unregister_code(KC_SLSH);
      break;
    case TD_HOLD:
      layer_off(LAYER_SLSH_HOLD);
      break;
    default:
      break;
  }
  td_slsh_state = TD_NONE;
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
      // LAYER_Z_TAP turned off in post_process_record_user()
      break;
    case TD_DOUBLE_TAP:
      unregister_code(KC_Z);
      break;
    case TD_HOLD:
      layer_off(LAYER_Z_HOLD);
      break;
    default:
      break;
  }
  td_z_state = TD_NONE;
}
