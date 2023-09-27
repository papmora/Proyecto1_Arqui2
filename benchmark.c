#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/time.h>
#include <sys/sysinfo.h>

// System Information
int systemInformation(const char *code) {
    // Guardar info en txt
    system("lscpu > lscpu_output.txt");

    // Obtencion del size de una Linea de cache de L1 - Datos
    long l1_cache_line_size = sysconf(_SC_LEVEL1_DCACHE_LINESIZE);
    // Obtencion del size de una Instancia de cache de L1 - Datos
    long l1_cache_size = sysconf(_SC_LEVEL1_DCACHE_SIZE);

    // Validacion de L1 - Datos
    if (l1_cache_line_size == -1 || l1_cache_size == -1) {
        perror("Error al obtener el tamaños de cache");
        return 1;
    }

    // Obtencion del size de una Linea de cache de L1 - Datos
    long l2_cache_line_size = sysconf(_SC_LEVEL2_CACHE_LINESIZE);
    // Obtencion del size de una Instancia de cache de L1 - Datos
    long l2_cache_size = sysconf(_SC_LEVEL2_CACHE_SIZE);

    // Validacion de L1 - Datos
    if (l2_cache_line_size == -1 || l2_cache_size == -1) {
        perror("Error al obtener el tamaños de cache");
        return 1;
    }

    // Obtencion del size de una Linea de cache de L1 - Datos
    long l3_cache_line_size = sysconf(_SC_LEVEL3_CACHE_LINESIZE);
    // Obtencion del size de una Instancia de cache de L1 - Datos
    long l3_cache_size = sysconf(_SC_LEVEL3_CACHE_SIZE);

    // Validacion de L1 - Datos
    if (l3_cache_line_size == -1 || l3_cache_size == -1) {
        perror("Error al obtener el tamaños de cache");
        return 1;
    }

    // Pagina de RAM
    long page_size = sysconf(_SC_PAGESIZE);
    // Cantidad total de RAM en bytes:
    long num_phys_pages = sysconf(_SC_PHYS_PAGES);
    // Conversion:
    long long total_memory = (long long)page_size * num_phys_pages;
    // RAM en Gigas:
    double total_memory_gb = (double)total_memory / (1024 * 1024 * 1024);

    // Obtencion precisa de CPU y Threads
    long num_cores = sysconf(_SC_NPROCESSORS_ONLN);
    long num_threads = sysconf(_SC_NPROCESSORS_CONF);

    printf("----- Especificaciones del sistema para %s -----\n", code);
    printf("Número de núcleos(Number of Cores): %ld\n", num_cores);
    printf("Número de hilos(Number of Threads): %ld\n", num_threads);
    system("lscpu | grep 'Thread(s) per core'");
    system("lscpu | grep 'Core(s) per socket'");
    printf("Cantidad total de memoria RAM: %.2f GB\n", total_memory_gb);
    // Cache L1:
    printf("Cache L1:\n");
    printf("Tamaño de línea: %ld bytes\n", l1_cache_line_size);
    printf("Tamaño de Instancia: %ld bytes\n", l1_cache_size);
    system("lscpu | grep 'L1d cache'");
    // Cache L2:
    printf("Cache L2:\n");
    printf("Tamaño de línea: %ld bytes\n", l2_cache_line_size);
    printf("Tamaño de Instancia: %ld bytes\n", l2_cache_size);
    system("lscpu | grep 'L2 cache'");
    // Cache L3:
    printf("Cache L3:\n");
    printf("Tamaño de línea: %ld bytes\n", l3_cache_line_size);
    printf("Tamaño de Instancia: %ld bytes\n", l3_cache_size);
    system("lscpu | grep 'L3 cache'");

    printf("Procesador:\n");
    system("lscpu | grep 'Model name'");

    return 0;
}

// Execution
void benchmark(const char *bench, int times) {

    // UwU
    systemInformation(bench);

    char perf_report_command[100];
    char perf_variations_command[100];

    for (int i = 1; i <= times; i++) {
        sprintf(perf_report_command, "perf record -e cycles,cache-misses,cache-references,context-switches -o %s_%d.data ./%s", bench, i, bench);
        system(perf_report_command);
    }
    sprintf(perf_variations_command, "perf stat -e cycles,cache-misses,cache-references,context-switches -r %d ./%s", times, bench);
    system(perf_variations_command);
}

int main(int argc, char *argv[]) {


    if (argc != 4) {
        printf("Usage: %s <benchmark> <times> <cpus>\n", argv[0]);
        printf("Benchmark options: bench1_c or bench2_cpp\n");
        return 1;
    }

    // Selected Benchmark:
    const char *bench = argv[1];
    // Repetitions:
    int times = atoi(argv[2]);
    // Number of CPUs:
    // int cpus = atoi(argv[3]);

    // Conditional:
    if (strcmp(bench, "bench1_c") == 0) {
        benchmark("bench1_c", times);

    } else if (strcmp(bench, "bench2_cpp") == 0) {
        benchmark("bench2_cpp", times);

    } else {
        printf("Invalid option: %s\n", bench);
        return 1;
    }

    printf("Benchmarks completed\n");
    return 0;
}