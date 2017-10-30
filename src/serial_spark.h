#ifndef __SERIAL_SPARK_H_
#define __SERIAL_SPARK_H_

int do_new_game(char *params);
int do_end_turn(char *params);
int do_move_piece(char *params);
int do_promote(char *params);
int do_calibrate(char *params);
int do_end_game(char *params);
int do_scan_wifi(char *params);
int do_set_wifi(char *params);

#endif /* __SERIAL_SPARK_H_ */
