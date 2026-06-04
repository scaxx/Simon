#include <iostream> 
#include <unistd.h>

// --- CONSTANTES DEL SISTEMA ---
// Valores fijos que definen los límites del programa.
#define MAX_JUGADORES 10 // Capacidad máxima del vector de jugadores
#define MAX_PARTIDAS 50  // Capacidad máxima del historial de partidas
#define MAX_SECUENCIA 10  // Largo máximo absoluto de colores en el juego

// Configuración de longitud de la secuencia de colores según la dificultad elegida
#define LARGO_PRINCIPIANTE 6
#define LARGO_INTERMEDIO 8
#define LARGO_AVANZADO 10

// Puntos otorgados al jugador por cada ronda superada con éxito en cada nivel
#define PTS_PRINCIPIANTE 1
#define PTS_INTERMEDIO 3
#define PTS_AVANZADO 5

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
    int puntaje_maximo;
};    
struct Juego {
    Jugador jugadores[MAX_JUGADORES];
    int cantJugadores=0;
};

struct Partida {
    string aliasJugador;    // Quién jugó la partida
    string nivel;           // En qué dificultad (Principiante, Intermedio, Avanzado)
    int puntajeObtenido;    // Cuántos puntos hizo
    string resultado;       // El estado final ("Completado", "Perdido" o "Abandonado")
};



// ****************************************************


// ****************** Prototipos de Funciones y Procedimientos ************

Jugador crear_jugador();
void menu();
void imprimirJugador(Jugador j);
bool fecha_valida(Fecha f);
void gestion(Juego &juego_actual);
void opcionesInformes(Juego &juego_actual);
void mostrarTodosLosJugadores(Juego &juego_actual);
void agregar_jugador(Juego &juego_actual);
bool sonNumeros(string);
int convertirOpcion(string);
void esperar();
int leerEntrada();
void menuGestionDeJugadores();


// ************************************************************************

int main () {
    // 1. Creamos la estructura general del juego (que adentro ya tiene el arreglo y el contador)
    Juego miJuego; 

    // 2. Inicializamos la variable opcion
    int opcion;

    // 3. Mostramos el menú y manejamos las decisiones del usuario
    do {

        menu(); //Muestra las opciones en la consola

        opcion = leerEntrada(); //Usamos la función para leer con getline y convertir la opción en un número - no hay errores en el buffer

        system("clear");

        switch (opcion) {
            case 1:// Si digita 1, entra al menú para gestionar jugadores
                gestion(miJuego);
                break;

            case 2://Si digita 2, se debería inicializar el juego. Por el momento se despliega un mensaje.
                cout << "Juego momentáneamente fuera de servicio :(" << endl;
                break;

            case 3://Si digital 3, se despliegan los informes de los jugadores. Por el momento se despliega un mensaje.
                opcionesInformes(miJuego);
                break;
                
            case 4: //Sale del juego
                cout << "¡Gracias por jugar a SIMON! Saliendo del programa..." << endl;
                break;
                    
            default: //Sólo si opción < 1 u opción > 4
                cout << "¡ERROR! Opción inválida. Vuelve a intentar con un número del 1 al 4." << endl;
                esperar();
                break;
        }

        if (opcion != 4) {
            esperar();
        }

    } while (opcion != 4);    
  
    return 0;
}


//CREAR JUGADOR
Jugador crear_jugador(){
    Jugador j;
    system("clear");

    cout << "Ingresa tu alias: ";
    getline(cin, j.alias); 
    system("clear");

    cout << "Ingresa tu nombre: ";
    getline(cin, j.nombre); 
    system("clear");

    cout << "Ingresa tu apellido: ";
    getline (cin, j.apellido);
    system("clear");


    //Inicializamos el puntaje máximo en 0 para cada jugador creado
    j.puntaje_maximo = 0;

    do {
        string diaTexto;
        string anioTexto;
        cout << "Fecha de nacimiento:" << endl << " Día >> ";
        getline(cin, diaTexto); //Así evitamos el error del buffer y mantenemos la lógica en todo el código (getline)

        cout << " Mes (número del 1 al 12) >> ";
        getline(cin, j.fecha_nacimiento.mes); 
        
        cout << " Año (mayor o igual a 2000) >> ";
        getline(cin, anioTexto);
        system("clear");

        //Validamos y convertimos el contenido de diaTexto
        if (diaTexto.length() > 0 && sonNumeros(diaTexto)) {
            j.fecha_nacimiento.dia = stoi(diaTexto);
        } else {
            j.fecha_nacimiento.dia = -1; //Genera el error en la verificación de la fecha (fecha inválida)
        }

        //Validamos y convertimos el contenido de anioTexto
        if (anioTexto.length() > 0 && sonNumeros(anioTexto)) {
            j.fecha_nacimiento.anio = stoi(anioTexto);
        } else {
            j.fecha_nacimiento.anio = -1; //Genera el error en la verificación de la fecha (fecha inválida)
        }

        // Le pasamos la fecha cargada al bool
        if (!fecha_valida(j.fecha_nacimiento)) {
            cout << "¡ERROR! Fecha de nacimiento inválida. Intenta de nuevo." << endl;
        }

    } while (!fecha_valida(j.fecha_nacimiento)); // Se repite si bool da false

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

//MENU PARA GESTIÓN DE JUGADORES
void menuGestionDeJugadores() {
    cout << "MENÚ DE GESTIÓN" << endl;
    cout << "   1. Nuevo jugador" << endl;
    cout << "   2. Eliminar jugador" << endl;
    cout << "   3. Modificar jugador" << endl;
    cout << "   4. Consultar jugador" << endl;
    cout << "   5. Lista de jugadores" << endl;
    cout << "   6. Salir" << endl;
    cout << ">> ";
}

//DENTRO DEL MENÚ DE GESTIÓN
void gestion(Juego &juego_actual) {
    int opcionGestion; //Utilizamos int pero luego leemos la entrada con la función para realizar el cambio de type sin errores

    do {

        menuGestionDeJugadores(); //Mostramos el menú de gestión
        opcionGestion = leerEntrada(); //Usamos la función para leer con getline y convertir la opción a un número - no hay errores en el buffer
        system("clear");

        switch (opcionGestion) {
        case 1:
            crear_jugador();
            break;
        case 2:
            cout << "Opción en desarrollo" << endl;
            break;
        case 3:
            cout << "Opción en desarrollo" << endl;
            break;
        case 4:
            cout << "Opción en desarrollo" << endl;
            break;
        case 5:
            cout << "Opción en desarrollo" << endl;
            break;
        case 6:
            cout << "Regresando al menú principal..." << endl;
            break;
        default:
            cout << "¡ERROR! Opción inválida. Vuelve a intentar con un número del 1 al 6." << endl;
            esperar();
            break;
        }

        esperar();//Para visualizar correctamente los datos en caso de haber creado o editado un jugador

    } while (opcionGestion != 6);

}

// IMPRIMIR DATOS DE UN JUGADOR
void imprimirJugador(Jugador j) {
    cout << "Alias: " << j.alias << endl;
    cout << "Nombre: " <<j.nombre << endl;
    cout << "Apellido: " << j.apellido << endl;
    cout << "Fecha de nacimiento: " << endl << "Día: " << j.fecha_nacimiento.dia << endl;
    cout << "Mes: " << j.fecha_nacimiento.mes << endl;
    cout << "Año: " << j.fecha_nacimiento.anio << endl;
};

//COMPRUEBA SI LA FECHA INGRESADA ES VÁLIDA
bool fecha_valida(Fecha f) {
    // 1. Validar el año (debe ser mayor o igual a 2000)
    if (f.anio < 2000) {
        cout << "¡ERROR! El año ingresado debe ser mayor o igual a 2000." << endl;
        return false; 
    }

    //1.2 Validar que el año ingresado no sea mayor al año actual (igualmente estaría incorrecto)
    if (f.anio > 2026) {
        cout << "¡ERROR! El año ingresado debe ser menor a 2026." << endl;
        return false; 
    }

    // 2. Validar que el día no sea negativo o cero
    if (f.dia < 1 || f.dia > 31) {
        cout << "¡ERROR! El día ingresado no es válido." << endl;
        return false;
    }

    // 3. Determinar el límite de días según el mes escrito por el usuario
    int max_dias=0;
    // No tuvimos en cuenta el año bisciesto ¡¡te queremos Nico!!
    if (f.mes == "2") {
            max_dias = 28;
    } 
    // Meses con 30 días
    else if (f.mes == "4" || f.mes == "6" || f.mes == "9" || f.mes == "11") {
        max_dias = 30;
    } 
    //Meses con 31 días
    else if (f.mes == "1" || f.mes == "3" || f.mes == "5" || f.mes == "7" || f.mes == "8" || f.mes == "10" || f.mes == "12"){
        max_dias = 31;
    }
    //Si se ingresa algo diferente de los números esperados. 
    else {
        cout << "¡ERROR! El mes ingresado no es válido." << endl;
        return false;
    }
   
    //Si el día ingresado supera el máximo. 
    if (f.dia > max_dias) {
        cout << "¡ERROR! El día ingresado no es válido." << endl;
        return false;
    }

    return true; // Si pasó todos los filtros, la fecha es válida
}

// Guarda el jugador en el arreglo
void agregar_jugador(Juego &juego_actual) {
    if (juego_actual.cantJugadores < MAX_JUGADORES) {
        
        // Guarda el jugador creado en la posición libre actual
        juego_actual.jugadores[juego_actual.cantJugadores] = crear_jugador();
        
        cout << "--- JUGADOR REGISTRADO CON ÉXITO ---" << endl;
        // Imprime el jugador que acabamos de guardar
        imprimirJugador(juego_actual.jugadores[juego_actual.cantJugadores]);
        
        juego_actual.cantJugadores++; // Sumamos 1 al contador
    } else {
        cout << "¡ERROR! Máximo de jugadores alcanzado." << endl;
    }
}

// toma un string de entrada que representa la opción elegida 
// chequea que sea válida y devuelve el número correspondiente, o -1 si no es correcto
int convertirOpcion(string s) {
    if (!sonNumeros(s))
        return -1;
    int opcion = stoi(s);
    /*if (opcion < 1 || opcion > 4)
        return -1;
    else */ //Este bloque verifica que el valor esté dentro de determinado rango, para la fecha no nos sirve porque quedarían números fuera
    return opcion;
}

//Usamos la función para verificar que el texto está compuesto únicamente por números
bool sonNumeros(string s) {
    for (int i = 0; i < s.length(); i++)
        if (s[i] < '0' || s[i] > '9') //Convertimos la función en general para que podamos usarla en todos los casos
            return false;             //Verificamos qué números necesitamos dentro de cada función
    return true;
}

//Procedimiento general que utilizamos en casos de error
void esperar() {
    string continuar;
    cout << endl << "Presiona Enter para continuar...";
    getline(cin, continuar);
    system("clear");
}

int leerEntrada() {
    string s;
    getline(cin, s);
    return convertirOpcion(s);
}