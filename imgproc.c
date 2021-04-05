#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "pnglite.h"
#include "image.h"

int main(int argc, char* argv[]) {
    if(argc > 6) {
      // I think u can have a lotttt of plugins
      //printf("ERROR: Too many arguments\n");
    }
    char* command = argv[1];

    struct Image* image = img_read_png(argv[3]);

    if (argc == 1) {
      printf("Usage: imgproc <command> [<command args...>]\n");
      printf("Commands are: \n");
      printf("  list\n");
      printf("  exec <plugin> <input img> <output img> [<plugin args...>]\n");
    }
    
    if(argc < 5) {
      if(strcmp(command, "list") == 0) {
            printf("Loaded 5 plugin(s)\n");
            /*
            
            NEED TO ITERATE THROUGH PLUGINS!
            
            */
	    printf("Loaded 5 plugin(s)\n");
	    printf(" mirrorh: mirror image horizontally\n");
	    printf(" mirrorv: mirror image vertically\n");
	    printf("  swapbg: swap blue and green color component values\n");
	    printf("    tile: tile source image in an NxN arrangement\n");
	    printf("  expose: adjust the intensity of all pixels");


            return 0;
        } else {
            printf("ERROR: Expected \"list\" as an input\n");
            return 1;
        }
    }

    char* plugin = argv[2];
    char* inputImg = argv[3];
    char* outputImg = argv[4];
    // argv[5] might be plugin args
    
    if(argc >= 5) {
      if(strcmp("exec", command) != 0) {
	  printf("ERROR: Expected \"exec\" as an input\n");
	  return 1;
	}
    }

    if(argc == 5) {
      if(strcmp("swapbg", plugin) == 0) {
	//do swapbg code
      } else if(strcmp("mirrorh", plugin) == 0) {
	//do mirrorh code
      } else if(strcmp("mirrorv", plugin) == 0) {
        //do mirrorv code
      }  else {
	printf("ERROR: 4 inputs expected\n");
	return 1;
      }
    }

    if(argc == 6) {
        if(strcmp("tile", plugin) == 0) {
           // tile_args = atoi(argv[5]);
        } else if(strcmp("expose", argv[2]) == 0) {
           // expose_args = atoi(argv[5]);
        } else {
            printf("ERROR: 6 inputs expected\n");
        }
    }

    return 0;
}
