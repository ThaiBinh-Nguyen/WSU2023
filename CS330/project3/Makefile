TARGET = pi
CFLAGS += -std=c99 -Wall
CPPFLAGS += -D_GNU_SOURCE
LDLIBS = -lm

.PHONY: all clean

all: $(TARGET)

clean:
	$(RM) -r $(TARGET) *~ *.dSYM core *.o
