#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <semaphore.h>
#include <errno.h>

#define SOCKET_PATH "/tmp/producer_consumer_socket"
#define SHM_NAME "/producer_consumer_shm"
#define MAX_MSG_SIZE 1024

// Shared memory structure
typedef struct {
    char buffer[MAX_MSG_SIZE];
} shared_mem_t;

// Global variables
int queue_depth = 0;
int echo = 0;
char *message = NULL;
int use_shared_memory = 0;
int is_producer = 0;

// Functions for shared memory mode
void producer_shared();
void consumer_shared();

// Functions for socket mode
void producer_socket();
void consumer_socket();

// Helper functions
void parse_arguments(int argc, char *argv[]);

#define SEM_EMPTY "/sem_empty"
#define SEM_FULL "/sem_full"
#define SEM_MUTEX "/sem_mutex"

void parse_arguments(int argc, char *argv[]) {
    int opt;
    while ((opt = getopt(argc, argv, "pcm:q:use")) != -1) {
        switch (opt) {
            case 'p':
                is_producer = 1;
                break;
            case 'c':
                is_producer = 0;
                break;
            case 'm':
                message = strdup(optarg);
                break;
            case 'q':
                queue_depth = atoi(optarg);
                if (queue_depth <= 0) {
                    fprintf(stderr, "Queue depth must be a positive integer.\n");
                    exit(EXIT_FAILURE);
                }
                break;
            case 'u':
                use_shared_memory = 0;
                break;
            case 's':
                use_shared_memory = 1;
                break;
            case 'e':
                echo = 1;
                break;
            default:
                fprintf(stderr, "Usage: %s -p|-c -m message -q queue_size -s|-u [-e]\n", argv[0]);
                exit(EXIT_FAILURE);
        }
    }

    if (queue_depth == 0) {
        fprintf(stderr, "Error: Queue size must be specified with -q.\n");
        exit(EXIT_FAILURE);
    }

    if (is_producer && message == NULL) {
        fprintf(stderr, "Error: Producer must specify a message with -m \"message\".\n");
        exit(EXIT_FAILURE);
    }
}

void producer_shared() {
    int shm_fd = shm_open(SHM_NAME, O_CREAT | O_RDWR, 0666);
    if (shm_fd == -1) {
        perror("shm_open");
        exit(EXIT_FAILURE);
    }

    ftruncate(shm_fd, sizeof(shared_mem_t));
    shared_mem_t *shm_ptr = mmap(0, sizeof(shared_mem_t), PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);
    if (shm_ptr == MAP_FAILED) {
        perror("mmap");
        exit(EXIT_FAILURE);
    }

    sem_t *empty = sem_open(SEM_EMPTY, O_CREAT, 0666, queue_depth);
    sem_t *full = sem_open(SEM_FULL, O_CREAT, 0666, 0);
    sem_t *mutex = sem_open(SEM_MUTEX, O_CREAT, 0666, 1);

    if (empty == SEM_FAILED || full == SEM_FAILED || mutex == SEM_FAILED) {
        perror("sem_open");
        exit(EXIT_FAILURE);
    }

    sem_wait(empty);
    sem_wait(mutex);

    strncpy(shm_ptr->buffer, message, MAX_MSG_SIZE);

    if (echo) {
        printf("Produced: %s\n", message);
        fflush(stdout);
    }

    sem_post(mutex);
    sem_post(full);

    munmap(shm_ptr, sizeof(shared_mem_t));
    close(shm_fd);
    sem_close(empty);
    sem_close(full);
    sem_close(mutex);

    sem_unlink(SEM_EMPTY);
    sem_unlink(SEM_FULL);
    sem_unlink(SEM_MUTEX);
    shm_unlink(SHM_NAME);
}

void consumer_shared() {
    int shm_fd = shm_open(SHM_NAME, O_RDWR, 0666);
    if (shm_fd == -1) {
        perror("shm_open");
        exit(EXIT_FAILURE);
    }

    shared_mem_t *shm_ptr = mmap(0, sizeof(shared_mem_t), PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);
    if (shm_ptr == MAP_FAILED) {
        perror("mmap");
        exit(EXIT_FAILURE);
    }

    sem_t *empty = sem_open(SEM_EMPTY, 0);
    sem_t *full = sem_open(SEM_FULL, 0);
    sem_t *mutex = sem_open(SEM_MUTEX, 0);

    if (empty == SEM_FAILED || full == SEM_FAILED || mutex == SEM_FAILED) {
        perror("sem_open");
        exit(EXIT_FAILURE);
    }

    sem_wait(full);
    sem_wait(mutex);

    if (echo) {
        printf("Consumed: %s\n", shm_ptr->buffer);
        fflush(stdout);
    }

    sem_post(mutex);
    sem_post(empty);

    munmap(shm_ptr, sizeof(shared_mem_t));
    close(shm_fd);
    sem_close(empty);
    sem_close(full);
    sem_close(mutex);
}

void producer_socket() {
    int server_fd = socket(AF_UNIX, SOCK_STREAM, 0);
    if (server_fd == -1) {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    struct sockaddr_un addr;
    memset(&addr, 0, sizeof(struct sockaddr_un));
    addr.sun_family = AF_UNIX;
    strncpy(addr.sun_path, SOCKET_PATH, sizeof(addr.sun_path) - 1);

    unlink(SOCKET_PATH);

    if (bind(server_fd, (struct sockaddr *)&addr, sizeof(struct sockaddr_un)) == -1) {
        perror("bind");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    if (listen(server_fd, 5) == -1) {
        perror("listen");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    int client_fd = accept(server_fd, NULL, NULL);
    if (client_fd == -1) {
        perror("accept");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    if (write(client_fd, message, strlen(message) + 1) == -1) {
        perror("write");
        close(client_fd);
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    if (echo) {
        printf("Produced (socket): %s\n", message);
        fflush(stdout);
    }

    close(client_fd);
    close(server_fd);
    unlink(SOCKET_PATH);
}

void consumer_socket() {
    int client_fd = socket(AF_UNIX, SOCK_STREAM, 0);
    if (client_fd == -1) {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    struct sockaddr_un addr;
    memset(&addr, 0, sizeof(struct sockaddr_un));
    addr.sun_family = AF_UNIX;
    strncpy(addr.sun_path, SOCKET_PATH, sizeof(addr.sun_path) - 1);

    while (connect(client_fd, (struct sockaddr *)&addr, sizeof(struct sockaddr_un)) == -1) {
        if (errno == ENOENT) {
            usleep(100000); // wait and retry
            continue;
        }
        perror("connect");
        close(client_fd);
        exit(EXIT_FAILURE);
    }

    char buf[MAX_MSG_SIZE];
    ssize_t num_read = read(client_fd, buf, sizeof(buf));
    if (num_read > 0) {
        if (echo) {
            printf("Consumed (socket): %s\n", buf);
            fflush(stdout);
        }
    } else {
        perror("read");
    }

    close(client_fd);
}