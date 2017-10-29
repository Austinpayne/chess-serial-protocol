#ifndef __SERIAL_H_
#define __SERIAL_H_

#define CMD_FMT  "%01x"
#define MOVE_FMT "%.4s"
#define CMD_STATUS     0x0
#define CMD_NEW_GAME   0x1
#define CMD_END_TURN   0x2
#define CMD_MOVE_PIECE 0x3
#define CMD_PROMOTE    0x4
#define CMD_CALIBRATE  0x5
#define CMD_END_GAME   0x6
#define SCAN_WIFI      0x7
#define SET_WIFI       0x8

#define OK 0

#define CLEAR_BUFF(b,s,i)  memset(b, 0, s); i = 0
// use SEND_CMD_P in other macros, PF is format specifier
#ifdef SPARK
#define SEND_CMD(c)        Serial1.printlnf(CMD_FMT, (c))
#define SEND_CMD_P(c,PF,p) Serial1.printlnf(CMD_FMT " " PF, (c), (p))
#else // assume stm32
#define SEND_CMD(c)        printf(CMD_FMT "\n", (c))
#define SEND_CMD_P(c,PF,p) printf(CMD_FMT " " PF "\n", (c), (p))
#endif
#define SEND_MOVE(m)       SEND_CMD_P(CMD_MOVE_PIECE, MOVE_FMT, (m))

typedef int (*cmd_f)(char *);

int do_status(char *params);
int do_new_game(char *params);
int do_end_turn(char *params);
int do_promote(char *params);
int do_end_game(char *params);
int do_scan_wifi(char *params);
int do_set_wifi(char *params);
int do_serial_command(char *cmd_str);
void rx_serial_command(char c);

#endif /* __SERIAL_H_ */
