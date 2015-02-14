INCLUDE = -I/usr/include/rhtvision
LIBS    = -lrhtv -lstdc++
CC      = gcc
TARGET  = dlgdsn
SOURCE  = codebldr.cc consted.cc dsgobjs.cc freedsgn.cc propdlgs.cc propedit.cc strmoper.cc tappwnd.cc
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

