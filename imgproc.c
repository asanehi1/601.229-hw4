#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "pnglite.c"
#include "image.c"

int main(int argc, char* argv[]) {
    if(argc > 6) {
        printf("ERROR: Too many arguments\n");
    }

    struct Image* image = img_read_png(argv[3]);
    
    if(argc == 2) {
        if(strcmp("list", argv[0]) == 0) {
            printf("Loaded 5 plugin(s)\n");
            /*
            
            NEED TO ITERATE THROUGH PLUGINS!
            
            */
            return 0;
        } else {
            printf("ERROR: Expected \"list\" as an input\n");
            return 1;
        }
    }

    if(argc >= 5) {
        if(strcmp("exec", argv[0]) != 0) {
            printf("ERROR: Expected \"exec\" as an input\n");
            return 1;
        }
    }

    if(argc == 5) {
        if(strcmp("swapbg", argv[1]) != 0 && strcmp("mirrorh", argv[1]) != 0 && strcmp("mirrorv", argv[1]) != 0) {
            printf("ERROR: 4 inputs expected\n");
            return 1;
        }
    }

    if(argc == 6) {
        if(strcmp("tile", argv[1]) == 0) {
           // tile_args = atoi(argv[5]);
        } else if(strcmp("expose", argv[1]) == 0) {
           // expose_args = atoi(argv[5]);
        } else {
            printf("ERROR: 6 inputs expected\n");
        }
    }

    return 0;
}