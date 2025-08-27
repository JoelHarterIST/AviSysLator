CC = gcc

CFLAGS = -DAVISYSLATOR_GNC
LDFLAGS = -lm
LDFLAGS += -lcyaml
LDFLAGS += -lyaml

TARGET = avsl

OBJS = main/src/main.o
OBJS += userif/src/userif.o
OBJS += guidance/guitrns/src/guitrns.o
OBJS += plant/ptrns/src/ptrns.o

INCDIR = -I./common/src
INCDIR += -I./main/src
INCDIR += -I./userif/src
INCDIR += -I./guidance/guitrns/src
INCDIR += -I./plant/ptrns/src
INCDIR += -I./lib/gncmath/src

LIBDIR = -L./lib/gncmath

LIBS = -lgncmath

LD_LIBRARY_PATH=/usr/local/lib:$LD_LIBRARY_PATH

EXTS = ./main/src/main_ext.h
EXTS += ./lib/gncmath/src/gncmath.h
EXTS += ./guidance/guitrns/src/guitrns_ext.h
EXTS += ./plant/ptrns/src/ptrns_ext.h

.SUFFIXES: .c .o

$(TARGET): $(OBJS)
	cd "./lib/gncmath" && make lib && cd ../../
	$(CC) -o $(TARGET) $(INCDIR) $^ $(LIBDIR) $(LIBS) $(LDFLAGS)
	python create_symbolmap_yaml.py --obj_file $(TARGET) --header_file_list $(EXTS) --sim_mode "SILS"

.c.o:
	$(CC) $(CFLAGS) $(INCDIR) -c $< -o $@ $(LIBDIR) $(LIBS)

.PHONY: debug
debug: CFLAGS += -g3
debug: TARGET = debug_out
debug: clean $(TARGET)

.PHONY: clean
clean:
	$(RM) $(TARGET) $(OBJS)
