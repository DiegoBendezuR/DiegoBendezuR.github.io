/**
* @file Project1.cpp
* @author Diego M Bendezu Robalino
* 
* @brief Software Reverse Engineering & Binary Reconstruction - Project One
*
* @details This source file contains the C++ code reconstructed through reverse engineering
* of the compiled binary executable Project1.o. The binary was first disassembled
* into assembly language to analyze control flow, stack memory operations, function
* signatures, and embedded string data.
* 
* This high-level implementation reproduces the original binary structure, which authenticates
* user credentials, displays investment client account selections, and allows updates to
* client service choices.
*/

#include <iostream>
#include <string>


// =================================================
// Global variables identified from binary metadata
// =================================================

// Variables used in CheckUserPermissionAccess()
std::string username;                  // Stores input string for user authentication
std::string password;                  // Stores input string for password authentication
int answer;                            // Stores authentication status (1 = Granted, 2 = Denied)

// Variables used in DisplayInfo()
// Initialized client name strings (padded with spaces for console alignment)
std::string name1 = "Bob Jones   ";
std::string name2 = "Sarah Davis ";
std::string name3 = "Amy Friendly";
std::string name4 = "Johnny Smith";
std::string name5 = "Carol Spears";

// Variables used in DisplayInfo() & ChangeCustomerChoice()
// Service options: 1 = Brokerage, 2 = Retirement
int num1 = 1;
int num2 = 2;
int num3 = 1;
int num4 = 1;
int num5 = 2;

// Variables used in ChangeCustomerChoice()
int changechoice;                 // Stores selected client index (1-5) to modify
int newservice;                   // Stores newly selected service ID (1 or 2)

// Variable used in main()
int choice;                       // Stores main menu navigation choice (1-3)


// ====================
// Function Prototypes
// ====================
int CheckUserPermissionAccess();
void DisplayInfo();
void ChangeCustomerChoice();


int main() {
    // Added banner displaying test credentials for program verification
    std::cout << "--- RECONSTRUCTED PROGRAM (TEST CREDENTIALS) ---" << std::endl;
    std::cout << "  Use Username: admin | Password: Password123   " << std::endl;
    std::cout << "      Created by Diego M Bendezu Robalino       " << std::endl;
    std::cout << "------------------------------------------------" << std::endl << std::endl;

    // Call authentication subroutine and evaluate authorization return code
    int accessResult = CheckUserPermissionAccess();

    // Terminate early if authentication failed (return value != 1)
    if (accessResult != 1) {
        return 0;
    }

    // Display company header upon successful authentication
    std::cout << std::endl << "Hello! Welcome to our Investment Company" << std::endl;

    // Main interactive loop processing user menu inputs
    while (true) {
        // Output menu options for display, modification, or exit
        std::cout << std::endl;
        std::cout << "What would you like to do?" << std::endl;
        std::cout << "DISPLAY the client list (enter 1)" << std::endl;
        std::cout << "CHANGE a client's choice (enter 2)" << std::endl;
        std::cout << "Exit the program.. (enter 3)" << std::endl;
        
        // Read console input into global choice variable
        std::cin >> choice;
        std::cout << "You chose " << choice << std::endl;

        // Route control flow based on selected menu option
        if (choice == 1) {
            DisplayInfo();                        // Print current client roster and services
        }
        else if (choice == 2) {
            ChangeCustomerChoice();               // Modify a specific client's service selection
        }
        else if (choice == 3) {
            break;                                // Break out of main menu loop to exit
        }
    }

    return 0;
}

int CheckUserPermissionAccess() {
    // Prompt the user and read input into the global username string variable
    std::cout << "Enter your username: ";
    std::cin >> username;

    // Prompt the user and read input into the global password string variable
    std::cout << "Enter your password: ";
    std::cin >> password;

    // Authenticate against test credentials
    // Note: 'admin' and 'Password123' are defined test values for reconstruction
    if (username == "admin" && password == "Password123") {
        answer = 1; // Access Granted
    }
    else {
        // Display error message when authentication fails
        std::cout << "Invalid Password. Please try again" << std::endl;
        answer = 2; // Access Denied / Invalid
    }

    // Returns status code
    return answer;
}

void DisplayInfo() {
    // Print the column headers for the table
    std::cout << "Client's Name     Service Selected (1 = Brokerage, 2 = Retirement)" << std::endl;

    // Print each client's name alongside their current service number
    // Blank spaces are hardcoded to keep the data aligned neatly under the headers
    std::cout << name1 << "         " << num1 << std::endl;
    std::cout << name2 << "         " << num2 << std::endl;
    std::cout << name3 << "         " << num3 << std::endl;
    std::cout << name4 << "         " << num4 << std::endl;
    std::cout << name5 << "         " << num5 << std::endl;
}

void ChangeCustomerChoice() {
    // Prompt the user to pick which client record (1 through 5) to update
    std::cout << "Enter the number of the client that you wish to change" << std::endl;
    std::cin >> changechoice;

    // Print the user's selected client number back to the console
    std::cout << "selected option " << changechoice << std::endl;

    // Prompt the user to enter the new service type
    std::cout << "Please enter the client's new service choice (1 = Brokerage, 2 = Retirement)" << std::endl;
    std::cin >> newservice;

    // Map the selected client number to its global variable and assign the new service value
    switch (changechoice) {
    case 1:
        num1 = newservice;         // Update Bob Jones' service selection
        break;
    case 2:
        num2 = newservice;         // Update Sarah Davis' service selection
        break;
    case 3:
        num3 = newservice;         // Update Any Friendly's service selection
        break;
    case 4:
        num4 = newservice;         // Update Johnny Smith's service selection
        break;
    case 5:
        num5 = newservice;         // Update Carol Spears' service selection
        break;
    default:
        // Do nothing if the user enters a client number oustside 1-5
        break;
    }
}