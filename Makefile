COMP := gcc
SRCDIR = src
BUILDDIR := build
INCDIR := include -Isvec
TARGET := $(BUILDDIR)/bin/unnamed

COMPFLAGS := -g -Wall -Wextra -Wcast-align -O0 -I$(INCDIR) -fsanitize=address 

SRC := $(wildcard $(SRCDIR)/*.c)
OBJS := $(SRC:src/%.c=build/%.o)

all: $(TARGET)

$(TARGET): $(OBJS)
	$(COMP) $(COMPFLAGS) $(OBJS) -o $(TARGET)

$(BUILDDIR)/%.o: $(SRCDIR)/%.c

	mkdir -p $(BUILDDIR)
	$(COMP) $(COMPFLAGS) -c $< -o $@

clean:
	rm -f build/*.o build/bin/$(TARGET)
