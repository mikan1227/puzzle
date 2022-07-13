#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <time.h>
#include "main.h"
#include "util.h"

#define TIMER_PERIOD 500000

typedef enum {
    timer_nop,
    timer_run,
    timer_end,
} timer_mode_t;

static timer_mode_t timer_mode = timer_nop;
static BOOL timer_timeout = FALSE;
static time_t timer_start_time = 0;
static int timer_set_sec = 0;

void *timer_func(void *arg) {
    while (timer_mode != timer_end) {
        usleep(TIMER_PERIOD);
        if (timer_mode == timer_end) {
            break;
        }
        else if (timer_mode == timer_run) {
            time_t now = time(NULL);
            if (now - timer_start_time >= timer_set_sec) {
                timer_timeout = TRUE;
                timer_mode = timer_nop;
            }
        }
    }
    pthread_exit(NULL);
}

void timer_start(int sec) {
    timer_set_sec = sec;
    timer_start_time = time(NULL);
    timer_timeout = FALSE;
    timer_mode = timer_run;
}

void timer_stop(void) {
    timer_mode = timer_nop;
    timer_timeout = FALSE;
}

void timer_exit(void) {
    timer_mode = timer_end;
}

BOOL timer_istimeout(void) {
    return timer_timeout;
}

void print_time(const char *restrict s, time_t timeval) {
    char buf[64];
    struct tm *localtm = localtime(&timeval);
    strftime(buf, sizeof(buf), "%Y/%m/%d %H:%M:%S", localtm);
    printf("%s%s\n", s, buf);
}

void print_elapsed(time_t start, time_t end) {
    int hour, minute, second;
    hour = (end - start) / 3600;
    minute = (end - start) / 60 % 60;
    second = (end - start) % 60;
    printf("elapsed time: %02d:%02d:%02d\n", hour, minute, second);
}