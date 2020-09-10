/* Copyright (c), Niclas Meyer <niclas@countingsort.com>
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#ifndef OYSTER_INPUT_CURSOR_H
#define OYSTER_INPUT_CURSOR_H

#include <wayland-server.h>
#include <wlr/types/wlr_cursor.h>
#include <wlr/types/wlr_xcursor_manager.h>

struct oys_cursor {
    struct oys_input *input;
    struct wlr_cursor *cursor;
    struct wlr_xcursor_manager *xcursor_manager;

    struct wl_listener motion;
    struct wl_listener motion_absolute;
    struct wl_listener frame;
};

struct oys_cursor *cursor_create(struct oys_input *input);
void cursor_destroy(struct oys_cursor *cursor);

#endif /* OYSTER_INPUT_CURSOR_H */
