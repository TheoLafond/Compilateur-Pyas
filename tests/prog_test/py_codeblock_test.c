#include <stdlib.h>
#include <stdio.h>

#include <pyas/py_codeblock.h>


int main(void){

  py_cb_t codeblock = py_cb_create();
  py_cb_add_local_count(123456, codeblock);

  pyobj_t pyobj_list   = pyobj_create(PYOBJ_LIST);
  pyobj_t pyobj_string = pyobj_create(PYOBJ_STRING);
  pyobj_t pyobj_number = pyobj_create(PYOBJ_NUMBER_S_COMPLEX);

  py_cb_add_cellvars(pyobj_list, codeblock);
  py_cb_add_parent(pyobj_number, codeblock);
  py_cb_add_names(pyobj_string, codeblock);


  py_cb_print(codeblock);

  py_cb_delete(codeblock);

  return 0;
}
