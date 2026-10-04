/**
 * @file ClientManager.h
 * @author Diego M Bendezú Robalino
 * @brief Header definition for managing client records and input streams.
 * 
 * @details The ClientManager class serves as the main controller for client record management within
 * the application. It replaces legacy global variables with an encapsulated dynamic collection (std::vector<Client>),
 * enforcing object-oriented encapsulation principles and preventing direct, unauthorized memory exposure.
 * The class exposes public member interfaces for rendering client records and updating service choices, while
 * encapsulating private helper functions for stream sanitization and internal container initialization.
 */

#ifndef CLIENT_MANAGER_H
#define CLIENT_MANAGER_H

#include "Client.h"
#include <vector>

 /**
  * @class ClientManager
  * 
  * @brief Controller class managing a collection of Client domain objects and controlling input stream validation.
  *
  * This class abstracts collection operations for Client objects, removing reliance on global scalar memory
  * allocations and procedural structures. It encapsulates data access routines and provides robust input
  * validation mechanisms to guard against unexpected stream failure states and out-of-bounds access.
  */
class ClientManager {
private:

    /**
     * @brief Dynamic container storing active Client entity instances.
     * Replaces legacy scalar global variables (name1 through name5, num1 through num5) with a dynamic,
     * heap-managed Standard Template Library vector collection.
     */
    std::vector<Client> clients;

    /**
     * @brief Clears error state flags on std::cin and discards invalid characters remaining in the input stream buffer.
     * Neutralizes potential infinite execution loops caused by type mismatch errors or unconsumed newline characters.
     */
    void clearInputStream();

public:

    /**
     * @brief Default constructor for ClientManager.
     * Automatically triggers default client collection initialization upon object instantiation.
     */
    ClientManager();

    /**
     * @brief Populates the internal vector container with baseline client records.
     * Clears any existing collection data prior to inserting initial Client domain models.
     */
    void initializeDefaultClients();

    /**
     * @brief Formats and outputs the complete client roster to the standard output console stream.
     * Marked as const to guarantee read-only evaluation without mutating instance state variables.
     */
    void displayClients() const;

    /**
     * @brief Interactively prompts the user to select a client ID and update their assigned service choice.
     * Enforces strict type verification, numeric range validation, and input stream recovery handling.
     */
    void changeCustomerChoice();
};

#endif // CLIENT_MANAGER_H