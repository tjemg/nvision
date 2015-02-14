INCLUDE = -I../../include
LIBS    = ../../libtv.a -lstdc++ -lX11 -lXmu -lgpm -lncurses
#INCLUDE = -I/usr/include/rhtvision
#LIBS    = -lrhtv -lstdc++
CC      = gcc
TARGET  = tcombo
SOURCE  = $(wildcard *.cpp)
OBJECTS = $(subst .cpp,.o,$(SOURCE))
C_FLAGS = -g3 -O0

all: $(TARGET)

$(TARGET): $(OBJECTS)
	gcc $(C_FLAGS) -o $@ $^ $(LIBS)

%.o: %.cpp
	$(CC) -c $(C_FLAGS) $(INCLUDE) $<

clean:
	@rm -rf  $(TARGET)
	@rm -rf  *.o
	@rm -rf  *~

