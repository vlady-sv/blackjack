#include <iostream>
#include <fstream>
#include <cstdlib>
#include <ctime>
#include <cstring>
#include <iomanip>

using namespace std;

enum elem{DIAMANTES=1, CORAZONES=2, PICAS=3, TREBOLES=4};

struct Juego{
    int valor;
    string symbol;
    elem elemen;
};

struct Nodo{
    Juego mano;
    Nodo *siguiente;
};

struct Lista{
    Nodo *cabeza;
    int numElemen;
};

void registroUser(char* nombre);
void verHistorial(fstream& record);
void start(fstream& record);
bool jugar(char* name, bool nom, Lista* user, Lista* banca, Lista* mazo);
void darCartas(bool wPlay, Lista* user, Lista* banca, Lista* mazo);
void crearMazo(Lista* mazo, Juego& jMazo);
Nodo* crearNodo(const Juego& jMano);
void elementos(Lista*, elem elemento);
void insertarFinal(Lista* lista , const Juego& jMano);
void actualizarHistorial(fstream& record, bool resultado, const char* nombre);
void liberarMemoria(Lista* lista);
int calcularPuntuacion(Lista* mano);

int main(){
    srand(time(0));
    fstream record;
    int opcion;

    do{
        system("cls");
        cout << setw(10) << "\n\n\t\t\t\t\t\t---> BIENVENIDO <---";
        cout << setw(15) << "\n\t\t\t\t\t\t    ~BLACKJACK~";

    
        cout << setw(10) << "\n\n\n\t\t\t- JUGAR [1]";
        cout << setw(10) << "\n\t\t\t- VER HISTORIAL DE REGISTROS [2]";
        cout << setw(10) << "\n\t\t\t- SALIR DEL JUEGO [0]";
        cout << setw(10) << "\n\n\t\t\tElige una opcion: ";
        cin >> opcion;
        switch(opcion){
            case 1: start(record);
            break;
            case 2: verHistorial(record);
            break;
            case 0: return 0;
            break;
            default: 
                cout << endl << "\n\n\t\t\t\t\t\tOPCION INVALIDA\n\t";
                cout << "\t\t\t" << system("pause");
                system("cls");
        }
    }while((opcion == 1 && opcion == 2 && opcion == 0 && opcion) || (opcion != 1 || opcion != 2 || opcion != 0));
    

    
    return 0;
}

//Crear un nodo nuevo de mano para nuestra lista
Nodo* crearNodo(const Juego& jMano){
    Nodo* nodo = new Nodo;
    nodo -> mano = jMano;
    nodo -> siguiente = nullptr;
    return nodo;
}

//Insertamos un nuevo nodo al final de la lista
void insertarFinal(Lista* lista , const Juego& jMano){
    Nodo* nuevo = crearNodo(jMano);
    if(lista -> cabeza == nullptr){
        lista->cabeza = nuevo;
    }else{
        Nodo* actual = lista ->cabeza;
        while(actual->siguiente != nullptr){
            actual = actual -> siguiente;
        }
        actual -> siguiente = nuevo;
    }
    lista -> numElemen++;
}

void start(fstream& record){
    system("cls");
    char resp, resNom;
    bool result, nom;
    //nom es para saber si se quiere registrar el nombre del usuario
    nom = false;

    do{
        Lista* user = new Lista;
        user -> cabeza = nullptr;
        user -> numElemen = 0;
        Lista* banca = new Lista;
        banca -> cabeza = nullptr;
        banca -> numElemen = 0;
        Lista* mazo = new Lista;
        mazo -> cabeza = nullptr;
        mazo -> numElemen = 0;
        Juego mUser;
        Juego mBanca;
        Juego jMazo;
        char name[30];  
        crearMazo(mazo, jMazo);
        result = jugar(name, nom, user, banca, mazo);
        //si ya se registro lo cambia a true por si el jugador quiere volver a jugar con el mismo nombre
        nom = true;
        if(result == true){
            cout << "\n\n\t\t\t\t\tFELICIDADES " << name <<  ", HAS GANADO!!!";
        }else{
            cout << "\n\n\n\t\t\t\t\tLO SIENTO " << name << ", PERDISTE";
        }
        actualizarHistorial(record, result, name);

        do{
            cout << "\n\n\n";
            cout << "\t\t\t\t\tQuieres volver a jugar? s/n: ";
            cin.ignore();
            cin >> resp;
        }while(resp != 's' && resp != 'S' && resp != 'n' && resp != 'N');
        if(resp == 's' || resp == 'S'){
            do{
                cout << "\n\n\t\t\t\t\tQuieres jugar con el mismo nombre? s/n: ";
                cin >> resNom;
            }while(resNom != 's' && resNom != 'S' && resNom != 'n' && resNom != 'N');
            if(resNom == 'n' || resNom == 'N'){
                //si quiere cambiar de nombre lo cambiamos nuevamente a false
                nom = false;
            }
        }
        liberarMemoria(mazo);
        liberarMemoria(user);
        liberarMemoria(banca);
    }while(resp == 's' || resp == 'S');

}


//Funcion para registrar un usuario
void registroUser(char* nombre){
    cout << setw(10) << "\n\n\t\t\t\t\t\tINICIA EL JUEGO";
    cout << "\n\n\t\t\tDame tu nombre: ";
    cin.ignore();
    cin.getline(nombre, 30);

}

//Funcion para crear el mazo de cartas
void crearMazo(Lista* mazo, Juego& jMazo){
    const int cartas = 52;
    int elemento, contDiam, contCor, contPic, contTreb;
    contDiam=contCor=contPic=contTreb=0;

    //Creamos 52 nodos vacios para llenarlo con los valores de todas las cartas
    for(int i=0; i<cartas; i++){
        insertarFinal(mazo, jMazo);
    }
    //Llenamos todas las casillas de valor con 0 para una verficacion mas adelante
    Nodo* aux = mazo -> cabeza;
    while(aux != nullptr){
        aux -> mano.valor = 0;
        aux = aux -> siguiente;
    }

    Nodo* actual = mazo -> cabeza;
    //Variable para asegurarnos de que todas las cartas sean llenadas con un elemento
    bool band = true; //true para saber que aun no se asigno y false para saber que ya se asigno
    while(actual != nullptr){
        do{
            elemento = 1 + rand()%(4+1-1);
            switch(elemento){
                case 1: 
                    //Asignamos como elemento diamantes
                    if(contDiam < 13){
                        contDiam++;
                        actual->mano.elemen = DIAMANTES;
                        band = false;
                    }
                    break;    
                //Corazones
                case 2:
                    if(contCor < 13){
                        contCor++;
                        actual->mano.elemen = CORAZONES;
                        band = false;
                    }
                    break;
                //Picas
                case 3:
                    if(contPic < 13){
                        contPic++;
                        actual->mano.elemen = PICAS;
                        band = false;
                    }
                    break; 
                //Y treboles
                case 4:
                    if(contTreb < 13){
                        contTreb++;
                        actual->mano.elemen = TREBOLES;
                        band = false;
                    }
                    break; 
            }
        }while(band == true);
        actual = actual -> siguiente;
        band = true;
        if(contDiam + contCor + contPic + contTreb == 52){
            break;
        }
    }

    for(int i=0; i<4; i++){
        switch(i){
            case 0: elementos(mazo, DIAMANTES);
                break;
            case 1: elementos(mazo, CORAZONES);
                break;
            case 2: elementos(mazo, PICAS);
                break;
            case 3: elementos(mazo, TREBOLES);
                break;
        }
    }



}

//Asignando los elementos en el mazo
void elementos(Lista* mazo, elem elemento){
   const int nCart = 13;
    bool usado[nCart + 1] = {false}; // Para rastrear qué valores ya se usaron para este elemento
    int auxCarta, cartasAsignadas = 0;
    Nodo* actual = mazo->cabeza;

    while (actual != nullptr && cartasAsignadas < nCart) {
        if (actual->mano.elemen == elemento && actual->mano.valor == 0) {
            do {
                auxCarta = 1 + rand() % nCart;
                //Si ya asignamos el valor, volvemos a realizar el proceso
            } while (usado[auxCarta]); 

            //Marcamos que ya usamos ese valor en el elemento.
            usado[auxCarta] = true;
            cartasAsignadas++;

            switch(auxCarta){
                case 1: actual->mano.symbol = "A";
                    actual->mano.valor = 11; 
                    break;
                case 2: actual->mano.symbol = "2"; 
                    actual->mano.valor = 2; 
                    break;
                case 3: actual->mano.symbol = "3"; 
                    actual->mano.valor = 3; 
                    break;
                case 4: actual->mano.symbol = "4"; 
                    actual->mano.valor = 4; 
                    break;
                case 5: actual->mano.symbol = "5"; 
                    actual->mano.valor = 5; 
                    break;
                case 6: actual->mano.symbol = "6"; 
                    actual->mano.valor = 6; 
                    break;
                case 7: actual->mano.symbol = "7"; 
                    actual->mano.valor = 7; 
                    break;
                case 8: actual->mano.symbol = "8"; 
                    actual->mano.valor = 8; 
                    break;
                case 9: actual->mano.symbol = "9"; 
                    actual->mano.valor = 9; 
                    break;
                case 10: actual->mano.symbol = "10"; 
                    actual->mano.valor = 10; 
                    break;
                case 11: actual->mano.symbol = "J"; 
                    actual->mano.valor = 10; 
                    break;
                case 12: actual->mano.symbol = "Q"; 
                    actual->mano.valor = 10; 
                    break;
                case 13: actual->mano.symbol = "K"; 
                    actual->mano.valor = 10; 
                    break;
            }
        }
        actual = actual->siguiente;
    }
}

//Funcion para empezar el juego
bool jugar(char* name, bool nom, Lista* user, Lista* banca, Lista* mazo){
    bool whoPlay = true;  //whoPlay es para saber si esta jugando el usuario=User o la banca=false
    int cart=2, puntBanca, puntUser, selec;

    //Registro de un usuario
    if(nom == false){
        registroUser(name);
    }

    //Para dar cartas al usuario
    for(int i=0; i<cart; i++){
        darCartas(whoPlay, user, banca, mazo);
    }
    whoPlay=false;

    //Para dar cartas a la banca
    for(int i=0; i<cart; i++){
        darCartas(whoPlay, user, banca, mazo);
        
    }
    system("cls");
    //BANCA PRIMERA PUNTUACION
    cout << setw(15) << "\n\n\t\t\t\t\t\tTURNO DE LA BANCA" << endl;
    cout << "\n\n\t\t\t\t Cartas:\n";
    Nodo* actualBanca = banca->cabeza;
    string aux;
    while(actualBanca != nullptr){
        switch(actualBanca -> mano.elemen){
            case DIAMANTES: aux = "Diamantes";
                break;
            case CORAZONES: aux = "Corazones";
                break;
            case PICAS: aux = "Picas";
                break;
            case TREBOLES:  aux = "Treboles";
                break;  
        }
    
    cout << setw(10) << "\t\t\t\t" <<actualBanca -> mano.symbol << " de " << aux;
    cout << endl;
    actualBanca = actualBanca -> siguiente;
    }
    puntBanca = calcularPuntuacion(banca);
    cout << setw(10) << "\n\t\t\t\t~~ Puntuacion de la banca: " << puntBanca << " ~~" << endl;

    //USUARIO PRIMERA PUNTUACION
    cout << setw(15) << "\n\n\n\t\t\t\t\t TURNO DEL USUARIO: " << name << endl;
    cout << "\n\n\t\t\t\t Cartas:\n";
    Nodo* actualUser = user->cabeza;
    while(actualUser != nullptr){
        switch(actualUser -> mano.elemen){
            case DIAMANTES: aux = "Diamantes";
                break;
            case CORAZONES: aux = "Corazones";
                break;
            case PICAS: aux = "Picas";
                break;
            case TREBOLES:  aux = "Treboles";
                break;  
        }
    
    cout << setw(10) << "\t\t\t\t" << actualUser -> mano.symbol << " de " << aux;
    cout << endl;
    actualUser = actualUser -> siguiente;
    }
    puntUser = calcularPuntuacion(user);
    
    cout << setw(10) << "\n\t\t\t\t~~ Puntuacion del usuario: " << puntUser << " ~~" << endl << endl;
    cout << setw(10) << "---------------------------------------------------------------------------------------------------------------------------------" << endl;
    //Turno del usuario
    while(true){
        if(puntUser == 21 || puntUser > 21){
            break; 
        }

        do{
            cout << "\n\n";
            cout << setw(15) << "\n\n\t\t\t\t\t TURNO DEL USUARIO: " << name << endl << endl;
            cout << "\t\t\t\t[1] Pedir carta" << endl;
            cout << "\t\t\t\t[2] Plantarse" << endl << endl;
            cout << "\t\t\t\tElige una opcion: ";
            cin >> selec;
        }while(selec != 1 && selec != 2);

        //Opcion de plantarse
        if(selec == 2){
            system("cls");
            break;
            //Opcion de pedir otra carta
        }else{
            whoPlay = true;
            darCartas(whoPlay, user, banca, mazo);
            system("cls");
        }

        cout << setw(15) << "\n\n\t\t\t\t\t TURNO DEL USUARIO: " << name << endl;
        cout << "\n\n\t\t\t\t Cartas:\n";
        actualUser = user->cabeza;
        while(actualUser != nullptr){
            switch(actualUser -> mano.elemen){
                case DIAMANTES: aux = "Diamantes";
                    break;
                case CORAZONES: aux = "Corazones";
                    break;
                case PICAS: aux = "Picas";
                    break;
                case TREBOLES:  aux = "Treboles";
                    break;  
            }
    
            cout << setw(10) << "\t\t\t\t" << actualUser -> mano.symbol << " de " << aux;
            cout << endl;
            actualUser = actualUser -> siguiente;
        }
        puntUser = calcularPuntuacion(user);
    
    cout << setw(10) << "\n\t\t\t\t~~ Puntuacion del usuario: " << puntUser << " ~~" << endl << endl;
    }

    //Si el usuario paso de 21, la banca gana en automatico
    if(puntUser > 21){
            system("pause");
            return false;
        }

    //Turno de la banca
    while(true){
        cout << setw(15) << "\n\n\t\t\t\t\t\tTURNO DE LA BANCA" << endl;
        cout << "\n\n\t\t\t\t Cartas:\n";
        actualBanca = banca->cabeza;
        while(actualBanca != nullptr){
            switch(actualBanca -> mano.elemen){
                case DIAMANTES: aux = "Diamantes";
                    break;
                case CORAZONES: aux = "Corazones";
                    break;
                case PICAS: aux = "Picas";
                    break;
                case TREBOLES:  aux = "Treboles";
                    break;  
            }   
            cout << setw(10) << "\t\t\t\t" << actualBanca -> mano.symbol << " de " << aux;
            cout << endl;
            actualBanca = actualBanca -> siguiente;
        }
        puntBanca = calcularPuntuacion(banca);
        cout << setw(10) << "\n\t\t\t\t~~ Puntuacion de la banca: " << puntBanca << " ~~"  << endl;
        system("pause");

        if(puntBanca < 17){
            cout << setw(10) << "\n\t\t\t\tLa banca pide una carta" << endl;
            cart = 1;
            whoPlay = false;
            darCartas(whoPlay, user, banca, mazo);
        }

        if(puntBanca >= 17 && puntBanca <= 21){
            cout << setw(10) << "\n\t\t\t\tLa banca se planta" << endl;
            break;
        }
        
        if(puntBanca > 21){
            return true;
        }
    }

    system("cls");
    cout << setw(10) << "\n\t\t\t\t\t~~ PUNTUACIONES: ~~" << endl;
    //Puntuacion de la banca
    cout << setw(15) << "\n\n\t\t\tCARTAS DE LA BANCA" << endl;
        actualBanca = banca->cabeza;
        while(actualBanca != nullptr){
            string aux;
            switch(actualBanca -> mano.elemen){
                case DIAMANTES: aux = "Diamantes";
                    break;
                case CORAZONES: aux = "Corazones";
                    break;
                case PICAS: aux = "Picas";
                    break;
                case TREBOLES:  aux = "Treboles";
                    break;  
            }   
            cout << setw(10) << "\t\t\t\t\t" << actualBanca -> mano.symbol << " de " << aux;
            cout << endl;
            actualBanca = actualBanca -> siguiente;
        }
        puntBanca = calcularPuntuacion(banca);
        cout << setw(10) << "\n\t\t\t\t\t~~ Puntuacion de la banca: " << puntBanca << " ~~" << endl;

    //Puntuacion del usuario
    cout << setw(15) << "\n\n\t\t\t\t CARTAS DEL USUARIO: " << name << endl;
        actualUser = user->cabeza;
        while(actualUser != nullptr){
            string aux;
            switch(actualUser -> mano.elemen){
                case DIAMANTES: aux = "Diamantes";
                    break;
                case CORAZONES: aux = "Corazones";
                    break;
                case PICAS: aux = "Picas";
                    break;
                case TREBOLES:  aux = "Treboles";
                    break;  
            }
    
            cout << setw(10) << "\t\t\t\t\t" << actualUser -> mano.symbol << " de " << aux;
            cout << endl;
            actualUser = actualUser -> siguiente;
        }
        puntUser = calcularPuntuacion(user);
    
    cout << setw(10) << "\n\t\t\t\t~~ Puntuacion de la usuario: " << puntUser << " ~~" <<  endl << endl;

    delete actualBanca;
    delete actualUser;

    //Comparacion de resultados si ninguno se paso de 21
    if(puntBanca == puntUser){
        return false;
    }

    if(puntBanca > puntUser && puntBanca <= 21){
        return false;
    }

    if(puntUser > puntBanca && puntUser <= 21){
        return true;
    }

    return true;
}

//Dar cartas
void darCartas(bool wPlay, Lista* user, Lista* banca, Lista* mazo){
    int elemenMazo = rand()%mazo->numElemen;
    Nodo* aux = nullptr;
    Nodo* actual = mazo->cabeza;

    for(int i=0; i<elemenMazo; i++){
        aux = actual;
        actual = actual->siguiente;
    }

    if(aux == nullptr){
        mazo->cabeza = actual-> siguiente;
    }else{
        aux->siguiente = actual -> siguiente;
    }
    mazo->numElemen--;
    actual -> siguiente = nullptr;
    //Para el usuario
    if (wPlay == true) {
        insertarFinal(user, actual->mano);
        //Para la banca
    } else {
        insertarFinal(banca, actual->mano); 
    }
}

//Calcular la puntuacion (suma de las cartas) 
int calcularPuntuacion(Lista* mano) {
    int puntuacion = 0, ases = 0;
    Nodo* actual = mano->cabeza;
    while (actual != nullptr) {
        puntuacion = puntuacion + actual->mano.valor;
        if (actual->mano.symbol == "A") { // Consideramos 'A' como string para "10"
            ases++;
        }
        actual = actual->siguiente;
    }

    // Ajustar Ases si la puntuación es > 21
    while (puntuacion > 21 && ases > 0) {
        puntuacion = puntuacion - 10; // Cambiar As de 11 a 1
        ases--;
    }
    return puntuacion;
}

//Ver historial de juegos
void verHistorial(fstream& record){
    system("cls");
    cout << "\n\t\t\t\t\t\tHISTORIAL DE JUEGO" << endl;
    string records = "records.txt";
    record.open(records, ios::in);
    string jugador, resultado, fecha;
    if(!record){
        cout << "El historial esta vacio";
    }else{
        cout << "\n" << setw(15) << "\t\t\t\t\tJugador" << setw(12) << "Resultado" << setw(15) << "Fecha" << endl;
        while(record >> jugador >> resultado >> fecha){
            cout << "\t\t\t\t" <<setw(15) << jugador << setw(12) << resultado << setw(15) << fecha;
            cout << endl;
        }
    }
    record.close();
    cout << "\n\t\t\t\t" << system("pause");
}

void actualizarHistorial(fstream& record, bool resultado, const char* nombre){
    time_t ahora = time(0);
    char fecha[30];
    strftime(fecha, sizeof(fecha), "%d-%m-%Y", localtime(&ahora));
    string result;
    record.open("records.txt", ios::app|ios::out);
    if(!record){
        cout << "El archivo no se abrio correctamente";
    }else{
        if(resultado == true){
            result = "GANADOR";
        }else{
            result = "PERDEDOR";
        }
        record << nombre << " " << result << " " << fecha << endl;
        record.close();
    }
}

void liberarMemoria(Lista* lista){
    Nodo* actual = lista -> cabeza;
    while(actual != nullptr){
        Nodo* temp = actual;
        actual = actual -> siguiente;
        delete temp;
    }
    lista -> cabeza = nullptr;
    lista -> numElemen = 0;
    delete lista;
}