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
struct Juego {
    Jugador jugadores[MAX_JUGADORES];
    int cantJugadores=0;
};



// ****************************************************


// ****************** Prototipos de Funciones y Procedimientos ************

Jugador crear_jugador(){
    Jugador j;
    system("clear");
    //cin.ignore();  por si lo necesitamos , nos dio error congela el programa

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
    cin.ignore();
    system("clear");

    cout << "Fecha de nacimiento:" << endl << " Mes >>";
    getline(cin, j.fecha_nacimiento.mes); 
    system("clear");
    
    cout << "Fecha de nacimiento:" << endl << " Año >>";
    cin >> j.fecha_nacimiento.anio;
    cin.ignore();
    system("clear");
    j.estado = true;
    return j;

};

//MENU
void menu() {
    cout << "Bienvenidos al Juego SIMON." << endl;
    cout << "Ingrese una de las siguientes opciones: " << endl;
    cout << "   1. Gestionar jugadores" << endl;
    cout << "   2. Jugar" << endl;
    cout << "   3. Informes" << endl;
    cout << "   4. Salir" << endl;
    cout << ">> ";
}
// IMPRIMIR JUGADOR
void imprimirJugador(Jugador j) {
  
    cout << "Alias: " << j.alias << endl;
    cout << "Nombre: " <<j.nombre << endl;
    cout << "Apellido: " << j.apellido << endl;
    cout << "Fecha de nacimiento: " << endl << "Día" << j.fecha_nacimiento.dia << endl;
    cout << "Mes:" << j.fecha_nacimiento.mes << endl;
    cout << "Año:" << j.fecha_nacimiento.mes << endl;
};

// ************************************************************************

int main () {
    menu();
    Jugador jugador;
    crear_jugador();
    imprimirJugador(jugador); 
  
    return 0;
}


