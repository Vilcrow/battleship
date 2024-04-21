default:
	-mkdir bin
	cd src/core && $(MAKE)
	cd src/server && $(MAKE)
	cd src/client && $(MAKE)

clean:
	cd src/core && $(MAKE) clean
	cd src/server && $(MAKE) clean
	cd src/client && $(MAKE) clean
