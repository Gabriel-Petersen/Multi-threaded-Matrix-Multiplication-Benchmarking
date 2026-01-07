Multi-threaded Matrix Multiplication Benchmarking

Este projeto explora diferentes técnicas de otimização de performance em CPU para a multiplicação de matrizes quadradas (C = A x B),
um dos pilares da computação de alto desempenho (HPC). O objetivo é demonstrar como o layout de memória, a hierarquia de cache e o paralelismo multi-core impactam drasticamente o tempo de execução.

Resultados (N=7500).
O benchmark abaixo reflete os tempos obtidos em uma matriz de 7500 x 7500 (ponto flutuante de precisão dupla), comparando a implementação ingênua com métodos otimizados.
Método             Tempo (ms)    Speedup                       Observação
Trivial             2.368.997     1.0x      Algoritmo base O(n³) com Cache Misses constantes.
Transposed Trivial   300.015      7.9x      Melhora a localidade espacial ao ler B sequencialmente.
Blocking (Tiling)    109.107      21.7x     Maximiza a localidade temporal mantendo blocos no Cache L1/L2.
Parallel             71.064       33.3x     Divisão de carga entre múltiplos núcleos via std::thread.
Transposed Parallel  68.489       34.5x     O estado da arte: Cache-friendly + Multi-core.

Implementações
1. Trivial (Baseline)
    A implementação clássica com três loops aninhados. O principal gargalo aqui é o acesso por colunas na Matriz B, que causa falhas de cache (Cache Misses) constantes, forçando a CPU a buscar dados na RAM lenta.
2. Transposição (Spatial Locality)
    Ao transpor a Matriz B antes do cálculo, transformamos acessos por coluna em acessos por linha. Isso permite que o Hardware Prefetcher da CPU antecipe os dados, carregando linhas inteiras para o cache antes de serem necessárias.
3. Blocking / Tiling (Temporal Locality)
    Divide as matrizes em blocos (Tiles) de 64 x 64. Esta técnica garante que os dados carregados no cache sejam reutilizados ao máximo antes de serem descartados, reduzindo drasticamente o tráfego no barramento de memória.
4. Multithreading
    Utiliza std::thread::hardware_concurrency() para identificar os núcleos disponíveis e dividir as linhas da matriz entre eles. A implementação foca em evitar a criação excessiva de threads dentro dos loops para reduzir o overhead do sistema operacional.

Análise de Performance
Os testes mostram um "ponto de cruzamento" (Crossover Point). Em matrizes pequenas, o custo de transpor a matriz ou criar threads pode não compensar. No entanto, conforme $N$ cresce e ultrapassa a capacidade do Cache L3, a eficiência da Transposição + Paralelismo torna-se a única forma de manter o escalonamento de performance.

Como Executar

Pré-requisitos:
Compilador G++ (suporte a C++20) ou MSVC.
Python 3 (para geração de gráficos).

Compilação (Linux/GCC):
g++ -O3 -Iinclude src/big_matrix.cpp src/main.cpp -o matrix_bench -lpthread
Nota: A flag -O3 é obrigatória para habilitar as otimizações de vetorização do compilador.

Gerando Gráficos:
python benchmarks/plot_results.py

Lições Aprendidas:
1. A diferença entre o código em modo Debug e Release (-O3) pode chegar a 50x.
2. O gargalo da computação moderna raramente é o processamento bruto, mas sim a velocidade de entrega de dados da RAM para a CPU (o Memory Wall).
3. Algoritmos conscientes de cache (Cache-aware) superam algoritmos puramente matemáticos.