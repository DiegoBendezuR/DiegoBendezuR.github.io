/**
 * @file ClientManager.h
 * @author Diego M Bendezú Robalino
 * @brief Header definition for client management, user interface, and DB control.
 *
 * @details This header defines the ClientManager orchestration class responsible for managing
 * interactive user options, input stream parsing, client sorting operations, binary searching,
 * and coordinating CRUD transactions between the in-memory vector state and SQLite persistence layer.
 */

#ifndef CLIENT_MANAGER_H
#define CLIENT_MANAGER_H

#include "Client.h"
#include "DatabaseManager.h"
#include <vector>

 /**
  * @class ClientManager
  *
  * @brief Orchestrates user interaction workflows and synchronizes client data structures with SQLite.
  */
class ClientManager {
private:
    std::vector<Client> clients;                       // Dynamic in-memory collection holding active client records.
    DatabaseManager dbManager;                         // Database management layer instance handling SQLite queries.

    /**
     * @brief Clears stream failure flags and flushes unread characters from the console input buffer.
     */
    void clearInputStream();

    /**
     * @brief Sorts the internal client vector alphabetically by client name using std::sort.
     */
    void sortByName();

    /**
     * @brief Sorts the internal client vector in ascending order by numerical client ID.
     */
    void sortById();

    /**
     * @brief Sorts the internal client vector numerically by service choice code.
     */
    void sortByServiceChoice();

    /**
     * @brief Performs an O(log n) binary search on sorted client data to find a target ID.
     *
     * @param targetId Numerical client identifier to search for.
     * @return Client* Pointer to matching Client instance if found; nullptr otherwise.
     */
    Client* binarySearchById(int targetId);

public:
    // =============================================
    // Constructors
    // =============================================

    /**
     * @brief Constructs ClientManager, initializes SQLite schema, and loads stored records.
     */
    ClientManager();


    // =============================================
    // Data Synchronization & Operations
    // =============================================

    /**
     * @brief Synchronizes the in-memory client list with current records stored in the SQLite database.
     */
    void refreshFromDatabase();

    /**
     * @brief Interactively prompts sorting preferences and displays the formatted client roster.
     */
    void displayClients();

    /**
     * @brief Prompts for a target client ID and updates their investment service tier in SQLite and local state.
     */
    void changeCustomerChoice();

    /**
     * @brief Interactively prompts the user to input data for a new client record and stores it in SQLite.
     */
    void addNewClient();

    /**
     * @brief Prompts the user for a client ID to delete from the SQLite database.
     */
    void deleteClient();


    // =============================================
    // Input Validation Helpers
    // =============================================

    /**
     * @brief Safely reads a strictly numeric integer from input without leaving trailing characters.
     *
     * @param prompt Text message displayed to user prior to input.
     * @param outValue Reference variable where parsed integer is stored upon successful extraction.
     * @return true If input was strictly valid integer without trailing non-numeric characters; false otherwise.
     */
    static bool getValidIntInput(const std::string& prompt, int& outValue);
};

#endif // CLIENT_MANAGER_H