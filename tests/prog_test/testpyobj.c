#include <pyas/all.h>
#include <unitest/unitest.h>
#include <pyas/py_codeblock.h>
#include <pyas/pyobj.h>
#include <pyas/lexem.h>
#include <pyas/list.h>

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int main ( int argc, char *argv[] ) {
  pyobj_t pyobj;
  list_t listlex = list_new();
  int i;
  for(i=1;i<argc;i+=2){
    listlex = list_add_first(listlex,lexem_new( argv[i], argv[i+1],0, 5, 6, -1));
  }

  pyobj = pyobj_list_lexem_to_pyobj_list(listlex,i/2);
  pyobj_print(pyobj);
  pyobj_delete(pyobj);
  list_delete(listlex,lexem_delete);
  return 0;
}
