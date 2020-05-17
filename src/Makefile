CC=gcc
CFLAGS=
LIBS=-lm
DEPS = backlightd.h sunrise.h timecalc.h
ODIR=../build

_OBJ = configuration.o driver.o timecalc.o sunrise.o
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))

$(ODIR)/%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

backlightd: $(OBJ)
	$(CC) -o $@ $^ backlightd.c $(LIBS)
	mv backlightd ../bin

tests: $(OBJ)
	$(CC) -ggdb -Wall -o $@ $^ tests.c $(LIBS) 
	mv tests ../bin

clean:
	rm -f $(ODIR)/*.o