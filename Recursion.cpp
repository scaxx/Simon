#include <iostream>
#include <unistd.h>
using namespace std;
// ej 1 imprime una palabra al reves
void imprimir (char arr[],int cant){
if( cant == 1){
    cout << arr[cant-1];
}
if ( cant > 1){
   cout << arr[cant -1];
   imprimir (arr, cant-1);
}
}

//Imprime números del 1 al N(limite que le pongo yo) ej 3
void imprimirHastaN (int n){
    if (n == 1){
        cout << n << endl;
    }
    imprimirHastaN (n - 1);
    cout << n << " ";
}

int sumarArr(int arr[], int cant){
    if (cant == 0){
        return 0;
    }
    return sumarArr(arr, cant -1) + arr[cant-1];

}


int main(){
    //ej 1
   /* int cant = 4;
    char arr[cant] = {'h','o','l','a'};
    imprimir (arr, cant);
    // ej 3
    int limite = 5;
    cout << "Imprimiendo hasta " << limite << ":" << endl;
    imprimirHastaN(limite); // Esto va a imprimir: 1 2 3 4 5
    cout << endl;
    return 0;
*/
int arr[5] = {1,2,3,4,5};
int resSuma = sumarArr (arr, 5);
cout << "La suma del arreglo es: " << resSuma;
}



