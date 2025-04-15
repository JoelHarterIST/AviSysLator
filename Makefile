CC = gcc

CFLAGS = -DAVISYSLATOR_GNC
LDFLAGS = -lm -lcyaml -lyaml

TARGET = avsl

OBJS = main/src/main.o
OBJS += userif/src/userif.o
OBJS += gncmath/src/gncmath.o
OBJS += guidance/src/guidance.o
OBJS += plant/src/plant.o

INCDIR = -I./common/src
INCDIR += -I./main/src
INCDIR += -I./userif/src
INCDIR += -I./gncmath/src
INCDIR += -I./guidance/src
INCDIR += -I./plant/src

LIBDIR = -L./gncmath

LIBS = -lgncmath

LD_LIBRARY_PATH=/usr/local/lib:$LD_LIBRARY_PATH

EXTS = ./main/src/main_ext.h
EXTS += ./gncmath/src/gncmath.h
EXTS += ./guidance/src/guidance_ext.h
EXTS += ./plant/src/plant_ext.h

.SUFFIXES: .c .o

$(TARGET): $(OBJS)
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
