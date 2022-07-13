#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "main.h"
#include "util.h"
#include "graph.h"

static int graph_badness(void);
static int graph_dist(int i, int j);
static int graph_find(int v1, int v2);
static void q_push(int val);
static int q_pop(void);
static int q_len(void);
static void q_clear(void);

static int graph[N][D] = {0};
static int queue[N + 1] = {0};
static int queue_r = 0;
static int queue_w = 0;
static int dist[N] = {0};

BOOL graph_init(void) {
    int i, j;
    for (i = 0; i < N; i++) {
        int val = (i + N - D / 2) % N;
        for (j = 0; j < D; j++) {
            graph[i][j] = val;
            val = (val + 1) % N;
            if (val == i) {
                val = (val + 1) % N;
            }
        }
    }
    return TRUE;
}

BOOL graph_solve(void) {
    int old, new;
    old = graph_badness();
    printf("badness = %d\n", old);
    while (old) {
        int v1, v2, v3, v4, p1, p2, p3, p4;
        if (timer_istimeout()) {
            return FALSE;
        }
        v1 = rand() % N;
        p1 = rand() % D;
        v2 = graph[v1][p1];
        p2 = graph_find(v2, v1);
        do {
            v3 = rand() % N;
            p3 = rand() % D;
            v4 = graph[v3][p3];
            p4 = graph_find(v4, v3);
        } while ((v1 == v3) || (v1 == v4) || (v2 == v3) || (v2 == v4));
        graph[v1][p1] = v4;
        graph[v2][p2] = v3;
        graph[v3][p3] = v2;
        graph[v4][p4] = v1;
        new = graph_badness();
        if (old < new) {
            graph[v1][p1] = v2;
            graph[v2][p2] = v1;
            graph[v3][p3] = v4;
            graph[v4][p4] = v3;
            new = old;
        }
        old = new;
    }
    return TRUE;
}

int graph_badness(void) {
    int i, j, d, badness = 0;
    int dist[N] = {0};
    for (i = 0; i < N - 1; i++) {
        for (j = i + 1; j < N; j++) {
            d = graph_dist(i, j);
            dist[d] = dist[d] + 1;
            if (d > K) {
                badness++;
            }
        }
    }
#if 0
    for (i = 0; i < N; i++) {
        if (dist[i])
            printf("%03d:%03d ", i, dist[i]);
    }
    printf("\n");
#endif
    return badness;
}

int graph_dist(int i, int j) {
    memset(dist, 0xFF, sizeof(dist));
    q_clear();
    dist[i] = 0;
    q_push(i);
    while (q_len()) {
        int v = q_pop();
        int d = dist[v];
        int p, q;
        for (p = 0; p < D; p++) {
            q = graph[v][p];
            if (q == j) {
                return d + 1;
            }
            if (dist[q] == -1) {
                dist[q] = d + 1;
                q_push(q);
            }
        }
    }
    printf("***** error (%d, %d) *****\n", i, j);  // ここには来ない
    exit(1);
    return 0;
}

int graph_find(int v1, int v2) {
    int i;
    for (i = 0; i < D; i++) {
        if (graph[v1][i] == v2) {
            return i;
        }
    }
    return -1;
}

void q_push(int val) {
    queue[queue_w] = val;
    queue_w = (queue_w + 1) % (N + 1);
}

int q_pop(void) {
    int val = queue[queue_r];
    queue_r = (queue_r + 1) % (N + 1);
    return val;
}

int q_len(void) {
    return (queue_w + N + 1 - queue_r) % (N + 1);
}

void q_clear(void) {
    queue_r = queue_w = 0;
}

void print_graph(void) {
    int i, j, d;
    int dist[N] = {0};
    printf("N = %03d\n", N);
    for (i = 0; i < N; i++) {
        printf("[%03d]", i);
        for (j = 0; j < D; j++) {
            printf(" %03d", graph[i][j]);
        }
        printf("\n");
    }
    for (i = 0; i < N - 1; i++) {
        for (j = i + 1; j < N; j++) {
            d = graph_dist(i, j);
            dist[d] = dist[d] + 1;
        }
    }
    for (i = 0; i < N; i++) {
        if (dist[i]) {
            printf("dist %03d: %03d\n", i, dist[i]);
        }
    }
}