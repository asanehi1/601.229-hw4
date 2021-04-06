#include <stdlib.h>
#include "image_plugin.h"

struct Arguments {
	int tiling_factor;
};

const char *get_plugin_name(void) {
	return "tile";
}

const char *get_plugin_desc(void) {
	return "tile source image in an NxN arrangement";
}

void *parse_arguments(int num_args, char *args[]) {
	(void) args; // this is just to avoid a warning about an unused parameter

	if (num_args != 1) {
		return NULL;
	}

    struct Arguments *tile_arg = calloc(1,sizeof(struct Arguments));
    tile_arg->tiling_factor = atoi(args[5]);

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

	  //unsigned num_pixels = source->width * source->height;
	unsigned width = source->width;
	unsigned height = source->height;
	for (unsigned row = 0; row < width; row++) {
		for (unsigned col = 0; col < height; col++) {
			
		}
	}


    free(args);
    return out;
}