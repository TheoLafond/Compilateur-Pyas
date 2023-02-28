/**
 * @file py_codeblock.h
 * @author Benjamin Marty <benjamin.marty@grenoble-inp.org>
 * @brief py_codeblock module.
 *
 * Py_codeblock module
 */


#ifndef _PY_CODEBLOCK_H_
#define _PY_CODEBLOCK_H_



/*
*
*
*
* We have changed py_codeblock(_t) to py_cb(_t) du to readability even though we
* still use py_codeblock in text like comment
*
*
*
*/

typedef struct py_codeblock *py_cb_t;
#include <pyas/callbacks.h>
#include <pyas/all.h>
#include <time.h>
#define BE_INTERNED 1
#define BE_STRING 0


/**
 * Enum type to make easy to handle py_codeblock's elements
 */
enum {
  VERSION_PYVM,
  ARG_COUNT,
  LOCAL_COUNT,
  STACK_SIZE,
  FLAGS,
  PARENT,
  MAGIC,
  TIMESTAMP,
  SOURCE_SIZE,
  INTERNED,
  BYTECODE,
  CONSTS,
  NAMES,
  VARNAMES,
  FREEVARS,
  CELLVARS,
  FILENAME,
  NAME,
  FIRSTLINELO,
  LNOTAB,

  NUMBER_OF_ELEMENTS
}Id;



/**
 * This function create a py_codeblock
 * @return the py_codeblock
 */
py_cb_t py_cb_create(void);

/**
 * This function prints a py_codeblock
 * @param[in] codeblock is the codeblock to print
 */
int py_cb_print(py_cb_t codeblock);

/**
 * This function frees a py_codeblock
 * @param[in] codeblock is the codeblock to frees
 */
void py_cb_delete(py_cb_t codeblock);

/**
 * This function is a prototype of functions to add "type_element" to
 * py_codeblock. Just add the type of element at the end of the function. There
 * is only one parameter selected between the fourth first argument
 * @param[in] integer is the integer we want to add OR
 * @param[in] uinteger is the integer we want to add OR
 * @param[in] time is the time we want to add OR
 * @param[in] pyobj is the pyobj we want to add
 * @param[out] codeblock is the codeblock where we add the value
 */
void py_cb_add_                  (int integer, uint32_t uinteger, pyobj_t pyobj, time_t time, py_cb_t codeblock);
void py_cb_add_version_pyvm      (int integer,      py_cb_t codeblock);
void py_cb_add_arg_count         (uint32_t integer, py_cb_t codeblock);
void py_cb_add_local_count       (uint32_t integer, py_cb_t codeblock);
void py_cb_add_stack_size        (uint32_t integer, py_cb_t codeblock);
void py_cb_add_flags             (uint32_t integer, py_cb_t codeblock);
void py_cb_add_parent            (pyobj_t pyobj,    py_cb_t codeblock);
void py_cb_add_magic             (uint32_t integer, py_cb_t codeblock);
void py_cb_add_timestamp         (time_t time,      py_cb_t codeblock);
void py_cb_add_source_size       (uint32_t integer, py_cb_t codeblock);
void py_cb_add_interned          (pyobj_t pyobj,    py_cb_t codeblock);
void py_cb_add_bytecode          (pyobj_t pyobj,    py_cb_t codeblock);
void py_cb_add_consts            (pyobj_t pyobj,    py_cb_t codeblock);
void py_cb_add_names             (pyobj_t pyobj,    py_cb_t codeblock);
void py_cb_add_varnames          (pyobj_t pyobj,    py_cb_t codeblock);
void py_cb_add_freevars          (pyobj_t pyobj,    py_cb_t codeblock);
void py_cb_add_cellvars          (pyobj_t pyobj,    py_cb_t codeblock);
void py_cb_add_filename          (pyobj_t pyobj,    py_cb_t codeblock);
void py_cb_add_name              (pyobj_t pyobj,    py_cb_t codeblock);
void py_cb_add_firstlinelo       (uint32_t integer, py_cb_t codeblock);
void py_cb_add_lnotab            (pyobj_t pyobj,    py_cb_t codeblock);

/**
 * This function return the matching Id depending on the string given
 * @param[in] source is the Id string
 * @return integer representing the Id
 */
int py_cb_str_to_id(char *source);

/**
 * This function calls the right add functions for set directives
 * @param[in] id is the integer which will help to call the good one
 * @param[in] lexem is where we take the information we will add
 * @param[out] codeblock is the codeblock where we will add the value
 */
void py_cb_call_set_function(int id, lexem_t lexem, py_cb_t codeblock);

/**
 * This function calls the right add functions for prologue directives
 * @param[in] id is the integer which will help to call the good one
 * @param[in] l_lexem is the list of lexem containing all the information
 * @param[out] codeblock is the codeblock where we will add the value
 * @param[in] size is the size of l_lexem, we don't go through the list 2 times
 */
void py_cb_call_prologue_function(int id, list_t l_lexem, py_cb_t codeblock, int size);

/**
 * This function calls the add functions to add lnotab to the codeblock
 * @param[in] string is lnotab
 * @param[out] codeblock is the codeblock where we will add the value
 * @param[in] size is the size of lnotab
 */
void py_cb_call_code(int id, char* string, py_cb_t codeblock, int size);

/**
 * This function writes the binary file by reading the elements of a codeblock
 * @param binary_file is the file being write in binary
 * @param[in] codeblock is the pycodeblock being read
 */
void py_cb_to_binary(FILE* binary_file, py_cb_t codeblock);

/**
 * This function writes in the binary file the elements of a function
 * @param binary_file is the file being write in binary
 * @param[in] codeblock is the pycodeblock being read
 * @param[in] interned is the list of interned element in our code
 */
void py_cb_to_binary_function(FILE* binary_file, py_cb_t codeblock, list_t interned);




                  /*   ***  Function for test or old  ***   */

/**
 * This function reset the table of presence for every component of the
 * py_codeblock and free if needed
 * @param[in] codeblock is the py_codeblock who'll be reset
 */
void py_cb_reset(py_cb_t codeblock);

#endif
