.PHONY : clean build

RM := rm
GCC := gcc
XML_CONF := xml2-config

CFLAGS = $(shell $(XML_CONF) --cflags) -Wall -g
INCLUDES = $(shell $(XML_CONF) --libs) -lcurl -lconfig

OBJS = $(wildcard ../avm/*.o ../*.o)
SRCS = $(wildcard *.c)
BINS = $(patsubst %.c,%,$(SRCS))
CLEAN_BINS = $(BINS:%=clean-%)

$(BINS):
	$(GCC) -o $@.bin $@.c $(OBJS) $(CFLAGS) -std=gnu11 $(INCLUDES)

$(CLEAN_BINS):
	$(RM) $(@:clean-%=%).bin || true

build: $(BINS)
clean: $(CLEAN_BINS)
all: build
