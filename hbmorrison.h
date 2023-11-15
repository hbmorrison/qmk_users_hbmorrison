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

#define LAYER_ENT_HOLD LAYER_NUM
#define LAYER_SPC_HOLD LAYER_NAV
#define LAYER_SLSH_TAP LAYER_SCUT
#define LAYER_SLSH_HOLD LAYER_NUM
#define LAYER_Z_TAP LAYER_SYM
#define LAYER_Z_HOLD LAYER_NAV

// Custom keycodes.

enum hbm_keycodes {
    M_ALT_TAB = SAFE_RANGE,
    M_APP1,
    M_APP2,
    M_APP3,
    M_APP4,
    M_APP5,
    M_APP6,
    M_APP7,
    M_APP8,
    M_1PASS,
    M_HIDE,
    M_MAX,
    M_CLOSE,
    M_NTRM,
    M_NDESK,
    M_PDESK,
    M_EMOJI,
    M_DDS,
    M_ESC_COLN,
    M_4RAND,
    M_ISCROS,
    M_ISWIN,
#ifdef HBM_HOMEROW_ENABLE
    // Dummy keycodes for MOD_T() overrides in the nav layer.
    DUMMY_CTL_X,
    DUMMY_CTL_C,
    DUMMY_SFT_CTL_C,
    DUMMY_CTL_V
#endif // HBM_HOMEROW_ENABLE
};

// Tap dance keys.

#ifdef TAP_DANCE_ENABLE
enum {
  TD_ENT_LAYER,
  TD_SPC_LAYER,
  TD_SLSH_LAYER,
  TD_Z_LAYER,
  TD_NAVX_LAYER
};
#endif // TAP_DANCE_ENABLE

// Alternative keys for UK ISO keyboard layouts.

#define KC_UK_DQUO LSFT(KC_2)
#define KC_UK_PND LSFT(KC_3)
#define KC_UK_PIPE LSFT(KC_NUBS)
#define KC_UK_BSLS KC_NUBS
#define KC_UK_AT LSFT(KC_QUOT)
#define KC_UK_TILDE LSFT(KC_BSLS)
#define KC_UK_HASH KC_BSLS

// Modified keys.

#define KC_CTL_X LCTL(KC_X)
#define KC_CTL_C LCTL(KC_C)
#define KC_CTL_V LCTL(KC_V)
#define KC_SFT_TAB LSFT(KC_TAB)
#define KC_CTL_TAB LCTL(KC_TAB)
#define KC_SFT_CTL_C LSFT(LCTL(KC_C))

// Modifier keys in the style of homerow mods.

#ifdef HBM_HOMEROW_ENABLE
#define KC_X_SFTALT LSA_T(KC_X)
#define KC_C_ALT LALT_T(KC_C)
#define KC_D_CTL LCTL_T(KC_D)
#define KC_V_SFTCTL C_S_T(KC_V)
#define KC_G_GUI LGUI_T(KC_G)
#define KC_M_GUI LGUI_T(KC_M)
#define KC_K_SFTCTL C_S_T(KC_K)
#define KC_H_CTL LCTL_T(KC_H)
#define KC_COMM_ALT LALT_T(KC_COMM)
#define KC_DOT_SFTALT LSA_T(KC_DOT)
#else // if ! HBM_HOMEROW_ENABLE
#define KC_X_SFTALT KC_X
#define KC_C_ALT KC_C
#define KC_D_CTL KC_D
#define KC_V_SFTCTL KC_V
#define KC_G_GUI KC_G
#define KC_M_GUI KC_M
#define KC_K_SFTCTL KC_K
#define KC_H_CTL KC_H
#define KC_COMM_ALT KC_COMM
#define KC_DOT_SFTALT KC_DOT
#endif // HBM_HOMEROW_ENABLE

// Modifier keys for the navigation layer. These issue Ctrl-X, Ctrl-C,
// Shift-Ctrl-C and Ctrl-V respectively when tapped and hold down Shift-Alt,
// Alt, Ctrl and Shift-Ctrl modifiers respectively when held, matching the base
// layer homerow mods. <MOD>_T() macros only work with 8-bit values so, for
// example, to issue LCTL(KC_C) the LALT_T(LCTL(KC_C)) should be overriden on
// process_record_user() to issue the correct 16-bit keycode for Ctrl-C.
// However, this would also unintentionally override the LALT_T(KC_C) that
// appears in the base layer, again because the macro only supports 8-bit
// values, so a dummy keycode is used here and in process_record_user() to
// uniquely identify these specific keypresses.

#ifdef HBM_HOMEROW_ENABLE
#define KC_CTL_X_SFTALT LSA_T(DUMMY_CTL_X)
#define KC_CTL_C_ALT LALT_T(DUMMY_CTL_C)
#define KC_SFT_CTL_C_CTL LCTL_T(DUMMY_SFT_CTL_C)
#define KC_CTL_V_SFTCTL C_S_T(DUMMY_CTL_V)
#else // if ! HBM_HOMEROW_ENABLE
#define KC_CTL_X_SFTALT LCTL(KC_X)
#define KC_CTL_C_ALT LCTL(KC_C)
#define KC_SFT_CTL_C_CTL LSFT(LCTL(KC_C))
#define KC_CTL_V_SFTCTL LCTL(KC_V)
#endif // HBM_HOMEROW_ENABLE

// Oneshot keys.

#define KC_OS_SFT OSM(MOD_LSFT)
#define KC_OS_CTL OSM(MOD_LCTL)
#define KC_OS_ALT OSM(MOD_LALT)
#define KC_OS_GUI OSM(MOD_LGUI)
#define KC_OS_SYM OSL(LAYER_SYM)
#define KC_OS_NAV OSL(LAYER_NAV)
#define KC_OS_NUM OSL(LAYER_NUM)
#define KC_OS_SCUT OSL(LAYER_SCUT)

// Optionally enable thumbkey layer keys.

#ifndef HBM_THUMBKEY_ENABLE
#define KC_SPC_LAYER KC_SPC
#define KC_ENT_LAYER KC_ENT
#else // if HBM_THUMBKEY_ENABLE
#ifdef TAP_DANCE_ENABLE
#define KC_SPC_LAYER TD(TD_SPC_LAYER)
#define KC_ENT_LAYER TD(TD_ENT_LAYER)
#else
#define KC_SPC_LAYER LT(LAYER_SPC_HOLD, KC_SPC)
#define KC_ENT_LAYER LT(LAYER_ENT_HOLD, KC_ENT)
#endif // TAP_DANCE_ENABLE
#endif // HBM_THUMBKEY_ENABLE

// Optionally enable sidekey layer keys.

#ifndef HBM_SIDEKEY_ENABLE
#define KC_Z_LAYER KC_Z
#define KC_SLSH_LAYER KC_SLSH
#else // if HBM_SIDEKEY_ENABLE
#ifdef TAP_DANCE_ENABLE
#define KC_Z_LAYER TD(TD_Z_LAYER)
#define KC_SLSH_LAYER TD(TD_SLSH_LAYER)
#else // if ! TAP_DANCE_ENABLE
#define KC_Z_LAYER OSL(LAYER_Z_HOLD)
#define KC_SLSH_LAYER OSL(LAYER_SLSH_HOLD)
#endif // TAP_DANCE_ENABLE
#endif // HBM_SIDEKEY_ENABLE

#if defined HBM_THUMBKEY_ENABLE || defined HBM_SIDEKEY_ENABLE
#define KC_NAV_LAYER KC_Z
#define KC_NUM_LAYER KC_SLSH
#else // if ! defined HBM_THUMBKEY_ENABLE || defined HBM_SIDEKEY_ENABLE
#define KC_NAV_LAYER MO(LAYER_NAV)
#define KC_NUM_LAYER MO(LAYER_NUM)
#endif // defined HBM_THUMBKEY_ENABLE || defined HBM_SIDEKEY_ENABLE

// Replace mousewheel scroll keys with page up and down if mouse keys are not
// enabled.

#ifdef MOUSEKEY_ENABLE
#define KC_SCROLL_UP KC_MS_WH_UP
#define KC_SCROLL_DN KC_MS_WH_DOWN
#else // if ! MOUSEKEY_ENABLE
#define KC_SCROLL_UP KC_PAGE_UP
#define KC_SCROLL_DN KC_PAGE_DOWN
#endif // MOUSEKEY_ENABLE

// Layout macros that allow preprocessor substitutions. Use these instead of the
// standard LAYOUT_ macros in keymap.c code. For example, the following will
// define the base layer for an OLKB Planck ortholinear keyboard:
//   [LAYER_BASE] = HBM_LAYOUT_planck_grid( HBM_LAYOUT_4x12_BASE ),

#define HBM_LAYOUT_split_3x5_2(...) LAYOUT_split_3x5_2(__VA_ARGS__)
#define HBM_LAYOUT_split_3x6_3(...) LAYOUT_split_3x6_3(__VA_ARGS__)
#define HBM_LAYOUT_planck_grid(...) LAYOUT_planck_grid(__VA_ARGS__)

// Base layer for five column split keyboards.

#define KM_5_BASE_1L KC_Q, KC_W, KC_F, KC_P, KC_B
#define KM_5_BASE_2L KC_A, KC_R, KC_S, KC_T, KC_G_GUI
#define KM_5_BASE_3L KC_Z_LAYER, KC_X_SFTALT, KC_C_ALT, KC_D_CTL, KC_V_SFTCTL

#define KM_5_BASE_1R KC_J, KC_L, KC_U, KC_Y, KC_BSPC
#define KM_5_BASE_2R KC_M_GUI, KC_N, KC_E, KC_I, KC_O
#define KM_5_BASE_3R KC_K_SFTCTL, KC_H_CTL, KC_COMM_ALT, KC_DOT_SFTALT, KC_SLSH_LAYER

#define KM_2THUMB_BASE_L KC_OS_SFT, KC_SPC_LAYER
#define KM_2THUMB_BASE_R KC_ENT_LAYER, KC_OS_SYM

#define KM_5_BASE_1 KM_5_BASE_1L, KM_5_BASE_1R
#define KM_5_BASE_2 KM_5_BASE_2L, KM_5_BASE_2R
#define KM_5_BASE_3 KM_5_BASE_3L, KM_5_BASE_3R
#define KM_2THUMB_BASE KM_2THUMB_BASE_L, KM_2THUMB_BASE_R

#define HBM_LAYOUT_3x5_2_BASE KM_5_BASE_1, KM_5_BASE_2, KM_5_BASE_3, KM_2THUMB_BASE

// Symbol layer for five column split keyboards.

#define KM_5_SYM_1L KC_EXLM, KC_UK_DQUO, KC_UK_PND, KC_DLR, KC_PERC
#define KM_5_SYM_2L KC_GRV, KC_UK_PIPE, KC_LBRC, KC_LCBR, KC_LPRN
#define KM_5_SYM_3L KC_NAV_LAYER, KC_UK_BSLS, KC_RBRC, KC_RCBR, KC_RPRN

#define KM_5_SYM_1R KC_CIRC, KC_AMPR, KC_ASTR, KC_UNDS, KC_PLUS
#define KM_5_SYM_2R KC_COLN, KC_UK_AT, KC_UK_TILDE, KC_MINS, KC_EQL
#define KM_5_SYM_3R KC_SCLN, KC_QUOT, KC_UK_HASH, KC_GT, KC_NUM_LAYER

#ifndef HBM_THUMBKEY_ENABLE
#define KM_2THUMB_SYM_L KC_ESC, KC_TAB
#else // if HBM_THUMBKEY_ENABLE
#define KM_2THUMB_SYM_L KC_NO, KC_NO
#endif // HBM_THUMBKEY_ENABLE
#define KM_2THUMB_SYM_R KC_NO, KC_OS_SCUT

#define KM_5_SYM_1 KM_5_SYM_1L, KM_5_SYM_1R
#define KM_5_SYM_2 KM_5_SYM_2L, KM_5_SYM_2R
#define KM_5_SYM_3 KM_5_SYM_3L, KM_5_SYM_3R
#define KM_2THUMB_SYM KM_2THUMB_SYM_L, KM_2THUMB_SYM_R

#define HBM_LAYOUT_3x5_2_SYM KM_5_SYM_1, KM_5_SYM_2, KM_5_SYM_3, KM_2THUMB_SYM

// Navigation layer for five column split keyboards.

#define KM_5_NAV_1L KC_PSCR, KC_MNXT, KC_MPLY, KC_VOLU, KC_BRIU
#define KM_5_NAV_2L KC_NO, KC_MPRV, KC_MUTE, KC_VOLD, KC_BRID
#define KM_5_NAV_3L KC_Z, KC_CTL_X_SFTALT, KC_CTL_C_ALT, KC_SFT_CTL_C_CTL, KC_CTL_V_SFTCTL

#define KM_5_NAV_1R KC_NO, M_PDESK, KC_CTL_TAB, M_ALT_TAB, M_NDESK
#define KM_5_NAV_2R KC_SCROLL_UP, KC_LEFT, KC_DOWN, KC_UP, KC_RGHT
#define KM_5_NAV_3R KC_SCROLL_DN, KC_HOME, KC_PGDN, KC_PGUP, KC_END

#define KM_2THUMB_NAV_L KC_NO, KC_TRNS
#ifdef HBM_THUMBKEY_ENABLE
#define KM_2THUMB_NAV_R KC_ESC, KC_NO
#else // if ! HBM_THUMBKEY_ENABLE
#define KM_2THUMB_NAV_R KC_NO, KC_NO
#endif // HBM_THUMBKEY_ENABLE

#define KM_5_NAV_1 KM_5_NAV_1L, KM_5_NAV_1R
#define KM_5_NAV_2 KM_5_NAV_2L, KM_5_NAV_2R
#define KM_5_NAV_3 KM_5_NAV_3L, KM_5_NAV_3R
#define KM_2THUMB_NAV KM_2THUMB_NAV_L, KM_2THUMB_NAV_R

#define HBM_LAYOUT_3x5_2_NAV KM_5_NAV_1, KM_5_NAV_2, KM_5_NAV_3, KM_2THUMB_NAV

// Number layer for five column split keyboards.

#define KM_5_NUM_1L KC_PAST, KC_1, KC_2, KC_3, KC_PPLS
#define KM_5_NUM_2L KC_PSLS, KC_4, KC_5, KC_6, KC_PMNS
#define KM_5_NUM_3L KC_0, KC_7, KC_8, KC_9, KC_DOT

#define KM_5_NUM_1R KC_NO, KC_F1, KC_F2, KC_F3, KC_BSPC
#define KM_5_NUM_2R KC_NO, KC_F4, KC_F5, KC_F6, KC_NO
#define KM_5_NUM_3R KC_F10, KC_F7, KC_F8, KC_F9, KC_SLSH

#ifdef HBM_THUMBKEY_ENABLE
#define KM_2THUMB_NUM_L KC_SFT_TAB, KC_TAB
#else // if ! HBM_THUMBKEY_ENABLE
#define KM_2THUMB_NUM_L KC_NO, KC_NO
#endif // HBM_THUMBKEY_ENABLE
#define KM_2THUMB_NUM_R KC_TRNS, KC_NO

#define KM_5_NUM_1 KM_5_NUM_1L, KM_5_NUM_1R
#define KM_5_NUM_2 KM_5_NUM_2L, KM_5_NUM_2R
#define KM_5_NUM_3 KM_5_NUM_3L, KM_5_NUM_3R
#define KM_2THUMB_NUM KM_2THUMB_NUM_L, KM_2THUMB_NUM_R

#define HBM_LAYOUT_3x5_2_NUM KM_5_NUM_1, KM_5_NUM_2, KM_5_NUM_3, KM_2THUMB_NUM

// Shortcut layer for five column split layouts.

#define KM_5_SCUT_1L M_ISWIN, M_ISCROS, KC_NO, M_4RAND, KC_NO
#define KM_5_SCUT_2L M_APP1, M_APP2, M_APP3, M_APP4, KC_NO
#define KM_5_SCUT_3L KC_NO, M_NTRM, KC_NO, M_1PASS, KC_NO

#define KM_5_SCUT_1R M_EMOJI, M_HIDE, M_MAX, M_CLOSE, KC_NO
#define KM_5_SCUT_2R M_ESC_COLN, M_APP5, M_APP6, M_APP7, M_APP8
#define KM_5_SCUT_3R KC_NO, KC_NO, KC_NO, M_DDS, KC_SLSH

#define KM_2THUMB_SCUT_L KC_CAPS, CW_TOGG
#define KM_2THUMB_SCUT_R KC_NO, KC_TRNS

#define KM_5_SCUT_1 KM_5_SCUT_1L, KM_5_SCUT_1R
#define KM_5_SCUT_2 KM_5_SCUT_2L, KM_5_SCUT_2R
#define KM_5_SCUT_3 KM_5_SCUT_3L, KM_5_SCUT_3R
#define KM_2THUMB_SCUT KM_2THUMB_SCUT_L, KM_2THUMB_SCUT_R

#define HBM_LAYOUT_3x5_2_SCUT KM_5_SCUT_1, KM_5_SCUT_2, KM_5_SCUT_3, KM_2THUMB_SCUT

// Base layer for six column split layouts.

#define KM_6_BASE_1R KC_J, KC_L, KC_U, KC_Y, KC_DEL, KC_BSPC

#define KM_3THUMB_BASE_L KC_OS_SFT, KC_SPC_LAYER, KC_TAB
#define KM_3THUMB_BASE_R KC_ESC, KC_ENT_LAYER, KC_OS_SYM

#define KM_6_BASE_1 M_ESC_COLN, KM_5_BASE_1L, KM_6_BASE_1R
#define KM_6_BASE_2 KC_ESC, KM_5_BASE_2L, KM_5_BASE_2R, KC_TAB
#define KM_6_BASE_3 KC_OS_SYM, KM_5_BASE_3L, KM_5_BASE_3R, KC_OS_SCUT
#define KM_3THUMB_BASE KM_3THUMB_BASE_L, KM_3THUMB_BASE_R

#define HBM_LAYOUT_3x6_3_BASE KM_6_BASE_1, KM_6_BASE_2, KM_6_BASE_3, KM_3THUMB_BASE

// Symbol layer for six column split layouts.

#define KM_6_SYM_1 KC_TRNS, KM_5_SYM_1L, KM_5_SYM_1R, KC_TRNS
#define KM_6_SYM_2 KC_TRNS, KM_5_SYM_2L, KM_5_SYM_2R, KC_TRNS
#define KM_6_SYM_3 KC_TRNS, KM_5_SYM_3L, KM_5_SYM_3R, KC_TRNS

#define KM_3THUMB_SYM_L KM_2THUMB_SYM_L, KC_TRNS
#define KM_3THUMB_SYM_R KC_TRNS, KM_2THUMB_SYM_R

#define KM_3THUMB_SYM KM_3THUMB_SYM_L, KM_3THUMB_SYM_R

#define HBM_LAYOUT_3x6_3_SYM KM_6_SYM_1, KM_6_SYM_2, KM_6_SYM_3, KM_3THUMB_SYM

// Navigation layer for six column split layouts.

#define KM_6_NAV_1 KC_TRNS, KM_5_NAV_1L, KM_5_NAV_1R, KC_TRNS
#define KM_6_NAV_2 KC_TRNS, KM_5_NAV_2L, KM_5_NAV_2R, KC_TRNS
#define KM_6_NAV_3 KC_TRNS, KM_5_NAV_3L, KM_5_NAV_3R, KC_TRNS

#define KM_3THUMB_NAV_L KM_2THUMB_NAV_L, KC_TRNS
#define KM_3THUMB_NAV_R KC_TRNS, KM_2THUMB_NAV_R

#define KM_3THUMB_NAV KM_3THUMB_NAV_L, KM_3THUMB_NAV_R

#define HBM_LAYOUT_3x6_3_NAV KM_6_NAV_1, KM_6_NAV_2, KM_6_NAV_3, KM_3THUMB_NAV

// Number layer for six column split layouts.

#define KM_6_NUM_1 KC_TRNS, KM_5_NUM_1L, KM_5_NUM_1R, KC_TRNS
#define KM_6_NUM_2 KC_TRNS, KM_5_NUM_2L, KM_5_NUM_2R, KC_TRNS
#define KM_6_NUM_3 KC_TRNS, KM_5_NUM_3L, KM_5_NUM_3R, KC_TRNS

#define KM_3THUMB_NUM_L KM_2THUMB_NUM_L, KC_TRNS
#define KM_3THUMB_NUM_R KC_TRNS, KM_2THUMB_NUM_R

#define KM_3THUMB_NUM KM_3THUMB_NUM_L, KM_3THUMB_NUM_R

#define HBM_LAYOUT_3x6_3_NUM KM_6_NUM_1, KM_6_NUM_2, KM_6_NUM_3, KM_3THUMB_NUM

// Shortcut layer for six column split layouts.

#define KM_6_SCUT_1 KC_TRNS, KM_5_SCUT_1L, KM_5_SCUT_1R, KC_TRNS
#define KM_6_SCUT_2 KC_TRNS, KM_5_SCUT_2L, KM_5_SCUT_2R, KC_TRNS
#define KM_6_SCUT_3 KC_TRNS, KM_5_SCUT_3L, KM_5_SCUT_3R, KC_TRNS

#define KM_3THUMB_SCUT_L KM_2THUMB_SCUT_L, KC_TRNS
#define KM_3THUMB_SCUT_R KC_TRNS, KM_2THUMB_SCUT_R

#define KM_3THUMB_SCUT KM_3THUMB_SCUT_L, KM_3THUMB_SCUT_R

#define HBM_LAYOUT_3x6_3_SCUT KM_6_SCUT_1, KM_6_SCUT_2, KM_6_SCUT_3, KM_3THUMB_SCUT

// Base layer for twelve column ortholinear layouts.

#define KM_12_BASE_4L KC_OS_SFT, KC_OS_CTL, KC_OS_ALT, KC_OS_GUI, KM_2THUMB_BASE_L
#define KM_12_BASE_4R KM_2THUMB_BASE_R, KC_OS_GUI, KC_OS_ALT, KC_OS_CTL, KC_OS_SFT

#define KM_12_BASE_4 KM_12_BASE_4L, KM_12_BASE_4R

#define HBM_LAYOUT_4x12_BASE KM_6_BASE_1, KM_6_BASE_2, KM_6_BASE_3, KM_12_BASE_4

// Symbol layer for twelve column ortholinear layouts.

#define KM_12_SYM_4L KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KM_2THUMB_SYM_L
#define KM_12_SYM_4R KM_2THUMB_SYM_R, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS

#define KM_12_SYM_4 KM_12_SYM_4L, KM_12_SYM_4R

#define HBM_LAYOUT_4x12_SYM KM_6_SYM_1, KM_6_SYM_2, KM_6_SYM_3, KM_12_SYM_4

// Navigation layer for twelve column ortholinear layouts.

#define KM_12_NAV_4L KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KM_2THUMB_NAV_L
#define KM_12_NAV_4R KM_2THUMB_NAV_R, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS

#define KM_12_NAV_4 KM_12_NAV_4L, KM_12_NAV_4R

#define HBM_LAYOUT_4x12_NAV KM_6_NAV_1, KM_6_NAV_2, KM_6_NAV_3, KM_12_NAV_4

// Number layer for twelve column ortholinear layouts.

#define KM_12_NUM_4L KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KM_2THUMB_NUM_L
#define KM_12_NUM_4R KM_2THUMB_NUM_R, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS

#define KM_12_NUM_4 KM_12_NUM_4L, KM_12_NUM_4R

#define HBM_LAYOUT_4x12_NUM KM_6_NUM_1, KM_6_NUM_2, KM_6_NUM_3, KM_12_NUM_4

// Shortcut layer for twelve column ortholinear layouts.

#define KM_12_SCUT_4L KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KM_2THUMB_SCUT_L
#define KM_12_SCUT_4R KM_2THUMB_SCUT_R, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS

#define KM_12_SCUT_4 KM_12_SCUT_4L, KM_12_SCUT_4R

#define HBM_LAYOUT_4x12_SCUT KM_6_SCUT_1, KM_6_SCUT_2, KM_6_SCUT_3, KM_12_SCUT_4

// Base layer for alternative wide twelve column ortholinear layouts.

#define KM_ALT12_BASE_1 KM_5_BASE_1L, M_ESC_COLN, KC_DEL, KM_5_BASE_1R
#define KM_ALT12_BASE_2 KM_5_BASE_2L, KC_ESC, KC_TAB, KM_5_BASE_2R
#define KM_ALT12_BASE_3 KM_5_BASE_3L, KC_OS_NAV, KC_OS_NUM, KM_5_BASE_3R

#define KM_ALT12_BASE_4L KC_OS_GUI, KC_OS_ALT, KC_OS_CTL, KM_2THUMB_BASE_L, KC_TAB
#define KM_ALT12_BASE_4R KC_ESC, KM_2THUMB_BASE_R, KC_OS_CTL, KC_OS_ALT, KC_OS_GUI

#define KM_ALT12_BASE_4 KM_ALT12_BASE_4L, KM_ALT12_BASE_4R

#define HBM_LAYOUT_ALT12_BASE KM_ALT12_BASE_1, KM_ALT12_BASE_2, KM_ALT12_BASE_3, KM_ALT12_BASE_4

// Symbol layer for alternative wide twelve column ortholinear layouts.

#define KM_ALT12_SYM_1 KM_5_SYM_1L, KC_TRNS, KC_TRNS, KM_5_SYM_1R
#define KM_ALT12_SYM_2 KM_5_SYM_2L, KC_TRNS, KC_TRNS, KM_5_SYM_2R
#define KM_ALT12_SYM_3 KM_5_SYM_3L, KC_TRNS, KC_TRNS, KM_5_SYM_3R

#define KM_ALT12_SYM_4L KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS
#define KM_ALT12_SYM_4R KC_TRNS, KC_TRNS, KC_OS_SCUT, KC_TRNS, KC_TRNS, KC_TRNS

#define KM_ALT12_SYM_4 KM_ALT12_SYM_4L, KM_ALT12_SYM_4R

#define HBM_LAYOUT_ALT12_SYM KM_ALT12_SYM_1, KM_ALT12_SYM_2, KM_ALT12_SYM_3, KM_ALT12_SYM_4

// Navigation layer for alternative wide twelve column ortholinear layouts.

#define KM_ALT12_NAV_1 KM_5_NAV_1L, KC_TRNS, KC_TRNS, KM_5_NAV_1R
#define KM_ALT12_NAV_2 KM_5_NAV_2L, KC_TRNS, KC_TRNS, KM_5_NAV_2R
#define KM_ALT12_NAV_3 KM_5_NAV_3L, KC_TRNS, KC_TRNS, KM_5_NAV_3R

#define KM_ALT12_NAV_4L KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS
#define KM_ALT12_NAV_4R KC_TRNS, KC_ESC, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS

#define KM_ALT12_NAV_4 KM_ALT12_NAV_4L, KM_ALT12_NAV_4R

#define HBM_LAYOUT_ALT12_NAV KM_ALT12_NAV_1, KM_ALT12_NAV_2, KM_ALT12_NAV_3, KM_ALT12_NAV_4

// Number layer for alternative wide twelve column ortholinear layouts.

#define KM_ALT12_NUM_1 KM_5_NUM_1L, KC_TRNS, KC_TRNS, KM_5_NUM_1R
#define KM_ALT12_NUM_2 KM_5_NUM_2L, KC_TRNS, KC_TRNS, KM_5_NUM_2R
#define KM_ALT12_NUM_3 KM_5_NUM_3L, KC_TRNS, KC_TRNS, KM_5_NUM_3R

#define KM_ALT12_NUM_4L KC_TRNS, KC_TRNS, KC_TRNS, KC_SFT_TAB, KC_TAB, KC_TRNS
#define KM_ALT12_NUM_4R KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS

#define KM_ALT12_NUM_4 KM_ALT12_NUM_4L, KM_ALT12_NUM_4R

#define HBM_LAYOUT_ALT12_NUM KM_ALT12_NUM_1, KM_ALT12_NUM_2, KM_ALT12_NUM_3, KM_ALT12_NUM_4

// Shortcut layer for alternative wide twelve column ortholinear layouts.

#define KM_ALT12_SCUT_1 KM_5_SCUT_1L, KC_TRNS, KC_TRNS, KM_5_SCUT_1R
#define KM_ALT12_SCUT_2 KM_5_SCUT_2L, KC_TRNS, KC_TRNS, KM_5_SCUT_2R
#define KM_ALT12_SCUT_3 KM_5_SCUT_3L, KC_TRNS, KC_TRNS, KM_5_SCUT_3R

#define KM_ALT12_SCUT_4L KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS
#define KM_ALT12_SCUT_4R KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS

#define KM_ALT12_SCUT_4 KM_ALT12_SCUT_4L, KM_ALT12_SCUT_4R

#define HBM_LAYOUT_ALT12_SCUT KM_ALT12_SCUT_1, KM_ALT12_SCUT_2, KM_ALT12_SCUT_3, KM_ALT12_SCUT_4

#endif // USERSPACE
