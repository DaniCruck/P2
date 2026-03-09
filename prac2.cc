// DNI 55392060X ILIEV ARKHIPOV, DANIEL VALERIEV
#include <iostream>
#include <vector>
#include <fstream>
#include <cstring>

using namespace std;

const int kMAXSTRING=50;

struct Player{
    unsigned int id;
    string name;
    string team;
    unsigned int dorsal;
    unsigned int position;
    vector<int> ratings;
};

struct BinPlayer{
    unsigned int id;
    char name[kMAXSTRING];
    char team[kMAXSTRING];
    unsigned int dorsal;
    unsigned int position;
    unsigned int numRatings;
};

struct Agency{
    string name;
    vector<Player> players;
    unsigned int nextId;
};

struct BinAgency{
    char name[kMAXSTRING];
    unsigned int nextId;
};

// Tipos de errores posibles
enum Error{
    ERR_ARGS,
    ERR_DORSAL,
    ERR_FILE,
    ERR_NAME,
    ERR_NAME_EXISTS,
    ERR_NO_PLAYERS_WITH_RATINGS,
    ERR_OPTION,
    ERR_PLAYER_NOT_EXISTS,
    ERR_POSITION,
    ERR_RATING,
    ERR_RATING_FORMAT
};

/*
Función que muestra los distintos tipos de errores posibles
e: tipo de error a mostrar
return: void
*/
void error(Error e){
    switch(e){
        case ERR_ARGS:
            cout << "ERROR: wrong arguments" << endl;
            break;
        case ERR_DORSAL:
            cout << "ERROR: wrong dorsal" << endl;
            break;
        case ERR_FILE:
            cout << "ERROR: cannot open file" << endl;
            break;
        case ERR_NAME:
            cout << "ERROR: wrong name" << endl;
            break;
        case ERR_NAME_EXISTS:
            cout << "ERROR: player name already exists" << endl;
            break;
        case ERR_NO_PLAYERS_WITH_RATINGS:
            cout << "ERROR: no players with ratings" << endl;
            break;
        case ERR_OPTION:
            cout << "ERROR: wrong menu option" << endl;
            break;
        case ERR_PLAYER_NOT_EXISTS:
            cout << "ERROR: player does not exist" << endl;
            break;
            case ERR_POSITION:
            cout << "ERROR: wrong position" << endl;
            break;
        case ERR_RATING:
            cout << "ERROR: wrong rating" << endl;
            break;
        case ERR_RATING_FORMAT:
            cout << "ERROR: wrong rating format" << endl;
    }
}

/*
Función que muestra el menú de importación/exportación por pantalla
return: void
*/
void showImportExportMenu(){
    cout << "[Import/Export]" << endl
        << "1- Import from CSV" << endl
        << "2- Export to CSV" << endl
        << "3- Load data" << endl
        << "4- Save data" << endl
        << "b- Back to main menu" << endl
        << "Option: ";
}

/*
Función que muestra el menú principal por pantalla
return: void
*/
void showMainMenu(){
    cout << "[Menu]" << endl
         << "1- Show players" << endl
         << "2- Add player" << endl
         << "3- Add player rating" << endl
         << "4- Delete player" << endl
         << "5- Show ranking" << endl
         << "6- Import/export" << endl
         << "q- Quit" << endl
         << "Option: ";
}
/*
Funcion para mostrar a todos los jugadores que pertenecn a
la agencia.
Return: Void
*/
void showPlayers(Agency agency){
    for(unsigned int i = 0; i < agency.players.size(); i++){
        cout << "Id: " << agency.players[i].id << endl;
        cout << "Name: " << agency.players[i].name << endl;
        cout << "Team: " << agency.players[i].team << endl;
        cout << "Dorsal: " << agency.players[i].dorsal << endl;
        cout << "Position: ";
        
        switch(agency.players[i].position){
            case 1:
                cout << "Point Guard" << endl;
                break;
            case 2:
                cout << "Shooting Guard" << endl;
                break;
            case 3:
                cout << "Small Forward" << endl;
                break;
            case 4:
                cout << "Power Forward" << endl;
                break;
            case 5:
                cout << "Center" << endl;
                break;
        }

        if(agency.players[i].ratings.size() > 0){
            cout << "Game | Rating" << endl;

            for(unsigned int j = 0; i < agency.players[i].ratings.size(); i++){
                cout << j << " | " << agency.players[i].ratings[j] << endl; 
            }
        }
    }
}

/*
Función de comrpobacion si el nombre de jugador o equipo cumple con los
requisitos necesarios.
Return; un bool del estado de cumplimiento.
*/
bool checkName(string name, Agency agency){
    bool isFormatCorrect = true;

    if(name.empty() || name[0] == ' '){
        isFormatCorrect = false;
    }
    else{
        bool newWord = true;
        for (size_t i = 0; i < name.size(); i++) {
            if (name[i] == ' ') {
                newWord = true; // El siguiente caracter empieza una palabra
            } else if (newWord) {
                // Si es inicio de palabra, DEBE ser mayúscula ('A'-'Z')
                if (name[i] < 'A' || name[i] > 'Z') {
                    isFormatCorrect = false;
                    i = name.size();
                }
                newWord = false;
            } else {
                // Si NO es inicio de palabra, DEBE ser minúscula ('a'-'z')
                if (name[i] < 'a' || name[i] > 'z') {
                    isFormatCorrect = false;
                    i = name.size();
                }
            }
        }
    }

    if (!isFormatCorrect) {
            error(ERR_NAME);
        }

    // 3. Comprobar si el jugador ya existe en la agencia
    for (unsigned int i = 0; i < agency.players.size(); i++) {
        if (name == agency.players[i].name) {
            error(ERR_NAME_EXISTS);
        }
    }
    return isFormatCorrect;
}

/*
Fncion para poder añadir jugadores adentro de la agencia
Return: void
*/
void addPlayer(Agency &agency){
    Player tempPlayer;
    bool correct = true;
    do{
        cout << "Enter player name: ";
        getline(cin, tempPlayer.name);
        correct = checkName(tempPlayer.name, agency);
    }while(!correct);
    do{
        correct = false;
        cout << "Enter team name: ";
        getline(cin, tempPlayer.team);
        correct = checkName(tempPlayer.team, agency);
    }while(!correct);
    do{
        tempPlayer.dorsal = 0;
        cout << "Enter dorsal: ";
        cin >> tempPlayer.dorsal;
        if(tempPlayer.dorsal <= 0 || tempPlayer.dorsal > 99){
            error(ERR_DORSAL);
        }
        cin.clear();
        while(cin.get() != '\n');
    }while(tempPlayer.dorsal <= 0 || tempPlayer.dorsal > 99);
    do{
        cout << "Enter position: ";
        cin >> tempPlayer.position;
        if(tempPlayer.position < 1 || tempPlayer.position > 5){
            error(ERR_POSITION);
        }
        cin.clear();
        cin.ignore();
    }while(tempPlayer.position < 1 || tempPlayer.position > 5);
    tempPlayer.id = agency.nextId;
    tempPlayer.ratings.clear();
    agency.players.push_back(tempPlayer);
    agency.nextId ++;
}

/*
Funcion para comprobar si el id introducido pertenece a algun jugador
registrado.
Return: un boolano que dice si existe o no ese jugador.
*/
bool checkId(Agency agency, string id){
    bool existsId = false;
    unsigned int intId;
    vector<Player> players = agency.players;

    if(id.empty()){
        existsId = false;
    }

    intId = stoi(id);

    for(size_t i = 0; i < agency.players.size(); i++){
        if(intId == agency.players[i].id){
            existsId = true;
        }
    }

    return existsId;
}

/*
Funcion para borrar jugadores de la agencia
Return: void
*/
void deletePlayer(Agency &agency){
    string id;
    unsigned int intId;
    bool isIdCorrect;

    cout << "Enter player id: ";
    getline(cin, id);

    isIdCorrect = checkId(agency, id);

    if(!isIdCorrect){
        error(ERR_PLAYER_NOT_EXISTS);
    }
    else{
        intId = stoi(id);
        for(unsigned int i = 0; i < agency.players.size(); i++){
            if(agency.players[i].id == intId){
                agency.players.erase(agency.players.begin() + i);
                i = agency.players.size();
            }
        }
    }
}

bool checkFormat(string ratings){
    bool formatError = false;
        if (ratings.empty()) {
            formatError = true;
        } else {
            for (size_t i = 0; i < ratings.size(); i++) {
                char c = ratings[i];
                // Permitimos solo dígitos, el signo menos y la coma
                if (!isdigit(c) && c != '-' && c != ',') {
                    formatError = true;
                    break;
                }
                // El '-' solo puede ir al principio de un número (índice 0 o justo después de una coma)
                if (c == '-' && i != 0 && ratings[i - 1] != ',') {
                    formatError = true; 
                    break;
                }
                // Evitar comas al inicio, al final, o comas consecutivas (",,")
                if (c == ',' && (i == 0 || i == ratings.size() - 1 || ratings[i - 1] == ',')) {
                    formatError = true; 
                    break;
                }
            }
        }

    return formatError;
}

bool checkRange(string ratings, vector<int> tempRatings){
    bool isCorrect = true;
    size_t start = 0;
    size_t end = ratings.find(',');

    while(end != string::npos){
        int value = stoi(ratings.substr(start, end - start));
        if(value < -50 || value > 50){
            isCorrect = false;
        }
        tempRatings.push_back(value);

        start = end + 1;
        end = ratings.find(',', start);

        int lastVal = stoi(ratings.substr(start));
        if (lastVal < -50 || lastVal > 50) {
            isCorrect = false;
        }
        tempRatings.push_back(lastVal);
    }
    return isCorrect;
}

/*
Funcion para añadir valoraciones de partido a un jugador
return: void
*/
void addPlayerRating(Agency &agency){
    bool idCorrect;
    bool validFormat;
    string id;
    string ratings;
    vector<int> tempRatings;
    unsigned int intId;

    cout << "Enter player id: ";
    getline(cin, id);

    idCorrect = checkId(agency, id); //Comprobación si existe un jugador con ese id
    if(!idCorrect){
        error(ERR_PLAYER_NOT_EXISTS);
    }
    else{
        intId = stoi(id);
        do{
            cout << "Enter ratings (comma-separated): ";
            getline(cin, ratings);
            validFormat = checkFormat(ratings);

            if(!validFormat){
                error(ERR_RATING_FORMAT);
            }
            else{
                validFormat = checkRange(ratings, tempRatings);
                if(!validFormat){
                    error(ERR_RATING);
                    tempRatings.clear();
                }
            }

        }while(!validFormat);
        for (unsigned int i = 0; i < tempRatings.size(); i++) {
        agency.players[intId].ratings.push_back(tempRatings[i]);
        }
    }
}

/*
Funcion para mostrar un ranking de los jugadores en funcion de su valoracion media
return: void
*/
void showRankings(Agency agency);

/*
Función para importar o exportar datos en ficheros CSV
o binarios
*/
void importExport(){
    char option;
    do{
        showImportExportMenu();
        cin >> option;
        cin.get();

        switch(option){
            case '1': // Import from CSV
                break;
            case '2': //Export from CSV
                break;
            case '3': //
                break;
            case '4':
                break;
            case 'b':
                break;
            default:
                error(ERR_OPTION);
        }
    }while(option != 'b');
}

/*
Función principal del programa, que muestra el menú principal por pantalla y permite al usuario elegir una opción
return: 0
*/
int main(int argc, char *argv[]){
    Agency agency;
    agency.name="ProSports Agency";
    agency.nextId=1;

    char option;
    do{
        showMainMenu();
        cin >> option;
        cin.get();

        switch(option){
            case '1': // Show players
                showPlayers(agency);
                break;
            case '2': // Add player
                addPlayer(agency);
                break;
            case '3': // Add player rating
                addPlayerRating(agency); //Fakta por implementar
                break;
            case '4':
                deletePlayer(agency); // Delete player
                break;
            case '5': // Show ranking
                //Falta por implementar
                break;
            case '6': // Import/export menu
                importExport(); //Falta por terminar
                break;
            case 'q':
                break;
            default: error(ERR_OPTION);
        }
    }while(option!='q');

    return 0;
}
