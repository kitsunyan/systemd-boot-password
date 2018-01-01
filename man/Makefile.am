man_MANS = \
	sbpctl.1

EXTRA_DIST = \
	sbpctl.xml

XSLTPROC_FLAGS = \
	--nonet \
	--stringparam man.authors.section.enabled 0 \
	--stringparam man.copyright.section.enabled 0

%.1: %.xml
	$(XSLTPROC) -o $@ $(XSLTPROC_FLAGS) \
		http://docbook.sourceforge.net/release/xsl/current/manpages/docbook.xsl $<

CLEANFILES = \
	$(man_MANS)
