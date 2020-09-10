/* Copyright (c), Niclas Meyer <niclas@countingsort.com>
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#include "desktop/output.h"

#include <stdlib.h>

#include <time.h>

#include <wlr/render/wlr_renderer.h>
#include <wlr/types/wlr_xcursor_manager.h>
#include <wlr/util/log.h>

#include "desktop/desktop.h"

static void
output_destroy_notify(struct wl_listener *listener, void *UNUSED(data))
{
    struct oys_output *output = wl_container_of(listener, output, destroy);

    wlr_output_layout_remove(
        output->desktop->output_layout, output->wlr_output);

    wl_list_remove(&output->link);
    wl_list_remove(&output->destroy.link);
    wl_list_remove(&output->frame.link);

    free(output);
}

static void
output_frame_notify(struct wl_listener *listener, void *UNUSED(data))
{
    struct oys_output *output     = wl_container_of(listener, output, frame);
    struct wlr_output *wlr_output = output->wlr_output;
    struct wlr_renderer *renderer =
        wlr_backend_get_renderer(wlr_output->backend);

    struct timespec now;
    clock_gettime(CLOCK_MONOTONIC, &now);

    if (!wlr_output_attach_render(wlr_output, NULL)) {
        wlr_log(WLR_ERROR, "Failed to attach renderer to output");
        return;
    }

    int width;
    int height;
    wlr_output_effective_resolution(wlr_output, &width, &height);

    wlr_renderer_begin(renderer, width, height);
    wlr_renderer_clear(renderer, (float[]){0.1f, 0.1f, 0.1f, 1.0f});
    wlr_output_render_software_cursors(wlr_output, NULL);
    wlr_renderer_end(renderer);

    wlr_output_commit(wlr_output);
}

static struct oys_output *
output_create(struct wlr_output *wlr_output, struct oys_desktop *desktop)
{
    struct oys_output *output = malloc(sizeof(*output));
    if (!output) {
        wlr_log(WLR_ERROR, "Failed to allocate output");
        return NULL;
    }

    output->wlr_output = wlr_output;
    output->desktop    = desktop;

    output->destroy.notify = output_destroy_notify;
    output->frame.notify   = output_frame_notify;

    wl_signal_add(&wlr_output->events.destroy, &output->destroy);
    wl_signal_add(&wlr_output->events.frame, &output->frame);

    return output;
}

void
new_output_notify(struct wl_listener *listener, void *data)
{
    struct oys_desktop *desktop =
        wl_container_of(listener, desktop, new_output);
    struct wlr_output *wlr_output = data;
    struct oys_input *input       = desktop->server->input;

    wlr_log(WLR_DEBUG, "New output %p: %s", wlr_output, wlr_output->name);

    if (!wl_list_empty(&wlr_output->modes)) {
        struct wlr_output_mode *mode =
            wl_container_of(wlr_output->modes.prev, mode, link);
        wlr_output_set_mode(wlr_output, mode);

        if (!wlr_output_commit(wlr_output)) {
            wlr_log(WLR_ERROR, "Failed to modeset output");
            return;
        }
    }

    struct oys_output *output = output_create(wlr_output, desktop);
    if (!output) {
        wlr_log(WLR_ERROR, "Failed to create output");
        return;
    }

    wl_list_insert(&desktop->outputs, &output->link);

    wlr_xcursor_manager_load(input->cursor->xcursor_manager, wlr_output->scale);

    wlr_output_create_global(wlr_output);
    wlr_output_layout_add_auto(desktop->output_layout, wlr_output);
}
