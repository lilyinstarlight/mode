EXE=mode
DEBUG?=1
RESOURCE?=

CC?=gcc
CXX?=g++

include platform.mk

SRCDIR=src
VENDORDIR=vendor
BUILDDIR=build
DATADIR=data
DISTDIR=dist

DIST=behaviours data fonts libs sounds specs textures $(EXE)

SRC=$(shell find '$(SRCDIR)' -maxdepth 1 -name '*.cpp' -exec sh -c "basename '{}'" ';')
DEP=$(shell echo '$(SRC)' | sed -e 's/\([^ ]*\).cpp/$(BUILDDIR)\/\1.d /g')
OBJ=$(shell echo '$(SRC)' | sed -e 's/\([^ ]*\).cpp/$(BUILDDIR)\/\1.o /g')

all: $(DEP) $(EXE) $(DATADIR)

$(EXE): $(OBJ) $(VENDORDIR)/lua/liblua.a
	$(CXX) $(LDFLAGS) -o $@ $^ $(LDLIBS)

$(BUILDDIR)/%.d: $(SRCDIR)/%.cpp
	$(CXX) $(CXXFLAGS) -DRESOURCE='$(RESOURCE)' -MQ $(@:.d=.o) -MF $@ -MM $<

$(BUILDDIR)/%.o: $(SRCDIR)/%.cpp
	$(CXX) $(CXXFLAGS) -DRESOURCE='$(RESOURCE)' -c -o $@ $<

$(VENDORDIR)/lua/liblua.a: $(VENDORDIR)/lua/makefile
	+make -C $(VENDORDIR)/lua CC=$(CC) MYCFLAGS='$(LUA_CFLAGS)' MYLDFLAGS='$(LUA_LDFLAGS)' MYLIBS='$(LUA_LDLIBS)' a

$(VENDORDIR)/lua/lua: $(VENDORDIR)/lua/makefile
	+make -C $(VENDORDIR)/lua CC=$(CC) MYCFLAGS='$(LUA_CFLAGS)' MYLDFLAGS='$(LUA_LDFLAGS)' MYLIBS='$(LUA_LDLIBS)' lua

$(DEP): | $(BUILDDIR)

$(OBJ): | $(BUILDDIR)

$(BUILDDIR):
	mkdir -p $@

$(DATADIR):
	mkdir -p $@

clean:
	$(RM) $(VENDORDIR)/lua/*.d $(VENDORDIR)/lua/*.o $(VENDORDIR)/lua/*.a $(VENDORDIR)/lua/lua
	$(RM) $(BUILDDIR)/*.d $(BUILDDIR)/*.o $(BUILDDIR)/*.a $(EXE)

dist: all
	mkdir -p $(DISTDIR)
	cp -a $(DIST) $(DISTDIR)/

distclean: clean
	$(RM) -r $(BUILDDIR) $(DATADIR) $(DISTDIR)

.PHONY: all clean dist distclean

ifneq ($(filter-out clean distclean,$(MAKECMDGOALS)),)
-include $(DEP)
endif
