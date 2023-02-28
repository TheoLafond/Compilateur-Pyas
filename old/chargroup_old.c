#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>

#include <pyas/chargroup.h>

chargroup_t initialisation(){
  chargroup_t chargroup;
  chargroup=calloc(1,sizeof(*chargroup));
  chargroup->group=calloc(NBRCHAR, sizeof(char));
  assert(chargroup->group!=NULL);
  chargroup->operateur=OP_RIEN;
  chargroup->taille=0;
  return chargroup;
}

void delete_chargroup(chargroup_t chargroup){
  free(chargroup->group);
  chargroup->operateur=OP_RIEN;
  chargroup->taille=0;
}

void print_chargroup(chargroup_t chargroup){
  int i;
  printf("%s\n",operator_name[chargroup->operateur]);
  for(i=0;i<NBRCHAR;i++){
    printf("%d ",chargroup->group[i]);
  }
  printf("\n");
}
chargroup_t gere_les_backslashs(char* source, chargroup_t chargroup){
  //TODO
  chargroup->operateur=OP_RIEN;
  chargroup->group[*source]=1;
  chargroup->taille++;
  return chargroup;
}
chargroup_t gere_les_crochets(char* source, int* a, chargroup_t chargroup){
  //TODO
  int i;
  chargroup->operateur=OP_RIEN;
  while(*source!=CROCHET_FERMANT){
    if(*source==TIRET){
      for(i=*(source-1)+1;i<=*(source+1);i++){
        chargroup->group[i]=1;
      }
      chargroup->taille=chargroup->taille+2;
      *a=*a+2;
      source=source+2;
    }
    else{
      if (*source == BACKSLASH){
        //printf("coucou\n");
        source++;
        chargroup->taille++;
        //printf("source: %s taille chargroup %d\n",source, chargroup->taille);
        chargroup=gere_les_backslashs(source,chargroup);
        source++;
        *a=*a+2;
        //printf("source: %s taille chargroup %d\n",source, chargroup->taille);
      }
      else{
        chargroup->group[*source]=1;
        chargroup->taille++;
        *a =*a +1;
        source++;
      }
    }
  }
    // printf("source: %s %d\n",source,chargroup->taille);
  source++;
  // printf("source: %s %d\n",source,chargroup->taille);
  chargroup->taille++;
  *a=*a+1;
  printf("%d\n",*a);
  return chargroup;
}

chargroup_t str_to_chargroup(char * source, int size){
  //TODO
  char* p=source;
  chargroup_t chargroup=initialisation();
  int mem=0;
  // printf("taille: %d\n",size);
  while(chargroup->taille<size){
    // printf("taille chargroup %d\n",chargroup->taille);
    int mem2=0;
    switch (*p) {
      case CIRCONFLEXE:
        mem++;
        p++;
        chargroup->taille++;
        break;
      case BACKSLASH:
        p++;
        chargroup->taille++;
        chargroup=gere_les_backslashs(p,chargroup);
        p++;
        break;
      case CROCHET_OUVRANT:
        p++;
        chargroup->taille++;
        chargroup=gere_les_crochets(p,&mem2,chargroup);
        p=p+mem2;
        break;
      case ASTERISQUE:
        chargroup->operateur=OP_ASTERISQUE;
        p++;
        chargroup->taille++;
        break;
      case PLUS:
        chargroup->operateur=OP_PLUS;
        p++;
        chargroup->taille++;
        break;
      case INTERRO:
        chargroup->operateur=OP_INTERRO;
        p++;
        chargroup->taille++;
        break;
      case POINT:
        for(int i=0;i<NBRCHAR;i++){
          chargroup->group[i]=1;
        }
        p++;
        chargroup->taille++;
        break;
      default:
        chargroup->group[*p]=1;
        p++;
        chargroup->taille++;
        break;
    }
  }
  // printf("taille chargroup %d \n", chargroup->taille);
  if (mem){
    int i;
    for(i=0;i<NBRCHAR;i++){
      if(chargroup->group[i]==1){
        chargroup->group[i]=0;
      }
      else{
        chargroup->group[i]=1;
      }
    }
  }
  mem=0;
  return chargroup;
}
