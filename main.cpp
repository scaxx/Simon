#include <iostream> 
#include <unistd.h>
#define MAX_JUGADORES 10
using namespace std;


/* JUEGO SIMÓN */

// ****************** Estructuras *********************
struct Fecha{
    int dia;
    string mes;
    int anio;
};
struct Jugador{
    string alias;
    string nombre;
    string apellido;
    Fecha fecha_nacimiento;
    bool estado;
};    

Jugador jugadores[MAX_JUGADORES];


// ****************************************************


// ****************** Prototipos de Funciones y Procedimientos ************

Jugador crear_jugador(Jugador &j){
    system("clear");
    cin.ignore();

    cout << "Ingrese su alias: ";
    getline(cin, j.alias); 
    system("clear");

    cout << "Ingrese nombres: ";
    getline(cin, j.nombre); 
    system("clear");

    cout << "Ingrese apellido: ";
    getline (cin, j.apellido);
    system("clear");
      
    cout << "Fecha de nacimiento:" << endl << " Día >>";
    cin >> j.fecha_nacimiento.dia; 
    system("clear");

    cout << "Fecha de nacimiento:" << endl << " Mes >>";
    getline(cin, j.fecha_nacimiento.mes); 
    system("clear");
    
    cout << "Fecha de nacimiento:" << endl << " Año >>";
    cin >> j.fecha_nacimiento.anio; 
    system("clear");
    return j;
}


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
    Jugador jugador;

    crear_jugador(jugador);

    cout << "Datos: " << endl;
    cout << "Alias: " << jugador.alias << endl;
    cout << "Nombre: " << jugador.nombre << endl;
    cout << "Apellido: " << jugador.apellido << endl;
    cout << "Fecha: " << jugador.fecha_nacimiento.dia << " de " << jugador.fecha_nacimiento.mes << " del año " << jugador.fecha_nacimiento.anio << endl;
}


