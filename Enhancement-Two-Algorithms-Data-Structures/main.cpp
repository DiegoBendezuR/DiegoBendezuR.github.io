/**
 * @file main.cpp
 * @author Diego M Bendezú Robalino
 * @brief Main entry point for the refactored Investment Application (Milestone Two).
 * 
 * @details This source file serves as the main execution driver for the modernized, 
 * Object-Oriented C++ implementation of the legacy investment portal. The legacy 
 * version relied on insecure global variables, exposed static credential buffers, 
 * unencapsulated global state, and non-modular control flow.
 *
 * The refactored entry point coordinates application startup, delegates access control 
 * to an encapsulated AuthenticationManager object, and drives an event loop managed 
 * through a modular ClientManager instance. Input sanitization routines prevent 
 * input stream corruption and infinite loops.
 */

#include <iostream>
#include <limits>
#include "AuthenticationManager.h"           // Encapsulated authentication controller interface.
#include "ClientManager.h"                   // Encapsulated client collection management interface.

int main() {

    // Render startup application branding banner.
    std::cout << "================================================" << std::endl;
    std::cout << "INVESTMENT FIRM PORTAL (CATEGORY 2 ENHANCEMENT) " << std::endl;
    std::cout << "      Created by Diego M Bendezú Robalino       " << std::endl;
    std::cout << "================================================" << std::endl << std::endl;

    // Instantiate AuthenticationManager to encapsulate identity verification.
    // Eliminates global credential variables previously stored in static memory.
    AuthenticationManager authManager;

    // Evaluate login attempt. Enforce early exit gate if authentication fails.
    if (!authManager.authenticateUser()) {
        std::cout << "Access Denied. Program terminating." << std::endl;
        return 0;            // Terminate execution immediately upon security access failure.
    }

    // Post-authentication user greeting.
    std::cout << "\nHello! Welcome to our Investment Company" << std::endl;

    // Instantiate ClientManager to manage active client data structures and user views.
    ClientManager clientManager;

    // Encapsulate main menu choice variable within local stack scope (replaces legacy global 'choice').
    int choice = 0;

    // Interactive navigation loop for application control flow.
    while (true) {

        // Output interactive main menu choices.
        std::cout << "\nWhat would you like to do?" << std::endl;
        std::cout << "1. DISPLAY the client list" << std::endl;
        std::cout << "2. CHANGE a client's choice" << std::endl;
        std::cout << "3. EXIT the program" << std::endl;
        std::cout << "Selection: ";

        // Read numeric input selection from standard input stream.
        std::cin >> choice;

        // Recover from stream failure when non-integer characters are entered.
        if (std::cin.fail()) {
            std::cin.clear();      // Clear stream failure flags (e.g., badbit, failbit).
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');            // Flush remaining buffer characters.
            std::cout << "\nInvalid input. Please enter a valid menu number (1-3)." << std::endl;
            continue;              // Prompt user again without processing uninitialized data.
        }

        // Route control flow based on verified selection.
        if (choice == 1) {
            clientManager.displayClients();                    // Delegate roster formatting and display to ClientManager.
        }
        else if (choice == 2) {
            clientManager.changeCustomerChoice();              // Delegate client record modification logic.
        }
        else if (choice == 3) {
            std::cout << "Exiting application. Goodbye!\n";    
            break;                                             // Terminate interactive loop.
        }
        else {
            // Out-of-bounds boundary check for numeric inputs outside 1-3.
            std::cout << "Option out of range. Please choose 1, 2, or 3.\n";
        }
    }

    return 0;
}