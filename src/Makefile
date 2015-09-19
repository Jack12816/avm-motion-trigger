SUBDIRS ?= avm test
CLEAN_SUBDIRS := $(SUBDIRS:%=clean-%)

SRCS := $(patsubst %.c,%,$(wildcard *.c))
CLEAN_SRCS := $(SRCS:%=clean-%)

$(SRCS): INFILE = $(@).c
$(SRCS): OUTFILE = $(@).o
$(SRCS):
	$(TEST) -f $(OUTFILE) || \
		$(GCC) -c $(INFILE) $(CFLAGS) -std=gnu11 $(INCLUDES)

$(CLEAN_SRCS): FILE = $(@:clean-%=%).o
$(CLEAN_SRCS):
	$(TEST) -f $(FILE) && \
		$(RM) $(FILE) 2>/dev/null || true

$(SUBDIRS):
	$(MAKE) all -C $@

$(CLEAN_SUBDIRS):
	$(MAKE) clean -C $(@:clean-%=%)

clean: $(CLEAN_SRCS) $(CLEAN_SUBDIRS)
build: $(SRCS) $(SUBDIRS)
all: build

.PHONY: all clean $(SUBDIRS) $(CLEAN_SUBDIRS)
