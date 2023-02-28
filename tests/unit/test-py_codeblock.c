#include <pyas/all.h>
#include <unitest/unitest.h>

#include <stdlib.h>
#include <stdio.h>
#include <string.h>


static void pyobj_basics( void ) {
  test_suite( "Basic test of py_codeblock module (pyobj_create, pyobj_print, pyobj_delete)" );

  py_cb_t codeblock = py_cb_create();

  test_assert((sizeof(codeblock) == 8), "A codeblock should be 8 bytes");
  test_oracle(py_cb_print(codeblock), "", "A new  codeblock should be print as ''");
  py_cb_delete(codeblock);
}

static void py_cb_id( void ) {
  test_suite( "Test of py_codeblock str_to_id function" );
  test_assert(py_cb_str_to_id("version_pyvm")==VERSION_PYVM, "Should be 'version_pyvm'");
  test_assert(py_cb_str_to_id("flags")==FLAGS, "Should be 'flags'");
  test_assert(py_cb_str_to_id("filename")==FILENAME, "Should be 'filename'");
  test_assert(py_cb_str_to_id("name")==NAME, "Should be 'name'");
  test_assert(py_cb_str_to_id("stack_size")==STACK_SIZE, "Should be 'stack_size'");
  test_assert(py_cb_str_to_id("arg_count")==ARG_COUNT, "Should be 'arg_count'");
  test_assert(py_cb_str_to_id("posonly_arg_count")==ARG_COUNT, "Should be 'posonly_arg_count'");
  test_assert(py_cb_str_to_id("kwonly_arg_count")==ARG_COUNT, "Should be 'kwonly_arg_count'");
  test_assert(py_cb_str_to_id("interned-strings")==INTERNED, "Should be 'interned'");
  test_assert(py_cb_str_to_id("constants")==CONSTS, "Should be 'consts'");
  test_assert(py_cb_str_to_id("names")==NAMES, "Should be 'names'");
  test_assert(py_cb_str_to_id("varnames")==VARNAMES, "Should be 'varnames'");
  test_assert(py_cb_str_to_id("freevars")==FREEVARS, "Should be 'freevars'");
  test_assert(py_cb_str_to_id("cellvars")==CELLVARS, "Should be 'cellvars'");
  test_assert(py_cb_str_to_id("code")==BYTECODE, "Should be 'code'");

}

static void py_cb_add_function( void ) {
  test_suite( "Test of py_codeblock add functions" );

  lexem_t lexem = lexem_new("string", "'bonjour'",0, 1, 1,-1);
  pyobj_t pyobj = pyobj_lexem_to_pyobj_val(lexem);
  py_cb_t codeblock = py_cb_create();

  py_cb_add_version_pyvm(123, codeblock);
  test_oracle(py_cb_print(codeblock), "Version pyvm : \t\t\t123\n",
              "Should be print as 'Version pyvm : \t\t\t123'");
  py_cb_reset(codeblock);

  py_cb_add_arg_count(234, codeblock);
  test_oracle(py_cb_print(codeblock), "Arg count : \t\t\t234\n",
              "Should be print as 'Arg count : \t\t\t234'");
  py_cb_reset(codeblock);

  py_cb_add_local_count(2, codeblock);
  test_oracle(py_cb_print(codeblock), "Local count : \t\t\t2\n",
              "Should be print as 'Local count : \t\t\t2'");
  py_cb_reset(codeblock);

  py_cb_add_stack_size(12, codeblock);
  test_oracle(py_cb_print(codeblock), "Stack size : \t\t\t12\n",
              "Should be print as 'Version pyvm : \t\t\t12'");
  py_cb_reset(codeblock);

  py_cb_add_flags(46, codeblock);
  test_oracle(py_cb_print(codeblock), "Flags : \t\t\t0x2e\n",
              "Should be print as 'Flags : \t\t\t0x40'");
  py_cb_reset(codeblock);

  py_cb_add_parent(pyobj, codeblock);
  test_oracle(py_cb_print(codeblock), "<Parent>\n\tbonjour, size = 7\n</Parent>\n",
              "Should be print as '<Parent>\n\tbonjour, size = 7</Parent>'");
  py_cb_reset(codeblock);

  py_cb_add_magic(1234, codeblock);
  test_oracle(py_cb_print(codeblock), "Magic : \t\t\t1234\n",
              "Should be print as 'Magic : \t\t\t1234'");
  py_cb_reset(codeblock);

  py_cb_add_timestamp(13, codeblock);
  test_oracle(py_cb_print(codeblock), "Timestamp : \t\t\t13\n",
              "Should be print as 'Timestamp : \t\t\t13'");
  py_cb_reset(codeblock);

  py_cb_add_source_size(1, codeblock);
  test_oracle(py_cb_print(codeblock), "Source size : \t\t\t1\n",
              "Should be print as 'Source size : \t\t\t1'");
  py_cb_reset(codeblock);

  pyobj = pyobj_lexem_to_pyobj_val(lexem);
  py_cb_add_interned(pyobj, codeblock);
  test_oracle(py_cb_print(codeblock), "<Interned>\n\tbonjour, size = 7\n</Interned>\n",
              "Should be print as '<Interned>\n\tbonjour, size = 7</Interned>'");
  py_cb_reset(codeblock);

  pyobj = pyobj_lexem_to_pyobj_val(lexem);
  py_cb_add_bytecode(pyobj, codeblock);
  test_oracle(py_cb_print(codeblock), "<Bytecode>\n\tbonjour, size = 7\n</Bytecode>\n",
              "Should be print as '<Bytecode>\n\tbonjour, size = 7</Bytecode>'");
  py_cb_reset(codeblock);

  pyobj = pyobj_lexem_to_pyobj_val(lexem);
  py_cb_add_consts(pyobj, codeblock);
  test_oracle(py_cb_print(codeblock), "<Consts>\n\tbonjour, size = 7\n</Consts>\n",
              "Should be print as '<Consts>\n\tbonjour, size = 7</Consts>'");
  py_cb_reset(codeblock);

  pyobj = pyobj_lexem_to_pyobj_val(lexem);
  py_cb_add_names(pyobj, codeblock);
  test_oracle(py_cb_print(codeblock), "<Names>\n\tbonjour, size = 7\n</Names>\n",
              "Should be print as '<Names>\n\tbonjour, size = 7</Names>'");
  py_cb_reset(codeblock);

  pyobj = pyobj_lexem_to_pyobj_val(lexem);
  py_cb_add_varnames(pyobj, codeblock);
  test_oracle(py_cb_print(codeblock), "<Varnames>\n\tbonjour, size = 7\n</Varnames>\n",
              "Should be print as '<Varnames>\n\tbonjour, size = 7</Varnames>'");
  py_cb_reset(codeblock);

  pyobj = pyobj_lexem_to_pyobj_val(lexem);
  py_cb_add_freevars(pyobj, codeblock);
  test_oracle(py_cb_print(codeblock), "<Freevars>\n\tbonjour, size = 7\n</Freevars>\n",
              "Should be print as '<Freevars>\n\tbonjour, size = 7</Freevars>'");
  py_cb_reset(codeblock);

  pyobj = pyobj_lexem_to_pyobj_val(lexem);
  py_cb_add_cellvars(pyobj, codeblock);
  test_oracle(py_cb_print(codeblock), "<Cellvars>\n\tbonjour, size = 7\n</Cellvars>\n",
              "Should be print as '<Cellvars>\n\tbonjour, size = 7</Cellvars>'");
  py_cb_reset(codeblock);

  pyobj = pyobj_lexem_to_pyobj_val(lexem);
  py_cb_add_filename(pyobj, codeblock);
  test_oracle(py_cb_print(codeblock), "<Filename>\n\tbonjour, size = 7\n</Filename>\n",
              "Should be print as '<Filename>\n\tbonjour, size = 7</Filename>'");
  py_cb_reset(codeblock);

  pyobj = pyobj_lexem_to_pyobj_val(lexem);
  py_cb_add_name(pyobj, codeblock);
  test_oracle(py_cb_print(codeblock), "<Name>\n\tbonjour, size = 7\n</Name>\n",
              "Should be print as '<Name>\n\tbonjour, size = 7</Name>'");
  py_cb_reset(codeblock);

  py_cb_add_firstlinelo(123456789, codeblock);
  test_oracle(py_cb_print(codeblock), "Firstlinelo : \t\t\t123456789\n",
              "Should be print as 'Firstlinelo : \t\t\t123456789'");
  py_cb_reset(codeblock);

  pyobj = pyobj_lexem_to_pyobj_val(lexem);
  py_cb_add_lnotab(pyobj, codeblock);
  test_oracle(py_cb_print(codeblock), "<Lnotab>\n\tbonjour, size = 7\n</Lnotab>\n",
              "Should be print as '<Lnotab>\n\tbonjour, size = 7</Lnotab>'");
  py_cb_reset(codeblock);

  lexem_delete(lexem);
  py_cb_delete(codeblock);
}


int main ( int argc, char *argv[] ) {

  unit_test( argc, argv );

  pyobj_basics();
  py_cb_id();
  py_cb_add_function();

  exit( EXIT_SUCCESS );
}
