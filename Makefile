CC = gcc
OBJ_DIR = build/objects
GCH_DIR = build/pch
BIN_DIR = build/bin
COMMON_DIR = common
LINUX_COMMON_DIR = linux/common
TESTS_COMMON_DIR = tests/common
TESTS_LINUX_DIR = tests/linux
LINUX_CNF_DIR = linux/cnf
CFLAGS = -g -Wall -Werror -pedantic -O1 -fsanitize=address,undefined
test_dir_validation_gch = $(GCH_DIR)/files.h.gch $(GCH_DIR)/create.h.gch $(GCH_DIR)/function_entry.h.gch $(GCH_DIR)/my_assert.h.gch
test_dir_validation_objs = $(OBJ_DIR)/files.o $(OBJ_DIR)/create.o $(OBJ_DIR)/cnf.o $(OBJ_DIR)/my_assert.o $(OBJ_DIR)/test_dir_validation.o
$(GCH_DIR)/files.h.gch:
	$(CC) $(CFLAGS) $(COMMON_DIR)/include/files.h -o $(GCH_DIR)/files.h.gch
$(GCH_DIR)/create.h.gch:
	$(CC) $(CFLAGS) $(LINUX_COMMON_DIR)/include/create.h -o $(GCH_DIR)/create.h.gch
$(GCH_DIR)/function_entry.h.gch:
	$(CC) $(CFLAGS) $(TESTS_LINUX_DIR)/include/function_entry.h -o $(GCH_DIR)/function_entry.h.gch
$(GCH_DIR)/my_assert.h.gch:
	$(CC) $(CFLAGS) $(TESTS_LINUX_DIR)/include/my_assert.h -o $(GCH_DIR)/my_assert.h.gch
$(OBJ_DIR)/files.o:
	$(CC) $(CFLAGS) -c $(COMMON_DIR)/src/files.c -o $(OBJ_DIR)/files.o
$(OBJ_DIR)/create.o:
	$(CC) $(CFLAGS) -c $(LINUX_COMMON_DIR)/src/create.c -o $(OBJ_DIR)/create.o
$(OBJ_DIR)/cnf.o:
	$(CC) $(CFLAGS) -c $(LINUX_CNF_DIR)/src/cnf.c -o $(OBJ_DIR)/cnf.o
$(OBJ_DIR)/my_assert.o:
	$(CC) $(CFLAGS) -c $(TESTS_LINUX_DIR)/src/my_assert.c -o $(OBJ_DIR)/my_assert.o
$(OBJ_DIR)/test_dir_validation.o:
	$(CC) $(CFLAGS) -c $(TESTS_LINUX_DIR)/src/test_dir_validation.c -o $(OBJ_DIR)/test_dir_validation.o
test_dir_validation: $(test_dir_validation_gch) $(test_dir_validation_objs)
	$(CC) $(CFLAGS) -I $(GCH_DIR) $(test_dir_validation_objs) -o $(BIN_DIR)/./test_dir_validation
