/*
 * command.h
 *
 * Created: 5/19/2024 8:38:57 PM
 *  Author: Admin
 */ 


#ifndef COMMAND_H_
#define COMMAND_H_

#include "cmdline.h"
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#define	COMMAND_MAX_LENGTH	32
#define MAX_PRECISION	(2)
#define ROUND(x) ((x) >= 0 ? (int)(x + 0.5) : (int)(x - 0.5))


void command_init(void);
void command_task_update(void*);
int32_t Map(int32_t x, int32_t in_min, int32_t in_max, int32_t out_min, int32_t out_max);
int	Cmd_help(int argc, char *argv[]);

char * ftoa(double f, char * buf, int precision);
int Cmd_set_laser(int argc, char *argv[]);
int Cmd_get_current(int argc, char *argv[]);
int Cmd_pd_get(int argc, char *argv[]);
int Cmd_sample_set(int argc, char *argv[]);
int Cmd_sample_trig(int argc, char *argv[]);
int Cmd_sample_status_get(int argc, char *argv[]);
int Cmd_sample_get(int argc, char *argv[]);
int Cmd_sample_get_char(int argc, char *argv[]);

#endif /* COMMAND_H_ */
