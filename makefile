all:
	make -C src mode
	cp src/mode .

clean:
	make -C src clean

.PHONY: all clean
