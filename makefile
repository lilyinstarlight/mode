EXE=mode

all:
	make -C src
	cp src/main $(EXE)

clean:
	make -C src clean

.PHONY: all clean
