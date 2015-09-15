TEST := test
RM := rm
MKDIR := mkdir
GCC := gcc
XML_CONF := xml2-config

.PHONY : clean build run

SRCS = $(wildcard src/*.c)
BINS = $(patsubst %.c,%,$(SRCS))

CFLAGS = `$(XML_CONF) --cflags`
INCLUDES = `$(XML_CONF) --libs` -lcurl

all: clean build run

clean:
	$(TEST) -d build && $(RM) build/*

$(BINS):
	$(GCC) $(CFLAGS) -std=gnu11 $(INCLUDES) -o $(@:src/%=build/%) $@.c

.setup-build:
	$(TEST) -d build || $(MKDIR) -p build

build: .setup-build $(BINS)

run: build
	./build/get-challenge
