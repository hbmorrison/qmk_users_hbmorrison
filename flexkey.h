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

enum fk_layers {
  LAYER_BASE,
  LAYER_SYM_LEFT,
  LAYER_SYM_RIGHT,
  LAYER_FILL_LEFT,
  LAYER_FILL_RIGHT,
  LAYER_NAV,
  LAYER_NUM,
  LAYER_FUNC
};

// Custom keycodes.

enum fk_keycodes {
  M_ALT_TAB = SAFE_RANGE
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

#define KC_SFT_TAB LSFT(KC_TAB)

// Modifier keys in the style of homerow mods.

#define KC_W_ALT LALT_T(KC_W)
#define KC_R_CTL LCTL_T(KC_R)
#define KC_T_SFT LSFT_T(KC_T)
#define KC_P_GUI LGUI_T(KC_P)
#define KC_L_GUI LGUI_T(KC_J)
#define KC_N_SFT LSFT_T(KC_N)
#define KC_I_CTL LCTL_T(KC_I)
#define KC_Y_ALT LALT_T(KC_Y)

// Oneshot keys.

#define KC_OS_SFT OSM(MOD_LSFT)
#define KC_OS_FUNC OSL(LAYER_FUNC)

// Thumbkey layer keys for nav and num layers.

#define KC_SPC_NAV_LAYER LT(LAYER_NAV, KC_SPC)
#define KC_ENT_NUM_LAYER LT(LAYER_NUM, KC_ENT)

// Sym layer keys.

#define KC_SYM_LEFT OSL(LAYER_SYM_LEFT)
#define KC_SYM_RIGHT OSL(LAYER_SYM_RIGHT)

// Fill layer keys.

#define KC_S_FILL_LEFT LT(LAYER_FILL_LEFT, KC_S)
#define KC_E_FILL_RIGHT LT(LAYER_FILL_RIGHT, KC_E)

// Logic for the inner and outer parts of the layout.

#ifdef FK_TOP_OUTER
#define KC_BASE_TL KC_Q, KC_W_ALT
#define KC_BASE_TR KC_Y_ALT, KC_BSPC
#define KC_NAV_Q KC_ESC
#else
#define KC_BASE_TL KC_W_ALT
#define KC_BASE_TR KC_Y_ALT
#define KC_NAV_Q KC_Q
#endif

#ifdef FK_MDL_OUTER
#ifdef FK_BTM_OUTER
#define KC_BASE_ML_OUTER KC_A
#define KC_BASE_MR_OUTER KC_O
#else
#define KC_BASE_ML_OUTER KC_SYM_LEFT
#define KC_BASE_MR_OUTER KC_SYM_RIGHT
#endif
#endif

#ifdef FK_MDL_OUTER
#if defined FK_TOP_OUTER || defined FK_BTM_OUTER
#define KC_BASE_ML KC_BASE_ML_OUTER, KC_R_CTL
#define KC_BASE_MR KC_I_CTL, KC_BASE_MR_OUTER
#else
// If no top or bottom outer key exists, this "middle" key actually ends up on
// the bottom layer.
#define KC_BASE_ML KC_R_CTL
#define KC_BASE_MR KC_I_CTL
#define FK_BTM_OUTER
#endif
#else
#define KC_BASE_ML KC_R_CTL
#define KC_BASE_MR KC_I_CTL
#endif

#ifdef FK_BTM_OUTER
#define KC_BASE_BL KC_SYM_LEFT, KC_X, KC_C
#define KC_BASE_BR KC_COMM, KC_DOT, KC_SYM_RIGHT
#define KC_NAV_Z KC_CAPS
#else
#define KC_BASE_BL KC_SYM_LEFT, KC_C
#define KC_BASE_BR KC_COMM, KC_SYM_RIGHT
#define KC_NAV_Z KC_Z
#endif

#ifdef FK_TOP_INNER
#define KC_BASE_INNER_TL KC_P_GUI, KC_B
#define KC_BASE_INNER_TR KC_J, KC_L_GUI
#else
#define KC_BASE_INNER_TL KC_P_GUI
#define KC_BASE_INNER_TR KC_L_GUI
#endif

#ifdef FK_MDL_INNER
#define KC_BASE_INNER_ML KC_T_SFT, KC_G
#define KC_BASE_INNER_MR KC_M, KC_N_SFT
#else
#define KC_BASE_INNER_ML KC_T_SFT
#define KC_BASE_INNER_MR KC_N_SFT
#endif

#ifdef FK_BTM_INNER
#define KC_BASE_INNER_BL KC_D, KC_V
#define KC_BASE_INNER_BR KC_K, KC_H
#define KC_NAV_V KC_NO
#define KC_NUM_V KC_NO
#else
#define KC_BASE_INNER_BL KC_D
#define KC_BASE_INNER_BR KC_H
#define KC_NAV_V KC_V
#define KC_NUM_K KC_K
#endif

#ifdef FK_THUMB_INNER
#ifdef FK_THUMB_OUTER
#define KM_THUMB_TRNS_L KC_TRNS, KC_TRNS
#define KM_THUMB_TRNS_R KC_TRNS, KC_TRNS
#else
#define KM_THUMB_TRNS_L KC_TRNS
#define KM_THUMB_TRNS_R KC_TRNS
#endif
#define KM_THUMB_TRNS KM_THUMB_TRNS_L, KM_THUMB_TRNS_R
#endif

// Filler for non-base layers and test platform.

#ifdef FK_TOP_OUTER
#define FL_TL KC_NO,
#define FL_TR ,KC_NO
#define TFL_TL
#define TFL_TR
#else
#define FL_TL
#define FL_TR
#define TFL_TL KC_NO,
#define TFL_TR ,KC_NO
#endif

#ifdef FK_MDL_OUTER
#define FL_ML KC_NO,
#define FL_MR ,KC_NO
#define TFL_ML
#define TFL_MR
#else
#define FL_ML
#define FL_MR
#define TFL_ML KC_NO,
#define TFL_MR ,KC_NO
#endif

#ifdef FK_BTM_OUTER
#define FL_BL KC_NO,
#define FL_BR ,KC_NO
#define TFL_BL
#define TFL_BR
#else
#define FL_BL
#define FL_BR
#define TFL_BL KC_NO,
#define TFL_BR ,KC_NO
#endif

#ifdef FK_TOP_INNER
#define FL_TI KC_NO, KC_NO,
#define TFL_TI
#else
#define FL_TI
#define TFL_TI KC_NO, KC_NO,
#endif

#ifdef FK_MDL_INNER
#define FL_MI KC_NO, KC_NO,
#define TFL_MI
#else
#define FL_MI
#define TFL_MI KC_NO, KC_NO,
#endif

#ifdef FK_BTM_INNER
#define FL_BI KC_NO, KC_NO,
#define TFL_BI
#else
#define FL_BI
#define TFL_BI KC_NO, KC_NO,
#endif

#ifdef FK_THUMB_OUTER
#define TFL_THL
#define TFL_THR
#else
#define TFL_THL KC_NO,
#define TFL_THR ,KC_NO
#endif

// Layout macros that allow preprocessor substitutions. Use these instead of the
//standard LAYOUT_ macros in keymap.c code. For example, the following will
//define the base layer for an OLKB Planck ortholinear keyboard: [LAYER_BASE] =
//FK_LAYOUT_planck_grid( FK_LAYOUT_BASE ),

#define FK_LAYOUT_ferris_sweep(...) LAYOUT_split_3x5_2(__VA_ARGS__)

// Base layer for flexkey keyboards.

#define KM_BASE_1L KC_BASE_TL, KC_F, KC_BASE_INNER_TL
#define KM_BASE_2L KC_BASE_ML, KC_S_FILL_LEFT, KC_BASE_INNER_ML
#define KM_BASE_3L KC_BASE_BL, KC_BASE_INNER_BL

#define KM_BASE_1R KC_BASE_INNER_TR, KC_U, KC_BASE_TR
#define KM_BASE_2R KC_BASE_INNER_MR, KC_E_FILL_RIGHT, KC_BASE_MR
#define KM_BASE_3R KC_BASE_INNER_BR, KC_BASE_BR

#define KM_BASE_1 KM_BASE_1L, KM_BASE_1R
#define KM_BASE_2 KM_BASE_2L, KM_BASE_2R
#define KM_BASE_3 KM_BASE_3L, KM_BASE_3R

#ifdef FK_THUMB_INNER
#ifdef FK_THUMB_OUTER
#define KM_THUMB_BASE_L KC_OS_SFT, KC_SPC_NAV_LAYER
#define KM_THUMB_BASE_R KC_ENT_NUM_LAYER, KC_BSPC
#else
#define KM_THUMB_BASE_L KC_SPC_NAV_LAYER
#define KM_THUMB_BASE_R KC_ENT_NUM_LAYER
#endif
#define KM_THUMB_BASE KM_THUMB_BASE_L, KM_THUMB_BASE_R
#endif

#ifdef FK_THUMB_INNER
#define FK_LAYOUT_BASE KM_BASE_1, KM_BASE_2, KM_BASE_3, KM_THUMB_BASE
#else
#define FK_LAYOUT_BASE KM_BASE_1, KM_BASE_2, KM_BASE_3
#endif

// Left fill layer for flexkey keyboards.

#define KM_FILL_LEFT_1L KC_LALT, KC_NO, KC_LGUI
#define KM_FILL_LEFT_2L KC_LCTL, KC_TRNS, KC_LSFT
#define KM_FILL_LEFT_3L KC_NO, KC_NO, KC_NO

#define KM_FILL_LEFT_1R KC_J, KC_NO, KC_BSPC
#define KM_FILL_LEFT_2R KC_M, KC_NO, KC_O
#define KM_FILL_LEFT_3R KC_K, KC_H, KC_SLSH

#define KM_FILL_LEFT_1 FL_TL KM_FILL_LEFT_1L, FL_TI KM_FILL_LEFT_1R FL_TR
#define KM_FILL_LEFT_2 FL_ML KM_FILL_LEFT_2L, FL_MI KM_FILL_LEFT_2R FL_MR
#define KM_FILL_LEFT_3 FL_BL KM_FILL_LEFT_3L, FL_BI KM_FILL_LEFT_3R FL_BR

#ifdef FK_THUMB_INNER
#define FK_LAYOUT_FILL_LEFT KM_FILL_LEFT_1, KM_FILL_LEFT_2, KM_FILL_LEFT_3, KM_THUMB_TRNS
#else
#define FK_LAYOUT_FILL_LEFT KM_FILL_LEFT_1, KM_FILL_LEFT_2, KM_FILL_LEFT_3
#endif

// Right fill layer for flexkey keyboards.

#define KM_FILL_RIGHT_1L KC_Q, KC_NO, KC_B
#define KM_FILL_RIGHT_2L KC_A, KC_TRNS, KC_G
#define KM_FILL_RIGHT_3L KC_Z, KC_D, KC_V

#define KM_FILL_RIGHT_1R KC_LGUI, KC_NO, KC_LALT
#define KM_FILL_RIGHT_2R KC_LSFT, KC_E, KC_LCTL
#define KM_FILL_RIGHT_3R KC_NO, KC_NO, KC_NO

#define KM_FILL_RIGHT_1 FL_TL KM_FILL_RIGHT_1L, FL_TI KM_FILL_RIGHT_1R FL_TR
#define KM_FILL_RIGHT_2 FL_ML KM_FILL_RIGHT_2L, FL_MI KM_FILL_RIGHT_2R FL_MR
#define KM_FILL_RIGHT_3 FL_BL KM_FILL_RIGHT_3L, FL_BI KM_FILL_RIGHT_3R FL_BR

#ifdef FK_THUMB_INNER
#define FK_LAYOUT_FILL_RIGHT KM_FILL_RIGHT_1, KM_FILL_RIGHT_2, KM_FILL_RIGHT_3, KM_THUMB_TRNS
#else
#define FK_LAYOUT_FILL_RIGHT KM_FILL_RIGHT_1, KM_FILL_RIGHT_2, KM_FILL_RIGHT_3
#endif

// Left symbol layer for flexkey keyboards.

#define KM_SYM_LEFT_1L KC_EXLM, KC_QUES, KC_CIRC
#define KM_SYM_LEFT_2L KC_GRV, KC_PLUS, KC_COLN
#define KM_SYM_LEFT_3L KC_X, KC_EQL, KC_SCLN

#define KM_SYM_LEFT_1R KC_AMPR, KC_ASTR, KC_OS_FUNC
#define KM_SYM_LEFT_2R KC_UK_AT, KC_UK_TILDE, KC_UNDS
#define KM_SYM_LEFT_3R KC_QUOT, KC_UK_HASH, KC_MINS

#define KM_SYM_LEFT_1 FL_TL KM_SYM_LEFT_1L, FL_TI KM_SYM_LEFT_1R FL_TR
#define KM_SYM_LEFT_2 FL_ML KM_SYM_LEFT_2L, FL_MI KM_SYM_LEFT_2R FL_MR
#define KM_SYM_LEFT_3 FL_BL KM_SYM_LEFT_3L, FL_BI KM_SYM_LEFT_3R FL_BR

#ifdef FK_THUMB_INNER
#define FK_LAYOUT_SYM_LEFT KM_SYM_LEFT_1, KM_SYM_LEFT_2, KM_SYM_LEFT_3, KM_THUMB_TRNS
#else
#define FK_LAYOUT_SYM_LEFT KM_SYM_LEFT_1, KM_SYM_LEFT_2, KM_SYM_LEFT_3
#endif

// Right symbol layer for flexkey keyboards.

#define KM_SYM_RIGHT_1L KC_UK_DQUO, KC_UK_PND, KC_DLR
#define KM_SYM_RIGHT_2L KC_UK_PIPE, KC_LBRC, KC_LCBR
#define KM_SYM_RIGHT_3L KC_UK_BSLS, KC_RBRC, KC_RCBR

#define KM_SYM_RIGHT_1R KC_PERC, KC_NO, KC_NO
#define KM_SYM_RIGHT_2R KC_LPRN, KC_LT, KC_NO
#define KM_SYM_RIGHT_3R KC_RPRN, KC_GT, KC_DOT

#define KM_SYM_RIGHT_1 FL_TL KM_SYM_RIGHT_1L, FL_TI KM_SYM_RIGHT_1R FL_TR
#define KM_SYM_RIGHT_2 FL_ML KM_SYM_RIGHT_2L, FL_MI KM_SYM_RIGHT_2R FL_MR
#define KM_SYM_RIGHT_3 FL_BL KM_SYM_RIGHT_3L, FL_BI KM_SYM_RIGHT_3R FL_BR

#ifdef FK_THUMB_INNER
#define FK_LAYOUT_SYM_RIGHT KM_SYM_RIGHT_1, KM_SYM_RIGHT_2, KM_SYM_RIGHT_3, KM_THUMB_TRNS
#else
#define FK_LAYOUT_SYM_RIGHT KM_SYM_RIGHT_1, KM_SYM_RIGHT_2, KM_SYM_RIGHT_3
#endif

// Navigation layer for flexkey keyboards.

#define KM_NAV_1L KC_NAV_Q, KC_PSCR, KC_NO
#define KM_NAV_2L KC_TAB, M_ALT_TAB, KC_NO
#define KM_NAV_3L KC_NAV_Z, KC_PGDN, KC_NAV_V

#define KM_NAV_1R LCTL(LGUI(KC_LEFT)), KC_UP, LCTL(LGUI(KC_RGHT))
#define KM_NAV_2R KC_LEFT, KC_DOWN, KC_RGHT
#define KM_NAV_3R KC_HOME, KC_PGUP, KC_END

#define KM_NAV_1 FL_TL KM_NAV_1L, FL_TI KM_NAV_1R FL_TR
#define KM_NAV_2 FL_ML KM_NAV_2L, FL_MI KM_NAV_2R FL_MR
#define KM_NAV_3 FL_BL KM_NAV_3L, FL_BI KM_NAV_3R FL_BR

#ifdef FK_THUMB_INNER
#ifdef FK_THUMB_OUTER
#define KM_THUMB_NAV_L KC_NO, KC_TRNS
#define KM_THUMB_NAV_R KC_ESC, KC_NO
#else
#define KM_THUMB_NAV_L KC_TRNS
#define KM_THUMB_NAV_R KC_ESC
#endif
#define KM_THUMB_NAV KM_THUMB_NAV_L, KM_THUMB_NAV_R
#endif

#ifdef FK_THUMB_INNER
#define FK_LAYOUT_NAV KM_NAV_1, KM_NAV_2, KM_NAV_3, KM_THUMB_NAV
#else
#define FK_LAYOUT_NAV KM_NAV_1, KM_NAV_2, KM_NAV_3
#endif

// Number layer for flexkey keyboards.

#define KM_NUM_1L KC_1, KC_2, KC_3
#define KM_NUM_2L KC_4, KC_5, KC_6
#define KM_NUM_3L KC_7, KC_8, KC_9

#define KM_NUM_1R KC_NO, KC_DEL, KC_BSPC
#define KM_NUM_2R KC_NO, KC_0, KC_NO
#define KM_NUM_3R KC_NUM_K, KC_DOT, KC_SLSH

#define KM_NUM_1 FL_TL KM_NUM_1L, FL_TI KM_NUM_1R FL_TR
#define KM_NUM_2 FL_ML KM_NUM_2L, FL_MI KM_NUM_2R FL_MR
#define KM_NUM_3 FL_BL KM_NUM_3L, FL_BI KM_NUM_3R FL_BR

#ifdef FK_THUMB_INNER
#ifdef FK_THUMB_OUTER
#define KM_THUMB_NUM_L KC_SFT_TAB, KC_TAB
#define KM_THUMB_NUM_R KC_TRNS, KC_NO
#else
#define KM_THUMB_NUM_L KC_TAB
#define KM_THUMB_NUM_R KC_TRNS
#endif
#define KM_THUMB_NUM KM_THUMB_NUM_L, KM_THUMB_NUM_R
#endif

#ifdef FK_THUMB_INNER
#define FK_LAYOUT_NUM KM_NUM_1, KM_NUM_2, KM_NUM_3, KM_THUMB_NUM
#else
#define FK_LAYOUT_NUM KM_NUM_1, KM_NUM_2, KM_NUM_3
#endif

// Shortcut layer for flexkey layouts.

#define KM_FUNC_1L KC_F1, KC_F2, KC_F3
#define KM_FUNC_2L KC_F4, KC_F5, KC_F6
#define KM_FUNC_3L KC_F7, KC_F8, KC_F9

#define KM_FUNC_1R KC_BRIU, KC_VOLU, KC_NO
#define KM_FUNC_2R KC_BRID, KC_VOLD, KC_MUTE
#define KM_FUNC_3R KC_F10, KC_F11, KC_F12

#define KM_FUNC_1 FL_TL KM_FUNC_1L, FL_TI KM_FUNC_1R FL_TR
#define KM_FUNC_2 FL_ML KM_FUNC_2L, FL_MI KM_FUNC_2R FL_MR
#define KM_FUNC_3 FL_BL KM_FUNC_3L, FL_BI KM_FUNC_3R FL_BR

#ifdef FK_THUMB_INNER
#define FK_LAYOUT_FUNC KM_FUNC_1, KM_FUNC_2, KM_FUNC_3, KM_THUMB_TRNS
#else
#define FK_LAYOUT_FUNC KM_FUNC_1, KM_FUNC_2, KM_FUNC_3
#endif

// Ferris sweep test platform - base layer.

#define KM_TEST_BASE_1 TFL_TL KM_BASE_1L, TFL_TI KM_BASE_1R TFL_TR
#define KM_TEST_BASE_2 TFL_ML KM_BASE_2L, TFL_MI KM_BASE_2R TFL_MR
#define KM_TEST_BASE_3 TFL_BL KM_BASE_3L, TFL_BI KM_BASE_3R TFL_BR

#ifdef FK_THUMB_INNER
#define KM_TEST_THUMB_BASE TFL_THL KM_THUMB_BASE_L, KM_THUMB_BASE_R TFL_THR
#else
#define KM_TEST_THUMB_BASE KC_NO, KC_NO, KC_NO, KC_NO
#endif

#define FK_TEST_LAYOUT_BASE KM_TEST_BASE_1, KM_TEST_BASE_2, KM_TEST_BASE_3, KM_TEST_THUMB_BASE

// Ferris sweep test platform - left fill layer.

#define KM_TEST_FILL_LEFT_1 TFL_TL KM_FILL_LEFT_1L, TFL_TI KM_FILL_LEFT_1R TFL_TR
#define KM_TEST_FILL_LEFT_2 TFL_ML KM_FILL_LEFT_2L, TFL_MI KM_FILL_LEFT_2R TFL_MR
#define KM_TEST_FILL_LEFT_3 TFL_BL KM_FILL_LEFT_3L, TFL_BI KM_FILL_LEFT_3R TFL_BR

#ifdef FK_THUMB_INNER
#define KM_TEST_THUMB_FILL_LEFT TFL_THL KM_THUMB_FILL_LEFT_L, KM_THUMB_FILL_LEFT_R TFL_THR
#else
#define KM_TEST_THUMB_FILL_LEFT KC_NO, KC_NO, KC_NO, KC_NO
#endif

#define FK_TEST_LAYOUT_FILL_LEFT KM_TEST_FILL_LEFT_1, KM_TEST_FILL_LEFT_2, KM_TEST_FILL_LEFT_3, KM_TEST_THUMB_FILL_LEFT

// Ferris sweep test platform - right fill layer.

#define KM_TEST_FILL_RIGHT_1 TFL_TL KM_FILL_RIGHT_1L, TFL_TI KM_FILL_RIGHT_1R TFL_TR
#define KM_TEST_FILL_RIGHT_2 TFL_ML KM_FILL_RIGHT_2L, TFL_MI KM_FILL_RIGHT_2R TFL_MR
#define KM_TEST_FILL_RIGHT_3 TFL_BL KM_FILL_RIGHT_3L, TFL_BI KM_FILL_RIGHT_3R TFL_BR

#ifdef FK_THUMB_INNER
#define KM_TEST_THUMB_FILL_RIGHT TFL_THL KM_THUMB_FILL_RIGHT_L, KM_THUMB_FILL_RIGHT_R TFL_THR
#else
#define KM_TEST_THUMB_FILL_RIGHT KC_NO, KC_NO, KC_NO, KC_NO
#endif

#define FK_TEST_LAYOUT_FILL_RIGHT KM_TEST_FILL_RIGHT_1, KM_TEST_FILL_RIGHT_2, KM_TEST_FILL_RIGHT_3, KM_TEST_THUMB_FILL_RIGHT

// Ferris sweep test platform - left symbol layer.

#define KM_TEST_SYM_LEFT_1 TFL_TL KM_SYM_LEFT_1L, TFL_TI KM_SYM_LEFT_1R TFL_TR
#define KM_TEST_SYM_LEFT_2 TFL_ML KM_SYM_LEFT_2L, TFL_MI KM_SYM_LEFT_2R TFL_MR
#define KM_TEST_SYM_LEFT_3 TFL_BL KM_SYM_LEFT_3L, TFL_BI KM_SYM_LEFT_3R TFL_BR

#ifdef FK_THUMB_INNER
#define KM_TEST_THUMB_SYM_LEFT TFL_THL KM_THUMB_SYM_LEFT_L, KM_THUMB_SYM_LEFT_R TFL_THR
#else
#define KM_TEST_THUMB_SYM_LEFT KC_NO, KC_NO, KC_NO, KC_NO
#endif

#define FK_TEST_LAYOUT_SYM_LEFT KM_TEST_SYM_LEFT_1, KM_TEST_SYM_LEFT_2, KM_TEST_SYM_LEFT_3, KM_TEST_THUMB_SYM_LEFT

// Ferris sweep test platform - right symbol layer.

#define KM_TEST_SYM_RIGHT_1 TFL_TL KM_SYM_RIGHT_1L, TFL_TI KM_SYM_RIGHT_1R TFL_TR
#define KM_TEST_SYM_RIGHT_2 TFL_ML KM_SYM_RIGHT_2L, TFL_MI KM_SYM_RIGHT_2R TFL_MR
#define KM_TEST_SYM_RIGHT_3 TFL_BL KM_SYM_RIGHT_3L, TFL_BI KM_SYM_RIGHT_3R TFL_BR

#ifdef FK_THUMB_INNER
#define KM_TEST_THUMB_SYM_RIGHT TFL_THL KM_THUMB_SYM_RIGHT_L, KM_THUMB_SYM_RIGHT_R TFL_THR
#else
#define KM_TEST_THUMB_SYM_RIGHT KC_NO, KC_NO, KC_NO, KC_NO
#endif

#define FK_TEST_LAYOUT_SYM_RIGHT KM_TEST_SYM_RIGHT_1, KM_TEST_SYM_RIGHT_2, KM_TEST_SYM_RIGHT_3, KM_TEST_THUMB_SYM_RIGHT

// Ferris sweep test platform - navigation layer.

#define KM_TEST_NAV_1 TFL_TL KM_NAV_1L, TFL_TI KM_NAV_1R TFL_TR
#define KM_TEST_NAV_2 TFL_ML KM_NAV_2L, TFL_MI KM_NAV_2R TFL_MR
#define KM_TEST_NAV_3 TFL_BL KM_NAV_3L, TFL_BI KM_NAV_3R TFL_BR

#ifdef FK_THUMB_INNER
#define KM_TEST_THUMB_NAV TFL_THL KM_THUMB_NAV_L, KM_THUMB_NAV_R TFL_THR
#else
#define KM_TEST_THUMB_NAV KC_NO, KC_NO, KC_NO, KC_NO
#endif

#define FK_TEST_LAYOUT_NAV KM_TEST_NAV_1, KM_TEST_NAV_2, KM_TEST_NAV_3, KM_TEST_THUMB_NAV

// Ferris sweep test platform - number layer.

#define KM_TEST_NUM_1 TFL_TL KM_NUM_1L, TFL_TI KM_NUM_1R TFL_TR
#define KM_TEST_NUM_2 TFL_ML KM_NUM_2L, TFL_MI KM_NUM_2R TFL_MR
#define KM_TEST_NUM_3 TFL_BL KM_NUM_3L, TFL_BI KM_NUM_3R TFL_BR

#ifdef FK_THUMB_INNER
#define KM_TEST_THUMB_NUM TFL_THL KM_THUMB_NUM_L, KM_THUMB_NUM_R TFL_THR
#else
#define KM_TEST_THUMB_NUM KC_NO, KC_NO, KC_NO, KC_NO
#endif

#define FK_TEST_LAYOUT_NUM KM_TEST_NUM_1, KM_TEST_NUM_2, KM_TEST_NUM_3, KM_TEST_THUMB_NUM

// Ferris sweep test platform - function layer.

#define KM_TEST_FUNC_1 TFL_TL KM_FUNC_1L, TFL_TI KM_FUNC_1R TFL_TR
#define KM_TEST_FUNC_2 TFL_ML KM_FUNC_2L, TFL_MI KM_FUNC_2R TFL_MR
#define KM_TEST_FUNC_3 TFL_BL KM_FUNC_3L, TFL_BI KM_FUNC_3R TFL_BR

#ifdef FK_THUMB_INNER
#define KM_TEST_THUMB_FUNC TFL_THL KM_THUMB_FUNC_L, KM_THUMB_FUNC_R TFL_THR
#else
#define KM_TEST_THUMB_FUNC KC_NO, KC_NO, KC_NO, KC_NO
#endif

#define FK_TEST_LAYOUT_FUNC KM_TEST_FUNC_1, KM_TEST_FUNC_2, KM_TEST_FUNC_3, KM_TEST_THUMB_FUNC

#endif // USERSPACE
