#include <pyas/all.h>
#include <unitest/unitest.h>

#include <stdlib.h>
#include <stdio.h>
#include <string.h>


static void pyobj_basics( void ) {
  test_suite( "Basic test of pyobj module (pyobj_create, pyobj_print, pyobj_delete)" );

  // test pyobj list
  pyobj_t pyobj1 = pyobj_create(PYOBJ_LIST);
  test_oracle( pyobj_print(pyobj1), "\tBonjour, size = 8\n\tBonjour, size = 8\n\tBonjour, size = 8\n",
	       "A pyobj_list should be printed as '\tBonjour, size = 8\n\tBonjour, size = 8\n\tBonjour, size = 8'\n" );
  free(pyobj1);

  // test pyobj string
  pyobj_t pyobj2 = pyobj_create(PYOBJ_STRING);
  test_oracle( pyobj_print(pyobj2), "\tBonjour, size = 8\n",
	       "A pyobj_string should be printed as '\tBonjour, size = 8\n'" );
  pyobj_delete(pyobj2);

  // test pyobj number_int
  pyobj_t pyobj3 = pyobj_create(PYOBJ_NUMBER_INT);
  test_oracle( pyobj_print(pyobj3), "\t1\n",
	       "A pyobj_int should be printed as '\t1\n'" );
  pyobj_delete(pyobj3);

  // test pyobj number_real
  pyobj_t pyobj4 = pyobj_create(PYOBJ_NUMBER_REAL);
  test_oracle( pyobj_print(pyobj4), "\t1.618000\n",
	       "A pyobj_real should be printed as '\t1.618000\n'" );
  pyobj_delete(pyobj4);

  // test pyobj number_s_real
  pyobj_t pyobj5 = pyobj_create(PYOBJ_NUMBER_S_REAL);
  test_oracle( pyobj_print(pyobj5), "\t1.618\n",
	       "A pyobj_s_real should be printed as '\t1.618\n'" );
  pyobj_delete(pyobj5);

  //test pyobj number_complex
  pyobj_t pyobj6 = pyobj_create(PYOBJ_NUMBER_COMPLEX);
  test_oracle( pyobj_print(pyobj6), "\t2.000000+i3.000000\n",
	       "A pyobj_complex should be printed as '\t2.000000+i3.000000\n'" );
  pyobj_delete(pyobj6);

  // test pyobj number_s_complex
  pyobj_t pyobj7 = pyobj_create(PYOBJ_NUMBER_S_COMPLEX);
  test_oracle( pyobj_print(pyobj7), "\t4+i5\n",
	       "A pyobj_s_complex should be printed as '\t4+i5\n'" );
  pyobj_delete(pyobj7);

  pyobj_t pyobj8 = pyobj_create(PYOBJ_NULL);
  test_oracle( pyobj_print(pyobj8), "Null\n",
	       "A pyobj_s_complex should be printed as 'Null'" );
  pyobj_delete(pyobj8);


  pyobj_t pyobj9 = pyobj_create(PYOBJ_FALSE);
  test_oracle( pyobj_print(pyobj9), "False\n",
	       "A pyobj_s_complex should be printed as 'False'" );
  pyobj_delete(pyobj9);

  pyobj_t pyobj10 = pyobj_create(PYOBJ_NONE);
  test_oracle( pyobj_print(pyobj10), "None\n",
	       "A pyobj_s_complex should be printed as 'None'" );
  pyobj_delete(pyobj10);

  pyobj_t pyobj11 = pyobj_create(PYOBJ_TRUE);
  test_oracle( pyobj_print(pyobj11), "True\n",
	       "A pyobj_s_complex should be printed as 'True'" );
  pyobj_delete(pyobj11);
}


int main ( int argc, char *argv[] ) {

  unit_test( argc, argv );

  pyobj_basics();

  exit( EXIT_SUCCESS );
}
