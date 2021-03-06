/* Copyright (c), Niclas Meyer <niclas@countingsort.com>
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#include <stdio.h>
#include <stdlib.h>

#include <unistd.h>

#include <wlr/util/log.h>

#include "server.h"

int
main(int argc, char **argv)
{
    int verbosity = 0;
    const char *usage =
        "Usage: oyster [options]\n"
        "\n"
        "  -h  Show help message and exit\n"
        "  -v  Show version number and exit\n"
        "  -V  Increase verbosity level\n";

    int option;
    while ((option = getopt(argc, argv, "hvV")) != -1) {
        switch (option) {
        case 'h':
            printf("%s", usage);
            exit(EXIT_SUCCESS);
            break;
        case 'v':
            printf("oyster version " OYSTER_VERSION "\n");
            exit(EXIT_SUCCESS);
            break;
        case 'V':
            ++verbosity;
            break;
        default:
            fprintf(stderr, "%s", usage);
            exit(EXIT_FAILURE);
        }
    }

    switch (verbosity) {
    case 0:
        wlr_log_init(WLR_ERROR, NULL);
        break;
    case 1:
        wlr_log_init(WLR_INFO, NULL);
        break;
    default:
        wlr_log_init(WLR_DEBUG, NULL);
    }

    fprintf(stderr, "Using oyster v" OYSTER_VERSION "\n");

    struct oys_server *server = server_create();

    if (!server) {
        wlr_log(WLR_ERROR, "Failed to initialize server");
        exit(EXIT_FAILURE);
    }

    if (!server_run(server)) {
        wlr_log(WLR_ERROR, "Failed to run server");
        exit(EXIT_FAILURE);
    }

    server_destroy(server);
}
