#include <stdio.h>
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
    tile_arg->tiling_factor = atoi(args[0]);

    return tile_arg;
}

unsigned get_excess(unsigned *excess, unsigned *start, unsigned *length) {
	if(start == length - 1) {
		if(excess > 0) {
			excess = excess - 1;
			start = 0;
			return 1;
		}
	}

	return 0;
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

	unsigned mini_w = width / args->tiling_factor;
	unsigned mini_h = height / args->tiling_factor;

	unsigned excess_w = width % args->tiling_factor;
	unsigned excess_h = height % args->tiling_factor;

	unsigned h, w;
	unsigned start_width = 0, start_height = 0;

	printf("HERE\n");

	for (unsigned row = 0; row < width; row++) {
		w = get_excess(&excess_w, &start_width, &mini_w);

		for (unsigned col = 0; col < height; col++) {
			h = get_excess(&excess_h, &start_height, &mini_h);

			unsigned s_index = args->tiling_factor * (col + row);
			unsigned d_index = width * (row + w) + (col + h);

			out->data[d_index] = source->data[s_index];
			start_height = start_height + 1;
		}

		start_width = start_width + 1;
	}


    free(args);
    return out;
}