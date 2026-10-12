/**
 * @file AuthenticationManager.cpp
 * @author Diego M Bendezú Robalino
 * @brief Encapsulates user login authentication and hides credential variables from global static memory.
 * 
 * @details Implements the AuthenticationManager class methods. Replaces unencapsulated global string
 * comparisons with localized, class-bound identity verification logic. Credential validation takes place 
 * entirely within stack-allocated temporary memory spaces during execution, mitigating persistent memory exposure.
 */

#include "AuthenticationManager.h"
#include <iostream>
#include <limits>

 /**
  * @brief Constructs the AuthenticationManager object and assigns default credential attributes.
  *
  * @details Assigns default administrative access values to private instance members upon instantiation.
  * Moving variables out of the global scope isolates access to explicit class interface methods.
  */
AuthenticationManager::AuthenticationManager() {

    // Encapsulate default administrative credentials inside class scope.
    storedUsername = "admin";                     // Internal default administrative username.
    storedPassword = "Password123";               // Internal default administrative password.
}

/**
 * @brief Interactively prompts the user for login credentials and performs security evaluation.
 *
 * @details Reads console input into stack-allocated local string buffers. Evaluates matching status
 * against encapsulated instance variables using standard string comparisons. If validation fails, 
 * a generic error message is emitted to prevent credential enumeration attacks.
 *
 * @return bool Returns true if authentication succeeds, false if credentials do not match.
 */
bool AuthenticationManager::authenticateUser() {

    // Local stack-allocated string buffers for processing transient logic input.
    std::string inputUsername;
    std::string inputPassword;

    // Prompt user and read input string into localized stack buffer.
    std::cout << "Enter your username: ";
    std::cin >> inputUsername;

    // Prompt user and read input string into localized stack buffer.
    std::cout << "Enter your password: ";
    std::cin >> inputPassword;

    // Evaluate credentials in an isolated local execution scope.
    if (inputUsername == storedUsername && inputPassword == storedPassword) {
        // Logically verify identity and return true to indicate successful access authorization.
        return true;
    }
    else {
        // Generic failure notification prevents user enumeration attacks.
        // Discloses no specific detail regarding whether username or password was incorrect.
        std::cout << "Invalid credentials. Please try again." << std::endl;

        // Return false to signal authorization failure to calling module.
        return false;
    }
}