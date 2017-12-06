CC := gcc
SRC := src
OBJ := obj
SRC_FILES := $(wildcard $(SRC)/*.c)
OBJ_FILES := $(patsubst $(SRC)/%.c,$(OBJ)/%.o,$(SRC_FILES))
LDFLAGS := -lm -lgsl -lgslcblas -lSDL2 -lGL -lGLU -lGLEW
CFLAGS := -Wall
DFLAGS := -gdwarf-2

trix2: $(OBJ_FILES)
	$(CC) $(LDFLAGS) -o $@ $^

dwarf: $(OBJ_FILES)
	$(CC) $(LDFLAGS) $(DFLAGS) -o $@ $^

debug: clean dwarf

do: clean trix2

$(OBJ)/%.o: $(SRC)/%.c
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm obj/*