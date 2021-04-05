#include <stdlib.h>
//#include "image.h"
#include "image_plugin.h"

struct Arguments {
	// This plugin doesn't accept any command line arguments;
	// just define a single dummy field.
	int dummy;
};

const char *get_plugin_name2(void) {
	return "mirrorh";
}

const char *get_plugin_desc2(void) {
	return "mirror image horizontally";
}

void *parse_arguments2(int num_args, char *args[]) {
	(void) args; // this is just to avoid a warning about an unused parameter

	if (num_args != 0) {
		return NULL;
	}
	return calloc(1, sizeof(struct Arguments));
}

struct Image *transform_image2(struct Image *source, void *arg_data) {
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
			uint8_t r, g, b, a;
			img_unpack_pixel(source->data[width * row + col], &r, &g, &b, &a);
			//need help to calculate the reverse index
			out->data[row] = img_pack_pixel(r, g, b, a);
		}
	}

    free(args);
    return out;
}
