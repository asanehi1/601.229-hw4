#include <stdlib.h>
//#include "image.h"
#include "image_plugin.h"
#include <stdio.h>

struct Arguments {
	// This plugin doesn't accept any command line arguments;
	// just define a single dummy field.
	int dummy;
};

const char *get_plugin_name(void) {
	return "mirrorh";
}

const char *get_plugin_desc(void) {
	return "mirror image horizontally";
}

void *parse_arguments(int num_args, char *args[]) {
	(void) args; // this is just to avoid a warning about an unused parameter

	if (num_args != 0) {
		return NULL;
	}
	return calloc(1, sizeof(struct Arguments));
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
	for (unsigned row = 0; row < height; row++) {
		for (unsigned col = 0; col < width; col++) {
			
			unsigned d_index = row * width + col;
			unsigned s_index = row * width + (width - 1 - col);
			out->data[d_index] = source->data[s_index];
		}
	}

    free(args);
    return out;
}
