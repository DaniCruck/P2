// DNI 55392060X ILIEV ARKHIPOV, DANIEL VALERIEV
#include <iostream>
#include <vector>
#include <fstream>
#include <cstring>
#include <sstream>

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

struct TPlayerRanking{
    int playerIndex;
    float averageRating;
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

            for(unsigned int j = 0; j < agency.players[i].ratings.size(); j++){
                cout << j + 1 << " | " << agency.players[i].ratings[j] << endl; 
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
        tempPlayer.dorsal = -1;
        cout << "Enter dorsal: ";
        cin >> tempPlayer.dorsal;
        if(tempPlayer.dorsal < 0 || tempPlayer.dorsal > 99){
            error(ERR_DORSAL);
        }
        cin.clear();
        while(cin.get() != '\n');
    }while(tempPlayer.dorsal < 0 || tempPlayer.dorsal > 99);
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
    bool isCorrect = false;
    unsigned int intId;

    if(id.empty()){
        isCorrect = false;
    }

    else{
        intId = stoi(id);
        for(unsigned int i = 0; i < agency.players.size(); i++){
            if(intId == agency.players[i].id){
                isCorrect = true;
                i = agency.players.size();
            }
        }
    }
    return isCorrect;
}

/*
Funcion para encontrar la posicion en la que esta el jugador deseado en funcion de su id
Return: el indice del vector en el que se encuentra el jugador
*/
int findPlayer(Agency agency, string id){
    int playerIndex = -1;
    unsigned int intId = stoi(id);

    for(unsigned int i = 0; i < agency.players.size(); i++){
        if(intId == agency.players[i].id){
            playerIndex = i;
            i = agency.players.size();
        }
    }
    return playerIndex;
}

/*
Funcion para borrar jugadores de la agencia
Return: void
*/
void deletePlayer(Agency &agency){
    string id;
    bool isIdCorrect;
    int playerIndex;

    cout << "Enter player id: ";
    getline(cin, id);

    isIdCorrect = checkId(agency, id);

    if(!isIdCorrect){
        error(ERR_PLAYER_NOT_EXISTS);
    }
    else{
        playerIndex = findPlayer(agency, id);
        agency.players.erase(agency.players.begin() + playerIndex);
    }
}

/*
Funcion para comprobar que el formato de las valoraciones esta correcto
Return: un bool que es true si está correcto
*/
bool checkFormat(string ratings){
    stringstream rating(ratings);
    string individualRating;
    bool isCorrect = true;

    if (ratings.empty()) {
        isCorrect = false;
    } 
    else if (ratings[0] == ',' || ratings[ratings.size() - 1] == ',') {
        isCorrect = false;
    } 
    else {
        while (isCorrect && getline(rating, individualRating, ',')) {
            
            // Si el individualRating está vacío, había dos comas seguidas (ej: "10,,20")
            if (individualRating.empty()) {
                isCorrect = false;
            } else {
                // 4. Analizamos los caracteres.
                for (size_t i = 0; i < individualRating.size() && isCorrect; i++) {
                    char c = individualRating[i];

                    // a) Comprobamos si es el primer carácter y es un signo menos
                    if (i == 0 && c == '-') {
                        // Evitamos un guion suelto (ej: "10,-,20")
                        if (individualRating.size() == 1) {
                            isCorrect = false;
                        }
                    } 
                    // b) Si no es un guion válido al inicio, comprobamos que sea un número
                    else if (c < '0' || c > '9') {
                        isCorrect = false;
                    }
                }
            }
        }
    }

    return isCorrect;
}
/*
Funcion para comprobar que las valoraciones entran dentro del rango establecido
Return: un bool que dice si se cumple el rango o no.
*/
bool checkRange(string ratings, vector<int> &tempRatings){
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
    }
    
    int lastVal = stoi(ratings.substr(start));
    if (lastVal < -50 || lastVal > 50) {
        isCorrect = false;
    }
    tempRatings.push_back(lastVal);
    
    return isCorrect;
}

/*
Funcion para añadir valoraciones a un jugador en funcion de su id
Return; void
*/
void addPlayerRating(Agency &agency){
    string playerId;
    string ratings;
    bool isIdCorrect;
    bool isValid = false;
    bool isFormatCorrect;
    bool isRangeCorrect;
    vector<int> tempRatings;
    int playerIndex;

    cout << "Enter player id: ";
    getline(cin, playerId);
    isIdCorrect = checkId(agency, playerId);

    if(!isIdCorrect){
        error(ERR_PLAYER_NOT_EXISTS);
    }
    else{
        playerIndex = findPlayer(agency, playerId);
        do{
            cout << "Enter ratings (comma-separated): ";
            getline(cin, ratings);
            
            isFormatCorrect = checkFormat(ratings);
            if(!isFormatCorrect){
                error(ERR_RATING_FORMAT);
            }
            else{
                isRangeCorrect = checkRange(ratings, tempRatings);
                if(!isRangeCorrect){
                    error(ERR_RATING);
                    tempRatings.clear();
                }
                else{
                    for(unsigned int i = 0; i < tempRatings.size(); i++){
                        agency.players[playerIndex].ratings.push_back(tempRatings[i]);
                    }
                    isValid = true;
                }
            }
        }while(!isValid);
    }
}

/*
Función que calcula la valoración media de un jugador
player: jugador del que se va a calcular la nota media
return: un float con el valor medio de todas sus valoraciones
*/
float calculateAverage(Player player) {
    float sum = 0;
    for (unsigned int i = 0; i < player.ratings.size(); i++) {
        sum += player.ratings[i];
    }
    return sum / player.ratings.size();
}

/*
Función que ordena de mayor a menor un vector temporal de ranking según la nota media
ranking: vector de tipo TPlayerRanking pasado por referencia para modificarlo directamente
return: void
*/
void sortRanking(vector<TPlayerRanking> &ranking) {
    // Recorremos todo el vector
    for (unsigned int i = 0; i < ranking.size(); i++) {
        unsigned int maxIndex = i;

        // Buscamos si hay algún otro jugador en el resto del vector con una nota mayor
        for (unsigned int j = i + 1; j < ranking.size(); j++) {
            if (ranking[j].averageRating > ranking[maxIndex].averageRating) {
                maxIndex = j; // Guardamos la posición del nuevo máximo
            }
        }

        // Si encontramos un jugador con mayor nota, intercambiamos sus posiciones
        if (maxIndex != i) {
            TPlayerRanking temp = ranking[i];
            ranking[i] = ranking[maxIndex];
            ranking[maxIndex] = temp;
        }
    }
}

/*
Funcion para mostrar un ranking de los jugadores en funcion de su valoracion media
return: void
*/
void showRankings(Agency agency){
    vector<TPlayerRanking> ranking;
    TPlayerRanking tempRanking;
    float tempAverageRating = 0;

    if(agency.players.size() < 1){
        error(ERR_NO_PLAYERS_WITH_RATINGS);
    }
    else{
        for(unsigned int i = 0; i < agency.players.size(); i++){
        if(agency.players[i].ratings.size() > 0){
            tempAverageRating = calculateAverage(agency.players[i]);
            tempRanking.playerIndex = i;
            tempRanking.averageRating = tempAverageRating;
            ranking.push_back(tempRanking);
            tempAverageRating = 0;
            }
        }

        sortRanking(ranking);

        cout << "Ranking | Id | Name | Dorsal | Position | Rating" << endl;
        for(unsigned int i = 0; i < ranking.size(); i++){
            cout << i + 1 << " | ";
            cout << agency.players[ranking[i].playerIndex].id << " | ";
            cout << agency.players[ranking[i].playerIndex].name << " | ";
            cout << agency.players[ranking[i].playerIndex].dorsal << " | ";
            switch(agency.players[ranking[i].playerIndex].position){
                case 1:
                    cout << "Point Guard" << " | ";
                    break;
                case 2:
                    cout << "Shooting Guard" << " | ";
                    break;
                case 3:
                    cout << "Small Forward" << " | ";
                    break;
                case 4:
                    cout << "Power Forward" << " | ";
                    break;
                case 5:
                    cout << "Center" << " | ";
                    break;
            }
            cout << ranking[i].averageRating << endl; 
        }
    }
}

void importCsv(){
    string fileName;
    Player tempPlayer;

    cout << "Enter filename: ";
    getline(cin, fileName); 

    ifstream file(fileName);

    if(file.is_open()){
        
    }
    else{
        error(ERR_FILE);
    }
}


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
            case '2': // Export from CSV
                break;
            case '3': // Import from bin
                break;
            case '4': // Export to bin
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
                showRankings(agency);//Falta por implementar
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
