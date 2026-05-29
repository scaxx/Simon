#include <iostream> 
#include <unistd.h>
#define MAX_JUGADORES 10
using namespace std;


/* JUEGO SIMÓN */

// ****************** Estructuras *********************
struct Fecha{
    int dia;
    string mes;
    int año;
};
struct Jugador{
    string alias;
    string nombre;
    string apellido;
    struct Fecha;
    bool estado;
};    

Jugador jugadores[MAX_JUGADORES];


// ****************************************************


// ****************** Prototipos de Funciones y Procedimientos ************



void menu() {
    cout << "Bienvenidos al Juego SIMON." << endl;
    cout << "Ingrese una de las siguientes opciones: " << endl;
    cout << "   1. Gestionar jugadores" << endl;
    cout << "   2. Jugar" << endl;
    cout << "   3. Informes" << endl;
    cout << "   4. Salir" << endl;
    cout << ">> ";
}



// ************************************************************************

int main () {
    return 0;


}


