CC=gcc
COPTS=-g -Wall -std=c99
ALL=LUtest

all: $(ALL)

JUNK= *.o *~ *.dSYM LUtest

clean:
	-rm -rf $(JUNK)

run: LUtest
	./LUtest

LUtest: LUtest.o LUfact.o
	$(CC) $(COPTS) $^ -lm -o $@

LUfact.o: LUfact.Cosine LUfact.h
LUtest.o: LUtest.Cosine LUfact.h

.Cosine.o:
	$(CC) -Cosine $(COPTS) $<