INCLUDE = -I/usr/include/rhtvision
LIBS    = ../../libtv.a -lstdc++ -lX11 -lXmu -lgpm -lncurses
CC      = gcc
TARGET  = tvguid01

all: $(TARGET)

$(TARGET): $(TARGET).o
	gcc -o $@ $< $(LIBS)

%.o: %.cc
	$(CC) -c $(INCLUDE) $<

clean:
	@rm -rf  $(TARGET)
	@rm -rf  *.o
	@rm -rf  *~

