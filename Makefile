SRC ?= src
BUILD ?= build
OBJ ?= $(BUILD)/obj
C_DIR ?= $(BUILD)/code
DIRS = $(SRC) $(BUILD) $(OBJ) $(C_DIR)

CFLAGS += -g -Wall -Wextra
CXXFLAGS += -g -Wall -Wextra
# LDFLAGS +=
LDLIBS += -lfl

LEX := flex
#LFLAGS += -d

YACC := bison
YFLAGS += -d -Wall

LEX_SRC = src/lex.l
LEX_SRC_C = $(patsubst $(SRC)/%.l, $(C_DIR)/%.yy.c, $(LEX_SRC))

YACC_SRC = src/main.y
YACC_SRC_C = $(patsubst $(SRC)/%.y, $(C_DIR)/%.tab.c, $(YACC_SRC))
YACC_SRC_H = $(patsubst $(SRC)/%.y, $(C_DIR)/%.tab.h, $(YACC_SRC))

C_SRC= src/tree.c

SOURCES = $(YACC_SRC_C) $(LEX_SRC_C)
OBJECTS = $(patsubst $(C_DIR)/%.c, $(OBJ)/%.o, $(SOURCES)) $(patsubst $(SCR)/%.c, $(OBJ)/%.o, $(C_SRC))

.PRECIOUS: $(SOURCES) $(OBJECTS) $(YACC_SRC_H)

.PHONY: all test

all: $(DIRS) $(BUILD)/calc

$(BUILD)/calc: $(OBJECTS)
	$(CC) $(LDFLAGS) $(LDLIBS) $^ -o $@

# Dependencies

$(LEX_SRC_C): $(YACC_SRC_H)

# Implicit rules

$(C_DIR)/%.yy.c: $(SRC)/%.l
	$(LEX) $(LFLAGS) -o $@ $<

$(C_DIR)/%.tab.c $(C_DIR)/%.tab.h: $(SRC)/%.y
	$(YACC) $(YFLAGS) -o $@ $<

$(OBJ)/%.o: $(C_DIR)/%.c
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@


$(OBJ)/%.o: $(SRC)/%.c
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@

$(DIRS):
	@mkdir -p $@

.PHONY: clean

clean:
	@rm -r build

