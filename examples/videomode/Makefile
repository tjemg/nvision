INCLUDE = -I../../include
LIBS    = ../../libtv.a -lstdc++ -lX11 -lXmu -lgpm -lncurses
CC      = gcc
TARGET  = videomode
SOURCE  = $(wildcard *.cc)
OBJECTS = $(subst .cc,.o,$(SOURCE))
C_FLAGS = -g3 -O0

all: $(TARGET)

$(TARGET): $(OBJECTS)
	gcc $(C_FLAGS) -o $@ $^ $(LIBS)

%.o: %.cc
	$(CC) -c $(C_FLAGS) $(INCLUDE) $<

clean:
	@rm -rf  $(TARGET)
	@rm -rf  *.o
	@rm -rf  *~

