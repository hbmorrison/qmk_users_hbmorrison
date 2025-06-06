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
#define TAPPING_TERM_TAP_DANCE_SHIFT 2000

// Caps word.

#define CAPS_WORD_IDLE_TIMEOUT 2000

// Layout macros that allow preprocessor substitutions. Use these instead of the
// standard LAYOUT_ macros in keymap.c code.

#define HBM_LAYOUT_ferris_sweep(...) LAYOUT_split_3x5_2(__VA_ARGS__)
