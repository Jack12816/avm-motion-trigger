OBJS = $(shell $(FIND) .. -type f -name *.o)
SRCS = $(wildcard *.c)
BINS = $(patsubst %.c,%,$(SRCS))
TEST_BINS = $(BINS:%=test-%)
CLEAN_BINS = $(BINS:%=clean-%)

$(BINS):
ifeq ($(OBJS),)
	$(MAKE) build -C ..
endif
	$(TEST) -f $@ || \
		$(GCC) -o $@ $@.c $(OBJS) $(CFLAGS) -std=gnu11 $(INCLUDES)

$(TEST_BINS): FILE = $(@:test-%=%)
$(TEST_BINS): test-%:%
	#
	# Test $(FILE)
	#
	@./$(FILE)
	@$(PRINTF) "\n\n"

$(CLEAN_BINS): FILE = $(@:clean-%=%)
$(CLEAN_BINS):
	$(TEST) -f $(FILE) && \
		$(RM) $(FILE) 2>/dev/null || true

clean: $(CLEAN_BINS)
test: $(TEST_BINS)
build: $(BINS)
all: build

.PHONY: clean build
