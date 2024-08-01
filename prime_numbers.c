#include <stdio.h>
#include <sys/time.h>

struct timeval start_timer, end_timer;


// Función para verificar si un número es primo
int es_primo(int num) {
    if (num < 2) {
        return 0;
    }
    for (int i = 2; i * i <= num; i++) {
        if (num % i == 0) {
            return 0;
        }
    }
    return 1;
}

// Función para contar la cantidad de números primos en un rango
int contar_primos(int inicio, int fin) {
    int count_n = 0;
    for (int num = inicio; num <= fin; num++) {
        if (es_primo(num)) {
            count_n++;
        }
    }
    return count_n;
}

int main() {
    int inicio, fin;

    printf("Rango (from): ");
    scanf("%d", &inicio);
    printf("Rango (to]: ");
    scanf("%d", &fin);

    gettimeofday(&start_timer, NULL); // Capturar tiempo de inicio

    printf("Números primos rango [%d, %d]: %d\n", inicio, fin, contar_primos(inicio,fin));

    gettimeofday(&end_timer, NULL); // Capturar tiempo de fin

    double tiempo_inicio = start_timer.tv_sec + start_timer.tv_usec / 1000000.0;
    double tiempo_fin = end_timer.tv_sec + end_timer.tv_usec / 1000000.0;
    double tiempo_total = tiempo_fin - tiempo_inicio;

    printf("Tiempo de ejecución: %.6f segundos\n", tiempo_total);

    return 0;
}
