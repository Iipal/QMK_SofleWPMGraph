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

#ifdef OLED_ENABLE

int  timer = 0;
char wpm_text[5];
int  x          = 31;
int  currwpm    = 0;
int  vert_count = 0;

//=============  USER CONFIG PARAMS  ===============
float max_wpm                  = 150.0f; // WPM value at the top of the graph window
int   graph_refresh_interval   = 80;     // how often screen will be refreshed with WPM values; in milliseconds
int   graph_line_thickness     = 3;      // determines thickness of graph line in pixels
int   graph_area_fill_interval = 1;      // determines how dense the horizontal lines under the graph line are lower = more dense
//=============  END USER PARAMS  ===============

static inline void render_wpm(void) {
    // get current WPM value
    currwpm = get_current_wpm();

    // check if it's been long enough before refreshing graph
    if (timer_elapsed(timer) > graph_refresh_interval) {
        // main calculation to plot graph line
        x = 32 - ((currwpm / max_wpm) * 32);

        // first draw actual value line
        for (int i = 0; i <= graph_line_thickness - 1; i++) {
            oled_write_pixel(1, x + i, true);
        }

        // then fill in area below the value line
        for (int i = 32; i > x; i--) {
            if (i % graph_area_fill_interval == 0) {
                oled_write_pixel(1, i, true);
            }
        }

        // then move the entire graph one pixel to the right
        oled_pan(false);

        // refresh the timer for the next iteration
        timer = timer_read();
    }

    // format current WPM value into a printable string
    itoa(currwpm, wpm_text, 10);

    // formatting for triple digit WPM vs double digits, then print WPM readout
    if (currwpm >= 100) {
        oled_set_cursor(14, 3);
        oled_write("WPM: ", false);
        oled_set_cursor(18, 3);
        oled_write(wpm_text, false);
    } else if (currwpm >= 10) {
        oled_set_cursor(15, 3);
        oled_write("WPM: ", false);
        oled_set_cursor(19, 3);
        oled_write(wpm_text, false);
    } else if (currwpm > 0) {
        oled_set_cursor(16, 3);
        oled_write("WPM: ", false);
        oled_set_cursor(20, 3);
        oled_write(wpm_text, false);
    }
}

static inline void print_status_narrow(void) {
    // Print current mode
    oled_write_P(PSTR("\n\n"), false);

    const uint8_t layer_idx = get_highest_layer(layer_state);

    switch (layer_idx) {
        case 0:
            oled_write_ln_P(PSTR("QWRT"), false);
            break;
        case 1:
            oled_write_ln_P(PSTR("GAME"), false);
            break;
        default:
            oled_write_ln_P(PSTR("LYR"), false);
            break;
    }

    oled_write_P(PSTR("\n\n"), false);

    switch (layer_idx) {
        case 0:
        case 1:
            oled_write_ln_P(PSTR("Base"), false);
            break;
        case 2:
            oled_write_ln_P(PSTR("RSE"), false);
            break;
        case 3:
            oled_write_ln_P(PSTR("LWR"), false);
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
