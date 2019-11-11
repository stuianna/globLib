
all: config docs stm32f767zi templates

clean: cleandoc cleanstm32f767zi cleantemplates noconfig

# Config
config:
	@tools/config.sh

noconfig:
	@tools/noconfig.sh

# MCU Static Libraries
stm32f767zi:
	@echo -e "\033[0;32m[Compiling STM32F767ZI Library]\033[0m"
	@cd lib/stm32f767zi; make;

cleanstm32f767zi:
	@cd lib/stm32f767zi; make clean;

# Templates
templates:
	@echo -e "\033[0;32m[Compiling STM32F767ZI Template]\033[0m"
	@cd template/stm32f767zi; make

cleantemplates:
	@cd template/stm32f767zi; make clean

# Documentation
docs:
	@echo -e "\033[0;32m[Compiling Doxygen Documentation]\033[0m"
	@cd doc/doxygen; doxygen doxy
	@cd lib/stm32f767zi/doc/doxygen; doxygen doxy
	@cd lib/common/doc/doxygen; doxygen doxy

cleandoc:
	@rm -rf doc/doxygen/html
	@rm -rf lib/stm32f767zi/doc/doxygen/html
	@rm -rf lib/common/doc/doxygen/html

viewdocs:
	@$$BROWSER doc/doxygen/html/index.html
