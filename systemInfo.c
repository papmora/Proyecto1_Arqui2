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

    // Obtencion del size de una Linea de cache de L2 - Datos
    long l2_cache_line_size = sysconf(_SC_LEVEL2_CACHE_LINESIZE);
    // Obtencion del size de una Instancia de cache de L1 - Datos
    long l2_cache_size = sysconf(_SC_LEVEL2_CACHE_SIZE);

    // Validacion de L1 - Datos
    if (l2_cache_line_size == -1 || l2_cache_size == -1) {
        perror("Error al obtener el tamaños de cache");
        return 1;
    }

    // Obtencion del size de una Linea de cache de L3 - Datos
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
    // long num_threads = sysconf(_SC_NPROCESSORS_CONF);

    printf("----- Especificaciones del sistema para %s -----\n\n", code);
    printf("Number of Cores: %21ld\n", num_cores);
    // printf("Number of Threads: %19ld\n", num_threads);
    system("lscpu | grep 'Thread(s) per core'");
    system("lscpu | grep 'Core(s) per socket'");
    printf("RAM Memory: %29.2f GB\n", total_memory_gb);
    // Cache L1:
    printf("Cache L1:\n");
    printf("Line size: %ld bytes\n", l1_cache_line_size);
    printf("Instance size: %ld bytes\n", l1_cache_size);
    system("lscpu | grep 'L1d cache'");
    // Cache L2:
    printf("Cache L2:\n");
    printf("Line size: %ld bytes\n", l2_cache_line_size);
    printf("Instance size: %ld bytes\n", l2_cache_size);
    system("lscpu | grep 'L2 cache'");
    // Cache L3:
    printf("Cache L3:\n");
    printf("Tamaño de línea: %ld bytes\n", l3_cache_line_size);
    printf("Instance size: %ld bytes\n", l3_cache_size);
    system("lscpu | grep 'L3 cache'");

    printf("Processor:\n");
    system("lscpu | grep 'Model name'");

    return 0;
}

int main(int argc, char *argv[]) {

    if (argc != 2) {

        printf("Usage: %s <systemInfo> <benchName>\n", argv[0]);
        return 1;
    }

    const char *bench = argv[1];
    printf("This: %s\n", bench);

    systemInformation(bench);
}