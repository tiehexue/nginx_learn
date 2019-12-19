//
//  one.h
//  nginx_learn
//
//  Created by Yuan Wang on 2019/12/19.
//  Copyright © 2019 Yuan Wang. All rights reserved.
//

#ifndef one_h
#define one_h

#define PORT 80
#define MAX_URL_LENGTH 150
#define INITIAL_SIZE 100

#define PANIC(func, ...) {                                 \
    int ret = func(__VA_ARGS__);                           \
    if (ret < 0) {                                         \
        printf("system call %s error: %d.\n", #func, ret); \
        exit(-1);                                          \
    }                                                      \
}

typedef struct {
    int fd;
    off_t offset;
} fd_offset;

extern const char *html_root;

void init_cache(void);
int hash_get(const char *path, u_long length);

int send_header(int client_socket);

#endif /* one_h */
