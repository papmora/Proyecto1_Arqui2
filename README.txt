Ejemplos de false sharing (falseS_ST y falseS_MT):

En estos ejemplos buscamos mostrar el concepto de false sharing haciendo uso del algoritmo de suma en 
paralelo que nos permite mostrar como este concepto se puede ver sin forzar nada, y que se encuentra presente
en los algoritmos de forma inadvertida debido a la competencia por las líneas de caché al modificar elementos 
cercanos en un arreglo compartido.

En este algoritmo hacemos uso de un arreglo compartido que almacena elementos de tipo ArrayElement. 
Cada hilo suma los valores de los elementos del arreglo en su propia sección (un subconjunto del arreglo) 
durante la ejecución del algoritmo de suma en paralelo.

Por lo que el false sharing ocurre de manera inadvertida debido a que varios hilos están accediendo a elementos
cercanos en el arreglo, lo que compite por las mismas líneas de caché. Como resultado, la interferencia en la 
caché puede ralentizar el rendimiento del programa, incluso cuando los elementos no se superponen en términos
de datos compartidos.

La versión multi-threading divide el 
trabajo entre varios hilos y acumula los resultados locales para calcular la suma total.

Bench1:

Este benchmark en C crea un arreglo compartido sharedArray con elementos alineados en líneas de caché para provocar 
false sharing. Luego, se ejecuta el programa en dos configuraciones:

Versión Single-Thread: El programa realiza operaciones de escritura en el arreglo en un solo hilo.

Versión Multi-Thread: El programa crea varios hilos que realizan operaciones de escritura en el arreglo simultáneamente.

Bench2:

Este benchmark, inicializa un arreglo compartido sharedArray con elementos iguales a 1 y luego se realizan 
operaciones de suma concurrentes en este arreglo. Se mide el tiempo de ejecución tanto en la versión single-thread 
como en la versión multi-thread y se compara el rendimiento. El false sharing se provoca al acceder y sumar 
elementos cercanos en el arreglo compartido.