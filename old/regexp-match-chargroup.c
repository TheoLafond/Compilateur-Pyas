#include <stdlib.h>
#include <stdio.h>

#include <pyas/regexp.h>
#include <pyas/chargroup.h>

int main ( int argc, char *argv[] ) {

  if ( argc < 3 ) {
    fprintf( stderr, "Usage :\n\t%s regexp text\n", argv[ 0 ] );
    exit( EXIT_FAILURE );
  }
  int compteur=0;
  while('\0'!=argv[1][compteur]){
    compteur++;
  }
  char * end = argv[ 2 ];
  chargroup_t chargroup = str_to_chargroup(argv[1], compteur);

  print_chargroup(chargroup);

  int is_match = re_match( chargroup, argv[2], &end );

  if ( is_match ) {
    printf( "The start of '%s' is %s, %s: '%s'.\n", argv[2], argv[ 1 ], *end ? "next" : "END", end );
  }
  else {
    printf( "The start of '%s' is *NOT* %s.\n", argv[2], argv[ 1 ] );
  }

  exit( EXIT_SUCCESS );
}
