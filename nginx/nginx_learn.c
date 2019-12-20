//
//  nginx_learn.c
//  nginx
//
//  Created by Yuan Wang on 2019/12/16.
//  Copyright © 2019 Yuan Wang. All rights reserved.
//
#include <stdio.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <sys/socket.h>

#include "one.h"

const char *html_root = NULL;

static int create_socket() {
    int server_socket = socket(AF_INET, SOCK_STREAM, 0);
    
    struct sockaddr_in server_addr;
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    server_addr.sin_port = htons(PORT);
    
    PANIC(bind, server_socket, (struct sockaddr *)&server_addr, sizeof(server_addr));
    
    printf("ready to serve.\n");
    
    return server_socket;
}

int main(int argc, const char * argv[]) {
    
    if (argc < 2) {
        printf("need to specify root dir.\n");
        exit(-1);
    } else {
        html_root = argv[1];
    }
    
    init_cache();
    
    int server_socket = create_socket();
    
    event_loop(server_socket);
    
    return 0;
}
