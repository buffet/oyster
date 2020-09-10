/* Copyright (c), Niclas Meyer <niclas@countingsort.com>
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#include "desktop/desktop.h"

#include <stdlib.h>

#include <wlr/types/wlr_export_dmabuf_v1.h>
#include <wlr/types/wlr_xdg_output_v1.h>
#include <wlr/util/log.h>

#include "desktop/output.h"

struct oys_desktop *
desktop_create(struct oys_server *server, struct wlr_renderer *renderer)
{
    struct oys_desktop *desktop = malloc(sizeof(*desktop));
    if (!desktop) {
        wlr_log(WLR_ERROR, "Failed to allocate desktop");
        return NULL;
    }

    desktop->server = server;

    desktop->compositor = wlr_compositor_create(server->display, renderer);
    desktop->data_device_manager =
        wlr_data_device_manager_create(server->display);
    desktop->output_layout = wlr_output_layout_create();

    wlr_export_dmabuf_manager_v1_create(server->display);
    wlr_xdg_output_manager_v1_create(server->display, desktop->output_layout);

    desktop->new_output.notify = new_output_notify;
    wl_signal_add(&server->backend->events.new_output, &desktop->new_output);

    wl_list_init(&desktop->outputs);

    return desktop;
}

void
desktop_destroy(struct oys_desktop *desktop)
{
    wlr_output_layout_destroy(desktop->output_layout);

    free(desktop);
}
