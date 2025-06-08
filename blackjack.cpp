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
bool jugar(char* name, bool nom, Lista* user, Juego& manoUser, Lista* banca, Juego& manoBanca, Lista* mazo);
void darCartas(bool wPlay, int cart, Lista* user, Juego& manoUser, Lista* banca, Juego& manoBanca, Lista* mazo);
void crearMazo(Lista* mazo, Juego& jMazo);
Nodo* crearNodo(const Juego& jMano);
void elementos(Lista*, elem elemento);
void insertarFinal(Lista* lista , const Juego& jMano);
void actualizarHistorial(fstream& record, bool resultado, const char* nombre);
void liberarMemoria(Lista* lista);

int main(){
    srand(time(0));
    fstream record;
    int opcion;

    do{
        cout << "\t\t---> BIENVENIDO <---";
        cout << "\t\t\n\nBLACKJACK";

    
        cout << "\t\n\n\nJUGAR [1]";
        cout << "\t\n\n\nVER HISTORIAL DE REGISTROS [2]";
        cout << "\t\n\n\nSALIR DEL JUEGO [0]";
        cout << "\t\n\n\nElige una opcion: ";
        cin >> opcion;
        switch(opcion){
            case 1: start(record);
            break;
            case 2: verHistorial(record);
            break;
            case 0: return 0;
            break;
        }
    }while(opcion != 1 && opcion != 2 && opcion != 0);
    

    
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
        result = jugar(name, nom, user, mUser, banca, mBanca, mazo);
        //si ya se registro lo cambia a true por si el jugador quiere volver a jugar con el mismo nombre
        nom = true;
        if(result == true){
            cout << "FELICIDADES " << name <<  ", HAS GANADO!!!";
        }else{
            cout << "LO SIENTO" << name << ", PERDISTE";
        }
        actualizarHistorial(record, result, name);

        do{
            cout << "Quieres volver a jugar? s/n:";
            cin.ignore();
            cin >> resp;
        }while(resp != 's' && resp != 'S' && resp != 'n' && resp != 'N');
        if(resp == 's' || resp == 'S'){
            do{
                cout << "Quieres jugar con el mismo nombre? s/n:";
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
    cout << "Dame tu nombre: ";
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
    while(actual != nullptr){
        elemento = 1 + rand()%(4+1-1);
        switch(elemento){
            case 1: 
                //Asignamos como elemento diamantes
                if(contDiam < 13){
                    contDiam++;
                    actual->mano.elemen = DIAMANTES;
                }
                break;    
                //Corazones
            case 2:
                if(contCor < 13){
                    contCor++;
                    actual->mano.elemen = CORAZONES;
                }
                break;
                //Picas
            case 3:
                if(contPic < 13){
                    contPic++;
                    actual->mano.elemen = PICAS;
                }
                break; 
                //Y treboles
            case 4:
                if(contTreb < 13){
                    contTreb++;
                    actual->mano.elemen = TREBOLES;
                }
                break; 
        }
        actual = actual -> siguiente;
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
    Nodo* actual = mazo -> cabeza;
    int vec[nCart] = {0};
    int auxCarta;
    bool band;

    for(int i=0; i< nCart; i++){
        band = false;
        auxCarta = 1 + rand()%(13+1-1);
        //Verificamos si el valor ya existe en nuestro vector
        for(int j=0; j<nCart; j++){
            if(vec[j] == auxCarta){
                band = true;
                break;
            }
        }

        if(band == true)
            break;
        
        for(int j=0; j<nCart; j++){
            if(vec[i] == 0)
                vec[i] = auxCarta;
        }

        while(actual != nullptr){
            if(actual -> mano.elemen == elemento && actual -> mano.valor != 0){
                switch(vec[i]){
                    case 1:
                        actual ->mano.symbol = 'A';
                        actual ->mano.valor = 11;
                        break;
                    case 2:
                        actual ->mano.symbol = '2';
                        actual ->mano.valor = 2;
                        break;
                    case 3:
                        actual ->mano.symbol = '3';
                        actual ->mano.valor = 3;
                        break;
                    case 4:
                        actual ->mano.symbol = '4';
                        actual ->mano.valor = 4;
                        break;
                    case 5:
                        actual ->mano.symbol = '5';
                        actual ->mano.valor = 5;
                        break;
                    case 6:
                        actual ->mano.symbol = '6';
                        actual ->mano.valor = 6;
                        break;
                    case 7:
                        actual ->mano.symbol = '7';
                        actual ->mano.valor = 7;
                        break;
                    case 8:
                        actual ->mano.symbol = '8';
                        actual ->mano.valor = 8;
                        break;
                    case 9:
                        actual ->mano.symbol = '9';
                        actual ->mano.valor = 9;
                        break;
                    case 10:
                        actual ->mano.symbol = "10";
                        actual ->mano.valor = 10;
                        break;
                    case 11:
                        actual ->mano.symbol = 'J';
                        actual ->mano.valor = 10;
                        break;
                    case 12:
                        actual ->mano.symbol = 'Q';
                        actual ->mano.valor = 10;
                        break;
                    case 13:
                        actual ->mano.symbol = 'K';
                        actual ->mano.valor = 10;
                        break;
                }
            }
            actual = actual->siguiente;
        }
    }

}

//Funcion para empezar el juego
bool jugar(char* name, bool nom, Lista* user, Juego& manoUser, Lista* banca, Juego& manoBanca, Lista* mazo){
    string aux;
    bool whoPlay = true;  //whoPlay es para saber si esta jugando el usuario=User o la banca=false
    int cart=2, puntBanca, puntUser, selec;
    puntBanca=puntUser=0;

    //Registro de un usuario
    if(nom == false){
        registroUser(name);
    }

    for(int i=0; i<cart; i++){
        if(whoPlay == true){
            darCartas(whoPlay, cart, user, manoUser, banca, manoBanca, mazo);
            whoPlay=false;
        }else{
            darCartas(whoPlay, cart, user, manoUser, banca, manoBanca, mazo);
        }
    }
    cart=0;

    cout << setw(15) << "BANCA" << endl;
    for(int i=0; i<banca->numElemen; i++){
        switch(manoBanca.elemen){
            case DIAMANTES: aux = "Diamantes";
                break;
            case CORAZONES: aux = "Corazones";
                break;
            case PICAS: aux = "Picas";
                break;
            case TREBOLES:  aux = "Treboles";
                break;  
        }
    
    cout << setw(10) << manoBanca.symbol << " de " << aux;
    cout << endl;
    puntBanca = puntBanca + manoBanca.valor;
    }
    
    cout << setw(10) << "Puntuacion de la banca: " << puntBanca << endl << endl;

    cout << setw(15) << "Usuario: " << name;

    for(int i=0; i<user->numElemen; i++){
        switch(manoUser.elemen){
            case DIAMANTES: aux = "Diamantes";
                break;
            case CORAZONES: aux = "Corazones";
                break;
            case PICAS: aux = "Picas";
                break;
            case TREBOLES:  aux = "Treboles";
                break;  
        }
    
    cout << setw(10) << manoUser.symbol << " de " << aux;
    cout << endl;
    puntUser = puntUser + manoUser.valor;
    }
    
    cout << setw(10) << "Puntuacion del usuario: " << puntUser;

    //Turno del usuario
    while(true){
        puntUser = 0;
        cout << "[1] Pedir carta" << endl;
        cout << "[2] Plantarse" << endl;
        cout << "Elige una opcion: ";
        cin >> selec;

        if(selec == 2){
            break;
        }else{
            cart = 1;
            whoPlay = true;
            darCartas(whoPlay, cart, user, manoUser, banca, manoBanca, mazo);
        }
        Nodo* temp = user->cabeza;  // Corrección: Se recorre la lista enlazada
        while (temp != nullptr) {
            manoUser = temp->mano; 
            temp = temp->siguiente;
        }
    
        cout << setw(10) << manoUser.symbol << " de " << aux;
        cout << endl;
        puntUser = puntUser + manoUser.valor;

        if(puntUser > 21){
            temp = user->cabeza;
            while(temp != nullptr){
                if(temp->mano.symbol == "A" && temp->mano.valor == 11){
                    temp->mano.valor = 1; 
                }
                temp = temp->siguiente;
            }
        }
        
        cout << setw(10) << "Nueva puntuacion del usuario: " << puntUser;
        if(puntUser == 21 || puntUser > 21){
            break;
        }
    }

    //Turno de la banca
    while(true){
        if(puntUser > 21){
            return false;
        }
        if(puntBanca < 17){
            cart = 1;
            whoPlay = false;
            darCartas(whoPlay, cart, user, manoUser, banca, manoBanca, mazo);
        }

        Nodo* temp = banca->cabeza;
        while (temp != nullptr) {
            manoBanca = temp->mano; 
            temp = temp->siguiente;
        }
    
        cout << setw(10) << manoBanca.symbol << " de " << aux;
        cout << endl;
        puntBanca = puntBanca + manoBanca.valor;
        if(puntBanca > 21){
            temp = banca->cabeza;
            while(temp != nullptr){
                if(temp->mano.symbol == "A" && temp->mano.valor == 11){
                    temp->mano.valor = 1; 
                }
                temp = temp->siguiente;
            }
        }
        puntBanca = 0;
        for(int i=0; i<banca->numElemen; i++){
            switch(manoBanca.elemen){
                case DIAMANTES: aux = "Diamantes";
                    break;
                case CORAZONES: aux = "Corazones";
                    break;
                case PICAS: aux = "Picas";
                    break;
                case TREBOLES:  aux = "Treboles";
                    break;  
            }
    
            cout << setw(10) << manoBanca.symbol << " de " << aux;
            cout << endl;
            puntBanca = puntBanca + manoBanca.valor;
        }
        cout << setw(10) << "Nueva puntuacion de la banca: " << puntBanca;

        if(puntBanca >= 17 && puntBanca <= 21){
            break;
        }
        if(puntBanca > 21){
            return true;
        }
    }

    //Comparacion de resultados si ninguno se paso de 21
    if(puntBanca == puntUser){
        return false;
    }

    if(puntBanca > puntUser && puntBanca < 21){
        return false;
    }

    if(puntUser > puntBanca && puntUser < 21){
        return true;
    }

    return true;
}

//Dar cartas
void darCartas(bool wPlay, int cart, Lista* user, Juego& manoUser, Lista* banca, Juego& manoBanca, Lista* mazo){
    int elemenMazo;

    if(wPlay == true){
         //Guardamos los resultados de cada vuelta en la lista correspondiente
        for(int i=0; i<cart; i++){
            Nodo* actual = mazo->cabeza;
            elemenMazo = 1 + rand()%(52-1+1);
            for(int j=0; j<elemenMazo; j++){
                if(elemenMazo == j && actual->mano.valor != 0){
                    if(user->numElemen == 0){
                        user->cabeza = actual;
                    }
                    manoUser = actual -> mano;
                    actual -> mano.valor = 0;

                    user->numElemen++;
                    break;
                }
            actual = actual->siguiente;
            }          
        }
    }else{
        for(int i=0; i<cart; i++){
            Nodo* actual = mazo->cabeza;
            elemenMazo = 1 + rand()%(52-1+1);
            for(int j=0; j<elemenMazo; j++){
                if(elemenMazo == j && actual->mano.valor != 0){
                    if(banca->numElemen == 0){
                        banca->cabeza = actual;
                    }
                    manoBanca = actual -> mano;
                    actual -> mano.valor = 0;

                    banca->numElemen++;
                    break;
                }
            actual = actual->siguiente;
            }
        }
    }
}

//Ver historial de juegos
void verHistorial(fstream& record){
    string records = "records.txt";
    record.open(records, ios::in);
    string jugador, resultado, fecha;
    if(!record){
        cout << "El historial esta vacio";
    }else{
        cout << "\n" << setw(12) << "Jugador" << setw(12) << "Resultado" << setw(15) << "Fecha";
        while(record >> jugador >> resultado >> fecha){
            cout << setw(12) << jugador << setw(12) << resultado << setw(15) << fecha;
            cout << endl;
        }
    }
    record.close();
}

void actualizarHistorial(fstream& record, bool resultado, const char* nombre){
    time_t ahora = time(0);
    char fecha[30];
    strftime(fecha, sizeof(fecha), "%Y-%m-%d %H:%M:%S", localtime(&ahora));
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
    lista = nullptr;
}
