/**
 * @file ClientManager.cpp
 * @author Diego M Bendezú Robalino
 * @brief Implementation of client roster operations, user prompts, and input sanitization.
 * 
 * @details This source file implements the operational logic for the ClientManager controller class.
 * Key architectural enhancements implemented here include replacing global static data structures with standard library
 * dynamic vectors, implementing input stream recovery via std::cin.clear() and std::cin.ignore(), and
 * enforcing rigorous boundary checks during interactive state mutation operations.
 */

#include "ClientManager.h"
#include <iostream>
#include <iomanip>
#include <limits>

 /**
  * @brief Default constructor initializing the ClientManager instance.
  * Automatically populates the internal collection with default investment client records upon construction.
  */
ClientManager::ClientManager() {
    initializeDefaultClients();
}

/**
 * @brief Clears error state flags on the standard input stream and flushes unread characters.
 *
 * @details When std::cin encounters an input type mismatch (e.g., non-numeric characters entered into an integer variable),
 * the failbit state flag is raised, blocking subsequent stream operations. This routine restores the stream to a valid
 * operational state by calling std::cin.clear() and flushes leftover buffer contents up to the delimiter using std::cin.ignore(),
 * resolving potential infinite input processing loops.
 */
void ClientManager::clearInputStream() {

    // Reset error state flags (failbit, badbit) on the std::cin stream.
    std::cin.clear();

    // Flush remaining characters from the stream buffer.
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

/**
 * @brief Seeds the internal collection vector with baseline client data records.
 *
 * @details Replaces hardcoded static global strings (name1 through name5) and global
 * integers (num1 through num5) from the legacy binary reference implementation. Employs std::vector::emplace_back
 * to construct Client instances directly in dynamic memory within the container, optimizing heap allocations.
 */
void ClientManager::initializeDefaultClients() {

    // Reset container state to prevent duplicate record insertion during re-initialization.
    clients.clear();

    // Construct and insert initial Client objects into the dynamic collection.
    // Client(int id, const std::string& name, int initialServiceChoice)
    clients.emplace_back(1, "Bob Jones", 1);
    clients.emplace_back(2, "Sarah Davis", 2);
    clients.emplace_back(3, "Amy Friendly", 1);
    clients.emplace_back(4, "Johnny Smith", 1);
    clients.emplace_back(5, "Carol Spears", 2);
}

/**
 * @brief Formats and displays client records in a structured tabular console view.
 *
 * @details Eliminates repetitive cout statements bound to global variables.
 * Utilizes standard library I/O manipulators (std::left, std::setw) to enforce structured column formatting across
 * variable-length client name strings and dynamically iterates through the internal vector container using range-based loops.
 */
void ClientManager::displayClients() const {

    // Output table header division line and column identifiers.
    std::cout << "\n----------------------------------------------------------------------------\n";
    std::cout << std::left << std::setw(8) << "ID"
        << std::setw(20) << "Client's Name"
        << "Service Selected (1 = Brokerage, 2 = Retirement)\n";
    std::cout << "----------------------------------------------------------------------------\n";

    // Sequentially traverse the encapsulated vector collection of Client objects.
    for (const auto& client : clients) {
        std::cout << std::left << std::setw(8) << client.getId()
            << std::setw(20) << client.getName()
            << client.getServiceChoice() << " (" << client.getServiceDescription() << ")\n";
    }
    std::cout << "----------------------------------------------------------------------------\n";
}

/**
 * @brief Handles interactive user modification of client investment service selections.
 *
 * @details The legacy binary implementation relied on unvalidated global integer assignments and direct switch-case mapping,
 * creating exposure to out-of-bounds access and unhandled input stream failures.
 * 
 * This refactored implementation enforces triple-layer input validation:
 * 1. Type evaluation via std::cin.fail() to catch non-numeric stream entries.
 * 2. Range validation constraining selection indices to active collection bounds (1 to clients.size()).
 * 3. Encapsulated object traversal updating member state strictly through public setter interfaces.
 */
void ClientManager::changeCustomerChoice() {

    int clientId = 0;                 // Holds validated client ID selection index.
    int newService = 0;               // Holds validated service choice selection (1 = Brokerage, 2 = Retirement).

    // Prompt and validate client ID selection within container boundary limits.
    while (true) {
        std::cout << "\nEnter the ID of the client that you wish to change (1-5): ";
        std::cin >> clientId;

        // Evaluates stream failure state, non-numeric input, and out-of-range client ID bounds.
        if (std::cin.fail() || clientId < 1 || clientId > static_cast<int>(clients.size())) {
            clearInputStream();        // Reset error state flags and discard invalid stream buffer contents.
            std::cout << "Invalid client selection. Please enter a valid client ID.\n";
            continue;                  // Re-prompt user for input.
        }
        break;         // Input successfully verified against type and dynamic bounds constraints.
    }

    std::cout << "Selected client ID: " << clientId << std::endl;

    // Prompt and validate new service selection choice (1 = Brokerage, 2 = Retirement).
    while (true) {
        std::cout << "Please enter the client's new service choice (1 = Brokerage, 2 = Retirement): ";
        std::cin >> newService;

        // Verifies service selection strictly matches allowed business domain choices (1 or 2).
        if (std::cin.fail() || (newService != 1 && newService != 2)) {
            clearInputStream();        // Reset error state flags and discard invalid stream buffer contents.
            std::cout << "Invalid service choice. Please enter 1 or 2.\n";
            continue;                  // Re-prompt user for input.
        }
        break;          // Input successfully verified against type and busines logic constraints.
    }

    // Search target client by unique ID and mutate service choice via public class interface.
    for (auto& client : clients) {
        if (client.getId() == clientId) {
            client.setServiceChoice(newService);       // Mutate instance state using encapsulated member setter.
            std::cout << "Service choice successfully updated for " << client.getName() << "!\n";
            break;       // Exit traversal loop once target record is updated.
        }
    }
}