/*
    Dependencies:
    sudo apt-get update
    sudo apt-get install gpiod libgpiod-dev

    Description:
    The DigitalPin class abstracts the operations of GPIO pins using the libgpiod library.
    This class provides functionality to configure a GPIO pin as input or output and perform read/write operations.
*/

#ifndef DIGITALPIN_H
#define DIGITALPIN_H

#include <gpiod.h>
#include <string>
#include <mutex>
#include <stdexcept>
#include <cstring> // For std::strerror

class DigitalPin {
public:
    // Enum to represent the direction of GPIO operations.
    enum class Direction {
        Input,
        Output
    };

    // Constructor to initialize a GPIO pin.
    DigitalPin(int pinNumber, Direction dir, const std::string& name = "");
    // Destructor to clean up GPIO resources.
    ~DigitalPin();

    // Reads the current value of the GPIO pin. Throws if the pin is configured as output.
    bool read() const;
    // Writes a value to the GPIO pin. Throws if the pin is configured as input.
    void write(bool value);
    // Returns the name of the pin.
    std::string getName() const;

private:
    int pinNumber_;
    Direction direction_;
    std::string name_;
    gpiod_chip* chip_;
    gpiod_line* line_;
    mutable std::mutex mutex_;  // Mutex to synchronize access in multi-threaded environments.
};

#endif // DIGITALPIN_H

#include "DigitalPin.h"

DigitalPin::DigitalPin(int pinNumber, Direction dir, const std::string& name)
    : pinNumber_(pinNumber), direction_(dir),
      name_(name.empty() ? "Pin" + std::to_string(pinNumber) : name) {
    chip_ = gpiod_chip_open_by_number(0);  // Open the default GPIO chip (chip0)
    if (!chip_) {
        throw std::runtime_error("Failed to open GPIO chip: " + std::string(std::strerror(errno)));
    }

    line_ = gpiod_chip_get_line(chip_, pinNumber_);
    if (!line_) {
        gpiod_chip_close(chip_);
        throw std::runtime_error("Failed to access GPIO line for pin number " + std::to_string(pinNumber_));
    }

    gpiod_line_request_config config;
    config.request_type = (dir == Direction::Input) ? GPIOD_LINE_REQUEST_DIRECTION_INPUT
                                                    : GPIOD_LINE_REQUEST_DIRECTION_OUTPUT;
    config.consumer = "DigitalPin";
    config.flags = 0;

    if (gpiod_line_request(line_, &config, 0) != 0) {
        gpiod_chip_close(chip_);
        throw std::runtime_error("Failed to configure GPIO pin: " + std::string(std::strerror(errno)));
    }
}

DigitalPin::~DigitalPin() {
    if (line_) {
        gpiod_line_release(line_);
    }
    if (chip_) {
        gpiod_chip_close(chip_);
    }
}

bool DigitalPin::read() const {
    std::lock_guard<std::mutex> lock(mutex_);
    if (direction_ != Direction::Input) {
        throw std::runtime_error("Attempted to read from an output-configured pin.");
    }
    return gpiod_line_get_value(line_) > 0;
}

void DigitalPin::write(bool value) {
    std::lock_guard<std::mutex> lock(mutex_);
    if (direction_ != Direction::Output) {
        throw std::runtime_error("Attempted to write to an input-configured pin.");
    }
    gpiod_line_set_value(line_, value ? 1 : 0);
}

std::string DigitalPin::getName() const {
    return name_;
}
