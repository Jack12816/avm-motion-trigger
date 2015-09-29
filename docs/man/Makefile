PAGE_FILES = $(wildcard *.[0-9])
PAGES = $(PAGE_FILES:%=docs-%)

$(PAGES): PAGE = $(@:docs-%=%)
$(PAGES): docs-%:%
	$(MAN) ./$(PAGE)

.PHONY: $(PAGES)d
