OBJS = $(shell $(FIND) .. -type f -name *.o)
BINS = $(patsubst %.c,%.out,$(wildcard *.c))
TEST_BINS = $(BINS:%.out=test-%)
CLEAN_BINS = $(BINS:%.out=clean-%)

$(BINS): INFILE = $(@:%.out=%.c)
$(BINS):
ifeq ($(OBJS),)
	$(MAKE) build -C ..
endif
	$(TEST) -f $@ || \
		$(GCC) -o $@ $(INFILE) $(OBJS) $(CFLAGS) -std=gnu11 $(INCLUDES)

$(TEST_BINS): BIN = $(@:test-%=%.out)
$(TEST_BINS): test-%:%.out
	#
	# Test $(BIN)
	#
	@./$(BIN)
	@$(PRINTF) "\n\n"

$(CLEAN_BINS): BIN = $(@:clean-%=%.out)
$(CLEAN_BINS):
	$(TEST) -f $(BIN) && \
		$(RM) $(BIN) 2>/dev/null || true

clean: $(CLEAN_BINS)
test: $(TEST_BINS)
build: $(BINS)
all: build

.PHONY: clean build
