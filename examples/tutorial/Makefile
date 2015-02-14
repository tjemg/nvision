INCLUDE = -I../../include
LIBS    = ../../libtv.a -lstdc++ -lX11 -lXmu -lgpm -lncurses
CC      = gcc
TARGET  = tvguid16

all: $(TARGET)

$(TARGET): $(TARGET).o
	gcc -o $@ $< $(LIBS)

%.o: %.cc
	$(CC) -c $(INCLUDE) $<

clean:
	@rm -rf  $(TARGET)
	@rm -rf  *.o
	@rm -rf  *~

