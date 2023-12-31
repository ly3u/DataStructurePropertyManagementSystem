#include "CSVReader.hpp"
#include "LinkedList.hpp"
#include <iostream>

using namespace std;

User *login(const string &username, const string &password, LinkedList &adminList, LinkedList &managerList, LinkedList &tenantList, FavouritePropertyLinkedList &favouriteList, RentRequestLinkedList &rentRequestList, PropertyLinkedList &propertyList, Admin &admin, Manager &manager, Tenant &tenant)
{
    User *user = adminList.login(username, password);
    if (user != NULL)
    {
        cout << "Let the grand spectacle of control and regulation unfold! Welcome, \n"
             << user->username << "!\n";
        admin.adminMenu(tenantList, managerList);
        return user;
    }

    user = managerList.login(username, password);
    if (user != NULL)
    {
        cout << "It's showtime – time to orchestrate the daily madness! Welcome, \n"
             << user->username << "!\n";
        manager.managerMenu(favouriteList, rentRequestList, propertyList, tenantList);
        return user;
    }

    user = tenantList.login(username, password);
    if (user != NULL)
    {
        cout << "Welcome to your digital abode, seeker of dwellings! Hello, \n"
             << user->username << "!\n";
        Tenant *loggedInTenant = dynamic_cast<Tenant *>(user);
        if (loggedInTenant)
        {
            loggedInTenant->tenantMenu(user, favouriteList, rentRequestList, propertyList); // Pass the logged-in tenant user to the tenantMenu
        }
        return user;
    }

    cout << "Access denied – the digital gates remain locked!\n";

    return NULL;
}

string getLargestUserIdFromTenantList(LinkedList &tenantList)
{
    string maxId = "0"; // Start with an initial value
    Node *current = tenantList.begin();

    while (current)
    {
        Tenant *tenant = dynamic_cast<Tenant *>(current->data);
        if (tenant)
        {
            if (tenant->getUserId() > maxId)
            {
                maxId = tenant->getUserId();
            }
        }
        current = current->next;
    }

    return maxId;
}

bool isUsernameUnique(const string &username, const LinkedList &adminList, const LinkedList &managerList, const LinkedList &tenantList)
{
    // Check in all lists. If found in any, return false.
    if (adminList.searchByUsername(username) || managerList.searchByUsername(username) || tenantList.searchByUsername(username))
    {
        return false;
    }
    return true;
}

Tenant *registerTenant(LinkedList &tenantList)
{
    string username, password, name;
    int age;

    // Check for duplicate username
    while (true)
    {
        cout << "Enter desired username: ";
        cin >> username;

        Node *current = tenantList.begin();
        bool isDuplicate = false;

        while (current)
        {
            Tenant *tenant = dynamic_cast<Tenant *>(current->data);
            if (tenant && tenant->getUsername() == username)
            {
                cout << "Username already taken. Please choose another." << endl;
                isDuplicate = true;
                break;
            }
            current = current->next;
        }

        if (!isDuplicate)
            break; // exit the loop if no duplicate is found
    }

    // Prompt for password
    cout << "Enter password: ";
    cin >> password;

    cin.ignore(); // To clear the newline character from the buffer

    // Prompt for name
    cout << "Enter your full name: ";
    getline(cin, name);

    // Prompt for age
    cout << "Enter your age: ";
    cin >> age;

    // Assuming lastLogin is set to some default values during registration
    tm lastLogin = {0, 0, 0, 1, 0, 100};

    string largestUserId = getLargestUserIdFromTenantList(tenantList);
    string newUserId = to_string(stoi(largestUserId) + 1);

    // Create a new Tenant object
    Tenant *newTenant = new Tenant(newUserId, username, password, name, age, lastLogin);

    return newTenant;
}

void initialiseUser(LinkedList &adminList, LinkedList &managerList, LinkedList &tenantList)
{
    // Initialize Admin
    Admin *newAdmin = new Admin("1", "admin", "admin");
    adminList.insert(newAdmin);

    // Initialize Managers
    Manager *newManager = new Manager("11", "manager", "manager", 1);
    Manager *newManager1 = new Manager("12", "manager1", "manager1", 0);
    Manager *newManager2 = new Manager("13", "manager2", "manager2", 2);
    managerList.insert(newManager);
    managerList.insert(newManager1);
    managerList.insert(newManager2);

    // Initialize Tenants
    Tenant *newTenant = new Tenant("101", "tenant", "tenant", "yuwei", 21, {0, 0, 0, 1, 0, 100});
    Tenant *newTenant1 = new Tenant("102", "tenant1", "tenant1", "john", 30, {0, 1, 0, 1, 1, 50});
    Tenant *newTenant2 = new Tenant("103", "tenant2", "tenant2", "emma", 25, {1, 1, 0, 1, 0, 75});
    Tenant *newTenant3 = new Tenant("104", "tenant3", "tenant3", "sophia", 28, {1, 0, 1, 0, 0, 90});
    Tenant *newTenant4 = new Tenant("105", "tenant4", "tenant4", "liam", 22, {0, 0, 1, 1, 1, 60});
    tenantList.insert(newTenant);
    tenantList.insert(newTenant1);
    tenantList.insert(newTenant2);
    tenantList.insert(newTenant3);
    tenantList.insert(newTenant4);
}

void initialiseFavourites(FavouritePropertyLinkedList &favouriteList)
{
    FavouriteProperty newFavouriteProperty;
    newFavouriteProperty.ads_id = "100322885";
    newFavouriteProperty.userId = "101";
    favouriteList.insert(newFavouriteProperty);

    newFavouriteProperty.ads_id = "100320327";
    newFavouriteProperty.userId = "101";
    favouriteList.insert(newFavouriteProperty);

    newFavouriteProperty.ads_id = "99528357";
    newFavouriteProperty.userId = "101";
    favouriteList.insert(newFavouriteProperty);

    newFavouriteProperty.ads_id = "100322885";
    newFavouriteProperty.userId = "102";
    favouriteList.insert(newFavouriteProperty);

    newFavouriteProperty.ads_id = "100136931";
    newFavouriteProperty.userId = "102";
    favouriteList.insert(newFavouriteProperty);

    newFavouriteProperty.ads_id = "100322885";
    newFavouriteProperty.userId = "103";
    favouriteList.insert(newFavouriteProperty);

    newFavouriteProperty.ads_id = "99528357";
    newFavouriteProperty.userId = "103";
    favouriteList.insert(newFavouriteProperty);

    newFavouriteProperty.ads_id = "100322885";
    newFavouriteProperty.userId = "104";
    favouriteList.insert(newFavouriteProperty);

    newFavouriteProperty.ads_id = "100136931";
    newFavouriteProperty.userId = "104";
    favouriteList.insert(newFavouriteProperty);

    newFavouriteProperty.ads_id = "100321008";
    newFavouriteProperty.userId = "104";
    favouriteList.insert(newFavouriteProperty);

    newFavouriteProperty.ads_id = "100322885";
    newFavouriteProperty.userId = "105";
    favouriteList.insert(newFavouriteProperty);

    newFavouriteProperty.ads_id = "100136931";
    newFavouriteProperty.userId = "105";
    favouriteList.insert(newFavouriteProperty);

    newFavouriteProperty.ads_id = "100318501";
    newFavouriteProperty.userId = "105";
    favouriteList.insert(newFavouriteProperty);

    newFavouriteProperty.ads_id = "100208556";
    newFavouriteProperty.userId = "105";
    favouriteList.insert(newFavouriteProperty);

    newFavouriteProperty.ads_id = "100227255";
    newFavouriteProperty.userId = "105";
    favouriteList.insert(newFavouriteProperty);

    newFavouriteProperty.ads_id = "100316108";
    newFavouriteProperty.userId = "105";
    favouriteList.insert(newFavouriteProperty);

    newFavouriteProperty.ads_id = "100314994";
    newFavouriteProperty.userId = "105";
    favouriteList.insert(newFavouriteProperty);
}

void initialiseRentRequest(RentRequestLinkedList &rentRequestList)
{
    RentRequest newRequest;

    newRequest.adsId = "100322885";
    newRequest.userId = "101";
    newRequest.rentAmount = 1550;
    newRequest.status = RentRequestStatus::PENDING;
    rentRequestList.insert(newRequest);

    newRequest.adsId = "100136931";
    newRequest.userId = "102";
    newRequest.rentAmount = 1900;
    newRequest.status = RentRequestStatus::DECLINED;
    rentRequestList.insert(newRequest);

    newRequest.adsId = "100316108";
    newRequest.userId = "105";
    newRequest.rentAmount = 1800;
    newRequest.status = RentRequestStatus::APPROVED;

    newRequest.adsId = "100314994";
    newRequest.userId = "105";
    newRequest.rentAmount = 2800;
    newRequest.status = RentRequestStatus::PAID;

    rentRequestList.insert(newRequest);
}
int main()
{
    // Initialise User Object
    Admin admin("userId", "username", "password");
    Manager manager("userId", "username", "password", true);
    Tenant tenant("userId", "username", "password", "Test Tenant", 30, {0, 0, 0, 1, 0, 100});

    LinkedList adminList;
    LinkedList managerList;
    LinkedList tenantList;

    // Get username and password from user
    string username, password;
    string choice;
    PropertyLinkedList propertyList;
    readProperties(propertyList);
    FavouritePropertyLinkedList favouriteList;
    RentRequestLinkedList rentRequestList;

    initialiseUser(adminList, managerList, tenantList);
    initialiseFavourites(favouriteList);
    initialiseRentRequest(rentRequestList);

    do
    {
        cout << "\nStep right into the property management circus..." << endl;
        cout << "1. Login" << endl;
        cout << "2. Register as Tenant" << endl;
        cout << "Enter your choice: ";
        cin >> choice;

        if (choice == "1") // Login
        {
            cout << "Enter username: ";
            cin >> username;
            cout << "Enter password: ";
            cin >> password;

            // Login
            User *user = login(username, password, adminList, managerList, tenantList, favouriteList, rentRequestList, propertyList, admin, manager, tenant);
        }
        else if (choice == "2") // Register as Tenant
        {
            Tenant *newTenant = registerTenant(tenantList);
            if (newTenant)
            {
                tenantList.insert(newTenant);
                cout << "Tenant registered successfully!" << endl;
            }
            else
            {
                cout << "Failed to register tenant." << endl;
            }
        }

    } while (true);

    return 0;
}
