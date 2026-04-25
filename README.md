# Trabalho Prático de Análise de Complexidade

Este repositório reúne a implementação do trabalho prático de FPAA sobre estruturas de conjuntos disjuntos (\emph{Union-Find} / DSU).

O objetivo do projeto é comparar, na prática, o comportamento de três variantes:

- uma versão ingênua
- uma versão com união por rank
- uma versão com união por rank e compressão de caminho

Além do tempo de execução, o projeto também mede acessos instrumentados aos vetores internos da estrutura para ajudar na análise experimental.

## Estrutura do projeto

- `include/`: cabeçalhos das estruturas e utilitários
- `src/`: implementações e programa principal dos experimentos
- `scripts/`: script para converter os resultados em dados usados no artigo
- `results/`: saída do benchmark em CSV
- `docs/AMBIENTE.md`: informações da máquina e da compilação usadas nos experimentos

## Como compilar

Na raiz do projeto:

```bash
make
```

Isso gera o executável:

```bash
./bin/benchmark
```

## Como rodar os experimentos

Para executar o conjunto principal de testes e gerar o CSV:

```bash
./bin/benchmark --all
```

O arquivo gerado será:

```bash
results/benchmark.csv
```

Se quiser rodar um cenário isolado:

```bash
./bin/benchmark --single 10000 200000 42 0.25
```

## Dados para o artigo

Depois de gerar o CSV, é possível converter os dados para o formato usado no artigo com:

```bash
python3 scripts/export_latex_plot_data.py
```

Esse script atualiza os arquivos tabulares usados na versão local do artigo.

## Ambiente usado

Os experimentos deste projeto foram executados no seguinte ambiente:

- Apple M1
- 8 GB de RAM
- macOS 26.3
- Apple clang 17.0.0
- flags: `-std=c++17 -O3 -DNDEBUG`

Os detalhes completos estão em `docs/AMBIENTE.md`.

## Observação

O foco deste repositório é o código e os dados do experimento. A pasta do artigo foi mantida localmente, fora do versionamento principal, para deixar o repositório mais enxuto.
