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
    &do_status,
    &do_new_game,
    &do_end_turn,
    &do_move_piece,
    &do_promote,
    &do_calibrate,
    &do_end_game,
    &do_scan_wifi,
    &do_set_wifi,
    &do_send_log,
    &do_capture_castle,
    &do_reset,
    &do_user_turn
};

// -1 == cmd failed, 0 == ok, 1 == error, etc.
int do_status(char *params) {
    int status;
    if (params) { // only one parameter for status
        status = atoi(params);
        return status;
    }
    return -1;
}

// used to parse command params in comma separated form, i.e.
// params = "p0,p1,p2,...,pN"
// parses up to N or max_params parameters
// returns the number of params parsed (and hence the number of params
// stored in param_arr)
int parse_params(char *params, char **param_arr, int max_params) {
    #define SEPARATOR ","
    char *p, *tmp;
    int i = 0;

    if (!params || !max_params || !param_arr)
        return 0;

    p = strtok_r(params, SEPARATOR, &tmp);
    while (p) {
        param_arr[i] = p;
        i++;
        if (i < max_params)
            p = strtok_r(NULL, SEPARATOR, &tmp);
        else
            break;
    }

    return i; // number of params parsed
}

static int do_serial_command(char *cmd_str, int *cmd_ret) {
    char *cmd, *params, *tmp;
    int c;
    int ret = -1;

    cmd = strtok(cmd_str, " \n");
    params = strtok(NULL, "\n");

    if (!cmd) {
        LOG_ERR("No command found");
        return ret;
    }
    c = strtol(cmd, &tmp, 16); // cmd number is in hex
    if (strcmp(cmd, tmp) == 0) {
        LOG_ERR("Command bits are invalid");
        return ret;
    }
    if (c >= 0 && c < sizeof(cmds)/sizeof(cmd_f) && cmds[c]) {
        // params may be NULL
        if (cmd_ret)
            *cmd_ret = cmds[c](params);
        else
            cmds[c](params);
        ret = c;
    } else {
        LOG_WARN("Serial cmd %d not implemented", c);
    }

    return ret;
}

int rx_serial_command(char c, int *cmd_ret) {
    #define BUFF_SIZE 64
    static char rx_buffer[BUFF_SIZE];
    static int i = 0;
    return rx_serial_command_r(c, rx_buffer, &i, BUFF_SIZE, cmd_ret);
}

// recieves serial command into rx_buffer given
// one character, c, at a time. Returns one of the following:
// >= 0  = command rx'd/completed successfully (ret is command code)
// -1 = command not yet fully recieved, keep passing in characters
// -2 = command not rx'd or failed
// Can also use *cmd_ret to get the return status of the command run
int rx_serial_command_r(char c, char *rx_buffer, int *save_i, int size, int *cmd_ret) {
    int ret;
    if (*save_i < size) {
        if (c == '\r' || c == '\n') {
            rx_buffer[*save_i] = '\0';
            ret = do_serial_command(rx_buffer, cmd_ret);
            if (ret < 0) {
                LOG_ERR("Serial cmd failed");
                ret = FAIL;
            }
            CLEAR_BUFF(rx_buffer, size, *save_i);
        } else {
            rx_buffer[*save_i] = c;
            (*save_i)++;
            ret = CONTINUE;
        }
    } else {
        LOG_ERR("Serial buffer overflow");
        CLEAR_BUFF(rx_buffer, size, *save_i);
        ret = FAIL;
    }

    return ret;
}
