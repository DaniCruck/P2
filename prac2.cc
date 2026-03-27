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
Función para mostrar a todos los jugadores de la agencia.
agency: registro de la agencia declarada en main.
return: void
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
Función que comprueba el formato de nombre en general, donde se modifica el booleano.
names: los distintos nombres que recibe la función para comprobar su formato.
correctFormat: booleano que comprueba si el formato es correcto
return: void
*/
void checkNameFormat(string names, bool &correctFormat){
    bool newWord = true;
        for (size_t i = 0; i < names.size(); i++) {
            if(names.length() == 1){
                correctFormat = false;
                i = names.size();
            }
            else{
                if (newWord) {
                // Si es inicio de palabra, DEBE ser mayúscula ('A'-'Z')
                if (names[i] < 'A' || names[i] > 'Z') {
                    correctFormat = false;
                    i = names.size();
                }
                newWord = false;
                } else {
                    // Si NO es inicio de palabra, DEBE ser minúscula ('a'-'z')
                    if (names[i] < 'a' || names[i] > 'z') {
                        correctFormat = false;
                        i = names.size();
                    }
                }
            }
            
        }
}

/*
Función para comprobar si el nombre cumple los requisitos para añadir al
jugador
fullName: Nombre del jugador o del equipo
agency: registro de la agencia declarada en main
return: un bool del estado de cumplimiento.
*/
bool checkName(string fullName, Agency agency){
    bool isNameCorrect = true;
    string name;
    string surname;

    if(fullName.empty() || fullName[0] == ' '){
        isNameCorrect = false;
    }
    else{
        stringstream names (fullName);
        getline(names, name, ' ');
        checkNameFormat(name, isNameCorrect);

        getline(names, surname);
        checkNameFormat(surname, isNameCorrect);
    }

    if (!isNameCorrect) {
            error(ERR_NAME);
        }

    // 3. Comprobar si el jugador ya existe en la agencia
    for (unsigned int i = 0; i < agency.players.size(); i++) {
        if (name == agency.players[i].name) {
            error(ERR_NAME_EXISTS);
        }
    }
    return isNameCorrect;
}

/*
Función para poder añadir jugadores adentro de la agencia
agency: registro de la agencia declarado en main. 
return: void
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
Función que comprueba si el id introducido pertenece a algun jugador
registrado.
agency: registro de la agencia
id: cadena donde se almacena el id del jugador deseado.
return: un boolano que dice si existe o no ese jugador.
*/
bool checkId(Agency agency, string id){
    bool isIdCorrect = false;
    unsigned int intId;

    if(id.empty()){
        isIdCorrect = false;
    }

    else{
        intId = stoi(id);
        for(unsigned int i = 0; i < agency.players.size(); i++){
            if(intId == agency.players[i].id){
                isIdCorrect = true;
                i = agency.players.size();
            }
        }
    }
    return isIdCorrect;
}

/*
Función que encuentra la posicion en la que esta el jugador deseado en función de su id
agency: registro de la agencia.
id: cadena del id del jugador deseado.
return: el indice del vector en el que se encuentra el jugador
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
Función que borra jugadores de la agencia
agency: registro de la agencia.
return: void
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
Función que verifica si el formato de las valoraciones esta correcto.
ratings: cadena de valoraciones del jugador.
return: un bool que es true si está correcto
*/
bool checkFormat(string ratings){
    stringstream rating(ratings);
    string individualRating;
    bool isFormatCorrect = true;

    if (ratings.empty()) {
        isFormatCorrect = false;
    } 
    else if (ratings[0] == ',' || ratings[ratings.size() - 1] == ',') {
        isFormatCorrect = false;
    } 
    else {
        while (isFormatCorrect && getline(rating, individualRating, ',')) {
            
            // Si el individualRating está vacío, había dos comas seguidas (ej: "10,,20")
            if (individualRating.empty()) {
                isFormatCorrect = false;
            } else {
                // 4. Analizamos los caracteres.
                for (size_t i = 0; i < individualRating.size() && isFormatCorrect; i++) {
                    char c = individualRating[i];

                    // a) Comprobamos si es el primer carácter y es un signo menos
                    if (i == 0 && c == '-') {
                        // Evitamos un guion suelto (ej: "10,-,20")
                        if (individualRating.size() == 1) {
                            isFormatCorrect = false;
                        }
                    } 
                    // b) Si no es un guion válido al inicio, comprobamos que sea un número
                    else if (c < '0' || c > '9') {
                        isFormatCorrect = false;
                    }
                }
            }
        }
    }

    return isFormatCorrect;
}

/*
Función que verifica que las valoraciones entran dentro del rango establecido.
ratings: cadena de valoraciones que introduce el usuario.
tempRatings: vector de valoraciones temporal para almacenar esas valoraciones.
return: un bool que dice si se cumple el rango o no.
*/
bool checkRange(string ratings, vector<int> &tempRatings){
    bool isRangeCorrect = true;
    size_t start = 0;
    size_t end = ratings.find(',');

    while(end != string::npos){
        int value = stoi(ratings.substr(start, end - start));
        if(value < -50 || value > 50){
            isRangeCorrect = false;
        }
        tempRatings.push_back(value);

        start = end + 1;
        end = ratings.find(',', start);
    }
    
    int lastVal = stoi(ratings.substr(start));
    if (lastVal < -50 || lastVal > 50) {
        isRangeCorrect = false;
    }
    tempRatings.push_back(lastVal);
    
    return isRangeCorrect;
}

/*
Función para añadir valoraciones a un jugador en función de su id
agency: registro de la agencia.
return: void
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
Función para mostrar un ranking de los jugadores en función de su valoración media.
agency: registro de la agencia.
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

/*
Función que transforma los datos de un formato de texto plano al formato de registros.
agency: registro de la agencia.
line: linea de texto plano del archivo de texto a importar.
return: void
*/
void parseData(Agency &agency, string line){
    stringstream data(line);
    Player tempPlayer;
    string playerData;
    bool isNameCorrect = true;
    bool isRatingCorrect = true;
    getline(data, playerData, ',');
    isNameCorrect = checkName(playerData, agency);
    if(isNameCorrect){
        tempPlayer.name = playerData;
        getline(data, playerData, ',');
        isNameCorrect = checkName(playerData, agency);
        if(isNameCorrect){
            tempPlayer.team = playerData;
            getline(data, playerData, ',');
            if(playerData < "0" || playerData > "99"){
                error(ERR_DORSAL);
            }
            else{
                tempPlayer.dorsal = stoi(playerData);
                getline(data, playerData, ',');
                if(playerData < "1" || playerData > "5"){
                    error(ERR_POSITION);
                }
                else{
                    tempPlayer.position = stoi(playerData);
                    if(getline(data, playerData)){
                        isRatingCorrect = checkFormat(playerData);
                        if(isRatingCorrect){
                            isRatingCorrect = checkRange(playerData, tempPlayer.ratings);
                            if(isRatingCorrect){
                                tempPlayer.id = agency.nextId;
                                agency.players.push_back(tempPlayer);
                                agency.nextId ++;
                            }
                        }
                    }
                    else{
                    tempPlayer.id = agency.nextId;
                    agency.players.push_back(tempPlayer);
                    agency.nextId ++;
                    }

                }
            }
        }
    }
}

/*
Función que realiza la importacion de datos tras parsearlos a los formatos necesarios de
registros.
agency; registro de la agencia.
return: void
*/
void importCsv(Agency &agency){
    string fileName;
    string line;

    cout << "Enter filename: ";
    getline(cin, fileName); 

    ifstream file(fileName);

    if(file.is_open()){
        while(getline(file, line)){
            parseData(agency, line);
        }
        file.close();
    }
    else{
        error(ERR_FILE);
    }
}

/*
Función que realiza la transformacion de datos a al formato requerido en un archivo csv
agency: registro de la agencia.
return: void
*/
void exportCsv(Agency agency){
    string fileName;

    cout << "Enter filename: ";
    getline(cin, fileName);
    if(fileName.empty()){
        error(ERR_FILE);
    }
    else{
        ofstream file(fileName);
        if(file.is_open()){
            for(unsigned int i = 0; i < agency.players.size(); i++){
                file << agency.players[i].name << ',' << agency.players[i].team << ',';
                file << agency.players[i].dorsal << ',' << agency.players[i].position;
                if(agency.players[i].ratings.size() == 0){
                    file << endl;
                }
                else{
                    file << ',';
                    for(unsigned j = 0; j < agency.players[i].ratings.size(); j++){
                        file << agency.players[i].ratings[j];
                        if(j == agency.players[i].ratings.size() - 1){
                            file << endl;
                        }
                        else{
                            file << ',';
                        }
                    }
                }
            }
        }
    }
}

/*
Función para transferir los datos de jugador del registro binario al registro normal.
player: registro del jugador en formato normal.
binPlayer: registro de jugador en formato estático para ficheros binarios.
return: void
*/
void transferPlayerData(Player &player, BinPlayer binPlayer){
    player.id = binPlayer.id;
    player.name = binPlayer.name;
    player.team = binPlayer.team;
    player.dorsal = binPlayer.dorsal;
    player.position = binPlayer.position;
    player.ratings.clear();
}

/*
Función para cargar datos desde un archivo binario.
agency: registro de la agencia.
return: void
*/
void loadData(Agency &agency){
    string option;
    string fileName;
    BinAgency tempAgency;
    BinPlayer binTempPlayer;
    Player tempPlayer;
    int rating;
    bool confirm = false;

    do{
        cout << "All data will be erased. Continue? [y/n]: ";
        getline(cin, option);
        if(option == "y" || option == "Y" || option == "n" || option == "N"){
            confirm = true;
        }
    }while(!confirm);
    if(option == "n" || option == "N"){
    }
    else{
        option.clear();
        cout << "Enter filename: ";
        getline(cin, fileName);
        ifstream file(fileName, ios::binary);
        if(file.is_open()){
            agency.players.clear();
            file.read((char*)&tempAgency, sizeof(tempAgency));
            agency.name = tempAgency.name;
            agency.nextId = tempAgency.nextId;
            while(file.read((char*)&binTempPlayer, sizeof(binTempPlayer))){
                transferPlayerData(tempPlayer, binTempPlayer);
                for(unsigned int i = 0; i < binTempPlayer.numRatings; i++){
                    file.read((char*)&rating, sizeof(int));
                    tempPlayer.ratings.push_back(rating);
                }
                agency.players.push_back(tempPlayer);
            }
            file.close();
        }
        else{
            error(ERR_FILE);
        }
    }
}

/*
Función para guardar los datos en un fichero binario.
agency: registro de la agencia.
return: void
*/
void saveData(Agency agency){
    string fileName;
    BinAgency tempAgency;
    BinPlayer tempPlayer;

    cout << "Enter filename: ";
    getline(cin, fileName);

    ofstream file(fileName, ios::binary);
    if(file.is_open()){
        strcpy(tempAgency.name, agency.name.c_str());
        tempAgency.nextId = agency.nextId;
        tempAgency.name[kMAXSTRING - 1] = '\0';
        file.write((char*)&tempAgency, sizeof(tempAgency));

        for(unsigned int i = 0; i < agency.players.size(); i++){
            tempPlayer.id = agency.players[i].id;
            tempPlayer.dorsal = agency.players[i].dorsal;
            tempPlayer.position = agency.players[i].position;
            tempPlayer.numRatings = agency.players[i].ratings.size();

            strcpy(tempPlayer.name, agency.players[i].name.c_str());
            tempPlayer.name[kMAXSTRING - 1] = '\0';
            strcpy(tempPlayer.team, agency.players[i].team.c_str());
            tempPlayer.team[kMAXSTRING - 1] = '\0';

            file.write(reinterpret_cast<const char*>(&tempPlayer), sizeof(BinPlayer));
            
            for (unsigned int j = 0; j < agency.players[i].ratings.size(); j++) {
                int rating = agency.players[i].ratings[j];
                file.write(reinterpret_cast<const char*>(&rating), sizeof(int));
            }
    }
        file.close();
    }
    else{
        error(ERR_FILE);
    }
}

/*
Función para importar o exportar datos en ficheros CSV
o binarios.
agency: registro de la agencia.
return: void
*/
void importExport(Agency &agency){
    char option;
    cin.clear();
    do{
        showImportExportMenu();
        cin >> option;
        cin.get();

        switch(option){
            case '1': // Import from CSV
            importCsv(agency);
                break;
            case '2': // Export from CSV
            exportCsv(agency);
                break;
            case '3': // Import from bin
                loadData(agency);
                break;
            case '4': // Export to bin
                saveData(agency);
                break;
            case 'b':
                break;
            default:
                error(ERR_OPTION);
        }
    }while(option != 'b');
}

/*
Función para importar datos de un archivo a partir un argumento.
agency: registro de la agencia.
argumentType: entero del argumento para decir si es un fichero binario (1) o fichero de texto. (0)
fileName: cadena donde se almacena el nombre / ubicación del fichero.
return: void
*/
void importFromArgument(Agency &agency, int argumentType, string fileName){
    if(argumentType == 0){
        ifstream file(fileName);
        string line;
        if(file.is_open()){
            while(getline(file, line)){
                parseData(agency, line);
            }
            file.close();
        }
        else{
            error(ERR_FILE);
        }
    }
    else if(argumentType == 1){
        BinAgency tempAgency;
        BinPlayer binTempPlayer;
        Player tempPlayer;
        int rating;
        ifstream file(fileName, ios::binary);
        if(file.is_open()){
            agency.players.clear();
            file.read((char*)&tempAgency, sizeof(tempAgency));
            agency.name = tempAgency.name;
            agency.nextId = tempAgency.nextId;
            while(file.read((char*)&binTempPlayer, sizeof(binTempPlayer))){
                transferPlayerData(tempPlayer, binTempPlayer);
                for(unsigned int i = 0; i < binTempPlayer.numRatings; i++){
                    file.read((char*)&rating, sizeof(int));
                    tempPlayer.ratings.push_back(rating);
                }
                agency.players.push_back(tempPlayer);
            }
            file.close();
        }
        else{
            error(ERR_FILE);
        }
    }
}

/*
Función que procesa la cantidad de argumentos introducidos.
argc y argv[]: parámetros de argumentos del programa.
importFile: cadena usada especialmente para ficheros de texto donde se almacena su nombre / ubicación.
loadFile: cadena usada especialmente para ficheros binarios donde se almacena su nombre / ubicación. 
return: un booleano que verifica que se han introducido correctamente los argumentos
*/
bool processArguments(int argc, char *argv[], string &importFile, string &loadFile) {
    bool isCorrect = true; // Control de validez de argumentos

    for (int i = 1; i < argc && isCorrect; i++) {
        string currentArg = string(argv[i]); // Argumento actual evaluado

        if (currentArg == "-i") {
            if (importFile != "" || i + 1 >= argc) {
                isCorrect = false; 
            } else {
                importFile = string(argv[i + 1]); 
                i++; // Saltamos la lectura del nombre del fichero
            }
        } else if (currentArg == "-l") {
            if (loadFile != "" || i + 1 >= argc) {
                isCorrect = false; 
            } else {
                loadFile = string(argv[i + 1]); 
                i++; // Saltamos la lectura del nombre del fichero
            }
        } else {
            isCorrect = false; // Argumento desconocido
        }
    }
    return isCorrect;
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
    string importFile = ""; // Archivo de texto a importar
    string loadFile = ""; // Archivo binario a cargar
    bool argsCorrect = true; // Control de la corrección de argumentos

    argsCorrect = processArguments(argc, argv, importFile, loadFile);
    if(!argsCorrect){
        error(ERR_ARGS);
    }
    else{
        if(loadFile != ""){
            importFromArgument(agency, 1, loadFile);
        }
        if(importFile != ""){
            importFromArgument(agency, 0, importFile);
        }
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
                    importExport(agency); //Falta por terminar
                    break;
                case 'q':
                    break;
                default: error(ERR_OPTION);
            }
        }while(option!='q');
    }
    return 0;
}