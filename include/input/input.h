/* Copyright (c), Niclas Meyer <niclas@countingsort.com>
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#ifndef OYSTER_INPUT_INPUT_H
#define OYSTER_INPUT_INPUT_H

#include <wayland-server.h>

#include "input/cursor.h"
#include "input/seat.h"
#include "server.h"

struct oys_input {
    struct oys_server *server;
    struct oys_seat *seat;
    struct oys_cursor *cursor;
    struct wl_list keyboards; // struct oys_keyboard::link

    struct wl_listener new_input;
};

struct oys_input *input_create(struct oys_server *server);
void input_destroy(struct oys_input *input);

#endif /* OYSTER_INPUT_INPUT_H */
