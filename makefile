EXE=mode
DEBUG=1
RESOURCE=
MYOBJ=

include platform.mk

SRCDIR=src
VENDORDIR=vendor
BUILDDIR=build
DATADIR=data
DISTDIR=dist

DIST=behaviours data fonts libs sounds specs textures $(EXE)

SRC=$(shell find '$(SRCDIR)' -maxdepth 1 -name '*.cpp' -exec basename '{}' ';')
DEP=$(shell echo '$(SRC)' | sed -e 's/\([^ ]*\).cpp/$(BUILDDIR)\/\1.d /g')
OBJ=$(shell echo '$(SRC)' | sed -e 's/\([^ ]*\).cpp/$(BUILDDIR)\/\1.o /g')

all: $(DEP) $(EXE) $(DATADIR)

$(EXE): $(OBJ) $(VENDORDIR)/yaml-cpp/build/libyaml-cpp.a $(VENDORDIR)/lua/liblua.a $(MYOBJ)
	$(CXX) $(LDFLAGS) -o $@ $^ $(LDLIBS)

$(BUILDDIR)/%.d: $(SRCDIR)/%.cpp
	$(CXX) $(CXXFLAGS) -DRESOURCE='$(RESOURCE)' -MQ $(@:.d=.o) -MF $@ -MM $<

$(BUILDDIR)/%.o: $(SRCDIR)/%.cpp
	$(CXX) $(CXXFLAGS) -DRESOURCE='$(RESOURCE)' -c -o $@ $<

$(VENDORDIR)/yaml-cpp/CMakeLists.txt:
	git submodule update --init $(VENDORDIR)/yaml-cpp

$(VENDORDIR)/yaml-cpp/build/libyaml-cpp.a: $(VENDORDIR)/yaml-cpp/CMakeLists.txt
	mkdir $(VENDORDIR)/yaml-cpp/build
	cmake -S $(VENDORDIR)/yaml-cpp -B $(VENDORDIR)/yaml-cpp/build -DCMAKE_CXX_COMPILER='$(CXX)' -DCMAKE_CXX_STANDARD='$(YAML_CPP_CXX_STANDARD)' -DCMAKE_CXX_FLAGS='$(YAML_CPP_CXX_FLAGS)' -DYAML_BUILD_SHARED_LIBS=OFF -DYAML_CPP_BUILD_TOOLS=OFF -DYAML_CPP_BUILD_TESTS=OFF -DYAML_CPP_CLANG_FORMAT_EXE=
	+make -C $(VENDORDIR)/yaml-cpp/build

$(VENDORDIR)/lua/makefile:
	git submodule update --init $(VENDORDIR)/lua

$(VENDORDIR)/lua/liblua.a: $(VENDORDIR)/lua/makefile
	+make -C $(VENDORDIR)/lua CC='$(CC)' CFLAGS='$(LUA_CFLAGS)' MYLDFLAGS='$(LUA_LDFLAGS)' MYLIBS='$(LUA_LDLIBS)' a

$(VENDORDIR)/lua/lua: $(VENDORDIR)/lua/makefile
	+make -C $(VENDORDIR)/lua CC='$(CC)' CFLAGS='$(LUA_CFLAGS)' MYLDFLAGS='$(LUA_LDFLAGS)' MYLIBS='$(LUA_LDLIBS)' lua

$(VENDORDIR)/sol3/single/include/sol/sol.hpp:
	git submodule update --init $(VENDORDIR)/sol3

$(DEP): | $(BUILDDIR)

$(OBJ): | $(BUILDDIR)

$(BUILDDIR):
	mkdir -p $@

$(DATADIR):
	mkdir -p $@

clean:
	$(RM) -r $(VENDORDIR)/yaml-cpp/build
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
