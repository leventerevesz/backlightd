CC=gcc
CFLAGS+=
LIBS+=-lcurl

DEPS = backlightd.h timefunctions.h apicalls.h jsmn.h
OBJ = configuration.o driver.o timefunctions.o apicalls.o

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
