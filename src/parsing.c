/**
 * @file parsing.c
 * @author Antoine Bach
 * @date Sat Sep  24 2022
 * @brief Parser.
 *
 * Parser.
 */
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <pyas/all.h>

#define END 3
#define END_OPTION -2
#define ERROR_END 4
#define TYPE_STRICT_OK 2
#define TYPE_OK 1
/**
 * This function is used to parse end of line
 * @param[in] lexems is a pointer of lexem_list we have to parse
*/
int parse_end_of_line(list_t *lexems){
  if (TYPE_STRICT_OK==next_lexem_is(lexems, "newline")){
    //si on a un lexeme de type newline on avance
    lexem_advance(lexems);
  }
  else{
    print_parse_error("Unexpected input", lexems);
    //sinon c'est une erreur et on renvoie -1
    return -1;
  }
  while(TYPE_STRICT_OK==next_lexem_is(lexems, "newline")){
    //tant que l'on a un lexeme de type newline
    lexem_advance(lexems);
    if (1==list_empty(*lexems)){
      //si on a tout lu on renvoie que l'on est en fin de fichier
      return END;
    }
  }
  //si on a lu tout les newline et que l'on est pas en fin de fichier on renvoie 0
  return 0;
}
/**
 * This function is used to verify if we have type_set
 * @param[in] lexems is a pointer of lexem_list we have to parse
 * @param[in] option: prevent to return error if is option
*/
int type_set(list_t *lexems, int option){
  if(TYPE_STRICT_OK==next_lexem_is(lexems,"dir::set")){
    //on vérifie que l'on a bien un lexeme de type set
    *lexems=list_next(*lexems);
  }
  else{
    if(!option){
      //si c'est pas le cas et qu'on n'est pas dans le cas d'une option finale
      //on renvoie -1
      print_parse_error("Unexpected input",lexems);
      return -1;
    }
    //sinon on indique que l'on est dans le cas d'une option non présente
    return END_OPTION;
  }
  if (-1==blank_or_not(lexems)){
    //si l'on a un blanc on renvoie 0 sinon -1
    print_parse_error("No blank or Unexpected comment",lexems);
    return -1;
  }
  return 0;
}
/**
 * This function is used to parse tuple et liste
 * @param[in] nom: nom de ce que l'on parse
 * @param[in] required_lexeme1: type du premier lexeme grammaticalement nécessaire
 * @param[in] required_lexeme2: type du deuxième lexeme grammaticalement nécessaire
*/
int parse_tuple_or_list(list_t *lexems, char *required_lexeme1, char *required_lexeme2,pyobj_t * pyobj_tuple){
  if(TYPE_STRICT_OK!=next_lexem_is(lexems,required_lexeme1)){
    //si l'on a pas le type de léxème souhaité on renvoie erreur soit -1
    return -1;
  }
  lexem_advance(lexems);
  int size_tmp_list = 0;
  queue_t q_tmp_pyobj = queue_new();
  pyobj_t pyobj_const = NULL;
  while(-1!=blank_or_not(lexems)){
    // tant que l'on a un blance
    if(-1==parse_constant(lexems,&pyobj_const)){
      //on regarde si l'on a pas une constante
      if(TYPE_STRICT_OK==next_lexem_is(lexems,required_lexeme2)){
        // si on a une fin de tuple ou liste on a fini de lire et on renvoie 0
        if(size_tmp_list!=0){
          list_t tmp_pyobj = queue_to_list(&q_tmp_pyobj);
          *pyobj_tuple = pyobj_list_to_pyobj(tmp_pyobj, size_tmp_list);
          list_delete(tmp_pyobj, NULL);
        }
        else{
          *pyobj_tuple = pyobj_list_to_pyobj(NULL, size_tmp_list);
        }
        return 0;
      }
      else{
        //si pas de constante et pas fin de tuple ou liste on renvoie erreur
        return -1;
      }
    }
    else{
      q_tmp_pyobj = enqueue(q_tmp_pyobj,pyobj_const);
      size_tmp_list++;
    }
    lexem_advance(lexems);
  }
  if(TYPE_STRICT_OK==next_lexem_is(lexems,required_lexeme2)){
    // si on a une fin de tuple ou liste on a finit de lire et on renvoie 0
    return 0;
  }
  //sinon on renvoie -1
  return -1;
}
/**
 * This function is used to parse function
 * @param[in] lexems is pointer of lexem_list we have to parse
 * @param[out] pyobj_codeblock_p is a pointer of pyobj to fill
*/
int parse_function(list_t *lexems, pyobj_t *pyobj_codeblock_p){
  if (TYPE_STRICT_OK!=next_lexem_is(lexems,"dir::code_start")){
    // si l'on a pas une fonction on quitte
    return -1;
  }
  lexem_advance(lexems);
  if (-1==blank_or_not(lexems)){
    //on vérifie que l'on a un blanc
    print_parse_error("No blank or Unexpected comment",lexems);
    return -1;
  }
  //on récupère l'entier après code start que l'on va mettre dans first lineno
  char *first_line=lexem_return_value(lexem_advance(lexems));
  // on initialise la mémoire qui stockera l'adresse du début du code de la fonction
  list_t mem=*lexems;
  // on parse la fonction et on récupère le codeblock de la fonction
  py_cb_t codeblock=parse_pys(lexems, &mem);
  if (NULL == codeblock){
    // si le codeblock est vide cela veut dire que le parsing a une erreur
    return -1;
  }
  // on remplit first lineno, bytecode et lnotab
  codeblock_fill(&mem,&codeblock,first_line);
  // on complète le pyobj
  *pyobj_codeblock_p = pyobj_codeblock(codeblock);
  return 0;
}
/**
 * This function is used to parse complex
 * @param[in] lexems is pointer of lexem_list we have to parse
 * @param[out] pyobj_complex is a pointer of pyobj to fill
*/
int parse_complex(list_t *lexems, pyobj_t *pyobj_complex){
  char *end_real;
  char *end_im;
  double real,im;
  if(TYPE_OK!=next_lexem_is(lexems,"complex")){
    // si on a pas de complexe on renvoie erreur
    return -1;
  }
  real=strtod(lexem_return_value(lexem_advance(lexems)),&end_real);
  //récupère le double de la partie réelle du complexe
  while('+'!=*end_real && '-'!=*end_real){
    // tant que l'on a un blanc on avance
    end_real++;
  }
  if (TYPE_STRICT_OK!=next_lexem_is(lexems,"float") && TYPE_STRICT_OK!=next_lexem_is(lexems,"integer::dec")){
    // si on a pas un nombre on renvoie erreur
    print_parse_error("la partie imaginaire du complexe n'est ni un entier ni un float",lexems);
    return -1;
  }
  im=strtod(lexem_return_value(lexem_advance(lexems)),&end_im);
    //récupère le double de la partie imaginaire du complexe
  if('-'==*end_real){
    im=-im;
  }
  if('j'!=*lexem_return_value(lexem_peek(lexems))){
    //si l'on a pas le j du imaginaire c'est une erreur
    print_parse_error("la partie imaginaire n'est pas suivi de j ",lexems);
    return -1;
  }
  //remplit le pyobj
  *pyobj_complex= pyobj_complex_to_pyobj_val(real,im);
  return 0;
}

int parse_constant(list_t *lexems, pyobj_t *pyobj_p){
  if (0==parse_complex(lexems,pyobj_p)
  || 0==parse_tuple_or_list(lexems,"paren::left","paren::right",pyobj_p)
  || 0==parse_tuple_or_list(lexems,"brack::left","brack::right",pyobj_p)
  || 0==parse_function(lexems,pyobj_p)){
    return 0;
  }
  else if (TYPE_OK==next_lexem_is(lexems,"integer")
    || TYPE_STRICT_OK==next_lexem_is(lexems,"float")
    || TYPE_STRICT_OK==next_lexem_is(lexems,"string")
    || TYPE_OK==next_lexem_is(lexems,"Pycst")){
      //vérifie que l'on a soit un entier, un float, une string, un costante py,
      *pyobj_p = pyobj_lexem_to_pyobj_val(lexem_peek(lexems));
      return 0;
  }
  return -1;
}
/**
 * This function is used to parse instruction of code
 * @param[in] lexems is pointer of lexem_list we have to parse
*/
int parse_insn(list_t *lexems){
  if(TYPE_OK!=next_lexem_is(lexems,"insn")){
    //si ce n'est pas une instruction erreur et on renvoie -1
    return -1;
  }
  int a=next_lexem_is(lexems,"insn::1");
  int return_value=lexem_value_ok_or_not(lexem_peek(lexems),"RETURN_VALUE");
  lexem_advance(lexems);
  if(TYPE_STRICT_OK==a){
    //si instruction avec paramètre
    if(TYPE_STRICT_OK==next_lexem_is(lexems,"symbol") || TYPE_STRICT_OK==next_lexem_is(lexems,"integer::dec")){
      //on vérifie que l'on a bien un paramètre
      lexem_advance(lexems);
      return 0;
    }
    else{
      //sinon c'est une erreur si l'on a pas de paramètres ou si on a un
      //paramètre d'une autre nature qu'un symbole ou entier decimale
      print_parse_error("No or wrong parameter",lexems);
      return -1;
    }
  }
  //sinon instruction sans parametre
  if(TYPE_STRICT_OK!=next_lexem_is(lexems,"newline")){
    //on vérifie que l'on a bien pas de parametre, i e que l'on a un saut de ligne
    //on renvoie -1 si erreur
    print_parse_error("parameter following no parameter instruction",lexems);
    return -1;
  }
  if(1==return_value){
    return END;
  }
  //0 sinon
  return 0;
}
/**
 * This function is used to parse line and label of code of .pys file
 * @param[in]nom: nom de ce que l'on parse
 * @param[in] required_lexeme1: type du premier lexeme grammaticalement nécessaire
 * @param[in] required_lexeme2: type du deuxième lexeme grammaticalement nécessaire
*/
int parse_source_lineno_or_label(list_t *lexems, char* nom, char* required_lexeme1, char* required_lexeme2){
  int a=strcmp(nom,"source-lineno");
  if(TYPE_STRICT_OK!=next_lexem_is(lexems,required_lexeme1)){
    //vérifie que l'on a le bon type de lexeme
    return -1;
  }
  if(0==a){
    //si c'est une ligne
    *lexems=list_next(*lexems);
    if (-1==blank_or_not(lexems)){
      //on vérifie que l'on a un blanc
      print_parse_error("No blank or Unexpected comment",lexems);
      return -1;
    }
  }
  else{
    //si c'est un label on avance au prochain lexeme en évitant les blancs
    //car blanc pas obligatoire pour label
    lexem_advance(lexems);
  }
  if(TYPE_STRICT_OK==next_lexem_is(lexems,required_lexeme2)){
    //vérifie que l'on a bien le bon type de lexeme
    lexem_advance(lexems);
    return 0;
  }
  //sinon erreur on renvoie -1
  print_parse_error("Unexpected input",lexems);
  return -1;
}
/**
 * This function is used to parse list of instruction of code
 * @param[in] lexems is pointer of lexem_list we have to parse
 * @param[out] sortie is a pointer to remember if we are at end of file
*/
int parse_insn_sequence(list_t *lexems,int *sortie){
  if(-1==parse_insn(lexems)){
    return -1;
  }
  *sortie=parse_end_of_line(lexems);
  int a=0;
  while(0==(a=parse_insn(lexems))){
    //tant que l'on a une instruction on continue
    *sortie=parse_end_of_line(lexems);
    if(END==*sortie){
      //si on a fin de fichie on renvoie qu'on a une erreur
      return -1;
    }
  }
  if(END==a){
    // si l'on a un return value
    if (END==parse_end_of_line(lexems)){
      // si l'on a fini de lire le fichier le parsing est réussi
      return END;
    }
    if(TYPE_STRICT_OK==next_lexem_is(lexems,"dir::code_end")){
      //si on a fin de function on renvoie qu'on a fini de lire la fonction et
      //se place au lexème suivant si pas d'erreur
      return END;
    }
    if(END==parse_insn_sequence(lexems, sortie)){
      // si l'on a un return value on regarde si l'on a encore des instructions
      return END;
    }
  }
  if(TYPE_STRICT_OK==next_lexem_is(lexems,"dir::code_end")){
    //si on a fin de function on renvoie une erreur car on a pas de return value avant
    return -1;
  }
  return *sortie;
}
/**
 * This function is used to parse the content of code
 * @param[in] lexems is pointer of lexem_list we have to parse
*/
int parse_assembly_line(list_t *lexems){
  int sortie=0;
  //si l'on a un label, une ligne ou une instruction 0 sinon on renvoie -1
  if (0!=parse_source_lineno_or_label(lexems, "source-lineno", "dir::line", "integer::dec")){
    return -1;
  }
  sortie=parse_end_of_line(lexems);
  int label=parse_source_lineno_or_label(lexems, "label", "symbol", "colon");
  if(-1==label){
    int a=0;
    if(0!=(a=parse_insn_sequence(lexems,&sortie))){
      return a;
    }
    label=parse_source_lineno_or_label(lexems, "label", "symbol", "colon");
  }
  while(0==label){
    sortie=parse_end_of_line(lexems);
    int a=0;
    if(0!=(a=parse_insn_sequence(lexems,&sortie))){
      return a;
    }
    label=parse_source_lineno_or_label(lexems, "label", "symbol", "colon");
  }
  return 0;
}
/**
 * This function is used to parse the prologue
 * @param[in] nom:nom de ce que l'on parse
 * @param[in] test_value: type du lexeme que l'on veut vérifié s'il est présent ou non
 * @param[in] required_lexeme: type du lexeme nécessaire grammaticalement à la
 * test_value associée
 * @param[in] option: indique si test_value est optionnel ou non
 * @param[out] codeblock: on remplit codeblock des lexemes du prologue et le code
*/
int parse_rest_of_prologue_and_code(list_t *lexems, char* nom, char* test_value, char* required_lexeme, int option, py_cb_t codeblock){
  if(TYPE_STRICT_OK!=next_lexem_is(lexems,test_value)){
    //vérifie que l'on a le bon type de test_value
    if(option){
      //si option cela veut dire qu'option non présente et donc on renvoie
      //fin d'option
      return END_OPTION;
    }
    //sinon cela veut dire que c'est un erreur et on renvoie -1
    print_parse_error("Unexpected input",lexems);
    return -1;
  }
  //Stocke le type de l'élement du codeblock que nous allons remplir
  int futur_codeblock_type = py_cb_str_to_id(nom);

  lexem_advance(lexems);
  if(-1==parse_end_of_line(lexems)){
    //vérifie que l'on a bien un saut de ligne qui est obligatoire
    //renvoie -1 si erreur 0 sinon
    return -1;
  }
  //Création d'une file temporaire pour enlever les <eol> de ma liste initiale. Deviendra un liste
  queue_t q_tmp_pyobj = queue_new();
  //Entier correspondant à la taille de ma liste temporaire
  int l_size = 0;
  if(strcmp("dir::consts",test_value)!=0 && strcmp("dir::text",test_value)!=0 ){
    //vérifie que l'on est ni dans le cas d'une directive constantes et
    // ni dans le cas code qui nécessite l'utilisation d'une fonction
    while(TYPE_STRICT_OK==next_lexem_is(lexems,required_lexeme)){
      //on vérifie que tant que l'on a le type de lexeme nécessaire on continue
      //Je stock le lexem dans ma file temporaire
      lexem_t mem_lex = list_first(*lexems);

      lexem_advance(lexems);

      if(-1==parse_end_of_line(lexems)){
        //vérifie que l'on a bien un saut de ligne qui est obligatoire
        //renvoie -1 si erreur 0 sinon
        return -1;
      }
      //Ajout après vérification
      q_tmp_pyobj = enqueue(q_tmp_pyobj, pyobj_lexem_to_pyobj_val(mem_lex));
      l_size++;
    }
    if (q_tmp_pyobj!=queue_new()){
      list_t tmp_pyobj = queue_to_list(&q_tmp_pyobj);
      py_cb_call_prologue_function(futur_codeblock_type, tmp_pyobj, codeblock, l_size);
      list_delete(tmp_pyobj, NULL);
    }
  }
  else{
    //si l'on est dans le cas d'un directive constantes ou code
    if (strcmp("dir::consts",test_value)==0 ){
      //si directive constantes
      pyobj_t pyobj_const = NULL;
      while(-1!=parse_constant(lexems,&pyobj_const)){

        //tant que l'on a une constante on continue
        //lexem_t mem_lex = list_first(*lexems);
        lexem_advance(lexems);
        if(-1==parse_end_of_line(lexems)){
          //vérifie que l'on a bien un saut de ligne qui est obligatoire
          //renvoie -1 si erreur 0 sinon
          return -1;
        }
        q_tmp_pyobj = enqueue(q_tmp_pyobj, pyobj_const);
        l_size++;
      }
      if (q_tmp_pyobj!=queue_new()){
        list_t tmp_pyobj = queue_to_list(&q_tmp_pyobj);
        py_cb_call_prologue_function(futur_codeblock_type, tmp_pyobj, codeblock, l_size);
        list_delete(tmp_pyobj, NULL);
      }
    }
    else{
      //si l'on'est dans le cas du code
      int mem=0;
      while(0==(mem=parse_assembly_line(lexems))){
        //tant que l'on une ligne assembleur
        }
        if(mem==END){
          return 0;
        }
        if(mem==ERROR_END){
          print_parse_error("RETURN_VALUE présent mais non souhaité",lexems);
          return -1;
        }
        if (!list_empty(*lexems)){
          print_parse_error("il manque instruction RETURN_VALUE ou .code_end",lexems);
          return -1;
        }
        printf("[ERROR]il manque instruction RETURN_VALUE en fin de fichier.\n");
        return -1;
      }
    }
  //si tout est vérifié précedemment c'est que le parsing est correct
  //et donc on renvoie 0
  return 0;
}
/**
 *This function parse the set general of .pys file
 * @param[in] test_value:le char* correspondant au lexeme que l'on veut 03_test_lexer
 * @param[in] test_value_optionnal: s'il y a une option le char* correspondant au lexeme
 * que l'on veut tester pour voir si l'option est présente ou non sinon char* est vide
 * @param[in] required_lexeme: type du lexeme nécessaire au niveau grammaire pour la
 * set directive testée
 * @param[in] option_fin:int qui indique si on est dans le cas des deux options de fins et
 * donc test_value sera traité comme une option
 * @param[in] not_option=!strcmp("",test_value_optionnal);
 * permet de voir s'il y a une option
 * s'il y a une option print test_value_optionnal sinon test_value
 * @param[out] codeblock: on remplit codeblock des lexemes du prologue et le code
*/
int parse_set_general(list_t *lexems, char* test_value, char *test_value_optionnal, char* required_lexeme, int option_fin, py_cb_t codeblock){
  if(-1==type_set(lexems,option_fin)){
    //vérifie si l'on a bien le lexeme set et un blanc renvoie -1 si erreur 0 sinon
    return -1;
  }
  lexem_t a =(lexem_t) list_first(*lexems);
  if (lexem_value_ok_or_not(a,test_value_optionnal)){
    //on teste s'il y a une option
    *lexems=list_next(*lexems);
    if(-1==blank_or_not(lexems)){
      //s'il n'y a pas de blanc renvoie -1  sinon 0
      print_parse_error("No blank or Unexpected comment",lexems);
      return -1;
    }
    if(TYPE_STRICT_OK!=next_lexem_is(lexems,required_lexeme)){
      //vérifie que l'on a bien le bon type de lexeme renvoie -1 si erreur 0 sinon
      print_parse_error("Unexpected input",lexems);
      return -1;
    }
    lexem_t mem_lex = list_first(*lexems);
    lexem_advance(lexems);
    if(-1==parse_end_of_line(lexems)){
      //vérifie que l'on finit bien par un saut de ligne
      return -1;
    }
    py_cb_call_set_function(py_cb_str_to_id(test_value), mem_lex, codeblock);
    //si tous les tests sont passé ça veut dire que la set directive est correct
    int c=type_set(lexems,option_fin);
    if(-1==c){
      //s'il y a eu une option on regarde s'il y a une autre directive
      //set derrière renvoie -1 si erreur 0 sinon
      return -1;
    }
    else{
      if(END_OPTION==c){
        //si l'on est dans le cas des option de fin
        //et qu'il n'y a qu'une des deux option on renvoie 0 car les set
        //directive sont corrects
        return 0;
      }
    }
  }
  a =(lexem_t) list_first(*lexems);
  lexem_t mem_lex=NULL;
  if (lexem_value_ok_or_not(a,test_value)){
    //vérifie si on a bien la bonne set directive
    *lexems=list_next(*lexems);
    if(-1==blank_or_not(lexems)){
      //vérifie que l'on a un blanc renvoie -1 si erreur 0 sinon
      print_parse_error("No blank or Unexpected comment",lexems);
      return -1;
    }
    if(TYPE_STRICT_OK!=next_lexem_is(lexems,required_lexeme)){
      //vérifie que l'on a bien le bon type de lexème associé grammaticalement
      //à la set directive sélectionnée renvoie -1 si erreur 0 sinon
      print_parse_error("Unexpected input",lexems);
      return -1;
    }
    mem_lex = list_first(*lexems);
    lexem_advance(lexems);
  }
  else{
    if(!option_fin){
      print_parse_error("Not the right set-directives",lexems);
      return -1;
    }
    else{
      return 0;
    }
  }
  //si tous les tests sont passé ça veut dire que la set directive est correct
  //si on a bien un saut de ligne à la fin. Renvoie -1 si erreur 0 sinon
  if (-1==parse_end_of_line(lexems)){
      return -1;
  }
  py_cb_call_set_function(py_cb_str_to_id(test_value), mem_lex, codeblock);
  return 0;
}
/**
 * this function parse set directives
 * @param[in] lexems is a pointer of lexem_list that we go through
 * @param[in] codeblock is the codeblock to fill
*/
int parse_set_directives(list_t *lexems, py_cb_t codeblock){
  if(-1==parse_set_general(lexems, "version_pyvm", "", "integer::dec", 0, codeblock)){
    //parse set version pyvm (explication prototype de fonction l212) renvoie -1 si erreur 0 sinon
    return -1;
  }
  if(-1==parse_set_general(lexems, "flags", "", "integer::hex", 0, codeblock)){
    //parse set flags renvoie -1 si erreur 0 sinon
    return -1;
  }
  if(-1==parse_set_general(lexems, "filename", "", "string", 0, codeblock)){
    // parse set filename renvoie -1 si erreur 0 sinon
    return -1;
  }
  if(-1==parse_set_general(lexems, "name", "", "string", 0, codeblock)){
    //parse set name renvoie -1 si erreur 0 sinon
    return -1;
  }
  if(-1==parse_set_general(lexems, "stack_size", "source_size", "integer::dec", 0, codeblock)){
    //parse set stack_size et source_size renvoie -1 si erreur 0 sinon
    return -1;
  }
  if(-1==parse_set_general(lexems, "arg_count", "", "integer::dec", 0, codeblock)){
    //parse set arg_count renvoie -1 si erreur 0 sinon
    return -1;
  }
  return parse_set_general(lexems, "posonly_arg_count", "kwonly_arg_count", "integer::dec", 1, codeblock);
  //parse set posonly_arg_count et kwonly_arg_count renvoie -1 si erreur 0 sinon
}
/**
 * this function parse all the prologue
 * @param[in] lexems is a pointer of lexem_list that we go through
 * @param[in] codeblock is the codeblock to fill
*/
int parse_prologue(list_t *lexems, py_cb_t codeblock){
  if(-1==parse_set_directives(lexems, codeblock)){
    // parsing des directives set renvoie -1 si erreur 0 sinon
    return -1;
  }
  if(-1==parse_rest_of_prologue_and_code(lexems, "interned-strings", "dir::interned", "string", 1, codeblock)){
    //parse interned renvoie -1 si erreur 0 sinon (explication prototype de la fonction l145)
    return -1;
  }
  if(-1==parse_rest_of_prologue_and_code(lexems, "constants", "dir::consts", "", 0, codeblock)){
    //parse constants renvoie -1 si erreur 0 sinon
    return -1;
  }
  if(-1==parse_rest_of_prologue_and_code(lexems, "names", "dir::names", "string", 1, codeblock)){
    //parse names renvoie -1 si erreur 0 sinon
    return -1;
  }
  if(-1==parse_rest_of_prologue_and_code(lexems, "varnames", "dir::varnames", "string", 1, codeblock)){
    //parse varnames renvoie -1 si erreur 0 sinon
    return -1;
  }
  if(-1==parse_rest_of_prologue_and_code(lexems, "freevars", "dir::freevars", "string", 1, codeblock)){
    //parse freevars renvoie -1 si erreur 0 sinon
    return -1;
  }
  if(-1==parse_rest_of_prologue_and_code(lexems, "cellvars", "dir::cellvars", "string", 1, codeblock)){
    //parse cellvars renvoie -1 si erreur 0 sinon
    return -1;
  }
  return 0;
}

py_cb_t parse_pys(list_t *lexems, list_t *memoire){
  py_cb_t codeblock=py_cb_create();
  py_cb_add_source_size(2573, codeblock);
  time_t rawtime;
  py_cb_add_timestamp(time(&rawtime), codeblock);


  if(-1==parse_end_of_line(lexems)){
    //vérifie que l'on a bien un saut de ligne en début du fichier assembleur renvoie -1 si erreur 0 sinon
    py_cb_delete(codeblock);
    return NULL;
  }
  if(-1==parse_prologue(lexems, codeblock)){
    // parse  le prologue du code assembleur renvoie -1 si erreur 0 sinon
    py_cb_delete(codeblock);
    return NULL;
  }
  *memoire=*lexems;
  if(-1==parse_rest_of_prologue_and_code(lexems, "code", "dir::text", "", 0, codeblock)){
    //parse le code de l'assembleur renvoie -1 si erreur 0 sinon
    py_cb_delete(codeblock);
    return NULL;
  }
  return codeblock;
}
