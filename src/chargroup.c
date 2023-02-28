/**
* @file chargroup.c
* @author Antoine Bach
* @date 16/09/22
* @brief chargroup module
*
* chargroup module
*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <pyas/queue.h>
#include <pyas/chargroup.h>


#define NOTHING 0
#define ELEMENT 1
#define ERROR -1


static char* operator_name[]={"zero or more times", "one or more times", "zero or one time", "one time"};

/**
 * A chargroup type represents a lexeme. So it is composed of a type :
 * an array of size 128 and operator of chargroup
 */
struct char_group{
  char group[NBRCHAR]; //tableau contenant ASCII 128
  operator_t operateur; // permet de stocker les opérateurs
                        //qui ne sont pas considérés comme des caractères
};


chargroup_t initialisation(){
  chargroup_t chargroup= calloc(1, sizeof(*chargroup));
  assert(chargroup!=NULL);
  memset(chargroup->group, 0, sizeof(char)*NBRCHAR); // initialise le tableau qu'avec des zero
  chargroup->operateur=CHARGROUP_OPERATOR_ONCE;
  return chargroup;
}

int delete_chargroup(void *_chargroup){
  chargroup_t chargroup = _chargroup;
  free(chargroup);

  return 1;
}

operator_t chargroup_operator(chargroup_t chargroup){
  // retourne l'opérateur qui agit sur le chargroup
  return chargroup->operateur;
}

char* chargroup_group(chargroup_t chargroup){
  //retourne le chargroup
  return chargroup->group;
}

int print_chargroup(void *_chargroup){
  // fonction d'affichage
  chargroup_t chargroup = _chargroup;
  int i;
  printf("One in '");
  for(i=0;i<NBRCHAR;i++){
    if (chargroup->group[i]==1){
      printf("%c",(char)i);
    }
  }
  printf("', %s.",operator_name[chargroup->operateur]);

  return 1;
}
/**
 * character escaped: all characters have been escaped
 * with special case for n,t,r wich have their own definition ASCII:
 *line feed, horizontal tabulation, carriage return
 * @param[in] _chargroup to fill
 * @param[in] source is pointer of regexp
 */
int backslash_parse(char* source, chargroup_t chargroup){
  chargroup->operateur=CHARGROUP_OPERATOR_ONCE;
  switch (*source) {
    case 'n':
      chargroup->group[(int)'\n']=1;
      return (int)'\n';
    case 't':
      chargroup->group[(int)'\t']=1;
      return (int)'\t';
    case 'r':
      chargroup->group[(int)'\r']=1;
      return (int)'\r';
    default:
      chargroup->group[(int)*source]=1;
      return (int)*source;
  }
  return ERROR;
}
/**
 * parsing of regexp with bracket
 * @param[in] _chargroup to fill
 * @param[in] source is pointer of pointer of regexp
 */
int bracket_parse(char** source, chargroup_t chargroup){
  //fonction qui s'occupe des caractère entre crochet
  int i;
  int a=ERROR,b;
  chargroup->operateur=CHARGROUP_OPERATOR_ONCE;
  while(**source!=']' && **source!='\0'){
    //tant que l'on arrive pas à la fin du crochet on éxecute le code
    //si source vide ça veut dire regexp non conforme car pas de ']' (voir l.133)
    if(**source=='-'){
      // '-' est un opérateur dans crochet, i.e
      //tous les caracteres du caractere avant '-'
      //jusqu'à caractère après '-'
      a++;
      b=(int) (*(*source+1));
      if ( (a-1)==ERROR || b==']'|| b=='-'){
        //on s'assure que l'on ne lit pas ']'
        //ou '-' qui doivent être échappé obligatoirement
        //la première condition assure que
        //le premier caractère après '[' n'est pas '-'
        return ERROR;
      }
      if (b == '\\'){
        //permet de traiter le cas des caractères échappés s'il y a '-'
        //b prend la valeur du caractère échappé ou de sa valeur ASCII (e.g '\n')
        *source=*source+2;
        b=backslash_parse(*source,chargroup);
        (*source)--;
      }
      if (a>b){
        // on interdit que l'on n'est e.g [z-a] ASCII forcément croissant
        return ERROR;
      }
      for(i=a;i<=b;i++){
        //boucle qui met des 1 pour tous les caractères de a+1 à b
        //car a est traité par le else(l.129)
        chargroup->group[i]=1;
      }
      *source=*source+2;
    }
    else{
      if (**source == '\\'){
        //si caractère a est échappé a prend la valeur du caratère échappés
        // ou sa valeur ASCII
        (*source)++;
        a=backslash_parse(*source,chargroup);
        (*source)++;
      }
      else{
        a=(int)**source;
        if (a=='['){
          //le premier caractère ne peut être crochet caratère
          // car il doit être forcément échappé
          return ERROR;
        }
        chargroup->group[a]=1;
        (*source)++;
      }
    }
  }
  if (**source=='\0'){
    // si on arrive à source vide mais que l'on n'a pas
    //lu u ']' c'est une erreur regexp non valide
    return ERROR;
  }
  (*source)++;
  //si on arrive ici crochet valide et donc tout le crochet est considéré comme un élément
  return ELEMENT;
}
/**
 * This function reverses all value of chargroup if negation is odd
 * @param[in] compteur_negation is the number of negation
 * @param[in] chargroup is the chargroup to fill
 */
void negation_or_not(int compteur_negation, chargroup_t chargroup){
  if(compteur_negation%2==1){
    //vérifie que le '^' apparait un nomre impair
    // si paire on ne fait rien
    //si impaire on inverse
    //e.g ^^a c'est caractère a
    //mais ^^^a c'est tout sauf a
    int i;
    for(i=0; i<NBRCHAR; i++){
      if (chargroup->group[i]==1){
        chargroup->group[i]=0;
      }
      else{
        chargroup->group[i]=1;
      }
    }
  }
}
/**
 * This function shows if element is false
 * @param[in] detect_error is a memory to remember where we are in chargroup_parse
 * @param[in] end pointer of pointer to point the rest of regexp to parse
 * @param[in] p pointer of pointer of the regexp to parse
 * @param[in] compteur_negation is the number of negation
 * @param[in] chargroup is the chargroup to fill
 */
int element_or_error(int detect_error, char** end, char** p, int compteur_negation, chargroup_t chargroup){
  //fonction utilisée pour les case des opérateurs '\\','['et ''^' (voir switch str_to_chargroup)
  if(detect_error==ELEMENT){
    //si detect_error est un élément ça veut dire
    //que l'on a e.g 'a^b' donc 'a' regexp valide on coupe
    //après le 'a' et on met '^b' dans end
    *end=*p;
    negation_or_not(compteur_negation,chargroup);
    //vérifie s'il n'y avait pas de '^' avant
    return 1;
  }
  (*p)++;
  if(**p =='\0'){
    //c'est une erreur si '\\','['et ''^'
    //ne sont pas suivis par un ELEMENT
    return 0;
  }
  //si aucun des 2 cas avant on est correct
  return -1;
}
/**
 * This function return chargroup or error if operator is false
 * @param[in] detect_error is a memory to remember where we are in chargroup_parse
 * @param[in] end pointer of pointer to point the rest of regexp to parse
 * @param[in] p pointer of pointer of the regexp to parse
 * @param[in] compteur_negation is the number of negation
 * @param[in] chargroup is the chargroup to fill
 */
int operator_or_error(int detect_error, char** end, char** p, int compteur_negation, chargroup_t chargroup){
  //fonction utilisée pour '*','+','?'
  if (!detect_error){
    //si detect_error!=ELEMENT c'est une erreur
    return 0;
  }
  //sinon on vérifie qu'il n'y avait pas '^' avant
  //l'élément sur lequel agit l'opérateur
  negation_or_not(compteur_negation,chargroup);
  (*p)++;
  //on arrête la chaine car chaine forcément valide après
  //un opérateur s'il n'y a pas eu d'erreur avant
  //end stocke la valeur du reste de la regexp
  *end=*p;
  return 1;
}
/**
 * This function return chargroup or error if operator is false
 * @param[in] source is regexp for which we create a chargroup_list
 * @param[in] end pointer of pointer of the regexp to parse
 * @param[in] chargroup is the chargroup to fill
 */
int str_to_chargroup(char * source, char** end, chargroup_t chargroup){
  char* p=source;
  int compteur_negation=0;
  int detect_error=NOTHING; //initialisation de detect_error avec rien dedans
  int a,i;
  while(*p !='\0'){
    switch (*p) {
      case '^':
        //si pas d'erreur (voir commentaire element_or_error)
        a=element_or_error(detect_error,end,&p,compteur_negation,chargroup);
        if(a!=-1){
          return a;
        }
        //on incrémente compteur_negation pour lire la suite
        //de la chaine et inverser s'il faut à la fin
        compteur_negation++;
        break;
      case '\\':
        //si pas d'erreur (voir commenatire element_or_error)
        a=element_or_error(detect_error,end,&p,compteur_negation,chargroup);
        if(a!=-1){
          return a;
        }
        //voir commenatire backslash_parse)
        backslash_parse(p,chargroup);
        p++;
        detect_error=ELEMENT;
        //detect_error passe à ELEMENT car on a lu un element correct
        break;
      case '[':
        //si pas d'erreur (voir commenatire element_or_error)
        a=element_or_error(detect_error,end,&p,compteur_negation,chargroup);
        if(a!=-1){
          return a;
        }
        detect_error=bracket_parse(&p,chargroup);
        if(detect_error==ERROR){
          //si on a détecté une erreur on return erreur
          return 0;
        }
        if(*p =='\0'){
          //si chaine vide après ça veut dire qu'on a tout lu et
          //que tout est bon
          *end=p;
          //on vérifie que nombre impaire de négation
          negation_or_not(compteur_negation,chargroup);
          //on return 1 qui signifie chaine correct
          return 1;
        }
        break;
      case '*':
        //on met la valeur de l'opérateur correspondant
        //voir operator_or_error pour le return
        chargroup->operateur=CHARGROUP_OPERATOR_ZERO_OR_MORE;
        return operator_or_error(detect_error,end,&p,compteur_negation,chargroup);
      case '+':
        //on met la valeur de l'opérateur correspondant
        //voir operator_or_error pour le return
        chargroup->operateur=CHARGROUP_OPERATOR_ONE_OR_MORE;
        return operator_or_error(detect_error,end,&p, compteur_negation,chargroup);
      case '?':
        //on met la valeur de l'opérateur correspondant
        //voir operator_or_error pour le return
        chargroup->operateur=CHARGROUP_OPERATOR_ZERO_OR_ONE;
        return operator_or_error(detect_error,end,&p, compteur_negation,chargroup);
      case '.':
        if(detect_error==ELEMENT){
          //si element avant comme '.' est tous les caractères
          //on s'arrête la première partie est correct
          *end=p; //on stocke dans end la suite de la regexp
          //on vérifie que nombre impaire de négation
          negation_or_not(compteur_negation,chargroup);
          //on return succès
          return 1;
        }
        //sinon on met tout les caractères à 1
        for(i=0;i<NBRCHAR;i++){
          chargroup->group[i]=1;
        }
        detect_error=ELEMENT;
        p++;
        // et on relance le switch cas tant que la chaine n'est pas
        // vide ou si le caractère est affecté d'un opérateur
        break;
      default:
        if (detect_error==ELEMENT){
          //si element avant
          //on s'arrête la première partie est correct
          *end=p;//on stocke dans end la suite de la regexp
          //on vérifie que nombre impaire de négation
          negation_or_not(compteur_negation,chargroup);
          return 1;
        }
        // ']' interdit pour nous car caractère doit être échappé
        if (*p==']'){
          return 0;
        }
        //sinon on incrémente le chargroup à l'endroit du carctère
        chargroup->group[(int)*p]=1;
        detect_error=ELEMENT;
        p++;
        // et on relance le switch cas tant que la chaine n'est pas
        // vide ou si le caractère est affecté d'un opérateur
        break;
    }
  }
  //on vérifie si nombre impaire de négation
  negation_or_not(compteur_negation,chargroup);
  //on met end sur '\0'
  *end=p;
  return 1;
}
list_t chargroup_parse(char * source){
  char *end=source;
  int mem=1;
  if (*source=='\0'){
    return list_new(); // si la source est vide on return liste vide
  }
  queue_t queue=queue_new();
  while(mem && *end !='\0'){
    //sinon tant qu'il n'y a pas d'erreur ou que l'on n'a pas
    //lu la chaine on exécute str_to_chargroup
    chargroup_t chargroup=initialisation();
    mem=str_to_chargroup(source, &end, chargroup);
    if (!mem){
      //si erreur on retourne liste vide
      if(queue){
        list_delete(queue_to_list(&queue),delete_chargroup);
      }
      delete_chargroup(chargroup);
      return list_new();
    }
    //sinon on ajoute à la queue la première portion de regexp valide
    queue=enqueue(queue,chargroup);
    source=end;
    //source devient le reste de la chaine qui reste à analyser
  }
  // si pas d'erreur et on arrive fin de chaine
  //on transforme la queue en liste et on retourne la liste qui correspond à la regexp parsé
  return queue_to_list(&queue);
}
