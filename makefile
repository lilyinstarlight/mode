EXE=mode

all:
	make -C build
	cp build/main $(EXE)

clean:
	make -C build clean
	rm $(EXE)

.PHONY: all clean
