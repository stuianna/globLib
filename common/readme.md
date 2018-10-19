# Common

This module contains libraries designed to work with all microcontrollers. Each of these are 
already included in project template makefiles and documentation. The documentation for these
modules can be viewed by typing **make libdoc** from any project template and navigating to
common -> modules.

## Usage

Any sub-module can be included in the main application, for example to include the LSM91DS1
IMU sub-module:

```
#include <lsm9ds1.h>
```
The sub-module can then be used as per example.

## Examples

Example usage for each module is located in the examples directory from the globlib main
directory or on each sub-module's mainpage when viewing the Doxygen documentation.
