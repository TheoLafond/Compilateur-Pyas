CFLAGS = -Wall -Wextra -ggdb3 -DNDEBUG -Iinclude
LFLAGS = -ldl -lm

FUNC_TESTS_SRC = $(wildcard prog/*.c)
FUNC_TESTS_BIN = $(subst prog, bin, $(FUNC_TESTS_SRC:%.c=%.exe))

UNIT_TESTS_SRC = $(wildcard tests/unit/*.c)
UNIT_TESTS_BIN = $(subst tests/unit, bin/unit, $(UNIT_TESTS_SRC:%.c=%.exe))

PROG_TESTS_SRC = $(wildcard tests/prog_test/*.c)
PROG_TESTS_BIN = $(subst tests/prog_test, bin/prog_test, $(PROG_TESTS_SRC:%.c=%.exe))

SRC = $(wildcard src/*.c)
OBJ = $(SRC:%.c=%.o)
INCLUDES = $(wildcard include/*/*.h)

all : $(FUNC_TESTS_BIN) $(UNIT_TESTS_BIN) $(PROG_TESTS_BIN)

bin/%.exe : prog/%.o $(OBJ) $(INCLUDES)
	@mkdir -p bin
	$(CC) $(OBJ) $< $(LFLAGS) -o $@

bin/unit/%.exe : tests/unit/%.o $(OBJ) $(INCLUDES)
	@mkdir -p bin/unit
	$(CC) $(OBJ) $< $(LFLAGS) -o $@

bin/prog_test/%.exe : tests/prog_test/%.o $(OBJ) $(INCLUDES)
	@mkdir -p bin/prog_test
	$(CC) $(OBJ) $< $(LFLAGS) -o $@

clean :
	$(RM) $(OBJ)
	$(RM) $(FUNC_TESTS_BIN) $(UNIT_TESTS_BIN) $(PROG_TESTS_BIN)
	find . -name "*~" -delete
