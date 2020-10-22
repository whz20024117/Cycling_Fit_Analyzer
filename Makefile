CXX = g++
CFLAGS   := -Wall -g
LDFLAGS  := -Llib/build
LDLIBS   := -lfitsdk
CPPFLAGS := -Iinclude -Ilib/fitsdk -MMD -MP

SRC_DIR := src
OBJ_DIR := obj
BIN_DIR := bin

EXE := $(BIN_DIR)/beta
SRC := $(wildcard $(SRC_DIR)/*.cpp)
OBJ := $(patsubst $(SRC_DIR)/%.cpp, $(OBJ_DIR)/%.o, $(SRC))


all: $(EXE)
.PHONY: all clean

$(EXE) : $(OBJ) libfitsdk | $(BIN_DIR)
	$(CXX) $(LDFLAGS) $(OBJ) $(LDLIBS) -o $@

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp | $(OBJ_DIR)
	$(CXX) $(CPPFLAGS) $(CFLAGS) -c $< -o $@

$(BIN_DIR) $(OBJ_DIR):
	mkdir -p $@

.PHONY: libfitsdk
libfitsdk:
	$(MAKE) -C lib/fitsdk

clean:
	@$(RM) -rv $(BIN_DIR) $(OBJ_DIR)
	$(MAKE) clean -C lib/fitsdk

-include $(OBJ:.o=.d)

