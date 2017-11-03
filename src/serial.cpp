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
    int status;
    if (params) { // only one parameter for status
        status = atoi(params);
		LOG_TRACE("Got serial status: %d", status);
        return status;
    }
    LOG_ERR("No status sent");
    return -1;
}

int do_serial_command(char *cmd_str, int *expected) {
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
    if (c < sizeof(cmds)/sizeof(cmd_f) && cmds[c]) {
        ret = cmds[c](params); // params may be NULL
        if (expected) {
            if (*expected == c)
                *expected = ret;
            else
                *expected = -1;
        }
        ret = 0;
    } else {
        LOG_WARN("Serial cmd %d not implemented", c);
    }

    return ret;
}

int rx_serial_command(char c, int *expected) {
    #define BUFF_SIZE 64
    static char rx_buffer[BUFF_SIZE];
    return rx_serial_command_r(c, rx_buffer, BUFF_SIZE, expected);
}

// recieves serial command into rx_buffer given
// one character, c, at a time. Returns one of the following:
// 0  = command rx'd/completed successfully
// 1  = command not yet fully recieved, keep passing in characters
// -1 = command not rx'd or failed
// Can also use *cmd to pass in the expected cmd and upon return, *cmd
// will be the status of the parsed cmd if it matches the expected
int rx_serial_command_r(char c, char *rx_buffer, int size, int *expected) {
    static int i = 0;

    int ret;
    if (i < size) {
        if (c == '\r' || c == '\n') {
            rx_buffer[i] = '\0';
			LOG_INFO("%s", rx_buffer);
            if (do_serial_command(rx_buffer, expected) == 0) {
                LOG_TRACE("Serial cmd complete");
                ret = 0;
            } else {
                LOG_ERR("Serial cmd failed");
                ret = -1;
            }
            CLEAR_BUFF(rx_buffer, size, i);
        } else {
            rx_buffer[i] = c;
            i++;
            ret = 1;
        }
    } else {
        LOG_ERR("Serial buffer overflow");
        CLEAR_BUFF(rx_buffer, size, i);
        ret = -1;
    }

    return ret;
}
