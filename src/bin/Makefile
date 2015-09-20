OBJS = $(shell $(FIND) .. -type f -name *.o)
BINS = $(patsubst %.c,%,$(wildcard *.c))
CLEAN_BINS = $(BINS:%=clean-%)

$(BINS): INFILE = $(@:%=%.c)
$(BINS):
ifeq ($(OBJS),)
	$(MAKE) build -C ..
endif
	$(TEST) -f $@ || \
		$(GCC) -o $@ $(INFILE) $(OBJS) $(CFLAGS) -std=gnu11 $(INCLUDES)

$(CLEAN_BINS): BIN = $(@:clean-%=%)
$(CLEAN_BINS):
	$(TEST) -f $(BIN) && \
		$(RM) $(BIN) 2>/dev/null || true

clean: $(CLEAN_BINS)
build: $(BINS)
all: build

.PHONY: clean build
