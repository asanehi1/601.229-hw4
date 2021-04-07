#include <stdio.h>
#include <stdlib.h>
#include "image_plugin.h"
#include <string.h>
#include <ctype.h>

struct Arguments {
	int tiling_factor;
};

const char *get_plugin_name(void) {
	return "tile";
}

const char *get_plugin_desc(void) {
	return "tile source image in an NxN arrangement";
}

int isDigit(char args[]) {
	for (size_t i = 0; i < strlen(args); i++) {
		if(!isdigit(args[i])) {
			return 1;
		}
	}
	return 0;
}

void *parse_arguments(int num_args, char *args[]) {
	(void) args; // this is just to avoid a warning about an unused parameter

	if (num_args != 1) {
		return NULL;
	}

	if(isDigit(args[0]) == 1) {
		return NULL;
	}

    struct Arguments *tile_arg = calloc(1,sizeof(struct Arguments));
    tile_arg->tiling_factor = atoi(args[0]);

    return tile_arg;
}

struct Image *transform_image(struct Image *source, void *arg_data) {
    struct Arguments *args = arg_data;
    // Allocate a result Image                                                                                                                                                                
        struct Image *out = img_create(source->width, source->height);
        if (!out) {
                free(args);
                return NULL;
        }

        unsigned width = source->width;
        unsigned height = source->height;
        int n = args->tiling_factor;

        unsigned mini_w = width / n;
        unsigned mini_h = height / n;

        int excess_w = width % n;
        int excess_h = height % n;

        unsigned h = 0,  w = 0;
        unsigned exH = 0, exW = 0;

        int x = 0;

        for (int a = 0; a < n; a++) {
          if (excess_h > 0) {
            excess_h --;
            h = mini_h + 1;
          } else {
            h = mini_h;
          }
          for(int b = 0; b < n; b++) {
            if (excess_w > 0) {
              printf("%d\n", excess_w);
              excess_w --;
              exW ++;
              w = mini_w + 1;
              x = 1;
            } else {
              w = mini_w;
            }
            for (unsigned i = 0; i < h; i++) {
              for (unsigned j = 0; j < w; j++) {
                out->data[(a * mini_h * width) + (b * (mini_w + x)) + j + i * width] = source->data[(i * width * n) + j * n];
              }
            }
          }
          excess_w = exW;
          exW = 0;
          x = 0;

        }


    free(args);
    return out;
}


