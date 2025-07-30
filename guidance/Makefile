CC = g++

CFLAGS = -std=c++14
LDFLAGS = -lm -lcyaml -lyaml -lgtest -lgtest_main

COVERAGE_FLAGS = --coverage

TARGET = guidance_test
TARGET_DEBUG = debug_out

OBJS = src/guidance.o
OBJS += test/guidance_test.o

INCDIR = -I./src -I../common/src -I../main/src -I../gncmath/src -I../plant/src -I../userif/src

LIBDIR = -L../gncmath -L../userif

LIBS = -lgncmath -luserif

LD_LIBRARY_PATH=/usr/local/lib:$LD_LIBRARY_PATH

EXTS = ./src/guidance_ext.h
EXTS += ../gncmath/src/gncmath.h

.SUFFIXES: .c .o
.SUFFIXES: .cpp .o

$(TARGET): $(OBJS)
	cd "../gncmath" && make lib && cd ../guidance
	cd "../userif" && make lib && cd ../guidance
	$(CC) -o $(TARGET) $(INCDIR) $^ $(LIBDIR) $(LIBS) $(LDFLAGS)
	python ../create_symbolmap_yaml.py --obj_file $(TARGET) --header_file_list $(EXTS) --sim_mode "SILS"

.c.o:
	$(CC) $(CFLAGS) $(INCDIR) -c $< -o $@ $(LIBDIR) $(LIBS)

.cpp.o:
	$(CC) $(CFLAGS) $(INCDIR) -c $< -o $@ $(LIBDIR) $(LIBS)

.PHONY: debug
debug: CFLAGS += -g3
debug: TARGET = $(TARGET_DEBUG)
debug: clean $(TARGET)

.PHONY: test
test: clean $(TARGET)
	./$(TARGET)

.PHONY: coverage
coverage: CFLAGS += $(COVERAGE_FLAGS)
coverage: LDFLAGS += $(COVERAGE_FLAGS)
coverage: clean $(TARGET)
	./$(TARGET)
	lcov -d src/ -c --rc lcov_branch_coverage=1 -o lcovdata.info
	genhtml lcovdata.info --branch-coverage -o ./coverage

.PHONY: clean
clean:
	$(RM) $(TARGET) $(TARGET_DEBUG) $(OBJS)
	find . -name '*.gcov' -type f | xargs rm -rf
	find . -name '*.gcda' -type f | xargs rm -rf
	find . -name '*.gcno' -type f | xargs rm -rf
	rm -rf lcovdata.info
	rm -rf coverage
