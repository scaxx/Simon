#include <iostream> 
#include <unistd.h>
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
    int puntaje_acumulado; //Revisar
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
void listaDeJugadores(Sistema &juego_actual); //Procedimiento que despliega la lista de jugadores
//void eliminarJugador(Sistema &juego_actual); //Procedimiento para eliminar jugador

//JUEGO
void generarSecuenciaAleatoria (char secuencia[], int largoDificultad); 
void comenzarPartida (Sistema &juego_actual);
void registrarPartida(Sistema &juego_actual, Partida nueva_partida); //Procedimiento para registrar una partida en el arreglo partidas

// Sección de Informes
void menuInformes(); //Procedimiento que despliega el menú de Informes
void informes(Sistema &juego_actual); //Procedimiento que permite operar dentro de la sección Informes
void mostrarHistorialPartidas(Sistema juego_actual); //Procedimiento que despliega el historial de todas las partidas jugadas
void partidasPorJugador(Sistema juego_actual); //Procedimiento que despliega una lista de todas las partidas jugadas por cada jugador
void rankingGeneral(Sistema juego_actual); //Procedimiento que despliega una lista de todos los jugadores de mayor a menor utilizando los puntajes acumulados
void mejorJugadorPorNivel(Sistema juego_actual); //Procedimiento que despliega una lista de los mejores jugadores según cada nivel
void estadisticasGenerales(Sistema juego_actual); //Procedimiento que despliega las estadísticas generales

//void mostrarTodosLosJugadores(Juego &juego_actual); //Procedimiento que despliega un listado de todos los jugadores del sistema - no sé si ponerlo jeee

// Funciones y procedimientos generales
bool verificarCampoVacio(string entrada); //Función para verificar que los campos no se encuentren vacíos
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

            case 2://Si digita 2, se debería inicializar el juego. Por el momento se despliega un mensaje
                cout << "Juego momentáneamente fuera de servicio :(" << endl;
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

        if (opcion != 4) {
            //esperar(); Para visualizar correctamente la opción seleccionada
            // Por el momento lo comentamos para recordar que tenemos que realizar una función que espere un tiempo determinado antes de borrar pantalla
            // Para que el jugador pueda visualizar los datos con éxito
        }

    } while (opcion != 4);
}

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
    } while (verificarCampoVacio(j.nombre)); //Se repite mientras j.nombre esté vacío

    do {
        system("clear");
        cout << "Ingresa tu apellido: "; //Aplicamos la misma lógica que en el campo anterior
        getline(cin, j.apellido);
    } while (verificarCampoVacio(j.apellido)); //Se repite mientras j.apellido esté vacío

    //Inicializamos el puntaje del jugador en 0
    j.puntaje_acumulado = 0;

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
            //imprimirJugador(juego_actual.jugadores[juego_actual.cantJugadores]);
            
            juego_actual.cantJugadores++; // Sumamos 1 al contador de jugadores
        }
    } else {
        cout << "¡ERROR! Máximo de jugadores alcanzado." << endl;
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

/* Lo dejamos comentado para revisar después
//ELIMINAR JUGADOR
void eliminarJugador(Juego &juego_actual) {
    string aliasBuscado;
    system("clear");
    cout << "Ingresa el alias del jugador que deseas eliminar: " << endl;
    getline(cin, aliasBuscado);

    //Usamos la función para buscar el jugador mediante el alias
    int posicion = buscarJugador(juego_actual, aliasBuscado);

    //Si el alias no se encuentra en el arreglo jugadores
    if (posicion == -1) {
        cout << "No se encontró ningún jugador con el alias " << aliasBuscado << endl;
        esperar();
    } else { //Si el alias se encuentra en el arreglo jugadores
        //Ingresamos al arreglo para empezar a comparar
        for (int i = 0; i < juego_actual.cantJugadores; i++){
            //Cuando encontramos el alias
            if (juego_actual.jugadores[i].alias == aliasBuscado) {
                //Empezamos la lógica para correr los elementos del arreglo (los jugadores) hacia la izquierda
                for ( int j = i ; j < juego_actual.cantJugadores - 1; j++){
                    juego_actual.jugadores[j] = juego_actual.jugadores[j + 1];
                    
                }
                //Bajamos 1 en el tope del arreglo de jugadores
                juego_actual.cantJugadores--;
                cout << "¡Jugador eliminado con éxito!" << endl; //Avisamos al jugador que el jugador seleccionado ha sido eliminado
                return; //Terminamos al función para que no siga buscando jugadores
            }
        }        
    }
    
} */

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
            cout << "No puedes editar a otros jugadores. Vuelve a intentarlo" << endl;
            esperar();
        } else {
            
            //Editamos el nombre (no puede quedar vacío)
            do {
                system("clear");
                cout << "EDITAR JUGADOR: " << endl;
                cout << "Ingresa el nuevo nombre: ";
                getline(cin, nombreTemp); //Guardamos el nombre editado en una variable temporal
            } while(verificarCampoVacio(nombreTemp)); //Si true nombreTemp está vacío y se vuelve a pedir el nombre
            juego_actual.jugadores[posicion].nombre = nombreTemp; //Sale del do-while porque se ingresa un nombre válido y se guarda en el lugar correspondiente

            do {
                system("clear");
                cout << "EDITAR JUGADOR: " << endl;
                cout << "Ingresa el nuevo apellido: ";
                getline(cin, apellidoTemp); //Guardamos el apellido editado en una variable temporal
            } while(verificarCampoVacio(apellidoTemp)); //Si true apellidoTemp está vacío y se vuelve a pedir el apellido
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
        cout << endl << "El alias '" << aliasBuscado << "' no existe en el sistema." << endl;
    } else {
        cout << endl << "--- DATOS ENCONTRADOS ---" << endl; 
        imprimirJugador(juego_actual.jugadores[posicion]);// Utilizamos la función de imprimirJugador 
    }
    esperar(); // Para que el usuario pueda ver los datos antes de borrar la pantalla
}



//LISTA DE JUGADORES

// LISTADO DE LOS JUGADORES ACTIVOS EN ORDEN ALFABÉTICO
void listaDeJugadores(Sistema &juego_actual) {
     // VALIDACIÓN: Si no hay jugadores en el sistema
    if (juego_actual.cantJugadores == 0) {
        cout << "No hay ningún jugador registrado en el sistema." << endl;
        esperar();
        return;
    } 
    //BUCLE DE ORDENAMIENTO BURBUJA 
    for ( int i = 0; i < juego_actual.cantJugadores - 1; i++){
        for ( int j = 0; j < juego_actual.cantJugadores -1; j++){
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

// PARA INICIAR UNA PARTIDA (Punto 5)
void comenzarPartida(Sistema &juego_actual){
    system("clear");
    cout << "Iniciando la partida de SIMON" << endl;

    string aliasBuscado; //SOLICITAR ALIAS
    cout << "Ingresa el alias con el que deseas jugar" << endl;
    getline(cin, aliasBuscado);

    int posicion = buscarJugador(juego_actual, aliasBuscado); 

    if (posicion == -1){
        cout << "El alias" << aliasBuscado << " no se encuentra en el sistema. " << endl;
        cout << "Debes darte de alta en el menú de Gestión de Jugadores. " << endl;
        esperar();
        return; //corta y vuelve al menú principal
    } 
    if (juego_actual.jugadores[posicion].estado == false){
        cout << "El jugador" << aliasBuscado << " se encuentra INACTIVO." << endl;
        cout << "No puedes jugar hasta volver a estar activo." << endl;
        cout << "En gestión de jugadores debes darte de alta." << endl;
        esperar();
        return; //corta y vuelve al menu principal
    }

    //PASÓ LOS FILTOS, ASIQUE ESTÁ ACTIVO Y PUEDE JUGAR
    cout << "¡Bienvenid@ de nuevo, " << juego_actual.jugadores[posicion].nombre << "!" << endl;

}

//NIVELES (Punto 5.1)





// DESARROLLO DE PARTIDAS
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
        
        int posicion = buscarJugador(juego_actual, partida_nueva.aliasJugador); //Buscamos la posición del jugador mediante su alias

        juego_actual.jugadores[posicion].puntaje_acumulado += partida_nueva.puntajeObtenido; //Le sumo los puntos correspondientes al jugador en el arreglo jugadores usando su alias
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
            cout << "Nivel: " << juego_actual.partidas[i].nivel << endl;
            cout << "Puntaje: " << juego_actual.partidas[i].puntajeObtenido << endl;
            cout << "Resultado: " << juego_actual.partidas[i].resultado << endl;
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
                //cout << "Nivel: " << juego_actual.partidas[i].nivel << endl; //  cuando imprimimos el nivel solo dice 0,1 o 2 
                cout << "Nivel: ";
                switch(juego_actual.partidas[i].nivel) {
                    case PRINCIPIANTE: cout << "Principiante"; break;
                    case INTERMEDIO:   cout << "Intermedio"; break;
                    case AVANZADO:     cout << "Avanzado"; break;
                }
                cout << endl;
                cout << "Puntaje: " << juego_actual.partidas[i].puntajeObtenido << endl;
                cout << "Resultado: " << juego_actual.partidas[i].resultado << endl;
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

    cout << "Rankig General (mayor a menor puntaje): " << endl;

    if (juego_actual.cantJugadores == 0) {

        //En caso de que no hayan jugadores en el sistema
        cout << "Aún no hay jugadores registrados." << endl;
    
    } else {

        //Llamamos al procedimiento que ordena los jugadores por puntaje (mayor a menor)
        ordenarJugadoresPorPuntaje(juego_actual);

        //Hacemos un for para recorrer el arreglo de jugadores que ya están ordenados
        for (int i = 0; i < juego_actual.cantJugadores; i++) {
            
            //Imprimimos los datos ALIAS Y PUNTAJE 
            cout << i + 1 << " - " << juego_actual.jugadores[i].alias << endl;
            cout << "Puntaje: " << juego_actual.jugadores[i].puntaje_acumulado << endl;
            cout << "=================================================================" << endl;
        }
        
    }

    esperar();

}

//MEJOR JUGADOR POR NIVEL
void mejorJugadorPorNivel (Sistema juego_actual){
int maxPrincipiante = -1;
string aliasPrincipiante = "";

int maxIntermedio = -1;
string aliasIntermedio = "";

int maxAvanzado = -1;
string aliasAvanzado = "";

for (int i = 0 ; i < juego_actual.cantPartidas; i++ ){
    if(juego_actual.partidas[i].nivel == PRINCIPIANTE){
        if (juego_actual.partidas[i].puntajeObtenido > maxPrincipiante){
            maxPrincipiante = juego_actual.partidas[i].puntajeObtenido;
            aliasPrincipiante = juego_actual.partidas[i].aliasJugador;
        }
    }
    else if (juego_actual.partidas[i].nivel == INTERMEDIO){
        if(juego_actual.partidas[i].puntajeObtenido > maxIntermedio){
            maxIntermedio= juego_actual.partidas[i].puntajeObtenido;
            aliasIntermedio = juego_actual.partidas[i].aliasJugador;
        } 
    }
    else if (juego_actual.partidas[i].nivel == AVANZADO){
        if(juego_actual.partidas[i].puntajeObtenido> maxAvanzado){
            maxAvanzado = juego_actual.partidas[i].puntajeObtenido;
            aliasAvanzado = juego_actual.partidas[i].aliasJugador;
        }
    }

}
cout << "--- Mejores jugadores por NIVEL ---" << endl;
    if (maxPrincipiante == -1){
        cout << "Nivel Principiante: No hay partidas registradas en este nivel." << endl;
    }
     else {
        cout << "Nivel Princpiante: " << aliasPrincipiante << " con " << maxPrincipiante << "puntos." << endl;
     }
 if (maxIntermedio == -1){
        cout << "No hay jugadores que hayan jugado en este nivel" << endl;
 }else {
        cout << "Nivel Intermedio: " << aliasIntermedio << " con " << maxIntermedio << "puntos." << endl;
     }
  if (maxAvanzado == -1){
        cout << "No hay jugadores que hayan jugado en este nivel" << endl;
    }else {
        cout << "Nivel Avanzado: " << aliasAvanzado << " con " << maxAvanzado << "puntos." << endl;
     }

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
    cout << "Cantidad total de jugadores : " << juego_actual.cantJugadores << endl; //Ya tenemos este dato

    for (int i = 0 ; i < juego_actual.cantJugadores ; i++){ //Acá pide los activos asique los salimos a buscar y los contabilizamos 
        if (juego_actual.jugadores[i].estado == true){
            contActivos ++;
        }
    }
    cout << "La cantidad de jugadores activos es: " << contActivos << endl;
    //PORMEDIO DE PUNTAJE
    int sumaPuntajes = 0;
    for(int i = 0; i<juego_actual.cantPartidas; i++){
        sumaPuntajes += juego_actual.partidas[i].puntajeObtenido;
        if(juego_actual.partidas[i].resultado == COMPLETADO ){  // PORCENTAJE DE PARTIDAS GANADAS (seguimos adentro del for)
        partidasGanadas++;
        }

        if (juego_actual.partidas[i].nivel == PRINCIPIANTE){
            cantPrin++;
        }else if (juego_actual.partidas[i].nivel == INTERMEDIO){
            cantInt++;
        }else if (juego_actual.partidas[i].nivel == AVANZADO){
            cantAva++;
        }
    }
    
    if (juego_actual.cantPartidas == 0){
        cout << "Aún no hay partidas jugadas"<< endl;
    } else {
        cout << "El promedio de puntajes es: " << (float)sumaPuntajes / juego_actual.cantPartidas << endl; //agrego float por si da con coma
        cout << "El porcentaje de partidas ganadas es: " << (float)partidasGanadas *100 / juego_actual.cantPartidas << endl;
    }

    // Asumimos que el más jugado fue principiante
string nivelMasJugado = "Principiante";
int maxPartidasNivel = cantPrin;

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

cout << "El nivel mas jugado fue el " << nivelMasJugado << " con " << maxPartidasNivel << " partidas." << endl; //Variables ganadoras
}

//CHEQUEAR VALIDACIONES punto 8










// ---------- PROCEDIMIENTOS Y FUNCIONES ----------

//Algoritmo burbuja de ordenamiento para puntajes (mayor a menor)
void ordenarJugadoresPorPuntaje(Sistema &juego_actual) {
    //Variable temporal para mover los jugadores dentro del arreglo y no perder ninguno
    Jugador aux;

    for (int i = 0; i < juego_actual.cantJugadores - 1; i++) {
        for (int j = i + 1; j < juego_actual.cantJugadores; j++) {
            //Si el elemento en el índice j tiene mayor puntaje que el elemento en el índice i los intercambiamos
            if (juego_actual.jugadores[j].puntaje_acumulado > juego_actual.jugadores[i].puntaje_acumulado) {
                aux = juego_actual.jugadores[i]; //El elemento en el índice i lo guardo en aux para no perderlo
                juego_actual.jugadores[i] = juego_actual.jugadores[j]; //El elemento en el índice i ahora tiene el contenido del elemento en el índice j
                juego_actual.jugadores[j] = aux; //El elemento en el índice j ahora tiene el contenido de aux, es decir el contenido inicial de i

            }    
        }
    }
}

//Función que se utliza para chequear que los campos dentro de los formularios no estén vacíos
bool verificarCampoVacio(string s) {

    if (s.length() == 0) {
        
        cout << "Este campo no puede permanecer vacío." << endl;
        cout << "Por favor, ingresa una entrada válida." << endl;
        esperar();
        return true; //El campo está vacío

    }

    return false; //El campo no está vacío, entonces continúa

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
    system("clear");
}

//Función para pedir el alias del jugador
bool pedirAlias (Sistema &juego_actual, Jugador &j) {
    
    //Definimos una variable para verificar que el alias esté disponible, asumimos que es false
    bool aliasDisponible = false; 

    do {

        system("clear");
        cout << "Ingresa tu alias: ";
        getline(cin, j.alias);
        verificarCampoVacio(j.alias);

        int posicionAlias = buscarJugador(juego_actual, j.alias);

        //Verificamos si el alias está disponible
        if (posicionAlias == -1) {
            aliasDisponible = true;
        } else {
            /*El alias ya existe pero está inactivo, así que devolvemos 
            los datos del jugador para que no pida nuevamente los datos*/
            if (juego_actual.jugadores[posicionAlias].estado == false) {
                juego_actual.jugadores[posicionAlias].estado = true; //Seteamos el estado a true (activo)
                cout << "¡Bienvenid@ devuelta" << juego_actual.jugadores[posicionAlias].alias << "!" << endl;
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

        if(!verificarCampoVacio(anioTexto)) { //El campo no está vacío
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

        if(!verificarCampoVacio(mesTexto)) { //Verificamos que no está vacío
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

        if(!verificarCampoVacio(diaTexto)) { //Verificamos que el campo no esté vacío
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
    if (anio >= 2000 && anio < 2026) { //Habría que verificar bien esta restricción
        return true;
    } else {
        return false;
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