#include <stdlib.h>
#include <stdio.h>

#include <pyas/pyobj.h>


int main(void){

  pyobj_t pyobj_list   = pyobj_create(PYOBJ_LIST);
  pyobj_t pyobj_string = pyobj_create(PYOBJ_STRING);
  pyobj_t pyobj_number = pyobj_create(PYOBJ_NUMBER_S_COMPLEX);

  printf("pyobj_list :\n");
  pyobj_print(pyobj_list);

  printf("***********\n");

  printf("pyobj_string :\n");
  pyobj_print(pyobj_string);

  printf("***********\n");

  printf("pyobj_number :\n");
  pyobj_print(pyobj_number);


  pyobj_delete(pyobj_list);
  pyobj_delete(pyobj_string);
  pyobj_delete(pyobj_number);


  return 0;
}
