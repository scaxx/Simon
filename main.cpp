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

Jugador crear_jugador();
void menu();
void imprimirJugador(Jugador j);
bool fecha_valida(Fecha f);
void agregar_jugador(Juego &juego_actual);

// ************************************************************************

int main () {
    // 1. Creamos la estructura general del juego (que adentro ya tiene el arreglo y el contador)
    Juego miJuego; 
    // 2. Mostramos el menú inicial
    int opcion = 0;
    while (opcion != 4) {
        menu(); // Imprime las opciones (1. Gestionar, 2. Jugar, etc.)
        cin >> opcion;
        cin.ignore(); // Limpiamos el buffer para que los getline de abajo no se salten

        switch (opcion) {
            case 1:
                // Si digita 1, entra a pedir los datos y guardarlos
                agregar_jugador(miJuego); 
                break;
            
            case 4:
                cout << "\n¡Gracias por jugar al SIMON! Saliendo del programa..." << endl;
                break;
                
            default:
                cout << "\n[ERROR] Opcion invalida. Intente con un numero del 1 al 4." << endl;
                cout << "Presione Enter para continuar...";
                cin.get();
                system("clear");
                break;
        }
    }
  
    return 0;
}


//CREAR JUGADOR
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

    do {
        cout << "Fecha de nacimiento:" << endl << " Dia >> ";
        cin >> j.fecha_nacimiento.dia; 
        cin.ignore();

        cout << " Mes (numero del 01 al 12) >> ";
        getline(cin, j.fecha_nacimiento.mes); 
        
        cout << " Anio (Mayor o igual a 2000) >> ";
        cin >> j.fecha_nacimiento.anio;
        cin.ignore();
        system("clear");

        // AQUÍ ES DONDE SE CONECTAN: Le pasamos la fecha cargada a tu bool
        if (!fecha_valida(j.fecha_nacimiento)) {
            cout << "[ERROR] Fecha invalida o anio menor a 2000. Intente de nuevo.\n" << endl;
        }

    } while (!fecha_valida(j.fecha_nacimiento)); // Se repite si tu bool da false

    j.estado = true;
    return j;
};

//MENÚ
void menu() {
    cout << "Bienvenidos al Juego SIMON." << endl;
    cout << "Ingrese una de las siguientes opciones: " << endl;
    cout << "   1. Gestionar jugadores" << endl;
    cout << "   2. Jugar" << endl;
    cout << "   3. Informes" << endl;
    cout << "   4. Salir" << endl;
    cout << ">> ";
};

// IMPRIMIR DATOS DE UN JUGADOR
void imprimirJugador(Jugador j) {
    cout << "Alias: " << j.alias << endl;
    cout << "Nombre: " <<j.nombre << endl;
    cout << "Apellido: " << j.apellido << endl;
    cout << "Fecha de nacimiento: " << endl << "Día" << j.fecha_nacimiento.dia << endl;
    cout << "Mes:" << j.fecha_nacimiento.mes << endl;
    cout << "Año:" << j.fecha_nacimiento.anio << endl;
};
//COMPRUEBA SI LA FECHA INGRESADA ES VÁLIDA
bool fecha_valida(Fecha f) {
    // 1. Validar el año (mayor o igual a 2000)
    if (f.anio < 2000) {
        return false; 
    }

    // 2. Validar que el día no sea negativo o cero
    if (f.dia < 1) {
        return false;
    }

    // 3. Determinar el límite de días según el mes escrito por el usuario
    int max_dias=0;
// No tuvimos en cuenta el año bisciesto ¡¡te queremos Nico!!
    if (f.mes == "2" || f.mes == "02") {
            max_dias = 28;
    } 
    // Meses con 30 días
    else if (f.mes == "4" || f.mes == "04" || f.mes == "6" || f.mes == "06" || f.mes == "9" || f.mes == "09" || f.mes == "11") {
        max_dias = 30;
    } 
    //Meses con 31 días
    else if (f.mes == "1" || f.mes == "01" || f.mes == "3" || f.mes == "03" || f.mes == "5" || f.mes == "05" || f.mes == "7" || f.mes == "07" || f.mes == "8" || f.mes == "08" || f.mes == "10" || f.mes == "12"){
        max_dias = 31;
    }
    //Si se ingresa algo diferente de los números esperados. 
    else {
        cout << "[ERROR] El mes ingresado no existe." << endl;
        return false;
    }
   
    //Si el día ingresado supera el máximo. 
    if (f.dia > max_dias) {
        return false;
    }

    return true; // Si pasó todos los filtros, la fecha es válida
}

// NUEVA FUNCIÓN: Guarda el jugador en el arreglo
void agregar_jugador(Juego &juego_actual) {
    if (juego_actual.cantJugadores < MAX_JUGADORES) {
        
        // Guarda el jugador creado en la posición libre actual
        juego_actual.jugadores[juego_actual.cantJugadores] = crear_jugador();
        
        cout << "\n--- JUGADOR REGISTRADO CON EXITO ---" << endl;
        // Imprime el jugador que acabamos de guardar
        imprimirJugador(juego_actual.jugadores[juego_actual.cantJugadores]);
        
        juego_actual.cantJugadores++; // Sumamos 1 al contador
    } else {
        cout << "[ERROR] Maximo de jugadores alcanzado." << endl;
    }
}