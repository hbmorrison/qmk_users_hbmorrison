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

#include QMK_KEYBOARD_H
#include "tenkey.h"

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
  [LAYER_BASE] = TENKEY_LAYOUT_ferris_sweep( TENKEY_TEST_LAYOUT_BASE ),
  [LAYER_FILL_LEFT] = TENKEY_LAYOUT_ferris_sweep( TENKEY_TEST_LAYOUT_FILL_LEFT ),
  [LAYER_FILL_RIGHT] = TENKEY_LAYOUT_ferris_sweep( TENKEY_TEST_LAYOUT_FILL_RIGHT ),
  [LAYER_SYM_LEFT] = TENKEY_LAYOUT_ferris_sweep( TENKEY_TEST_LAYOUT_SYM_LEFT ),
  [LAYER_SYM_RIGHT] = TENKEY_LAYOUT_ferris_sweep( TENKEY_TEST_LAYOUT_SYM_RIGHT ),
  [LAYER_NAV] = TENKEY_LAYOUT_ferris_sweep( TENKEY_TEST_LAYOUT_NAV ),
  [LAYER_NUM] = TENKEY_LAYOUT_ferris_sweep( TENKEY_TEST_LAYOUT_NUM ),
  [LAYER_FUNC] = TENKEY_LAYOUT_ferris_sweep( TENKEY_TEST_LAYOUT_FUNC )
};
