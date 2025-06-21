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
  LAYER_CTLS
};

// Custom keycodes.

enum hbm_keycodes {
  M_ALT_TAB = SAFE_RANGE,
  M_NDESK,
  M_PDESK,
  M_OVERVIEW,
  M_FULLSCREEN,
  M_MINIMISE,
  M_EMOJI,
  M_ISWINDOWS,
  M_ISCHROMEOS,
  M_ISLINUX
};

// Tap dance codes.

enum {
  TD_LSFT,
  TD_RSFT
};

// Left- and right-handed shift keys.

#define KC_TD_LSFT TD(TD_LSFT)
#define KC_TD_RSFT TD(TD_RSFT)

// Alternative keys for UK ISO keyboard layouts.

#define KC_UK_DQUO LSFT(KC_2)
#define KC_UK_PND LSFT(KC_3)
#define KC_UK_PIPE LSFT(KC_NUBS)
#define KC_UK_BSLS KC_NUBS
#define KC_UK_AT LSFT(KC_QUOT)
#define KC_UK_TILDE LSFT(KC_BSLS)
#define KC_UK_HASH KC_BSLS

// Bare modifier keys.

#define KC_LCA LCTL(KC_LALT)
#define KC_LCS LCTL(KC_LSFT)
#define KC_RCS RCTL(KC_RSFT)
#define KC_RCA RCTL(KC_LALT)

// Modified keys.

#define KC_SFT_TAB LSFT(KC_TAB)
#define KC_CTL_TAB LCTL(KC_TAB)

// Layer keys.

#define KC_NAV_KEY KC_SPC
#define KC_NUM_KEY KC_ENT
#define KC_FUNC_KEY KC_F
#define KC_CTLS_KEY KC_U

#define KC_NAV LT(LAYER_NAV, KC_NAV_KEY)
#define KC_NUM LT(LAYER_NUM, KC_NUM_KEY)
#define KC_FUNC LT(LAYER_FUNC, KC_FUNC_KEY)
#define KC_CTLS LT(LAYER_CTLS, KC_CTLS_KEY)

// Homerow modifier keys.

#define KC_LCA_KEY KC_Z
#define KC_LGUI_KEY KC_X
#define KC_LALT_KEY KC_C
#define KC_LCTL_KEY KC_D
#define KC_LCS_KEY KC_V
#define KC_RCS_KEY KC_K
#define KC_RCTL_KEY KC_H
#define KC_RALT_KEY KC_COMMA
#define KC_RGUI_KEY KC_DOT
#define KC_RCA_KEY KC_SLSH

#define KC_HR_LCA LCA_T(KC_LCA_KEY)
#define KC_HR_LGUI LGUI_T(KC_LGUI_KEY)
#define KC_HR_LALT LALT_T(KC_LALT_KEY)
#define KC_HR_LCTL LCTL_T(KC_LCTL_KEY)
#define KC_HR_LCS C_S_T(KC_LCS_KEY)
#define KC_HR_RCS C_S_T(KC_RCS_KEY)
#define KC_HR_RCTL LCTL_T(KC_RCTL_KEY)
#define KC_HR_RALT LALT_T(KC_RALT_KEY)
#define KC_HR_RGUI LGUI_T(KC_RGUI_KEY)
#define KC_HR_RCA LCA_T(KC_RCA_KEY)

// Base layer.

#define KM_BASE_1L KC_Q, KC_W, KC_FUNC, KC_P, KC_B
#define KM_BASE_2L KC_A, KC_R, KC_S, KC_T, KC_G
#define KM_BASE_3L KC_HR_LCA, KC_HR_LGUI, KC_HR_LALT, KC_HR_LCTL, KC_HR_LCS

#define KM_BASE_1R KC_J, KC_L, KC_CTLS, KC_Y, KC_BSPC
#define KM_BASE_2R KC_M, KC_N, KC_E, KC_I, KC_O
#define KM_BASE_3R KC_HR_RCS, KC_HR_RCTL, KC_HR_RALT, KC_HR_RGUI, KC_HR_RCA

#define KM_BASE_1 KM_BASE_1L, KM_BASE_1R
#define KM_BASE_2 KM_BASE_2L, KM_BASE_2R
#define KM_BASE_3 KM_BASE_3L, KM_BASE_3R

#define KM_BASE_THUMB KC_TD_LSFT, KC_NAV, KC_NUM, KC_TD_RSFT

#define LAYOUT_BASE KM_BASE_1, KM_BASE_2, KM_BASE_3, KM_BASE_THUMB

// Left symbol layer.

#define KM_LSYM_1L KC_EXLM, KC_UK_DQUO, KC_UK_PND, KC_DLR, KC_PERC
#define KM_LSYM_2L KC_GRV, KC_UK_PIPE, KC_LBRC, KC_LCBR, KC_LPRN
#define KM_LSYM_3L M_EMOJI, KC_UK_BSLS, KC_RBRC, KC_RCBR, KC_RPRN

#define KM_LSYM_1R KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS
#define KM_LSYM_2R KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS
#define KM_LSYM_3R KC_RCS, KC_RCTL, KC_RALT, KC_RGUI, KC_RCA

#define KM_LSYM_1 KM_LSYM_1L, KM_LSYM_1R
#define KM_LSYM_2 KM_LSYM_2L, KM_LSYM_2R
#define KM_LSYM_3 KM_LSYM_3L, KM_LSYM_3R

#define KM_LSYM_THUMB KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS

#define LAYOUT_LSYM KM_LSYM_1, KM_LSYM_2, KM_LSYM_3, KM_LSYM_THUMB

// Right symbol layer.

#define KM_RSYM_1L KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS
#define KM_RSYM_2L KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS
#define KM_RSYM_3L KC_LCA, KC_LGUI, KC_LALT, KC_LCTL, KC_LCS

#define KM_RSYM_1R KC_CIRC, KC_AMPR, KC_ASTR, KC_NO, KC_DEL
#define KM_RSYM_2R KC_SCLN, KC_QUOT, KC_UK_HASH, KC_MINS, KC_EQL
#define KM_RSYM_3R KC_COLN, KC_UK_AT, KC_UK_TILDE, KC_UNDS, KC_PLUS

#define KM_RSYM_1 KM_RSYM_1L, KM_RSYM_1R
#define KM_RSYM_2 KM_RSYM_2L, KM_RSYM_2R
#define KM_RSYM_3 KM_RSYM_3L, KM_RSYM_3R

#define KM_RSYM_THUMB KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS

#define LAYOUT_RSYM KM_RSYM_1, KM_RSYM_2, KM_RSYM_3, KM_RSYM_THUMB

// Number layer.

#define KM_NUM_1L KC_NO, KC_1, KC_2, KC_3, KC_PSLS
#define KM_NUM_2L KC_NO, KC_4, KC_5, KC_6, KC_DOT
#define KM_NUM_3L KC_NO, KC_7, KC_8, KC_9, KC_0

#define KM_NUM_1R KC_MS_BTN2, KC_NO, KC_NO, KC_NO, KC_NO
#define KM_NUM_2R KC_MS_BTN1, KC_NO, KC_NO, KC_NO, KC_NO
#define KM_NUM_3R KC_RCS, KC_RCTL, KC_RALT, KC_RGUI, KC_RCA

#define KM_NUM_1 KM_NUM_1L, KM_NUM_1R
#define KM_NUM_2 KM_NUM_2L, KM_NUM_2R
#define KM_NUM_3 KM_NUM_3L, KM_NUM_3R

#define KM_NUM_THUMB KC_SFT_TAB, KC_TAB, KC_TRNS, KC_TRNS

#define LAYOUT_NUM KM_NUM_1, KM_NUM_2, KM_NUM_3, KM_NUM_THUMB

// Navigation layer.

#define KM_NAV_1L KC_NO, KC_NO, KC_NO, KC_NO, KC_MS_BTN2
#define KM_NAV_2L KC_NO, KC_NO, KC_NO, KC_NO, KC_MS_BTN1
#define KM_NAV_3L KC_LCA, KC_LGUI, KC_LALT, KC_LCTL, KC_LCS

#define KM_NAV_1R M_OVERVIEW, M_PDESK, KC_CTL_TAB, M_ALT_TAB, M_NDESK
#define KM_NAV_2R M_FULLSCREEN, KC_LEFT, KC_DOWN, KC_UP, KC_RIGHT
#define KM_NAV_3R M_MINIMISE, KC_HOME, KC_PGDN, KC_PGUP, KC_END

#define KM_NAV_1 KM_NAV_1L, KM_NAV_1R
#define KM_NAV_2 KM_NAV_2L, KM_NAV_2R
#define KM_NAV_3 KM_NAV_3L, KM_NAV_3R

#define KM_NAV_THUMB KC_TRNS, KC_TRNS, KC_ESC, KC_TRNS

#define LAYOUT_NAV KM_NAV_1, KM_NAV_2, KM_NAV_3, KM_NAV_THUMB

// Function key layer.

#define KM_FUNC_1L KC_NO, KC_NO, KC_TRNS, KC_NO, KC_NO
#define KM_FUNC_2L KC_NO, KC_NO, KC_NO, KC_NO, KC_NO
#define KM_FUNC_3L KC_LCA, KC_LGUI, KC_LALT, KC_LCTL, KC_LCS

#define KM_FUNC_1R KC_F1, KC_F2, KC_F3, KC_F4, KC_F5
#define KM_FUNC_2R KC_F6, KC_F7, KC_F8, KC_F9, KC_F10
#define KM_FUNC_3R KC_F11, KC_F12, KC_F13, KC_F14, KC_F15

#define KM_FUNC_1 KM_FUNC_1L, KM_FUNC_1R
#define KM_FUNC_2 KM_FUNC_2L, KM_FUNC_2R
#define KM_FUNC_3 KM_FUNC_3L, KM_FUNC_3R

#define KM_FUNC_THUMB KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS

#define LAYOUT_FUNC KM_FUNC_1, KM_FUNC_2, KM_FUNC_3, KM_FUNC_THUMB

// Controls layer.

#define KM_CTLS_1L KC_NO, KC_MPLY, KC_MUTE, KC_PSCR, M_ISWINDOWS
#define KM_CTLS_2L KC_NO, KC_MNXT, KC_VOLU, KC_BRIU, M_ISCHROMEOS
#define KM_CTLS_3L KC_NO, KC_MPRV, KC_VOLD, KC_BRID, M_ISLINUX

#define KM_CTLS_1R KC_NO, KC_NO, KC_TRNS, KC_NO, KC_NO
#define KM_CTLS_2R KC_NO, KC_NO, KC_NO, KC_NO, KC_NO
#define KM_CTLS_3R KC_NO, KC_NO, KC_NO, KC_NO, KC_NO

#define KM_CTLS_1 KM_CTLS_1L, KM_CTLS_1R
#define KM_CTLS_2 KM_CTLS_2L, KM_CTLS_2R
#define KM_CTLS_3 KM_CTLS_3L, KM_CTLS_3R

#define KM_CTLS_THUMB KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS

#define LAYOUT_CTLS KM_CTLS_1, KM_CTLS_2, KM_CTLS_3, KM_CTLS_THUMB

#endif // USERSPACE
