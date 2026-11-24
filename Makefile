# This Makefile is essentially use for tests
CC = gcc
OBJ_DIR = build/test/objects
GCH_DIR = build/test/pch
BIN_DIR = build/test/bin
COMMON_DIR = common
LINUX_COMMON_DIR = linux/common
TESTS_COMMON_DIR = tests/common
TESTS_LINUX_DIR = tests/linux
LINUX_CNF_DIR = linux/cnf
LINUX_CDF_DIR = linux/cdf
CFLAGS = -g -Wall -Werror -pedantic -O1 -fsanitize=address,undefined
$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)
$(GCH_DIR):
	mkdir -p $(GCH_DIR)
$(BIN_DIR):
	mkdir -p $(BIN_DIR)
# For Object-files
# CFLAGS_RELEASE = -O2 -flto -DNDEBUG -fstack-protector-strong -D_FORTIFY_SOURCE=2 -fPIE
# For Executable-files
# LDFLAGS_RELEASE = -O2 -flto -pie -Wl,-z,relro,-z,now -s
$(GCH_DIR)/files.h.gch: $(COMMON_DIR)/include/files.h | $(GCH_DIR)
	$(CC) $(CFLAGS) $(COMMON_DIR)/include/files.h -o $(GCH_DIR)/files.h.gch
$(GCH_DIR)/create.h.gch: $(LINUX_COMMON_DIR)/include/create.h | $(GCH_DIR)
	$(CC) $(CFLAGS) $(LINUX_COMMON_DIR)/include/create.h -o $(GCH_DIR)/create.h.gch
$(GCH_DIR)/function_entry.h.gch: $(TESTS_LINUX_DIR)/include/function_entry.h | $(GCH_DIR)
	$(CC) $(CFLAGS) $(TESTS_LINUX_DIR)/include/function_entry.h -o $(GCH_DIR)/function_entry.h.gch
$(GCH_DIR)/my_assert.h.gch: $(TESTS_COMMON_DIR)/include/my_assert.h | $(GCH_DIR)
	$(CC) $(CFLAGS) $(TESTS_LINUX_DIR)/include/my_assert.h -o $(GCH_DIR)/my_assert.h.gch
$(OBJ_DIR)/files.o: $(COMMON_DIR)/src/files.c | $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $(COMMON_DIR)/src/files.c -o $(OBJ_DIR)/files.o
$(OBJ_DIR)/create.o: $(LINUX_COMMON_DIR)/src/create.c | $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $(LINUX_COMMON_DIR)/src/create.c -o $(OBJ_DIR)/create.o
$(OBJ_DIR)/create_cdf.o: $(LINUX_COMMON_DIR)/src/create.c | $(OBJ_DIR)
	$(CC) $(CFLAGS) -DCDF -c $(LINUX_COMMON_DIR)/src/create.c -o $(OBJ_DIR)/create_cdf.o
$(OBJ_DIR)/cnf.o: $(LINUX_CNF_DIR)/src/cnf.c | $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $(LINUX_CNF_DIR)/src/cnf.c -o $(OBJ_DIR)/cnf.o
$(OBJ_DIR)/cdf.o: $(LINUX_CDF_DIR)/src/cdf.c | $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $(LINUX_CDF_DIR)/src/cdf.c -o $(OBJ_DIR)/cdf.o
$(OBJ_DIR)/my_assert.o: $(TESTS_LINUX_DIR)/src/my_assert.c | $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $(TESTS_LINUX_DIR)/src/my_assert.c -o $(OBJ_DIR)/my_assert.o
$(OBJ_DIR)/test_validate_dirpath.o: $(TESTS_LINUX_DIR)/src/test_validate_dirpath.c | $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $(TESTS_LINUX_DIR)/src/test_validate_dirpath.c -o $(OBJ_DIR)/test_validate_dirpath.o
$(OBJ_DIR)/test_validate_filename.o: $(TESTS_LINUX_DIR)/src/test_validate_filename.c
	$(CC) $(CFLAGS) -c $(TESTS_LINUX_DIR)/src/test_validate_filename.c -o $(OBJ_DIR)/test_validate_filename.o
linux_cnf_gch = $(GCH_DIR)/files.h.gch $(GCH_DIR)/create.h.gch
linux_cnf_obj =	$(OBJ_DIR)/files.o $(OBJ_DIR)/create.o $(OBJ_DIR)/cnf.o
linux_cdf_gch = $(GCH_DIR)/files.h.gch $(GCH_DIR)/create.h.gch
linux_cdf_obj = $(OBJ_DIR)/files.o $(OBJ_DIR)/create_cdf.o $(OBJ_DIR)/cdf.o
linux_test_validate_filename_gch = $(GCH_DIR)/files.h.gch $(GCH_DIR)/create.h.gch $(GCH_DIR)/my_assert.h.gch $(GCH_DIR)/function_entry.h.gch
linux_test_validate_filename_obj = $(OBJ_DIR)/files.o $(OBJ_DIR)/create.o $(OBJ_DIR)/test_validate_filename.o $(OBJ_DIR)/my_assert.o
test_validate_dirpath_gch = $(GCH_DIR)/files.h.gch $(GCH_DIR)/create.h.gch $(GCH_DIR)/function_entry.h.gch $(GCH_DIR)/my_assert.h.gch
test_validate_dirpath_objs = $(OBJ_DIR)/files.o $(OBJ_DIR)/create.o $(OBJ_DIR)/my_assert.o $(OBJ_DIR)/test_validate_dirpath.o
linux_cdf: $(linux_cdf_gch) $(linux_cdf_obj) | $(BIN_DIR)
	$(CC) $(CFLAGS) -I $(GCH_DIR) $(linux_cdf_obj) -o $(BIN_DIR)/linux_cdf
linux_cnf: $(linux_cnf_gch) $(linux_cnf_obj) | $(BIN_DIR)
	$(CC) $(CFLAGS) -I $(GCH_DIR) $(linux_cnf_obj) -o $(BIN_DIR)/linux_cnf
linux_test_validate_dirpath: $(test_validate_dirpath_gch) $(test_validate_dirpath_objs) | $(BIN_DIR)
	$(CC) $(CFLAGS) -I $(GCH_DIR) $(test_validate_dirpath_objs) -o $(BIN_DIR)/linux_test_validate_dirpath
linux_test_validate_filename: $(linux_test_validate_filename_gch) $(linux_test_validate_filename_obj)
	$(CC) $(CFLAGS) -I $(GCH_DIR) $(linux_test_validate_filename_obj) -o $(BIN_DIR)/linux_test_validate_filename
.PHONY: clean
clean:
	rm -rf build

