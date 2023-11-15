## Como Executar

Esse programa compreende a política de Escalonamento do seu sistema operacional.

Para executá-lo basta compilar:

```
    gcc sched_profiler.c -O2 -o sched_profiler -lpthread
```

E executar com o taskset, utilizando apenas um núcleo:

```
    taskset -c 0 ./sched_profiler <número de threads> <tamanho do buffer>
```

## Exemplo

```
    taskset -c 0 ./sched_profiler 4 10000000
```

A saída está no arquivo 'example.txt'.

## Autores

Gabriel Tabajara, Giovani Schenato, Lucas Dellatorre.