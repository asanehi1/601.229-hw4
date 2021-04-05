#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <dirent.h>
#include "pnglite.h"
#include "image.h"

struct Plugin {
    void *handle;
    const char *(*get_plugin_name)(void);
    const char *(*get_plugin_desc)(void);
    void *(*parse_arguments)(int num_args, char *args[]);
    struct Image *(*transform_image)(struct Image *source, void *arg_data);
};

struct Plugin * init_plugin(void) {
  struct Plugin *newPlug;
  const char * plugDirect;

  if (getenv("PLUGIN_DIR") != NULL) {
    // we assume it has pathname of plugin direct
    plugDirect = getenv("PLUGIN_DIR");
  } else {
    // assume that the plugin shared libraries are in the ./plugins directory
    plugDirect = "./plugins";
  }
  // once we have plugin directory path
  DIR *direct = opendir(plugDirect);
  // use readdir to find all of the files in the plugin directory that end in ".so"
  // dynamically load all of the files ending in ".so" with dlopen
  // use dlsym to find the addresses of the plugin’s get_plugin_name, get_plugin_desc, parse_arguments, and transform_image funcs
  // save these into struct Plugin *newPLug
  

  closedir(direct);

  return newPlug;
  
}

int main(int argc, char* argv[]) {
    if(argc > 6) {
      // I think u can have a lotttt of plugins
      //printf("ERROR: Too many arguments\n");
    }
    char* command = argv[1];

    if (argc == 1) {
      printf("Usage: imgproc <command> [<command args...>]\n");
      printf("Commands are: \n");
      printf("  list\n");
      printf("  exec <plugin> <input img> <output img> [<plugin args...>]\n");
    }

    // HERES WHERE ALL THE IMPORTANT INFO IS 
    struct Plugin * plugList = init_plugin();
    
    if(argc < 5) {
      if(strcmp(command, "list") == 0) {
            /*
            
            NEED TO ITERATE THROUGH PLUGINS!
	   
            */
	    
	    return 0;
        } else {
            printf("ERROR: Expected \"list\" as an input\n");
            return 1;
        }
    }

    char* plugin = argv[2];
    struct Image* inputImg = img_read_png(argv[3]);
    struct Image* outputImg = img_read_png(argv[4]);
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
