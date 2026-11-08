
   
ifeq ($(OS),Windows_NT)
MKDIR = if not exist bin mkdir bin
RMDIR = rmdir /s /q
BIN_DIR = bin
UNINSTALL_DIR = ..\create
else
MKDIR = mkdir -p ./bin
RMDIR = rm -rf
BIN_DIR = ./bin
UNINSTALL_DIR = ../create
endif

.PHONY: all install clean uninstall

all: install

install: create_file.c createmore.c createdirf.c
	$(MKDIR)
	gcc create_file.c createmore.c -o $(BIN_DIR)/cfmore
	gcc create_file.c createdirf.c -o $(BIN_DIR)/cdf

clean:
	$(RMDIR) $(BIN_DIR)

uninstall:
	$(RMDIR) $(UNINSTALL_DIR)

