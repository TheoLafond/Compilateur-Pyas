/**
 * @file doclex.h
 * @author Theo Lafond <theo.lafond@phelma.grenoble-inp.fr>
 * @brief doclex module.
 *
 * Doclex module
 */

#ifndef _DOCLEX_H_
#define _DOCLEX_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <pyas/chargroup.h>
#include <pyas/queue.h>

/**
 * The type doclex is a list of deflex
 * it is used to represent a file of lexeme definition
 */
typedef list_t doclex_t;


/**
 * A deflef type represents a lexeme. So it is composed of a type : the name of
 * the lexeme and a list of chargroup to represent the lexeme
 */
typedef struct deflex{
  char* type;
  unsigned char opcode;
  list_t list_chargroup;
} *deflex_t;


/**
 * This function delets spaces, tabs and \n at the end of a string
 * @param[in] string is the string tu reduce
 * @param[in] size is the size of the string
 */
void del_space_after_str(char* string, int size);

/**
 * This function creats a doclex_t representing the lexem definition file
 * @author Theo Lafond <theo.lafond@phelma.grenoble-inp.fr>
 * @param[in] filename of the lexeme definition file
 * @return doclex created
 */
doclex_t file_to_doclex(const char* filename);

/**
 * This function prints doclex
 * @param[in] doclex is the doclex to print
 */
void doclex_print(doclex_t doclex);

/**
 * This function prints deflex
 * @param[in] _deflex is a pointer to the deflex to print
 */
int deflex_print(void *_deflex);

/**
 * This function frees a doclex and all of what's in it
 * @param[in] doclex is the doclex to delete
 */
void delete_doclex(doclex_t doclex);

#ifdef __cplusplus
}
#endif

#endif /* _DOCLEX_H_ */
