
#include <iostream>
#include <cstdlib>
#include <cctype>
#include <string>
#include <iomanip>
#include <fstream>
#include <vector>

using namespace std;
const string ClientsFileName = "Clients.txt";

void ShowMainMenu();

enum enMainMenuOptions { eClientList = 1, eAddNewClient = 2, eDeleteClient = 3, eUpdateClientInfo = 4, eFindClient = 5, Exit = 6 };

struct stClient {

    string AccountNumber = "";
    string PinCode = "";
    string Name = "";
    string Phone = "";
    double AccountBalance = 0;
    bool MarkForDelete = false;
};

vector <string> SplitString(string Line, string Seperator) {

    vector <string> vRecord;
    short pos = 0;
    string word = "";

    while ((pos = Line.find(Seperator)) != Line.npos) {

        word = Line.substr(0, pos);
        if (word != "") {

            vRecord.push_back(word);
        }
        Line.erase(0, pos + Seperator.length());
    }
    if (Line != "") {

        vRecord.push_back(Line);
    }
    return vRecord;
}

stClient ConvertLineToRecord(string Line, string Seperator = "#//#") {

    vector <string> vRecord;
    stClient Client;

    vRecord = SplitString(Line, Seperator);

    Client.AccountNumber = vRecord[0];
    Client.PinCode = vRecord[1];
    Client.Name = vRecord[2];
    Client.Phone = vRecord[3];
    Client.AccountBalance = stod(vRecord[4]);

    return Client;
}

vector <stClient> LoadClientData(string FileName) {

    fstream ClientsFile;
    vector <stClient> vClientData;

    ClientsFile.open(FileName, ios::in);

    if (ClientsFile.is_open()) {

        string Line;
        stClient Client;

        while (getline(ClientsFile, Line)) {

            Client = ConvertLineToRecord(Line);
            vClientData.push_back(Client);
        }
        ClientsFile.close();
    }
    return vClientData;
}

void PrintClientData(stClient Client) {

    cout << "| " << left << setw(17) << Client.AccountNumber;
    cout << "| " << left << setw(11) << Client.PinCode;
    cout << "| " << left << setw(30) << Client.Name;
    cout << "| " << left << setw(15) << Client.Phone;
    cout << "| " << left << setw(15) << Client.AccountBalance << "     |";
}

void ShowAllClientsScreen() {

    vector <stClient> vClientData = LoadClientData(ClientsFileName);

    cout << "\n\t\t\t\tClient List (" << vClientData.size() << ") Client (s).\n";
    cout << "_________________________________________________";
    cout << "_______________________________________________________\n\n";
    cout << "| " << left << setw(17) << "Account Number";
    cout << "| " << left << setw(11) << "Pin Code";
    cout << "| " << left << setw(30) << "Client Name";
    cout << "| " << left << setw(15) << "Phone";
    cout << "| " << left << setw(15) << "Account Balance     |";
    cout << "\n_________________________________________________";
    cout << "_______________________________________________________\n" << endl;
    
    for (stClient& Client : vClientData) {

        PrintClientData(Client);
        cout << endl;
    }

    cout << "_________________________________________________";
    cout << "_______________________________________________________";

}

void ShowProgramEndScreen() {

    cout << "------------------------------------\n";
    cout << "\tProgram Ends :-)\n";
    cout << "------------------------------------\n";
}

bool ClientExistsByAccountNumber(string AccountNumber, string FileName) {

    fstream ClientsFile;
    ClientsFile.open(FileName, ios::in);

    if (ClientsFile.is_open()) {

        string DataLine;
        stClient Client;

        while (getline(ClientsFile, DataLine)) {

            Client = ConvertLineToRecord(DataLine);

            if (Client.AccountNumber == AccountNumber) {

                ClientsFile.close();
                return true;
            }
        }
        return false;
    }
}

stClient ReadNewClient() {

    stClient Client;

    cout << "\n\n\Enter Account Number ? ";
    getline(cin >> ws, Client.AccountNumber);

    while (ClientExistsByAccountNumber(Client.AccountNumber, ClientsFileName)) {

        cout << "Client with Account Number [" << Client.AccountNumber << "] already exists, Enter another Account Number ? ";
        getline(cin >> ws, Client.AccountNumber);
    }

    cout << "Enter PinCode ? ";
    getline(cin, Client.PinCode);
    cout << "Enter Name ? ";
    getline(cin, Client.Name);
    cout << "Enter Phone ? ";
    getline(cin, Client.Phone);
    cout << "Enter Account Balace ? ";
    cin >> Client.AccountBalance;

    return Client;
}

string ConvertRecordToLine(stClient Client, string Seperator = "#//#") {

    string Line;

    Line += Client.AccountNumber + Seperator;
    Line += Client.PinCode + Seperator;
    Line += Client.Name + Seperator;
    Line += Client.Phone + Seperator;
    Line += to_string(Client.AccountBalance);

    return Line;
}

void AddDataLineToFile(string FileName, string DataLine) {

    fstream ClientsFile;
    ClientsFile.open(FileName, ios::out | ios::app);

    if (ClientsFile.is_open()) {

        ClientsFile << DataLine << endl;

        ClientsFile.close();
    }
}

void AddNewClient() {

    stClient Client;
    Client = ReadNewClient();
    AddDataLineToFile(ClientsFileName, ConvertRecordToLine(Client));
}

void ShowAddNewClientsScreen() {

    cout << "------------------------------------\n";
    cout << "\tAdd New Clients Screen\n";
    cout << "------------------------------------\n";
}

void AddNewClients() {

    char AddMore = 'Y';
   
    do {

        system("cls");

        ShowAddNewClientsScreen();

        cout << "Adding New Client:\n\n";

        AddNewClient();
        cout << "\n\nClient Added Successfuly, do you want to add more clients? Y/N ";
        cin >> AddMore;

    } while (toupper(AddMore) == 'Y');
    
}

string ReadClientAccountNumber() {

    string AccountNumber;
    cout << "Please enter Account Number ? ";
    cin >> AccountNumber;

    return AccountNumber;
}

bool FindClientByAccountNumber(string AccountNumber, vector <stClient> vClientData, stClient& Client) {

    for (stClient& sClient : vClientData) {

        if (sClient.AccountNumber == AccountNumber) {

            Client = sClient;
            return true;
        }
    }
    return false;
}

void PrintClientCard(stClient Client) {

    cout << "\n\nThe following are the Client Details:\n";
    cout << "------------------------------------------\n";
    cout << "Account Number : " << Client.AccountNumber << endl;
    cout << "Pin Code       : " << Client.PinCode << endl;
    cout << "Name           : " << Client.Name << endl;
    cout << "Phone          : " << Client.Phone << endl;
    cout << "Account Balance : " << Client.AccountBalance << endl;
    cout << "------------------------------------------\n";

}

bool MarkClientByAccountNumberForDelete(string AccountNumber, vector <stClient>& vClientData) {

    for (stClient& Client : vClientData) {

        if (Client.AccountNumber == AccountNumber) {

            Client.MarkForDelete = true;
            return true;
        }
    }
    return false;
}

void SaveClientDataToFile(string FileName, vector <stClient> vClientData) {

    fstream ClientsFile;
    ClientsFile.open(FileName, ios::out);

    string DataLine;

    if (ClientsFile.is_open()) {

        for (stClient& Client : vClientData) {

            if (Client.MarkForDelete == false) {

                DataLine = ConvertRecordToLine(Client);
                ClientsFile << DataLine << endl;
            }
        }
        ClientsFile.close();
    }
}

bool DeleteClientsByAccountNumber(string AccountNumber, vector <stClient>& vClientsData) {

    stClient Client;

    char DeleteClient = 'n';

    if (FindClientByAccountNumber(AccountNumber, vClientsData, Client)) {

        PrintClientCard(Client);
       
        cout << "\nAre you sure you want to delete this client? y/n ? ";
        cin >> DeleteClient;
        if (toupper(DeleteClient) == 'Y') {

            MarkClientByAccountNumberForDelete(AccountNumber, vClientsData);
            SaveClientDataToFile(ClientsFileName, vClientsData);

            //Refresh vector after client delete.
            vClientsData = LoadClientData(ClientsFileName);

            cout << "\n\nClient Deleted successfuly.";
            return true;
        }
    }
    else {

        cout << "\n\nClient with Account Number (" << AccountNumber << ") is Not Found!.";
        return false;
    }
}

void ShowDeleteClientScreen() {

    cout << "------------------------------------\n";
    cout << "\tDelete Client Screen\n";
    cout << "------------------------------------\n";
    
    vector <stClient> vClientsData = LoadClientData(ClientsFileName);
    string AccountNumber = ReadClientAccountNumber();
    DeleteClientsByAccountNumber(AccountNumber, vClientsData);
}

stClient ChangeClientInfo(stClient Client) {


    cout << "\n\nEnter PinCode ? ";
    getline(cin >> ws, Client.PinCode);
    cout << "Enter Name ? ";
    getline(cin, Client.Name);
    cout << "Enter Phone ? ";
    getline(cin, Client.Phone);
    cout << "Enter Account Balace ? ";
    cin >> Client.AccountBalance;

    return Client;
}

bool UpdateClientInfoByAccountNumber(string AccountNumber, vector <stClient>& vClientData) {

    stClient Client;
    char Answer;

    if (FindClientByAccountNumber(AccountNumber, vClientData, Client)) {

        PrintClientCard(Client);

        cout << "\n\nAre you sure you want to Update This Client? y/n ? ";
        cin >> Answer;
        if (toupper(Answer) == 'Y') {

            for (stClient& Client : vClientData) {

                if (Client.AccountNumber == AccountNumber) {

                    Client = ChangeClientInfo(Client);
                    break;
                }
            }

            SaveClientDataToFile(ClientsFileName, vClientData);
            cout << "\n\nClient Updated successfuly.";

            return true;
        }
    }
}

void ShowUpdateClientScreen() {

    cout << "------------------------------------\n";
    cout << "\tUpdate Client Info Screen\n";
    cout << "------------------------------------\n";

    vector <stClient> vClientsData = LoadClientData(ClientsFileName);
    string AccountNumber = ReadClientAccountNumber();
    UpdateClientInfoByAccountNumber(AccountNumber, vClientsData);
}

void ShowFindClientScreen() {

    cout << "------------------------------------\n";
    cout << "\tFind Client Screen\n";
    cout << "------------------------------------\n\n\n";

    vector <stClient> vClientData = LoadClientData(ClientsFileName);
    string AccountNumber = ReadClientAccountNumber();
    stClient Client;

    if (FindClientByAccountNumber(AccountNumber, vClientData, Client)) {

        PrintClientCard(Client);

    }
    else {

        cout << "\n\nClient with Account Number(" << AccountNumber << ") is Not Found!.";
    }   
}

void GoBackToMainMenu() {

    cout << "\n\n\nPress any key to go back to Main Menu...";
    system("pause > 0");
    ShowMainMenu();
}

void PerformMainMenuOption(enMainMenuOptions MainMenuOption) {

    switch (MainMenuOption)
    {
    case eClientList:

        system("cls");
        ShowAllClientsScreen();
        GoBackToMainMenu();
        break;
    case eAddNewClient:

        system("cls");
        AddNewClients();
        GoBackToMainMenu();
        break;
    case eDeleteClient:

        system("cls");
        ShowDeleteClientScreen();
        GoBackToMainMenu();
        break;
    case eUpdateClientInfo:

        system("cls");
        ShowUpdateClientScreen();
        GoBackToMainMenu();
        break;
    case eFindClient:

        system("cls");
        ShowFindClientScreen();
        GoBackToMainMenu();
        break;
    case Exit:

        system("cls");
        ShowProgramEndScreen();
        system("pause > 0");
        break;
    default:
        break;
    }
}

short ReadMainMenuOption() {

    short UserChoice;
    cout << "Choose what do you want to do? [1 To 6]? ";
    cin >> UserChoice;

    return UserChoice;
}

void ShowMainMenu() {

    system("cls");
    cout << "=====================================================\n";
    cout << "\t\tMain Menu Screen\n";
    cout << "=====================================================\n";
    cout << "\t[1] Show Client List.\n";
    cout << "\t[2] Add New Client.\n";
    cout << "\t[3] Delete Client.\n";
    cout << "\t[4] Update Client Info.\n";
    cout << "\t[5] Find Client.\n";
    cout << "\t[6] Exit.\n";
    cout << "=====================================================\n";
    PerformMainMenuOption((enMainMenuOptions)ReadMainMenuOption());
}

int main()
{   

    ShowMainMenu();
    system("pause > 0");
    
    return 0;
}
