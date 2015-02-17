SOURCES = $(wildcard *.cc) $(wildcard *.c)
OBJECTS = $(subst .c,.o,$(subst .cc,.o,$(SOURCES)))
INCLUDE = -I../include
C_FLAGS = -g3 -O0


all: $(OBJECTS)

%.o: %.cc
	gcc -c $(C_FLAGS) $(INCLUDE) $<

%.o: %.c
	gcc -c $(C_FLAGS) $(INCLUDE) $<

clean:
	rm -rf *.o
