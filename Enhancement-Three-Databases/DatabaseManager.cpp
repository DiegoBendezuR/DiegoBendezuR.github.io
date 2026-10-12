/**
 * @file DatabaseManager.cpp
 * @author Diego M Bendezú Robalino
 * @brief Implementation of SQLite database operations including persistent authentication and CRUD routines.
 *
 * @details Implements low-level SQLite C-API database management, schema creation, seed initialization,
 * parameterized SQL security features, and persistent storage abstractions for both client records and user credentials.
 */

#include "DatabaseManager.h"
#include <iostream>

 /**
  * @brief Constructs a DatabaseManager instance and initializes connection to SQLite persistent storage.
  * Evaluates connection success status and logs errors if file handles cannot be instantiated.
  *
  * @param databaseName String file path or name representing target SQLite database file.
  */
DatabaseManager::DatabaseManager(const std::string& databaseName) : db(nullptr), dbPath(databaseName) {
    int rc = sqlite3_open(dbPath.c_str(), &db);       // Attempt connection open handle to SQLite database file

    // Evaluate connection return code to verify handle initialization success
    if (rc != SQLITE_OK) {
        std::cerr << "Database Connection Error: " << sqlite3_errmsg(db) << std::endl;
        db = nullptr;                                 // Reset pointer handle to null state upon connection failure
    }
}

/**
 * @brief Destructor releasing SQLite database memory structures and closing file connections safely.
 */
DatabaseManager::~DatabaseManager() {

    // Verify active pointer instance exists prior to closing storage stream
    if (db != nullptr) {
        sqlite3_close(db);
        db = nullptr;                                  // Clear pointer handle to eliminate dangling memory reference
    }
}

/**
 * @brief Initializes database table schemas for client data and user authentication credentials.
 * Executes DDL SQL queries to enforce relational structure, constraints, and primary keys.
 *
 * @return true if database structure was created or verified successfully; false if error occurs.
 */
bool DatabaseManager::initializeDatabase() {

    // Guard clause verifying active database handle before executing DDL queries
    if (db == nullptr) return false;

    // Create CLIENTS table schema defining primary keys and mandatory attributes
    const char* createClientsTableSql =
        "CREATE TABLE IF NOT EXISTS CLIENTS ("
        "ID INTEGER PRIMARY KEY NOT NULL, "
        "NAME TEXT NOT NULL, "
        "SERVICE_CHOICE INTEGER NOT NULL);";

    // Create USERS authentication table schema for secure credential management
    const char* createUsersTableSql =
        "CREATE TABLE IF NOT EXISTS USERS ("
        "USERNAME TEXT PRIMARY KEY NOT NULL, "
        "PASSWORD TEXT NOT NULL);";

    // Pointer buffer variable initialized to capture C-style string error execution logs
    char* errorMessage = nullptr;
    
    // Execute table instantiation statement for client records
    if (sqlite3_exec(db, createClientsTableSql, nullptr, nullptr, &errorMessage) != SQLITE_OK) {
        std::cerr << "SQL Schema Error (CLIENTS): " << errorMessage << std::endl;         // Log table creation exception error message
        sqlite3_free(errorMessage);
        return false;
    }

    // Execute table instantiation statement for system user records
    if (sqlite3_exec(db, createUsersTableSql, nullptr, nullptr, &errorMessage) != SQLITE_OK) {
        std::cerr << "SQL Schema Error (USERS): " << errorMessage << std::endl;            // Log schema creation exception error message
        sqlite3_free(errorMessage);
        return false;
    }

    return true;
}

/**
 * @brief Seeds default client records and baseline administrator login credentials if tables are empty.
 * Prevents duplicate seed insertion by evaluating row count aggregate functions prior to insertion.
 *
 * @return true if seeding logic evaluated and executed without stream errors; false otherwise.
 */
bool DatabaseManager::seedInitialDataIfEmpty() {

    // Guard clause ensuring active database connection before checking table row counts
    if (db == nullptr) return false;

    // Evaluate row count within CLIENTS table to check for empty state
    const char* countClientsSql = "SELECT COUNT(*) FROM CLIENTS;";
    sqlite3_stmt* stmt = nullptr;
    int clientCount = 0;

    // Prepare SQL count statement handle for client table query execution
    if (sqlite3_prepare_v2(db, countClientsSql, -1, &stmt, nullptr) == SQLITE_OK) {

        // Step query execution forward to extract single aggregate row count value
        if (sqlite3_step(stmt) == SQLITE_ROW) {
            clientCount = sqlite3_column_int(stmt, 0);
        }
    }
    sqlite3_finalize(stmt);                   // Deallocate prepared query statement resources from system memory

    // Populate CLIENTS table with initial default baseline dataset if empty
    if (clientCount == 0) {
        const char* insertClientSql = "INSERT INTO CLIENTS (ID, NAME, SERVICE_CHOICE) VALUES (?, ?, ?);";
        struct DefaultRecord { int id; const char* name; int service; };
        DefaultRecord defaults[] = {
            {1, "Bob Jones", 1},
            {2, "Sarah Davis", 2},
            {3, "Amy Friendly", 1},
            {4, "Johnny Smith", 1},
            {5, "Carol Spears", 2}
        };

        // Iterate through baseline array to seed each default client record
        for (const auto& rec : defaults) {
            sqlite3_stmt* insertStmt = nullptr;                    // Local statement handle for individual insertion step

            // Compile prepared statement safely prior to value parameter binding
            if (sqlite3_prepare_v2(db, insertClientSql, -1, &insertStmt, nullptr) == SQLITE_OK) {
                sqlite3_bind_int(insertStmt, 1, rec.id);
                sqlite3_bind_text(insertStmt, 2, rec.name, -1, SQLITE_STATIC);
                sqlite3_bind_int(insertStmt, 3, rec.service);
                sqlite3_step(insertStmt);
            }
            sqlite3_finalize(insertStmt);                          // Release statement allocation resource after record step completes
        }
    }

    // Evaluate row count within USERS table to check for empty state
    const char* countUsersSql = "SELECT COUNT(*) FROM USERS;";
    stmt = nullptr;
    int userCount = 0;

    // Compile count statement handle to evaluate USERS credential table state
    if (sqlite3_prepare_v2(db, countUsersSql, -1, &stmt, nullptr) == SQLITE_OK) {

        // Execute query step to fetch row count total from first column output
        if (sqlite3_step(stmt) == SQLITE_ROW) {
            userCount = sqlite3_column_int(stmt, 0);
        }
    }
    sqlite3_finalize(stmt);                                         // Deallocate prepared statement statement handle from memory

    // Populate USERS table with initial default administrator credentials if empty
    if (userCount == 0) {
        const char* insertUserSql = "INSERT INTO USERS (USERNAME, PASSWORD) VALUES (?, ?);";
        sqlite3_stmt* userStmt = nullptr;

        // Compile query statement to prepare for admin credential parameter binding
        if (sqlite3_prepare_v2(db, insertUserSql, -1, &userStmt, nullptr) == SQLITE_OK) {
            sqlite3_bind_text(userStmt, 1, "admin", -1, SQLITE_STATIC);
            sqlite3_bind_text(userStmt, 2, "Password123", -1, SQLITE_STATIC);
            sqlite3_step(userStmt);
        }
        sqlite3_finalize(userStmt);                                  // Free statement handle after admin insertion completes
    }

    return true;
}

/**
 * @brief Validates user credentials against database authentication records using parameterized queries.
 * Protects against SQL injection security vulnerabilities by utilizing prepared statement binding.
 *
 * @param inputUsername Account username string provided by user interface.
 * @param inputPassword Account password string provided by user interface.
 * @return true if credentials match persistent storage records; false otherwise.
 */
bool DatabaseManager::validateUserCredentials(const std::string& inputUsername, const std::string& inputPassword) {

    // Guard clause ensuring active database connection handle exists before performing authentication
    if (db == nullptr) return false;

    // Prepared statement query isolates query logic from user input payloads
    const char* selectUserSql = "SELECT PASSWORD FROM USERS WHERE USERNAME = ?;";
    sqlite3_stmt* stmt = nullptr;
    bool isValid = false;

    // Compile secure parameterized SELECT query to prevent SQL injection vulnerabilities
    if (sqlite3_prepare_v2(db, selectUserSql, -1, &stmt, nullptr) == SQLITE_OK) {
        sqlite3_bind_text(stmt, 1, inputUsername.c_str(), -1, SQLITE_TRANSIENT);

        // Step query execution forward to inspect matching user row
        if (sqlite3_step(stmt) == SQLITE_ROW) {
            const unsigned char* storedPasswordText = sqlite3_column_text(stmt, 0);

            // Ensure non-null pointer response before parsing retrieved password payload
            if (storedPasswordText != nullptr) {
                std::string storedPassword = reinterpret_cast<const char*>(storedPasswordText);

                // Perform exact string match comparison against stored persistent password
                if (storedPassword == inputPassword) {
                    isValid = true;
                }
            }
        }
    }

    sqlite3_finalize(stmt);                                // Clean up compiled statement allocation to prevent memory leaks
    return isValid;
}

/**
 * @brief Retrieves all active client records from persistent SQLite storage into a vector list.
 *
 * @param clientList Reference vector populated with instantiated Client objects from query results.
 * @return true if query execution completed successfully; false if statement fails.
 */
bool DatabaseManager::loadAllClients(std::vector<Client>& clientList) {

    // Guard clause ensuring active database connection handle exists prior to running queries
    if (db == nullptr) return false;
    clientList.clear();
    const char* selectSql = "SELECT ID, NAME, SERVICE_CHOICE FROM CLIENTS;";
    sqlite3_stmt* stmt = nullptr;

    // Compile SELECT query to fetch all client records from persistent storage
    if (sqlite3_prepare_v2(db, selectSql, -1, &stmt, nullptr) != SQLITE_OK) {
        return false;
    }

    // Step through query cursor and deserialize SQLite rows into object structures
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        int id = sqlite3_column_int(stmt, 0);
        const unsigned char* nameText = sqlite3_column_text(stmt, 1);
        int serviceChoice = sqlite3_column_int(stmt, 2);
        std::string name = nameText ? reinterpret_cast<const char*>(nameText) : "";
        clientList.emplace_back(id, name, serviceChoice);                     // Construct and append Client object directly into vector
    }

    sqlite3_finalize(stmt);                      // Deallocate statement memory allocation after iteration finishes
    return true;
}

/**
 * @brief Updates a targeted client's service tier selection code within persistent storage.
 *
 * @param clientId Primary key numerical identifier of target client record.
 * @param newService Updated service tier integer code (1 = Brokerage, 2 = Retirement).
 * @return true if table update query finalized successfully; false otherwise.
 */
bool DatabaseManager::updateClientServiceChoice(int clientId, int newService) {

    // Guard clause ensuring active database connection handle exists before attempting UPDATE operation
    if (db == nullptr) return false;
    const char* updateSql = "UPDATE CLIENTS SET SERVICE_CHOICE = ? WHERE ID = ?;";
    sqlite3_stmt* stmt = nullptr;

    // Prepare update query statement with positional parameter placeholders
    if (sqlite3_prepare_v2(db, updateSql, -1, &stmt, nullptr) != SQLITE_OK) {
        return false;
    }

    sqlite3_bind_int(stmt, 1, newService);                    // Bind target new service tier integer value to first query parameter
    sqlite3_bind_int(stmt, 2, clientId);                      // Bind targeted client primary key identifier to second query parameter

    int result = sqlite3_step(stmt);
    sqlite3_finalize(stmt);
    return (result == SQLITE_DONE);
}

/**
 * @brief Computes the next available primary key client identifier using SQL aggregation.
 *
 * @return int Next unique integer identifier for client assignment.
 */
int DatabaseManager::getNextAvailableId() {

    // Guard clause returning baseline ID fallback if database connection handle is invalid
    if (db == nullptr) return 1;
    const char* maxSql = "SELECT MAX(ID) FROM CLIENTS;";
    sqlite3_stmt* stmt = nullptr;
    int maxId = 0;

    // Prepare aggregate function query statement to compute highest existing primary key
    if (sqlite3_prepare_v2(db, maxSql, -1, &stmt, nullptr) == SQLITE_OK) {

        // Step query execution forward to extract maximum primary key aggregate result
        if (sqlite3_step(stmt) == SQLITE_ROW) {
            maxId = sqlite3_column_int(stmt, 0);
        }
    }
    sqlite3_finalize(stmt);                        // Free statement resources following aggregate computation execution
    return maxId + 1;
}

/**
 * @brief Inserts a new client domain record into persistent SQLite storage.
 *
 * @param newClient Client instance containing record attributes to insert.
 * @return true if transaction insertion statement finalized successfully; false otherwise.
 */
bool DatabaseManager::addClientRecord(const Client& newClient) {

    // Guard clause verifying database handle connection before executing record insertion query
    if (db == nullptr) return false;
    const char* insertSql = "INSERT INTO CLIENTS (ID, NAME, SERVICE_CHOICE) VALUES (?, ?, ?);";
    sqlite3_stmt* stmt = nullptr;

    // Prepare SQL insertion statement with positional parameter placeholders
    if (sqlite3_prepare_v2(db, insertSql, -1, &stmt, nullptr) != SQLITE_OK) {
        return false;
    }

    sqlite3_bind_int(stmt, 1, newClient.getId());                                            // Bind generated unique primary key ID to first parameter
    sqlite3_bind_text(stmt, 2, newClient.getName().c_str(), -1, SQLITE_TRANSIENT);           // Bind client full name string payload to second parameter
    sqlite3_bind_int(stmt, 3, newClient.getServiceChoice());                                 // Bind service selection tier integer code to third parameter

    // Step statement execution to process SQL insertion query into persistent table
    int result = sqlite3_step(stmt);
    sqlite3_finalize(stmt);
    return (result == SQLITE_DONE);
}

/**
 * @brief Deletes a target client record from persistent database storage using primary key matching.
 *
 * @param clientId Primary key identifier of client record to remove.
 * @return true if deletion executed and affected one or more database rows; false otherwise.
 */
bool DatabaseManager::deleteClientRecordById(int clientId) {

    // Guard clause verifying active database connection before executing deletion query
    if (db == nullptr) return false;
    const char* deleteSql = "DELETE FROM CLIENTS WHERE ID = ?;";
    sqlite3_stmt* stmt = nullptr;

    // Prepare parameter deletion statement to guarantee primary key row removal
    if (sqlite3_prepare_v2(db, deleteSql, -1, &stmt, nullptr) != SQLITE_OK) {
        return false;
    }

    // Bind primary key parameter integer to target specific client row for deletion
    sqlite3_bind_int(stmt, 1, clientId);
    int result = sqlite3_step(stmt);
    sqlite3_finalize(stmt);
    return (result == SQLITE_DONE && sqlite3_changes(db) > 0);
}