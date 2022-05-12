/* Copyright 2020 Josef Adamcik
 * Modification for VIA support and RGB underglow by Jens Bonk-Wiltfang
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

// Sets up what the OLED screens display.

#ifndef OLED_ENABLE
#define OLED_ENABLE
#endif

#ifdef OLED_ENABLE

#define DISPLAY_WIDTH 32

char    wpm_text[8] = "WPM: ";
int     timer       = 0;
int     y           = DISPLAY_WIDTH - 1;
uint8_t currwpm     = 0;
uint8_t oldwpm      = 0;

//=============  USER CONFIG PARAMS  ===============
#define WPM_GRAPH_SPEED_MAX 130.0f    // WPM value at the top of the graph window
#define WPM_GRAPH_REFRESH_INTERVAL 80 // how often screen will be refreshed with WPM values; in milliseconds
#define WPM_GRAPH_REFRESH_TIMEOUT 800 // how often screen will be refreshed with WPM values; in milliseconds
#define WPM_GRAPH_LINE_THICKNESS 3    // determines thickness of graph line in pixels

#if defined(OLED_BRIGHTNESS) && OLED_BRIGHTNESS < 100
#    define __WPM_GRAPH_BRIGHTNESS_DEFINED 1

#    define WPM_GRAPH_SPEED_HIGHLIGHT 50.0f // At what WPM screen brightness will start increasing
#    define WPM_GRAPH_BRIGHTNESS_MAX 255    // Max screen brightness
#endif
//=============  END USER PARAMS  ===============

static inline void render_wpm(void) {
    const int elapsed = timer_elapsed(timer);

    // get current WPM value
    oldwpm  = currwpm;
    currwpm = get_current_wpm();

    if (elapsed > WPM_GRAPH_REFRESH_INTERVAL) {
        // main calculation to plot graph line
        y = DISPLAY_WIDTH - ((currwpm / WPM_GRAPH_SPEED_MAX) * DISPLAY_WIDTH);

        // first draw actual value line
        for (int i = 0; i < WPM_GRAPH_LINE_THICKNESS; i++) {
            oled_write_pixel(1, y + i, true);
        }

        // then fill in area below the value line
        for (int i = DISPLAY_WIDTH; i > y && 0 <= i; i--) {
            oled_write_pixel(1, i, true);
        }

        // then move the entire graph one pixel to the right
        oled_pan(false);

        // refresh the timer for the next iteration
        timer = timer_read();
    }

    // formatting for triple digit WPM vs double digits, then print WPM readout,
    // or does not print anything if WPM is 0
    int8_t pos_offset = -1;
    if (100 <= currwpm) {
        pos_offset = 0;
    } else if (10 <= currwpm) {
        pos_offset = 1;
    } else if (0 < currwpm) {
        pos_offset = 2;
    }
    if (0 <= pos_offset) {
        if (currwpm != oldwpm) {
            itoa(currwpm, wpm_text + 4, 10);
        }
        oled_set_cursor(14 + pos_offset, 3);
        oled_write(wpm_text, false);
    }

// brightness adjust when riched WPM_GRAPH_SPEED_HIGHLIGHT
#ifdef __WPM_GRAPH_BRIGHTNESS_DEFINED
    const uint8_t currbr = oled_get_brightness();
    uint8_t       newbr  = currbr;

    if (WPM_GRAPH_SPEED_HIGHLIGHT <= currwpm) {
        const int   brsteps = (WPM_GRAPH_SPEED_MAX - WPM_GRAPH_SPEED_HIGHLIGHT) - (WPM_GRAPH_SPEED_MAX - ((currwpm > WPM_GRAPH_SPEED_MAX) ? WPM_GRAPH_SPEED_MAX : currwpm));
        const float _newbr  = OLED_BRIGHTNESS + (((float)(WPM_GRAPH_BRIGHTNESS_MAX - OLED_BRIGHTNESS) / (WPM_GRAPH_SPEED_MAX - WPM_GRAPH_SPEED_HIGHLIGHT)) * (float)brsteps);

        newbr = (uint8_t)((_newbr > WPM_GRAPH_BRIGHTNESS_MAX) ? WPM_GRAPH_BRIGHTNESS_MAX : _newbr);
    } else if (currbr != OLED_BRIGHTNESS) {
        newbr = OLED_BRIGHTNESS;
    }

    if (currbr != newbr) {
        oled_set_brightness(newbr);
    }

#endif
}

static inline void print_status_narrow(void) {
    // Print current mode
    oled_write_P(PSTR("\n\n"), false);

    const uint8_t layer_idx = get_highest_layer(layer_state);

    switch (layer_idx) {
        case 0:
            oled_write_ln_P(PSTR("qwrt"), false);
            break;
        case 1:
            oled_write_ln_P(PSTR("GAME"), true);
            break;
        default:
            oled_write_ln_P(PSTR("lyr"), false);
            break;
    }

    oled_write_P(PSTR("\n\n"), false);

    switch (layer_idx) {
        case 0:
        case 1:
            oled_write_ln_P(PSTR("Base"), false);
            break;
        case 2:
            oled_write_ln_P(PSTR("RSE"), true);
            break;
        case 3:
            oled_write_ln_P(PSTR("LWR"), true);
            break;
        default:
            oled_write_ln_P(PSTR("Undef"), false);
            break;
    }
}

oled_rotation_t oled_init_user(oled_rotation_t rotation) {
    if (is_keyboard_master()) {
        return OLED_ROTATION_270;
    }
    return OLED_ROTATION_180;
}

bool oled_task_user(void) {
    if (is_keyboard_master()) {
        print_status_narrow();
    } else {
        render_wpm();
    }

    return false;
}

#endif
