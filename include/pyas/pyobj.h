/**
 * @file pyobj.h
 * @author Benjamin Marty <benjamin.marty@grenoble-inp.org>
 * @brief pyobj module.
 *
 * Pyobj module
 */

#ifndef _PYOBJ_H_
#define _PYOBJ_H_

typedef struct pyobj *pyobj_t;

#include <stdint.h>
#include <stdio.h>
#include <pyas/all.h>
#include <pyas/py_codeblock.h>

/**
 * Enum type used to make pyobj type use easier
 */
enum{
  PYOBJ_NULL=         (int)'0',
  PYOBJ_FALSE=        (int)'F',
  PYOBJ_NONE=         (int)'N',
  PYOBJ_TRUE=         (int)'T',
  PYOBJ_LIST=         (int)'[',
  PYOBJ_TUPPLE=       (int)'(',
  PYOBJ_CODEBLOCK=    (int)'c',
  PYOBJ_NUMBER_S_REAL=    (int)'f',
  PYOBJ_NUMBER_REAL=      (int)'g',
  PYOBJ_NUMBER_INT=       (int)'i',
  PYOBJ_STRING=           (int)'s',
  PYOBJ_NUMBER_S_COMPLEX= (int)'x',
  PYOBJ_NUMBER_COMPLEX=   (int)'y'
}pyobj_type_value;

/**
 * The type pyobj_type represent what is the type of the pyobj
 */
typedef unsigned int pyobj_type;


/**
 * This function prints pyobj
 * @param[in] pyobj is the pyobj to print
 */
int pyobj_print(pyobj_t pyobj);

/**
 * This function frees pyobj
 * @param[in] pyobj is the pyobj to frees
 */
void pyobj_delete(pyobj_t pyobj);

/**
 * This function transforms a complex into a pyobj
 * @param[in] real is real part of complex Number
 * @param[in] im is imaginary part of complex number
 * @return pyobj
 */
pyobj_t pyobj_complex_to_pyobj_val(double real, double im);

/**
 * This function transforms a complex into a pyobj
 * @param[in] lexem that will be a pyobj
 * @return pyobj
 */
pyobj_t pyobj_lexem_to_pyobj_val(lexem_t lexem);

/**
 * This function transforms a string into a pyobj
 * @param[in] string is the string to transform into a pyobj
 * @param[in] size is the length of the string
 * @return pyobj
 */
pyobj_t pyobj_string(char *string, int size);

/**
 * This function transforms a py_codeblock into a pyobj
 * @param[in] py_codeblock that will be a pyobj
 * @return pyobj
 */
pyobj_t pyobj_codeblock(py_cb_t codeblock);

/**
 * This function transforms a list of pyobj into a pyobj list
 * @param[in] list_pyobj is the list of pyobj to transform into a pyobj
 * @param[in] size is the size of the list
 * @return pyobj
 */
pyobj_t pyobj_list_to_pyobj(list_t list_pyobj, int size);

/**
 * This function writes a pyobj into a binary file
 * @param[in] binary_file is the file where we want to write our pyobj
 * @param[in] pyobj is the pyobj we will transform in binary
 * @param[in] interned is the list of interned pyobj
 * @return pyobj
 */
void pyobj_to_binary(FILE* binary_file, pyobj_t pyobj, list_t interned);

/**
 * This type represent which one of the lexem are interned or not. We use a
 * python string to manipulate easily our interned string
 */
typedef struct already_interned{
  pyobj_t string;
  char interned_or_not;
} *already_interned_t;

/**
 * This function frees a interned_string object
 * @param[in] already_interned is the struct we want to free
 */
int already_interned_delete(void* already_interned);

/**
 * This function add a pyobj to the interned_string object
 * @param[in] interned is the pyobj we want to add to the struct
 */
list_t pyobj_interned_to_list_already_interned(pyobj_t interned);




                  /*   ***  Function for test or old  ***   */

/**
 * This function create a pyobj, only use in test for pyobj_(delete/print)
 * @param[in] type is the type of the pyobj created
 */
pyobj_t pyobj_create(int type);

/**
 * This function transforms a list of lexemes into a pyobj list
 * @param[in] list_lexem is the list of lexemes to transform into a pyobj
 * @param[in] size is the size of the list
 * @return pyobj
 */
pyobj_t pyobj_list_lexem_to_pyobj_list(list_t list_lexem, int size);


#endif
