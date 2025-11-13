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
INCDIR += -I./lib/CMatrixMath

LIBDIR = -L./lib/gncmath
LIBDIR += -L./lib/CMatrixMath/build/release

LIBS = -lgncmath
LIBS += -lCMatrixMath

LD_LIBRARY_PATH=/usr/local/lib:$LD_LIBRARY_PATH

EXTS = ./main/src/main_int.h
EXTS += ./lib/gncmath/src/gncmath.h
EXTS += ./lib/CMatrixMath/src/matrices.h
EXTS += ./guidance/guitrns/src/guitrns_ext.h
EXTS += ./guidance/guitrns/src/guitrns_int.h
EXTS += ./plant/ptrns/src/ptrns_ext.h
EXTS += ./plant/ptrns/src/ptrns_int.h

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
