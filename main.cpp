#include <iostream> 
#include <chrono>  //Para hacer sleep_for
#include <thread>  //Para hacer sleep_for
#include <cstdlib> // Contiene rand y srand
#include <ctime>   // Contiene time()




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

// Constantes de tiempo para la lógica del juego
#define TIEMPO_PRINCIPIANTE 1.5
#define TIEMPO_INTERMEDIO 1.3
#define TIEMPO_AVANZADO 1

// Constante para cambiar dinámicamente el año (fecha de nacimiento)
#define ANIO_ACTUAL 2026

using namespace std;


/* JUEGO SIMÓN */

// ****************** Estructuras *********************
struct Fecha{
    int dia;
    int mes;
    int anio;
};

struct Jugador{
    string alias;
    string nombre;
    string apellido;
    Fecha fecha_nacimiento;
    bool estado;
};  

enum Resultado {COMPLETADO, PERDIDO, ABANDONADO};
enum Nivel {PRINCIPIANTE, INTERMEDIO, AVANZADO};

struct Partida {
    string aliasJugador;    // Quién jugó la partida
    Nivel nivel;            // Dificultad: Principiante, Intermedio, Avanzado
    int puntajeObtenido;    // Cuántos puntos hizo
    Resultado resultado;    // Resultado final de la partida: Completado, perdido o abandonado
};

struct Sistema {
    Jugador jugadores[MAX_JUGADORES]; //Arreglo de jugadores y el límite permitido de jugadores en el mismo
    int cantJugadores; //Tope del arreglo

    Partida partidas[MAX_PARTIDAS]; //Arreglo de partidas y el límite permitido de partidas en el mismo
    int cantPartidas; //Tope del arreglo
};

// ****************** Prototipos de Funciones y Procedimientos ************

//Menú principal e iniciar juego
void desplegarMenuPrincipal(Sistema &juego_actual);//Procedimiento que despliega el menú principal
void menuPrincipal(); //Menú principal

// Creación de Jugador
Jugador crear_jugador(Sistema &juego_actual); //Procedimiento para crear un jugador
void agregar_jugador(Sistema &juego_actual); //Procedimiento que agrega un jugador creado recientemente a un arreglo de jugadores

// Gestión de Jugadores
void menuGestionDeJugadores(); //Procedimiento que habilita el menú de Gestión de Jugadores
void gestion(Sistema &juego_actual); //Procedimiento que permite operar dentro de la sección de Gestión de Jugadores
int buscarJugador(Sistema juego_actual, string aliasBuscado); //Procedimiento para buscar a un jugador en específico mediante el alias
void imprimirJugador(Jugador j); //Procedimiento para imprimir al jugador seleccionado
void editarJugador(Sistema &juego_actual); //Procedimiento para editar jugador
void bajaJugador (Sistema &juego_actual); // Pasa el jugador de activo a inactivo
void consultarJugador(Sistema juego_actual); //Procedimiento que imprime los datos del Alias consultado
void listaDeJugadores(Sistema juego_actual); //Procedimiento que despliega la lista de jugadores

//JUEGO
void generarSecuenciaAleatoria (char secuencia[], int largoDificultad); //Procedimiento que genera la secuencia aleatoria del juego
void comenzarPartida (Sistema &juego_actual); //Procedimiento que comienza la partida
bool validarEstadoJugador(Sistema &juego_actual, string aliasBuscado); //Función que valida que el jugador exista y esté activo
void seleccionarNivel(Nivel &nivelElegido, int &largoDificultad, int &puntosPorRonda, float &timepoMuestra); //Procedimiento que muestra el menú de niveles y setea las variables de dificultad
Resultado ejecutarRonda(char secuenciaCreadaPartida[], int ronda, float tiempoMuestra, int largoDificultad, int puntosPorRonda, int puntajeActual); //Función que ejecuta una ronda del juego y devuelve el resultado
void registrarPartida(Sistema &juego_actual, Partida nueva_partida); //Procedimiento para registrar una partida en el arreglo partidas

// Sección de Informes
void menuInformes(); //Procedimiento que despliega el menú de Informes
void informes(Sistema &juego_actual); //Procedimiento que permite operar dentro de la sección Informes
void mostrarHistorialPartidas(Sistema juego_actual); //Procedimiento que despliega el historial de todas las partidas jugadas en orden cronológico
void mostrarPartidasPorJugador(Sistema juego_actual); //Procedimiento que despliega una lista de todas las partidas jugadas por cada jugador
void rankingGeneral(Sistema juego_actual); //Procedimiento que despliega una lista de todos los jugadores de mayor a menor utilizando los puntajes acumulados
void mejorJugadorPorNivel(Sistema juego_actual); //Procedimiento que despliega una lista de los mejores jugadores según cada nivel
void estadisticasGenerales(Sistema juego_actual); //Procedimiento que despliega las estadísticas generales

// Funciones y procedimientos generales
string mostrarNivel(Nivel nivel); //Función que devuelve al jugador el nivel
string mostrarResultado(Resultado resultado); //Función que devuelve el resultado
int puntajeAcumuladoPorJugador(Sistema juego_actual, string aliasBuscado); //Función que devuelve el puntaje acumulado por el jugador en todas las partidas
int cantPartidasPorJugador(Sistema juego_actual, string aliasBuscado); //Función para determinar la cantidad de partidas de un jugador
bool verificarCampoInvalido(string entrada); //Función para verificar que los campos no se encuentren vacíos
bool sonNumeros(string entrada); //Función para verificar que los datos ingresados son números
int convertirOpcion(string entrada); //Función para leer un string y devolver un int
void esperar(); //Procedimiento para casos de error
int leerEntrada(); //Función que lee la entrada del jugador y utliza la función convertirOpcion para devolver un número
bool verificarAnio(int anio); //Función para verificar que el año ingresado sea válido
bool verificarMes(int mes); //Función para verificar que el mes ingresado por el jugador sea válido
int maxDias(int mes); //Función para determinar el máximo de días que tiene el mes ingresado por el jugador
bool verificarDia(int dia, int mes); //Función para verificar que el día ingresado sea válido
bool pedirAlias(Sistema &juego_actual, Jugador &j); //Función para pedir el alias
void pedirAnio(Fecha &f); //Procedimiento para pedir el año
void pedirMes(Fecha &f); //Procedimiento para pedir el mes
void pedirDia(Fecha &f); //Procedimiento para pedir el día
void ordenarJugadoresPorPuntaje(Sistema &juego_actual);

// ************************************************************************

int main () {
    // ACTIVAR LA SEMILLA ALEATORIA (Se hace UNA sola vez al principio de todo)
    srand(time(0));

    // Creamos la estructura general del juego (que adentro ya tiene el arreglo y el contador)
    Sistema miJuego; 

    // Seteamos las variables en 0 para evitar errores (porque sacamos el valor de la estructura para que quede todo prolijo jeje)
    miJuego.cantJugadores = 0;
    miJuego.cantPartidas = 0;

    // Desplegamos el menú principal para que el jugador pueda interactuar con el juego
    desplegarMenuPrincipal(miJuego);
  
    return 0;
}

// ---------- MENÚS ----------

//MENÚ PRINCIPAL
void menuPrincipal() {
    cout << "Bienvenidos al Juego SIMON." << endl;
    cout << "Ingrese una de las siguientes opciones: " << endl;
    cout << "   1. Gestionar jugadores" << endl;
    cout << "   2. Jugar" << endl;
    cout << "   3. Informes" << endl;
    cout << "   4. Salir" << endl;
    cout << ">> ";
}

//DENTRO DEL MENÚ PRINCIPAL
void desplegarMenuPrincipal(Sistema &juego_actual) {
    // Inicializamos la variable opcion donde se guarda la elección del jugador
    int opcion;

    // Mostramos el menú y manejamos las decisiones del jugador
    do {

        menuPrincipal(); //Muestra las opciones en la consola

        opcion = leerEntrada(); //Usamos la función para leer con getline y convertir la opción en un número (no hay errores en el buffer)

        system("clear");

        switch (opcion) {
            case 1:// Si digita 1, entra al menú para gestionar jugadores
                gestion(juego_actual);
                break;

            case 2:
                comenzarPartida(juego_actual);
                break;

            case 3://Si digital 3, se despliegan los informes de los jugadores
                informes(juego_actual);
                break;
                
            case 4: //Sale del juego
                cout << "¡Gracias por jugar a SIMON! Saliendo del programa..." << endl;
                break;
                    
            default: //Sólo si opción < 1 u opción > 4
                cout << "¡ERROR! Opción inválida. Vuelve a intentar con un número del 1 al 4." << endl;
                esperar();
                break;
        }

    } while (opcion != 4);
}

//MENU PARA GESTIÓN DE JUGADORES
void menuGestionDeJugadores() {
    cout << "MENÚ DE GESTIÓN" << endl;
    cout << "   1. Nuevo jugador" << endl;
    cout << "   2. Baja de jugador" << endl;
    cout << "   3. Modificar jugador" << endl;
    cout << "   4. Consultar jugador" << endl;
    cout << "   5. Lista de jugadores" << endl;
    cout << "   6. Salir" << endl;
    cout << ">> ";
}

//DENTRO DEL MENÚ DE GESTIÓN
void gestion(Sistema &juego_actual) {
    int opcionGestion; //Como en informes, utilizamos int pero luego leemos la entrada con la función para realizar el cambio de type sin errores

    do {

        menuGestionDeJugadores(); //Mostramos el menú de gestión
        opcionGestion = leerEntrada(); //Usamos la función para leer con getline y convertir la opción a un número (no hay errores en el buffer)
        system("clear");

        switch (opcionGestion) {
        case 1:
            cout << "---- ALTA DE JUGADOR ----"<< endl;
            //Usamos la función para agregar un nuevo jugador
            agregar_jugador(juego_actual);
            break;
        case 2:
            cout << "---- BAJA DE JUGADOR ----"<< endl;
            //Usamos la función para dar de baja sin eliminar
            bajaJugador(juego_actual);
            break;
        case 3:
            cout << "---- MODIFICACIÓN DE JUGADOR ----" << endl;
            //Utilizamos el procedimiento para editar el jugador
            editarJugador(juego_actual);
            break;
        case 4:
            cout << "---- CONSULTA DE JUGADOR ----"<< endl;
            //Consultamos al jugador, dentro usamos la función buscar jugador y utilizammos otra para imprimirlo
            consultarJugador(juego_actual);
            break;
        case 5:
            cout << "---- CONSULTA DE JUGADORES ----"<< endl;
            //Usamos el procedimiento para mostrar la lista de jugadores
            listaDeJugadores(juego_actual);            
            break;
        case 6:
            cout << "Regresando al menú principal..." << endl;
            break;
        default:
            cout << "¡Error! Opción inválida. Vuelve a intentar con un número del 1 al 6." << endl;
            esperar();
            break;
        }

        //esperar(); Para visualizar correctamente los datos en caso de haber creado o editado un jugador
        // Por el momento lo comentamos para recordar que tenemos que realizar una función que espere un tiempo determinado antes de borrar pantalla
        // Para que el jugador pueda visualizar los datos con éxito

    } while (opcionGestion != 6);

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
}

//DENTRO DEL MENÚ DE INFORMES
void informes(Sistema &juego_actual) {
    int opcionInformes; //Utilizamos int pero luego leemos la entrada con una función para realizar el cambio de type sin errores
    
    do {

        menuInformes(); //Mostramos el menú de informes
        opcionInformes = leerEntrada(); //Usamos la función para leer con getline y convertir a número
        system("clear");

        switch (opcionInformes) {
        case 1:
            cout << "---- HISTORIAL DE PARTIDAS ----" << endl;
            mostrarHistorialPartidas(juego_actual);
            break;
        case 2:
            cout << "---- PARTIDAS POR JUGADOR ----" << endl;
            mostrarPartidasPorJugador(juego_actual);
            break;
        case 3:
            cout << "---- RANKING GENERAL ----" << endl;
            rankingGeneral(juego_actual);
            break;
        case 4:
            cout << "---- MEJOR JUGADOR POR NIVEL ----" << endl;
            mejorJugadorPorNivel(juego_actual);
            break;
        case 5:
            cout << "---- ESTADÍSTICAS GENERALES ----" << endl;
            estadisticasGenerales(juego_actual);
            break;
        case 6:
            cout << "Volviendo al menú principal..." << endl;
            break;
        default:
            cout << "¡Error! Opción inválida. Vuelve a intentar con un número del 1 al 6." << endl;
            esperar();
            break;
        }

    } while (opcionInformes != 6);
    
}

// ---------- GESTIÓN DE JUGADORES ----------

//CREAR JUGADOR
Jugador crear_jugador(Sistema &juego_actual){
    Jugador j;

    //Si el alias se encuentra en el sistema devolvemos los datos y no creamos otro jugador (duplicado)
    if(!pedirAlias(juego_actual, j)) { //Si devuelve false el jugador vuelve a estar activo
        return j;
    }

    do {
        system("clear");
        cout << "Ingresa tu nombre: "; //En este caso no verificamos que se ingresen sólo letras porque puede ser un nombre con diferentes símbolos (Ej: El hijo de Elon Musk)
        getline(cin, j.nombre);
    } while (verificarCampoInvalido(j.nombre)); //Se repite mientras j.nombre esté vacío

    do {
        system("clear");
        cout << "Ingresa tu apellido: "; //Aplicamos la misma lógica que en el campo anterior
        getline(cin, j.apellido);
    } while (verificarCampoInvalido(j.apellido)); //Se repite mientras j.apellido esté vacío


    pedirAnio(j.fecha_nacimiento);
    pedirMes(j.fecha_nacimiento);
    pedirDia(j.fecha_nacimiento);

    system("clear");
    j.estado = true;
    return j;

}

//Guarda el jugador en el arreglo
void agregar_jugador(Sistema &juego_actual) {
    if (juego_actual.cantJugadores < MAX_JUGADORES) {

        //Llamamos a la función para pedir datos y verificar si se reactiva un jugador o no
        Jugador resultado = crear_jugador(juego_actual);

        // Revisamos si el alias del resultado ya existe en nuestro arreglo antes de meterlo al final
        int posicion = buscarJugador(juego_actual, resultado.alias);

        // Si posicion es menor que cantJugadores, significa que el alias ya está en el arreglo
        if (posicion >= 0 && posicion < juego_actual.cantJugadores) {
            // Reactivamos el jugador
            cout << "JUGADOR REACTIVADO" << endl;
            return;
        } 
        else {
            // Si es un jugador nuevo lo guardamos en la posición libre actual y sumamos uno al contador
            juego_actual.jugadores[juego_actual.cantJugadores] = resultado;
            
            cout << "JUGADOR REGISTRADO CON ÉXITO" << endl;
            
            juego_actual.cantJugadores++; // Sumamos 1 al contador de jugadores
        }
    } else {
        cout << "¡Error! Máximo de jugadores alcanzado." << endl;
    }
}

//BAJA DE JUGADOR
//Baja sin eliminar jugador (estado)
void bajaJugador(Sistema &juego_actual) {
    string aliasBuscado;
    system("clear");
    cout << "---- BAJA DE JUGADOR ---" << endl;
    cout << "Ingresa el alias del jugador que deseas dar de baja: " << endl;
    getline(cin, aliasBuscado);

    int posicion = buscarJugador(juego_actual, aliasBuscado);
    
    if (posicion == -1) { // SI NO EXISTE
        cout << "No se encontró ningún jugador con el alias " << aliasBuscado << endl;
        esperar();
    } 
    else { // SI EXISTE INACTIVO
        if (juego_actual.jugadores[posicion].estado == false) { 
            cout << "El jugador '" << aliasBuscado << "' ya se encuentra inactivo en el sistema." << endl;
        } 
        else { // SI EXISTE ACTVO 
            juego_actual.jugadores[posicion].estado = false; 
            cout << "¡Jugador " << aliasBuscado << " dado de baja con éxito! (Estado: Inactivo)" << endl;
        }
        esperar(); 
    }
}


//EDITAR JUGADOR
void editarJugador(Sistema &juego_actual) {
    string aliasBuscado;
    string nombreTemp;
    string apellidoTemp;
    system("clear");
    //Pedimos el alias para buscar el jugador que se quiere editar
    cout << "Ingresa el alias del jugador que quieres editar: " << endl;
    getline(cin, aliasBuscado);

    //Obtenemos la posición del alias buscado en el arreglo de jugadores
    int posicion = buscarJugador(juego_actual, aliasBuscado);

    //No se encuentra la posición en el arreglo
    if (posicion == -1) {
        cout << "El alias ingresado no se encuentra registrado." << endl;
        esperar();
    } else {
        //Validamos el estado del jugador encontrado
        if (juego_actual.jugadores[posicion].estado == false ) {
            cout << "El jugador " << aliasBuscado << " está inactivo y no puede ser modificado." << endl;
            esperar();
        } else {
            
            //Editamos el nombre (no puede quedar vacío)
            do {
                system("clear");
                cout << "EDITAR JUGADOR: " << endl;
                cout << "Ingresa el nuevo nombre: ";
                getline(cin, nombreTemp); //Guardamos el nombre editado en una variable temporal
            } while(verificarCampoInvalido(nombreTemp)); //Si true nombreTemp está vacío y se vuelve a pedir el nombre
            juego_actual.jugadores[posicion].nombre = nombreTemp; //Sale del do-while porque se ingresa un nombre válido y se guarda en el lugar correspondiente

            do {
                system("clear");
                cout << "EDITAR JUGADOR: " << endl;
                cout << "Ingresa el nuevo apellido: ";
                getline(cin, apellidoTemp); //Guardamos el apellido editado en una variable temporal
            } while(verificarCampoInvalido(apellidoTemp)); //Si true apellidoTemp está vacío y se vuelve a pedir el apellido
            juego_actual.jugadores[posicion].apellido = apellidoTemp; //Sale del do-while porque se ingresa un apellido válido y se guarda en el lugar correspondiente

            //Editamos la fecha usando las funciones definidas para eso
            pedirAnio(juego_actual.jugadores[posicion].fecha_nacimiento);
            pedirMes(juego_actual.jugadores[posicion].fecha_nacimiento);
            pedirDia(juego_actual.jugadores[posicion].fecha_nacimiento);

            system("clear");
            cout <<"¡Jugador actualizado con éxito!" << endl;
            esperar();
        }
    
    }
}

//CONSULTAR JUGADOR
//Devuelve la posición en el arreglo si lo encuentra o -1 si no existe
int buscarJugador(Sistema juego_actual, string aliasBuscado) {
    for (int i = 0; i < juego_actual.cantJugadores; i++) {

        if (juego_actual.jugadores[i].alias == aliasBuscado) {
            return i; //Encontramos el alias buscado y devolvemos la posición en la que se encuentra
        }

    }
    return -1; //Si no encontramos el alias, termina el bucle for y devuelve -1
}

//IMPRIMIR DATOS DE UN JUGADOR
void imprimirJugador(Jugador j) {

    cout << "Alias: " << j.alias << endl;
    cout << "Nombre: " << j.nombre << endl;
    cout << "Apellido: " << j.apellido << endl;
    cout << "Fecha de nacimiento: " 
        << j.fecha_nacimiento.dia << "/" 
        << j.fecha_nacimiento.mes << "/" 
        << j.fecha_nacimiento.anio << endl;
    
    if (j.estado) {
        cout << "Estado: Activo" << endl;
    } else {
        cout << "Estado: Inactivo" << endl;
    }
    cout << "---------------------------------------" << endl;
    
}

void consultarJugador(Sistema juego_actual) {
    string aliasBuscado;
    system("clear");
    cout << "--- CONSULTAR JUGADOR ---" << endl;
    cout << "Ingresa el alias del jugador que deseas buscar: ";
    getline(cin, aliasBuscado);

    int posicion = buscarJugador(juego_actual, aliasBuscado); // Buscamos la posición del jugador en el arreglo

    if (posicion == -1) { //No se encuentra el alias
        cout << endl << "El alias " << aliasBuscado << " no existe en el sistema." << endl;
    } else {
        cout << endl << "--- DATOS ENCONTRADOS ---" << endl; 
        imprimirJugador(juego_actual.jugadores[posicion]);// Utilizamos la función de imprimirJugador 
    }
    esperar(); // Para que el usuario pueda ver los datos antes de borrar la pantalla
}

//LISTA DE JUGADORES

// LISTADO DE LOS JUGADORES ACTIVOS EN ORDEN ALFABÉTICO
void listaDeJugadores(Sistema juego_actual) {
     // VALIDACIÓN: Si no hay jugadores en el sistema
    if (juego_actual.cantJugadores == 0) {
        cout << "No hay ningún jugador registrado en el sistema." << endl;
        esperar();
        return;
    } 
    //BUCLE DE ORDENAMIENTO BURBUJA 
    for ( int i = 0; i < juego_actual.cantJugadores - 1; i++){
        for ( int j = 0; j < juego_actual.cantJugadores - 1; j++){
            if(juego_actual.jugadores[j].alias > juego_actual.jugadores[j+1].alias){
                Jugador aux = juego_actual.jugadores[j]; //Uso aux para no perder ningún dato
                juego_actual.jugadores[j] = juego_actual.jugadores[j+1]; //El de la derecha pasa a la izquierda
                juego_actual.jugadores[j+1] = aux; //El aux pasa a la derecha
            }
        }
    }
    // BUCLE DE IMPRESIÓN
    int contadorActivos = 0; // Para saber si realmente imprimimos a alguien
    system("clear");
    cout << "=================================================================" << endl;
    cout << "                      LISTA DE JUGADORES ACTIVOS                 " << endl;
    cout << "=================================================================" << endl;

    for ( int i = 0 ; i < juego_actual.cantJugadores ; i++){
        if (juego_actual.jugadores[i].estado == true){
            imprimirJugador(juego_actual.jugadores[i]);
            contadorActivos++;
        }
    }
    // Si el contador quedó en 0 significa que todos los registrados están INACTIVOS
    if (contadorActivos == 0) {
        cout << "No hay jugadores activos en este momento en el sistema." << endl;
    } else {
        cout << "Total de jugadores activos mostrados: " << contadorActivos << endl;
    }

    cout << "=================================================================" << endl;
    esperar(); // Pausa para que el usuario pueda leer la lista con calma
}

// ********************* JUEGO PUNTO 5  **************************************

// DESARROLLO DE PARTIDAS
//Función que lleva adelante la ronda
Resultado ejecutarRonda(char secuenciaCreadaPartida[], int ronda, float tiempoMuestra, int largoDificultad, int puntosPorRonda, int puntajeActual) {
    system("clear");
    cout << "=== RONDA " << ronda << " ===" << endl;
    cout << "Simón dice: ";

    //Mostramos la secuencia generada hasta la ronda actual
    for (int i = 0; i < ronda; i++) {
        cout << secuenciaCreadaPartida[i] << " "; 
    }
    cout << endl;

    //Pausamos la pantalla según el float de la dificultad (multiplicado por 1000 para pasarlo a milisegundos enteros)
    this_thread::sleep_for(chrono::milliseconds((int)(tiempoMuestra * 1000)));
    system("clear");

    //Turno de la entrada del usuario
    cout << "=== TU TURNO ===" << endl;
    cout << "Ingresá la secuencia de colores juntos (ej: RVAN, sin espacios) o 'S' para abandonar." << endl;
    cout << "Respuesta: ";

    string secuencia;
    getline(cin, secuencia); //Guardamos la respuesta del usuario

    // Guardamos la longitud una sola vez para optimizar rendimiento
    int longitudRespuesta = secuencia.length();

    //Convertimos la respuesta del jugador a mayúsculas para evitar errores por minúsculas
    for (int i = 0; i < longitudRespuesta; i++) {
        secuencia[i] = toupper(secuencia[i]);
    }

    //Si el jugador decide abandonar la partida
    if (secuencia == "S") {
        cout << "Abandonaste la partida :(" << endl;
        esperar();
        return ABANDONADO;
    }

    //Si el jugador ingresa menos caracteres de los esperados
    if (secuencia.length() != ronda) {
        cout << "¡Ronda perdida!" << endl;
        esperar();
        return PERDIDO;
    }

    //Verificamos colores y las secuencias
    for (int i = 0; i < ronda; i++) {
        if ((secuencia[i] != 'R' && secuencia[i] != 'V' && secuencia[i] != 'A' && secuencia[i] != 'N') || (secuencia[i] != secuenciaCreadaPartida[i])) {
            cout << "¡Color incorrecto, ronda perdida!" << endl;
            esperar();
            return PERDIDO;
        }
    }

    // Si llegó acá, la ronda es correcta. Evaluamos si continúa o si es el final de la partida
    if (ronda < largoDificultad) {
        cout << "¡Excelente! Seguimos con la siguiente ronda..." << endl;
        cout << "Puntaje acumulado: " << puntajeActual + puntosPorRonda << endl;
        esperar();
    } else {
        cout << "¡FELICITACIONES! Completaste todas las rondas con éxito." << endl;
        cout << "Puntos totales de la partida: " << puntajeActual + puntosPorRonda << endl;
        esperar();
    }
    
    //Si el ciclo termina, el jugador completa el nivel correctamente
    return COMPLETADO;
}

//Procedimiento para seleccionar el nivel
void seleccionarNivel(Nivel &nivelElegido, int &largoDificultad, int &puntosPorRonda, float &tiempoMuestra) {
    //Variable para almacenar la opción del usuario
    int opcionNivel;
    
    do {
        //Limpiamos pantalla y mostramos las opciones al usuario
        system("clear");
        cout << "=== SELECCIONA EL NIVEL DE DIFICULTAD ===" << endl;
        cout << "1. Principiante (Secuencia: 6, Puntos: 1, Tiempo: 1.5s)" << endl;
        cout << "2. Intermedio   (Secuencia: 8, Puntos: 3, Tiempo: 1.3s)" << endl;
        cout << "3. Avanzado     (Secuencia: 10, Puntos: 5, Tiempo: 1.0s)" << endl;
        cout << "Opción: ";

        //Leemos la entrada y la guardamos en opcionNivel
        opcionNivel = leerEntrada();

        //Bucle para las opciones disponibles
        switch (opcionNivel) {
            case 1:
                nivelElegido = PRINCIPIANTE; // Selecciona el nivel principiante
                largoDificultad = LARGO_PRINCIPIANTE;
                puntosPorRonda = PTS_PRINCIPIANTE;
                tiempoMuestra = TIEMPO_PRINCIPIANTE;
                break;
            case 2:
                nivelElegido = INTERMEDIO;  // Selecciona el nivel medio
                largoDificultad = LARGO_INTERMEDIO;
                puntosPorRonda = PTS_INTERMEDIO;
                tiempoMuestra = TIEMPO_INTERMEDIO;
                break;
            case 3:
                nivelElegido = AVANZADO;    // Selecciona el nivel avanzado
                largoDificultad = LARGO_AVANZADO;
                puntosPorRonda = PTS_AVANZADO;
                tiempoMuestra = TIEMPO_AVANZADO;
                break;
                //En caso de que se ingrese algo diferente a lo esperado
            default: 
                cout << "¡Opción inválida! Por favor, selecciona 1, 2 o 3." << endl;
                esperar(); 
                break;
        }
        
    } while (opcionNivel != 1 && opcionNivel != 2 && opcionNivel != 3);
}

//Validamos el estado del jugador para comenzar a jugar
bool validarEstadoJugador(Sistema &juego_actual, string aliasBuscado) {
    //Buscamos la posición del jugador en el arreglo
    int posicion = buscarJugador(juego_actual, aliasBuscado);

    //Devuelve true o false dependiendo del estado del jugador
    return juego_actual.jugadores[posicion].estado; //Devuelve un boolean, por eso no es necesario hacer un if 

}

//Función para comenzar partida
void comenzarPartida(Sistema &juego_actual){
    system("clear");
    cout << "=== INICIANDO PARTIDA DE SIMÓN ===" << endl;

    string aliasBuscado;
    cout << "Ingresa el alias con el que deseas jugar: " << endl;
    getline(cin, aliasBuscado);

    //Buscamos al jugador
    int posicion = buscarJugador(juego_actual, aliasBuscado); 

    //No existe, por lo que lo enviamos a darse de alta
    if (posicion == -1){
        cout << "El alias " << aliasBuscado << " no se encuentra en el sistema. " << endl;
        cout << "Debes darte de alta en el menú de Gestión de Jugadores. " << endl;
        esperar();
        return;
    } 

    //El jugador existe pero está inactivo
    if (validarEstadoJugador(juego_actual, aliasBuscado) == false){
        cout << "El jugador " << aliasBuscado << " se encuentra inactivo." << endl;
        cout << "No puedes jugar hasta volver a estar activo." << endl;
        cout << "En Gestión de Jugadores debes darte de alta." << endl;
        esperar();
        return;
    }

    cout << "¡Bienvenid@, " << juego_actual.jugadores[posicion].alias << "!" << endl;
    esperar();

    // Variables para la partida
    Nivel nivelElegido; 
    int largoDificultad;
    int puntosPorRonda;
    float tiempoMuestra;

    seleccionarNivel(nivelElegido, largoDificultad, puntosPorRonda, tiempoMuestra);    
    cout << endl << "¡Nivel configurado con éxito!" << endl; 
    esperar();

    // VARIABLES DEL JUEGO
    char secuenciaCreadaPartida[MAX_SECUENCIA];
    int puntajePartida = 0;
    Resultado resultadoFinal = PERDIDO;
    bool sigueJugando = true;

    //Llamamos a la función encargada de generar la secuencia completa para jugar
    generarSecuenciaAleatoria(secuenciaCreadaPartida, largoDificultad);

    //Bucle para las rondas del juego
    for (int ronda = 1; ronda <= largoDificultad && sigueJugando; ronda++) {
        
        //La función ejecutarRonda se encarga de todo lo relacioando con el juego
        Resultado resultadoRonda = ejecutarRonda(secuenciaCreadaPartida, ronda, tiempoMuestra, largoDificultad, puntosPorRonda, puntajePartida);
        
        //Completa la ronda entonces sumamos puntos
        if (resultadoRonda == COMPLETADO) {
            puntajePartida += puntosPorRonda;

            if (ronda == largoDificultad) {
                resultadoFinal = COMPLETADO; 
            }
        } 
        else if (resultadoRonda == ABANDONADO) {
            resultadoFinal = ABANDONADO;
            sigueJugando = false; 
        } 
        else { 
            resultadoFinal = PERDIDO;
            sigueJugando = false; 
        }
        
    }

    // REGISTRO LOS DATOS DE LA PARTIDA JUGADA
    Partida partida_nueva;
    partida_nueva.aliasJugador = aliasBuscado;
    partida_nueva.nivel = nivelElegido;
    partida_nueva.puntajeObtenido = puntajePartida;
    partida_nueva.resultado = resultadoFinal;

    //Registramos la partida en el arreglo de partidas
    registrarPartida(juego_actual, partida_nueva);

    cout << "Gracias por jugar a SIMÓN. Volviendo al menú principal..." << endl;
    esperar();

}

// Función que llena el arreglo de la secuencia con colores aleatorios
void generarSecuenciaAleatoria (char secuencia[], int largoDificultad){

    // Recorremos el arreglo hasta el largo máximo del nivel elegido
    for (int i = 0; i < largoDificultad; i++) {   
        int numero = rand() % 4; // 1. Generamos un número aleatorio entre 0 y 3
        switch (numero) { // 2. Convertimos ese número en la letra correspondiente
            case 0:
                secuencia[i] = 'R'; // Rojo
                break;
            case 1:
                secuencia[i] = 'V'; // Verde
                break;
            case 2:
                secuencia[i] = 'A'; // Azul
                break;
            case 3:
                secuencia[i] = 'N'; // Naranja
                break;
        }
    }

}

// REGISTRO DE PARTIDAS
void registrarPartida(Sistema &juego_actual, Partida partida_nueva) {
    
    if (juego_actual.cantPartidas < MAX_PARTIDAS) { //Si hay lugar en el arreglo partidas

        juego_actual.partidas[juego_actual.cantPartidas] = partida_nueva; //Guardamos los datos de la partida actual en la posición correspondiente
        juego_actual.cantPartidas++; //Aumentamos el contador de partidas
        
        cout << "¡Partida registrada con éxito!" << endl; //Mensaje para el jugador :)

    } else { //Si no hay lugar en el arreglo
        cout << "Máximo de partidas guardadas alcanzado :(" << endl;
        esperar();
    }

}

// INFORMES
//HISTOIAL COMPLETO DE PARTIDAS
void mostrarHistorialPartidas(Sistema juego_actual) {
    //Si no hay partidas guardadas en el sistema
    if (juego_actual.cantPartidas == 0) {
        cout << "Aún no hay partidas registradas en el sistema." << endl;
    } else { //Hay una o más partidas guardadas
        //Hacemos un bucle for para recorrer el arreglo partidas e imprimir los datos de cada elemento dentro el arreglo
        for (int i = 0; i < juego_actual.cantPartidas; i++) {
            cout << "Jugador: " << juego_actual.partidas[i].aliasJugador << endl;
            cout << "Nivel: " << mostrarNivel(juego_actual.partidas[i].nivel) << endl;
            cout << "Puntaje: " << juego_actual.partidas[i].puntajeObtenido << endl;
            cout << "Resultado: " << mostrarResultado(juego_actual.partidas[i].resultado) << endl;
            cout << "=================================================================" << endl;
        }
    }
    esperar();
}

//PARTIDAS POR JUGADOR
void mostrarPartidasPorJugador(Sistema juego_actual) {
    string aliasBuscado;
    cout << "Ingresa el alias del jugador para ver sus patidas: ";
    getline(cin, aliasBuscado);

    int posicion = buscarJugador(juego_actual, aliasBuscado);

    if (posicion == -1) {
        cout << "El alias ingresado no se encuentra registrado." << endl;
    } else { //Se encuentra el alias, por lo que buscamos en el arrego partidas todas las veces que aparezca
        
        //Inicializamos una variable bandera para los casos en que hay un jugador registrado pero no ha jugado
        bool haJugado = false; //Asumimos que no ha jugado

        for (int i = 0; i < juego_actual.cantPartidas; i++) {
            if (juego_actual.partidas[i].aliasJugador == aliasBuscado) {
                cout << "Jugador: " << juego_actual.partidas[i].aliasJugador << endl;
                cout << "Nivel: " << mostrarNivel(juego_actual.partidas[i].nivel) << endl;
                cout << "Puntaje: " << juego_actual.partidas[i].puntajeObtenido << endl;
                cout << "Resultado: " << mostrarResultado(juego_actual.partidas[i].resultado) << endl;
                cout << "=================================================================" << endl;

                haJugado = true; //Se encontró por lo menos una partida que coincide con el alias
            }
        }

        //Si el jugador está ingresado en el sistema pero no ha jugado
        if (haJugado == false) {
            cout << "El jugador ingresado no tiene partidas registradas." << endl;
        }
    }
    esperar();
}

//RANKING GENERAL
void rankingGeneral(Sistema juego_actual) {

    cout << "Ranking General (mayor a menor puntaje): " << endl;
   

    if (juego_actual.cantJugadores == 0) {

        //En caso de que no hayan jugadores en el sistema
        cout << "Aún no hay jugadores registrados." << endl;
    
    } else {

        //Llamamos al procedimiento que ordena los jugadores por puntaje (mayor a menor)
        ordenarJugadoresPorPuntaje(juego_actual);

        //Hacemos un for para recorrer el arreglo de jugadores que ya están ordenados
        for (int i = 0; i < juego_actual.cantJugadores; i++) {
            
            //Imprimimos los datos ALIAS, CANTIDAD DE PARTIDAS JUGADAS Y PUNTAJE 
            cout << i + 1 << " - " << juego_actual.jugadores[i].alias << endl;
            cout << "Cantidad de partidas jugadas: " << cantPartidasPorJugador(juego_actual, juego_actual.jugadores[i].alias) << endl;
            cout << "Puntaje: " << puntajeAcumuladoPorJugador(juego_actual, juego_actual.jugadores[i].alias)<< endl;
            cout << "=================================================================" << endl;
        }
        
    }

    esperar();

}

//MEJOR JUGADOR POR NIVEL
void mejorJugadorPorNivel (Sistema juego_actual){
    //Definimos variables para buscar el máximo en cada nivel
    int maxPrincipiante = -1;
    string aliasPrincipiante = "";

    int maxIntermedio = -1;
    string aliasIntermedio = "";

    int maxAvanzado = -1;
    string aliasAvanzado = "";

    //Recorremos el bucle de partidas
    for (int i = 0 ; i < juego_actual.cantPartidas; i++ ){

        //Buscamos el máximo en el nivel PRINCIPIANTE
        if(juego_actual.partidas[i].nivel == PRINCIPIANTE){
            if (juego_actual.partidas[i].puntajeObtenido > maxPrincipiante){
                maxPrincipiante = juego_actual.partidas[i].puntajeObtenido;
                aliasPrincipiante = juego_actual.partidas[i].aliasJugador;
            }
        }

        //Buscamos el máximo en el nivel INTERMEDIO
        if (juego_actual.partidas[i].nivel == INTERMEDIO){
            if(juego_actual.partidas[i].puntajeObtenido > maxIntermedio){
                maxIntermedio= juego_actual.partidas[i].puntajeObtenido;
                aliasIntermedio = juego_actual.partidas[i].aliasJugador;
            } 
        }

        //Buscamos el máximo en el nivel AVANZADO
        if (juego_actual.partidas[i].nivel == AVANZADO){
            if(juego_actual.partidas[i].puntajeObtenido> maxAvanzado){
                maxAvanzado = juego_actual.partidas[i].puntajeObtenido;
                aliasAvanzado = juego_actual.partidas[i].aliasJugador;
            }
        }

    }

    //Devolvemos los datos encontrados
    cout << "--- Mejores jugadores por NIVEL ---" << endl;

    if (maxPrincipiante == -1){
        cout << "Nivel Principiante: No hay partidas registradas en este nivel." << endl;
    } else {
        cout << "Nivel Principiante: " << aliasPrincipiante << " con " << maxPrincipiante << "puntos." << endl;
    }

    if (maxIntermedio == -1){
            cout << "Nivel Intermedio: No hay partidas registradas en este nivel." << endl;
    } else {
            cout << "Nivel Intermedio: " << aliasIntermedio << " con " << maxIntermedio << "puntos." << endl;
    }

    if (maxAvanzado == -1){
        cout << "Nivel Avanzado: No hay partidas registradas en este nivel." << endl;
    } else {
        cout << "Nivel Avanzado: " << aliasAvanzado << " con " << maxAvanzado << "puntos." << endl;
    }

    esperar();

}

//ESTADISTICAS GENERALES
void estadisticasGenerales(Sistema juego_actual){
    int contActivos = 0;
    int partidasGanadas = 0; // Para contar las que tienen resultado COMPLETADO
    // Para contar cuántas veces se jugó cada nivel
    int cantPrin = 0;
    int cantInt = 0;
    int cantAva = 0;

    cout << "=== ESTADÍSTICAS GENERALES ===" << endl;
    cout << "Cantidad total de jugadores: " << juego_actual.cantJugadores << endl; //Ya tenemos este dato

    for (int i = 0 ; i < juego_actual.cantJugadores ; i++){ //Acá pide los activos asique los salimos a buscar y los contabilizamos 
        if (juego_actual.jugadores[i].estado == true){
            contActivos ++;
        }
    }
    cout << "Cantidad de jugadores activos: " << contActivos << endl;

    cout << "Cantidad total de partidas jugadas: " << juego_actual.cantPartidas << endl; //Cantidad de partidas jugadas en el sistema

    //PROMEDIO DE PUNTAJE
    int sumaPuntajes = 0;
    for(int i = 0; i<juego_actual.cantPartidas; i++){
        sumaPuntajes += juego_actual.partidas[i].puntajeObtenido;
        if(juego_actual.partidas[i].resultado == COMPLETADO ){  // PORCENTAJE DE PARTIDAS GANADAS (seguimos adentro del for)
        partidasGanadas++;
        }

        if (juego_actual.partidas[i].nivel == PRINCIPIANTE){
            cantPrin++;
        }if (juego_actual.partidas[i].nivel == INTERMEDIO){
            cantInt++;
        }if (juego_actual.partidas[i].nivel == AVANZADO){
            cantAva++;
        }
    }
    
    if (juego_actual.cantPartidas == 0){
        cout << "Aún no se han jugado partidas" << endl;
    } else {
        cout << "El promedio de puntajes es: " << (float)sumaPuntajes / juego_actual.cantPartidas << endl; //agrego float por si da con coma
        cout << "El porcentaje de partidas ganadas es: " << (float)partidasGanadas * 100 / juego_actual.cantPartidas << endl;
    }

    //Habría que revisar esta parte porque creo que se puede dividir en otra función
    // Declaramos las variables para operar
    string nivelMasJugado = "";
    int maxPartidasNivel = 0;

    // ¿Principiante le gana a nuestro máximo actual?
    if (cantPrin > maxPartidasNivel) {
        nivelMasJugado = "Principiante";
        maxPartidasNivel = cantPrin;
    }

    // ¿Intermedio le gana a nuestro máximo actual?
    if (cantInt > maxPartidasNivel) {
        nivelMasJugado = "Intermedio";
        maxPartidasNivel = cantInt;
    }

    // ¿Avanzado le gana a nuestro máximo actual?
    if (cantAva > maxPartidasNivel) {
        nivelMasJugado = "Avanzado";
        maxPartidasNivel = cantAva;
    }

    //Si el juego no tiene partidas aún
    if (juego_actual.cantPartidas == 0) {
        cout << "Aún no se ha jugado ninguna partida" << endl;
        esperar();
        return;
    } else { //Si tenemos un nivel máximo
        cout << "El nivel mas jugado fue el " << nivelMasJugado << " con " << maxPartidasNivel << " partidas." << endl;
    }
    esperar();
}


// ---------- PROCEDIMIENTOS Y FUNCIONES ----------

//Función que muestra el nivel al jugador
string mostrarNivel(Nivel nivel) {
    switch (nivel) {
        case PRINCIPIANTE:
            return "Principiante";
        case INTERMEDIO:
            return "Intermedio";
        case AVANZADO:
            return "Avanzado";
        default:
            return "Desconocido";
    }
}

//Función que muestra el resultado del juego
string mostrarResultado(Resultado resultado) {
    switch (resultado) {
        case COMPLETADO:
            return "Completado";
        case PERDIDO:
            return "Perdido";
        case ABANDONADO:
            return "Abandonado";
        default:
            return "Desconocido";
    }
}

//Función para obtener el puntaje acumulado por jugador
int puntajeAcumuladoPorJugador(Sistema juego_actual, string aliasBuscado) {
    int puntajeAcumulado = 0; //Inicializamos un contador para las partidas
    
    //Recorremos el arreglo partidas y buscamos el alias
    for (int i = 0; i < juego_actual.cantPartidas; i++){
        //Si encontramos un alias que coincide dentro de las partidas
        if(aliasBuscado == juego_actual.partidas[i].aliasJugador){
            //Sumamos el puntaje obtenido en esa partida específica al puntaje acumulado
            puntajeAcumulado += juego_actual.partidas[i].puntajeObtenido;
        }
    }

    //Devolvemos la suma de los puntajes de todas las partidas
    return puntajeAcumulado;
    
}

//Función para obtener la cantidad de partidas por jugador
int cantPartidasPorJugador(Sistema juego_actual, string aliasBuscado) {
    int cantidadPartidas = 0; //Inicializamos un contador para las partidas
    
    //Recorremos el arreglo partidas y buscamos el alias especificado por el usuario
    for (int i = 0; i < juego_actual.cantPartidas; i++) {
        if (juego_actual.partidas[i].aliasJugador == aliasBuscado) {
            //Si encontramos el alias hacemos cantidadPartidas++
            cantidadPartidas++;
        }
    }
    
    //Devolvemos la cantidad de partidas
    return cantidadPartidas;
    
}

//Algoritmo burbuja de ordenamiento para puntajes (mayor a menor)
//Usamos el & para modificar la copia de Sistema que recibe Ranking (que es donde se va a usar esta función)
//Sin tocar el Sistema original
void ordenarJugadoresPorPuntaje(Sistema &juego_actual) {

    //Variable temporal para mover los jugadores dentro del arreglo y no perder ninguno
    Jugador aux;

    for (int i = 0; i < juego_actual.cantJugadores - 1; i++) {
        for (int j = i + 1; j < juego_actual.cantJugadores; j++) {
            //Si el elemento en el índice j tiene mayor puntaje que el elemento en el índice i los intercambiamos
            if (puntajeAcumuladoPorJugador(juego_actual, juego_actual.jugadores[j].alias) > puntajeAcumuladoPorJugador(juego_actual, juego_actual.jugadores[i].alias) ) {
                aux = juego_actual.jugadores[i]; //El elemento en el índice i lo guardo en aux para no perderlo
                juego_actual.jugadores[i] = juego_actual.jugadores[j]; //El elemento en el índice i ahora tiene el contenido del elemento en el índice j
                juego_actual.jugadores[j] = aux; //El elemento en el índice j ahora tiene el contenido de aux, es decir el contenido inicial de i

            }    
        }
    }
}

//Función que se utliza para chequear que los campos dentro de los formularios sean válidos
bool verificarCampoInvalido(string s) {

    //Verificamos que el campo no contenga lo siguiente
    //Es poco probable que s tenga un largo de 0 porque tanto enter como espacio serían 1,
    //Pero lo hacemos en caso de que ocurra algún error inesperado
    if (s.length() == 0 || s == " " || s == "\n") {
        
        cout << "Este campo no puede permanecer vacío." << endl;
        cout << "Por favor, ingresa una entrada válida." << endl;
        esperar();
        return true; //El campo es inválido

    }

    return false; //El campo es válido, entonces continúa

}

//Toma un string de entrada que representa la opción elegida 
//Chequea que sea válida y devuelve el número correspondiente, o -1 si no es correcto
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
    this_thread::sleep_for(chrono::milliseconds(1000));
    system("clear");
    return;

}

//Función para pedir el alias del jugador
bool pedirAlias (Sistema &juego_actual, Jugador &j) {
    
    //Definimos una variable para verificar que el alias esté disponible, asumimos que es false
    bool aliasDisponible = false; 

    do {

        do {

            system("clear");
            cout << "Ingresa tu alias: ";
            getline(cin, j.alias);
        } while (verificarCampoInvalido(j.alias));

        int posicionAlias = buscarJugador(juego_actual, j.alias);

        //Verificamos si el alias está disponible
        if (posicionAlias == -1) {
            aliasDisponible = true;
        } else {
            /*El alias ya existe pero está inactivo, así que devolvemos 
            los datos del jugador para que no pida nuevamente los datos*/
            if (juego_actual.jugadores[posicionAlias].estado == false) {
                juego_actual.jugadores[posicionAlias].estado = true; //Seteamos el estado a true (activo)
                cout << "¡Bienvenid@ devuelta " << juego_actual.jugadores[posicionAlias].alias << "!" << endl;
                esperar();

                j = juego_actual.jugadores[posicionAlias]; //Guardamos los datos para devolverlos al jugador
                return false; //Evitamos crear otra vez el mismo jugador
            //El alias existe y está activo, devolvemos error y pedimos un alias nuevo
            } else {
                cout << "El alias " << juego_actual.jugadores[posicionAlias].alias << " se encuentra en uso por un jugador activo." << endl;
                cout << "Por favor, ingresa un alias válido." << endl;
                esperar();
            }
        }

    } while (aliasDisponible == false);

    return true; //El alias no existe, continuamos con la creación del jugador
}

//Procedimiento para pedir el año
void pedirAnio(Fecha &f) {
    string anioTexto; //Definimos una variable para manejar el año como string (para usar con getline)
    bool anioOk = false; //Asumimos que año no está ok

    do {
        system("clear");
        cout << "Ingresa tu fecha de nacimiento: " << endl;
        cout << "Año (mayor o igual a 2000): ";
        getline(cin, anioTexto);

        if(!verificarCampoInvalido(anioTexto)) { //El campo no está vacío
            if(sonNumeros(anioTexto)) {
                int anioTemp = stoi(anioTexto);//Convertimos anioTexto en número y lo guardamos en anioTemp
                if(verificarAnio(anioTemp)) { //Usamos la función que creamos para verificar si es un año ok
                    f.anio = anioTemp; //Guardamos el valor de anioTemp en el campo anio del jugador determinado
                    anioOk = true;
                } else { //El jugador ingresa un número fuera de rango
                    cout << "¡Error! Año fuera del rango permitido (2000 a 2026)." << endl;
                    esperar();
                }
            } else { //El jugador no ingresa números
                cout << "¡Error! Sólo se admiten números en este campo." << endl;
                esperar();
            }
        }

    } while (!anioOk);
}

//Procedimiento para pedir el mes
void pedirMes(Fecha &f){
    string mesTexto; //Usamos la misma lógica que en el campo anterior (getline)
    bool mesOk = false; //Asumimos que el mes no está ok

    do {
        system("clear");
        cout << "Mes (entre 1 y 12): ";
        getline(cin, mesTexto);

        if(!verificarCampoInvalido(mesTexto)) { //Verificamos que no está vacío
            if(sonNumeros(mesTexto)) { //Verificamos que el jugador ingresa sólo números
                int mesTemp = stoi(mesTexto); //Si ingresa números, los pasamos a int y guardamos en mesTemp
                if(verificarMes(mesTemp)) { //Usamos la función que definimos para verificar que el mes está ok
                    f.mes = mesTemp;
                    mesOk = true;
                } else { //El jugador ingresa números fuera de rango
                    cout << "¡Error! Mes inválido. Prueba utilizar números del 1 al 12." << endl;
                    esperar();
                }
            } else { //El jugador no ingresa números
                cout << "¡Error! Sólo se admiten números en este campo" << endl;
                esperar();
            }
        }
    } while (!mesOk);
}

//Procedimiento para pedir el día
void pedirDia(Fecha &f){
    string diaTexto; //Seguimos la lógica del bloque anterior
    bool diaOk = false; //Asumimos que diaOk false

    do {
        system("clear");
        cout << "Día (del 1 al " << maxDias(f.mes) << "); "<< endl;
        getline(cin, diaTexto);

        if(!verificarCampoInvalido(diaTexto)) { //Verificamos que el campo no esté vacío
            if(sonNumeros(diaTexto)) { //Verificamos que el jugador ingresa números
                int diaTemp = stoi(diaTexto); //Convertimos el contenido de diaTexto en número con stoi y guardamos en diaTemp
                if(verificarDia(diaTemp, f.mes)) { //Verificamos que el día sea válido para el mes ingresado anteriormente
                    f.dia = diaTemp; //Guardamos el valor de diaTemp en f.dia
                    diaOk = true; //Como es true termina el do-while
                } else { //Ingresa un día fuera de rango para el mes
                    cout << "¡Error! El día no es válido para el mes ingresado" << endl;
                    esperar();
                }
            } else { //El jugador no ingresa números
                cout << "¡Error! Sólo se admiten números en este campo." << endl;
                esperar();
            }
        }
    } while (!diaOk);
}

//Función para leer entradas y, mediante la función convertirOpción se devuelve un número
int leerEntrada() {
    string s;
    getline(cin, s);
    return convertirOpcion(s);
}

//Función para verificar el año ingresado por el jugador
bool verificarAnio(int anio) {
    if (anio >= 2000 && anio <= ANIO_ACTUAL) { //Usamos esta constante para que sea más fácil editar el año.
        return true;                           //La letra no lo aclara pero no debería ser posible registrar jugadores nacidos desde el
    } else {                                   //2023 aprox en adelante (por edad) ni personas nacidas posterior al año actual
        return false;                          //Por eso pusimos el tope de edad aunque se puede quitar fácilmente
    }
}

//Función para verificar el mes ingresado por el jugador
bool verificarMes(int mes) {
    if (mes >= 1 && mes <= 12) {
        return true;
    } else {
        return false;
    }
}

//Función para determinar el máximo de días que tiene el mes ingresado por el jugador
int maxDias(int mes) {
    if (mes == 2) {
        return 28;
    }
    if (mes == 4 || mes == 6 || mes == 9 || mes == 11) {
        return 30;
    }
    return 31; //Si no devolvió ni 28 ni 30 es porque el mes tiene 31 días, por lo que no tiene sentido verificar mediante un if
}

//Función para verificar el dia ingresado por el jugador
bool verificarDia(int dia, int mes) {
    int maximoDias = maxDias(mes);
    if (dia >= 1 && dia <= maximoDias) {
        return true; //Devolvemos true sólo si el día ingresado está entre 1 y maximoDias o es igual a uno de ellos
    } else {
        return false;
    }
}