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
INC = include
SRC = src
COMMON_DIR_INC = $(COMMON_DIR)/$(INC)
COMMON_DIR_SRC = $(COMMON_DIR)/$(SRC)
LINUX_COMMON_DIR_INC = $(LINUX_COMMON_DIR)/$(INC)
LINUX_COMMON_DIR_SRC = $(LINUX_COMMON_DIR)/$(SRC)
LINUX_CNF_DIR_SRC = $(LINUX_CNF_DIR)/$(SRC)
LINUX_CDF_DIR_SRC = $(LINUX_CDF_DIR)/$(SRC)
TESTS_LINUX_DIR_INC = $(TESTS_LINUX_DIR)/$(INC)
TESTS_LINUX_DIR_SRC = $(TESTS_LINUX_DIR)/$(SRC)
CFLAGS = -g -Wall -Werror -pedantic -O1 -fsanitize=address,undefined
LDFLAGS =  -fsanitize=address,undefined
$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)
$(GCH_DIR):
	mkdir -p $(GCH_DIR)
$(BIN_DIR):
	mkdir -p $(BIN_DIR)
$(GCH_DIR)/%.h.gch:	$(COMMON_DIR_INC)/%.h | $(GCH_DIR)
	$(CC) $(CFLAGS) -x c-header -c $< -o $@
$(GCH_DIR)/%.h.gch:	$(LINUX_COMMON_DIR_INC)/%.h | $(GCH_DIR)
	$(CC) $(CFLAGS) -x c-header -c $< -o $@
$(GCH_DIR)/%.h.gch: $(TESTS_LINUX_DIR_INC)/%.h | $(GCH_DIR)
	$(CC) $(CFLAGS) -x c-header -c $< -o $@
$(OBJ_DIR)/%.o: $(COMMON_DIR_SRC)/%.c | $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@
$(OBJ_DIR)/%.o: $(LINUX_COMMON_DIR_SRC)/%.c | $(OBJ_DIR)
	$(CC) $(CFLAGS) -c  $< -o $@
$(OBJ_DIR)/%.o: $(LINUX_COMMON_DIR_SRC)/%.c | $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@
$(OBJ_DIR)/%.o: $(LINUX_CNF_DIR_SRC)/%.c | $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@
$(OBJ_DIR)/%.o: $(LINUX_CDF_DIR_SRC)/%.c | $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@
$(OBJ_DIR)/%.o: $(TESTS_LINUX_DIR_SRC)/%.c | $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@
$(OBJ_DIR)/create_cdf.o: $(LINUX_COMMON_DIR_SRC)/create.c | $(OBJ_DIR)
	$(CC) -DCDF $(CFLAGS) -c $(LINUX_COMMON_DIR_SRC)/create.c -o $(OBJ_DIR)/create_cdf.o
ALWAYS_OBJ = $(OBJ_DIR)/files.o $(OBJ_DIR)/create.o
ALWAYS_CDF_OBJ = $(OBJ_DIR)/files.o  $(OBJ_DIR)/create_cdf.o
ALWAYS_TEST_OBJ = $(OBJ_DIR)/my_assert.o
linux_cnf_obj =	$(ALWAYS_OBJ) $(OBJ_DIR)/cnf.o
linux_cdf_obj = $(ALWAYS_CDF_OBJ) $(OBJ_DIR)/cdf.o
linux_test_validate_filename_obj = $(ALWAYS_OBJ) $(ALWAYS_TEST_OBJ) $(OBJ_DIR)/test_validate_filename.o
test_validate_dirpath_objs = $(ALWAYS_OBJ) $(ALWAYS_TEST_OBJ) $(OBJ_DIR)/test_validate_dirpath.o
linux_cdf: $(linux_cdf_obj) | $(BIN_DIR)
	$(CC) $(LDFLAGS) -I $(GCH_DIR) $^ -o $@
linux_cnf: $(linux_cnf_gch) $(linux_cnf_obj) | $(BIN_DIR)
	$(CC) $(LDFLAGS) -I $(GCH_DIR) $^ -o $@
linux_test_validate_dirpath: $(test_validate_dirpath_objs) | $(BIN_DIR)
	$(CC) $(LDFLAGS) -I $(GCH_DIR) $^ -o $@
linux_test_validate_filename: $(linux_test_validate_filename_obj) | $(BIN_DIR)
	$(CC) $(LDFLAGS) -I $(GCH_DIR) $^ -o $@

# For Object-files
# CFLAGS_RELEASE = -O2 -flto -DNDEBUG -fstack-protector-strong -D_FORTIFY_SOURCE=2 -fPIE
# For Executable-files
# LDFLAGS_RELEASE = -O2 -flto -pie -Wl,-z,relro,-z,now -s
.PHONY: clean
clean:
	rm -rf build

