/**
 * @file pyasm.c
 * @author Antoine Bach and Theo Lafond
 * @date Sat Oct  08 2022
 * @brief Pyasm.
 *
 * Pyasm.
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <pyas/regexp.h>
#include <pyas/queue.h>
#include <pyas/chargroup.h>
#include <pyas/lexem.h>
#include <pyas/list.h>
#include <pyas/parsing.h>
#include <pyas/pyobj.h>
#include <pyas/py_codeblock.h>
#include <assert.h>

#define END 2
/**
 * struct of list_label that stores the labels declarations
 */
typedef struct label_def{
  int bytecode_place;
  char* name;
} *label_def_t;
/**
 * initialize list_label and allocate it
 * @param[in] bytecode_place is the current place in the code
 * @param[in] name is the string represent the name of the label
 */
label_def_t new_label_def(int bytecode_place, char* name){
  label_def_t label_def;
  label_def = calloc(1,sizeof(*label_def));
  label_def->bytecode_place = bytecode_place;
  if ( name  && *name  ) label_def->name  = strdup( name );
  return label_def;
}
/**
 * free name of label and the stuct
 * @param[in] list_label_def is pointer of the list of label which have been filled
 */
int delete_label_def(void * label_def){
  free(((label_def_t)label_def)->name);
  free(label_def);
  return 1;
}
/**
 * print of list_label
 * @param[in] list_label_def is pointer of the list of label which have been filled
 */
int print_label_def(void * label_def){
  printf("Declaration du label %s en place %d.\n", ((label_def_t)label_def)->name,((label_def_t)label_def)->bytecode_place);
  return 1;
}
/**
 * parse end of line
 * @param[in] lexems is the pointer to the list of lexems
 */
int parse_end_of_line_pyasm(list_t *lexems){
  if (2==next_lexem_is(lexems, "newline")){
    //si on a un lexeme de type newline on avance
    lexem_advance(lexems);
  }
  while(2==next_lexem_is(lexems, "newline")){
    //tant que l'on a un lexeme de type newline
    lexem_advance(lexems);
    if (1==list_empty(*lexems)){
      //si on a tout lu on renvoie que l'on est en fin de fichier
      return END;
    }
  }
  //si on a lu tous les newline et que l'on est pas en fin de fichier on renvoie 0
  return 0;
}
/**
 * run through code to fill lnotab
 * @param[in] lexems is the pointer to the list of lexems
 * @param[in] bytecode_car is the pointer to the current value of the parameter instruction
 * @param[out] bytecode_string is the pointer of bytecode
 * @param[out] bytecode_element is the pointer to the current length of bytecode
 * @param[out] list_label_def_p is pointer of the list of label which have been filled
 */
int parse_insn_bytecode_fill(list_t *lexems, short *bytecode_car, char ** bytecode_string, int *bytecode_element, list_t list_label_def){
  if(1!=next_lexem_is(lexems,"insn")){
    //si ce n'est pas une instruction erreur et on renvoie -1
    return -1;
  }
  int a=next_lexem_is(lexems,"insn::1");
  //récupère l'instruction nécessaire pour savoir si saut absolu ou relatif des labels
  char *instruction_label=lexem_return_value(lexem_peek(lexems));
  //on ajoute au bytecode la place la valeur de l'opcode
  (*bytecode_string)[*bytecode_element]= (char) lexem_return_opcode(lexem_peek(lexems));
  //on incrémente la taille courante
  (*bytecode_element)++;
  lexem_advance(lexems);
  if(2==a){
    //on vérifie que l'on a bien un paramètre
    if(2==next_lexem_is(lexems,"symbol")){
      // si l'on a un appel de label, on parcourt la liste des labels
      //tant que l'on a pas le bon label
      while(!list_empty(list_label_def) && strcmp(lexem_return_value(lexem_peek(lexems)),((label_def_t)list_first(list_label_def))->name)){
        list_label_def = list_next(list_label_def);
      }
      //si saut absolu bytecode_car vaut la valeur de la place de la déclaration du label
      if (!strcmp(instruction_label,"JUMP_IF_TRUE_OR_POP") ||
          !strcmp(instruction_label,"JUMP_IF_FALSE_OR_POP") ||
          !strcmp(instruction_label,"JUMP_ABSOLUTE") ||
          !strcmp(instruction_label,"POP_JUMP_IF_TRUE") ||
          !strcmp(instruction_label,"POP_JUMP_IF_FALSE") ||
          !strcmp(instruction_label,"CONTINUE_LOOP")){
        *bytecode_car = (short) ((label_def_t)list_first(list_label_def))->bytecode_place;
      }
      else{
        //sinon il vaut l'écart entre l'appel et la déclaration du label
        *bytecode_car = ((short) ((label_def_t)list_first(list_label_def))->bytecode_place) - *bytecode_element-2;
      }
    }
    else{
      //si l'on a pas un label on ajoute la valeur de l'instruction
      *bytecode_car=(short) atoi(lexem_return_value(lexem_peek(lexems)));
    }
    //on ajoute les deux char qui représente la valeur de l'instruction
    char * tmp = (char *) bytecode_car;
    (*bytecode_string)[*bytecode_element]=tmp[0];
    (*bytecode_element)++;
    (*bytecode_string)[*bytecode_element]=tmp[1];
    (*bytecode_element)++;
    lexem_advance(lexems);
    return 0;
  }
  return 0;
}
/**
 * run through code to fill lnotab
 * @param[in] lexems is the pointer to the list of lexems
 * @param[in] line is the pointer to the lnotab length
 * @param[in] line_car is the pointer to the current value of line
 * @param[in] lnotab_element is the pointer to the current length of lnotab
 * @param[in] bytecode_element is the pointer to the current length of bytecode
 * @param[out] lnotab_string is the pointer of lnotab
 * @param[in] bytecode_memory is the memory of the previous length of bytecode
 */
int parse_source_lineno_or_label_lnotab_and_label_fill(list_t *lexems,
  int* line, int *line_car, int* lnotab_element, int *bytecode_element,
  char ** lnotab_string, int *bytecode_memory){

  int a=next_lexem_is(lexems,"symbol");
  //permet de vérifier si l'on a un label
  lexem_advance(lexems);
  if(2!=a && *lnotab_element<=*line){
    //tant que l'on a une ligne et que l'on est encore dans la taille de lnotab
    //on calcule le delta de bytecode entre les deux lignes précédentes
    int delta_byte=(*bytecode_element-*bytecode_memory);
    char bytecode_delta= (char) delta_byte;
    //on ajoute le delta bytecode dans lnotab
    (*lnotab_string)[*lnotab_element]=bytecode_delta;
    *bytecode_memory=*bytecode_element;
    //on incrémente la taille courante de lnotab
    (*lnotab_element)++;
    //on calcule le delta de ligne entre les deux lignes précédentes
    char delta =(char) (atoi(lexem_return_value(lexem_peek(lexems)))-*line_car);
    *line_car=atoi(lexem_return_value(lexem_peek(lexems)));
    //on ajoute le delta line dans lnotab
    (*lnotab_string)[*lnotab_element]=delta;
    //on incrémente la taille courante de lnotab
    (*lnotab_element)++;
  }
  lexem_advance(lexems);
  return 0;
}
/**
 * run through code to fill lnotab and codeblock
 * @param[in] lexems is the pointer to the list of lexems
 * @param[in] line is the pointer to the lnotab length
 * @param[in] line_car is the pointer to the current value of line
 * @param[in] lnotab_element is the pointer to the current length of lnotab
 * @param[in] bytecode_car is the pointer to the current value of the parameter instruction
 * @param[in] bytecode_element is the pointer to the current length of bytecode
 * @param[out] bytecode_string is pointer of bytecode
 * @param[out] lnotab_string is the pointer of lnotab
 * @param[in] bytecode_memory is the memory of the previous length of bytecode
 * @param[in] list_label_def_p is pointer of the list of label which have been filled
 */
int parse_assembly_line_pyasm(list_t *lexems, int* line, int *line_car,
  int *lnotab_element, short* bytecode_car, int *bytecode_element,
  char ** bytecode_string, char **lnotab_string, int *bytecode_memory,list_t list_label_def){

  int a=0;
  while(END!=a){
    if (0==parse_insn_bytecode_fill(lexems, bytecode_car, bytecode_string, bytecode_element,list_label_def)){
      a=parse_end_of_line_pyasm(lexems);
      if(END!=a && 2==next_lexem_is(lexems,"dir::code_end")){
        return 0;
      }
    }
    else if (0==parse_source_lineno_or_label_lnotab_and_label_fill(lexems, line,
      line_car, lnotab_element, bytecode_element,lnotab_string, bytecode_memory)
   || 0==parse_source_lineno_or_label_lnotab_and_label_fill(lexems, line,
     line_car, lnotab_element, bytecode_element, lnotab_string, bytecode_memory)){
      a=parse_end_of_line_pyasm(lexems);
    }
  }
  return 0;
}
/**
 * run through code to calculate length of bytecode
 * @param[in] lexems is the pointer to the list of lexems
 * @param[out] bytecode_length is the pointer to the length of bytecode
 */
int parse_insn_compteur(list_t *lexems, int* bytecode_length){
  if(1!=next_lexem_is(lexems,"insn")){
    //si ce n'est pas un insn c'est que l'on a une ligne ou un label
    return -1;
  }
  int a=next_lexem_is(lexems,"insn::1");
  lexem_advance(lexems);
  if(2==a){
    // si l'on a une instruction a paramètre on incrémente de 3 bytecode_length
    lexem_advance(lexems);
    *bytecode_length=*bytecode_length+3;
    return 0;
  }
  // sinon on incrémente de 1 bytecode_length
  (*bytecode_length)++;
  return 0;
}
/**
 * run through code to calculate length of lnotab, to fill firstlinelo in codeblock
 * and fill list_of_label
 * @param[in] lexems is the pointer to the list of lexems
 * @param[in] nom is string to indiquate if it is line or label that we verify
 * @param[out] line is the pointer to the length of lnotab
 * @param[out] codeblock is a pointer of codeblock to fill
 * @param[out] list_label_def_p is pointer of the list of label which have been filled
 * @param[out] bytecode_length is the pointer to the length of bytecode
 * @param[in] first_line is either null or the number after codestart
 */
int parse_source_lineno_or_label_compteur(list_t *lexems, char* nom, int* line, py_cb_t *codeblock,list_t *list_label_def_p,int * bytecode_length,char *first_line){
  int a=strcmp(nom,"source-lineno");
  if(0!=a){
    //si on a une déclaration de label on stocke la position (id est la taille du bytecode) dans list_label_def_p
    *list_label_def_p = list_add_first(*list_label_def_p,new_label_def(*bytecode_length,lexem_return_value(list_first(*lexems))));
  }
  if(0==a && 2==next_lexem_is(lexems,"symbol")){
    //si l'on vérifiait que c'était une ligne mais que c'est une déclaration de label on quitte la fonction
    return -1;
  }
  lexem_advance(lexems);
  if(0==a){
    //si l'on a une ligne
    if(0==*line){
      //si c'est la première ligne du code
      if (NULL==first_line){
        // si l'on est pas dans une fonction on récupère la valeur de la première ligne de code
        first_line=lexem_return_value(lexem_peek(lexems));
      }
      // on ajoute dans les deux cas first line au codeblock
      py_cb_call_code(FIRSTLINELO, first_line, *codeblock, *line);
    }
    //on incrémente le nombre de ligne
    (*line)++;
  }
  lexem_advance(lexems);
  return 0;
}
/**
 * run through code to calculate length of lnotab, firstlinelo and bytecode
 * @param[in] lexems is the pointer to the list of lexems
 * @param[out] line is the pointer to the length of lnotab
 * @param[out] bytecode_length is the pointer to the length of bytecode
 * @param[out] codeblock is a pointer of codeblock to fill
 * @param[out] list_label_def_p is pointer of the list of label which have been filled
 * @param[in] first_line is either null or the number after codestart
 */
int parse_assembly_line_compteur(list_t *lexems, int* line, int* bytecode_length, py_cb_t *codeblock,list_t *list_label_def_p, char* first_line){
  int a=0;
  while(END!=a){
    //tant que l'on a pas une fin de fichier
    if (0==parse_insn_compteur(lexems, bytecode_length)){
      //calcul de la taille de bytecode
      a=parse_end_of_line_pyasm(lexems);
      if(END!=a && 2==next_lexem_is(lexems,"dir::code_end")){
        //si l'on a une fin de fonction on a fini de lire le code de la fonction
        return 0;
      }
    }
    else if(0==parse_source_lineno_or_label_compteur(lexems, "source-lineno", line, codeblock,list_label_def_p,bytecode_length, first_line)
   || 0==parse_source_lineno_or_label_compteur(lexems, "label", line, codeblock,list_label_def_p,bytecode_length, first_line)){
     a=parse_end_of_line_pyasm(lexems);
     // calcul de la taille de lnotab
    }
  }
  return 0;
}
/**
 * fill lnotab, firstlinelo and bytecode of the codeblock passed in parameter
 * @param[in] lexems is the pointer to the list of lexems that starts at the first .text
 * @param[out] codeblock is a pointer to the codeblock to fill
 * @param[in] bytecode_length is the pointer to the length of bytecode
 * @param[in] line is the pointer to the length of lnotab
 * @param[in] list_label_def is list of label to use to fill bytecode
 * @param[in] first_line is either null or the number after codestart
 */
int lnotab_bytecode_label_fill(list_t *lexems, py_cb_t *codeblock, int bytecode_length, int line, list_t list_label_def, char *first_line){
  line=line*2;
  //lnotab a pour taille 2 fois le nombre de ligne dans code
  char *bytecode_string=malloc(bytecode_length);
  char *lnotab_string=malloc(line);
  // on alloc bytecode et lnotab
  lexem_advance(lexems);
  parse_end_of_line_pyasm(lexems);
  short bytecode_car=0;
  int lnotab_element=0;
  int bytecode_element=0;
  int line_car=0;
  if (NULL!=first_line){
    //si on a une fonction, first line est la valeur du code start stocké dans first_line
    line_car= atoi(first_line);
  }
  int bytecode_memory=0;
  //remplissage de lnotab et bytecode
  parse_assembly_line_pyasm(lexems, &line, &line_car, &lnotab_element,
    &bytecode_car, &bytecode_element, &bytecode_string, &lnotab_string,
    &bytecode_memory,list_label_def);
  if (NULL==first_line || lnotab_string[1]==0){
    //si on n'est pas d'en une fonction ou si on est dans une fonction lambda
    // on enlève les 2 premiers éléments
    char *lnotab=malloc(line-2);
    int i;
    for (i = 2; i < line; i++){
      lnotab[i-2]=lnotab_string[i];
    }
    //on ajoute dans le codeblock lnotab et bytecode
    py_cb_call_code(LNOTAB, lnotab, *codeblock, line-2);
    free(lnotab);
  }
  else{
    // sinon on renvoie lnotab en entier
    py_cb_call_code(LNOTAB, lnotab_string, *codeblock, line);
  }
  //on ajoute dans le codeblock lnotab et bytecode
  py_cb_call_code(BYTECODE, bytecode_string, *codeblock, bytecode_length);
  free(lnotab_string);
  free(bytecode_string);
  return 0;
}

int codeblock_fill(list_t *lexems, py_cb_t *codeblock, char *first_line){
  //on garde l'adresse du début du code
  list_t lexem_list_save = *lexems;
  lexem_advance(&lexem_list_save);
  parse_end_of_line_pyasm(&lexem_list_save);
  //on passe le .text
  int bytecode_length=0;
  int line=0;
  list_t list_label_def = list_new();
  //on initialise la liste des labels
  parse_assembly_line_compteur(&lexem_list_save, &line, &bytecode_length,codeblock,&list_label_def,first_line);
  // on mesure la taille de lnotab, bytecode et on remplit liste des labels
  lnotab_bytecode_label_fill(lexems,codeblock,bytecode_length,line,list_label_def,first_line);
  //on remplit lnotab et bytecode et on remplace les labels par leur valeur dans la liste
  list_delete(list_label_def,delete_label_def);
  return 0;
}
