/**
 * @file AuthenticationManager.h
 * @author Diego M Bendezú Robalino
 * @brief Header definition for user authentication handling via database storage.
 *
 * @details This header defines the AuthenticationManager class architecture, which manages
 * user authentication routines and access control. It encapsulates the validation logic
 * and decouples console user interaction from underlying SQLite database credential operations.
 */

#ifndef AUTHENTICATION_MANAGER_H
#define AUTHENTICATION_MANAGER_H

#include <string>
#include "DatabaseManager.h"

 /**
  * @class AuthenticationManager
  *
  * @brief Manages system access control and credential verification against the database.
  */
class AuthenticationManager {
private:
    DatabaseManager dbManager;                // Database abstraction layer instance for executing security queries.

public:
    // ==========================================
    // Constructors
    // ==========================================

    /**
     * @brief Constructs an AuthenticationManager instance and initializes database resources.
     */
    AuthenticationManager();


    // ==========================================
    // Operational Methods
    // ==========================================

    /**
     * @brief Interactively prompts user for credentials and verifies them against SQLite data.
     *
     * @details Prompts the console operator for username and password inputs, enforces maximum
     * attempt limits, and queries the database via parameterized SQL statements to grant access.
     *
     * @return true If input credentials match a valid stored user record; false otherwise.
     */
    bool authenticateUser();
};

#endif // AUTHENTICATION_MANAGER_H