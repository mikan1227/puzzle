#ifndef _UTIL_H_
#define _UTIL_H_

#include <time.h>

void *timer_func(void *arg);
void timer_start(int sec);
void timer_stop(void);
void timer_exit(void);
BOOL timer_istimeout(void);
void print_time(const char *restrict s, time_t timeval);
void print_elapsed(time_t start, time_t end);
#endif /* _UTIL_H_ */
