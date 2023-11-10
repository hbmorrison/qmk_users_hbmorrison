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

#include QMK_KEYBOARD_H
#include "hbmorrison.h"

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
  [LAYER_BASE] = HBM_LAYOUT_planck_grid( HBM_LAYOUT_ALT12_BASE ),
  [LAYER_SYM] = HBM_LAYOUT_planck_grid( HBM_LAYOUT_ALT12_SYM ),
  [LAYER_NAV] = HBM_LAYOUT_planck_grid( HBM_LAYOUT_ALT12_NAV ),
  [LAYER_NUM] = HBM_LAYOUT_planck_grid( HBM_LAYOUT_ALT12_NUM ),
  [LAYER_SCUT] = HBM_LAYOUT_planck_grid( HBM_LAYOUT_ALT12_SCUT )
};