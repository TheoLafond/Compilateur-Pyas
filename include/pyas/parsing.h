/**
 * @file parsing.h
 * @author Antoine Bach
 * @date Sat Sep  24 2022
 * @brief Parser.
 *
 * Parser.
 */
#ifndef _PARSER_H_
#define _PARSER_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <pyas/all.h>

/**
 * The function parse constants of .pys file
 * @param[in] lexems is a pointer of lexem list created
 * @param[out] pyobj_p is a pointer of pyobj we have to fill
 */
int parse_constant(list_t *lexems, pyobj_t * pyobj_p);

/**
 * The function parse all .pys file
 * @param[in] lexems is a pointer of lexem list created
 * @param[in] memoire is a pointer of the beginning of the code of .pys file
 */
py_cb_t parse_pys(list_t *lexems, list_t *memoire);

#ifdef __cplusplus
}
#endif

#endif /* _PARSER_H_ */
