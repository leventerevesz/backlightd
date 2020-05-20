CC=gcc
CFLAGS+=
LIBS=-lm

DEPS = backlightd.h sunrise.h timecalc.h
OBJ = configuration.o driver.o timecalc.o sunrise.o

%.o: %.c $(DEPS)
	$(CC) $(CFLAGS) -c -o $@ $<

backlightd: $(OBJ) backlightd.o
	$(CC) -o $@ $^ $(LIBS)

tests: $(OBJ) tests.o
	$(CC) -ggdb -Wall -o $@ $^ $(LIBS)

clean:
	rm -f ./*.o
	rm -f ./backlightd
	rm -f ./tests
