/**
 * @file doclex.c
 * @author Theo Lafond <theo.lafond@phelma.grenoble-inp.fr>
 * @brief doclex module.
 *
 * Doclex module
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>

#include <pyas/doclex.h>

#define TAILLE_MOT 256
#define TAILLE_OPCODE 8

/**
 * Initializing a deflex
 * @param[in] type of the deflex
 * @return the new deflex
 */
deflex_t deflex_new(char * type){
  deflex_t deflex;
  deflex = calloc(1,sizeof(*deflex));
  deflex->type = type;
  deflex->opcode = 0;
  deflex->list_chargroup = queue_new();
  return deflex;
}

void del_space_after_str(char* string, int size){
  char *cara = string + size-1;
  while(cara!=string && (*cara == ' ' || *cara == '\t' || *cara == '\r' || *cara == '\n')){
    cara--;
  }
  *(cara + 1) = '\0';
}


doclex_t file_to_doclex(const char* filename){
  FILE *file;
  char ligne[TAILLE_MOT], regexp[TAILLE_MOT], key[TAILLE_MOT], opcode[TAILLE_OPCODE];
  queue_t queue_doclex;
  deflex_t deflex; //init du deflex (structure de donnée qui décrit un lexem)
  char *cara; //aucun lien, fils unique (variable de parcourt de caractère du file)
  char regexp_lecture; //bool qui est à 1 quand on est en train de lire une regexp
  int placeregexp=0, placekey = 0, placeopcode = 0;

  file = fopen(filename,"r");
  queue_doclex = queue_new();
  regexp[0] = '\0';key[0] = '\0'; opcode[0] = '\0';

  if ( NULL == file || getc(file) == EOF){
    exit(EXIT_SUCCESS);
  }
  fseek(file, -1, 1);

  while(fgets(ligne,TAILLE_MOT,file)){ //parcourt des lignes
    placeregexp = 0;
    placekey = 0;
    placeopcode = 0;
    regexp_lecture = 0;
    cara = ligne;
    while(*cara == ' '|| *cara == '\t'){
      //on saute les éventuels tab ou espaces en début de ligne
      cara++;
    }
    while(*cara!='\n' || *cara == 0){
      //parcourt des caractères dans la ligne
      if(*cara =='#' || *cara == 10){
        if(cara == ligne){
          //s'il y a un commentaire, on passe à la ligne suivante
          break;
        }
        else if(*(cara-1)!='\\'){
          break;
        }
      }
      if(*cara != ' '&& *cara != '\t' && regexp_lecture == 0){
        //si nous ne somme pas en mode de lecture de regexp et qu'il n'y a pas d'espaces
        //alors on stock les caractères de la key
        key[placekey]=*cara;
        placekey++;
      }
      else if(regexp_lecture == 0){
        //par contre s'il y a des espaces ou des tabs, on les saute
        while(*cara == ' ' || *cara == '\t'){
          cara++;
        }
        //pour ensuite passer en mode lecture de regexp
        regexp_lecture = 1;
        if(*cara == 13){
          break;
        }
        cara--;
      }
      else if(*cara != '$'&& regexp_lecture == 1){
        //ajouter les caractères au regexp
        regexp[placeregexp]=*cara;
        placeregexp++;
      }
      else if(*cara == '$'){
        regexp_lecture = 2;
      }
      else if(regexp_lecture == 2){
        opcode[placeopcode]=*cara;
        placeopcode++;
      }
      cara++;
    }
    if(placeregexp!=0){
      //à la fin d'une ligne, si celle ci contenait une regexp,
      if(placeopcode != 0){
        del_space_after_str(opcode,placeopcode);
      }
      del_space_after_str(regexp,placeregexp);
      key[placekey] = '\0';
      //on crée un deflex
      deflex = deflex_new(strdup(key));
      //dans lequel on ajoute les chargroup associés à la regexp
      deflex->list_chargroup = chargroup_parse(regexp);
      //et on ajoute le deflex à la queue
      queue_doclex = enqueue(queue_doclex,deflex);
      if(placeopcode != 0){
        deflex -> opcode = (char) strtol(opcode,NULL,0);
      }
    }
  }
  fclose(file);
  return queue_to_list(&queue_doclex);
}

void doclex_print(doclex_t doclex){
  list_print(doclex, deflex_print);
}

int deflex_print(void *_deflex){
  deflex_t deflex = _deflex;
  if(deflex->opcode != 0){
    printf("Key : %s; Opcode : 0x%x\n", deflex->type,deflex->opcode);
  }
  else {
    printf("Key : %s\n", deflex->type);
  }

  list_print(deflex->list_chargroup,print_chargroup);

  return 1;
}

int delete_deflex(void *_deflex){
  deflex_t deflex = _deflex;

  free(deflex->type);
  list_delete(deflex->list_chargroup,delete_chargroup);
  free(deflex);

  return 1;
}

void delete_doclex(doclex_t doclex){
  list_delete(doclex,delete_deflex);
}
