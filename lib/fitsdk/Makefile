CFLAGS   := -Wall
LDFLAGS  :=           
LDLIBS   := 
CPPFLAGS := -MMD -MP

LIB = ../build/libfitsdk.a
OBJ_DIR := ../obj

SRC := $(wildcard *.cpp)
OBJ := $(patsubst %.cpp, $(OBJ_DIR)/%.o, $(SRC))

all: $(LIB)

.PHONY: all clean

$(LIB) : $(OBJ)
	ar rcs $(LIB) $(OBJ)

$(OBJ_DIR)/%.o : %.cpp | $(OBJ_DIR)
	$(CXX) $(CPPFLAGS) $(CFLAGS) -c $< -o $@

$(OBJ_DIR):
	mkdir -p $@

clean:
	@$(RM) -rv $(LIB) $(OBJ_DIR)

-include $(OBJ:.o=.d)