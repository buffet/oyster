/* Copyright (c), Niclas Meyer <niclas@countingsort.com>
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#include "input/seat.h"

#include <stdlib.h>

#include <wlr/util/log.h>

struct oys_seat *
seat_create(struct oys_input *input)
{
    struct oys_seat *seat = malloc(sizeof(*seat));
    if (!seat) {
        wlr_log(WLR_ERROR, "Failed to allocate seat");
        return NULL;
    }

    seat->input = input;
    seat->seat  = wlr_seat_create(input->server->display, "seat-0");

    return seat;
}

void
seat_destroy(struct oys_seat *seat)
{
    free(seat);
}
