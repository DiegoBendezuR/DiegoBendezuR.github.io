/**
 * @file main.cpp
 * @author Diego M Bendezú Robalino
 * @brief Main entry point featuring full CRUD interactive menu navigation.
 *
 * @details Serves as the primary execution driver for the application. Coordinates
 * authentication validation via the AuthenticationManager layer before handing off control
 * to the main event loop, which processes user selections and routes workflow logic through
 * the ClientManager system.
 */

#include <iostream>
#include "AuthenticationManager.h"
#include "ClientManager.h"

 /**
  * @brief Main execution function for the Investment Firm Portal application.
  *
  * @return int Exit status code (0 indicating successful execution).
  */
int main() {
    // =========================================================================
    // Program Header & Banner Output
    // =========================================================================
    std::cout << "================================================" << std::endl;
    std::cout << "INVESTMENT FIRM PORTAL (CATEGORY 3 ENHANCEMENT) " << std::endl;
    std::cout << "      Created by Diego M Bendezu Robalino       " << std::endl;
    std::cout << "================================================" << std::endl << std::endl;

    // =========================================================================
    // User Authentication Protocol
    // =========================================================================
    AuthenticationManager authManager;

    // Verify credentials against persistent database storage before granting portal access
    if (!authManager.authenticateUser()) {
        std::cout << "Access Denied. Program terminating." << std::endl;
        return 0;
    }

    // Reset input stream flags prior to entering the line-based menu loop
    std::cin.clear();

    std::cout << "\nHello! Welcome to our Investment Company" << std::endl;

    // =========================================================================
    // Interactive Menu Driver Loop
    // =========================================================================
    ClientManager clientManager;
    int choice = 0;

    while (true) {
        // Render application navigation options
        std::cout << "\nWhat would you like to do?" << std::endl;
        std::cout << "1. DISPLAY the client list" << std::endl;
        std::cout << "2. CHANGE a client's choice" << std::endl;
        std::cout << "3. ADD a new client record" << std::endl;
        std::cout << "4. DELETE a client record" << std::endl;
        std::cout << "5. EXIT the program" << std::endl;

        // Perform strict integer input parsing to guard against trailing character contamination
        if (!ClientManager::getValidIntInput("Selection: ", choice)) {
            std::cout << "\nInvalid input. Please enter a valid menu number (1-5)." << std::endl;
            continue;
        }

        // =========================================================================
        // Menu Option Routing Logic
        // =========================================================================
        if (choice == 1) {
            // Retrieve and render client records with customizable sorting
            clientManager.displayClients();
        }
        else if (choice == 2) {
            // Modify active investment service plan for a target client ID
            clientManager.changeCustomerChoice();
        }
        else if (choice == 3) {
            // Interactively create and persist a new client record into SQLite database
            clientManager.addNewClient();
        }
        else if (choice == 4) {
            // Prompt confirmation and permanently erase client record by unique ID
            clientManager.deleteClient();
        }
        else if (choice == 5) {
            // Gracefully break event loop and terminate application
            std::cout << "Exiting application. Goodbye!\n";
            break;
        }
        else {
            // Re-prompt user if numeric selection falls outside the active option range [1-5]
            std::cout << "Option out of range. Please choose a number between 1 and 5.\n";
        }
    }

    return 0;
}