/**
 * @file py_codeblock.c
 * @author Benjamin Marty <benjamin.marty@grenoble-inp.org>
 * @brief py_codeblock module.
 *
 * Py_codeblock module
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <assert.h>
#include <stdint.h>
#include <pyas/py_codeblock.h>
#include <pyas/list.h>


struct py_codeblock{
  int8_t *is_here;
  int version_pyvm;
  struct{
    uint32_t  arg_count;
    uint32_t  local_count;
    uint32_t  stack_size;
    uint32_t  flags;
  }header;
  pyobj_t parent;
  struct{
    struct{
      uint32_t  magic;
      time_t    timestamp;
      uint32_t  source_size;
    }header;
    struct{
      pyobj_t interned;
      pyobj_t bytecode;
      pyobj_t consts;
      pyobj_t names;
      pyobj_t varnames;
      pyobj_t freevars;
      pyobj_t cellvars;
    }content;
    struct{
      pyobj_t   filename;
      pyobj_t   name;
      uint32_t  firstlinelo;
      pyobj_t   lnotab;
    }trailer;
  }binary;
};

py_cb_t py_cb_create(void){
  py_cb_t codeblock = calloc(1, sizeof(*codeblock));
  assert(codeblock);

  codeblock->is_here = calloc(NUMBER_OF_ELEMENTS, sizeof(int8_t));

  return codeblock;
}

int py_cb_print(py_cb_t codeblock){
  if (codeblock->is_here[VERSION_PYVM]) printf("Version pyvm : \t\t\t%d\n", codeblock->version_pyvm);
  if (codeblock->is_here[ARG_COUNT])    printf("Arg count : \t\t\t%d\n", codeblock->header.arg_count);
  if (codeblock->is_here[LOCAL_COUNT])  printf("Local count : \t\t\t%d\n", codeblock->header.local_count);
  if (codeblock->is_here[STACK_SIZE])   printf("Stack size : \t\t\t%d\n", codeblock->header.stack_size);
  if (codeblock->is_here[FLAGS])        printf("Flags : \t\t\t0x%x\n", codeblock->header.flags);
  if (codeblock->is_here[PARENT]){
    printf("<Parent>\n");
    pyobj_print(codeblock->parent);
    printf("</Parent>\n");
  }
  if (codeblock->is_here[MAGIC])        printf("Magic : \t\t\t%d\n", codeblock->binary.header.magic);
  if (codeblock->is_here[TIMESTAMP])    printf("Timestamp : \t\t\t%ld\n", codeblock->binary.header.timestamp);
  if (codeblock->is_here[SOURCE_SIZE])  printf("Source size : \t\t\t%d\n", codeblock->binary.header.source_size);
  if (codeblock->is_here[INTERNED]){
    printf("<Interned>\n");
    pyobj_print(codeblock->binary.content.interned);
    printf("</Interned>\n");
  }
  if (codeblock->is_here[BYTECODE]){
    printf("<Bytecode>\n");
    pyobj_print(codeblock->binary.content.bytecode);
    printf("</Bytecode>\n");
  }
  if (codeblock->is_here[CONSTS]){
    printf("<Consts>\n");
    pyobj_print(codeblock->binary.content.consts);
    printf("</Consts>\n");
  }
  if (codeblock->is_here[NAMES]){
    printf("<Names>\n");
    pyobj_print(codeblock->binary.content.names);
    printf("</Names>\n");
  }
  if (codeblock->is_here[VARNAMES]){
    printf("<Varnames>\n");
    pyobj_print(codeblock->binary.content.varnames);
    printf("</Varnames>\n");
  }
  if (codeblock->is_here[FREEVARS]){
    printf("<Freevars>\n");
    pyobj_print(codeblock->binary.content.freevars);
    printf("</Freevars>\n");
  }
  if (codeblock->is_here[CELLVARS]){
    printf("<Cellvars>\n");
    pyobj_print(codeblock->binary.content.cellvars);
    printf("</Cellvars>\n");
  }
  if (codeblock->is_here[FILENAME]){
    printf("<Filename>\n");
      pyobj_print(codeblock->binary.trailer.filename);
    printf("</Filename>\n");
  }
  if (codeblock->is_here[NAME]){
    printf("<Name>\n");
    pyobj_print(codeblock->binary.trailer.name);
    printf("</Name>\n");
  }
  if (codeblock->is_here[FIRSTLINELO])  printf("Firstlinelo : \t\t\t%d\n", codeblock->binary.trailer.firstlinelo);
  if (codeblock->is_here[LNOTAB]){
    printf("<Lnotab>\n");
    pyobj_print(codeblock->binary.trailer.lnotab);
    printf("</Lnotab>\n");
  }

  return 1;
}

void py_cb_reset(py_cb_t codeblock){
  if (codeblock->is_here[PARENT])   pyobj_delete(codeblock->parent);
  if (codeblock->is_here[INTERNED]) pyobj_delete(codeblock->binary.content.interned);
  if (codeblock->is_here[BYTECODE]) pyobj_delete(codeblock->binary.content.bytecode);
  if (codeblock->is_here[CONSTS])   pyobj_delete(codeblock->binary.content.consts);
  if (codeblock->is_here[NAMES])    pyobj_delete(codeblock->binary.content.names);
  if (codeblock->is_here[VARNAMES]) pyobj_delete(codeblock->binary.content.varnames);
  if (codeblock->is_here[FREEVARS]) pyobj_delete(codeblock->binary.content.freevars);
  if (codeblock->is_here[CELLVARS]) pyobj_delete(codeblock->binary.content.cellvars);
  if (codeblock->is_here[FILENAME]) pyobj_delete(codeblock->binary.trailer.filename);
  if (codeblock->is_here[NAME])     pyobj_delete(codeblock->binary.trailer.name);
  if (codeblock->is_here[LNOTAB])   pyobj_delete(codeblock->binary.trailer.lnotab);

  int i;
  for (i=0; i<NUMBER_OF_ELEMENTS; i++){
    codeblock->is_here[i] = 0;
  }
}


void py_cb_delete(py_cb_t codeblock){
  if (codeblock->is_here[PARENT])   pyobj_delete(codeblock->parent);
  if (codeblock->is_here[INTERNED]) pyobj_delete(codeblock->binary.content.interned);
  if (codeblock->is_here[BYTECODE]) pyobj_delete(codeblock->binary.content.bytecode);
  if (codeblock->is_here[CONSTS])   pyobj_delete(codeblock->binary.content.consts);
  if (codeblock->is_here[NAMES])    pyobj_delete(codeblock->binary.content.names);
  if (codeblock->is_here[VARNAMES]) pyobj_delete(codeblock->binary.content.varnames);
  if (codeblock->is_here[FREEVARS]) pyobj_delete(codeblock->binary.content.freevars);
  if (codeblock->is_here[CELLVARS]) pyobj_delete(codeblock->binary.content.cellvars);
  if (codeblock->is_here[FILENAME]) pyobj_delete(codeblock->binary.trailer.filename);
  if (codeblock->is_here[NAME])     pyobj_delete(codeblock->binary.trailer.name);
  if (codeblock->is_here[LNOTAB])   pyobj_delete(codeblock->binary.trailer.lnotab);

  free(codeblock->is_here);
  free(codeblock);
}



//DO NOT USE, ONLY USED IN DOC
void py_cb_add_                  (int integer, uint32_t uinteger, pyobj_t pyobj, time_t time, py_cb_t codeblock){
  uinteger++;
  uinteger = integer + time;
  pyobj_print(pyobj);
  py_cb_print(codeblock);
}
void py_cb_add_version_pyvm      (int integer,      py_cb_t codeblock){
   codeblock->is_here[VERSION_PYVM] = 1;
   codeblock->version_pyvm = integer;
}
void py_cb_add_arg_count         (uint32_t integer, py_cb_t codeblock){
  codeblock->is_here[ARG_COUNT] = 1;
  codeblock->header.arg_count = integer;
}
void py_cb_add_local_count       (uint32_t integer, py_cb_t codeblock){
  codeblock->is_here[LOCAL_COUNT] = 1;
  codeblock->header.local_count = integer;
}
void py_cb_add_stack_size        (uint32_t integer, py_cb_t codeblock){
  codeblock->is_here[STACK_SIZE] = 1;
  codeblock->header.stack_size = integer;
}
void py_cb_add_flags             (uint32_t integer, py_cb_t codeblock){
  codeblock->is_here[FLAGS] = 1;
  codeblock->header.flags = integer;
}
void py_cb_add_parent            (pyobj_t pyobj,    py_cb_t codeblock){
  codeblock->is_here[PARENT] = 1;
  codeblock->parent = pyobj;
}
void py_cb_add_magic             (uint32_t integer, py_cb_t codeblock){
  codeblock->is_here[MAGIC] = 1;
  codeblock->binary.header.magic = integer;
}
void py_cb_add_timestamp         (time_t time,      py_cb_t codeblock){
  codeblock->is_here[TIMESTAMP] = 1;
  codeblock->binary.header.timestamp = time;
}
void py_cb_add_source_size       (uint32_t integer, py_cb_t codeblock){
  codeblock->is_here[SOURCE_SIZE] = 1;
  codeblock->binary.header.source_size = integer;
}
void py_cb_add_interned          (pyobj_t pyobj,    py_cb_t codeblock){
  codeblock->is_here[INTERNED] = 1;
  codeblock->binary.content.interned = pyobj;
}
void py_cb_add_bytecode          (pyobj_t pyobj,    py_cb_t codeblock){
  codeblock->is_here[BYTECODE] = 1;
  codeblock->binary.content.bytecode = pyobj;
}
void py_cb_add_consts            (pyobj_t pyobj,    py_cb_t codeblock){
  codeblock->is_here[CONSTS] = 1;
  codeblock->binary.content.consts = pyobj;
}
void py_cb_add_names             (pyobj_t pyobj,    py_cb_t codeblock){
  codeblock->is_here[NAMES] = 1;
  codeblock->binary.content.names = pyobj;
}
void py_cb_add_varnames          (pyobj_t pyobj,    py_cb_t codeblock){
  codeblock->is_here[VARNAMES] = 1;
  codeblock->binary.content.varnames = pyobj;
}
void py_cb_add_freevars          (pyobj_t pyobj,    py_cb_t codeblock){
  codeblock->is_here[FREEVARS] = 1;
  codeblock->binary.content.freevars = pyobj;
}
void py_cb_add_cellvars          (pyobj_t pyobj,    py_cb_t codeblock){
  codeblock->is_here[CELLVARS] = 1;
  codeblock->binary.content.cellvars = pyobj;
}
void py_cb_add_filename          (pyobj_t pyobj,    py_cb_t codeblock){
  codeblock->is_here[FILENAME] = 1;
  codeblock->binary.trailer.filename = pyobj;
}
void py_cb_add_name              (pyobj_t pyobj,    py_cb_t codeblock){
  codeblock->is_here[NAME] = 1;
  codeblock->binary.trailer.name = pyobj;
}
void py_cb_add_firstlinelo       (uint32_t integer, py_cb_t codeblock){
  codeblock->is_here[FIRSTLINELO] = 1;
  codeblock->binary.trailer.firstlinelo = integer;
}
void py_cb_add_lnotab            (pyobj_t pyobj,    py_cb_t codeblock){
  codeblock->is_here[LNOTAB] = 1;
  codeblock->binary.trailer.lnotab = pyobj;
}



int py_cb_str_to_id(char *source){
  if(!strcmp(source, "version_pyvm"))       return VERSION_PYVM;
  if(!strcmp(source, "flags"))              return FLAGS;
  if(!strcmp(source, "filename"))           return FILENAME;
  if(!strcmp(source, "name"))               return NAME;
  if(!strcmp(source, "stack_size"))         return STACK_SIZE;
  if(!strcmp(source, "arg_count"))          return ARG_COUNT;
  if(!strcmp(source, "posonly_arg_count"))  return ARG_COUNT;
  if(!strcmp(source, "kwonly_arg_count"))   return ARG_COUNT;
  if(!strcmp(source, "interned-strings"))   return INTERNED;
  if(!strcmp(source, "constants"))          return CONSTS;
  if(!strcmp(source, "names"))              return NAMES;
  if(!strcmp(source, "varnames"))           return VARNAMES;
  if(!strcmp(source, "freevars"))           return FREEVARS;
  if(!strcmp(source, "cellvars"))           return CELLVARS;
  if(!strcmp(source, "code"))               return BYTECODE;
  return -1;
}

void py_cb_call_set_function(int id, lexem_t lexem, py_cb_t codeblock){
  switch(id){
    case VERSION_PYVM : py_cb_add_version_pyvm(atoi(lexem_return_value(lexem)), codeblock);    break;
    case FLAGS        : py_cb_add_flags(strtol(lexem_return_value(lexem),NULL,0), codeblock);  break;
    case FILENAME     : py_cb_add_filename(pyobj_lexem_to_pyobj_val(lexem), codeblock);        break;
    case NAME         : py_cb_add_name(pyobj_lexem_to_pyobj_val(lexem), codeblock);            break;
    case STACK_SIZE   : py_cb_add_stack_size(atoi(lexem_return_value(lexem)), codeblock);      break;
    case ARG_COUNT    : py_cb_add_arg_count(atoi(lexem_return_value(lexem)), codeblock);       break;
  }
}

void py_cb_call_prologue_function(int id, list_t l_pyobj, py_cb_t codeblock, int size){
  switch(id){
    case INTERNED : py_cb_add_interned (pyobj_list_to_pyobj(l_pyobj, size), codeblock); break;
    case CONSTS   : py_cb_add_consts   (pyobj_list_to_pyobj(l_pyobj, size), codeblock); break;
    case NAMES    : py_cb_add_names    (pyobj_list_to_pyobj(l_pyobj, size), codeblock); break;
    case VARNAMES : py_cb_add_local_count(size, codeblock); py_cb_add_varnames (pyobj_list_to_pyobj(l_pyobj, size), codeblock); break;
    case FREEVARS : py_cb_add_freevars (pyobj_list_to_pyobj(l_pyobj, size), codeblock); break;
    case CELLVARS : py_cb_add_cellvars (pyobj_list_to_pyobj(l_pyobj, size), codeblock); break;
  }
}
void py_cb_call_code(int id, char* string, py_cb_t codeblock, int size){
  switch(id){
    case LNOTAB :       py_cb_add_lnotab (pyobj_string(string,size), codeblock);    break;
    case BYTECODE :     py_cb_add_bytecode (pyobj_string(string,size), codeblock);  break;
    case FIRSTLINELO :  py_cb_add_firstlinelo(atoi(string), codeblock);             break;
  }
}



//Function that write an integer on how many bytes (size) we want in the file
void write_int_in_file(FILE* binary_file, int i, int size){
  fwrite(&i, 1, size, binary_file);
  fseek(binary_file, 0, SEEK_END);
}

//Function that write zero on 4 bytes in the file
void write_nullsize_in_file(FILE* binary_file){
  int zero_inside = 0;
  fwrite(&zero_inside, 1, 4, binary_file);
  fseek(binary_file, 0, SEEK_END);
}

//Function that write a null tuple in the file
void write_nulltupl_in_file(FILE* binary_file){
  int tuple = '(';
  fwrite(&tuple, 1, 1, binary_file);
  fseek(binary_file, 0, SEEK_END);
  int zero_inside = 0;
  fwrite(&zero_inside, 1, 4, binary_file);
  fseek(binary_file, 0, SEEK_END);
}

void py_cb_to_binary(FILE* binary_file, py_cb_t codeblock){
  //Get the list of every interned element
  list_t list_already_interned = pyobj_interned_to_list_already_interned(codeblock->binary.content.interned);

  //Header serialization
  (codeblock->is_here[VERSION_PYVM] ? write_int_in_file(binary_file, codeblock->version_pyvm, 2)              : write_nullsize_in_file(binary_file));
  (codeblock->is_here[SOURCE_SIZE]  ? write_int_in_file(binary_file, codeblock->binary.header.source_size, 2) : write_nullsize_in_file(binary_file));
  (codeblock->is_here[TIMESTAMP]    ? write_int_in_file(binary_file, codeblock->binary.header.timestamp, 4)   : write_nullsize_in_file(binary_file));
  int code_type = PYOBJ_CODEBLOCK; fwrite(&code_type, 1, 1, binary_file); fseek(binary_file, 0, SEEK_END);
  (codeblock->is_here[ARG_COUNT]    ? write_int_in_file(binary_file, codeblock->header.arg_count, 4)    : write_nullsize_in_file(binary_file));
  (codeblock->is_here[LOCAL_COUNT]  ? write_int_in_file(binary_file, codeblock->header.local_count, 4)  : write_nullsize_in_file(binary_file));
  (codeblock->is_here[FLAGS]        ? write_int_in_file(binary_file, codeblock->header.stack_size, 4)   : write_nullsize_in_file(binary_file));
  (codeblock->is_here[STACK_SIZE]   ? write_int_in_file(binary_file, codeblock->header.flags, 4)        : write_nullsize_in_file(binary_file));

  //Content serialization
  (codeblock->is_here[BYTECODE] ? pyobj_to_binary(binary_file, codeblock->binary.content.bytecode,NULL)                    : write_nullsize_in_file(binary_file));
  (codeblock->is_here[CONSTS]   ? pyobj_to_binary(binary_file, codeblock->binary.content.consts, list_already_interned)    : write_nulltupl_in_file(binary_file));
  (codeblock->is_here[NAMES]    ? pyobj_to_binary(binary_file, codeblock->binary.content.names, list_already_interned)     : write_nulltupl_in_file(binary_file));
  (codeblock->is_here[VARNAMES] ? pyobj_to_binary(binary_file, codeblock->binary.content.varnames, list_already_interned)  : write_nulltupl_in_file(binary_file));
  (codeblock->is_here[FREEVARS] ? pyobj_to_binary(binary_file, codeblock->binary.content.freevars, list_already_interned)  : write_nulltupl_in_file(binary_file));
  (codeblock->is_here[CELLVARS] ? pyobj_to_binary(binary_file, codeblock->binary.content.cellvars,list_already_interned)   : write_nulltupl_in_file(binary_file));

  //Trailer serialization
  (codeblock->is_here[FILENAME]     ? pyobj_to_binary(binary_file, codeblock->binary.trailer.filename, list_already_interned)  : write_nullsize_in_file(binary_file));
  (codeblock->is_here[NAME]         ? pyobj_to_binary(binary_file, codeblock->binary.trailer.name, list_already_interned)      : write_nullsize_in_file(binary_file));
  (codeblock->is_here[FIRSTLINELO]  ? write_int_in_file(binary_file, codeblock->binary.trailer.firstlinelo, 4)                 : write_nullsize_in_file(binary_file));
  (codeblock->is_here[LNOTAB]       ? pyobj_to_binary(binary_file, codeblock->binary.trailer.lnotab, NULL)                     : write_nullsize_in_file(binary_file));

  //Free list_already_interned
  list_delete(list_already_interned,already_interned_delete);
}



void py_cb_to_binary_function(FILE* binary_file, py_cb_t codeblock, list_t list_already_interned){

  //Function's header serialization
  int code_type = PYOBJ_CODEBLOCK; fwrite(&code_type, 1, 1, binary_file); fseek(binary_file, 0, SEEK_END);
  (codeblock->is_here[ARG_COUNT]    ? write_int_in_file(binary_file, codeblock->header.arg_count, 4)    : write_nullsize_in_file(binary_file));
  (codeblock->is_here[LOCAL_COUNT]  ? write_int_in_file(binary_file, codeblock->header.local_count, 4)  : write_nullsize_in_file(binary_file));
  (codeblock->is_here[FLAGS]        ? write_int_in_file(binary_file, codeblock->header.stack_size, 4)   : write_nullsize_in_file(binary_file));
  (codeblock->is_here[STACK_SIZE]   ? write_int_in_file(binary_file, codeblock->header.flags, 4)        : write_nullsize_in_file(binary_file));

  //Function's content serialization
  (codeblock->is_here[BYTECODE] ? pyobj_to_binary(binary_file, codeblock->binary.content.bytecode,NULL)                    : write_nullsize_in_file(binary_file));
  (codeblock->is_here[CONSTS]   ? pyobj_to_binary(binary_file, codeblock->binary.content.consts, list_already_interned)    : write_nulltupl_in_file(binary_file));
  (codeblock->is_here[NAMES]    ? pyobj_to_binary(binary_file, codeblock->binary.content.names, list_already_interned)     : write_nulltupl_in_file(binary_file));
  (codeblock->is_here[VARNAMES] ? pyobj_to_binary(binary_file, codeblock->binary.content.varnames, list_already_interned)  : write_nulltupl_in_file(binary_file));
  (codeblock->is_here[FREEVARS] ? pyobj_to_binary(binary_file, codeblock->binary.content.freevars, list_already_interned)  : write_nulltupl_in_file(binary_file));
  (codeblock->is_here[CELLVARS] ? pyobj_to_binary(binary_file, codeblock->binary.content.cellvars,list_already_interned)   : write_nulltupl_in_file(binary_file));

  //Function's trailer serialization
  (codeblock->is_here[FILENAME]     ? pyobj_to_binary(binary_file, codeblock->binary.trailer.filename, list_already_interned)  : write_nullsize_in_file(binary_file));
  (codeblock->is_here[NAME]         ? pyobj_to_binary(binary_file, codeblock->binary.trailer.name, list_already_interned)      : write_nullsize_in_file(binary_file));
  (codeblock->is_here[FIRSTLINELO]  ? write_int_in_file(binary_file, codeblock->binary.trailer.firstlinelo, 4)                 : write_nullsize_in_file(binary_file));
  (codeblock->is_here[LNOTAB]       ? pyobj_to_binary(binary_file, codeblock->binary.trailer.lnotab, NULL)                     : write_nullsize_in_file(binary_file));

}
