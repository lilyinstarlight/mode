all: mode

mode:
	make -C src

clean:
	make -C src clean

.PHONY: all clean
