#include <stdlib.h>
#include <stdio.h>

#include <pyas/chargroup.h>
#include <pyas/doclex.h>
#include <pyas/queue.h>

int main(int argc, char const *argv[]){
  if(argc<2){
    fprintf(stderr, "erreur\n");
    exit(EXIT_FAILURE);
  }
  doclex_t doclex;
  doclex = file_to_doclex(argv[1]);
  doclex_print(doclex);
  printf("\n");
  delete_doclex(doclex);

  return EXIT_SUCCESS;
}
