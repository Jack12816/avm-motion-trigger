SUBDIRS ?= avm test

BUILD_SUBDIRS := $(SUBDIRS:%=build-%)
CLEAN_SUBDIRS := $(SUBDIRS:%=clean-%)

OBJS := $(patsubst %.c,%.o,$(wildcard *.c))
CLEAN_OBJS := $(OBJS:%=clean-%)

$(OBJS): INFILE = $(@:%o=%c)
$(OBJS): OUTFILE = $(@)
$(OBJS):
	$(TEST) -f $(OUTFILE) || \
		$(GCC) -c $(INFILE) $(CFLAGS) -std=gnu11 $(INCLUDES)

$(CLEAN_OBJS): FILE = $(@:clean-%=%)
$(CLEAN_OBJS):
	$(TEST) -f $(FILE) && \
		$(RM) $(FILE) 2>/dev/null || true

$(BUILD_SUBDIRS): DIR = $(@:build-%=%)
$(BUILD_SUBDIRS):
	$(MAKE) all -C $(DIR)

$(CLEAN_SUBDIRS): DIR = $(@:clean-%=%)
$(CLEAN_SUBDIRS):
	$(MAKE) clean -C $(DIR)

clean: $(CLEAN_OBJS) $(CLEAN_SUBDIRS)
build: $(OBJS) $(BUILD_SUBDIRS)
all: build

.PHONY: all clean $(BUILD_SUBDIRS) $(CLEAN_SUBDIRS) $(OBJS)
