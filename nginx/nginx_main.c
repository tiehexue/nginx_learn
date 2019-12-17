//
//  main.c
//  nginx
//
//  Created by Yuan Wang on 2019/12/13.
//  Copyright © 2019 Yuan Wang. All rights reserved.
//

#include <stdio.h>

extern int nginx_main(int argc, char *const *argv);

int main(int argc, const char * argv[]) {
    return nginx_main(argc, argv);
}
