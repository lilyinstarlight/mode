EXE=mode

CP=cp
RM=rm -f

all:
	+make -C build
	$(CP) build/main $(EXE)

clean:
	+make -C build clean
	$(RM) $(EXE)

.PHONY: all clean
