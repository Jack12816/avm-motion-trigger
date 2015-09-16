.PHONY : clean build

RM := rm
GCC := gcc
XML_CONF := xml2-config

CFLAGS = $(shell $(XML_CONF) --cflags) -Wall -g
INCLUDES = $(shell $(XML_CONF) --libs) -lcurl

SRCS = $(wildcard *.c)
OBJS = $(patsubst %.c,%,$(SRCS))
CLEAN_OBJS = $(OBJS:%=clean-%)

$(OBJS):
	$(GCC) -c $@.c $(CFLAGS) -std=gnu11 $(INCLUDES)

$(CLEAN_OBJS):
	$(RM) $(@:clean-%=%).o || true

build: $(OBJS)
clean: $(CLEAN_OBJS)
all: build
