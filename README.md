# DigitalPin Class for GPIO Management

## Overview
The `DigitalPin` class provides a robust and thread-safe interface for managing digital GPIO pins on Linux systems. It utilizes the libgpiod library to facilitate both input and output operations, ensuring ease of use and effective management of GPIO resources.

## Features
- **Thread Safety**: Ensures safe use in multi-threaded applications.
- **Error Handling**: Robust error handling with clear exception messages.
- **Ease of Use**: Simplifies the GPIO management with straightforward functions for pin reading and writing.

## Dependencies
This class requires the libgpiod library. Follow these steps to install the necessary dependencies on a Linux system:

```bash
sudo apt-get update
sudo apt-get install gpiod libgpiod-dev
```

## Installation
To use the `DigitalPin` class in your projects, clone this repository to your local machine:

```bash
git clone https://github.com/kpf5297/DigitalPin_cpp_linux.git
```

Navigate to the repository directory:

```bash
cd DigitalPin
```

Include the `DigitalPin.h` and `DigitalPin.cpp` files in your project.

## Usage
To initialize a digital pin, include the header in your project and create an instance of the `DigitalPin`:

```cpp
#include "DigitalPin.h"

// Create a DigitalPin object for GPIO pin 27 as output
DigitalPin pin(27, DigitalPin::Direction::Output, "MyOutputPin");

// Write to the pin
pin.write(true);

// Read from the pin (if configured as input)
bool pinState = pin.read();
```
## License
This project is licensed under the MIT License - see the [LICENSE.md](LICENSE.md) file for details.
