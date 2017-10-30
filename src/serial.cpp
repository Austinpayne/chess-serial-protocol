#include "serial.h"
#ifdef SPARK
#include "Particle.h"
#else
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#endif

// cmd format: `0x[CMD] [params?]\n`
// or `0x[CMD]\n`
cmd_f cmds[] = {
    &do_status,     // FROM motor control
    &do_new_game,   // FROM motor control
    &do_end_turn,   // TO   motor control
    &do_move_piece, // TO   motor control
    &do_promote,    // FROM motor control
    &do_calibrate,  // TO   motor control
    &do_end_game,   // FROM motor control
    &do_scan_wifi,  // FROM motor control
    &do_set_wifi    // FROM motor control
};

// -1 == cmd failed, 0 == ok, 1 == error, etc.
int do_status(char *params) {
    char *p;
    int status;
    if ((p = strtok(params, ",")) != NULL) {
        status = atoi(p);
		LOG_TRACE("Got serial status: %d", status);
        return status;
    }
    return -1;
}

int do_serial_command(char *cmd_str) {
    char *cmd, *params, *tmp;
    int c;
    int ret = -1;

    cmd = strtok(cmd_str, " \n");
    params = strtok(NULL, "\n");

    if (!cmd) {
        LOG_ERR("No command found");
        return ret;
    }
    LOG_INFO("Got cmd %.4s", cmd);
    c = strtol(cmd, &tmp, 16); // cmd number is in hex
    if (strcmp(cmd, tmp) == 0) {
        LOG_ERR("Command bits are invalid");
        return ret;
    }
    LOG_INFO("cmd number: %d", c);
    if (c < sizeof(cmds) && cmds[c]) {
        ret = cmds[c](params); // params may be NULL
    } else {
        LOG_WARN("Serial cmd %d not implemented", c);
    }

    return ret;
}

void rx_serial_command(char c) {
    #define SERIAL_BUFF_SIZE 64

    static char rx_buffer[SERIAL_BUFF_SIZE];
    static int i = 0;

    if (i < SERIAL_BUFF_SIZE) {
        if (c == '\r' || c == '\n') {
            rx_buffer[i] = '\0';
			LOG_INFO("%s", rx_buffer);
            if (do_serial_command(rx_buffer) == 0) {
                LOG_TRACE("Serial cmd complete");
            } else {
                LOG_ERR("Serial cmd failed");
            }
            CLEAR_BUFF(rx_buffer, SERIAL_BUFF_SIZE, i);
        } else {
            rx_buffer[i] = c;
            i++;
        }
    }
}
