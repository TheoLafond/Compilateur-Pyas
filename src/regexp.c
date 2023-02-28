/**
 * @file regexp.c
 * @author François Cayre <francois.cayre@grenoble-inp.fr>
 * @date Fri Jul  28 09:56:03 2022
 * @brief regexp
 *
 * regexp code, as in the project's pdf document
 *
 */

#include <stdio.h>

#include <pyas/regexp.h>
#include <pyas/chargroup.h>
#include <pyas/lexem.h>
#include <pyas/doclex.h>

int re_match_zero_or_more(char *tab, char *source, char **end){
  char *t = source;

  while(tab[(int)*t] && '\0' != *t){
    t++;
  }

  *end = t;

  return 1;
}
int re_match_one_or_more(char *tab, char *source, char **end){
  char *t = source;
  int is_here = 0;
  while(tab[(int)*t] && '\0' != *t){
    t++; //On passe de false a true et on incremente pour garder la fin
    is_here = 1; /// C'est pas opti de toujours mettre le meme nombre a la meme valeur non ?
  }

  *end = t;
  return is_here;
}

int re_match_zero_or_one(char *tab, char *source, char **end){
  char *t = source;
  int count = 0;
  while(tab[(int)*t] && '\0' != *t && !count){
    t++;
    count++;
  }
  *end = t;

  return 1;
}

int re_match_nothing(char *tab, char *source, char **end){
  if(tab[(int)*source]){
    *end = source+1;
    return 1;
  }
  return 0;
}


//Test à partir d'un chargroup si il convient pour la chaine source à tester ou non
int re_match_once( chargroup_t chargroup, char *source, char **end ) {
  if ( NULL == source) {
    return 0;
  }
  if ('\0' == *source){
    return 1;
  }


  switch (chargroup_operator(chargroup)) {
    case CHARGROUP_OPERATOR_ZERO_OR_MORE:   return re_match_zero_or_more  ( chargroup_group(chargroup), source, end) ;
    case CHARGROUP_OPERATOR_ONE_OR_MORE:    return re_match_one_or_more   ( chargroup_group(chargroup), source, end) ;
    case CHARGROUP_OPERATOR_ZERO_OR_ONE:    return re_match_zero_or_one   ( chargroup_group(chargroup), source, end) ;
    default :                               return re_match_nothing       ( chargroup_group(chargroup), source, end) ;
  }
}



//Parcours la liste de chargroup pour tester la valeur du lexem trouvé
int re_match (list_t chargroup_list, char *source, char **end){
  list_t l=chargroup_list;
  char*source_save = source;
  //Contient un booleen correspondant à l'atteinte, ou non, du dernier chargroup de la liste.
  //Initialisé à false
  int mem_end_chargroup_list = 0;

  //Tant que la liste de chargroup n'est pas vide et que end n'est pas la fin de la ligne
  while (!list_empty(l) && '\0' != **end){

    //Test avec un seul chargroup, si ca marche on avance dans les chargroups, sinon on return false
    if(re_match_once(list_first(l), source, end)){
      //Nouvelle chaine a test est celle qui reste après le test du chargroup
      source = *end;
      //On avance dans les chargroups
      l = list_next(l);
      //Si la liste de chargroup est vide et que end n'est pas la fin de la ligne
      if(list_empty(l) && '\0' != **end){
        //On dit que le lexem est trouvé pour cette liste
        mem_end_chargroup_list = 1;
      }
    }

    else return 0;
  }
  if(mem_end_chargroup_list || '\0' == **end){
    return *end - source_save;
  }

  return 0;
}
int re_match_print(list_t list, char *source, char **end){
  list_t list_suite=list_next(list);
  int operator=-1;
  if(list_empty(list_suite)){
    operator=chargroup_operator(list_first(list));
  }
  int is_match=re_match(list,source,end);
  return (is_match || CHARGROUP_OPERATOR_ZERO_OR_MORE==operator || CHARGROUP_OPERATOR_ZERO_OR_ONE==operator);
}
