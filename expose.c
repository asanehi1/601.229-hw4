#include <stdio.h>
#include <stdlib.h>
#include "image_plugin.h"
#include <string.h>
#include <ctype.h>

struct Arguments {
	float expose_factor;
};

const char *get_plugin_name(void) {
	return "expose";
}

const char *get_plugin_desc(void) {
	return "adjust the intensity of all pixels";
}

int isFloat(char args[]) {
	for (size_t i = 0; i < strlen(args); i++) {
		if(!isdigit(args[i]) && args[i] != '.') {
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

	if(isFloat(args[0]) == 1) {
		return NULL;
	}


    struct Arguments *tile_arg = calloc(1,sizeof(struct Arguments));
    tile_arg->expose_factor = atof(args[0]);

    return tile_arg;
}

static uint32_t calc_color_component(uint32_t pix, float factor) {
	uint8_t r, g, b, a;
	img_unpack_pixel(pix, &r, &g, &b, &a);
	if(factor * r > 255) {
		r = 255;
	} else {
		r = factor * r;
	}

	if(factor * g > 255) {
		g = 255;
	} else {
		g = factor * g;
	}

	if(factor * b > 255) {
		b = 255;
	} else {
		b = factor * b;
	}

	return img_pack_pixel(r, g, b, a);
}

struct Image *transform_image(struct Image *source, void *arg_data) {
    struct Arguments *args = arg_data;
    // Allocate a result Image
	struct Image *out = img_create(source->width, source->height);
	if (!out) {
		free(args);
		return NULL;
	}

	unsigned num_pixels = source->width * source->height;
	for (unsigned i = 0; i < num_pixels; i++) {
		out->data[i] = calc_color_component(source->data[i], args->expose_factor);
	}

    free(args);
    return out;
}
