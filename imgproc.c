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

void delete(struct Plugin * plugList) {
  int i = 0;
  while ((plugList+i)->handle != NULL) {
    dlclose((plugList+i)->handle);
    i++;
  }
}

// if plugin param is "list", printf all the needed values                                                                                                                                                  
struct Plugin * init_plugin(int * num_plugin) {
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
  // i is counter for structs                                                                                                                                                                               
  int i = 0;

  // on piazza they said no more than 15                                                                                                                                                                    
  struct Plugin *newPlug = malloc(15 * sizeof(struct Plugin));
  if(!newPlug) {
    printf("Error: Could not allocate Plugin\n");
    closedir(direct);
    return NULL;
  }

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
        delete(newPlug);
        closedir(direct);
        return NULL;
      }

      strcat(filePath, plugDirect);
      strcat(filePath, "/");
      strcat(filePath, read->d_name);
      //printf("%s\n", filePath);                                                                                                                                                                          

      void * handle = dlopen(filePath, RTLD_LAZY);

      if (dlsym(handle, "get_plugin_name") != NULL) {
        (newPlug + i)->handle = handle;

        *(void **) &((newPlug + i)->get_plugin_name) = dlsym((newPlug + i)->handle, "get_plugin_name");
        *(void **) &((newPlug + i)->get_plugin_desc) = dlsym((newPlug + i)->handle, "get_plugin_desc");
        *(void **) &((newPlug + i)->parse_arguments) = dlsym((newPlug + i)->handle, "parse_arguments");
        *(void **) &((newPlug + i)->transform_image) = dlsym((newPlug + i)->handle, "transform_image");

        i++;
      
      }

      //dlclose(handle); (this is too early)                                                                                                                                                               
      free(filePath);
    }
  }

  *num_plugin = i;
  closedir(direct);
  return newPlug;
}

int get_image(char * plugin, char* argv[], int argc, struct Image *source, 
              struct Plugin *plugList, int num) {
                
 for (int i = 0; i < num; i++) {
    if(strcmp((plugList+i)->get_plugin_name(), plugin) == 0) {

      void *p = (plugList+i)->parse_arguments(argc - 5, argv + 5);
      if(!p) {
        printf("ERROR: Failed to parse arguments\n");
        img_destroy(source);
        delete(plugList);
        return 1;
      }

      struct Image *im = (plugList + i)->transform_image(source, p); //get output image
      if(!im) {
        img_destroy(source);
        delete(plugList);
        return 1;
      }
      
      img_write_png(im, argv[4]); //write ouput image to file
      img_destroy(im); //destroy output image
      img_destroy(source);
      delete(plugList);

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
  if(argc > 6) {                                                                                                                                                           
    printf("ERROR: Too many arguments\n");  
    return 1;                                                                                                                                                            
  }

  char* command = argv[1];

  if (argc == 1) {
    usage_info();
    return 1;
  }

  // array of plugins 
  int n = 0;                                                                                                                                                                                    
  struct Plugin * plugList = init_plugin(&n);
  if(plugList == NULL) {
    return 1;
  }

  if(argc < 5) {
    if(argc == 2 && strcmp(command, "list") == 0) {
      //iterate thru plugList                                                                                                                                                                             

      for (int i = 0; i < n; i++) {
        printf("%s: %s\n", (plugList + i)->get_plugin_name(),  (plugList + i)->get_plugin_desc());
      }

      delete(plugList);
      return 0;

    } else {
      printf("ERROR: Expected 1 argument, \"list\" as an input\n");
      usage_info();
      delete(plugList);
      return 1;
    }
  }
  

  char* plugin = argv[2];
  struct Image* inputImg = img_read_png(argv[3]);
  if(!inputImg) {
    printf("Error: Could not open inputImage\n");
    return 1;
  }
  // argv[5] might be plugin args                            

  if(argc >= 5) {
    return get_image(plugin, argv, argc, inputImg, plugList, n); 
    // if(strcmp("exec", command) != 0) {
    //   printf("ERROR: Expected \"exec\" as an input\n");
    //   usage_info();
    //   img_destroy(inputImg);
    //   delete(plugList);
    //   return 1;
    // }
  }

  // if(argc == 5) {
  //   if(strcmp("swapbg", plugin) == 0) {
  //     return get_image(plugin, argv, argc, inputImg, plugList, n);                                                                                                                                                                               
  //   } else if(strcmp("mirrorh", plugin) == 0) {
  //     return get_image(plugin, argv, argc, inputImg, plugList, n);                                                                                                                                                                                   
  //   } else if(strcmp("mirrorv", plugin) == 0) {
  //     return get_image(plugin, argv, argc, inputImg, plugList, n);                                                                                                                                                                                    
  //   }  else {
  //     printf("ERROR: Invalid Arguments\n");
  //     usage_info();
  //     img_destroy(inputImg);
  //     delete(plugList);
  //     return 1;
  //   }
  // }

  // if(argc == 6) {
  //   if(strcmp("tile", plugin) == 0) {
  //     return get_image(plugin, argv, argc, inputImg, plugList, n);                                                                                                                                                             
  //   } else if(strcmp("expose", argv[2]) == 0) {
  //     return get_image(plugin, argv, argc, inputImg, plugList, n);                                                                                                                                                                
  //   } else {
  //     printf("ERROR: Invalid Arguments\n");
  //     usage_info();
  //     img_destroy(inputImg);
  //     delete(plugList);
  //     return 1;
  //   }
  // }

  //img_destroy(inputImg);
  //delete(plugList);
  return 0;
}






