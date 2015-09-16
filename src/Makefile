SUBDIRS := avm test
CLEAN_SUBDIRS := $(SUBDIRS:%=clean-%)

all: $(SUBDIRS)
build: all
$(SUBDIRS):
	$(MAKE) all -C $@

clean: $(CLEAN_SUBDIRS)
$(CLEAN_SUBDIRS):
	$(MAKE) clean -C $(@:clean-%=%)

.PHONY: all clean $(SUBDIRS) $(CLEAN_SUBDIRS)
