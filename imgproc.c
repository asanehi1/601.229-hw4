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

void delete(struct Plugin ** plugList, int plugin_size) {
  for(int i = 0; i < plugin_size; i += sizeof(struct Plugin*)) {
    dlclose((*(plugList+i))->handle);
    //free(*(plugList+i));
  }

  free(plugList);
}

// if plugin param is "list", printf all the needed values                                                                                                                                                  
struct Plugin ** init_plugin(int * num_plugin) {
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
    printf("Error: Not valid directory\n");
    return NULL;
  }

  struct dirent* read;

  // on piazza they said no more than 15                                                                                                                                                                    
  // struct Plugin *newPlug = malloc(15 * sizeof(struct Plugin));
  struct Plugin **newPlug = malloc(sizeof(struct Plugin *));
  if(!newPlug) {
    printf("Error: Could not allocate Plugin\n");
    closedir(direct);
    return NULL;
  }

  // i is counter for structs                                                                                                                                                                               
  int i = 0;

  // use readdir to find all of the files in the plugin directory that end in ".so"                                                                                                                         
  while ((read = readdir(direct)) != NULL) {
    // this code checks if it has .so in file name
                                                                                                                                     
    char *period = strrchr(read->d_name, '.');
    if (period && !strcmp(period, ".so")) {

      // dynamically load all of the files ending in ".so" with dlopen                                                                                                                                      
      // must load whole path name (not just .so file name)                                                                                                                                                 
      char *filePath = (char *) calloc(256, sizeof(char));
      if(!filePath) {
        printf("Error: Could not allocate filepath\n");
        delete(newPlug, i);
        closedir(direct);
        return NULL;
      }

      strcat(filePath, plugDirect);
      strcat(filePath, "/");
      strcat(filePath, read->d_name);                                                                                                                                                                       

      void * handle = dlopen(filePath, RTLD_LAZY);
      struct  Plugin *p = malloc(sizeof(struct Plugin));

      if (dlsym(handle, "get_plugin_name") != NULL) {
        (p)->handle = handle;

        *(void **) &((p)->get_plugin_name) = dlsym((p)->handle, "get_plugin_name");
        *(void **) &((p)->get_plugin_desc) = dlsym((p)->handle, "get_plugin_desc");
        *(void **) &((p)->parse_arguments) = dlsym((p)->handle, "parse_arguments");
        *(void **) &((p)->transform_image) = dlsym((p)->handle, "transform_image");
        *(newPlug + i) = p;

        i = i + sizeof(struct Plugin *);
      }
                                                                                                                                                            
      free(filePath);
    }
  }

  *num_plugin = i;
  closedir(direct);
  return newPlug;
}

int get_image(char * plugin, char* argv[], int argc, struct Image *source, 
              struct Plugin **plugList, int plugin_size) {
                
 for (int i = 0; i < plugin_size; i += sizeof(struct Plugin*)) {
    if(strcmp((*(plugList+i))->get_plugin_name(), plugin) == 0) {

      void *p = (*(plugList+i))->parse_arguments(argc - 5, argv + 5); //parse argments to plugin
      if(!p) {
        printf("ERROR: Failed to parse arguments\n");
        img_destroy(source);
        delete(plugList, plugin_size);
        return 1;
      }

      struct Image *im = (*(plugList + i))->transform_image(source, p); //get output image
      if(!im) {
        img_destroy(source);
        delete(plugList, plugin_size);
        return 1;
      }
      
      img_write_png(im, argv[4]); //write ouput image to file
      img_destroy(im); //destroy output image
      img_destroy(source); //destroy input image
      delete(plugList, plugin_size);

      return 0;
    }
  }

  return 0;
}

void usage_info() {
    printf("Usage: imgproc <command> [<command args...>]\n");
    printf("Commands are: \n");
    printf("  list\n");
    printf("  exec <plugin> <input img> <output img> [<plugin args...>]\n");
}

int main(int argc, char* argv[]) {
  char* command = argv[1];

  if (argc == 1) {
    usage_info();
    return 0;
  }

  // array of plugins 
  int plugin_size = 0;                                                                                                                                                                                    
  struct Plugin **plugList = init_plugin(&plugin_size);
  if(plugList == NULL) {
    return 1;
  }

  if(argc < 5) {
    if(argc == 2 && strcmp(command, "list") == 0) {
      //iterate through plugList                                                                                                                                                                           
      for (int i = 0; i < plugin_size; i += sizeof(struct Plugin*)) {
        printf("%s: %s\n", (*(plugList + i))->get_plugin_name(),  (*(plugList + i))->get_plugin_desc());
      }

      delete(plugList, plugin_size);
      return 0;

    } else {
      printf("ERROR: Invalid Number of Arguments\n");
      usage_info();
      delete(plugList, plugin_size);
      return 1;
    }
  }
  
  struct Image* inputImg = img_read_png(argv[3]);
  if(!inputImg) {
    printf("ERROR: Could not open input Image\n");
    return 1;
  }                         

  if(argc >= 5) {
    return get_image(argv[2], argv, argc, inputImg, plugList, plugin_size); 
  }

  return 0;
}



