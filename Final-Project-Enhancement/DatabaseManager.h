/**
 * @file DatabaseManager.h
 * @author Diego M Bendezú Robalino
 * @brief Header definition for SQLite database management layer.
 *
 * @details This header defines the DatabaseManager class architecture, which encapsulates low-level
 * SQLite database operations (data access layer). It handles table creation, database schema initialization,
 * persistent CRUD operations for client records, auto-increment identifier generation, and secure,
 * parameterized user credential verification.
 */

#ifndef DATABASE_MANAGER_H
#define DATABASE_MANAGER_H

#include <string>
#include <vector>
#include "Client.h"
#include "sqlite3.h"

 /**
  * @class DatabaseManager
  *
  * @brief Manages connection lifecycle, SQL query execution, and data persistence for the application.
  */
class DatabaseManager {
private:
    sqlite3* db;                         // Pointer reference to active SQLite database connection instance.
    std::string dbPath;                  // File path string pointing to local SQLite database file.

public:
    // =============================================
    // Constructors & Destructor
    // =============================================

    /**
     * @brief Parameterized constructor opening target SQLite database file connection.
     *
     * @param databaseName String containing path to SQLite database file (defaults to "investment_portal.db").
     */
    explicit DatabaseManager(const std::string& databaseName = "investment_portal.db");

    /**
     * @brief Destructor closing open SQLite database handles cleanly upon object destruction.
     */
    ~DatabaseManager();


    // =============================================
    // Database Initialization Operations
    // =============================================

    /**
     * @brief Creates required database tables (CLIENTS and USERS) if they do not exist.
     *
     * @return true if schema creation commands executed successfully; false otherwise.
     */
    bool initializeDatabase();

    /**
     * @brief Seeds default client and user administrative accounts if database tables are empty.
     *
     * @return true if initial population succeeded or tables were non-empty; false on error.
     */
    bool seedInitialDataIfEmpty();

    
    // =============================================
    // Client Table Operations
    // =============================================

    /**
     * @brief Loads all client records from persistent storage into a vector list.
     *
     * @param clientList Reference to target vector populated with retrieved Client objects.
     * @return true if data extraction executed successfully; false on query failure.
     */
    bool loadAllClients(std::vector<Client>& clientList);

    /**
     * @brief Updates the service selection tier for an existing client record by unique ID.
     *
     * @param clientId Target account numerical identifier.
     * @param newService Updated service selection tier integer code (1 or 2).
     * @return true if update statement succeeded; false on execution failure.
     */
    bool updateClientServiceChoice(int clientId, int newService);

    /**
     * @brief Inserts a new client record into the CLIENTS database table.
     *
     * @param newClient Constant reference to Client instance to persist.
     * @return true if record insertion succeeded; false on execution error.
     */
    bool addClientRecord(const Client& newClient);

    /**
     * @brief Removes a client record matching the specified identifier from storage.
     *
     * @param clientId Target account numerical identifier to purge.
     * @return true if deletion statement executed and affected rows > 0; false otherwise.
     */
    bool deleteClientRecordById(int clientId);

    /**
     * @brief Queries database to determine next available incremental primary key identifier.
     *
     * @return int Next unique account identifier integer.
     */
    int getNextAvailableId();


    // =========================================================================
    // User Authentication Database Methods
    // =========================================================================

    /**
     * @brief Verifies whether input credentials match a valid entry in the USERS table.
     *
     * @details Employs parameterized SQLite statements to securely evaluate username and
     * password inputs against stored user records while preventing SQL injection vulnerabilities.
     *
     * @param inputUsername Username string entered by user.
     * @param inputPassword Password string entered by user.
     * @return true if matching record is located and credentials match; false otherwise.
     */
    bool validateUserCredentials(const std::string& inputUsername, const std::string& inputPassword);
};

#endif // DATABASE_MANAGER_H