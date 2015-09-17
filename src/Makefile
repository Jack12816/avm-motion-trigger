SUBDIRS := avm test
CLEAN_SUBDIRS := $(SUBDIRS:%=clean-%)

RM := rm
GCC := gcc
XML_CONF := xml2-config

CFLAGS = $(shell $(XML_CONF) --cflags) -Wall -g
INCLUDES = $(shell $(XML_CONF) --libs) -lcurl -lconfig

SRCS = $(wildcard *.c)
OBJS = $(patsubst %.c,%,$(SRCS))
CLEAN_OBJS = $(OBJS:%=clean-%)

$(OBJS):
	$(GCC) -c $@.c $(CFLAGS) -std=gnu11 $(INCLUDES)

$(CLEAN_OBJS):
	$(RM) $(@:clean-%=%).o || true

$(SUBDIRS):
	$(MAKE) all -C $@

$(CLEAN_SUBDIRS):
	$(MAKE) clean -C $(@:clean-%=%)

build: $(OBJS) $(SUBDIRS)
clean: $(CLEAN_OBJS) $(CLEAN_SUBDIRS)
all: build

.PHONY : all clean $(SUBDIRS) $(CLEAN_SUBDIRS)
