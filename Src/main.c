#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>
#include "main.h"
#include "util.h"
#include "graph.h"

int main(int argc, char **argv) {
    int ret;
    time_t start, end;
    pthread_t timer_thread;
    ret = pthread_create(&timer_thread, NULL, timer_func, NULL);
    if (ret) {
        perror("pthread_create");
        exit(EXIT_FAILURE);
    }

    start = time(NULL);
    print_time("start   time: ", start);

    for (;;) {
        srand((unsigned int)time(NULL));
        for (;;) {
            timer_start(GRAPH_INIT_TIMEOUT);
            if (graph_init()) {
                timer_stop();
                break;
            }
        }
        timer_start(GRAPH_SOLVE_TIMEOUT);
        if (graph_solve()) {
            print_graph();
            break;
        }
    }

    end = time(NULL);
    print_time("end     time: ", end);
    
    timer_exit();
    ret = pthread_join(timer_thread, NULL);
    if (ret) {
        perror("pthread_join");
        exit(EXIT_FAILURE);
    }

    print_elapsed(start, end);
    return 0;
}