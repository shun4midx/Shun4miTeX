#!/usr/bin/env bash
set -euo pipefail

INPUT="${1:-eq.tex}"
BASENAME="${INPUT%.tex}"

pdflatex -interaction=nonstopmode -halt-on-error "$INPUT"

pdftocairo \
  -png \
  -singlefile \
  -r 900 \
  "${BASENAME}.pdf" \
  "${BASENAME}"

convert "${BASENAME}.png" \
  -fuzz 2% -trim +repage \
  -shave 1x1 \
  -bordercolor black -border 20 \
  out.png