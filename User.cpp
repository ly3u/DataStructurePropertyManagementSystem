#include "LinkedList.hpp"
#include "CSVReader.hpp"
#include "RentRequest.hpp"
#include <chrono>
#include <algorithm>
#include <vector>
#include <sstream>

using namespace chrono;
using namespace std;

User::User(string userId, string username, string password)
    : userId(userId), username(username), password(password) {}

void User::display() const
{
    cout << "User ID: " << userId << ", Username: " << username << "\n";
}

Admin::Admin(string userId, string username, string password)
    : User(userId, username, password) {}

void Admin::display() const
{
    cout << "Admin: " << endl;
    User::display();
}

void Admin::adminMenu(LinkedList &tenantList, LinkedList &managerList)
{
    string choice;

    PropertyLinkedList propertyList;
    readProperties(propertyList);

    do
    {
        cout << "\nAdmin's Domain: \n";
        cout << "\n1. Breathe life into the Manager Realm: Forge a new overseer or alter their fate\n";
        cout << "2. Unveil the Chronicles of Tenants and Properties with the power of filtering\n";
        cout << "3. Retreat from this digital dominion – Logout for now\n";
        cout << "Enter your command wisely: ";

        cin >> choice;
        if (choice == "1")
        {
            string managerChoice;
            cout << "\n1. Add new manager\n";
            cout << "2. Modify manager status\n";
            cout << "3. Back\n";

            cin >> managerChoice;

            if (managerChoice == "1")
            {
                managerList.display();
                string userId;
                string username;
                string password;
                // Assuming you want to take inputs for the userId, username, and password
                cout << "Enter Manager's User ID: ";
                cin >> userId;
                cout << "Enter Manager's Username: ";
                cin >> username;
                cout << "Enter Manager's Password: ";
                cin >> password;

                Manager *newManager = new Manager(userId, username, password, 1);
                managerList.insert(newManager);
            }
            else if (managerChoice == "2")
            {
                managerList.display();
                string userId;
                cout << "Enter the User ID of the manager you wish to modify: ";
                cin >> userId;
                managerList.setManagerStatusByUserId(userId);
            }
            else if (managerChoice == "3")
            {
            }
            else
            {
                cout << "Invalid choice. Choose between 1 and 3.";
            }
        }
        else if (choice == "2")
        {
            string tenantOrProperty;
            cout << "\n1. Display Tenants\n";
            cout << "2. Display Properties\n";
            cout << "3. Back\n";

            cin >> tenantOrProperty;

            if (tenantOrProperty == "1")
            {
                adminTenantMenu(tenantList);
            }
            else if (tenantOrProperty == "2")
            {
                adminPropertyMenu();
            }
            else if (tenantOrProperty == "3")
            {
            }
            else
            {
                cout << "Invalid choice. Choose between 1 and 3.";
            }
        }
        else if (choice == "3")
        {
            cout << "Initiating the sacred ritual of logout...\n";
        }
        else
        {
            cout << "Ah, the realm of confusion! Enter a domain between 1 and 3, oh mighty administrator.\n";
        }
    } while (choice != "3");
}

void Admin::adminPropertyMenu()
{
    FilterCriteria criteria;

    PropertyLinkedList propertyList;
    readProperties(propertyList);

    // Monthly Rent
    cout << "Enter minimum monthly rent: ";
    cin >> criteria.min_monthly_rent;
    cout << "Enter maximum monthly rent: ";
    cin >> criteria.max_monthly_rent;

    // Completion Year
    cout << "Enter minimum completion year (or any negative number to skip): ";
    cin >> criteria.min_completion_year;
    if (criteria.min_completion_year < 0)
        criteria.min_completion_year = INT_MIN;

    cout << "Enter maximum completion year (or any negative number to skip): ";
    cin >> criteria.max_completion_year;
    if (criteria.max_completion_year < 0)
        criteria.max_completion_year = INT_MAX;

    // Rooms
    cout << "Enter minimum number of rooms: ";
    cin >> criteria.min_rooms;
    cout << "Enter maximum number of rooms: ";
    cin >> criteria.max_rooms;

    // Parking
    cout << "Enter minimum number of parking spots (or any negative number to skip): ";
    cin >> criteria.min_parking;
    if (criteria.min_parking < 0)
        criteria.min_parking = INT_MIN;

    cout << "Enter maximum number of parking spots: ";
    cin >> criteria.max_parking;
    if (criteria.max_parking < 0)
        criteria.max_parking = INT_MAX;

    // Bathroom
    cout << "Enter minimum number of bathrooms (or 0 to skip): ";
    cin >> criteria.min_bathroom;
    cout << "Enter maximum number of bathrooms (or 0 to skip): ";
    cin >> criteria.max_bathroom;

    // Size
    cout << "Enter minimum property size: ";
    cin >> criteria.min_size;
    cout << "Enter maximum property size: ";
    cin >> criteria.max_size;

    // Location
    cout << "Enter desired location (or press enter to skip): ";
    cin.ignore(); // To clear the input buffer
    getline(cin, criteria.location);

    // Property Type
    cout << "Enter property type ('Condominium', 'Apartment', 'Service Residence', 'Flat' - or press enter to skip): ";
    getline(cin, criteria.property_type);

    // Furnished
    cout << "Is the property furnished? (Enter 'Partially Furnished', 'Fully Furnished', 'Not Furnished', or press enter to skip): ";
    getline(cin, criteria.furnished);

    // Region
    cout << "Enter desired region (or press enter to skip): ";
    getline(cin, criteria.region);

    // Facilities
    cout << "Enter required facilities (comma-separated, e.g., 'pool,gym' - or press enter to skip): ";
    string facilities;
    getline(cin, facilities);
    stringstream ss(facilities);
    string facility;
    while (getline(ss, facility, ','))
    {
        criteria.required_facilities.push_back(facility);
    }

    // Additional Facilities
    cout << "Enter required additional facilities (comma-separated - or press enter to skip): ";
    string additional_facilities;
    getline(cin, additional_facilities);
    stringstream ss2(additional_facilities);
    string additional_facility;
    while (getline(ss2, additional_facility, ','))
    {
        criteria.required_additional_facilities.push_back(additional_facility);
    }

    // Number of rows to display
    int numRows;
    cout << "Enter the number of rows to display: ";
    while (!(cin >> numRows))
    {
        cout << "Invalid input. Please enter a number: ";
        cin.clear();                                         // Clear the error flags
        cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Discard invalid input
    }

    propertyList.filter(criteria).display(numRows);
}

void Admin::adminTenantMenu(LinkedList &tenantList)
{
    TenantFilterCriteria tenantCriteria;

    cout << "Enter tenant name (or press enter to skip): ";
    cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Clear the input buffer
    getline(cin, tenantCriteria.name);

    // Input validation loop for minimum age
    while (true)
    {
        cout << "Enter minimum age: ";
        if (cin >> tenantCriteria.min_age)
        {
            break;
        }
        else
        {
            cout << "Invalid input! Please enter a valid age." << endl;
            cin.clear();                                         // Clear the error flag
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Clear the input buffer
        }
    }

    // Input validation loop for maximum age
    while (true)
    {
        cout << "Enter maximum age: ";
        if (cin >> tenantCriteria.max_age)
        {
            break;
        }
        else
        {
            cout << "Invalid input! Please enter a valid age." << endl;
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
    }

    char choice;
    cout << "Do you want to filter by last login date? (y/n): ";
    cin >> choice;

    if (choice == 'y' || choice == 'Y')
    {
        tenantCriteria.useDate = true;

        // Input validation loop for start date
        while (true)
        {
            cout << "Enter start date (YYYY MM DD): ";
            if (cin >> tenantCriteria.startDate.tm_year >> tenantCriteria.startDate.tm_mon >> tenantCriteria.startDate.tm_mday)
            {
                break;
            }
            else
            {
                cout << "Invalid input! Please enter a valid date in the format YYYY MM DD." << endl;
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
            }
        }

        // Input validation loop for end date
        while (true)
        {
            cout << "Enter end date (YYYY MM DD): ";
            if (cin >> tenantCriteria.endDate.tm_year >> tenantCriteria.endDate.tm_mon >> tenantCriteria.endDate.tm_mday)
            {
                break;
            }
            else
            {
                cout << "Invalid input! Please enter a valid date in the format YYYY MM DD." << endl;
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
            }
        }
    }
    tenantList.filter(tenantCriteria).display();
}

Manager::Manager(string userId, string username, string password, bool status)
    : User(userId, username, password), status(status) {}

void Manager::display() const
{
    cout << "Manager: " << endl;
    User::display();
    cout << "Status: " << (status ? "Active" : "Inactive") << "\n";
}

void Manager::displaySortedTenants(LinkedList &tenantList)
{
    // Get the current date
    auto now = system_clock::now();
    time_t now_time_t = system_clock::to_time_t(now);

    // Convert the tenant list to a vector
    vector<Tenant *> tenants = tenantList.to_vector();
    vector<User *> users(tenants.begin(), tenants.end());

    // Sort the vector by last login date
    sort(users.begin(), users.end(), [](User *a, User *b)
         {
        Tenant* tenantA = dynamic_cast<Tenant*>(a);
        Tenant* tenantB = dynamic_cast<Tenant*>(b);
        if (tenantA != nullptr && tenantB != nullptr) {
            return difftime(mktime(&(tenantA->lastLogin)), mktime(&(tenantB->lastLogin))) < 0;
        }
        return false; });

    // Display the sorted tenants
    for (User *user : users)
    {
        Tenant *tenant = dynamic_cast<Tenant *>(user);
        if (tenant != nullptr)
        {
            tenant->display();

            // Calculate and display the number of days since the last login
            time_t lastLogin_time_t = mktime(&tenant->lastLogin);
            system_clock::time_point now_time_point = system_clock::from_time_t(now_time_t);
            system_clock::time_point lastLogin_time_point = system_clock::from_time_t(lastLogin_time_t);
            duration<int, ratio<60 * 60 * 24>> days_since_last_login = duration_cast<duration<int, ratio<60 * 60 * 24>>>(now_time_point - lastLogin_time_point);

            // Format last login date as dd-mm-yyyy
            char lastLoginDate[11];
            strftime(lastLoginDate, sizeof(lastLoginDate), "%d-%m-%Y", &tenant->lastLogin);

            cout << "Last login: " << lastLoginDate << ", Days since last login: " << days_since_last_login.count() << "\n";
        }
    }
}

void Manager::managerMenu(FavouritePropertyLinkedList &favouriteList, RentRequestLinkedList &rentRequestList, PropertyLinkedList &propertyList, LinkedList &tenantList)
{
    string choice;
    string userIdToDelete;
    do
    {
        cout << "\nManager's Realm: \n";
        cout << "1. Unveil the Details of Registered Tenants\n";
        cout << "2. Embark on a Quest for Tenant Info\n";
        cout << "3. Banish Inactive Tenants into the Abyss\n";
        cout << "4. Summon the Legends of Top 10 Properties\n";
        cout << "5. Dive into the Dance of Tenancy Process\n";
        cout << "6. Tame the Wild Beast of Confirmed Payments\n";
        cout << "7. Logout\n";
        cout << "Enter Your Command: ";
        cin >> choice;

        if (choice == "1")
        {
            tenantList.display();
        }
        else if (choice == "2")
        {
            searchTenantInfo(tenantList);
        }
        else if (choice == "3")
        {
            // Iterate over the list of tenants
            displaySortedTenants(tenantList);
            cout << "Enter the sacrificial User ID of the tenant you wish to obliterate, or 'cancel' to backtrack: ";
            cin >> userIdToDelete;
            if (userIdToDelete == "cancel")
            {
                cout << "Deletion ritual aborted – their digital existence lives on.\n";
            }
            else
            {
                // Delete tenant from linked list and csv file
                tenantList.remove(userIdToDelete);
                cout << "Tenant bearing User ID " << userIdToDelete << " has been vanquished from our realms.\n";
            }
        }
        else if (choice == "4")
        {
            favouriteList.summarizeTop10Properties(propertyList);
        }
        else if (choice == "5")
        {
            rentRequestList.managePendingRentRequests(propertyList);
        }
        else if (choice == "6")
        {
            rentRequestList.manageTenancyPayment();
        }
        else if (choice == "7")
        {
            cout << "Commencing the art of logout...\n";
        }
        else
        {
            cout << "Ah, the land of indecision! Enter a realm between 1 and 7, mortal.\n";
        }
    } while (choice != "7");
}

void Manager::searchTenantInfo(LinkedList &list)
{
    int criteriaChoice;
    cout << "\nChoose a criteria to search by: \n";
    cout << "1. User ID\n";
    cout << "2. Name\n";
    cout << "3. Age\n";
    cout << "4. Last Login\n";
    cout << "Enter Your Command: ";
    cin >> criteriaChoice;

    switch (criteriaChoice)
    {
    case 1:
    {
        string userId;
        cout << "Enter User ID to search: ";
        cin >> userId;
        list.searchTenantByUserId(userId);
        break;
    }
    case 2:
    {
        string name;
        cout << "Enter Name to search: ";
        cin.ignore();       // Ignore the newline character in the input buffer
        getline(cin, name); // Use getline to read a full line of text
        list.searchTenantByName(name);
        break;
    }
    case 3:
    {
        int age;
        cout << "Enter Age to search: ";
        cin >> age;
        list.searchTenantByAge(age);
        break;
    }
    case 4:
    {
        tm lastLogin;
        cout << "Enter Last Login date (format: dd mm yyyy): ";
        cin >> lastLogin.tm_mday >> lastLogin.tm_mon >> lastLogin.tm_year;
        lastLogin.tm_mon -= 1;     // tm_mon is months since January - [0,11]
        lastLogin.tm_year -= 1900; // tm_year is years since 1900
        list.searchTenantByLastLogin(lastLogin);
        break;
    }
    default:
        cout << "Invalid choice. Please enter a number between 1 and 4.\n";
    }
}

void Manager::setStatus(bool newStatus)
{
    if (newStatus == 0 || newStatus == 1)
    {
        status = newStatus; // Assuming the status member variable exists in the Manager class
    }
    else
    {
        cout << "Invalid status value. Status should be 0 (inactive) or 1 (active).";
    }
}

Tenant::Tenant(string userId, string username, string password, string name, int age, tm lastLogin)
    : User(userId, username, password), name(name), age(age), lastLogin(lastLogin) {}

void Tenant::display() const
{
    cout << "Tenant: " << endl;
    User::display();
    cout << "Name: " << name
         << ", Age: " << age
         << ", Last Login: " << (lastLogin.tm_year + 1900) << "-" << (lastLogin.tm_mon + 1) << "-" << lastLogin.tm_mday << "\n";
}

void Tenant::tenantMenu(User *loggedInUser, FavouritePropertyLinkedList &favouriteList, RentRequestLinkedList &rentRequestList, PropertyLinkedList &propertyList)
{
    string choice;
    int numRows;
    FavouriteProperty newFavouriteProperty;
    do
    {
        cout << "\nTenant's Universe: \n";
        cout << "1. Sort and unveil the secrets of property information\n";
        cout << "2. Seek and reveal property details through the digital maze\n";
        cout << "3. Save a special property, a cozy corner of your world\n";
        cout << "4. Send forth your rent request like a virtual carrier pigeon\n";
        cout << "5. Trace the path of property history through the annals of time\n";
        cout << "6. Logout\n";
        cout << "Your Move, O' Inhabitant. " << endl
             << "Choose Wisely:";

        cin >> choice;

        if (choice == "1")
        {
            int sortChoice, algoChoice;
            cout << "Choose a sorting criterion:" << endl;
            cout << "1. Monthly Rent" << endl;
            cout << "2. Location" << endl;
            cout << "3. Size" << endl;
            cout << "Enter your choice (1/2/3): ";
            cin >> sortChoice;

            cout << "Choose a sorting algorithm:" << endl;
            cout << "1. Merge Sort" << endl;
            cout << "2. Quick Sort" << endl;
            cout << "Enter your choice (1/2): ";
            cin >> algoChoice;

            switch (sortChoice)
            {
            case 1:
                if (algoChoice == 1)
                    propertyList.mergeSortByCriterion(SortCriteria::MonthlyRent);
                else
                    propertyList.quickSortByCriterion(SortCriteria::MonthlyRent);
                break;
            case 2:
                if (algoChoice == 1)
                    propertyList.mergeSortByCriterion(SortCriteria::Location);
                else
                    propertyList.quickSortByCriterion(SortCriteria::Location);
                break;
            case 3:
                if (algoChoice == 1)
                    propertyList.mergeSortByCriterion(SortCriteria::Size);
                else
                    propertyList.quickSortByCriterion(SortCriteria::Size);
                break;
            default:
                cout << "Invalid choice." << endl;
            }
            cout << "Enter the number of rows to display: ";
            while (!(cin >> numRows))
            {
                cout << "Invalid input. Please enter a number: ";
                cin.clear();                                         // Clear the error flags
                cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Discard invalid input
            }
            propertyList.display(numRows);
        }
        else if (choice == "2")
        {
            // Display menu and get user's choice
            int choice, searchAlgorithm;
            cout << "Choose the criteria for property search:" << endl;
            cout << "1. Ads ID" << endl;
            cout << "2. Property Name" << endl;
            cout << "3. Completion Year" << endl;
            cout << "4. Monthly Rent" << endl;
            cout << "5. Location" << endl;
            cout << "6. Property Type" << endl;
            cout << "7. Number of Rooms" << endl;
            cout << "8. Number of Parking Spaces" << endl;
            cout << "9. Number of Bathrooms" << endl;
            cout << "10. Size" << endl;
            cout << "11. Furnished" << endl;
            cout << "12. Region" << endl;
            cout << "Enter your choice: ";
            cin >> choice;

            cout << "Choose a search algorithm:" << endl;
            cout << "1. Linear Search" << endl;
            cout << "2. Binary Search" << endl; // Or any other search algorithm you want to include
            cout << "Enter your choice (1/2): ";
            cin >> searchAlgorithm;

            // prompt the user for the required input
            SearchCriteria criteria;
            string str_value;
            int int_value;

            switch (choice)
            {
            case 1:
                criteria = SearchCriteria::ADS_ID;
                cout << "Enter Property Listing Id: ";
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                getline(cin, str_value);
                break;
            case 2:
                criteria = SearchCriteria::PROP_NAME;
                cout << "Enter Property Name: ";
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                getline(cin, str_value);
                break;
            case 3:
                criteria = SearchCriteria::COMPLETION_YEAR;
                cout << "Enter Completion Year: ";
                cin >> int_value;
                break;
            case 4:
                criteria = SearchCriteria::MONTHLY_RENT;
                cout << "Enter Monthly Rent: ";
                cin >> int_value;
                break;
            case 5:
                criteria = SearchCriteria::LOCATION;
                cout << "Enter Location: ";
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                getline(cin, str_value);
                break;
            case 6:
                criteria = SearchCriteria::PROPERTY_TYPE;
                cout << "Enter Property Type: (Condominium/Apartment/Service Residence/Studio/Flat/Duplex)";
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                getline(cin, str_value);
                break;
            case 7:
                criteria = SearchCriteria::ROOMS;
                cout << "Enter Number of Rooms: ";
                cin >> int_value;
                break;
            case 8:
                criteria = SearchCriteria::PARKING;
                cout << "Enter Number of Parking Spaces: ";
                cin >> int_value;
                break;
            case 9:
                criteria = SearchCriteria::BATHROOM;
                cout << "Enter Number of Bathrooms: ";
                cin >> int_value;
                break;
            case 10:
                criteria = SearchCriteria::SIZE;
                cout << "Enter Size: ";
                cin >> int_value;
                break;
            case 11:
                criteria = SearchCriteria::FURNISHED;
                cout << "Is the property furnished? (Fully Furnished/Partially Furnished/Not Furnished): ";
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                getline(cin, str_value);
                break;
            case 12:
                criteria = SearchCriteria::REGION;
                cout << "Enter Region: ";
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                getline(cin, str_value);
                break;
            default:
                cout << "Invalid choice!" << endl;
                return;
            }

            propertyList.updateVectorFromList();
            Property *result;

            switch (searchAlgorithm)
            {
            case 1:
                if (isStringCriteria(criteria))
                {
                    result = propertyList.linearSearch(criteria, str_value, 0);
                }
                else
                {
                    result = propertyList.linearSearch(criteria, "", int_value);
                }
                break;
            case 2:
                if (isStringCriteria(criteria))
                {
                    result = propertyList.binarySearch(criteria, str_value, 0);
                }
                else
                {
                    result = propertyList.binarySearch(criteria, "", int_value);
                }
                break;
            default:
                cout << "Invalid search algorithm choice!" << endl;
                return;
            }

            if (!result)
            {
                cout << "No other property found with the given criteria." << endl;
            }
        }
        else if (choice == "3")
        {
            string adsId;
            cout << "Enter Favourite Property Listing Id: ";
            cin >> adsId;
            if (!propertyList.adsIdExists(adsId))
            {
                cout << "No such property listing exist!";
            }
            else
            {
                newFavouriteProperty.ads_id = adsId;
                newFavouriteProperty.userId = this->getUserId(); // Using the getUserId() function of the Tenant class
                favouriteList.insert(newFavouriteProperty);
            }
        }
        else if (choice == "4")
        {
            cout << "Your favorite properties:" << endl;

            // Display favorite properties associated with the logged-in tenant's userId
            favouriteList.displayForUser(this->getUserId());

            int selectedPropertyIndex;
            bool validInput = false;
            while (!validInput)
            {
                cout << "Enter the index of the property you wish to request (or enter 'cancel' to exit): ";
                string userInput;
                cin >> userInput;

                if (userInput == "cancel")
                {
                    cout << "Request cancelled." << endl;
                    break; // Exit the loop if user input is 'cancel'
                }

                // Convert user input to an integer
                selectedPropertyIndex = stoi(userInput);

                // Check for failed conversion or invalid index
                if (cin.fail() || !favouriteList.propertyAtIndexExists(selectedPropertyIndex))
                {
                    cin.clear();                                         // Clear the error flags
                    cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Discard invalid input
                    cout << "Invalid input. Please enter a valid index number or 'cancel' to exit." << endl;
                    continue;
                }

                validInput = true; // Exit the loop
            }

            if (validInput)
            {
                RentRequest newRequest;
                newRequest.userId = this->getUserId();

                // Get the adsId of the selected property.
                newRequest.adsId = favouriteList.getAdsIdAtIndex(selectedPropertyIndex);

                newRequest.rentAmount = propertyList.getMonthlyRentByAdsId(newRequest.adsId);

                newRequest.status = RentRequestStatus::PENDING;

                rentRequestList.insert(newRequest); // Add the request to the rent request list
                cout << "Rent request placed successfully!" << endl;
            }
        }
        else if (choice == "5")
        {
            rentRequestList.displayUserRentHistory(this->getUserId());
        }
        else if (choice == "6")
        {
            cout << "Escaping...\n";
        }
        else
        {
            cout << "Ah, the land of indecision! Enter a realm between 1 and 6, mortal.\n";
        }
    } while (choice != "6"); // This condition should check against a string
}

bool Tenant::isStringCriteria(SearchCriteria criteria)
{
    switch (criteria)
    {
    case SearchCriteria::ADS_ID:
    case SearchCriteria::PROP_NAME:
    case SearchCriteria::LOCATION:
    case SearchCriteria::PROPERTY_TYPE:
    case SearchCriteria::FURNISHED:
    case SearchCriteria::REGION:
        return true;
    default:
        return false;
    }
}


