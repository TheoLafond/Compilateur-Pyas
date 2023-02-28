#include <pyas/all.h>
#include <unitest/unitest.h>

#include <stdlib.h>
#include <stdio.h>
#include <string.h>


static void queue_basics( void ) {
  test_suite( "Basic test of list module, list of lexems (queue_new, queue_empty, enqueue, queue_to_list)" );

  queue_t q = queue_new();

  // test sur queue vide
  test_assert( q == NULL,
	       "An empty queue is NULL" );
  test_assert( queue_empty(q) == 1,
	       "An empty queue is NULL" );

  q = enqueue( q, lexem_new( "int", "42",0, 1, 8, -1) );


  // test liste à un élément
  test_assert( q != NULL,
	       "A non-empty queue should be not NULL" );
  test_assert( queue_empty(q) == 0,
	       "queue_empty() should be 1" );

  // ajout en tête d'un second élément
  q = enqueue( q, lexem_new( "str", "ficelle",0, 1, 0,-1 ) );

  // passage de queue a liste
  list_t l = list_new();
  l = queue_to_list(&q);

    // test de list_to_queue avec list_print
  test_assert( list_length(l) == 2,
	       "list_length() should be 2 after queue_to_list()" );
  test_oracle( list_print( l, lexem_print ), "( [1:8:int] 42 [1:0:str] ficelle )",
	       "List is OK after list_del_first()");

  // free memory
  list_delete( q, lexem_delete );
}


int main ( int argc, char *argv[] ) {

  unit_test( argc, argv );

  queue_basics();

  exit( EXIT_SUCCESS );
}
