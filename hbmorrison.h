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
  LAYER_SYM,
  LAYER_NAV,
  LAYER_NUM,
  LAYER_SCUT
};

// Custom keycodes.

enum hbm_keycodes {
  M_ALT_TAB = SAFE_RANGE,
  M_APP1,
  M_APP2,
  M_APP3,
  M_APP4,
  M_APP5,
  M_APP6,
  M_1PASS,
  M_NDESK,
  M_PDESK,
  M_TCLOSE,
  M_WMIN,
  M_WKILL,
  M_WFOCUS,
  M_NTERM,
  M_EMOJI,
  M_ISCROS,
  M_ISWIN,
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

#define KC_OSM_SHIFT OSM(MOD_LSFT)
#define KC_SFT_TAB LSFT(KC_TAB)
#define KC_CTL_TAB LCTL(KC_TAB)

// Modifier keys in the style of homerow mods.

#define KC_T_SFT LSFT_T(KC_T)
#define KC_N_SFT RSFT_T(KC_N)
#define KC_I_CTL LCTL_T(KC_I)
#define KC_R_CTL LCTL_T(KC_R)
#define KC_X_ALT LALT_T(KC_X)
#define KC_DOT_ALT LALT_T(KC_DOT)
#define KC_D_GUI LGUI_T(KC_D)
#define KC_H_GUI LGUI_T(KC_H)
#define KC_S_CS C_S_T(KC_S)
#define KC_E_CS C_S_T(KC_E)
#define KC_A_MEH MEH_T(KC_A)
#define KC_O_MEH MEH_T(KC_O)

// Layer keys.

#define KC_SYM OSL(LAYER_SYM)
#define KC_SCUT OSL(LAYER_SCUT)
#define KC_SPC_NAV LT(LAYER_NAV, KC_SPC)
#define KC_ENT_NUM LT(LAYER_NUM, KC_ENT)

// Base layer.

#define KM_BASE_1L KC_Q, KC_W, KC_F, KC_P, KC_B
#define KM_BASE_2L KC_A_MEH, KC_R_CTL, KC_S_CS, KC_T_SFT, KC_G
#define KM_BASE_3L KC_Z, KC_X_ALT, KC_C, KC_D_GUI, KC_V

#define KM_BASE_1R KC_J, KC_L, KC_U, KC_Y, KC_BSPC
#define KM_BASE_2R KC_M, KC_N_SFT, KC_E_CS, KC_I_CTL, KC_O_MEH
#define KM_BASE_3R KC_K, KC_H_GUI, KC_COMM, KC_DOT_ALT, KC_SLSH

#define KM_BASE_1 KM_BASE_1L, KM_BASE_1R
#define KM_BASE_2 KM_BASE_2L, KM_BASE_2R
#define KM_BASE_3 KM_BASE_3L, KM_BASE_3R

#define KM_BASE_THUMB KC_OSM_SHIFT, KC_SPC_NAV, KC_ENT_NUM, KC_SYM

#define LAYOUT_BASE KM_BASE_1, KM_BASE_2, KM_BASE_3, KM_BASE_THUMB

// Symbol layer.

#define KM_SYM_1L KC_EXLM, KC_UK_DQUO, KC_UK_PND, KC_DLR, KC_PERC
#define KM_SYM_2L KC_GRV, KC_UK_PIPE, KC_LBRC, KC_LCBR, KC_LPRN
#define KM_SYM_3L KC_NO, KC_UK_BSLS, KC_RBRC, KC_RCBR, KC_RPRN

#define KM_SYM_1R KC_CIRC, KC_AMPR, KC_ASTR, KC_UNDS, KC_PLUS
#define KM_SYM_2R KC_COLN, KC_UK_AT, KC_UK_TILDE, KC_MINS, KC_EQL
#define KM_SYM_3R KC_SCLN, KC_QUOT, KC_UK_HASH, KC_GT, KC_QUES

#define KM_SYM_1 KM_SYM_1L, KM_SYM_1R
#define KM_SYM_2 KM_SYM_2L, KM_SYM_2R
#define KM_SYM_3 KM_SYM_3L, KM_SYM_3R

#define KM_SYM_THUMB KC_NO, KC_NO, KC_NO, KC_SCUT

#define LAYOUT_SYM KM_SYM_1, KM_SYM_2, KM_SYM_3, KM_SYM_THUMB

// Navigation layer.

#define KM_NAV_1L KC_PSCR, KC_MPLY, KC_VOLU, KC_BRIU, KC_NO
#define KM_NAV_2L KC_NO, KC_MNXT, KC_VOLD, KC_BRID, KC_NO
#define KM_NAV_3L KC_CAPS, KC_MPRV, KC_MUTE, KC_NO, KC_NO

#define KM_NAV_1R KC_NO, M_PDESK, KC_CTL_TAB, M_ALT_TAB, M_NDESK
#define KM_NAV_2R KC_WH_U, KC_LEFT, KC_DOWN, KC_UP, KC_RIGHT
#define KM_NAV_3R KC_WH_D, KC_HOME, KC_PGDN, KC_PGUP, KC_END

#define KM_NAV_1 KM_NAV_1L, KM_NAV_1R
#define KM_NAV_2 KM_NAV_2L, KM_NAV_2R
#define KM_NAV_3 KM_NAV_3L, KM_NAV_3R

#define KM_NAV_THUMB KC_NO, KC_TRNS, KC_ESC, KC_NO

#define LAYOUT_NAV KM_NAV_1, KM_NAV_2, KM_NAV_3, KM_NAV_THUMB

// Number layer.

#define KM_NUM_1L KC_NO, KC_1, KC_2, KC_3, KC_NO
#define KM_NUM_2L KC_NO, KC_4, KC_5, KC_6, KC_DOT
#define KM_NUM_3L KC_NO, KC_7, KC_8, KC_9, KC_0

#define KM_NUM_1R KC_NO, KC_F1, KC_F2, KC_F3, KC_BSPC
#define KM_NUM_2R KC_NO, KC_F4, KC_F5, KC_F6, KC_NO
#define KM_NUM_3R KC_NO, KC_F7, KC_F8, KC_F9, KC_F10

#define KM_NUM_1 KM_NUM_1L, KM_NUM_1R
#define KM_NUM_2 KM_NUM_2L, KM_NUM_2R
#define KM_NUM_3 KM_NUM_3L, KM_NUM_3R

#define KM_NUM_THUMB KC_SFT_TAB, KC_TAB, KC_TRNS, KC_NO

#define LAYOUT_NUM KM_NUM_1, KM_NUM_2, KM_NUM_3, KM_NUM_THUMB

// Shortcut layer.

#define KM_SCUT_1L M_ISWIN, M_ISCROS, M_WFOCUS, M_1PASS, KC_NO
#define KM_SCUT_2L KC_NO, M_APP1, M_APP2, M_APP3, KC_NO
#define KM_SCUT_3L KC_NO, M_APP4, M_APP5, M_APP6, KC_NO

#define KM_SCUT_1R M_TCLOSE, KC_NO, KC_NO, KC_NO, KC_NO
#define KM_SCUT_2R M_WMIN, M_NTERM, M_EMOJI, KC_NO, KC_NO
#define KM_SCUT_3R M_WKILL, KC_NO, KC_NO, KC_NO, KC_NO

#define KM_SCUT_1 KM_SCUT_1L, KM_SCUT_1R
#define KM_SCUT_2 KM_SCUT_2L, KM_SCUT_2R
#define KM_SCUT_3 KM_SCUT_3L, KM_SCUT_3R

#define KM_SCUT_THUMB KC_NO, KC_NO, KC_NO, KC_NO

#define LAYOUT_SCUT KM_SCUT_1, KM_SCUT_2, KM_SCUT_3, KM_SCUT_THUMB

// Planck base layer.

#define KM_PLANCK_BASE_1 KC_ESC, KM_BASE_1L, KM_BASE_1R, KC_BSPC
#define KM_PLANCK_BASE_2 KC_TAB, KM_BASE_2L, KM_BASE_2R, KC_DEL
#define KM_PLANCK_BASE_3 KC_CAPS, KM_BASE_3L, KM_BASE_3R, KC_SCUT

#define KM_PLANCK_BASE_THUMB KC_LSFT, KC_LCTL, KC_LALT, KC_LGUI, KM_BASE_THUMB, KC_LEFT, KC_DOWN, KC_UP, KC_RIGHT

#define PLANCK_LAYOUT_BASE KM_PLANCK_BASE_1, KM_PLANCK_BASE_2, KM_PLANCK_BASE_3, KM_PLANCK_BASE_THUMB

// Planck symbol layer.

#define KM_PLANCK_SYM_1 KC_TRNS, KM_SYM_1L, KM_SYM_1R, KC_TRNS
#define KM_PLANCK_SYM_2 KC_TRNS, KM_SYM_2L, KM_SYM_2R, KC_TRNS
#define KM_PLANCK_SYM_3 KC_TRNS, KM_SYM_3L, KM_SYM_3R, KC_TRNS

#define KM_PLANCK_SYM_THUMB KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KM_SYM_THUMB, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS

#define PLANCK_LAYOUT_SYM KM_PLANCK_SYM_1, KM_PLANCK_SYM_2, KM_PLANCK_SYM_3, KM_PLANCK_SYM_THUMB

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

// Planck shortcut layer.

#define KM_PLANCK_SCUT_1 KC_TRNS, KM_SCUT_1L, KM_SCUT_1R, KC_TRNS
#define KM_PLANCK_SCUT_2 KC_TRNS, KM_SCUT_2L, KM_SCUT_2R, KC_TRNS
#define KM_PLANCK_SCUT_3 KC_TRNS, KM_SCUT_3L, KM_SCUT_3R, KC_TRNS

#define KM_PLANCK_SCUT_THUMB KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KM_SCUT_THUMB, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS

#define PLANCK_LAYOUT_SCUT KM_PLANCK_SCUT_1, KM_PLANCK_SCUT_2, KM_PLANCK_SCUT_3, KM_PLANCK_SCUT_THUMB

#endif // USERSPACE
