CC = g++

CFLAGS = -std=c++14
LDFLAGS = -lm -lgtest -lgtest_main

COVERAGE_FLAGS = --coverage

TARGET = mdlrand_test
TARGET_DEBUG = debug_out
TARGET_LIB = libmdlrand.a

OBJS_LIB = src/mdlrand.o
OBJS_LIB += src/mt19937ar.o

OBJS_TEST = test/mdlrand_test.o
OBJS = $(OBJS_LIB) $(OBJS_TEST)

INCDIR = -I./src
INCDIR += -I../gncmath/src

LIBDIR = 

LIBS = 

.SUFFIXES: .c .o
.SUFFIXES: .cpp .o

$(TARGET): $(OBJS)
	$(CC) -o $(TARGET) $(INCDIR) $^ $(LIBDIR) $(LIBS) $(LDFLAGS)

$(TARGET_LIB): $(OBJS_LIB)
	ar rcs $(TARGET_LIB) $^

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
	genhtml lcovdata.info --branch-coverage -o ./test/coverage

./PHONY: lib
lib: clean $(TARGET_LIB)

.PHONY: clean
clean:
	$(RM) $(TARGET) $(TARGET_DEBUG) $(TARGET_LIB) $(OBJS_LIB) $(OBJS_TEST)
	find . -name '*.gcov' -type f | xargs rm -rf
	find . -name '*.gcda' -type f | xargs rm -rf
	find . -name '*.gcno' -type f | xargs rm -rf
	rm -rf lcovdata.info
	rm -rf test/coverage
