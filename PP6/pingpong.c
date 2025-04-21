#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <signal.h>
#include <time.h>
#include <errno.h>

static volatile sig_atomic_t done = 0;
static pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
static pthread_cond_t  cond12 = PTHREAD_COND_INITIALIZER;
static pthread_cond_t  cond21 = PTHREAD_COND_INITIALIZER;
static int turn = 0;  // 1 => thread1’s turn, 2 => thread2’s turn

static void sigint_handler(int sig) {
    done = 1;
}

static void *thread1_func(void *arg) {
    pthread_mutex_lock(&mutex);
    // initial ping
    printf("thread 1: ping thread 2\n");
    fflush(stdout);
    turn = 2;
    pthread_cond_signal(&cond12);

    while (!done) {
        struct timespec ts;
        clock_gettime(CLOCK_REALTIME, &ts);
        ts.tv_sec += 1;
        // wait until it's our turn or timeout
        while (turn != 1 && !done) {
            if (pthread_cond_timedwait(&cond21, &mutex, &ts) == ETIMEDOUT)
                break;
        }
        if (done || turn != 1) break;
        printf("thread 1: pong! thread 2 ping received\n");
        fflush(stdout);
        printf("thread 1: ping thread 2\n");
        fflush(stdout);
        turn = 2;
        pthread_cond_signal(&cond12);
    }

    pthread_mutex_unlock(&mutex);
    return NULL;
}

static void *thread2_func(void *arg) {
    pthread_mutex_lock(&mutex);

    while (!done) {
        struct timespec ts;
        clock_gettime(CLOCK_REALTIME, &ts);
        ts.tv_sec += 1;
        // wait until it's our turn or timeout
        while (turn != 2 && !done) {
            if (pthread_cond_timedwait(&cond12, &mutex, &ts) == ETIMEDOUT)
                break;
        }
        if (done || turn != 2) break;
        printf("thread 2: pong! thread 1 ping received\n");
        fflush(stdout);
        printf("thread 2: ping thread 1\n");
        fflush(stdout);
        turn = 1;
        pthread_cond_signal(&cond21);
    }

    pthread_mutex_unlock(&mutex);
    return NULL;
}

int main(void) {
    struct sigaction sa = { .sa_handler = sigint_handler };
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    sigaction(SIGINT, &sa, NULL);

    pthread_t t1, t2;
    if (pthread_create(&t1, NULL, thread1_func, NULL) != 0) {
        perror("pthread_create");
        exit(1);
    }
    if (pthread_create(&t2, NULL, thread2_func, NULL) != 0) {
        perror("pthread_create");
        exit(1);
    }

    pthread_join(t1, NULL);
    pthread_join(t2, NULL);
    return 0;
}