install:
	-$(TEST) -d $(DESTDIR)$(sysconfdir) || \
		$(INSTALL) -dm755 $(DESTDIR)$(sysconfdir)
	$(INSTALL) -m644 avm-motion-trigger.conf $(DESTDIR)$(sysconfdir)

uninstall:
	-$(TEST) -f $(DESTDIR)$(sysconfdir)/avm-motion-trigger.conf && \
		$(RM) $(DESTDIR)$(sysconfdir)/avm-motion-trigger.conf
