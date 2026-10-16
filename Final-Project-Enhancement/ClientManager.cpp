/**
 * @file ClientManager.cpp
 * @author Diego M Bendezú Robalino
 * @brief Implementation of the ClientManager controller layer.
 *
 * @details Implements user interface interaction routines, database state synchronization,
 * robust input stream parsing, binary searching, quick sorting algorithms, and full CRUD operations.
 */

#include "ClientManager.h"
#include <iostream>
#include <iomanip>
#include <limits>
#include <algorithm>
#include <sstream>

 // ============================================================================
 // Constructor & Database Synchronization
 // ============================================================================

 /**
  * @brief Constructs a ClientManager instance bound to the SQLite database.
  * Initializes database tables, populates baseline seed records if needed, and
  * hydrates the local vector cache with persisted records.
  */
ClientManager::ClientManager() : dbManager("investment_portal.db") {

    // Ensure database connection and tables are successfully initialized before operations
    if (dbManager.initializeDatabase()) {
        dbManager.seedInitialDataIfEmpty();              // Insert default sample clients if table is empty
        refreshFromDatabase();                           // Populate local vector cache with persisted SQLite records
    }
}

/**
 * @brief Fetches the latest client records from the database layer into the local vector cache.
 */
void ClientManager::refreshFromDatabase() {
    
    dbManager.loadAllClients(clients);                    // Query database and replace local vector content
}


// ============================================================================
// Input Validation & Helper Routines
// ============================================================================

/**
 * @brief Clears error flags and flushes remaining characters from the standard input stream.
 */
void ClientManager::clearInputStream() {

    std::cin.clear();                                                              // Reset error bits on standard input stream
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');            // Discard unread trailing characters until newline
}

/**
 * @brief Reads user input line-by-line to extract a single strictly numeric integer.
 * Prevents trailing non-numeric characters (e.g., "2a") from being partially parsed.
 *
 * @param prompt Display string presented to the user.
 * @param outValue Reference parameter where the valid parsed integer is stored.
 * @return true If input was successfully parsed as a pure integer; false otherwise.
 */
bool ClientManager::getValidIntInput(const std::string& prompt, int& outValue) {

    std::string line;
    std::cout << prompt;

    // Discard unread trailing characters until newline
    if (!std::getline(std::cin, line)) {
        return false;
    }

    // Wrap string line into stream to isolate single token evaluation
    std::stringstream ss(line);
    int tempValue = 0;
    char extraChar = '\0';

    // Verify stream reads exact integer AND contains no trailing leftover characters (e.g. 'a')
    if (ss >> tempValue && !(ss >> extraChar)) {
        outValue = tempValue;
        return true;
    }

    return false;
}


// ============================================================================
// Sorting & Search Algorithms
// ============================================================================

/**
 * @brief Sorts the in-memory client roster alphabetically by name using std::sort and lambda expressions.
 */
void ClientManager::sortByName() {

    // Execute quicksort on vector using custom string comparison lambda
    std::sort(clients.begin(), clients.end(), [](const Client& a, const Client& b) {
        return a.getName() < b.getName();
        });
}

/**
 * @brief Sorts the in-memory client roster numerically by account ID using std::sort and lambda expressions.
 */
void ClientManager::sortById() {

    // Execute quicksort on vector using integer primary key comparison lambda
    std::sort(clients.begin(), clients.end(), [](const Client& a, const Client& b) {
        return a.getId() < b.getId();
        });
}

/**
 * @brief Sorts the in-memory client roster numerically by service choice using std::sort.
 */
void ClientManager::sortByServiceChoice() {

    // Execute quicksort on vector using service choice integer comparison lambda
    std::sort(clients.begin(), clients.end(), [](const Client& a, const Client& b) {
        return a.getServiceChoice() < b.getServiceChoice();
        });
}

/**
 * @brief Performs a binary search on the sorted roster vector to locate a client by ID.
 * Assumes the roster vector has already been sorted numerically by ID.
 *
 * @param targetId Numerical identifier to search for.
 * @return Client* Pointer to the matching Client object, or nullptr if not found.
 */
Client* ClientManager::binarySearchById(int targetId) {

    // Locate lower bound iterator using O(log n) logarithmic search on sorted vector
    auto it = std::lower_bound(clients.begin(), clients.end(), targetId,
        [](const Client& client, int id) {
            return client.getId() < id;
        });

    // Return direct memory address of object if match is confirmed within vector bounds
    if (it != clients.end() && it->getId() == targetId) {
        return &(*it);
    }
    return nullptr;
}


// ============================================================================
// User Interface & CRUD Controller Operations
// ============================================================================

/**
 * @brief Interactively displays the client roster, offering options to sort by name or ID.
 * Refreshes data directly from SQLite prior to rendering formatted output tables.
 */
void ClientManager::displayClients() {

    refreshFromDatabase();                                            // Retrieve latest records prior to rendering output

    int sortChoice = 0;
    std::cout << "\nHow would you like to sort the roster?\n";
    std::cout << "1. Sort by Client Name (Alphabetical)\n";
    std::cout << "2. Sort by Client ID (Numerical)\n";
    std::cout << "3. Sort by Service Choice\n";

    // Validate integer selection and check against valid range boundaries (1, 2, or 3)
    if (!getValidIntInput("Selection: ", sortChoice) || (sortChoice < 1 || sortChoice > 3)) {
        std::cout << "Invalid choice. Defaulting to sort by Name.\n";
        sortByName();
    }
    // Handle Option 1: Alphabetical sorting
    else if (sortChoice == 1) {
        sortByName();
    }
    // Handle Option 2: Numerical ID sorting
    else if (sortChoice == 2) {
        sortById();
    }
    // Handle Option 3: Service Choice sorting
    else {
        sortByServiceChoice();
    }

    std::cout << "\n----------------------------------------------------------------------------------------------------------------------------------\n";
    std::cout << std::left << std::setw(8) << "ID"
        << std::setw(22) << "Client's Name"
        << "Service Selected (1 = Brokerage, 2 = Retirement, 3 = Estate and Legacy Planning, 4 = Life Insurance)\n";
    std::cout << "----------------------------------------------------------------------------------------------------------------------------------\n";

    // Iterate through client vector and output formatted row fields
    for (const auto& client : clients) {
        std::cout << std::left << std::setw(8) << client.getId()
            << std::setw(22) << client.getName()
            << client.getServiceChoice() << " (" << client.getServiceDescription() << ")\n";
    }
    std::cout << "----------------------------------------------------------------------------------------------------------------------------------\n";
}

/**
 * @brief Prompts for a target client ID, searches database records, and updates the service tier code.
 * Validates selection input and synchronizes both database storage and local state.
 */
void ClientManager::changeCustomerChoice() {

    refreshFromDatabase();                             // Ensure operational state matches database truth

    int targetId = 0;
    int newService = 0;

    // Repetitively prompt until user provides a valid numeric ID
    while (true) {

        // Extract ID and enforce positive key constraint check
        if (!getValidIntInput("\nEnter the ID of the client that you wish to change: ", targetId) || targetId < 1) {
            std::cout << "Invalid client selection. Please enter a valid numerical client ID.\n";
            continue;
        }
        break;
    }

    sortById();                                                   // Pre-condition requirement for binary search execution
    Client* targetClient = binarySearchById(targetId);            // erform O(log n) lookup for record address

    // Terminate mutation flow early if record is absent
    if (targetClient == nullptr) {
        std::cout << "Error: Client ID " << targetId << " was not found in the database.\n";
        return;
    }

    // Confirm target record discovery to user
    std::cout << "Found record for: " << targetClient->getName() << std::endl;

    // Prompt user for updated service selection code
    while (true) {

        // Validate choice against allowed plan integers (1 through 4)
        if (!getValidIntInput("Please enter the client's new service choice (1 = Brokerage, 2 = Retirement, 3 = Estate and Legacy Planning, 4 = Life Insurance):", newService)
            || (newService < 1 || newService > 4)) {
            std::cout << "Invalid service choice. Please enter a number between 1 and 4.\n";
            continue;
        }
        break;                    // Exit validation loop on valid service input
    }

    // Execute SQL UPDATE statement via database persistence layer
    if (dbManager.updateClientServiceChoice(targetId, newService)) {
        targetClient->setServiceChoice(newService);
        std::cout << "Successfully updated database and local state for " << targetClient->getName() << "!\n";
    }
    // Handle database error and abort memory mutation
    else {
        std::cout << "Database transaction failed. Rolling back local changes.\n";
    }
}

/**
 * @brief Collects new client information, generates an auto-incremented primary key,
 * and executes an INSERT operation into the SQLite database.
 */
void ClientManager::addNewClient() {

    refreshFromDatabase();                          // Synchronize cache prior to determining next auto ID key

    std::string newName;
    int serviceChoice = 0;

    std::cout << "\n--- ADD NEW CLIENT RECORD ---\n";

    // Loop until non-empty client name string is provided
    while (true) {
        std::cout << "Enter client full name: ";
        std::getline(std::cin, newName);

        // Reject blank name inputs
        if (newName.empty()) {
            std::cout << "Client name cannot be empty. Please try again.\n";
            continue;
        }
        break;                                      // Exit loop on valid non-empty string input
    }

    // Loop until valid service code is entered
    while (true) {

        // Read integer selection safely line-by-line
        if (!getValidIntInput("Select service choice (1 = Brokerage, 2 = Retirement, 3 = Estate and Legacy Planning, 4 = Life Insurance): ", serviceChoice)
            || (serviceChoice < 1 || serviceChoice > 4)) {
            std::cout << "Invalid selection. Please enter a number between 1 and 4.\n";
            continue;
        }
        break;                                       // Exit loop once valid choice code is captured
    }

    int generatedId = dbManager.getNextAvailableId();                 // Compute next primary key (MAX ID + 1)
    Client tempClient(generatedId, newName, serviceChoice);           // Construct temporary Client instance

    // Persist new client entity into SQLite database table
    if (dbManager.addClientRecord(tempClient)) {
        // Re-hydrate local vector cache to mirror persisted database state
        refreshFromDatabase();
        std::cout << "Successfully added client '" << newName << "' with Assigned ID: " << generatedId << "!\n";
    }
    // Handle database insertion transaction error
    else {
        std::cout << "Error: Failed to insert record into SQLite database.\n";
    }
}

/**
 * @brief Prompts for a client ID, confirms user intent, and removes the client record from the database.
 */
void ClientManager::deleteClient() {
    refreshFromDatabase();                                    // Ensure local roster cache is up to date

    int targetId = 0;
    std::cout << "\n--- DELETE CLIENT RECORD ---\n";

    // Prompt user for target client primary key
    if (!getValidIntInput("Enter the ID of the client to delete: ", targetId) || targetId < 1) {
        std::cout << "Invalid Client ID entered.\n";
        return;
    }

    sortById();                                               // Ensure vector order satisfies binary search prerequisite
    Client* targetClient = binarySearchById(targetId);        // Search for target record in local memory

    // Verify record exists prior to deletion confirmation
    if (targetClient == nullptr) {
        std::cout << "Error: No record found with Client ID " << targetId << ".\n";
        return;
    }

    // Request confirmation from user before executing irreversible deletion
    std::cout << "Are you sure you want to PERMANENTLY delete record for '"
        << targetClient->getName() << "'? (y/n): ";

    std::string confirmInput;                                 // Buffer string for confirmation line input
    std::getline(std::cin, confirmInput);                     // Read confirmation input safely as full string

    // Convert input string to lowercase for strict comparison
    std::string lowerInput = confirmInput;
    for (auto& c : lowerInput) {
        c = static_cast<char>(std::tolower(c));
    }

    // Check if input strictly matches 'y' or 'yes'
    if (lowerInput == "y" || lowerInput == "yes") {

        // Execute SQL DELETE statement via database layer
        if (dbManager.deleteClientRecordById(targetId)) {
            refreshFromDatabase();
            std::cout << "Client record successfully deleted!\n";
        }
        // Handle database execution failure
        else {
            std::cout << "Error: Could not execute deletion statement.\n";
        }
    }
    // Abort removal operation if user input is anything else
    else {
        std::cout << "Deletion operation canceled.\n";
    }
}