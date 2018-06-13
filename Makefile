# Copyright 2018 KanzakiKino
COMPILER = gcc
INCLUDE  = -I./include
CFLAGS   = -I -g -Wall -Werror $(INCLUDE)

TARGET   = ./lib/libeasyff.a
SOURCE   = ./src
SOURCES  = $(wildcard $(SOURCE)/*.c)
OBJECT   = ./obj
OBJECTS  = $(addprefix $(OBJECT)/, $(notdir $(SOURCES:.c=.o)))
DEPENDS  = $(OBJECTS:.o=.d)

$(TARGET): $(OBJECTS)
	-mkdir -p $(shell dirname $(TARGET))
	$(AR) $(ARFLAGS) $@ $^

$(OBJECT)/%.o: $(SOURCE)/%.c
	-mkdir -p $(OBJECT)
	$(COMPILER) $(CFLAGS) -o $@ -c $<

all: clean $(TARGET)

test: $(TARGET)
	rdmd -L-L./lib/ ./test.d

clean:
	-rm -rf $(OBJECT) $(TARGET)

-include $(DEPENDS)
