PAGE_FILES = $(wildcard *.[0-9])
BUILD_PAGES = $(PAGE_FILES:%=build-%.gz)
CLEAN_PAGES = $(PAGE_FILES:%=clean-%.gz)
DOCS_PAGES = $(PAGE_FILES:%=docs-%)
INSTALL_PAGES = $(PAGE_FILES:%=install-%.gz)
UNINSTALL_PAGES = $(PAGE_FILES:%=uninstall-%.gz)

$(BUILD_PAGES): IN_FILE = $(@:build-%.gz=%)
$(BUILD_PAGES): OUT_FILE = $(@:build-%=%)
$(BUILD_PAGES):
	$(TEST) -f $(OUT_FILE) || \
		gzip -c $(IN_FILE) > $(OUT_FILE)

$(CLEAN_PAGES): PAGE = $(@:clean-%=%)
$(CLEAN_PAGES):
	$(TEST) -f $(PAGE) && \
		$(RM) $(PAGE) 2>/dev/null || true

build: $(BUILD_PAGES)
clean: $(CLEAN_PAGES)

$(DOCS_PAGES): PAGE = $(@:docs-%=%)
$(DOCS_PAGES):
	$(MAN) ./$(PAGE)

$(INSTALL_PAGES): PAGE = $(@:install-%=%)
$(INSTALL_PAGES): PAGE_SUF = $(suffix $(@:install-%.gz=%))
$(INSTALL_PAGES): PAGE_DIR = $(PAGE_SUF:.%=man%)
$(INSTALL_PAGES):
	-$(TEST) -d $(DESTDIR)$(mandir) || \
		$(INSTALL) -dm755 $(DESTDIR)$(mandir)
	-$(TEST) -d $(DESTDIR)$(mandir)/$(PAGE_DIR) || \
		$(INSTALL) -dm755 $(DESTDIR)$(mandir)/$(PAGE_DIR)
	$(INSTALL) -m644 $(PAGE) $(DESTDIR)$(mandir)/$(PAGE_DIR)

$(UNINSTALL_PAGES): PAGE = $(@:uninstall-%=%)
$(UNINSTALL_PAGES): PAGE_SUF = $(suffix $(@:uninstall-%.gz=%))
$(UNINSTALL_PAGES): PAGE_DIR = $(PAGE_SUF:.%=man%)
$(UNINSTALL_PAGES):
	-$(TEST) -f $(DESTDIR)$(mandir)/$(PAGE_DIR)/$(PAGE) && \
		$(RM) $(DESTDIR)$(mandir)/$(PAGE_DIR)/$(PAGE)

install: build $(INSTALL_PAGES)
uninstall: $(UNINSTALL_PAGES)

.PHONY: $(DOCS_PAGES)
