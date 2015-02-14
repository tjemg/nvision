SOURCES = $(wildcard *.cc) $(wildcard *.c)
OBJECTS = $(subst .c,.o,$(subst .cc,.o,$(SOURCES)))
INCLUDE = -I../include


all: $(OBJECTS)

%.o: %.cc
	gcc -c $(C_FLAGS) $(INCLUDE) $<

%.o: %.c
	gcc -c $(C_FLAGS) $(INCLUDE) $<

