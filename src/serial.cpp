#include "Particle.h"
#include "serial.h"
#ifdef SPARK
#include "serial_spark.h"
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

int do_serial_command(char *cmd_str) {
    char *cmd, *params, *tmp;
    int c;
    int ret = -1;

    cmd = strtok(cmd_str, " \n");
    params = strtok(NULL, "\n");

    if (!cmd) {
#ifdef SPARK
        Log.error("No command found");
#endif
        return ret;
    }
#ifdef SPARK
    Log.info("Got cmd %.4s", cmd);
#endif
    c = strtol(cmd, &tmp, 16); // cmd number is in hex
    if (strcmp(cmd, tmp) == 0) {
#ifdef SPARK
        Log.error("Command bits are invalid");
#endif
        return ret;
    }
#ifdef SPARK
    Log.info("cmd number: %d", c);
#endif
    if (c < sizeof(cmds) && cmds[c]) {
        ret = cmds[c](params); // params may be NULL
    } else {
#ifdef SPARK
        Log.warn("Serial cmd %d not implemented", c);
#endif
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
            if (do_serial_command(rx_buffer) == 0) {
#ifdef SPARK
                Log.trace("Serial cmd complete");
#endif
            } else {
#ifdef SPARK
                Log.error("Serial cmd failed");
#endif
            }
            CLEAR_BUFF(rx_buffer, SERIAL_BUFF_SIZE, i);
        } else {
            rx_buffer[i] = c;
            i++;
        }
    }
}
