#include "b.h"

#define OBJ_DIR "obj"
#define PROG_NAME "nojs"
#define GCC "gcc"
#define FLAGS "-Wall -Wextra -g -O2"

int main(void) {
  INFO("Start building Nojs\n");
  
  if (!dir_exists(OBJ_DIR))
    make_dir(OBJ_DIR, 0755);
  
  Array *c_files = find_all_files("./src", "c");
  
  char *current_dir = pwd();
  char *obj_dir = pathjoin(current_dir, OBJ_DIR);
  Array *o_files = array_new(c_files->count);
  
  for (size_t i = 0; i < c_files->count; i++) {
    char* name = path_basename((char *)c_files->items[i]);
    char* obj_file_path = pathjoin(obj_dir, name);
    obj_file_path = change_extension(obj_file_path, "o");
    
    RUN(GCC, "-c", (char *)c_files->items[i], "-o", obj_file_path, "-I./src", FLAGS);
    array_add(o_files, obj_file_path);
  }
  
  char *objs = o_files->items[0];
  for(size_t i = 1; i < o_files->count; i++){
    objs = strcat_new(objs, " ");
    objs = strcat_new(objs, o_files->items[i]); 
  }
  
  RUN(GCC, objs, "-o", PROG_NAME);
  
  INFO("Building completed successfully\n");
  
  array_free(c_files);
  array_free(o_files);
  free(current_dir);
  free(obj_dir);
  
  return 0;
}
