SOURCES = $(wildcard *.cc) $(wildcard x11/*.cc) $(wildcard linux/*.cc) $(wildcard unix/*.cc)
OBJECTS = $(subst .cc,.o,$(SOURCES))
INCLUDE = -I../include


all: $(OBJECTS)

%.o: %.cc
	gcc -c $(C_FLAGS) $(INCLUDE) $<

clean:
	rm -rf *.o
	rm -rf *.a
