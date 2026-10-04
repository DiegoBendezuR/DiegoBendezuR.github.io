/**
 * @file AuthenticationManager.h
 * @author Diego M Bendezú Robalino
 * @brief Header definition for user authentication handling.
 * 
 * @details This header defines the AuthenticationManager class responsible for validating user login
 * credentials. By transitioning from global variables to an object-oriented architecture,
 * sensitive fields (storedUsername and storedPassword) are strictly encapsulated within private class instance
 * memory rather than residing exposed in global static data segments
 */

#ifndef AUTHENTICATION_MANAGER_H
#define AUTHENTICATION_MANAGER_H

#include <string>

 /**
  * @class AuthenticationManager
  * 
  * @brief Handles identity verification, input authentication, and credential storage.
  */
class AuthenticationManager {
private:

    // =========================================================================
    // ENCAPSULATED MEMBER VARIABLES
    // =========================================================================
    // In the legacy C++ codebase, username and password were stored as unencapsulated 
    // global strings, allowing unrestricted memory inspection and unauthorized external
    // modifications across the execution lifecycle.

    std::string storedUsername;             // Stores the authorized system username in private object scope.
    std::string storedPassword;             // Stores the authorized system password in private object scope.

public:

    // =========================================================================
    // CONSTRUCTORS AND PUBLIC METHODS
    // =========================================================================

    /**
     * @brief Default constructor for AuthenticationManager.
     * Initializes internal authentication state and default login credentials.
     */
    AuthenticationManager();

    /**
     * @brief Prompts for user credentials and verifies them against stored values.
     *
     * @return true if both username and password match authorized credentials; false otherwise.
     */
    bool authenticateUser();
};

#endif // AUTHENTICATION_MANAGER_H