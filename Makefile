# This Makefile is essentially use for tests
CC = gcc
COMMON_DIR_INC = common/include
COMMON_DIR_SRC = common/src
LINUX_COMMON_DIR_INC = linux/common/include
LINUX_COMMON_DIR_SRC = linux/common/src
LINUX_CDF_DIR_SRC = linux/cdf/src
LINUX_CDF_DIR_SRC = linux/cnf/src
COMON_DIR = common
LINUX_COMMON_DIR = linux/common
LINUX_CDF_DIR = linux/cdf
LINUX_CNF_DIR = linux/cnf
TESTS_LINUX_DIR = tests
TESTS_LINUX_DIR_INC = tests/linux/include
TESTS_LINUX_DIR_SRC = tests/linux/src
OBJ_DIR = build/tests/objects
PCH_DIR = build/tests/pch
BIN_DIR = build/tests/bin
CFLAGS = -g -Wall -Werror -pedantic -O1 -fsanitize=address,undefined
CPPFLAGS = -MMD -MP
LDFLAGS =  -fsanitize=address,undefined
dirs = $(COMON_DIR) $(LINUX_COMMON_DIR) $(LINUX_CDF_DIR) $(LINUX_CNF_DIR) $(TESTS_LINUX_DIR)
OBJ_SUBDIRS = $(addprefix $(OBJ_DIR)/, $(dirs))
PCH_SUBDIRS = $(addprefix $(PCH_DIR)/, $(dirs))
BIN_SUBDIRS = $(addprefix $(BIN_DIR)/, $(dirs))
INCS = $(addprefix /include/, $(dirs))
SRCS = $(addprefix /src/, $(dirs))
define PCH_RULE
$(PCH_DIR)/$(1)/%.h.gch: $(1)/include/%.h | $(PCH_DIR)/$(1)
	$(CC) $(CFLAGS) -x c-header -c $$< -o $$@ 
endef
define OBJ_RULE
$(OBJ_DIR)/$(1)/%.o: $(1)/src/%.c | $(OBJ_DIR)/$(1)
	$(CC) $(CFLAGS) $(CPPFLAGS) -c $$< -o $$@ 
endef
$(BIN_SUBDIRS) $(PCH_SUBDIRS) $(OBJ_SUBDIRS):
	mkdir -p $@
$(foreach dir,$(dirs),$(eval $(call PCH_RULE,$(dir))))
$(foreach dir,$(dirs),$(eval $(call OBJ_RULE,$(dir))))
$(OBJ_DIR)/$(LINUX_COMMON_DIR)/create_cdf.o: $(LINUX_COMMON_DIR_SRC)/create.c | $(OBJ_DIR)/$(LINUX_COMMON_DIR)
	$(CC) -DCDF $(CFLAGS) -c $^ -o $@
ALWAYS_OBJ = $(OBJ_DIR)/$(COMON_DIR)/files.o $(OBJ_DIR)/$(LINUX_COMMON_DIR)/create.o
ALWAYS_CDF_OBJ = $(OBJ_DIR)/$(COMON_DIR)/files.o  $(OBJ_DIR)/$(LINUX_COMMON_DIR)/create_cdf.o
ALWAYS_TEST_OBJ = $(OBJ_DIR)/my_assert.o
linux_cnf_obj =	$(ALWAYS_OBJ) $(OBJ_DIR)/cnf.o
linux_cdf_obj = $(ALWAYS_CDF_OBJ) $(OBJ_DIR)/$(LINUX_CDF_DIR)/cdf.o
linux_test_validate_filename_obj = $(ALWAYS_OBJ) $(ALWAYS_TEST_OBJ) $(OBJ_DIR)/$(TESTS_LINUX_DIR)/test_validate_filename.o
test_validate_dirpath_objs = $(ALWAYS_OBJ) $(ALWAYS_TEST_OBJ) $(OBJ_DIR)/$(TESTS_LINUX_DIR)/test_validate_dirpath.o
linux_cdf: $(linux_cdf_obj) | $(BIN_DIR)/$(LINUX_CDF_DIR)
	$(CC) $(LDFLAGS) -I $(PCH_DIR)/$(LINUX_CDF_DIR) -I $(PCH_DIR)/$(LINUX_COMMON_DIR) -I $(PCH_DIR)/$(COMON_DIR) $^ -o $(BIN_DIR)/$(LINUX_CDF_DIR)/$@
linux_cnf: $(linux_cnf_obj) | $(BIN_DIR)/$(LINUX_CNF_DIR)
	$(CC) $(LDFLAGS) -I $(PCH_DIR)/$(LINUX_CNF_DIR) -I $(PCH_DIR)/$(LINUX_COMMON_DIR) -I $(PCH_DIR)/$(COMON_DIR) $^ -o $(BIN_DIR)/$(LINUX_CNF_DIR)/$@
linux_test_validate_dirpath: $(test_validate_dirpath_objs) | $(BIN_DIR)/$(TESTS_LINUX_DIR)
	$(CC) $(LDFLAGS) -I $(PCH_DIR)/$(TESTS_LINUX_DIR) -I $(PCH_DIR)/$(LINUX_COMMON_DIR) -I $(PCH_DIR)/$(COMON_DIR) $^ -o $(BIN_DIR)/$(TESTS_LINUX_DIR)/$@
linux_test_validate_filename: $(linux_test_validate_filename_obj) | $(BIN_DIR)/$(TESTS_LINUX_DIR)
	$(CC) $(LDFLAGS) -I $(PCH_DIR)/$(TESTS_LINUX_DIR) -I $(PCH_DIR)/$(LINUX_COMMON_DIR) -I $(PCH_DIR)/$(COMON_DIR) $^ -o $(BIN_DIR)/$(TESTS_LINUX_DIR)/$@

# For Object-files
# CFLAGS_RELEASE = -O2 -flto -DNDEBUG -fstack-protector-strong -D_FORTIFY_SOURCE=2 -fPIE
# For Executable-files
# LDFLAGS_RELEASE = -O2 -flto -pie -Wl,-z,relro,-z,now -s
.PHONY: clean
clean:
	rm -rf build

