CC = gcc
TARGET = energy_manager
SRC_DIR = src
OBJ_DIR = obj
FLAGS = -g -O3 
DEPS = -lncurses
HEADERS = headers
OBJ = $(OBJ_DIR)/data_parser.c.o \
	  $(OBJ_DIR)/energy_manager.c.o \
	  $(OBJ_DIR)/hashmap.c.o \
	  $(OBJ_DIR)/main.c.o \
	  $(OBJ_DIR)/mem_check.c.o \
	  $(OBJ_DIR)/prompt.c.o \
	  $(OBJ_DIR)/act_impl.c.o \
	  $(OBJ_DIR)/algo.c.o \
	  $(OBJ_DIR)/log.c.o


all: .dst_check $(OBJ)
	@echo "Linking $(TARGET)"
	@$(CC) $(OBJ) -o $(TARGET) $(DEPS)

# Check if any additional directories need to be created
.dst_check:
	@echo "Checking for destination directory"
	@if [ ! -d $(OBJ_DIR) ]; then \
		mkdir $(OBJ_DIR); \
	fi

$(OBJ_DIR)/data_parser.c.o: $(SRC_DIR)/data_parser.c
	@echo "Building data_parser.c"
	@$(CC) -c $(SRC_DIR)/data_parser.c $(FLAGS) -o $(OBJ_DIR)/data_parser.c.o -I $(HEADERS)


$(OBJ_DIR)/energy_manager.c.o: $(SRC_DIR)/energy_manager.c
	@echo "Building energy_manager.c"
	@$(CC) -c $(SRC_DIR)/energy_manager.c $(FLAGS) -o $(OBJ_DIR)/energy_manager.c.o -I $(HEADERS)


$(OBJ_DIR)/hashmap.c.o: $(SRC_DIR)/hashmap.c
	@echo "Building hashmap.c"
	@$(CC) -c $(SRC_DIR)/hashmap.c $(FLAGS) -o $(OBJ_DIR)/hashmap.c.o -I $(HEADERS)


$(OBJ_DIR)/main.c.o: $(SRC_DIR)/main.c
	@echo "Building main.c"
	@$(CC) -c $(SRC_DIR)/main.c $(FLAGS) -o $(OBJ_DIR)/main.c.o -I $(HEADERS)


$(OBJ_DIR)/mem_check.c.o: $(SRC_DIR)/mem_check.c
	@echo "Building mem_check.c"
	@$(CC) -c $(SRC_DIR)/mem_check.c $(FLAGS) -o $(OBJ_DIR)/mem_check.c.o -I $(HEADERS)


$(OBJ_DIR)/prompt.c.o: $(SRC_DIR)/prompt.c
	@echo "Building prompt.c"
	@$(CC) -c $(SRC_DIR)/prompt.c $(FLAGS) -o $(OBJ_DIR)/prompt.c.o -I $(HEADERS)

$(OBJ_DIR)/act_impl.c.o: $(SRC_DIR)/act_impl.c
	@echo "Building act_impl.c"
	@$(CC) -c $(SRC_DIR)/act_impl.c $(FLAGS) -o $(OBJ_DIR)/act_impl.c.o -I $(HEADERS)

$(OBJ_DIR)/algo.c.o: $(SRC_DIR)/algo.c
	@echo "Building algo.c"
	@$(CC) -c $(SRC_DIR)/algo.c $(FLAGS) -o $(OBJ_DIR)/algo.c.o -I $(HEADERS)

$(OBJ_DIR)/log.c.o: $(SRC_DIR)/log.c
	@echo "Building log.c"
	@$(CC) -c $(SRC_DIR)/log.c $(FLAGS) -o $(OBJ_DIR)/log.c.o -I $(HEADERS)


# Cleanup operation
.PHONY: clean
clean:
	@rm -rf $(OBJ_DIR)
	@rm -rf $(TARGET)
