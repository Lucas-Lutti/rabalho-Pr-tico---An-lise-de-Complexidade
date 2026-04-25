#!/usr/bin/env bash
# Gera artigo/pdf/main.pdf a partir de main.tex (template SBC).
#
# O modelo SBC usa \usepackage{times} em sbc-template.sty — isso corresponde a Times
# (clone URW) com pdfLaTeX. O Tectonic usa XeTeX e tende a Latin Modern; para fidelidade
# ao template, prefira pdfLaTeX (MacTeX/TeX Live no PATH).
set -euo pipefail
ROOT="$(cd "$(dirname "$0")" && pwd)"
OUT="$ROOT/pdf"
mkdir -p "$OUT"
cd "$ROOT"

# Caminhos comuns no macOS / TeX Live
export PATH="/Library/TeX/texbin:/usr/local/texlive/2025/bin/universal-darwin:/usr/local/texlive/2024/bin/universal-darwin:/usr/local/texlive/2023/bin/universal-darwin:$PATH"

run_pdflatex() {
  pdflatex -interaction=nonstopmode -halt-on-error -file-line-error -output-directory="$OUT" main.tex
}

run_bibtex() {
  BIBINPUTS="${ROOT}:${BIBINPUTS:-}"
  BSTINPUTS="${ROOT}:${BSTINPUTS:-}"
  export BIBINPUTS BSTINPUTS
  ( cd "$ROOT" && bibtex "pdf/main" )
}

if command -v pdflatex >/dev/null 2>&1; then
  run_pdflatex
  run_bibtex
  run_pdflatex
  run_pdflatex
elif command -v tectonic >/dev/null 2>&1 || [[ -n "${TECTONIC:-}" && -x "${TECTONIC}" ]] || [[ -x "$ROOT/../.tools/tectonic" ]]; then
  TECTONIC_BIN="tectonic"
  command -v tectonic >/dev/null 2>&1 || TECTONIC_BIN="${TECTONIC:-$ROOT/../.tools/tectonic}"
  echo "AVISO: pdfLaTeX não encontrado. Usando Tectonic (XeTeX): o sbc-template usa Times New Roman/Arial do sistema quando disponíveis; em Linux, instale fontes equivalentes ou use pdfLaTeX." >&2
  "$TECTONIC_BIN" --keep-intermediates --outdir "$OUT" main.tex
else
  echo "Instale MacTeX, TeX Live ou BasicTeX para obter pdflatex (recomendado para o template SBC)." >&2
  exit 1
fi

echo "PDF: $OUT/main.pdf"
