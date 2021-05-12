CC = gcc
TARGET = energy_manager
SRC_DIR = src
OBJ_DIR = obj
HEADERS = headers
OBJ = $(OBJ_DIR)/data_parser.c.o \
	  $(OBJ_DIR)/mem_check.c.o \
	  $(OBJ_DIR)/main.c.o

all: .dst_check $(OBJ)
	@echo "Linking $(TARGET)"
	@$(CC) $(OBJ) -o $(TARGET)

# Check if any directories need to be created
.dst_check:
	@echo "Checking for destination directory"
	@if [[ ! -d $(OBJ_DIR) ]]; then \
		mkdir $(OBJ_DIR); \
	fi

$(OBJ_DIR)/main.c.o: $(SRC_DIR)/main.c
	@echo "Building $(OBJ_DIR)/main.c.o" 
	@$(CC) -c $(SRC_DIR)/main.c -O3 -g -o $(OBJ_DIR)/main.c.o -I $(HEADERS)

$(OBJ_DIR)/data_parser.c.o: $(SRC_DIR)/data_parser.c
	@echo "Building $(OBJ_DIR)/data_parser.c.o" 
	@$(CC) -c $(SRC_DIR)/data_parser.c -O3 -g -o $(OBJ_DIR)/data_parser.c.o -I $(HEADERS)

$(OBJ_DIR)/mem_check.c.o: $(SRC_DIR)/mem_check.c
	@echo "Building $(OBJ_DIR)/mem_check.c.o" 
	@$(CC) -c $(SRC_DIR)/mem_check.c -O3 -g -o $(OBJ_DIR)/mem_check.c.o -I $(HEADERS)


# Cleanup operation
.PHONY: clean
clean:
	@rm -rf $(OBJ_DIR)
	@rm -rf $(TARGET)
