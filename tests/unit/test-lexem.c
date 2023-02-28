#include <pyas/all.h>
#include <unitest/unitest.h>

#include <stdlib.h>
#include <stdio.h>
#include <string.h>


static void lexem_basics( void ) {
  test_suite( "Basic test of lexem module" );
  lexem_t lex = lexem_new("float","0.548",0,8,9,-1);

  test_oracle( lexem_print(lex), "[8:9:float] 0.548",
	       "Lexem print is ok");
  test_assert( strcmp(lexem_return_type(lex),"float")==0,
	       "Return type is ok" );
 test_assert( strcmp(lexem_return_value(lex),"0.548")==0,
	       "Return value is ok" );
  // free memory
  lexem_delete( lex );
}


int main ( int argc, char *argv[] ) {

  unit_test( argc, argv );

  lexem_basics();

  exit( EXIT_SUCCESS );
}
