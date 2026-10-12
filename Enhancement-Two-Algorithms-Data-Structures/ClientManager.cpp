/**
 * @file ClientManager.cpp
 * @author Diego M Bendezú Robalino
 * @brief Implementation of client roster operations, user prompts, input sanitization, and sorting algorithms.
 * 
 * @details This source file implements the operational logic for the ClientManager controller class.
 * Key architectural enhancements implemented here include replacing global static data structures with standard library
 * dynamic vectors, implementing input stream recovery via std::cin.clear() and std::cin.ignore(), and
 * enforcing rigorous boundary checks during interactive state mutation operations.
 * Additionally, it integrates C++ Standard Template Library (STL) algorithms such as std::sort and std::lower_bound
 * to provide dynamic runtime ordering and efficient binary search operations over client records.
 */

#include "ClientManager.h"
#include <iostream>
#include <iomanip>
#include <limits>
#include <algorithm>           // Required for std::sort and std::lower_bound algorithms

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
 * @brief Sorts the vector alphabetically by client name using std::sort.
 * 
 * @details Utilizes the STL std::sort algorithm in conjunction with a custom lambda expression
 * to perform an in-place sort of the client records based on alphabetical order of the client name.
 * Time Complexity: O(N log N)
 */
void ClientManager::sortByName() {

    // Sort client records in alphabetical order using std::sort and a comparison lambda on client names.
    std::sort(clients.begin(), clients.end(), [](const Client& a, const Client& b) {
        return a.getName() < b.getName();       // Compares client names alphabetically.
        });
}

/**
 * @brief Sorts the vector numerically by client ID using std::sort.
 * 
 * @details Utilizes the STL std::sort algorithm along with a lambda comparator to reorder client
 * records sequentially in ascending numeric order based on their unique client ID.
 * Time Complexity: O(N log N)
 */
void ClientManager::sortById() {

    // Sort client records in ascending numerical order using std::sort and a comparison lambda on client IDs.
    std::sort(clients.begin(), clients.end(), [](const Client& a, const Client& b) {
        return a.getId() < b.getId();           // Compares client unique IDs numerically.
        });
}

/**
 * @brief Executes Binary Search on sorted client vector using std::lower_bound.
 * 
 * @details Requires the vector to be sorted by client ID beforehand. Leverages std::lower_bound to perform
 * a logarithmic binary search over contiguous memory, locating the target client record in O(log N) time complexity.
 * Time Complexity: O(log N)
 */
Client* ClientManager::binarySearchById(int targetId) {

    // Perform binary search for targetId using std::lower_bound with a predicate comparison.
    auto it = std::lower_bound(clients.begin(), clients.end(), targetId,
        [](const Client& client, int id) {
            return client.getId() < id;       // Binary search predicate comparing client ID to target ID.
        });

    // Verify if iterator points to a valid element within bounds and matches the requested ID.
    if (it != clients.end() && it->getId() == targetId) {
        return &(*it); // Return pointer to matching Client object.
    }
    return nullptr; // Target not found.
}

/**
 * @brief Formats and displays client records in a structured tabular console view.
 *
 * @details Eliminates repetitive cout statements bound to global variables.
 * Utilizes standard library I/O manipulators (std::left, std::setw) to enforce structured column formatting across
 * variable-length client name strings and dynamically iterates through the internal vector container using range-based loops.
 */
void ClientManager::displayClients() {

    // Local variable to store user's sorting preference choice.
    int sortChoice = 0;

    // Prompt user to choose between alphabetical and numerical sorting modes.
    std::cout << "\nHow would you like to sort the roster?\n";
    std::cout << "1. Sort by Client Name (Alphabetical)\n";
    std::cout << "2. Sort by Client ID (Numerical)\n";
    std::cout << "Selection: ";
    std::cin >> sortChoice;

    // Validate user input selection and execute chosen sort algorithm.
    if (std::cin.fail() || (sortChoice != 1 && sortChoice != 2)) {
        clearInputStream();       // Handle input mismatch or out-of-range value by resetting stream.
        std::cout << "Invalid choice. Defaulting to sort by Name.\n";
        sortByName();             // Fallback to default alphabetical sorting.
    }
    else if (sortChoice == 1) {
        sortByName();             // Apply alphabetical sort by name.
    }
    else {
        sortById();               // Apply numerical sort by client ID.
    }

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
 * This refactored implementation enforces robust input validation and algorithmic lookup:
 * 1. Type and bounds checking via std::cin.fail() to sanitize input stream entries for requested Client IDs.
 * 2. Guaranteed collection ordering by numerical ID via sortById() prior to search execution.
 * 3. Logarithmic lookup using binarySearchById() to efficiently locate the targeted object in memory.
 * 4. Safe state mutation updating member state strictly through public setter interfaces on the retrieved object pointer.
 */
void ClientManager::changeCustomerChoice() {

    int targetId = 0;                 // Holds requested target client ID identifier.
    int newService = 0;               // Holds validated service choice selection (1 = Brokerage, 2 = Retirement).

    // Loop until a valid numeric target client ID is successfully entered.
    while (true) {
        std::cout << "\nEnter the ID of the client that you wish to change (1-5): ";
        std::cin >> targetId;

        // Evaluate stream state and ensure ID falls within lower boundary constraints.
        if (std::cin.fail() || targetId < 1) {
            clearInputStream();        // Reset error state flags and discard invalid stream buffer contents.
            std::cout << "Invalid client selection. Please enter a valid client ID.\n";
            continue;                  // Re-prompt user for input.
        }
        break;         // Input successfully verified against type and dynamic bounds constraints.
    }

    // Ensure clients vector is sorted by ID prior to invoking binary search.
    sortById();

    // Perform O(log N) binary search to locate target client record pointer.
    Client* targetClient = binarySearchById(targetId);

    // Validate search result pointer to ensure record existence.
    if (targetClient == nullptr) {
        std::cout << "Error: Client ID " << targetId << " was not found in the database.\n";
        return;
    }

    // Output target client details once matching record is found.
    std::cout << "Found record for: " << targetClient->getName() << std::endl;

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
        break;          // Input successfully verified against type and business logic constraints.
    }

    // Mutate state of target client record through public setter interface.
    targetClient->setServiceChoice(newService);

    // Display confirmation message indicating successful state change.
    std::cout << "Successfully updated service choice for " << targetClient->getName() << "!\n";

}