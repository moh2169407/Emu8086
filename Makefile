# The goal for this makefile is build portable executable file and build and run tests
DEBUG = TRUE

# Check windows?
ifeq ($(OS), Windows_NT)
	DETECTED_OS := Windows        
	$(error "Haven't implemented windows support yet")
else # UNIX like system
	UNAME_S = $(shell uname -s)
	ifeq ($(UNAME_S), Darwin)
	DETECTED_OS := MAC
endif
ifeq ($(UNAME_S), Linux)
	DETECTED_OS := Linux
endif
endif
ifneq  ($(shell command -v gcc),)
	CC  = gcc
else 
	CC = cc
endif
BUILDDIR = build
BINDIR = $(BUILDDIR)/bin
TARGET = $(BINDIR)/8086
SRCDIR = src
SRC = $(wildcard $(SRCDIR)/*.c)
OBJ = $(SRC:$(SRCDIR)/%.c=$(BUILDDIR)/%.o)

INCDIR = -Iinclude -Isvec

TESTDIR = test
TESTBIN = $(BUILDDIR)/$(TESTDIR)
TESTOBJ = $(wildcard $(TESTDIR)/*.c)
TESTS = $(TESTOBJ:$(TESTDIR)/%.c=$(TESTBIN)/%)

CFLAGS = $(INCDIR)

DEPS = $(SRC:$(SRCDIR)/%.c=$(BUILDDIR)/%.k)

# Excluding main object to link lib 
LIBOBJ := $(shell echo $(OBJ) | tr " " "\n"| grep -v "main")
LIBTARGET = $(BINDIR)/lib8086.a

ifeq ($(DEBUG), TRUE)
	CFLAGS += -g -Wall -Wextra
	CFLAGS += -Werror -Wpedantic
	ifeq ($(shell echo | $(CC) -E -dM - | grep -c "__clang__"),0)
	CFLAGS += -fanalyzer
endif
else 
	CFLAGS += -DNDEBUG -O2
endif

.PHONY: all test test-%

SHELL = /bin/sh

all: $(TARGET)
	@echo "Build complete: $(TARGET)"

$(BUILDDIR):
	@echo "Creating build directory..."
	mkdir -p build

$(BINDIR): | $(BUILDDIR)
	@echo "Creating bin directory..."
	mkdir -p build/bin

$(TESTBIN):
	@echo "Creating test directory..."
	mkdir -p build/test

$(TARGET): $(OBJ) 
	@echo "Linking $@..."
	$(CC) $(CFLAGS) $(OBJ) -o $@

$(BUILDDIR)/%.o: $(SRCDIR)/%.c | $(BINDIR)
	@echo "Compiling $<..."
	$(CC) $(CFLAGS) -MMD -MP -c $< -o $@ 

$(TESTBIN)/%: $(TESTDIR)/%.c | $(TESTBIN) $(LIBTARGET)
	@echo "Compiling and linking test: $@..."
	$(CC) -l8086 -L$(BINDIR) $(CFLAGS) $< -o $@ 

$(LIBTARGET): $(OBJ)
	@echo "Archiving $@..."
	@ar -ruv $(LIBTARGET) $(OBJ)

-include $(DEPS)

test: $(TESTS)  
	@echo "Running all tests: $(TESTS)"
	@for TEST in $(TESTS); do \
		echo "--- $$TEST ---"; \
		./$$TEST; \
		done 
test-%: $(TESTBIN)/%-test
	./$(TESTBIN)/$*-test

clean:
	@echo "Cleaning object and dependency files..."
	rm -f $(BUILDDIR)/*.o
	rm -f $(BUILDDIR)/*.d
	rm -rf $(TESTBIN)

cleanclean: 
	@echo "Removing entire build directory..."
	rm -rf $(BUILDDIR)
