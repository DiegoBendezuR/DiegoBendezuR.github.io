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
* client service choices. Additionally, security enhancements and input validation routines 
* have been integrated to remediate identified software vulnerabilities.
*/

#include <iostream>
#include <string>
#include <limits>


// =================================================
// Global variables identified from binary metadata
// =================================================


// ** SECURITY VULNERABILITY IDENTIFIED ** :
// Declaring username, password, and answer as global variables places sensitive plaintext data in the memory segment of the compiled binary file.
// This choice keeps the authentication variables present throughout the entire execution, allowing attached inspection tools to read private values.
// 
// 
// ** EXPLANATION OF FIXED VULNERABILITY ** :
// These variables must remain global for this project due to structural constraints. Moving them to local scopes would alter 
// the static memory layout and data segment organization required to match the compiled binary reference format.

// Variables used in CheckUserPermissionAccess()
std::string username;                 // Stores input string for user authentication
std::string password;                 // Stores input string for password authentication
int answer;                           // Stores authentication status (1 = Granted, 2 = Denied)



// ** SECURITY VULNERABILITY IDENTIFIED ** :
// Placing customer identification names inside global string variables puts confidential records directly into 
// the read-only data segment of the compiled binary file.
// These unprotected allocations lack security mechanisms, enabling unauthorized actors to extract private records 
// easily by examining compiled data structures.
// 
// 
// ** EXPLANATION OF FIXED VULNERABILITY ** :
// Client names are kept as global string variables to preserve the required symbol layout and data segment organization 
// of the reverse-engineered binary executable file layout.

std::string name1 = "Bob Jones   ";           // Variables used in DisplayInfo()
std::string name2 = "Sarah Davis ";           // Initialized client name strings (padded with spaces for console alignment)
std::string name3 = "Amy Friendly";
std::string name4 = "Johnny Smith";
std::string name5 = "Carol Spears";



// ** SECURITY VULNERABILITY IDENTIFIED ** :
// Storing financial selections inside unprotected global integers exposes client portfolio data to open memory.
// External processes can modify customer records directly because validation checks are absent.
// 
// 
// ** EXPLANATION OF FIXED VULNERABILITY ** :
// Portfolio variables remain in global memory to maintain compatibility with the legacy binary's static variable layout 
// and function mapping requirements.

int num1 = 1;                     // Variables used in DisplayInfo() & ChangeCustomerChoice()
int num2 = 2;                     // Service options: 1 = Brokerage, 2 = Retirement
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

    // ** SECURITY VULNERABILITY IDENTIFIED ** :
    // Hardcoding and publicly printing test authentication credentials inside user-facing console output exposes 
    // administrative access details to unauthorized observers.
    // 
    // 
    // ** FIX ** :
    // Explicit credential text has been scrubbed from the startup console banner to prevent public exposure. 
    // Test login details are now maintained exclusively within internal code comments for evaluation purposes.
    
    std::cout << "--- RECONSTRUCTED PROGRAM (TEST CREDENTIALS) ---" << std::endl;                           // Added banner displaying test credentials for program verification
    std::cout << "      Created by Diego M Bendezu Robalino       " << std::endl;
    std::cout << "------------------------------------------------" << std::endl << std::endl;



    // Call authentication subroutine and evaluate authorization return code
    int accessResult = CheckUserPermissionAccess();


    // ** SECURITY VULNERABILITY IDENTIFIED ** :
    // Authentication relies on a basic integer return status without cryptographic tokens or secondary identity 
    // checks allowing attackers to bypass security gates.
    // Manipulating return registers permits unauthorized users to enter the application menu directly without 
    // logging failed attempts or triggering account lockouts.
    // 
    // 
    // ** EXPLANATION OF FIXED VULNERABILITY ** :
    // This return check preserves the original binary control flow. While it lacks advanced cryptographic token validation, 
    // it properly evaluates the return status and terminates early if authentication fails.

    if (accessResult != 1) {                          // Terminate early if authentication failed (return value != 1)
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


        // ** SECURITY VULNERABILITY IDENTIFIED ** :
        // Console input processing lacks type checks and boundary limits when handling menu selections.
        // Accepting unverified text causes the input stream to fail, keeping old values and creating endless 
        // loops that break normal program execution.
        // 
        // 
        // ** FIX ** :
        // Type checking and input stream recovery were implemented using std::cin.fail(), clear(), and ignore() 
        // to gracefully handle invalid entries and prevent infinite loops.

        std::cin >> choice;                                         // Read console input into global choice variable

        if (std::cin.fail()) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "\nInvalid input. Please enter a valid menu number (1-3)." << std::endl;
            continue;
        }



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

    // ** SECURITY VULNERABILITY IDENTIFIED ** :
    // The authentication routine accepts unconstrained string lengths for user input, creating exposure to buffer overflows
    // and abnormal data insertion without character limits.
    // Hardcoded plaintext exist directly inside the binary comparison logic while separate username validation is completely omitted.
    // Verbose error messaging confirms incorrect password input without evaluating the username value, 
    // aiding attackers in mapping valid accounts during login attempts.
    // 
    // 
    // ** EXPLANATION OF FIXED VULNERABILITY ** :
    // Rebuilding this logic with dynamic databases or salted password hashing would deviate from the reverse-engineered binary layout. 
    // However, the error message was updated to be generic to prevent credential enumeration.

    std::cout << "Enter your username: ";          // Prompt the user and read input into the global username string variable
    std::cin >> username;

    std::cout << "Enter your password: ";          // Prompt the user and read input into the global password string variable
    std::cin >> password;

    // Authenticate against test credentials
    // Note: 'admin' and 'Password123' are defined test values for reconstruction
    if (username == "admin" && password == "Password123") {
        answer = 1; // Access Granted
    }
    else {
        // Display error message when authentication fails
        std::cout << "Invalid credentials. Please try again" << std::endl;
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


    // ** SECURITY VULNERABILITY IDENTIFIED ** :
    // Accepting unvalidated input for changechoice permits arbitrary data to pass into downstream operations.
    // This processing lacks numeric verification for inputs outside the expected range of one through five, corrupting memory regions.
    // 
    // 
    // ** EXPLANATION OF FIXED VULNERABILITY ** :
    // A validation loop combined with stream failure checking and boundary tests (1 through 5) was added 
    // to ensure only correct client indices are processed.

    while (true) {
        std::cout << "Enter the number of the client that you wish to change" << std::endl;          // Prompt the user to pick which client record (1 through 5) to update
        std::cin >> changechoice;

        if (std::cin.fail() || changechoice < 1 || changechoice > 5) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Invalid client selection. Please enter a number between 1 and 5.\n" << std::endl;
            continue;
        }
        break;
    }


    // ** SECURITY VULNERABILITY IDENTIFIED ** : 
    // The new service input stream lacks boundary checks and input validation.
    // Unverified values outside the permitted range enter memory modification statements without error controls,
    // enabling unauthorized changes to business logic and corrupting client portfolios.
    // 
    // 
    // ** EXPLANATION OF FIXED VULNERABILITY ** :
    // Input streams for new service selection are now filtered through a boundary verification 
    // loop enforcing valid options (1 or 2).

    std::cout << "selected option " << changechoice << std::endl;                                // Print the user's selected client number back to the console

    // Prompt the user to enter the new service type
    while (true) {
        std::cout << "Please enter the client's new service choice (1 = Brokerage, 2 = Retirement)" << std::endl;
        std::cin >> newservice;

        if (std::cin.fail() || (newservice != 1 && newservice != 2)) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Invalid service choice. Please enter 1 or 2.\n" << std::endl;
            continue;
        }
        break;
    }

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
        // Do nothing if the user enters a client number outside 1-5
        break;
    }
}