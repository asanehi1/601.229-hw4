#include <stdlib.h>
#include "image_plugin.h"

struct Arguments {
	// This plugin doesn't accept any command line arguments;
	// just define a single dummy field.
	int dummy;
};

const char *get_plugin_name(void) {
	return "mirrorv";
}

const char *get_plugin_desc(void) {
	return "mirror image vertically";
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
	for (unsigned row = 0; row < width; row++) {
		for (unsigned col = 0; col < height; col++) {
			uint8_t r, g, b, a;
			img_unpack_pixel(source->data[width * row + col], &r, &g, &b, &a);
			//need help to calculate the upside down index
			out->data[row] = img_pack_pixel(r, g, b, a);
		}
	}

    free(args);
    return out;
}