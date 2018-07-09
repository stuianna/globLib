This directory contains a working project template for each MCU.

The goal of each directory is to provide a minimum working example specific to a MCU. Any user should be able to copy a directory,
compile, and flash to the target MCU.

Each individual directory should contain the following:
	
	-lib (directory) 
		- Custom low level modules specific to the MCU.

	-ref (directory)
		- Datasheets / reference manuals specific to the MCU.

	-src (directory)
		- A directory for application specific source code which could / will be modified
		- A file main.c which contains a minimum working "blink" example
		- Startup files should also be contained here

	-Makefile - Containing the minimum commands
		-make 
		-make all
		-make clean

	-Linker script

	-README. This should detail
		- A copy of the above directory / file descriptions.
		- Any other directories present, their purpose and a brief description on how their contents can be obtained.
		- The name and last known working version of the compiler required to build the minimum working example.
		- What software is requried to flash to the MCU and brief instructions on how to do it.

	
	
	
	
	

	
