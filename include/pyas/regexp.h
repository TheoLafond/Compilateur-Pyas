/**
 * @file regexp.h
 * @author Benjamin Marty <benjamin.marty@grenoble-inp.org>
 * @brief regexp module.
 *
 * Regexp module
 */

#ifndef _REGEXP_H_
#define _REGEXP_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <pyas/chargroup.h>
#include <pyas/list.h>


/**
 * This function match a part of a string with a list of chargroup nd return the
 * rest if the line isn't fully match
 * @param[in] chargroup_list is the list of chargroup that will match with
 * the source
 * @param[in] source is the beginning of the source
 * @param[out] end is a pointer to the end of the matched string, used to get
 * the start of the following source in the line
 * @return length between source and end or false
 */
int re_match (list_t chargroup_list, char *source, char **end);

/**
 * This function match is the same as above but specificly use when nothing is
 * match (when the operator is * or ?) When nothing is match, it return the
 * between source and *end so it is 0 which is false... but in this case it is
 * true. This function is called when we are in this case and we want to return
 * true.
 */
int re_match_print(list_t list, char *source, char **end);

#ifdef __cplusplus
}
#endif

#endif /* _REGEXP_H_ */
