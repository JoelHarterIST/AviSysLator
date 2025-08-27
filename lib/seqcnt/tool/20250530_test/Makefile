PROGRAM = seqcnt_tool_test
OBJS = ../../src/seqcnt.o
OBJS += main.o


CC = gcc

.SUFFIXES: .c .o

$(PROGRAM): $(OBJS)
	$(CC) -o $(PROGRAM) $^

.c.o:
	$(CC) $(CFLAGS) -c $< -o $@

.PHONY: clean
clean:
	$(RM) $(PROGRAM) $(OBJS)
