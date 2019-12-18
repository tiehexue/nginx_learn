//
//  nginx_learn.c
//  nginx
//
//  Created by Yuan Wang on 2019/12/16.
//  Copyright © 2019 Yuan Wang. All rights reserved.
//

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <assert.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/fcntl.h>

#define PORT 80
#define MAX_URL_LENGTH 150

#define PANIC(func, ...) {                                 \
    int ret = func(__VA_ARGS__);                           \
    if (ret < 0) {                                         \
        printf("system call %s error: %d.\n", #func, ret); \
        exit(-1);                                          \
    }                                                      \
}

extern int hash_get_value(const char *path, u_long length);

const char *html_root = 0;
const char *status_header = "HTTP/1.0 200 OK\nServer: nginx_learn\n";
const char *html_content_type[] = {
    "Content-Type: text/html;charset=utf-8\n\n",
    "Content-Type: application/javascript;charset=utf-8\n\n",
    "Content-Type: text/css;charset=utf-8\n\n",
    "Content-Type: image/png;charset=utf-8\n\n",
    "Content-Type: image/jpg;charset=utf-8\n\n"
};

struct stat filesize(int fd) {
    struct stat st;
    assert(fstat(fd, &st) != -1);
    return st;
}

const char * map_uri_to_path(char *uri, char *path) {
    
    
    
    if (uri[5] == ' ') {
        strncpy(path, "/index.html", 11);
    } else {
        char *http = strstr(uri, "HTTP");
        strncpy(path, uri + 4, (int)(http - uri - 5));
    }
    
    const char *postfix = strrchr(path, '.');
    
    if (!strcmp(postfix, ".js")) {
        return html_content_type[1];
    } else if (!strcmp(postfix, ".css")) {
        return html_content_type[2];
    } else if (!strcmp(postfix, ".png")){
        return html_content_type[3];
    } else if (!strcmp(postfix, ".jpg")){
        return html_content_type[4];
    } else {
        return html_content_type[0];
    }
}

int main(int argc, const char * argv[]) {
    
    if (argc < 2) {
        printf("need to specify index file.\n");
        exit(-1);
    } else {
        html_root = argv[1];
    }
    
    int server_socket = socket(AF_INET, SOCK_STREAM, 0);
    
    struct sockaddr_in server_addr;
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    server_addr.sin_port = htons(PORT);
    
    PANIC(bind, server_socket, (struct sockaddr *)&server_addr, sizeof(server_addr));
    
    PANIC(listen, server_socket, 128);
    
    printf("ready to serve.\n");
    
    int client_socket = 0;
    
    while ((client_socket = accept(server_socket, NULL, NULL))) {
        
        char buf[1024] = {0};
        read(client_socket, buf, 1024);
        buf[1023] = 0;
        //printf("%s", buf);
        
        if (strlen(buf)) {
        
            char path[MAX_URL_LENGTH] = {0};
            const char *content_type = map_uri_to_path(buf, path);
            
            int fd = hash_get_value(path, strlen(path));
            if (fd > 0) {
                struct stat st = filesize(fd);
                
                write(client_socket, status_header, strlen(status_header));
                write(client_socket, content_type, strlen(content_type));
                
                sendfile(fd, client_socket, 0, (off_t *)&st.st_size, NULL, 0);
            } else {
                char *status = "HTTP/1.0 404 OK\n";
                write(client_socket, status, strlen(status));
            }
            
            close(client_socket);
        }
    }
    
    close(server_socket);
    
    return 0;
}
