/**
 * @file regexp.c
 * @author François Cayre <cayre@uvolante.org>
 * @date Sun Aug  7 21:22:53 2022
 * @brief Unit tests for regexps.
 *
 * Unit tests for regexps.
 */

#include <pyas/regexp.h>
#include <unitest/unitest.h>

static void re_interface( void ) { 
  char      *end; 

  test_suite( "Interface robustness" ); 

  test_assert(  re_match( NULL, "", &end ),
		"NULL regexp always matches" ); 
  test_assert(  re_match( "", "", NULL ),
		"NULL end is OK" ); 
  test_assert( !re_match( "", NULL, &end ),
	       "NULL source never matches" ); 
  test_assert( !re_match( NULL, NULL, NULL ),
	       "NULL everywhere means NULL source" ); 
}

static void re_basics( void ) { 
  char      *end; 

  test_suite( "Basic matches" ); 

  test_assert( re_match( "coucou", "coucou", NULL ), 
	       "Can match a constant string" ); 
  test_assert( re_match( "coucou", "coucou, allons manger des gâteaux", &end ), 
	       "Can match a string prefix" ); 
  test_oracle( printf( "%s", end ), ", allons manger des gâteaux", 
	       "And suffix is OK" );   
  test_assert( !re_match( "coucou", "cou", NULL ), 
	       "A regexp may be longer than the source" ); 
}

static void re_op_dot( void ) { 
  char      *end; 

  test_suite( "'any character' operator matches" ); 

  test_assert( !re_match( ".", "", NULL ), 
	       "Must have at least char to match" ); 
  test_assert( re_match( ".", "z", NULL ) && re_match( ".", "a", NULL ), 
	       "Any character matches" ); 

  test_assert( re_match( "z.p", "zipend", &end ),
	       "Can match 'any character' anywhere" ); 
  test_oracle( printf( "%s", end ), "end",
	       "And suffix is OK" ); 
}

static void re_op_star( void ) { 
  char      *end; 

  test_suite( "star operator matches" ); 

  test_assert( re_match( "a*", NULL, NULL ), 
	       "Can match NULL source with star" );
  test_assert( re_match( "a*", "", NULL ), 
	       "Can match empty source with star" );
  test_assert( re_match( "a*", "baaaaa", NULL ), 
	       "Star matches wrong prefix" );
  test_assert( re_match( "a*", "a", NULL ), 
	       "Can match one character with star" );
  test_assert( re_match( "a*", "aaaaaaabb", &end ), 
	       "Can match several characters with star" );
  test_oracle( printf( "%s", end ), "bb",
	       "And suffix is OK" ); 
}

static void re_mixed( void ) { 
  char      *end; 

  test_suite( "A little bit of everything" ); 

  test_assert( re_match( "z.a*b", "zzbend", &end ), 
	       "Test 1 matches" ); 
  test_oracle( printf( "%s", end ), "end",
	       "Test 1 match is OK" ); 
}

int main ( int argc, char *argv[] ) { 

  unit_test( argc, argv );

  re_interface();
  re_basics();
  re_op_dot();
  re_op_star();
  re_mixed(); 
  
  exit( EXIT_SUCCESS ); 
}
