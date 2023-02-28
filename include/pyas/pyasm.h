/**
 * @file pyasm.h
 * @author Antoine Bach
 * @date Sat Oct  08 2022
 * @brief Pyasm.
 *
 * Pyasm.
 */
#ifndef _PYASM_H_
#define _PYASM_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <pyas/lexem.h>
#include <pyas/py_codeblock.h>

/**
 * fill lnotab, firstlinelo and bytecode of the codeblock passed in parameter
 * @param lexems is the pointer to the list of lexems that starts at the first .text
 * @param codeblock is a pointer to the codeblock to fill
 * @param first_line is either null or the number after codestart
 */
int codeblock_fill(list_t *lexems, py_cb_t *codeblock, char* first_line);

#ifdef __cplusplus
}
#endif

#endif /* _PYASM_H_ */
