#include <iostream>
#include <string>
#include <fstream>
#include <stdexcept>
using namespace std;

string companyName = "AutoWorld Motors";

class Vehicle
{
private:
    string brand;
    string model;

public:
    static int count;

    Vehicle(string b, string m)
    {
        brand = b;
        model = m;
        count++;
    }

    virtual void displayInfo()
    {
        cout << "Company: " << companyName << endl;
        cout << "Brand: " << brand << endl;
        cout << "Model: " << model << endl;
    }

    void updateModel(string newModel)
    {
        model = newModel;
        cout << "Model updated to: " << model << endl;
    }

    string getBrand() const { return brand; }
    string getModel() const { return model; }

    static int getCount() { return count; }

    virtual void writeToFile(ofstream& file)
    {
        file << "Company: " << companyName << endl;
        file << "Brand: " << brand << endl;
        file << "Model: " << model << endl;
    }

    virtual ~Vehicle() {}
};

int Vehicle::count = 0;

class Car : public Vehicle
{
private:
    int seats;

public:
    Car(string b, string m, int s) : Vehicle(b, m)
    {
        if (s < 0)
            throw invalid_argument("Error: Number of seats cannot be negative!");
        seats = s;
    }

    void displayInfo() override
    {
        Vehicle::displayInfo();
        cout << "Seats: " << seats << endl;
        cout << "------------------------" << endl;
    }

    void writeToFile(ofstream& file) override
    {
        Vehicle::writeToFile(file);
        file << "Type: Car" << endl;
        file << "Seats: " << seats << endl;
        file << "------------------------" << endl;
    }
};

class Motorbike : public Vehicle
{
private:
    int engineCapacity;

public:
    Motorbike(string b, string m, int ec) : Vehicle(b, m)
    {
        if (ec < 0)
            throw invalid_argument("Error: Engine capacity cannot be negative!");
        engineCapacity = ec;
    }

    void displayInfo() override
    {
        Vehicle::displayInfo();
        cout << "Engine Capacity: " << engineCapacity << " cc" << endl;
        cout << "------------------------" << endl;
    }

    void writeToFile(ofstream& file) override
    {
        Vehicle::writeToFile(file);
        file << "Type: Motorbike" << endl;
        file << "Engine Capacity: " << engineCapacity << " cc" << endl;
        file << "------------------------" << endl;
    }
};

// Write vehicles to file
void writeToFile(Vehicle* vehicles[], int size)
{
    ofstream file("vehicles.txt");

    if (!file)
    {
        cout << "Error opening file!" << endl;
        return;
    }

    file << "Vehicle Summary (Company: " << companyName << ")\n";
    file << "Total vehicles: " << Vehicle::getCount() << "\n\n";

    for (int i = 0; i < size; i++)
    {
        if (vehicles[i] != nullptr)
        {
            file << "Vehicle #" << (i + 1) << ":\n";
            vehicles[i]->writeToFile(file);
        }
    }

    file.close();
}


void readFromFile()
{
    ifstream file("vehicles.txt");

    if (!file)
    {
        cout << "Error: File not found!" << endl;
        return;
    }

    cout << "\n=== Reading vehicles.txt ===\n";
    string line;
    while (getline(file, line))
        cout << line << endl;
}

int main()
{
    const int size = 10;
    Vehicle* vehicles[size] = { nullptr };
    char option;

    do
    {
        cout << "\n=== Vehicle Management System ===\n";
        cout << "1 - Insert new vehicle\n";
        cout << "2 - Display list of every vehicle";
        cout << "3 - Update vehicle model\n";
        cout << "4 - Write to file\n";
        cout << "5- Read file\n";
        cout << "6 - Quit\n";
        cout << "Insert Your Option here: ";
        cin >> option;
        cin.ignore();

        switch (option)
        {

        case 1:
        {
            int index = -1;
            for (int i = 0; i < size; i++)
                if (vehicles[i] == nullptr) 
                { 
                    index = i; 
                    break; 
                }

            if (index == -1) 
            { 
                cout << "List full!\n";
                 break; 
            }

            cout << "Enter type (1= Car, 2= Motorbike): ";
            int type;
             cin >> type; 

            string make, model;
            cout << "Enter make: "; getline(cin, make);
            cout << "Enter model: "; getline(cin, model);

            if (type == 1)
            {
                int seats;
                cout << "Seats: "; 
                cin >> seats;
                cin.ignore();
                try { vehicles[index] = new Car(make, model, seats); }
                catch (exception& e) 
                { 
                    cout << e.what() << endl; 
                }
            }
            else if (type == 2)
            {
                int cc;
                cout << "Engine CC: "; cin >> cc;
                cin.ignore();
                try { vehicles[index] = new Motorbike(make, model, cc); }
                catch (exception& e) { cout << e.what() << endl; }
            }
            else cout << "Invalid type.\n";

            break;
        }

        
        case 2:
            if (Vehicle::getCount() == 0)
                cout << "No vehicles.\n";
            else
            {
                cout << "\n=== Vehicle List ===\n";
                for (int i = 0; i < size; i++)
                    if (vehicles[i] != nullptr)
                        vehicles[i]->displayInfo();
            }
            break;

  
        case 3:
        {
            if (Vehicle::getCount() == 0)
            {
                cout << "No vehicles.\n";
                break;
            }

            int map[20];
            int n = 1;
            for (int i = 0; i < size; i++)
            {
                if (vehicles[i] != nullptr)
                {
                    cout << n << ". ";
                    vehicles[i]->displayInfo();
                    map[n] = i;
                    n++;
                }
            }

            cout << "Choose vehicle to update: ";
            int choice; cin >> choice; cin.ignore();

            if (choice < 1 || choice >= n)
            {
                cout << "Invalid choice.\n";
                break;
            }

            cout << "Enter new model: ";
            string newModel;
            getline(cin, newModel);

            vehicles[map[choice]]->updateModel(newModel);
            break;
        }

        
        case 4:
            writeToFile(vehicles, size);
            cout << "Written to file.\n";
            break;

        
        case 5:
            readFromFile();
            break;

        case 6:
            cout << "Exit\n";
            break;

        default:
            cout << "Invalid option.\n";
        }

    } while (option != 'Q' && option != 'q');

    for (int i = 0; i < size; i++)
        delete vehicles[i];
}
