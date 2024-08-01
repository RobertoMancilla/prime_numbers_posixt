#include <stdio.h>
#include <sys/time.h>
#include <pthread.h>

#define MAX_THREADS 6 //cantidad de threads 

struct timeval start_timer, end_timer;

// struct para pasar los argumentos del hilo
struct thread_args
{
    int inicio;
    int fin;
    int contador;
};

// si el numero es primo o no (true 1 - false 0)
int is_prime(int num)
{
    if (num < 2)
    {
        return 0;
    }
    for (int i = 2; i * i <= num; i++)
    {
        if (num % i == 0)
        {
            return 0;
        }
    }
    return 1;
}

// esta funcion se ejecuta en cada hilo (entra en el for que recorre MAX_THREADS)
/* esta funcion debe de ser una funcion de apuntador porque el parammetro que recibe
la funcion de crear hilo es un apuntador*/
void *count_prime_n_by_subrange(void *args)
{
    struct thread_args *targs = (struct thread_args *)args;
    targs->contador = 0; // inicializamos en 0 ya que es nuestro contador de cada hilo
    int n;
    for (n = targs->inicio; n <= targs->fin; n++)
    {
        if (is_prime(n))
        {
            targs->contador++;
        }
    }
    //salimos del thread
    pthread_exit(NULL);
}
// int pthread_create(pthread_t *thread, const pthread_attr_t *attr,void *(*start_routine) (void *), void *arg);

// Función para crear y ejecutar los hilos
int count_prime_n_full_thread(int inicio, int fin)
{
    pthread_t threads[MAX_THREADS];                      // arreglo con el no., de hilo (de tipo pthread_t obvio)
    struct thread_args pointer_thread_args[MAX_THREADS]; /* Nuestro apuntador a los argumentos de nustro struct
                                             este contiene los args(inicio, fin, count) que se le pasan a cada thread*/
    int count_primos = 0;
    int subrango = (fin - inicio + 1) / MAX_THREADS; // divir la cantidad de numero en subrangos para cada hilo
    /*Con esta variable usando modulo vemos los elementos adicionales que deben de ser asiganados (ejem: 10%3=1)
    //                                                                           elem%max_threds=threds_que_sobran
    de esta manera lo THREADS SE REPARTEN "EQUITATIVAMENTE" la carga */
    int carga_thread = (fin - inicio + 1) % MAX_THREADS;
    // Obtendremos el inicio de cada subrango en el bucle. ()
    int i_inicio_subrango = inicio;
    int i_fin_subrango;

    // Crear y ejecutar los hilos
    for (int n_thread = 0; n_thread < MAX_THREADS; n_thread++)
    {
        // n_thread
        pointer_thread_args[n_thread].inicio = i_inicio_subrango; // asignamos a cada hilo el inicio del su subrango
        // calculamos el ultimo elemento de nuestro subrango, y si carga_threead es > a 0 se le suma 1.
        pointer_thread_args[n_thread].fin = i_inicio_subrango + subrango - 1 + (carga_thread > 0 ? 1 : 0);
        // Actualizamos el índice de inicio para el próximo hilo (+1).
        i_inicio_subrango = pointer_thread_args[n_thread].fin + 1;

        // reducimos el numero de elementos adicionales que deben ser asignados para cada subrango
        carga_thread--;

        //  apuntador de nuestro array de hilos[MAX_THREADS], null, funcion, apuntador void de la funcion
        //  Se pasa la dirección de la estructura thread_args del hilo actual como argumento.
        pthread_create(&threads[n_thread], NULL, count_prime_n_by_subrange, (void *)&pointer_thread_args[n_thread]);
        // int pthread_create(pthread_t *thread, const pthread_attr_t *attr,void *(*start_routine) (void *), void *arg);
    }

    // Esperar a que los hilos terminen y sumar los resultados
    for (int i = 0; i < MAX_THREADS; i++)
    {
        // espera al hilo indicado por threads[i] termine su ejecucion y se une al main thread
        pthread_join(threads[i], NULL); //NULL, no retorna nada
        count_primos += pointer_thread_args[i].contador; //se va sumando al count la suma de cada count de cada thread
    }
    return count_primos;
}

int main()
{
    int inicio, fin;

    printf("Rango (from): ");
    scanf("%d", &inicio);
    printf("Rango (to]: ");
    scanf("%d", &fin);

    gettimeofday(&start_timer, NULL); // Capturar tiempo de inicio

    int cantidad_primos = count_prime_n_full_thread(inicio, fin);

    gettimeofday(&end_timer, NULL); // Capturar tiempo de fin

    double tiempo_inicio = start_timer.tv_sec + start_timer.tv_usec / 1000000.0;
    double tiempo_fin = end_timer.tv_sec + end_timer.tv_usec / 1000000.0;
    double tiempo_total = tiempo_fin - tiempo_inicio;

    printf("Números primos rango [%d, %d]: %d\n", inicio, fin, cantidad_primos);
    printf("Tiempo de ejecución: %.4f segundos\n", tiempo_total);

    return 0;
}
