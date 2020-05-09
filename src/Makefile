CC=gcc
CFLAGS=-ggdb -Wall
LIBS=-lm
DEPS = backlightd.h sunrise.h
ODIR=../build

_OBJ = tests.o configuration.o driver.o timecalc.o sunrise.o
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))

$(ODIR)/%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

tests: $(OBJ)
	$(CC) -o $@ $^ $(LIBS) 
	mv tests ../bin

clean:
	rm -f $(ODIR)/*.o