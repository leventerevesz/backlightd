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

install:
	install ./backlightd /usr/bin
	install ./backlightd.service /lib/systemd/system/

remove:
	rm /usr/bin/backlightd
	rm /lib/systemd/system/backlightd.service

clean:
	rm -f ./*.o
	rm -f ./backlightd
	rm -f ./tests

PHONY: install, remove, clean