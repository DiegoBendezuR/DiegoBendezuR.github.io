/**
 * @file Client.h
 * @author Diego M Bendezú Robalino
 * @brief Header definition for the Client domain model.
 * 
 * @details This header defines the Client class architecture, which encapsulates individual customer
 * account data to replace unorganized scalar global variables (e.g., name1-name5, num1-num5).
 * Data encapsulation is strictly enforced via private attributes and public accessor/mutator methods.
 */

#ifndef CLIENT_H
#define CLIENT_H

#include <string>

/**
* @class Client
* 
* @brief Represents an individual investment client and their account choices.
*/
class Client {
private:
    int id;                         // Unique numerical identifier for the client record.
    std::string name;               // Full name of the client stored in dynamically allocated instance.
    int serviceChoice;              // Selected service tier code: 1 = Brokerage, 2 = Retirement.

public:
    // =============================================
    // Constructors
    // =============================================

    /**
    * @brief Default constructor initializing members to safe neutral baseline values.
    */
    Client();

    /**
    * @brief Parameterized constructor creating a populated client instance.
    * 
    * @param clientId Unique numerical identifier assigned to the account.
    * @param clientName String containing the full legal name of the client.
    * @param initialService Integer representing initial choice (1 = Brokerage, 2 = Retirement).
    */
    Client(int clientId, const std::string& clientName, int initialService);



    // ==========================================
    // Accessors (Getters)
    // ==========================================

    /**
     * @brief Retrieves the unique client identifier.
     * 
     * @return int Current account ID.
     */
    int getId() const;

    /**
     * @brief Retrieves the full name of the client.
     * 
     * @return std::string Client name string reference.
     */
    std::string getName() const;

    /**
     * @brief Retrieves the numerical service selection code.
     * 
     * @return int Numerical service option code (1 or 2).
     */
    int getServiceChoice() const;



    // ==========================================
    // Mutators (Setters)
    // ==========================================

    /**
     * @brief Updates the client's investment service selection with strict range validation.
     * 
     * @param newChoice Integer representing new service selection (1 = Brokerage, 2 = Retirement).
     */
    void setServiceChoice(int newChoice);

    

    // ==========================================
    // Utility Methods
    // ==========================================

    /**
     * @brief Converts numerical service choice codes into formatted, readable text strings.
     * 
     * @return std::string Plaintext label representing active service ("Brokerage" or "Retirement").
     */
    std::string getServiceDescription() const;
};

#endif // CLIENT_H