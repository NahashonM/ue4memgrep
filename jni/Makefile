
.DEFAULT_GOAL := test.exe


SRC_FILES := $(wildcard *.c)
OBJ_FILES := $(patsubst %.c,%.o,$(SRC_FILES))

test.exe: $(OBJ_FILES)
	gcc -o $@ $(patsubst %.o,objects/%.o,$(OBJ_FILES))

%.o: %.c
	gcc -c $< -o objects/$@

.PHONY := clean
clean:
	rm -rf objects/*