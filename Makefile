include Makefile.shared

BINS = $(patsubst src/test/%.c,%,$(wildcard src/test/*.c))
TEST_BINS = $(BINS:%=test-%)

build:
	$(MAKE) build -C src/
clean:
	$(MAKE) clean -C src/
test:
	$(MAKE) test -C src/test/
$(TEST_BINS):
	$(MAKE) $@ -C src/test/
all: build

.PHONY: all clean test
