# Ambiente de execução

Dados da máquina em que os experimentos deste trabalho foram rodados.

- **Processador:** Apple M1
- **Memória RAM:** 8 GB
- **Sistema operacional:** macOS 26.3 (build 25D125)
- **Compilador:** `g++` / Apple clang version 17.0.0 (clang-1700.0.13.5)
- **Flags:** `-std=c++17 -O3 -DNDEBUG`

Comando de build (a partir da raiz do projeto):

```bash
make clean && make
```

Comando de benchmark completo (gera CSV em `results/`):

```bash
./bin/benchmark --all
```
