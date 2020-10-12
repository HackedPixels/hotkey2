include config.mk

addparent = $(foreach A,$(1),$(addsuffix $(subst /,,$(A)).o,$(A)))

#SUBDIRS := $(wildcard */)
SUBDIRS := libini/
SUBCLEAN := $(addsuffix .clean,$(SUBDIRS))
LIBTARGETS := $(call addparent,$(SUBDIRS))

# this Makefile needs love.
hotkey_cc: $(LIBTARGETS) main.o
	$(CC) $(DEBUG) $^ -o hotkey_cc

hotkeyd: eval.o
	$(CC) $(DEBUG) $^ -o hotkeyd
	

%.o: %.c
	$(CC) $(DEBUG) -c $< -o $@

clean: $(SUBCLEAN)
	rm -f *.so *.o

$(SUBCLEAN): %.clean:
	$(MAKE) -C $* clean

$(LIBTARGETS):
	$(MAKE) -C $(dir $@) $(notdir $@)
