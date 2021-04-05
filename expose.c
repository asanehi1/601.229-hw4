#include <stdlib.h>
#include "image_plugin.h"

struct Arguments {
	float expose_factor;
};

const char *get_plugin_name1(void) {
	return "expose";
}

const char *get_plugin_desc1(void) {
	return "adjust the intensity of all pixels";
}

void *parse_arguments1(int num_args, char *args[]) {
	(void) args; // this is just to avoid a warning about an unused parameter

	if (num_args != 1) {
		return NULL;
	}


    struct Arguments *tile_arg = calloc(1,sizeof(struct Arguments));
    //need to convert to float
    //tile_arg->expose_factor = args[5];

    return tile_arg;
}

struct Image *transform_image1(struct Image *source, void *arg_data) {
    struct Arguments *args = arg_data;
    // Allocate a result Image
	struct Image *out = img_create(source->width, source->height);
	if (!out) {
		free(args);
		return NULL;
	}

    free(args);
    return out;
}
