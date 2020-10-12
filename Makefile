addparent = $(foreach A,$(1),$(addsuffix $(subst /,,$(A)).o,$(A)))

SUBDIRS := $(wildcard */)
SUBCLEAN := $(addsuffix .clean,$(SUBDIRS))
LIBTARGETS := $(call addparent,$(SUBDIRS))

# this Makefile needs love.
bin: $(LIBTARGETS) main.o
	$(CC) $$DEBUG $^ -o hotkey

%.o: %.c
	$(CC) $$DEBUG -c $< -o $@

clean: $(SUBCLEAN)

$(SUBCLEAN): %.clean:
	$(MAKE) -C $* clean

$(LIBTARGETS): 
	$(MAKE) -C $(dir $@) $(notdir $@)
