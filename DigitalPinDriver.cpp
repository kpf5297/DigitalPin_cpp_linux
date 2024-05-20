/**
 * @file DigitalPinDriver.cpp
 * @brief Driver for demonstrating the usage of the DigitalPin class.
 *
 * This driver application provides a command-line interface to control GPIO pins 
 * on a Linux system using the DigitalPin class. It allows the user to toggle an 
 * output pin and read the state of an input pin interactively.
 *
 * Usage:
 * - '1': Turn on the output pin (e.g., light up an LED).
 * - '0': Turn off the output pin.
 * - 'r': Read and display the state of the input pin (e.g., a button).
 * - 'q': Quit the application.
 *
 * @author Kevin Fox
 * @date May 20, 2024
 * @location Pittsburgh, PA
 *
 * Compilation:
 * This file should be compiled with the DigitalPin class implementation and linked 
 * against the libgpiod library. Example GCC command:
 * g++ -o DigitalPinDriver DigitalPinDriver.cpp DigitalPin.cpp -lgpiod
 *
 * Run:
 * ./DigitalPinDriver
 *
 * This program is dependent on the libgpiod library for GPIO manipulation. Ensure 
 * libgpiod and its development headers are installed on your system.
 */
#include "DigitalPin.h"
#include <iostream>
#include <string>

int main() {
    std::cout << "DigitalPin Driver Application" << std::endl;

    try {
        // Initialize a DigitalPin for output (e.g., LED, Relay)
        DigitalPin outputPin(17, DigitalPin::Direction::Output, "LED");

        // Initialize a DigitalPin for input (e.g., Button)
        DigitalPin inputPin(27, DigitalPin::Direction::Input, "Button");

        std::cout << "Setup complete. Press '1' to turn on the LED, '0' to turn off, and 'r' to read the button state. Press 'q' to quit." << std::endl;

        char command;
        bool running = true;

        while (running) {
            std::cin >> command;
            switch (command) {
                case '1':
                    outputPin.write(true);
                    std::cout << "LED turned ON." << std::endl;
                    break;
                case '0':
                    outputPin.write(false);
                    std::cout << "LED turned OFF." << std::endl;
                    break;
                case 'r':
                    std::cout << "Button state: " << (inputPin.read() ? "Pressed" : "Not pressed") << std::endl;
                    break;
                case 'q':
                    running = false;
                    break;
                default:
                    std::cout << "Invalid command." << std::endl;
            }
        }
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }

    std::cout << "Exiting DigitalPin Driver Application." << std::endl;
    return 0;
}
