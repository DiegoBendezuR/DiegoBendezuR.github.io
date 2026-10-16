/**
 * @file Client.cpp
 * @author Diego M Bendezú Robalino
 * @brief Implementation of the Client domain model.
 * 
 * @details Implements object initialization routines, getter routines, defensive mutators, 
 * and display string formatting to decouple account state management from external console input/output logic.
 */

#include "Client.h"

 /**
  * @brief Constructs a blank Client instance with default initialization state.
  * Sets ID to zero, name to empty string, and defaults service selection to Brokerage (1).
  */
Client::Client() {
    id = 0;
    name = "";
    serviceChoice = 1;
}

/**
 * @brief Constructs a Client instance populated with explicit attribute values.
 *
 * @param clientId Unique account identification number.
 * @param clientName Constant reference to the client's name string.
 * @param initialService Integer representing initial service selection (1 = Brokerage, 2 = Retirement).
 */
Client::Client(int clientId, const std::string& clientName, int initialService) {
    id = clientId;
    name = clientName;
    serviceChoice = initialService;
}

/**
 * @brief Accessor method returning the client's numerical identifier.
 * Designated as const to prevent accidental modification of instance state.
 *
 * @return int The client account identifier.
 */
int Client::getId() const {
    return id;
}

/**
 * @brief Accessor method returning the client's full name.
 * Designated as const to preserve object state during read operations.
 *
 * @return std::string Account holder name.
 */
std::string Client::getName() const {
    return name;
}

/**
 * @brief Accessor method returning the numerical service selection code.
 * Designated as const to guarantee thread-safe read access to state.
 *
 * @return int Integer code representing active service (1 through 4).
 */
int Client::getServiceChoice() const {
    return serviceChoice;
}

/**
 * @brief Mutator method to update the client's service tier.
 * Implements defensive validation checks ensuring service choices must strictly evaluate between
 * 1 and 4 before mutating private class member attributes.
 *
 * @param newChoice Desired service option integer (1 through 4).
 */
void Client::setServiceChoice(int newChoice) {
    if (newChoice >= 1 && newChoice <= 4) {
        serviceChoice = newChoice;
    }
}

/**
 * @brief Helper function translating stored service integer tokens into human-readable text.
 * Eliminates duplicate conditional lookup logic inside display methods throughout the application layer.
 *
 * @return std::string Plaintext name of current investment plan ("Brokerage", "Retirement", "Estate and Legacy Planning", or "Life Insurance").
 */
std::string Client::getServiceDescription() const {
    if (serviceChoice == 1) {
        return "Brokerage";
    }
    else if (serviceChoice == 2) {
        return "Retirement";
    }
    else if (serviceChoice == 3) {
        return "Estate and Legacy Planning";
    }
    else if (serviceChoice == 4) {
        return "Life Insurance";
    }
    else {
        return "Unknown";
    }
}