#ifndef TICKPACK_H
#define TICKPACK_H
#include <cpu.h>

/* Start ticking service that calls app_tick_callback every interval usecs */
void init_ticks(int interval, IntHandler *app_tick_callback);

/* Shut down ticking service */
void shutdown_ticks(void);

#endif
