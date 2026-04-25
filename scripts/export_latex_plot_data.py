#!/usr/bin/env python3
"""
Lê results/benchmark.csv e gera arquivos em artigo/data/ para pgfplots e tabela.
Execute após `bin/benchmark --all` quando quiser atualizar gráficos/tabela do artigo.
"""

from __future__ import annotations

import argparse
import csv
import os
import sys
from collections import defaultdict


def load_rows(path: str) -> list[dict[str, str]]:
    with open(path, newline="", encoding="utf-8") as f:
        return list(csv.DictReader(f))


def main() -> None:
    root = os.path.abspath(os.path.join(os.path.dirname(__file__), ".."))
    p = argparse.ArgumentParser()
    p.add_argument(
        "--csv",
        default=os.path.join(root, "results", "benchmark.csv"),
    )
    p.add_argument(
        "--out-dir",
        default=os.path.join(root, "artigo", "data"),
    )
    args = p.parse_args()
    if not os.path.isfile(args.csv):
        print(f"CSV não encontrado: {args.csv}", file=sys.stderr)
        sys.exit(1)
    os.makedirs(args.out_dir, exist_ok=True)

    by_n: dict[int, dict[str, dict[str, str]]] = defaultdict(dict)
    for r in load_rows(args.csv):
        n = int(r["n"])
        by_n[n][r["variant"]] = r

    ns = sorted(by_n.keys())
    lines_t = ["n\tnaive_ms\tunion_rank_ms\ttarjan_ms"]
    lines_a = ["n\tnaive_acc\tunion_rank_acc\ttarjan_acc"]
    for n in ns:
        g = by_n[n]
        for v in ("naive", "union_rank", "tarjan"):
            if v not in g:
                print(f"Faltando variante {v} para n={n}", file=sys.stderr)
                sys.exit(1)
        t0 = float(g["naive"]["time_ns"]) / 1e6
        t1 = float(g["union_rank"]["time_ns"]) / 1e6
        t2 = float(g["tarjan"]["time_ns"]) / 1e6
        a0 = int(g["naive"]["total_accesses"])
        a1 = int(g["union_rank"]["total_accesses"])
        a2 = int(g["tarjan"]["total_accesses"])
        lines_t.append(f"{n}\t{t0:.6f}\t{t1:.6f}\t{t2:.6f}")
        lines_a.append(f"{n}\t{a0}\t{a1}\t{a2}")

    path_t = os.path.join(args.out_dir, "tempo_por_n.dat")
    path_a = os.path.join(args.out_dir, "acessos_por_n.dat")
    with open(path_t, "w", encoding="utf-8") as f:
        f.write("\n".join(lines_t) + "\n")
    with open(path_a, "w", encoding="utf-8") as f:
        f.write("\n".join(lines_a) + "\n")

    def fmt_acc(a: int) -> str:
        return f"{a:,}".replace(",", "\\,")

    def rows_for_n(n: int) -> list[str]:
        g = by_n[n]
        m = int(g["naive"]["m"])
        rows = []
        labels = [
            ("naive", "Ingênua"),
            ("union_rank", "União por rank"),
            ("tarjan", "Rank + compressão"),
        ]
        for v, lab in labels:
            t_s = float(g[v]["time_ns"]) / 1e9
            acc = int(g[v]["total_accesses"])
            rows.append(
                f"{n} & {m} & {lab} & {t_s:.6f} & {fmt_acc(acc)} \\\\"
            )
        return rows

    n_min, n_max = ns[0], ns[-1]
    table_path = os.path.join(args.out_dir, "tabela_resumo.inc.tex")
    with open(table_path, "w", encoding="utf-8") as f:
        for line in rows_for_n(n_min):
            f.write(line + "\n")
        for line in rows_for_n(n_max):
            f.write(line + "\n")
        f.write("\\hline\n")

    print(f"Gravado: {path_t}\nGravado: {path_a}\nGravado: {table_path}")


if __name__ == "__main__":
    main()
