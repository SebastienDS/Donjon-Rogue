.PHONY: all run clean create_out test-leaks test-full-leaks doc

SRC_DIR = src
HEADERS_DIR = include
OUT_DIR = bin
OBJ_DIR = ${OUT_DIR}/obj

EXE = ${OUT_DIR}/main
ARGS = 

CFLAGS = -I ${HEADERS_DIR} -ansi -Wall -Wfatal-errors -g -lMLV

SRC = $(shell find ${SRC_DIR} -name \*.c)
OBJ = $(patsubst ${SRC_DIR}/%.c, ${OBJ_DIR}/%.o, ${SRC})
INC = $(shell find ${HEADERS_DIR} -name \*.h)


all: create_out ${EXE}

run: all
	./${EXE} ${ARGS}

${EXE}: ${OBJ}
	gcc $^ -o $@ ${CFLAGS}

${OBJ_DIR}/%.o: ${SRC_DIR}/%.c ${INC}
	gcc -c $< -o $@ ${CFLAGS}

clean: 
	@rm -rf ${OUT_DIR}
	@rm -rf html

create_out:
	@mkdir -p ${OUT_DIR}
	@mkdir -p $(shell dirname ${OBJ})

test-leaks: all
	valgrind ./${EXE} ${ARGS} 2>${OUT_DIR}/leaks.log

test-full-leaks: all
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes ./${EXE} ${ARGS} 2>${OUT_DIR}/leaks.log

doc:
	doxygen .config/doxygen