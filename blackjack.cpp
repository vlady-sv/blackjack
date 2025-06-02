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
    char symbol;
    elem elemen;
};

void registroUser(char* nombre);
void verHistorial(fstream* record);
void start(fstream* record);
bool jugar(char* name, bool nom);
void darCartas(bool wPlay, int cont, int cart);

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

void start(fstream* record){
    char name[30];
    char resp, resNom;
    bool result, nom;
    //nom es para saber si se quiere registrar el nombre del usuario
    nom = false;

    do{
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
    int elemen, carta;

    if(wPlay == true){
        for(int i=0; i<cart; i++){
            elemen = 4 + rand()%(4-1+1);
            carta = 1 + rand()%(13-1+1);
            //Guardar los resultados de cada vuelta en una lista
            
        }
    }else{
        for(int i=0; i<cart; i++){
            elemen = 4 + rand()%(4-1+1);
            carta = 1 + rand()%(13-1+1);
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
