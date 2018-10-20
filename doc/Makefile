all:
	cd doxygen; doxygen doxy
	cd ../ATTINY13A/doc/doxygen; doxygen doxy
	cd ../STM32F103CB/doc/doxygen; doxygen doxy
	cd ../common/doc/doxygen; doxygen doxy

clean:
	rm -rf doxygen/html
	rm -rf ../STM32F103CB/doc/doxygen/html
	rm -rf ../ATTINY13A/doc/doxygen/html
	rm -rf ../common/doc/doxygen/html
