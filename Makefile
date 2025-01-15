cc = cc

NAME = librbac

UNAME_S = $(shell uname -s)

LDFLAGS = -lsqlite3
CFLAGS  = -std=c17 -O3 -fPIC -Wall -Wextra

# respect traditional UNIX paths
INCDIR  = /usr/local/include
LIBDIR  = /usr/local/lib

ifeq ($(UNAME_S),Darwin)
$(NAME).dylib: clean
	$(CC) -dynamiclib -o $@ rbac.c $(CFLAGS) $(LDFLAGS)
endif
ifeq ($(UNAME_S),Linux)
$(NAME).so: clean
	$(CC) -shared -o $@ rbac.c $(CFLAGS) $(LDFLAGS)
endif

.PHONY: tests
tests: clean
	$(CC) -o tests/tests tests/unity.c tests/rbac_test.c bluesky.c $(CFLAGS) $(LDFLAGS)
	tests/tests
	rm -f tests/tests

.PHONY: valgrind
valgrind:
	$(CC) -g -o $@ rbac.c example.c $(CFLAGS) $(LDFLAGS)
	valgrind --leak-check=full ./valgrind 2>&1 | awk -F':' '/definitely lost:/ {print $2}'
	rm -f valgrind

.PHONY: install
install: 
	cp rbac.h $(INCDIR)
ifeq ($(UNAME_S),Linux)
	cp rbac.h $(INCDIR)
	cp $(NAME).so $(LIBDIR)
endif
ifeq ($(UNAME_S),Darwin)
	cp rbac.h $(INCDIR)
	cp $(NAME).dylib $(LIBDIR)
endif

uninstall:
	rm -f $(INCDIR)/rbac.h
ifeq ($(UNAME_S),Linux)
	rm -f $(INCDIR)/$(NAME).so
endif
ifeq ($(UNAME_S),Darwin)
	rm -f $(INCDIR)/$(NAME).dylib
endif

.PHONY: clean
clean:
	rm -f $(NAME).dylib
	rm -f $(NAME).so
	rm -f example
	rm -f tests/tests
	rm -f bin/example

.PHONY: example
example: clean
	$(CC) -g -o bin/$@ rbac.c example.c $(CFLAGS) $(LDFLAGS)
