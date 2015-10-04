include Makefile.shared

BINS = $(patsubst src/test/%.c,%,$(wildcard src/test/*.c))
TEST_BINS = $(BINS:%=test-%)
TEST_MEM_BINS = $(BINS:%=test-mem-%)

MAN_PAGES = $(patsubst docs/man/%,%,$(wildcard docs/man/*.[0-9]))
DOCS = $(MAN_PAGES:%=docs-%)

build:
	$(MAKE) build -C src/
	$(MAKE) build -C docs/man/
clean:
	$(MAKE) clean -C src/
	$(MAKE) clean -C docs/man/
test:
	$(MAKE) test -C src/test/
$(TEST_BINS):
	$(MAKE) $@ -C src/test/
$(TEST_MEM_BINS):
	$(MAKE) $@ -C src/test/
$(DOCS):
	$(MAKE) $@ -C docs/man/
install:
	$(MAKE) $@ -C src/
	$(MAKE) $@ -C docs/man/
	$(MAKE) $@ -C dist/
uninstall:
	$(MAKE) $@ -C src/
	$(MAKE) $@ -C docs/man/
	$(MAKE) $@ -C dist/
all: build

.PHONY: all clean test
