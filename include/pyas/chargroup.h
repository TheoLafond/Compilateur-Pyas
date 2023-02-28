/**
* @file chargroup.h
* @author Antoine Bach
* @date 16/09/22
* @brief chargroup module
*
* Chargroup module
*/

#ifndef _CHARGROUP_H_
#define _CHARGROUP_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <pyas/list.h>
#define NBRCHAR 128

/**
 * Enum operator_t used to memorize operator of the regexp
 */

typedef enum{
  CHARGROUP_OPERATOR_ZERO_OR_MORE,
  CHARGROUP_OPERATOR_ONE_OR_MORE,
  CHARGROUP_OPERATOR_ZERO_OR_ONE,
  CHARGROUP_OPERATOR_ONCE,
} operator_t;

/**
 * A chargroup type represents a lexeme. So it is composed of a type :
 * array  ASCII 128
 * permet de stocker les opérateurs
 * qui ne sont pas considérés comme des caractères
 */

typedef struct char_group *chargroup_t;

/**
 * cthis function return a new chargroup initialized
 */

chargroup_t initialisation();

/**
 * This function frees a chargroup and all of what's in it
 * @param[in] _chargroup is a pointer to the chargroup_list to delete
 */

int delete_chargroup(void *_chargroup);

/**
 * This function return the array of chargroup
 * @param[in] chargroup is a chargroup
 */

operator_t chargroup_operator(chargroup_t chargroup);

/**
 * This function return operator of chargroup
 * @param[in] chargroup is a chargroup
 */

char* chargroup_group(chargroup_t chargroup);

/**
 * This function prints clist of hargroup
 * @param[in] _chargroup is a pointer to the chargroup_list to print
 */

int print_chargroup(void *_chargroup);

/**
 * create a list of chargroup
 * @param[in] regexp of definitionLexeme.txt
 * @return list of chargroup
 */

list_t chargroup_parse(char * source);

#ifdef __cplusplus
}
#endif

#endif /* _CHARGROUP_H_ */
