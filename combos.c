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

// Modifier combos.

const uint16_t PROGMEM lctlsft_combo[] = {KC_V, KC_D, COMBO_END};
const uint16_t PROGMEM lctl_combo[] = {KC_D, KC_C, COMBO_END};
const uint16_t PROGMEM lalt_combo[] = {KC_C, KC_X, COMBO_END};
const uint16_t PROGMEM lgui_combo[] = {KC_X, KC_Z, COMBO_END};
const uint16_t PROGMEM lmeh_combo[] = {KC_D, KC_C, KC_X, COMBO_END};

const uint16_t PROGMEM rctlsft_combo[] = {KC_K, KC_H, COMBO_END};
const uint16_t PROGMEM rctl_combo[] = {KC_H, KC_COMMA, COMBO_END};
const uint16_t PROGMEM ralt_combo[] = {KC_COMMA, KC_DOT, COMBO_END};
const uint16_t PROGMEM rgui_combo[] = {KC_DOT, KC_SLSH, COMBO_END};
const uint16_t PROGMEM rmeh_combo[] = {KC_H, KC_COMMA, KC_DOT, COMBO_END};

// Combo definitions.

combo_t key_combos[] = {
  COMBO(lctlsft_combo, LCTL(KC_LSFT)),
  COMBO(lctl_combo, KC_LCTL),
  COMBO(lalt_combo, KC_LALT),
  COMBO(lgui_combo, KC_LGUI),
  COMBO(lmeh_combo, LALT(LCTL(KC_LSFT))),
  COMBO(rctlsft_combo, RCTL(KC_RSFT)),
  COMBO(rctl_combo, KC_RCTL),
  COMBO(ralt_combo, KC_LALT),
  COMBO(rgui_combo, KC_LGUI),
  COMBO(rmeh_combo, LALT(RCTL(KC_RSFT)))
};
