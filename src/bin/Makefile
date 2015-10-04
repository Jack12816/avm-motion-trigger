OBJS = $(shell $(FIND) .. -type f -name *.o)
BINS = $(patsubst %.c,%,$(wildcard *.c))
CLEAN_BINS = $(BINS:%=clean-%)
INSTALL_BINS = $(BINS:%=install-%)
UNINSTALL_BINS = $(BINS:%=uninstall-%)

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

$(INSTALL_BINS): BIN = $(@:install-%=%)
$(INSTALL_BINS):
	-$(TEST) -d $(DESTDIR)$(bindir) || \
		$(INSTALL) -dm755 $(DESTDIR)$(bindir)
	$(INSTALL) -m755 $(BIN) $(DESTDIR)$(bindir)

$(UNINSTALL_BINS): BIN = $(@:uninstall-%=%)
$(UNINSTALL_BINS):
	-$(TEST) -f $(DESTDIR)$(bindir)/$(BIN) && \
		$(RM) $(DESTDIR)$(bindir)/$(BIN)

install: $(INSTALL_BINS)
uninstall: $(UNINSTALL_BINS)
all: build

.PHONY: clean build
