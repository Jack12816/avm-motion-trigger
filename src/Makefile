SUBDIRS ?= utils avm sensors test bin

BUILD_SUBDIRS := $(SUBDIRS:%=build-%)
INSTALL_SUBDIRS := $(SUBDIRS:%=install-%)
UNINSTALL_SUBDIRS := $(SUBDIRS:%=uninstall-%)
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

$(INSTALL_SUBDIRS): DIR = $(@:install-%=%)
$(INSTALL_SUBDIRS):
	$(MAKE) install -C $(DIR)

$(UNINSTALL_SUBDIRS): DIR = $(@:uninstall-%=%)
$(UNINSTALL_SUBDIRS):
	$(MAKE) uninstall -C $(DIR)

install: build $(INSTALL_SUBDIRS)
uninstall: $(UNINSTALL_SUBDIRS)

all: build

.PHONY: all clean $(BUILD_SUBDIRS) $(CLEAN_SUBDIRS) $(OBJS)
