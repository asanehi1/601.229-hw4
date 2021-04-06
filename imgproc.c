#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <dirent.h>
#include <dlfcn.h>
#include "pnglite.h"
#include "image.h"
#include "image_plugin.h"

struct Plugin {
    void *handle;
    const char *(*get_plugin_name)(void);
    const char *(*get_plugin_desc)(void);
    void *(*parse_arguments)(int num_args, char *args[]);
    struct Image *(*transform_image)(struct Image *source, void *arg_data);
};



// if plugin param is "list", printf all the needed values
struct Plugin * init_plugin() {
  // on piazza they said no more than 15
  struct Plugin *newPlug = (struct Plugin *) malloc(15 * sizeof(struct Plugin));  
  char *plugDirect;

  if (getenv("PLUGIN_DIR") != NULL) {
    // we assume it has pathname of plugin direct
    plugDirect = getenv("PLUGIN_DIR");
  } else {
    // assume that the plugin shared libraries are in the ./plugins directory
    plugDirect = "./plugins";
  }
  
  DIR *direct = opendir(plugDirect);
  //open directory with plugin direct path
  if (direct  == NULL) {
    printf("Error: Not valid directory");
  }

  struct dirent* read;
  // i is counter for structs
  int i = 0;
  
  // use readdir to find all of the files in the plugin directory that end in ".so"
  while ((read = readdir(direct)) != NULL) {
    // this code checks if it has .so in file name
    char *period = strrchr(read->d_name, '.');
    if (period && !strcmp(period, ".so")) {
      printf("found a .so file!\n");

      // dynamically load all of the files ending in ".so" with dlopen
      // must load whole path name (not just .so file name)
       char *filePath = (char *) calloc(256, sizeof(char));
       strcat(filePath, plugDirect);
       strcat(filePath, "/");
       strcat(filePath, read->d_name);
       printf("%s\n", filePath);
       
       (newPlug + i)->handle = dlopen(filePath, RTLD_LAZY);
       
       //*(void **) &((newPlug + i)->get_plugin_name) = dlsym((newPlug + i)->handle, "get_plugin_name");
       //printf((newPlug + i)->get_plugin_name());
       
       dlsym((newPlug + i)->handle, "get_plugin_name");
       dlsym((newPlug + i)->handle, "get_plugin_desc");
       dlsym((newPlug + i)->handle, "parse_arguments");
       dlsym((newPlug + i)->handle, "transform_image");
       // these should be saved into newPlug (TODO, asked on piazza)
              
       dlclose((newPlug + i)->handle);
       i++;
       free(filePath);
      }
  }
  

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

    // array of plugins
    struct Plugin * plugList = init_plugin();
    
    if(argc < 5) {
      if(strcmp(command, "list") == 0) {
	//iterate thru plugList, doens't work til I cast correctly in plug_init func (asked on piazza)
	/*
	int i = 0;
	while ((plugList+i)->get_plugin_name() != '\0') {
	  printf("%s: %s\n", (plugList + i)->get_plugin_name(),  (plugList + i)->get_plugin_desc());
	  i++;
	}
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
