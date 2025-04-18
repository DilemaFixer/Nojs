#include "b.h"
#include "lib/log/logger.h"
#include "lib/try/try.h"

#define OBJ_DIR "obj"
#define PROG_NAME "nojs"
#define GCC "gcc"
#define FLAGS "-Wall -Wextra -g -O2"
#define INCLUDES "-I./include -I./lib/log -I./lib/try -I./lib/arr"

int main(void) {
  INFO("Start building Nojs\n");
  
  if (!dir_exists(OBJ_DIR))
    make_dir(OBJ_DIR, 0755);
  
  Array *c_files = find_all_files("./src", "c");
  
  Array *log_files = find_all_files("./lib/log", "c");
  Array *try_files = find_all_files("./lib/try", "c");
  
  for (size_t i = 0; i < log_files->count; i++) {
    array_add(c_files, log_files->items[i]);
  }
  
  for (size_t i = 0; i < try_files->count; i++) {
    array_add(c_files, try_files->items[i]);
  }
  
  char *current_dir = pwd();
  char *obj_dir = pathjoin(current_dir, OBJ_DIR);
  Array *o_files = array_new(c_files->count);
  
  for (size_t i = 0; i < c_files->count; i++) {
    char* name = path_basename((char *)c_files->items[i]);
    char* obj_file_path = pathjoin(obj_dir, name);
    obj_file_path = change_extension(obj_file_path, "o");
    
    RUN(GCC, "-c", (char *)c_files->items[i], "-o", obj_file_path, FLAGS, INCLUDES);
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
  array_free(log_files);
  array_free(try_files);
  array_free(o_files);
  free(current_dir);
  free(obj_dir);
  
  return 0;
}
