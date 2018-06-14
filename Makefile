# Copyright 2018 KanzakiKino
COMPILER = gcc
INCLUDE  = ./include
CFLAGS   = -I -g -Wall -Werror -I$(INCLUDE)

TARGET   = ./lib/libeasyff.a
SOURCE   = ./src
SOURCES  = $(wildcard $(SOURCE)/*.c)
OBJECT   = ./obj
OBJECTS  = $(addprefix $(OBJECT)/, $(notdir $(SOURCES:.c=.o)))
DEPENDS  = $(OBJECTS:.o=.d)

INSTALL_INCLUDE = /usr/include/easyff

$(TARGET): $(OBJECTS)
	-mkdir -p $(shell dirname $(TARGET))
	$(AR) $(ARFLAGS) $@ $^

$(OBJECT)/%.o: $(SOURCE)/%.c
	-mkdir -p $(OBJECT)
	$(COMPILER) $(CFLAGS) -o $@ -c $<

all: clean $(TARGET)

test: $(TARGET)
	rdmd -L-L./lib/ --force ./test.d

clean:
	-rm -rf $(OBJECT) $(TARGET)

install:
	-mkdir -p $(INSTALL_INCLUDE)
	cp $(wildcard $(INCLUDE)/ff*.h) $(INSTALL_INCLUDE)
	cp $(TARGET) /usr/lib

-include $(DEPENDS)
