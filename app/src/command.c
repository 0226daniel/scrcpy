#include "command.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ARRAY_LEN(a) (sizeof(a) / sizeof(a[0]))

process_t adb_execute(const char *serial, const char *const adb_cmd[], int len) {
    const char *cmd[len + 4];
    int i;
    cmd[0] = "adb";
    if (serial) {
        cmd[1] = "-s";
        cmd[2] = serial;
        i = 3;
    } else {
        i = 1;
    }

    memcpy(&cmd[i], adb_cmd, len * sizeof(const char *));
    cmd[len + i] = NULL;
    return cmd_execute(cmd[0], cmd);
}

process_t adb_forward(const char *serial, uint16_t local_port, const char *device_socket_name) {
    char local[4 + 5 + 1]; // tcp:PORT
    char remote[108 + 14 + 1]; // localabstract:NAME
    sprintf(local, "tcp:%" PRIu16, local_port);
    snprintf(remote, sizeof(remote), "localabstract:%s", device_socket_name);
    const char *const adb_cmd[] = {"forward", local, remote};
    return adb_execute(serial, adb_cmd, ARRAY_LEN(adb_cmd));
}

process_t adb_reverse(const char *serial, const char *device_socket_name, uint16_t local_port) {
    char local[4 + 5 + 1]; // tcp:PORT
    char remote[108 + 14 + 1]; // localabstract:NAME
    sprintf(local, "tcp:%" PRIu16, local_port);
    snprintf(remote, sizeof(remote), "localabstract:%s", device_socket_name);
    const char *const adb_cmd[] = {"reverse", remote, local};
    return adb_execute(serial, adb_cmd, ARRAY_LEN(adb_cmd));
}

process_t adb_reverse_remove(const char *serial, const char *device_socket_name) {
    char remote[108 + 14 + 1]; // localabstract:NAME
    snprintf(remote, sizeof(remote), "localabstract:%s", device_socket_name);
    const char *const adb_cmd[] = {"reverse", "--remove", remote};
    return adb_execute(serial, adb_cmd, ARRAY_LEN(adb_cmd));
}

process_t adb_push(const char *serial, const char *local, const char *remote) {
    const char *const adb_cmd[] = {"push", (char *) local, (char *) remote};
    return adb_execute(serial, adb_cmd, ARRAY_LEN(adb_cmd));
}