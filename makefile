all: mode

mode:
	make -C src ../mode

clean:
	make -C src clean

.PHONY: all clean
