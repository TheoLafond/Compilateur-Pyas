
/**
 * @file lexem.c
 * @author François Cayre <francois.cayre@grenoble-inp.fr>
 * @date Fri Jul  2 17:59:27 2021
 * @brief Lexems.
 *
 * Lexems.
 */

#define _POSIX_C_SOURCE 200809L /* strdup(3) */
#define SIZE_VALUE 256


#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>

#include <pyas/lexem.h>
#include <pyas/list.h>
#include <pyas/doclex.h>
#include <pyas/regexp.h>
#include <pyas/queue.h>


/**
*Type to describe a lexem
*/
struct lexem {
  char *type;
  char *value;
  unsigned char opcode;
  int   line;
  int   column;
};



lexem_t lexem_new( char *type, char *value,unsigned char opcode, int line, int column , int sizevalue) {
  lexem_t lex = calloc( 1, sizeof( *lex ) );

  assert( lex );

  if ( type  && *type  ) lex->type  = strdup( type );
  if(sizevalue == -1){
    if ( value && *value ) lex->value = strdup( value );
  }
  else{
    lex->value = strndup( value,sizevalue );
  }
  lex->opcode   = opcode;
  lex->line   = line;
  lex->column = column;

  return lex;
}

int     lexem_print( void *_lex ) {
  lexem_t lex = _lex;

  return printf( "[%d:%d:%s] %s",
		 lex->line,
		 lex->column,
		 lex->type,
		 lex->value );
}

int     lexem_delete( void *_lex ) {
  lexem_t lex = _lex;

  if ( lex ) {
    free( lex->type );
    free( lex->value );
  }

  free( lex );

  return 0;
}

char *  lexem_return_value(lexem_t lexem){
  return lexem->value;
}

char *  lexem_return_type(lexem_t lexem){
  return lexem->type;
}

unsigned char  lexem_return_opcode(lexem_t lexem){
  return lexem->opcode;
}

list_t lex (char *regexp_file, char *source_file){

  //Creation de la structure représentant les lexems
  doclex_t doclex = file_to_doclex(regexp_file);

  //Creation de la queue qui va recevoir tous les lexems
  queue_t queue_lexem = queue_new();
  assert (queue_lexem);

  //ouverture du fichier pys
  FILE *source;
  source = fopen (source_file, "r");

  //Test si la source a pu etre ouverte et si le fichier est vide
  if ( NULL == source || getc(source) == EOF){
    printf("\n");
    exit(EXIT_SUCCESS);
  }
  //Retour en arrière dans la lecture du fichier du a getc()
  fseek(source, -1, 1);

  unsigned int line=1;
  unsigned int column=1;
  char *buffer = NULL, *end = NULL, *buffer_place,*i_string;
  size_t bufsize = 0;
  size_t characters;
  list_t doclex_place;
  int nbmatch=0, decalage;
  //on parcourt les lignes du fichier
  while ((int)(characters = getline(&buffer,&bufsize,source))!=-1 ){
    column = 1;

    //buffer_place est la variable qui nous sert de debut de value dans rematch
    buffer_place = buffer;
    end = buffer;

    //tant que buffer place n'est pas vide
    while(0!=strlen(buffer_place)){
      doclex_place = doclex;
      //On parcourt les regexp à la recherche de la premiere qui match
      while(!list_empty(doclex_place)){
        //si ça match
        if((nbmatch = re_match(((deflex_t) list_first(doclex_place))->list_chargroup, buffer_place, &end))){
          //si ça match on sort du parcourt des regexp car on a trouvé la bonne
          break;
        }
        doclex_place = list_next(doclex_place);
      }
      //si on est arrivé à bout des regexps, c'est que l'expression n'est pas un
      // lexem
      if(list_empty(doclex_place)){
        printf("Expression line %d column %d is not a lexem\n",line,column);
        exit(EXIT_FAILURE);
      }
      else{
        // sinon on va l'ajouter à la queue des Lexems

        //mais d'abord on va echaper les \n et \t dans les strings dans cette condition :
        if (0== strcmp(((deflex_t)list_first(doclex_place))->type,"string")){
          //si on a une string, processus pour transformer les \n en 10
          decalage = 0;
          for(i_string=buffer_place;i_string<buffer_place+nbmatch;i_string++){
            *i_string = *(i_string+decalage);
            if (*i_string == '\\'){
              if (*(i_string+1+decalage) == 'n'){
                //ne peut pas dépasser car il y a toujours le guimet à la fin des str
                *i_string = 10;
                decalage++;
                nbmatch--;
              }
              if (*(i_string+1+decalage) == 't'){
                //ne peut pas dépasser car il y a toujours le guimet à la fin des str
                *i_string = 9;
                decalage++;
                nbmatch--;
              }
            }
          }
        }

        //ajout à la queue des lexems
        queue_lexem = enqueue( queue_lexem, lexem_new(((deflex_t)list_first(doclex_place))->type, buffer_place,((deflex_t)list_first(doclex_place))->opcode, line, column,nbmatch));
        column += nbmatch;
      }
      buffer_place = end;
    }
    line++;
  }

  fclose(source);
  //Supression de la liste contenant les définitons des lexems
  delete_doclex(doclex);
  free(buffer);

  return queue_to_list(&queue_lexem);
}
int lexem_type_strict(lexem_t lex, char *type){
  return !strcmp(lex->type,type);
}

int lexem_type(lexem_t lex, char *type){
  return lex->type == strstr(lex->type, type);
}
int lexem_value_ok_or_not(lexem_t lex,char *value){
  return !strcmp(lex->value,value);
}

int blank_or_not(list_t *lexems){
  lexem_t a =(lexem_t) list_first(*lexems);
  if(lexem_type_strict(a, "blank")){
    *lexems=list_next(*lexems);
  }
  else{
    return -1;
  }
  a =(lexem_t) list_first(*lexems);
  if (lexem_type_strict(a, "comment")){
    return -1;
  }
  return 0;
}

lexem_t lexem_peek(list_t *lexems){
  lexem_t a =(lexem_t) list_first(*lexems);
  while(lexem_type_strict(a, "blank") || lexem_type_strict(a, "comment")){
    *lexems=list_next(*lexems);
    a =(lexem_t) list_first(*lexems);
  }
  lexem_t b= (lexem_t) list_first(*lexems);
  return b;
}

lexem_t lexem_advance(list_t *lexems){
  if (list_empty(*lexems)){
    exit(EXIT_FAILURE);
  }
  lexem_t lex=lexem_peek(lexems);
  *lexems=list_next(*lexems);
  return lex;
}

int next_lexem_is(list_t *lexems, char *type){
  lexem_t a=lexem_peek(lexems);
  if (lexem_type_strict(a, type)==1){
    return 2;
  }
  return lexem_type(a,type);
}

void print_parse_error(char *msg, list_t *lexems){
  lexem_t lex=(lexem_t) list_first(*lexems);
  printf("[ERROR %d:%d] %s.\n",lex->line,lex->column,msg);
}
