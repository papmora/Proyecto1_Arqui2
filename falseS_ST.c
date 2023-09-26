#include <stdio.h>

#define ARRAY_SIZE 1000000

// Definir una estructura simple para representar un elemento en el arreglo
struct ArrayElement {
    int value;
};

// Arreglo compartido
struct ArrayElement sharedArray[ARRAY_SIZE];

// Función que realiza la suma
int sumInSingleThread() {
    int totalSum = 0;

    for (int i = 0; i < ARRAY_SIZE; i++) {
        // Realizar una operación de suma en el elemento del arreglo
        totalSum += sharedArray[i].value;
    }

    return totalSum;
}

int main() {
    // Inicializar el arreglo compartido con valores
    for (int i = 0; i < ARRAY_SIZE; i++) {
        sharedArray[i].value = 1;  // Inicializar todos los elementos con el mismo valor
    }

    // Realizar la suma en single threading
    int result = sumInSingleThread();

    // Imprimir el resultado
    printf("Single Threading Sum Result: %d\n", result);

    return 0;
}
