/* Copyright (c), Niclas Meyer <niclas@countingsort.com>
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#ifndef OYSTER_INPUT_SEAT_H
#define OYSTER_INPUT_SEAT_H

#include <wlr/types/wlr_seat.h>

#include "input/input.h"

struct oys_seat {
    struct oys_input *input;
    struct wlr_seat *seat;
};

struct oys_seat *seat_create(struct oys_input *input);
void seat_destroy(struct oys_seat *seat);

#endif /* OYSTER_INPUT_SEAT_H */
