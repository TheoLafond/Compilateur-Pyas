
/**
 * @file lexem.h
 * @author François Cayre <francois.cayre@grenoble-inp.fr>
 * @date Fri Jul  2 17:59:42 2021
 * @brief Lexems.
 *
 * Lexems.
 */

#ifndef _LEXEM_H_
#define _LEXEM_H_

#ifdef __cplusplus
extern "C" {
#endif

#define ERROR 0
#define ERROR_REACH_END_LIST 2
#define MATCHED 3
#define PARTIAL_MATCH 4
#define PERFECT_MATCH 5

#include <pyas/list.h>


/**
 * Type pointer to a lexem
 */
typedef struct lexem *lexem_t;


/**
 * Creates and allocates a new lexem
 * @param sizevalue if this parameter is not -1, it is the number of characters to
 * take from the value
 */
lexem_t lexem_new( char *type, char *value, unsigned char opcode, int line, int column, int sizevalue );

/**
 * Print a lexem
 */
int     lexem_print( void *_lex );

/**
 * Delete de lexem
 */
int     lexem_delete( void *_lex );

int     lexem_type(lexem_t lex, char *type);

int     lexem_type_strict(lexem_t lex, char *type);

/**
 * Return the value of a lexem
 */
char *  lexem_return_value(lexem_t lexem);
/**
 * Return the type of a lexem
 */
char *  lexem_return_type(lexem_t lexem);
/**
 * Return the opcode of a lexem
 */
unsigned char  lexem_return_opcode(lexem_t lexem);

/**
 * Read the source file and gives a list containing the type and the chargroup list
 * given for a lexem
 * @author Theo Lafond <theo.lafond@phelma.grenoble-inp.fr>
 * @param[in] regexp_file : name of the file containing lexems definition
 * @param[in] source_file : name of the file containing the assembly code
 *
 * @return a list containing all fo the lexems from the source file in order
 */
list_t  lex (char *regexp_file, char *source_file);

/**
 * Compares the lexem value and the value
 */
int lexem_value_ok_or_not(lexem_t lex,char *value);

int blank_or_not(list_t *lexems);

/**
 * Returns the firts lexem of the lexem list
 */
lexem_t lexem_peek(list_t *lexems);

/**
 * Points the list pointer to the next element
 */
lexem_t lexem_advance(list_t *lexems);

/**
 * Compares the first lexem type and type
 */
int next_lexem_is(list_t *lexems, char *type);
void print_parse_error(char *msg, list_t *lexems);

#ifdef __cplusplus
}
#endif

#endif /* _LEXEM_H_ */
