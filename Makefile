include Makefile.shared

BINS = $(patsubst src/test/%.c,%,$(wildcard src/test/*.c))
TEST_BINS = $(BINS:%=test-%)
TEST_MEM_BINS = $(BINS:%=test-mem-%)

MAN_PAGES = $(patsubst docs/man/%,%,$(wildcard docs/man/*.[0-9]))
DOCS = $(MAN_PAGES:%=docs-%)

build:
	$(MAKE) build -C src/
clean:
	$(MAKE) clean -C src/
test:
	$(MAKE) test -C src/test/
$(TEST_BINS):
	$(MAKE) $@ -C src/test/
$(TEST_MEM_BINS):
	$(MAKE) $@ -C src/test/
$(DOCS):
	$(MAKE) $@ -C docs/man/
all: build

.PHONY: all clean test
