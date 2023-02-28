#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <pyas/pyobj.h>


int main(void){

  char* filename = "binary.txt";
  FILE* binary_file = fopen(filename, "w+");
  if (!binary_file) {
    perror("fopen");
    exit(EXIT_FAILURE);
  }

  pyobj_t pyobj_string = pyobj_create(PYOBJ_STRING);
  pyobj_t pyobj_null = pyobj_create(PYOBJ_NULL);
  pyobj_t pyobj_false = pyobj_create(PYOBJ_FALSE);
  pyobj_t pyobj_true = pyobj_create(PYOBJ_TRUE);
  pyobj_t pyobj_none = pyobj_create(PYOBJ_NONE);
  pyobj_t pyobj_int = pyobj_create(PYOBJ_NUMBER_INT);
  pyobj_t pyobj_real = pyobj_create(PYOBJ_NUMBER_REAL);
  pyobj_t pyobj_list = pyobj_create(PYOBJ_TUPPLE);

  pyobj_to_binary(binary_file, pyobj_string, NULL);
  pyobj_to_binary(binary_file, pyobj_null, NULL);
  pyobj_to_binary(binary_file, pyobj_false, NULL);
  pyobj_to_binary(binary_file, pyobj_true, NULL);
  pyobj_to_binary(binary_file, pyobj_none, NULL);
  pyobj_to_binary(binary_file, pyobj_int, NULL);
  pyobj_to_binary(binary_file, pyobj_real, NULL);
  pyobj_to_binary(binary_file, pyobj_list, NULL);

  pyobj_delete(pyobj_string);
  pyobj_delete(pyobj_null);
  pyobj_delete(pyobj_false);
  pyobj_delete(pyobj_true);
  pyobj_delete(pyobj_none);
  pyobj_delete(pyobj_int);
  pyobj_delete(pyobj_real);
  pyobj_delete(pyobj_list);

  fclose(binary_file);
  return 0;
}
