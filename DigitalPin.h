/*
    DigitalPin.h

    Provides a thread-safe and robust interface for managing digital GPIO pins on Linux systems
    using the libgpiod library. This class allows easy configuration of GPIO pins as either inputs
    or outputs and supports read and write operations with proper error handling and thread safety.

    Dependencies: libgpiod and libgpiod-dev
    Install with: sudo apt-get install gpiod libgpiod-dev

    Author: Kevin Fox
    Date: 20 MAY 2024
    License: MIT
*/

#ifndef DIGITALPIN_H
#define DIGITALPIN_H

#include <gpiod.h>
#include <string>
#include <mutex>

class DigitalPin {
public:
    // Enum to define the direction of the GPIO pin
    enum class Direction {
        Input,
        Output
    };

    /**
     * Constructor for DigitalPin.
     * Initializes a GPIO pin with the specified direction and optional name.
     * 
     * @param pinNumber The GPIO pin number.
     * @param dir The direction of the GPIO pin (Input or Output).
     * @param name An optional descriptive name for the pin.
     * @throws std::runtime_error if the pin initialization fails.
     */
    DigitalPin(int pinNumber, Direction dir, const std::string& name = "");

    /**
     * Destructor for DigitalPin.
     * Releases the GPIO pin and closes the GPIO chip connection.
     */
    ~DigitalPin();

    /**
     * Reads the current value of the GPIO pin configured as input.
     * 
     * @return True if the pin is high, false if low.
     * @throws std::runtime_error if attempted on an output-configured pin.
     */
    bool read() const;

    /**
     * Writes a value to the GPIO pin configured as output.
     * 
     * @param value The boolean value to write to the pin (true for high, false for low).
     * @throws std::runtime_error if attempted on an input-configured pin.
     */
    void write(bool value);

    /**
     * Gets the name of the GPIO pin.
     * 
     * @return The name of the pin.
     */
    std::string getName() const;

private:
    int pinNumber_;                  // GPIO pin number
    Direction direction_;            // Direction of the GPIO pin
    std::string name_;               // Optional descriptive name for the GPIO pin
    gpiod_chip* chip_;               // Pointer to the GPIO chip
    gpiod_line* line_;               // Pointer to the GPIO line
    mutable std::mutex mutex_;       // Mutex to ensure thread-safe operations
};

#endif // DIGITALPIN_H
