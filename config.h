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

#pragma once

// Tapping config.

#define PERMISSIVE_HOLD
#define RETRO_TAPPING_PER_KEY
#define TAPPING_TERM_PER_KEY
#define TAPPING_TERM 200
#define TAPPING_TERM_LAYER 200

// Double tap shift for caps word.

#define DOUBLE_TAP_SHIFT_TURNS_ON_CAPS_WORD
#define CAPS_WORD_IDLE_TIMEOUT 10000

// Combo timings.

#define COMBO_TERM 25

// Enable constant scroll wheel.

#define MK_3_SPEED
#define MK_W_OFFSET_1 2
#define MK_W_INTERVAL_1 80

// Layout macros that allow preprocessor substitutions. Use these instead of the
// standard LAYOUT_ macros in keymap.c code.

#define HBM_LAYOUT_ferris_sweep(...) LAYOUT_split_3x5_2(__VA_ARGS__)
#define HBM_LAYOUT_planck(...) LAYOUT_planck_grid(__VA_ARGS__)
