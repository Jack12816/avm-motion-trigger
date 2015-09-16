all: build
build:
	$(MAKE) build -C src/
clean:
	$(MAKE) clean -C src/

.PHONY: all clean
