#Documentation

This directory contains the main Doxygen generated documentation for the globLib project. 
The documentation covers the common library and each microcontroller module available.

The repository needs to be stored on the local machine to view the Doxygen documentation.
The documentation can be opened at Doxygen/html/index.html or with the make recipe 
**make libdoc** in any project template folder.

## Recompiling the Documentation

Recompiling the documentation requires a local version of Doxygen. This can be obtained from
https://www.stack.nl/~dimitri/doxygen/manual/install.html or possibly using the package
manager 

The documentation for the entire project can be build by running **make** from this directory,
or removed with **make clean**

## Datasheets

Any relevant datasheets should be contained in the doc folder for the respective hardware. 
