CFLAGS = -D_REENTRANT -Wall -pedantic -Isrc
LDLIBS = -lpthread

#ifdef DEBUG
CFLAGS += -g
LDFLAGS += -g
#endif

TARGETS = test/test libthreadcommander.so libthreadcommander.a

all: $(TARGETS)

src/threadcommander.o: src/threadcommander.c src/threadcommander.h
test/test: test/test.o src/threadcommander.o

shared: libthreadpool.so
static: libthreadpool.a

test/test.o: test/test.c src/threadcommander.h
	$(CC) -c -fPIC ${CFLAGS} -o $@ $<

libthreadcommander.so: src/threadcommander.c src/threadcommander.h
	$(CC) -shared -fPIC ${CFLAGS} -o $@ $< ${LDLIBS}

src/threadcommander.o: src/threadcommander.c src/threadcommander.h
	$(CC) -c -fPIC ${CFLAGS} -o $@ $<

libthreadcommander.a: src/threadcommander.o
	ar rcs $@ $^
	
clean:
	rm -f $(TARGETS) *~ */*~ */*.o
	
test: $(TARGETS)
	./test/test