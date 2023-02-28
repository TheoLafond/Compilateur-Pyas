#include <pyas/all.h>
#include <pyas/doclex.h>
#include <unitest/unitest.h>

#include <stdlib.h>
#include <stdio.h>
#include <string.h>


void parsing_doclex(void){
  test_suite("testing doclex parsing");
  test_oracle(file_to_doclex_testing("definitionLexeme.txt"),"key :blank;regexp :[ \\t]+//key :newline;regexp :\\n+//key :comment;regexp :\\#^\\n*//key :paren::left;regexp :(//key :paren::right;regexp :)//key :brack::left;regexp :\\[//key :brack::right;regexp :\\]//key :colon;regexp :://key :semicolon;regexp :;//key :dir::consts;regexp :\\.consts//key :integer::dec;regexp :[+\\-]?[1-9][0-9]*//key :integer::hex;regexp :0x[a-b0-9A-Z]*//key :string;regexp :\"^[\\n\"]*\"//key :Pycst::None;regexp :None//key :Pycst::True;regexp :True//key :Pycst::False;regexp :False//key :dir::set;regexp :\\.set//key :dir::interned;regexp :\\.interned//key :dir::names;regexp :\\.names//key :dir::text;regexp :\\.text//key :dir::line;regexp :\\.line//key :lab::aj::jitop;regexp :JUMP_IF_TRUE_OR_POP//key :lab::aj::jifop;regexp :JUMP_IF_FALSE_OR_POP//key :lab::aj::ja;regexp :JUMP_ABSOLUTE//key :lab::aj::pjit;regexp :POP_JUMP_IF_TRUE//key :lab::aj::pjif;regexp :POP_JUMP_IF_FALSE//key :lab::aj::cl;regexp :CONTINUE_LOOP//key :lab::rj::jf;regexp :JUMP_FORWARD//key :lab::rj::fi;regexp :FOR_ITER//key :lab::rj::sl;regexp :SETUP_LOOP//key :lab::rj::se;regexp :SETUP_EXCPET//key :lab::rj::sf;regexp :SETUP_FINALLY//key :lab::rj::sw;regexp :SETUP_WITH//key :lab::rj::cf;regexp :CALL_FINALLY//key :symbol;regexp :[a-zA-Z_][a-zA-Z0-9_]*//key :float;regexp :[0-9]*\\.?[0-9]*[eE][+\\-]?[0-9]*//key :float;regexp :[0-9]*\\.*[0-9]*//","works");

}

int main(int argc, char const *argv[]) {
  unit_test(argc,argv);

  parsing_doclex();

  exit(EXIT_SUCCESS);
}
