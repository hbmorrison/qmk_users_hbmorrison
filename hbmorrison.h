/*
Copyright KM_2023 Hannah Blythe Morrison

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

#ifndef USERSPACE
#define USERSPACE

#include "quantum.h"

// Layers and layer aliases.

enum hbm_layers {
  LAYER_BASE,
  LAYER_LSYM,
  LAYER_RSYM,
  LAYER_NAV,
  LAYER_NUM
};

// Custom keycodes.

enum hbm_keycodes {
  M_ALT_TAB = SAFE_RANGE,
  M_NDESK,
  M_PDESK
};

// Tapdance keycodes.

enum {
  TD_RSFT
};

// Alternative keys for UK ISO keyboard layouts.

#define KC_UK_DQUO LSFT(KC_2)
#define KC_UK_PND LSFT(KC_3)
#define KC_UK_PIPE LSFT(KC_NUBS)
#define KC_UK_BSLS KC_NUBS
#define KC_UK_AT LSFT(KC_QUOT)
#define KC_UK_TILDE LSFT(KC_BSLS)
#define KC_UK_HASH KC_BSLS

// Modified keys.

#define KC_OSM_LSFT OSM(MOD_LSFT)
#define KC_OSM_RSFT TD(TD_RSFT)
#define KC_SFT_TAB LSFT(KC_TAB)
#define KC_CTL_TAB LCTL(KC_TAB)
#define KC_GUI_TAB LGUI(KC_TAB)

// Modifier keys in the style of homerow mods.

#define KC_D_CTL LCTL_T(KC_D)
#define KC_C_ALT LALT_T(KC_C)
#define KC_X_GUI LGUI_T(KC_X)
#define KC_H_CTL RCTL_T(KC_H)
#define KC_COMMA_ALT LALT_T(KC_COMMA)
#define KC_DOT_GUI LGUI_T(KC_DOT)
#define KC_F7_CTL RCTL_T(KC_F7)
#define KC_F8_ALT LALT_T(KC_F8)
#define KC_F9_GUI LGUI_T(KC_F9)

// Left- and right-hand modifier bits.

#define MOD_BITS_LEFT (MOD_BIT(KC_LSFT) | MOD_BIT(KC_LCTL))
#define MOD_BITS_RIGHT (MOD_BIT(KC_RSFT) | MOD_BIT(KC_RCTL))

// Layer keys.

#define KC_Z_RSYM OSL(LAYER_RSYM)
#define KC_SLSH_LSYM OSL(LAYER_LSYM)
#define KC_SPC_NAV LT(LAYER_NAV, KC_SPC)
#define KC_ENT_NUM LT(LAYER_NUM, KC_ENT)

// Base layer.

#define KM_BASE_1L KC_Q, KC_W, KC_F, KC_P, KC_B
#define KM_BASE_2L KC_A, KC_R, KC_S, KC_T, KC_G
#define KM_BASE_3L KC_Z_RSYM, KC_X_GUI, KC_C_ALT, KC_D_CTL, KC_V

// Fifth keycode in KM_BASE_1R is missing so that it can be defined differently
// in the Ferris and Planck keymaps.

#define KM_BASE_1R KC_J, KC_L, KC_U, KC_Y
#define KM_BASE_2R KC_M, KC_N, KC_E, KC_I, KC_O
#define KM_BASE_3R KC_K, KC_H_CTL, KC_COMMA_ALT, KC_DOT_GUI, KC_SLSH_LSYM

#define KM_BASE_1 KM_BASE_1L, KM_BASE_1R, KC_BSPC
#define KM_BASE_2 KM_BASE_2L, KM_BASE_2R
#define KM_BASE_3 KM_BASE_3L, KM_BASE_3R

#define KM_BASE_THUMB KC_OSM_LSFT, KC_SPC_NAV, KC_ENT_NUM, KC_OSM_RSFT

#define LAYOUT_BASE KM_BASE_1, KM_BASE_2, KM_BASE_3, KM_BASE_THUMB

// Left symbol layer.

#define KM_LSYM_1L KC_EXLM, KC_UK_DQUO, KC_UK_PND, KC_DLR, KC_PERC
#define KM_LSYM_2L KC_GRV, KC_UK_PIPE, KC_LBRC, KC_LCBR, KC_LPRN
#define KM_LSYM_3L KC_CAPS, KC_UK_BSLS, KC_RBRC, KC_RCBR, KC_RPRN

#define KM_LSYM_1R KC_NO, KC_NO, KC_NO, KC_NO, KC_NO
#define KM_LSYM_2R KC_NO, KC_NO, KC_NO, KC_NO, KC_NO
#define KM_LSYM_3R KC_NO, KC_NO, KC_NO, KC_NO, KC_SLSH

#define KM_LSYM_1 KM_LSYM_1L, KM_LSYM_1R
#define KM_LSYM_2 KM_LSYM_2L, KM_LSYM_2R
#define KM_LSYM_3 KM_LSYM_3L, KM_LSYM_3R

#define KM_LSYM_THUMB KC_NO, KC_NO, KC_NO, KC_TRNS

#define LAYOUT_LSYM KM_LSYM_1, KM_LSYM_2, KM_LSYM_3, KM_LSYM_THUMB

// Right symbol layer.

#define KM_RSYM_1L KC_NO, KC_NO, KC_NO, KC_NO, KC_NO
#define KM_RSYM_2L KC_NO, KC_NO, KC_NO, KC_NO, KC_NO
#define KM_RSYM_3L KC_Z, KC_NO, KC_NO, KC_NO, KC_NO

#define KM_RSYM_1R KC_CIRC, KC_AMPR, KC_ASTR, KC_UNDS, KC_PLUS
#define KM_RSYM_2R KC_COLN, KC_UK_AT, KC_UK_TILDE, KC_MINS, KC_EQL
#define KM_RSYM_3R KC_SCLN, KC_QUOT, KC_UK_HASH, KC_GT, KC_QUES

#define KM_RSYM_1 KM_RSYM_1L, KM_RSYM_1R
#define KM_RSYM_2 KM_RSYM_2L, KM_RSYM_2R
#define KM_RSYM_3 KM_RSYM_3L, KM_RSYM_3R

#define KM_RSYM_THUMB KC_NO, KC_NO, KC_NO, KC_TRNS

#define LAYOUT_RSYM KM_RSYM_1, KM_RSYM_2, KM_RSYM_3, KM_RSYM_THUMB

// Navigation layer.

#define KM_NAV_1L KC_NO, KC_MPLY, KC_MUTE, KC_PSCR, KC_NO
#define KM_NAV_2L KC_NO, KC_MNXT, KC_VOLU, KC_BRIU, KC_NO
#define KM_NAV_3L KC_NO, KC_MPRV, KC_VOLD, KC_BRID, KC_NO

#define KM_NAV_1R KC_GUI_TAB, M_PDESK, KC_CTL_TAB, M_ALT_TAB, M_NDESK
#define KM_NAV_2R KC_WH_U, KC_LEFT, KC_DOWN, KC_UP, KC_RIGHT
#define KM_NAV_3R KC_WH_D, KC_HOME, KC_PGDN, KC_PGUP, KC_END

#define KM_NAV_1 KM_NAV_1L, KM_NAV_1R
#define KM_NAV_2 KM_NAV_2L, KM_NAV_2R
#define KM_NAV_3 KM_NAV_3L, KM_NAV_3R

#define KM_NAV_THUMB KC_NO, KC_TRNS, KC_ESC, KC_NO

#define LAYOUT_NAV KM_NAV_1, KM_NAV_2, KM_NAV_3, KM_NAV_THUMB

// Number layer.

#define KM_NUM_1L KC_NO, KC_1, KC_2, KC_3, KC_NO
#define KM_NUM_2L KC_NO, KC_4, KC_5, KC_6, KC_PSLS
#define KM_NUM_3L KC_NO, KC_7, KC_8, KC_9, KC_DOT

#define KM_NUM_1R KC_NO, KC_F1, KC_F2, KC_F3, KC_F10
#define KM_NUM_2R KC_BTN1, KC_F4, KC_F5, KC_F6, KC_F11
#define KM_NUM_3R KC_BTN2, KC_F7_CTL, KC_F8_ALT, KC_F9_GUI, KC_F12

#define KM_NUM_1 KM_NUM_1L, KM_NUM_1R
#define KM_NUM_2 KM_NUM_2L, KM_NUM_2R
#define KM_NUM_3 KM_NUM_3L, KM_NUM_3R

#define KM_NUM_THUMB KC_0, KC_TAB, KC_TRNS, KC_NO

#define LAYOUT_NUM KM_NUM_1, KM_NUM_2, KM_NUM_3, KM_NUM_THUMB

// Planck base layer.

#define KM_PLANCK_BASE_1 KC_ESC, KM_BASE_1L, KM_BASE_1R, KC_DEL, KC_BSPC
#define KM_PLANCK_BASE_2 KC_TAB, KM_BASE_2L, KM_BASE_2R, KC_MEH
#define KM_PLANCK_BASE_3 KC_CAPS, KM_BASE_3L, KM_BASE_3R, CW_TOGG

#define KM_PLANCK_BASE_MODS KC_LSFT, KC_LCTL, KC_LALT, KC_LGUI
#define KM_PLANCK_BASE_ARROWS KC_LEFT, KC_DOWN, KC_UP, KC_RIGHT
#define KM_PLANCK_BASE_THUMB KM_PLANCK_BASE_MODS, KM_BASE_THUMB, KM_PLANCK_BASE_ARROWS

#define PLANCK_LAYOUT_BASE KM_PLANCK_BASE_1, KM_PLANCK_BASE_2, KM_PLANCK_BASE_3, KM_PLANCK_BASE_THUMB

// Planck left symbol layer.

#define KM_PLANCK_LSYM_1 KC_TRNS, KM_LSYM_1L, KM_LSYM_1R, KC_TRNS
#define KM_PLANCK_LSYM_2 KC_TRNS, KM_LSYM_2L, KM_LSYM_2R, KC_TRNS
#define KM_PLANCK_LSYM_3 KC_TRNS, KM_LSYM_3L, KM_LSYM_3R, KC_TRNS

#define KM_PLANCK_LSYM_THUMB KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KM_LSYM_THUMB, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS

#define PLANCK_LAYOUT_LSYM KM_PLANCK_LSYM_1, KM_PLANCK_LSYM_2, KM_PLANCK_LSYM_3, KM_PLANCK_LSYM_THUMB

// Planck right symbol layer.

#define KM_PLANCK_RSYM_1 KC_TRNS, KM_RSYM_1L, KM_RSYM_1R, KC_TRNS
#define KM_PLANCK_RSYM_2 KC_TRNS, KM_RSYM_2L, KM_RSYM_2R, KC_TRNS
#define KM_PLANCK_RSYM_3 KC_TRNS, KM_RSYM_3L, KM_RSYM_3R, KC_TRNS

#define KM_PLANCK_RSYM_THUMB KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KM_RSYM_THUMB, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS

#define PLANCK_LAYOUT_RSYM KM_PLANCK_RSYM_1, KM_PLANCK_RSYM_2, KM_PLANCK_RSYM_3, KM_PLANCK_RSYM_THUMB

// Planck navigation layer.

#define KM_PLANCK_NAV_1 KC_TRNS, KM_NAV_1L, KM_NAV_1R, KC_TRNS
#define KM_PLANCK_NAV_2 KC_TRNS, KM_NAV_2L, KM_NAV_2R, KC_TRNS
#define KM_PLANCK_NAV_3 KC_TRNS, KM_NAV_3L, KM_NAV_3R, KC_TRNS

#define KM_PLANCK_NAV_THUMB KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KM_NAV_THUMB, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS

#define PLANCK_LAYOUT_NAV KM_PLANCK_NAV_1, KM_PLANCK_NAV_2, KM_PLANCK_NAV_3, KM_PLANCK_NAV_THUMB

// Planck number layer.

#define KM_PLANCK_NUM_1 KC_TRNS, KM_NUM_1L, KM_NUM_1R, KC_TRNS
#define KM_PLANCK_NUM_2 KC_TRNS, KM_NUM_2L, KM_NUM_2R, KC_TRNS
#define KM_PLANCK_NUM_3 KC_TRNS, KM_NUM_3L, KM_NUM_3R, KC_TRNS

#define KM_PLANCK_NUM_THUMB KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KM_NUM_THUMB, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS

#define PLANCK_LAYOUT_NUM KM_PLANCK_NUM_1, KM_PLANCK_NUM_2, KM_PLANCK_NUM_3, KM_PLANCK_NUM_THUMB

#endif // USERSPACE
