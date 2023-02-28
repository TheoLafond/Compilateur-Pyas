/**
 * @file pyobj.c
 * @author Benjamin Marty <benjamin.marty@grenoble-inp.org>
 * @brief pyobj module.
 *
 * Pyobj module
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <pyas/pyobj.h>

struct pyobj{
  pyobj_type type;
  unsigned int refcount;

  union{
    struct{
      pyobj_t *value;
      int32_t size;
    }list;

    struct{
      char *buffer;
      int length;
    }string;

    py_cb_t codeblock;

    union{
      int32_t integer;
      double   real;
      char*    s_real;
      struct{
        double   re;
        double   im;
      }complex;
      struct{
        char  *s_re;
        char  *s_im;
      }s_complex;
    }number;
  }py;
};

pyobj_t pyobj_list_lexem_to_pyobj_list(list_t list_lexem, int size){
  pyobj_t pyobj = calloc(1,sizeof(*pyobj));
  pyobj_t *tab = calloc(size,sizeof(pyobj_t));
  pyobj_t *i;
  pyobj->type = PYOBJ_TUPPLE;
  pyobj->py.list.value = tab;
  pyobj->py.list.size = size;
  for(i=tab;i<tab+size;i++){
    *i = pyobj_lexem_to_pyobj_val(list_first(list_lexem));
    list_lexem = list_next(list_lexem);
  }
  return pyobj;
}

pyobj_t pyobj_list_to_pyobj(list_t list_pyobj, int size){
  pyobj_t pyobj = calloc(1,sizeof(*pyobj));
  pyobj->py.list.size = size;
  pyobj->type = PYOBJ_TUPPLE;
  if(size!=0){
    pyobj_t *tab = calloc(size,sizeof(pyobj_t));
    pyobj_t *i;
    pyobj->py.list.value = tab;
    for(i=tab;i<tab+size;i++){
      *i = list_first(list_pyobj);
      list_pyobj = list_next(list_pyobj);
    }
  }
  else{
    pyobj->py.list.value = NULL;
  }
  return pyobj;
}

pyobj_t pyobj_string(char *string, int size){
  pyobj_t pyobj = calloc(1,sizeof(*pyobj));
  pyobj->type = PYOBJ_STRING;
  pyobj->py.string.length = size;
  char * pyobj_string=malloc(size);
  int i;
  for (i = 0; i < size; i++){
    pyobj_string[i]=string[i];
  }
  pyobj->py.string.buffer = pyobj_string;
  return pyobj;
}

pyobj_t pyobj_codeblock(py_cb_t codeblock){
  pyobj_t pyobj = calloc(1,sizeof(*pyobj));
  pyobj->type = PYOBJ_CODEBLOCK;
  pyobj->py.codeblock = codeblock;
  return pyobj;
}

pyobj_t pyobj_complex_to_pyobj_val(double real, double im){
    pyobj_t pyobj = calloc(1,sizeof(*pyobj));
    pyobj->type = PYOBJ_NUMBER_COMPLEX;
    pyobj->py.number.complex.re =real;
    pyobj->py.number.complex.im =im;
    return pyobj;
}

pyobj_t pyobj_lexem_to_pyobj_val(lexem_t lexem){
  pyobj_t pyobj = calloc(1,sizeof(*pyobj));
  char *endptr;
  if(lexem_type_strict(lexem,"float")){
    pyobj->type = PYOBJ_NUMBER_REAL;
    pyobj->py.number.real = strtod(lexem_return_value(lexem),&endptr);
    if (*endptr != '\0'){
      printf("Number too long for float in lexem :");
      lexem_print(lexem);
      printf("\n");
      exit(EXIT_FAILURE);
    }
  }
  else if(lexem_type_strict(lexem,"integer::hex")){
    pyobj->type = PYOBJ_NUMBER_INT;
    pyobj->py.number.integer = strtol(lexem_return_value(lexem),&endptr,0);
    if (*endptr != '\0'){
      printf("Number too long for int in lexem :");
      lexem_print(lexem);
      printf("\n");
      exit(EXIT_FAILURE);
    }
  }
  else if(lexem_type_strict(lexem,"integer::dec")){
    pyobj->type = PYOBJ_NUMBER_INT;
    pyobj->py.number.integer = strtol(lexem_return_value(lexem),&endptr,0);
    if (*endptr != '\0'){
      printf("Number too long for int in lexem :");
      lexem_print(lexem);
      printf("\n");
      exit(EXIT_FAILURE);
    }
  }
  else if(lexem_type_strict(lexem,"string")) {
    pyobj->type = PYOBJ_STRING;
    pyobj->py.string.length = strlen(lexem_return_value(lexem)) - 2;
    pyobj->py.string.buffer = strndup(&lexem_return_value(lexem)[1], pyobj->py.string.length);
  }
  else if(lexem_type_strict(lexem, "Pycst::None")) {
    pyobj->type = PYOBJ_NONE;
  }
  else if(lexem_type_strict(lexem, "Pycst::True")) {
    pyobj->type = PYOBJ_TRUE;
  }
  else if(lexem_type_strict(lexem, "Pycst::False")) {
    pyobj->type = PYOBJ_FALSE;
  }
  else if(lexem_type_strict(lexem, "Pycst::Null")) {
    pyobj->type = PYOBJ_NULL;
  }
  return pyobj;
}


//Meaningless function for testing purpooses (as matlab)
pyobj_t pyobj_create(int type){
  pyobj_t pyobj = calloc(1, sizeof(*pyobj));
  pyobj->type = type;
  int i=0;

  if (type == PYOBJ_NULL || type == PYOBJ_FALSE || type == PYOBJ_NONE || type == PYOBJ_TRUE){
    return pyobj;
  }
  switch(type){
    case PYOBJ_TUPPLE :
      pyobj->py.list.size = 3;
      pyobj->py.list.value = calloc(pyobj->py.list.size, sizeof(*pyobj));
      while(i<pyobj->py.list.size){
        pyobj->py.list.value[i] = pyobj_create(PYOBJ_STRING);
        i++;
      }
      return pyobj;
    case PYOBJ_STRING :
      pyobj->py.string.length = 7;
      pyobj->py.string.buffer = strdup("Bonjour");
      return pyobj;
    case PYOBJ_NUMBER_INT:
      pyobj->py.number.integer = 5;
      return pyobj;
    case PYOBJ_NUMBER_REAL:
      pyobj->py.number.real = 1.618;
      return pyobj;
    case PYOBJ_NUMBER_S_REAL:
      pyobj->py.number.s_real = strdup("1.618");
      return pyobj;
    case PYOBJ_NUMBER_COMPLEX:
      pyobj->py.number.complex.re = 2;
      pyobj->py.number.complex.im = 3;
      return pyobj;
    case PYOBJ_NUMBER_S_COMPLEX:
      pyobj->py.number.s_complex.s_re = strdup("4");
      pyobj->py.number.s_complex.s_im = strdup("5");
      return pyobj;
    default:
      printf("Error input");
      exit(EXIT_SUCCESS);
  }
}




void pyobj_delete(pyobj_t pyobj){
  int i=0;
  switch (pyobj->type){
    case PYOBJ_LIST:
      while(i<pyobj->py.list.size){
        pyobj_delete(pyobj->py.list.value[i]);
        i++;
      }
      free(pyobj->py.list.value);
      break;
    case PYOBJ_TUPPLE:
      while(i<pyobj->py.list.size){
        pyobj_delete(pyobj->py.list.value[i]);
        i++;
      }
      free(pyobj->py.list.value);
      break;

    case PYOBJ_STRING:
      free(pyobj->py.string.buffer);
      break;
    case PYOBJ_NUMBER_S_REAL : free(pyobj->py.number.s_real); break;
    case PYOBJ_NUMBER_S_COMPLEX : free(pyobj->py.number.s_complex.s_re); free(pyobj->py.number.s_complex.s_im); break;
    case PYOBJ_CODEBLOCK : py_cb_delete(pyobj->py.codeblock); break;
  }
  free(pyobj);
}


int pyobj_print(pyobj_t pyobj){
  int a,i=0;
  double re,im;
  switch (pyobj->type){
    case PYOBJ_NULL             : printf("Null\n");   break;
    case PYOBJ_FALSE            : printf("False\n");  break;
    case PYOBJ_NONE             : printf("None\n");   break;
    case PYOBJ_TRUE             : printf("True\n");   break;
    case PYOBJ_NUMBER_REAL      : printf("\t%f\n", pyobj->py.number.real); break;
    case PYOBJ_STRING           :
      printf("\t");
      for (i=0; i< pyobj->py.string.length; i++){
        printf("%c", pyobj->py.string.buffer[i]);
      }
      printf(", size = %d\n",pyobj->py.string.length);
      break;
    case PYOBJ_NUMBER_INT       : printf("\t%d\n", pyobj->py.number.integer); break;
    case PYOBJ_NUMBER_S_REAL    : printf("\t%s\n", pyobj->py.number.s_real); break;
    case PYOBJ_NUMBER_S_COMPLEX : printf("\t%s+i%s\n", pyobj->py.number.s_complex.s_re, pyobj->py.number.s_complex.s_im); break;
    case PYOBJ_NUMBER_COMPLEX   :
      a=pyobj->py.number.complex.im<0;
      re=pyobj->py.number.complex.re;
      im=pyobj->py.number.complex.im;
      printf("\t%e %c %ej\n", re, a ? '-' : '+', a ? -im : im);
      break;
    case PYOBJ_CODEBLOCK        : py_cb_print(pyobj->py.codeblock); break;
    case PYOBJ_LIST:
      for (i=0; i<pyobj->py.list.size; i++){
        pyobj_print(pyobj->py.list.value[i]);
      }
      break;
    case PYOBJ_TUPPLE:
      for (i=0; i<pyobj->py.list.size; i++){
        pyobj_print(pyobj->py.list.value[i]);
      }
      break;
    default :
      printf("Error !");
      exit(EXIT_SUCCESS);
  }
  return 1;
}

void write_char_in_file(FILE* binary_file, char c, int size){
  fwrite(&c, 1, size, binary_file);
  fseek(binary_file, 0, SEEK_END);
}

void pyobj_to_binary(FILE* binary_file, pyobj_t pyobj, list_t interned){
  int i;
  int string_type = pyobj->type;
  int j=0;
  char ok = 0;

  switch (pyobj->type){
    case PYOBJ_TUPPLE :
      //Type
      fwrite(&pyobj->type, 1, sizeof(char), binary_file);
      fseek(binary_file, 0, SEEK_END);
      //Size
      fwrite(&pyobj->py.list.size, 1, sizeof(int), binary_file);
      fseek(binary_file, 0, SEEK_END);
      //Value
      for (i=0; i<pyobj->py.list.size; i++){
        pyobj_to_binary(binary_file, pyobj->py.list.value[i], interned);
      }
      break;
    case PYOBJ_STRING:
      while(!list_empty(interned)){
        if (!strcmp(pyobj->py.string.buffer, (((already_interned_t)list_first(interned))->string)->py.string.buffer)){
          if(((already_interned_t)list_first(interned))->interned_or_not){
            //on a joute en tant que string ref à j (R)
            string_type = 'R';
            //Type
            fwrite(&string_type, 1, sizeof(char), binary_file);
            fseek(binary_file, 0, SEEK_END);
            //Ref
            fwrite(&j, 1, sizeof(int), binary_file);
            fseek(binary_file, 0, SEEK_END);
            ok = 1;
            break;
          }
          else {
            //on ajoute en tant que interned (t)
            string_type = 't';
            //Type
            fwrite(&string_type, 1, sizeof(char), binary_file);
            fseek(binary_file, 0, SEEK_END);
            //Size
            fwrite(&pyobj->py.string.length, 1, sizeof(int), binary_file);
            fseek(binary_file, 0, SEEK_END);
            //Value
            fwrite(pyobj->py.string.buffer, 1, pyobj->py.string.length, binary_file);
            fseek(binary_file, 0, SEEK_END);

            ((already_interned_t)list_first(interned))->interned_or_not = 1;
            ok = 1;
            break;
          }
        }
        interned = list_next(interned);
        j++;
      }
      if (!ok){
        //on ajoute en tant que string (s)
        //Type
        fwrite(&string_type, 1, sizeof(char), binary_file);
        fseek(binary_file, 0, SEEK_END);
        //Size
        fwrite(&pyobj->py.string.length, 1, sizeof(int), binary_file);
        fseek(binary_file, 0, SEEK_END);
        //Value
        fwrite(pyobj->py.string.buffer, 1, pyobj->py.string.length, binary_file);
        fseek(binary_file, 0, SEEK_END);
      }
      break;
    case PYOBJ_NONE :
      write_char_in_file(binary_file, pyobj->type, 1);
      break;
    case PYOBJ_NULL :
      write_char_in_file(binary_file, pyobj->type, 1);
      break;
    case PYOBJ_TRUE :
      write_char_in_file(binary_file, pyobj->type, 1);
      break;
    case PYOBJ_FALSE :
      write_char_in_file(binary_file, pyobj->type, 1);
      break;
    case PYOBJ_NUMBER_INT :
      //Type
      fwrite(&pyobj->type, 1, sizeof(char), binary_file);
      fseek(binary_file, 0, SEEK_END);
      //Value
      fwrite(&pyobj->py.number.integer, 1, sizeof(int), binary_file);
      fseek(binary_file, 0, SEEK_END);
      break;
    case PYOBJ_NUMBER_REAL :
      //Type
      fwrite(&pyobj->type, 1, sizeof(char), binary_file);
      fseek(binary_file, 0, SEEK_END);
      //Value
      fwrite(&pyobj->py.number.real, 1, sizeof(double), binary_file);
      fseek(binary_file, 0, SEEK_END);
      break;
    case PYOBJ_NUMBER_COMPLEX :
      //Type
      fwrite(&pyobj->type, 1, sizeof(char), binary_file);
      fseek(binary_file, 0, SEEK_END);
      //Value
      fwrite(&pyobj->py.number.complex.re, 1, sizeof(double), binary_file);
      fseek(binary_file, 0, SEEK_END);
      fwrite(&pyobj->py.number.complex.im, 1, sizeof(double), binary_file);
      fseek(binary_file, 0, SEEK_END);
      break;
    case PYOBJ_CODEBLOCK :
      py_cb_to_binary_function(binary_file, pyobj->py.codeblock,interned);
      break;
    default :
      exit(EXIT_SUCCESS);
  }
}

already_interned_t already_interned_new(pyobj_t string,char interned_or_not){
  already_interned_t already_interned= calloc(1,sizeof(*already_interned));
  already_interned->string = string;
  already_interned->interned_or_not = interned_or_not;
  return already_interned;
}

int already_interned_delete(void*already_interned){
  free(already_interned);
  return 1;
}

list_t pyobj_interned_to_list_already_interned(pyobj_t interned){
  queue_t list_already_interned = queue_new();
  int i;
  for (i=0;i<interned->py.list.size;i++){
    list_already_interned = enqueue(list_already_interned,already_interned_new(interned->py.list.value[i],0));
  }
  return queue_to_list(&list_already_interned);
}
