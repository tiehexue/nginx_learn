//
//  event_linux.c
//  nginx
//
//  Created by Yuan Wang on 2019/12/20.
//  Copyright © 2019 Yuan Wang. All rights reserved.
//

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/sendfile.h>

#include "one.h"

void event_loop(int server_socket) {
    
    PANIC(listen, server_socket, 2048);
    
    int client_socket = 0;
    while ((client_socket = accept(server_socket, NULL, 0))) {
        int fd = send_header(client_socket);
        if (fd > 0) {
            struct stat st;
            fstat(fd, &st);
            
            off_t offset = 0;
            int sent = sendfile(client_socket, fd, &offset, st.st_size);
            if (!sent) {
                printf("sendfile %ld bytes to %d with errono %d", offset, client_socket, errno);
            }
            
            close(client_socket);
        }
    }
}
