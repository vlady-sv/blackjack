#include <iostream>
#include <fstream>
#include <cstdlib>
#include <ctime>
#include <cstring>
#include <iomanip>

using namespace std;

enum elem{DIAMANTES=1, CORAZONES=2, PICAS=3, TREBOLES=4};

struct Nodo{
    Juego mano;
    Nodo *siguiente;
};

struct Lista{
    Nodo *cabeza;
    int numElemen;
};

struct Juego{
    int valor;
    char symbol;
    elem elemen;
};

void crearMazo(Lista* mazo, Juego& jMazo){
    const int cartas = 52;
    int elemento, contDiam, contCor, contPic, contTreb;
    contDiam=contCor=contPic=contTreb=0;
    bool band = false;
    //Creamos 52 nodos vacios para llenarlo con los valores de todas las cartas
    for(int i=0; i<cartas; i++){
        //Comentario para mi: Llenar todas las casillas de valor con NULL, para una verificacion mas adelante
        insertarFinal(mazo, jMazo);
    }

    Nodo* actual = mazo -> cabeza;
    while(band != true){
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
        if(contDiam == 13 && contCor == 13 && contPic == 13 && contTreb == 13){
            band = true;
        }
    }

    for(int i=0; i<4; i++){
        switch(i){
            case 0: elementos(mazo, jMazo, DIAMANTES);
                break;
            case 1: elementos(mazo, jMazo, CORAZONES);
                break;
            case 2: elementos(mazo, jMazo, PICAS);
                break;
            case 3: elementos(mazo, jMazo, TREBOLES);
                break;
        }
    }



}

void elementos(Lista* mazo, Juego &jMazo, elem elemento){
    const int nCart = 13;
    Nodo* actual = mazo -> cabeza;
    int vec[nCart] = {0};
    int auxCarta;
    bool band;

    for(int i=0; i< nCart; i++){
        band = false;
        auxCarta = 1 + rand()%(13+1-1);
        for(int j=0; j<nCart; j++){
            if(vec[i] == auxCarta){
                band == true;
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
            if(actual -> mano.elemen == elemento && actual -> mano.valor != NULL){
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
                        actual ->mano.symbol = '10';
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

void registroUser(char* nombre);
void verHistorial(fstream* record);
void start(fstream* record);
bool jugar(char* name, bool nom);
void darCartas(bool wPlay, int cont, int cart);
void crearMazo(Lista* mazo, Juego& jMazo);
Nodo* crearNodo(const Juego& jMano);
void elementos(Lista*, Juego &jMazo);
void insertarFinal(Lista* lista , const Juego& jMano);

int main(){
    fstream* record;
    int *ncarUser, *ncarCru, *ca, opcion;
    bool result;
    char resp;

    cout << "\t\t---> BIENVENIDO <---";
    cout << "\t\t\n\nBLACKJACK";

    do{
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

void start(fstream* record){
    char name[30];
    char resp, resNom;
    bool result, nom;
    //nom es para saber si se quiere registrar el nombre del usuario
    nom = false;

    do{
        crearMazo();
        result = jugar(name, nom);
        //si ya se registro lo cambia a true por si el jugador quiere volver a jugar con el mismo nombre
        nom = true;
        if(result == true){
            cout << "FELICIDADES " << name <<  ", HAS GANADO!!!";
        }else{
            cout << "LO SIENTO" << name << ", PERDISTE";
        }

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
    }while(resp == 's' || resp == 'S');

}


//Funcion para registrar un usuario
void registroUser(char* nombre){
    cout << "Dame tu nombre: ";
    cin.ignore();
    cin.getline(nombre, 30);

}

//Funcion para empezar el juego
bool jugar(char* name, bool nom){
    bool whoPlay = true;  //whoPlay es para saber si esta jugando el usuario=User o la banca=false
    int cUser, cBan, cart=2;
    cUser=cBan=1;

    //Registro de un usuario
    if(nom == false){
        registroUser(name);
    }

    for(int i=0; i<cart; i++){
        if(whoPlay == true){
            darCartas(whoPlay, cUser, cart);
            whoPlay=false;
        }else{
            darCartas(whoPlay,cBan, cart);
        }
    }
    cart=0;

    

    return;
}

//Dar cartas
void darCartas(bool wPlay, int cont, int cart){
    srand(time(0));
    int elemenMazo;

    if(wPlay == true){
        for(int i=0; i<cart; i++){
            elemenMazo = 52 + rand()%(52-1+1);
            //Guardar los resultados de cada vuelta en una lista
            
        }
    }else{
        for(int i=0; i<cart; i++){
            elemenMazo = 56 + rand()%(56-1+1);
            //Guardar los resultados de cada vuelta en una lista
        }
    }
}

void verHistorial(fstream record){
    string records = "records.txt";
    record.open(records, ios::in);
    string jugador, puntuacion, fecha;
    if(!record){
        cout << "El archivo no se abrio correctamente";
    }else{
        cout << "\n" << setw(12) << "Jugador" << setw(12) << "Puntuacion" << setw(15) << "Fecha";
        while(record << jugador << puntuacion << fecha){
            cout << setw(12) << jugador << setw(12) << puntuacion << setw(15) << fecha;
            cout << endl;
        }
    }
}

void crearMazo(){

}
