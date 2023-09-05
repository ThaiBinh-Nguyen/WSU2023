TARGET = myexp rerr
LDLIBS = -lm
CFLAGS += -std=c99 -D_DEFAULT_SOURCE -Wall

.PHONY: all clean

all: $(TARGET)

JUNK=*.o *~ *.dSYM

clean:
	$(RM) $(TARGET) $(JUNK)
