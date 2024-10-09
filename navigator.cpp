
#include "Navigator.h"

// Constructor
Navigator::Navigator(string fileName) {
    m_fileName = fileName;
}

Navigator::~Navigator() {
    //loop over all the airports stored in m_airports
    for (unsigned int i = 0; i < m_airports.size(); i++) {
        //delete the airport object pointed to by the current pointer
        delete m_airports[i];
    }
    //clear the vector of airport pointers
    m_airports.clear();

    //loop over all the routes stored in m_routes
    for (unsigned int i = 0; i < m_routes.size(); i++) {
        //delete the route object pointed to by the current pointer
        delete m_routes[i];
    }
    //clear the vector of route pointers
    m_routes.clear();
}

void Navigator::Start() {
    ReadFile();
    MainMenu();
}

void Navigator::DisplayAirports() {
    for (unsigned int i = 0; i < m_airports.size(); i++) {
        cout << i + 1 << ". " << *m_airports[i] << endl;
    }
}

void Navigator::ReadFile() {
    ifstream file(m_fileName);
    string line;
    while (getline(file, line)) {
        stringstream ss(line);
                string code, name, city, country, north, west;
        //retrieve all data from file into correct vairiables
        getline(ss, code, ',');
        getline(ss, name, ',');
        getline(ss, city, ',');
        getline(ss, country, ',');
        getline(ss, north, ',');
        getline(ss, west, ',');
        //fill the airports list
        m_airports.push_back(new Airport(code, name, city, country, stod(north), stod(west)));
    }
    file.close();
}

void Navigator::InsertNewRoute() {
    cout << "Enter the number of the airport to add to your Route: (-1 to end)" << endl;
    DisplayAirports();

    vector<int> selectedAirports;
    int selection = 0;

    // Loop until the user enters -1
    while (true) {
        cout << "Enter the number of the airport to add to your Route: (-1 to end)" << endl;
        cin >> selection;
        if (selection == -1) {
            if (selectedAirports.size() >= 2) {
                break;
            } else {
                cout << "Route cannot have less than two airports." << endl;
            }
        }

        //validate user input
        if (selection < 1 || selection > int(m_airports.size())) {
            cout << "Invalid. Enter the number of the airport to add to your Route: (-1 to end)" << endl;
        } else {
            selectedAirports.push_back(selection - 1);
        }
    }

    //create the new route and add the selected airports
    Route* newRoute = new Route();
    for (unsigned int i = 0; i < selectedAirports.size(); i++) {

        int index = selectedAirports[i];
        Airport* airport = m_airports[index];

        newRoute->InsertEnd(airport->GetCode(), airport->GetName(), airport->GetCity(), airport->GetCountry(), airport->GetNorth(), airport->GetWest());
    }

    //insert the new route into m_routes
    m_routes.push_back(newRoute);

    cout << "Done Building a New Route named " << newRoute->GetName() << endl;
}

void Navigator::MainMenu() {
    int choice = 0;
    //the main menu where user can choose what to do
    do {
        cout << "What would you like to do?" << endl;
        cout << "1. Create New Route" << endl;
        cout << "2. Display Route" << endl;
        cout << "3. Remove Airport from Route" << endl;
        cout << "4. Reverse Route" << endl;
        cout << "5. Exit" << endl;
        cin >> choice;

        switch (choice) {
            case 1:
                InsertNewRoute();
                break;
            case 2:
                DisplayRoute();
                break;
            case 3:
                RemoveAirportFromRoute();
                break;
            case 4:
                ReverseRoute();
                break;
            case 5:
                cout << "Routes removed from memory" << endl;
                cout << "Deleting Airports" << endl;
                cout << "Deleting Routes" << endl;
                break;
            default:
                cout << "Invalid." << endl;
        }
    } while (choice != 5);
}

int Navigator::ChooseRoute() {
    int choice;
    if (m_routes.empty()) {
              return -1;
    }
    //prompt the user and display routes
    cout << "Which route would you like to use?" << endl;
    for (unsigned int i = 0; i < m_routes.size(); i++) {
        cout << i + 1 << ". " << m_routes[i]->GetName() << endl;
    }
    cin >> choice;
    //validate user input
    while (choice < 1 || choice > int(m_routes.size())) {
        cout << "Invalid. Which route would you like to use?" << endl;
        cin >> choice;
    }
    return choice - 1;
}

void Navigator::DisplayRoute() {
    int index = ChooseRoute();
    if (index == -1) {
        cout << "No routes to display" << endl;
        return;
    }
    //display the route selected by the user
    Route* selectedRoute = m_routes[index];
    cout << selectedRoute->GetName() << endl;
    selectedRoute->DisplayRoute();
    cout << "The total miles of this route is " << RouteDistance(selectedRoute) << " miles" << endl;
}

void Navigator::RemoveAirportFromRoute() {
    int routeIndex = ChooseRoute();
    if (routeIndex == -1) {
        cout << "No routes to remove airports" << endl;
        return;
    }
    Route* selectedRoute = m_routes[routeIndex];
    if (selectedRoute->GetSize() <= ROUTE_MIN) {
        cout << "Route cannot have less than two airports." << endl;
        return;
    }
    //prompt the user
    cout << selectedRoute->GetName() << endl;
    selectedRoute->DisplayRoute();
    cout << "Which airport would you like to remove?" << endl;
    int airportIndex;
    cin >> airportIndex;
    //validate user input
    while (airportIndex < 1 || airportIndex > selectedRoute->GetSize()) {
              cout << "Invalid. Which airport would you like to remove?" << endl;
        cin >> airportIndex;
    }
    //remove the airport the user chooses
    selectedRoute->RemoveAirport(airportIndex - 1);
    //update and display the name and route
    selectedRoute->UpdateName();
    cout << selectedRoute->GetName() << endl;
    selectedRoute->DisplayRoute();
    cout << "Route named " << selectedRoute->GetName() << " updated" << endl;
}

double Navigator::RouteDistance(Route* route) {
    if (!route || route->GetSize() < 2) {
        return 0;
    }

    double totalDistance = 0.0;
    Airport* current = route->GetData(0);
    //making sure current and next are not nullptr
    while (current && current->GetNext()) {
        Airport* next = current->GetNext();

        //calculating the distance between the current airport and the next
        double distance = this->CalcDistance(current->GetNorth(), current->GetWest(), next->GetNorth(), next->GetWest());
        totalDistance += distance;
        current = next;
    }
    return totalDistance;
}

void Navigator::ReverseRoute() {
    int index = ChooseRoute();
    if (index == -1) {
        cout << "No routes to reverse" << endl;
        return;
    }
    //selecting the route to be reversed than calling the reverse rout function to reverse the route
    Route* selectedRoute = m_routes[index];
    selectedRoute->ReverseRoute();
    selectedRoute->UpdateName();
    cout << "Done reversing Route " << selectedRoute->GetName() << endl;
}