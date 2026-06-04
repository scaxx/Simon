#include <iostream> 
#include <unistd.h>

// --- CONSTANTES DEL SISTEMA ---

// Valores fijos que definen los límites del programa.
#define MAX_JUGADORES 10 // Capacidad máxima del arreglo de jugadores
#define MAX_PARTIDAS 50  // Capacidad máxima del historial de partidas
#define MAX_SECUENCIA 10  // Largo máximo de colores en el juego

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
    int cantJugadores = 0;

    Partida partidas[MAX_PARTIDAS];
    int cantPartidas = 0;
};

struct Partida {
    string aliasJugador;    // Quién jugó la partida
    string nivel;           // En qué dificultad (Principiante, Intermedio, Avanzado)
    int puntajeObtenido;    // Cuántos puntos hizo
    string resultado;       // El estado final ("Completado", "Perdido" o "Abandonado")
};


// ****************** Prototipos de Funciones y Procedimientos ************

//Menú principal
void menu(); //Procedimiento que despliega el menú principal

// Creación de Jugador

Jugador crear_jugador(Juego &juego_actual);
void agregar_jugador(Juego &juego_actual); //Procedimiento que agrega un jugador creado recientemente a un arreglo de jugadores
bool fecha_valida(Fecha f); //Procedimiento para validar la fecha introducida por el jugador (Crear Jugador)

// Gestión de Jugadores

void menuGestionDeJugadores(); //Procedimiento que habilita el menú de Gestión de Jugadores
void gestion(Juego &juego_actual); //Procedimiento que permite operar dentro de la sección de Gestión de Jugadores
int buscarJugador(Juego juego_actual, string aliasBuscado); //Procedimiento para buscar a un jugador en específico mediante el alias
void imprimirJugador(Jugador j); //Procedimiento para imprimir al jugador seleccionado

// Sección de Informes
void menuInformes(); //Procedimiento que despliega el menú de Informes
void informes(Juego &juego_actual); //Procedimiento que permite operar dentro de la sección Informes
void historialDePartidas(Juego juego_actual); //Procedimiento que despliega el historial de todas las partidas jugadas
void partidasPorJugador(Juego juego_actual); //Procedimiento que despliega una lista de todas las partidas jugadas por cada jugador
void rankingGeneral(Juego juego_actual); //Procedimiento que despliega una lista de todos los jugadores de mayor a menor utilizando los puntajes acumulados
void mejorJugadorPorNivel(Juego juego_actual); //Procedimiento que despliega una lista de los mejores jugadores según cada nivel
void estadisticasGenerales(Juego juego_actual); //Procedimiento que despliega las estadísticas generales

//void mostrarTodosLosJugadores(Juego &juego_actual); //Procedimiento que despliega un listado de todos los jugadores del sistema - no sé si ponerlo jeee

// Funciones y procedimientos generales

bool sonNumeros(string); //Función para verificar que los datos ingresados son números
int convertirOpcion(string); //Función para leer un string y devolver un int
void esperar(); //Procedimiento para casos de error
int leerEntrada(); //Función que lee la entrada del usuario y utliza la función convertirOpcion para devolver un número


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
                informes(miJuego);
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
            //esperar(); Para visualizar correctamente la opción seleccionada
            // Por el momento lo comentamos para recordar que tenemos que realizar una función que espere un tiempo determinado antes de borrar pantalla
            // Para que el usuario pueda visualizar los datos con éxito
        }

    } while (opcion != 4);    
  
    return 0;
}


//CREAR JUGADOR
Jugador crear_jugador(Juego &juego_actual){
    Jugador j;
    bool aliasDisponible = false; //Definimos una variable para verificar que el alias esté disponible, asumimos que es false

    do {

        system("clear");
        cout << "Ingresa tu alias: ";
        getline(cin, j.alias);

        int posicionAlias = buscarJugador(juego_actual, j.alias);

        //Verificamos que el alias está disponible
        if (posicionAlias == -1) {
            aliasDisponible = true;
        } else {
            //El alias ya existe pero está inactivo, así que devolvemos el jugador para que no pida nuevamente los datos
            if (juego_actual.jugadores[posicionAlias].estado == false) {
                juego_actual.jugadores[posicionAlias].estado = true; //Seteamos el estado a true = activo
                cout << "¡Bienvenid@ devuelta" << juego_actual.jugadores[posicionAlias].alias << "!" << endl;

                return juego_actual.jugadores[posicionAlias];
            //El alias existe y está activo, devolvemos error y pedimos un alias nuevo
            } else {
                cout << "El alias " << juego_actual.jugadores[posicionAlias].alias << " se encuentra en uso por un jugador activo." << endl;
                cout << "Por favor, ingresa un alias válido." << endl;
                esperar();
            }
        }

    } while (aliasDisponible == false);

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

//MENÚ PARA INFORMES
void menuInformes() {
    cout << "MENÚ DE INFORMES" << endl;
    cout << "   1. Historial completo de partidas" << endl;
    cout << "   2. Partidas por jugador" << endl;
    cout << "   3. Ranking general (por puntaje acumulado)" << endl;
    cout << "   4. Mejor jugador por nivel" << endl;
    cout << "   5. Estadísticas generales" << endl;
    cout << "   6. Volver al menú principal" << endl;
    cout << ">> ";
};

//DENTRO DEL MENÚ DE INFORMES
void informes(Juego &juego_actual) {
    int opcionInformes; //Utilizamos int pero luego leemos la entrada con una función para realizaar el cambio de type sin errores
    
    do {

        menuInformes(); //Mostramos el menú de informes
        opcionInformes = leerEntrada(); //Usamos la función para leer la entrada y convertir a número
        system("clear");

        switch (opcionInformes) {
        case 1:
            cout << "Funcionalidad momentáneamente fuera de servicio :(" << endl;
            break;
        case 2:
            cout << "Funcionalidad momentáneamente fuera de servicio :(" << endl;
            break;
        case 3:
            cout << "Funcionalidad momentáneamente fuera de servicio :(" << endl;
            break;
        case 4:
            cout << "Funcionalidad momentáneamente fuera de servicio :(" << endl;
            break;
        case 5:
            cout << "Funcionalidad momentáneamente fuera de servicio :(" << endl;
            break;
        case 6:
            cout << "Volviendo al menú principal..." << endl;
            break;
        default:
            cout << "¡ERROR! Opción inválida. Vuelve a intentar con un número del 1 al 6." << endl;
            esperar();
            break;
        }

    } while (opcionInformes != 6);
    
}

//DENTRO DEL MENÚ DE GESTIÓN
void gestion(Juego &juego_actual) {
    int opcionGestion; //Como en informes, utilizamos int pero luego leemos la entrada con la función para realizar el cambio de type sin errores

    do {

        menuGestionDeJugadores(); //Mostramos el menú de gestión
        opcionGestion = leerEntrada(); //Usamos la función para leer con getline y convertir la opción a un número - no hay errores en el buffer
        system("clear");

        switch (opcionGestion) {
        case 1:
            //Usamos la función para agregar un nuevo jugador
            agregar_jugador(juego_actual);
            break;
        case 2:
            cout << "Opción en desarrollo" << endl;
            break;
        case 3:
            cout << "Opción en desarrollo" << endl;
            break;
        case 4: {
            //Definimos la variable que vamos a necesitar para buscar el alias del usuario
            string aliasBuscado;
            cout << "Por favor, ingresa el alias que deseas buscar: " << endl;
            getline(cin, aliasBuscado);

            //Usamos la función que definimos para buscar usuarios mediante el alias
            int posicion = buscarJugador(juego_actual, aliasBuscado);

            //Verificamos los resultados que devuelve la función
            if (posicion == -1) {
                //Si posicion -1 el alias no existe, por tanto el jugador no existe
                cout << "¡Lo sentimos! No se encontró ningún jugador con el alias " << aliasBuscado << "." << endl;
            } else {
                //Posición != -1 significa que el alias existe y se devuelve su posición correspondiente en el arreglo
                cout << "JUGADOR " << aliasBuscado << " ENCONTRADO:" << endl;
                //Usamos la función para imprimir y devolvemos todos los datos del jugador
                imprimirJugador(juego_actual.jugadores[posicion]);
            }
            break;
        }
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

        //esperar(); Para visualizar correctamente los datos en caso de haber creado o editado un jugador
        // Por el momento lo comentamos para recordar que tenemos que realizar una función que espere un tiempo determinado antes de borrar pantalla
        // Para que el usuario pueda visualizar los datos con éxito

    } while (opcionGestion != 6);

}

//BUSCAR UN JUGADOR - Devuelve la posición en el arreglo si lo encuentra, o -1 si no existe
int buscarJugador(Juego juego_actual, string aliasBuscado) {
    for (int i = 0; i < juego_actual.cantJugadores; i++) {

        if (juego_actual.jugadores[i].alias == aliasBuscado) {
            return i; //Encontramos el alias buscado y devolvemos la posición en la que se encuentra
        }

    }
    return -1; //Si no encontramos el alias, termina el bucle for y devuelve -1
}

// IMPRIMIR DATOS DE UN JUGADOR
void imprimirJugador(Jugador j) {
    cout << "Alias: " << j.alias << endl;
    cout << "Nombre: " <<j.nombre << endl;
    cout << "Apellido: " << j.apellido << endl;
    cout << "Fecha de nacimiento: " << endl;
    cout << "  Día: " << j.fecha_nacimiento.dia << endl;
    cout << "  Mes: " << j.fecha_nacimiento.mes << endl;
    cout << "  Año: " << j.fecha_nacimiento.anio << endl;
    cout << "Puntaje Máximo: " << j.puntaje_maximo << endl;

    //Verificamos el estado actual del jugador
    if (j.estado) {
        cout << "Estado: Activo" << endl;
    } else {
        cout << "Estado: Inactivo" << endl;
    }
    
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

        //Llamamos a la función para pedir datos y verificar si se reactiva un jugador o no
        Jugador resultado = crear_jugador(juego_actual);

        // Revisamos si el alias del resultado ya existe en nuestro arreglo antes de meterlo al final
        int posicion = buscarJugador(juego_actual, resultado.alias);

        // Si posicion es menor que cantJugadores, significa que el alias ya está en el arreglo
        if (posicion >= 0 && posicion < juego_actual.cantJugadores) {
            // Reactivamos el jugador
            cout << "JUGADOR REACTIVADO" << endl;
        } 
        else {
            // Si es un jugador nuevo lo guardamos en la posición libre actual y sumamos uno al contador
            juego_actual.jugadores[juego_actual.cantJugadores] = resultado;
            
            cout << "JUGADOR REGISTRADO CON ÉXITO" << endl;
            imprimirJugador(juego_actual.jugadores[juego_actual.cantJugadores]);
            
            juego_actual.cantJugadores++; // Sumamos 1 al contador de jugadores
        }
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
    /*if (opcion < 1 || opcion > 4) // Este bloque verifica que el valor esté dentro de determinado rango
        return -1;                  // Para fecha y menú de gestión no sirve porque nos quedarían funcionalidades fuera
    else */                         // Se verifica sólo con los case en cada switch
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