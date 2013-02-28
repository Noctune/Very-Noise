CFLAGS = -Iinclude -Wall -pedantic -std=c99
SOURCES = src/cmdline.c src/generate.c src/main.c src/noise.c src/simplexnoise1234.c src/sum.c src/transform.c
INCLUDES = include/cmdline.h include/simplexnoise1234.h include/vnoise.h
OBJECTS  := $(SOURCES:src/%.c=obj/%.o)
EXFLAGS = -s 2209 -w 256 -h 256 -d 8

all: bin/vnoise

bin/vnoise: $(OBJECTS) |bin
	$(CC) $(CFLAGS) $(OBJECTS) -o bin/vnoise -lpng -lm

obj/%.o: src/%.c $(INCLUDES) |obj
	$(CC) $(CFLAGS) -c -o $@ $<

src/cmdline.c include/cmdline.h: cmdline.ggo
	gengetopt --header-output-dir include --src-output-dir src < cmdline.ggo

examples/simplex.png: bin/vnoise |examples
	./bin/vnoise $(EXFLAGS) -o examples/simplex.png

examples/fractal.png: bin/vnoise |examples
	./bin/vnoise $(EXFLAGS) -n 8 -o examples/fractal.png

examples/ridged.png: bin/vnoise |examples
	./bin/vnoise $(EXFLAGS) -n 8 -r -o examples/ridged.png

obj:
	mkdir -p obj
bin:
	mkdir -p bin
examples:
	mkdir -p examples

all-examples: examples/simplex.png examples/fractal.png examples/ridged.png

clean:
	rm -r obj
	rm src/cmdline.c include/cmdline.h

.PHONY: all-examples clean
