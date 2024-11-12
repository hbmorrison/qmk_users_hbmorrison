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

#ifndef USERSPACE
#define USERSPACE

#include "quantum.h"

// Supported operating systems.

enum hbm_operatingsystems {
  OS_WINDOWS,
  OS_CHROMEOS,
  OS_LINUX
};

// Layers and layer aliases.

enum hbm_layers {
  LAYER_BASE,
  LAYER_LSYM,
  LAYER_RSYM,
  LAYER_NAV,
  LAYER_NUM,
  LAYER_FUNC,
  LAYER_CTRL
};

// Custom keycodes.

enum hbm_keycodes {
  M_ALT_TAB = SAFE_RANGE,
  M_ESC_COLN,
  M_EQL_GT,
  M_NDESK,
  M_PDESK,
  M_OVERVIEW,
  M_FULLSCREEN,
  M_EMOJI,
  M_ISWINDOWS,
  M_ISCHROMEOS,
  M_ISLINUX
};

// Alternative keys for UK ISO keyboard layouts.

#define KC_UK_DQUO LSFT(KC_2)
#define KC_UK_PND LSFT(KC_3)
#define KC_UK_PIPE LSFT(KC_NUBS)
#define KC_UK_BSLS KC_NUBS
#define KC_UK_AT LSFT(KC_QUOT)
#define KC_UK_TILDE LSFT(KC_BSLS)
#define KC_UK_HASH KC_BSLS

// Modified tab keys.

#define KC_SFT_TAB LSFT(KC_TAB)
#define KC_CTL_TAB LCTL(KC_TAB)

// Layer keys.

#define KC_OSM_SFT OSM(MOD_LSFT)
#define KC_SPC_NAV LT(LAYER_NAV, KC_SPC)
#define KC_ENT_NUM LT(LAYER_NUM, KC_ENT)
#define KC_P_FUNC LT(LAYER_FUNC, KC_P)
#define KC_L_CTRL LT(LAYER_CTRL, KC_L)
#define KC_K_LSYM LT(LAYER_LSYM, KC_K)
#define KC_V_RSYM LT(LAYER_RSYM, KC_V)
#define KC_N_LSYM LT(LAYER_LSYM, KC_N)
#define KC_T_RSYM LT(LAYER_RSYM, KC_T)

// Modifier keys in the style of homerow mods.

#define KC_X_GUI LGUI_T(KC_X)
#define KC_C_ALT LALT_T(KC_C)
#define KC_D_CTL LCTL_T(KC_D)
#define KC_H_CTL LCTL_T(KC_H)
#define KC_COMMA_ALT LALT_T(KC_COMMA)
#define KC_DOT_GUI LGUI_T(KC_DOT)

// Base layer.

#define KM_BASE_1L KC_Q, KC_W, KC_F, KC_P_FUNC, KC_B
#define KM_BASE_2L KC_A, KC_R, KC_S, KC_T_RSYM, KC_G
#define KM_BASE_3L KC_Z, KC_X_GUI, KC_C_ALT, KC_D_CTL, KC_V

#define KM_BASE_1R KC_J, KC_L_CTRL, KC_U, KC_Y, KC_BSPC
#define KM_BASE_2R KC_M, KC_N_LSYM, KC_E, KC_I, KC_O
#define KM_BASE_3R KC_K, KC_H_CTL, KC_COMMA_ALT, KC_DOT_GUI, KC_SLSH

#define KM_BASE_1 KM_BASE_1L, KM_BASE_1R
#define KM_BASE_2 KM_BASE_2L, KM_BASE_2R
#define KM_BASE_3 KM_BASE_3L, KM_BASE_3R

#define KM_BASE_THUMB KC_OSM_SFT, KC_SPC_NAV, KC_ENT_NUM, KC_OSM_SFT

#define LAYOUT_BASE KM_BASE_1, KM_BASE_2, KM_BASE_3, KM_BASE_THUMB

// Left symbol layer.

#define KM_LSYM_1L KC_EXLM, KC_UK_DQUO, KC_UK_PND, KC_DLR, KC_PERC
#define KM_LSYM_2L KC_GRV, KC_UK_PIPE, KC_LBRC, KC_LCBR, KC_LPRN
#define KM_LSYM_3L KC_NO, KC_UK_BSLS, KC_RBRC, KC_RCBR, KC_RPRN

#define KM_LSYM_1R KC_NO, KC_NO, KC_NO, KC_NO, KC_NO
#define KM_LSYM_2R KC_NO, KC_NO, KC_NO, KC_NO, KC_NO
#define KM_LSYM_3R KC_TRNS, KC_NO, KC_NO, KC_NO, KC_NO

#define KM_LSYM_1 KM_LSYM_1L, KM_LSYM_1R
#define KM_LSYM_2 KM_LSYM_2L, KM_LSYM_2R
#define KM_LSYM_3 KM_LSYM_3L, KM_LSYM_3R

#define KM_LSYM_THUMB KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS

#define LAYOUT_LSYM KM_LSYM_1, KM_LSYM_2, KM_LSYM_3, KM_LSYM_THUMB

// Right symbol layer.

#define KM_RSYM_1L KC_NO, KC_NO, KC_NO, KC_NO, KC_NO
#define KM_RSYM_2L KC_NO, KC_NO, KC_NO, KC_NO, KC_NO
#define KM_RSYM_3L KC_NO, KC_NO, KC_NO, KC_NO, KC_TRNS

#define KM_RSYM_1R KC_CIRC, KC_AMPR, KC_ASTR, KC_UNDS, KC_PLUS
#define KM_RSYM_2R KC_COLN, KC_UK_AT, KC_UK_TILDE, KC_MINS, KC_EQL
#define KM_RSYM_3R KC_SCLN, KC_QUOT, KC_UK_HASH, M_EQL_GT, KC_QUES

#define KM_RSYM_1 KM_RSYM_1L, KM_RSYM_1R
#define KM_RSYM_2 KM_RSYM_2L, KM_RSYM_2R
#define KM_RSYM_3 KM_RSYM_3L, KM_RSYM_3R

#define KM_RSYM_THUMB KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS

#define LAYOUT_RSYM KM_RSYM_1, KM_RSYM_2, KM_RSYM_3, KM_RSYM_THUMB

// Number layer.

#define KM_NUM_1L KC_NO, KC_1, KC_2, KC_3, KC_PSLS
#define KM_NUM_2L KC_NO, KC_4, KC_5, KC_6, KC_DOT
#define KM_NUM_3L KC_NO, KC_7, KC_8, KC_9, KC_0

#define KM_NUM_1R KC_NO, KC_NO, KC_NO, KC_NO, KC_NO
#define KM_NUM_2R KC_MS_BTN1, KC_NO, KC_NO, KC_NO, KC_NO
#define KM_NUM_3R KC_MS_BTN2, KC_NO, KC_NO, KC_NO, KC_NO

#define KM_NUM_1 KM_NUM_1L, KM_NUM_1R
#define KM_NUM_2 KM_NUM_2L, KM_NUM_2R
#define KM_NUM_3 KM_NUM_3L, KM_NUM_3R

#define KM_NUM_THUMB KC_SFT_TAB, KC_TAB, KC_TRNS, KC_TRNS

#define LAYOUT_NUM KM_NUM_1, KM_NUM_2, KM_NUM_3, KM_NUM_THUMB

// Navigation layer.

#define KM_NAV_1L KC_NO, KC_NO, KC_NO, KC_NO, KC_NO
#define KM_NAV_2L KC_NO, KC_NO, KC_NO, KC_NO, KC_NO
#define KM_NAV_3L KC_NO, KC_NO, KC_NO, KC_NO, KC_NO

#define KM_NAV_1R M_OVERVIEW, M_PDESK, KC_CTL_TAB, M_ALT_TAB, M_NDESK
#define KM_NAV_2R M_FULLSCREEN, KC_LEFT, KC_DOWN, KC_UP, KC_RIGHT
#define KM_NAV_3R M_EMOJI, KC_HOME, KC_PGDN, KC_PGUP, KC_END

#define KM_NAV_1 KM_NAV_1L, KM_NAV_1R
#define KM_NAV_2 KM_NAV_2L, KM_NAV_2R
#define KM_NAV_3 KM_NAV_3L, KM_NAV_3R

#define KM_NAV_THUMB KC_TRNS, KC_TRNS, KC_ESC, M_ESC_COLN

#define LAYOUT_NAV KM_NAV_1, KM_NAV_2, KM_NAV_3, KM_NAV_THUMB

// Function key layer.

#define KM_FUNC_1L KC_NO, KC_NO, KC_TRNS, KC_NO, KC_NO
#define KM_FUNC_2L KC_NO, KC_NO, KC_NO, KC_NO, KC_NO
#define KM_FUNC_3L KC_NO, KC_NO, KC_NO, KC_NO, KC_NO

#define KM_FUNC_1R KC_F1, KC_F2, KC_F3, KC_F4, KC_F5
#define KM_FUNC_2R KC_F6, KC_F7, KC_F8, KC_F9, KC_F10
#define KM_FUNC_3R KC_F11, KC_F12, KC_F13, KC_F14, KC_F15

#define KM_FUNC_1 KM_FUNC_1L, KM_FUNC_1R
#define KM_FUNC_2 KM_FUNC_2L, KM_FUNC_2R
#define KM_FUNC_3 KM_FUNC_3L, KM_FUNC_3R

#define KM_FUNC_THUMB KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS

#define LAYOUT_FUNC KM_FUNC_1, KM_FUNC_2, KM_FUNC_3, KM_FUNC_THUMB

// Controls layer.

#define KM_CTRL_1L KC_NO, KC_MPLY, KC_MUTE, KC_PSCR, M_ISWINDOWS
#define KM_CTRL_2L KC_NO, KC_MNXT, KC_VOLU, KC_BRIU, M_ISCHROMEOS
#define KM_CTRL_3L KC_NO, KC_MPRV, KC_VOLD, KC_BRID, M_ISLINUX

#define KM_CTRL_1R KC_NO, KC_NO, KC_TRNS, KC_NO, KC_NO
#define KM_CTRL_2R KC_NO, KC_NO, KC_NO, KC_NO, KC_NO
#define KM_CTRL_3R KC_NO, KC_NO, KC_NO, KC_NO, KC_NO

#define KM_CTRL_1 KM_CTRL_1L, KM_CTRL_1R
#define KM_CTRL_2 KM_CTRL_2L, KM_CTRL_2R
#define KM_CTRL_3 KM_CTRL_3L, KM_CTRL_3R

#define KM_CTRL_THUMB KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS

#define LAYOUT_CTRL KM_CTRL_1, KM_CTRL_2, KM_CTRL_3, KM_CTRL_THUMB

#endif // USERSPACE
