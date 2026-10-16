/**
 * @file AuthenticationManager.cpp
 * @author Diego M Bendezú Robalino
 * @brief Implementation of database-driven user authentication workflows.
 *
 * @details Implements credential validation routines, authentication loop logic,
 * stream safety flushes, and database connections to handle persistent multi-user sign-ins
 * without relying on hardcoded credentials or scalar variables.
 */

#include "AuthenticationManager.h"
#include <iostream>
#include <limits>
#include <conio.h>                                  // Required for _getch() on Windows

 /**
  * @brief Constructs an AuthenticationManager instance and binds the persistent storage layer.
  * Initializes the target SQLite database file and executes initial schema setup and seeding routines.
  */
AuthenticationManager::AuthenticationManager() : dbManager("investment_portal.db") {

    // Verify database initialization succeeds before executing schema population routines
    if (dbManager.initializeDatabase()) {
        dbManager.seedInitialDataIfEmpty();           // Populates default administrative credentials if USERS table is empty
    }
}

/**
 * @brief Manages the interactive terminal authentication loop for client account access.
 * Prompts the user for account credentials, queries SQLite database tables via parameterized checks,
 * tracks failed attempts, and flushes leftover stream input upon successful authentication.
 *
 * @return true if the credentials match an active record in SQLite; false if max attempts are exceeded.
 */
bool AuthenticationManager::authenticateUser() {

    std::string inputUsername;                     // Temporary buffer holding interactive user account login entry.
    std::string inputPassword;                     // Temporary buffer holding interactive user password authentication entry.
    int maxAttempts = 3;                           // Hard threshold defining maximum allowed credential entry failures.
    int attempts = 0;                              // Counter tracking total authentication attempts in current execution session.

    std::cout << "\n========================================\n";
    std::cout << "         USER AUTHENTICATION            \n";
    std::cout << "========================================\n";

    // Loop continuously until valid credentials are provided or total attempts exceed threshold
    while (attempts < maxAttempts) {
        std::cout << "Enter Username: ";
        std::cin >> inputUsername;
        std::cout << "Enter Password: ";
        inputPassword = "";
        char ch = '\0';               // Variable tracking single character raw buffer input from console.

        
        // =================================================================================
        // Secure Password Masking & Input Capture Loop
        // =================================================================================
        // Iteratively captures raw keystrokes without echoing them to the terminal console 
        // to mitigate shoulder surfing vulnerabilities and protect sensitive credentials.
        // 
        // Reference pattern adapted from: 
        // https://www.studyplan.dev/intro-to-programming/user-input/q/password-input-without-display
        // =================================================================================
        while (true) {
            ch = _getch();                           // Capture single keystroke from console input buffer without screen echo

            if (ch == '\r' || ch == '\n') {          // Enter key terminates password entry sequence
                std::cout << std::endl;
                break;
            }
            else if (ch == '\b' || ch == 127) {      // Backspace key handling
                if (!inputPassword.empty()) {
                    inputPassword.pop_back();
                    std::cout << "\b \b";            // Overwrite and remove last printed asterisk from terminal display
                }
            }
            else if (ch >= 32 && ch <= 126) {        // Filter and accept printable ASCII character range
                inputPassword += ch;
                std::cout << '*';                    // Render visual mask character (*) in place of sensitive plaintext
            }
        }

        // Query SQLite database instead of comparing against hardcoded strings
        if (dbManager.validateUserCredentials(inputUsername, inputPassword)) {
            // Clear the leftover newline ('\n') sitting in the input buffer from pressing Enter after entering password
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

            // Notify user of authorized authorization status and return positive validation state
            std::cout << "\nAuthentication successful! Access granted.\n";
            return true;
        }

        attempts++;                                               // Increment failed attempt counter following database rejection.
        int remaining = maxAttempts - attempts;                   // Calculate remaining authentication opportunities prior to lockout.
        std::cout << "Invalid username or password.";

        // Display remaining attempts if security threshold has not been reached
        if (remaining > 0) {
            std::cout << " You have " << remaining << " attempt(s) remaining.\n\n";
        }
        // Terminate access attempt sequence when user exhausts all login opportunities
        else {
            std::cout << " Maximum attempts reached.\n";
        }
    }

    return false;
}