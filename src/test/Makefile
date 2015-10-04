VALGRIND := valgrind

OBJS = $(shell $(FIND) .. -type f -name *.o)
BINS = $(patsubst %.c,%.out,$(wildcard *.c))
TEST_BINS = $(BINS:%.out=test-%)
TEST_MEM_BINS = $(BINS:%.out=test-mem-%)
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
	@./$(BIN) || true
	@$(PRINTF) "\n\n"

$(TEST_MEM_BINS): BIN = $(@:test-mem-%=%.out)
$(TEST_MEM_BINS): test-mem-%:%.out
	#
	# Test $(BIN)
	#
	@$(VALGRIND) -v \
		--track-origins=yes \
		--leak-check=full \
		./$(BIN)
	@$(PRINTF) "\n\n"

$(CLEAN_BINS): BIN = $(@:clean-%=%.out)
$(CLEAN_BINS):
	$(TEST) -f $(BIN) && \
		$(RM) $(BIN) 2>/dev/null || true

clean: $(CLEAN_BINS)
test: $(TEST_BINS)
test-mem: $(TEST_MEM_BINS)
build: $(BINS)
install: ;
uninstall: ;
all: build

.PHONY: clean build
