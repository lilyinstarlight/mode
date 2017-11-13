EXE=mode

all:
	make -C src
	cp src/main $(EXE)

clean:
	make -C src clean
	rm $(EXE)

.PHONY: all clean
