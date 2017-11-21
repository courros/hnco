#!/usr/bin/gnuplot -persist
set grid
set xlabel "Number of evaluations"
set ylabel "Proportion of targets"
set key outside top center box opaque horizontal
set format x "10^{%L}"
set logscale x
set yrange [0:1]
set autoscale xfix

set offsets graph 0.05, graph 0.05, graph 0.05, graph 0.05
set term epscairo color enhanced
set output "graphics/one-max.eps"
set key title "one-max"
plot \
  "results/one-max/rls/ecdf.txt" using 1:2 with lines lw 2 title "rls", \
  "results/one-max/hc/ecdf.txt" using 1:2 with lines lw 2 title "hc", \
  "results/one-max/sa/ecdf.txt" using 1:2 with lines lw 2 title "sa", \
  "results/one-max/ea-1p1/ecdf.txt" using 1:2 with lines lw 2 title "ea-1p1", \
  "results/one-max/ea-1p10/ecdf.txt" using 1:2 with lines lw 2 title "ea-1p10", \
  "results/one-max/ea-10p1/ecdf.txt" using 1:2 with lines lw 2 title "ea-10p1", \
  "results/one-max/ea-1c10/ecdf.txt" using 1:2 with lines lw 2 title "ea-1c10", \
  "results/one-max/ga/ecdf.txt" using 1:2 with lines lw 2 title "ga", \
  "results/one-max/pbil/ecdf.txt" using 1:2 with lines lw 2 title "pbil", \
  "results/one-max/umda/ecdf.txt" using 1:2 with lines lw 2 title "umda"
set term pdfcairo color enhanced
set output "graphics/one-max.pdf"
replot
set term png enhanced
set output "graphics/one-max.png"
replot

set term epscairo color enhanced
set output "graphics/lin.eps"
set key title "lin"
plot \
  "results/lin/rls/ecdf.txt" using 1:2 with lines lw 2 title "rls", \
  "results/lin/hc/ecdf.txt" using 1:2 with lines lw 2 title "hc", \
  "results/lin/sa/ecdf.txt" using 1:2 with lines lw 2 title "sa", \
  "results/lin/ea-1p1/ecdf.txt" using 1:2 with lines lw 2 title "ea-1p1", \
  "results/lin/ea-1p10/ecdf.txt" using 1:2 with lines lw 2 title "ea-1p10", \
  "results/lin/ea-10p1/ecdf.txt" using 1:2 with lines lw 2 title "ea-10p1", \
  "results/lin/ea-1c10/ecdf.txt" using 1:2 with lines lw 2 title "ea-1c10", \
  "results/lin/ga/ecdf.txt" using 1:2 with lines lw 2 title "ga", \
  "results/lin/pbil/ecdf.txt" using 1:2 with lines lw 2 title "pbil", \
  "results/lin/umda/ecdf.txt" using 1:2 with lines lw 2 title "umda"
set term pdfcairo color enhanced
set output "graphics/lin.pdf"
replot
set term png enhanced
set output "graphics/lin.png"
replot

set term epscairo color enhanced
set output "graphics/leading-ones.eps"
set key title "leading-ones"
plot \
  "results/leading-ones/rls/ecdf.txt" using 1:2 with lines lw 2 title "rls", \
  "results/leading-ones/hc/ecdf.txt" using 1:2 with lines lw 2 title "hc", \
  "results/leading-ones/sa/ecdf.txt" using 1:2 with lines lw 2 title "sa", \
  "results/leading-ones/ea-1p1/ecdf.txt" using 1:2 with lines lw 2 title "ea-1p1", \
  "results/leading-ones/ea-1p10/ecdf.txt" using 1:2 with lines lw 2 title "ea-1p10", \
  "results/leading-ones/ea-10p1/ecdf.txt" using 1:2 with lines lw 2 title "ea-10p1", \
  "results/leading-ones/ea-1c10/ecdf.txt" using 1:2 with lines lw 2 title "ea-1c10", \
  "results/leading-ones/ga/ecdf.txt" using 1:2 with lines lw 2 title "ga", \
  "results/leading-ones/pbil/ecdf.txt" using 1:2 with lines lw 2 title "pbil", \
  "results/leading-ones/umda/ecdf.txt" using 1:2 with lines lw 2 title "umda"
set term pdfcairo color enhanced
set output "graphics/leading-ones.pdf"
replot
set term png enhanced
set output "graphics/leading-ones.png"
replot

set term epscairo color enhanced
set output "graphics/ridge.eps"
set key title "ridge"
plot \
  "results/ridge/rls/ecdf.txt" using 1:2 with lines lw 2 title "rls", \
  "results/ridge/hc/ecdf.txt" using 1:2 with lines lw 2 title "hc", \
  "results/ridge/sa/ecdf.txt" using 1:2 with lines lw 2 title "sa", \
  "results/ridge/ea-1p1/ecdf.txt" using 1:2 with lines lw 2 title "ea-1p1", \
  "results/ridge/ea-1p10/ecdf.txt" using 1:2 with lines lw 2 title "ea-1p10", \
  "results/ridge/ea-10p1/ecdf.txt" using 1:2 with lines lw 2 title "ea-10p1", \
  "results/ridge/ea-1c10/ecdf.txt" using 1:2 with lines lw 2 title "ea-1c10", \
  "results/ridge/ga/ecdf.txt" using 1:2 with lines lw 2 title "ga", \
  "results/ridge/pbil/ecdf.txt" using 1:2 with lines lw 2 title "pbil", \
  "results/ridge/umda/ecdf.txt" using 1:2 with lines lw 2 title "umda"
set term pdfcairo color enhanced
set output "graphics/ridge.pdf"
replot
set term png enhanced
set output "graphics/ridge.png"
replot

set term epscairo color enhanced
set output "graphics/jmp-5.eps"
set key title "jmp-5"
plot \
  "results/jmp-5/rls/ecdf.txt" using 1:2 with lines lw 2 title "rls", \
  "results/jmp-5/hc/ecdf.txt" using 1:2 with lines lw 2 title "hc", \
  "results/jmp-5/sa/ecdf.txt" using 1:2 with lines lw 2 title "sa", \
  "results/jmp-5/ea-1p1/ecdf.txt" using 1:2 with lines lw 2 title "ea-1p1", \
  "results/jmp-5/ea-1p10/ecdf.txt" using 1:2 with lines lw 2 title "ea-1p10", \
  "results/jmp-5/ea-10p1/ecdf.txt" using 1:2 with lines lw 2 title "ea-10p1", \
  "results/jmp-5/ea-1c10/ecdf.txt" using 1:2 with lines lw 2 title "ea-1c10", \
  "results/jmp-5/ga/ecdf.txt" using 1:2 with lines lw 2 title "ga", \
  "results/jmp-5/pbil/ecdf.txt" using 1:2 with lines lw 2 title "pbil", \
  "results/jmp-5/umda/ecdf.txt" using 1:2 with lines lw 2 title "umda"
set term pdfcairo color enhanced
set output "graphics/jmp-5.pdf"
replot
set term png enhanced
set output "graphics/jmp-5.png"
replot

set term epscairo color enhanced
set output "graphics/jmp-10.eps"
set key title "jmp-10"
plot \
  "results/jmp-10/rls/ecdf.txt" using 1:2 with lines lw 2 title "rls", \
  "results/jmp-10/hc/ecdf.txt" using 1:2 with lines lw 2 title "hc", \
  "results/jmp-10/sa/ecdf.txt" using 1:2 with lines lw 2 title "sa", \
  "results/jmp-10/ea-1p1/ecdf.txt" using 1:2 with lines lw 2 title "ea-1p1", \
  "results/jmp-10/ea-1p10/ecdf.txt" using 1:2 with lines lw 2 title "ea-1p10", \
  "results/jmp-10/ea-10p1/ecdf.txt" using 1:2 with lines lw 2 title "ea-10p1", \
  "results/jmp-10/ea-1c10/ecdf.txt" using 1:2 with lines lw 2 title "ea-1c10", \
  "results/jmp-10/ga/ecdf.txt" using 1:2 with lines lw 2 title "ga", \
  "results/jmp-10/pbil/ecdf.txt" using 1:2 with lines lw 2 title "pbil", \
  "results/jmp-10/umda/ecdf.txt" using 1:2 with lines lw 2 title "umda"
set term pdfcairo color enhanced
set output "graphics/jmp-10.pdf"
replot
set term png enhanced
set output "graphics/jmp-10.png"
replot

set term epscairo color enhanced
set output "graphics/djmp-5.eps"
set key title "djmp-5"
plot \
  "results/djmp-5/rls/ecdf.txt" using 1:2 with lines lw 2 title "rls", \
  "results/djmp-5/hc/ecdf.txt" using 1:2 with lines lw 2 title "hc", \
  "results/djmp-5/sa/ecdf.txt" using 1:2 with lines lw 2 title "sa", \
  "results/djmp-5/ea-1p1/ecdf.txt" using 1:2 with lines lw 2 title "ea-1p1", \
  "results/djmp-5/ea-1p10/ecdf.txt" using 1:2 with lines lw 2 title "ea-1p10", \
  "results/djmp-5/ea-10p1/ecdf.txt" using 1:2 with lines lw 2 title "ea-10p1", \
  "results/djmp-5/ea-1c10/ecdf.txt" using 1:2 with lines lw 2 title "ea-1c10", \
  "results/djmp-5/ga/ecdf.txt" using 1:2 with lines lw 2 title "ga", \
  "results/djmp-5/pbil/ecdf.txt" using 1:2 with lines lw 2 title "pbil", \
  "results/djmp-5/umda/ecdf.txt" using 1:2 with lines lw 2 title "umda"
set term pdfcairo color enhanced
set output "graphics/djmp-5.pdf"
replot
set term png enhanced
set output "graphics/djmp-5.png"
replot

set term epscairo color enhanced
set output "graphics/djmp-10.eps"
set key title "djmp-10"
plot \
  "results/djmp-10/rls/ecdf.txt" using 1:2 with lines lw 2 title "rls", \
  "results/djmp-10/hc/ecdf.txt" using 1:2 with lines lw 2 title "hc", \
  "results/djmp-10/sa/ecdf.txt" using 1:2 with lines lw 2 title "sa", \
  "results/djmp-10/ea-1p1/ecdf.txt" using 1:2 with lines lw 2 title "ea-1p1", \
  "results/djmp-10/ea-1p10/ecdf.txt" using 1:2 with lines lw 2 title "ea-1p10", \
  "results/djmp-10/ea-10p1/ecdf.txt" using 1:2 with lines lw 2 title "ea-10p1", \
  "results/djmp-10/ea-1c10/ecdf.txt" using 1:2 with lines lw 2 title "ea-1c10", \
  "results/djmp-10/ga/ecdf.txt" using 1:2 with lines lw 2 title "ga", \
  "results/djmp-10/pbil/ecdf.txt" using 1:2 with lines lw 2 title "pbil", \
  "results/djmp-10/umda/ecdf.txt" using 1:2 with lines lw 2 title "umda"
set term pdfcairo color enhanced
set output "graphics/djmp-10.pdf"
replot
set term png enhanced
set output "graphics/djmp-10.png"
replot

set term epscairo color enhanced
set output "graphics/fp-5.eps"
set key title "fp-5"
plot \
  "results/fp-5/rls/ecdf.txt" using 1:2 with lines lw 2 title "rls", \
  "results/fp-5/hc/ecdf.txt" using 1:2 with lines lw 2 title "hc", \
  "results/fp-5/sa/ecdf.txt" using 1:2 with lines lw 2 title "sa", \
  "results/fp-5/ea-1p1/ecdf.txt" using 1:2 with lines lw 2 title "ea-1p1", \
  "results/fp-5/ea-1p10/ecdf.txt" using 1:2 with lines lw 2 title "ea-1p10", \
  "results/fp-5/ea-10p1/ecdf.txt" using 1:2 with lines lw 2 title "ea-10p1", \
  "results/fp-5/ea-1c10/ecdf.txt" using 1:2 with lines lw 2 title "ea-1c10", \
  "results/fp-5/ga/ecdf.txt" using 1:2 with lines lw 2 title "ga", \
  "results/fp-5/pbil/ecdf.txt" using 1:2 with lines lw 2 title "pbil", \
  "results/fp-5/umda/ecdf.txt" using 1:2 with lines lw 2 title "umda"
set term pdfcairo color enhanced
set output "graphics/fp-5.pdf"
replot
set term png enhanced
set output "graphics/fp-5.png"
replot

set term epscairo color enhanced
set output "graphics/fp-10.eps"
set key title "fp-10"
plot \
  "results/fp-10/rls/ecdf.txt" using 1:2 with lines lw 2 title "rls", \
  "results/fp-10/hc/ecdf.txt" using 1:2 with lines lw 2 title "hc", \
  "results/fp-10/sa/ecdf.txt" using 1:2 with lines lw 2 title "sa", \
  "results/fp-10/ea-1p1/ecdf.txt" using 1:2 with lines lw 2 title "ea-1p1", \
  "results/fp-10/ea-1p10/ecdf.txt" using 1:2 with lines lw 2 title "ea-1p10", \
  "results/fp-10/ea-10p1/ecdf.txt" using 1:2 with lines lw 2 title "ea-10p1", \
  "results/fp-10/ea-1c10/ecdf.txt" using 1:2 with lines lw 2 title "ea-1c10", \
  "results/fp-10/ga/ecdf.txt" using 1:2 with lines lw 2 title "ga", \
  "results/fp-10/pbil/ecdf.txt" using 1:2 with lines lw 2 title "pbil", \
  "results/fp-10/umda/ecdf.txt" using 1:2 with lines lw 2 title "umda"
set term pdfcairo color enhanced
set output "graphics/fp-10.pdf"
replot
set term png enhanced
set output "graphics/fp-10.png"
replot

set term epscairo color enhanced
set output "graphics/nk.eps"
set key title "nk"
plot \
  "results/nk/rls/ecdf.txt" using 1:2 with lines lw 2 title "rls", \
  "results/nk/hc/ecdf.txt" using 1:2 with lines lw 2 title "hc", \
  "results/nk/sa/ecdf.txt" using 1:2 with lines lw 2 title "sa", \
  "results/nk/ea-1p1/ecdf.txt" using 1:2 with lines lw 2 title "ea-1p1", \
  "results/nk/ea-1p10/ecdf.txt" using 1:2 with lines lw 2 title "ea-1p10", \
  "results/nk/ea-10p1/ecdf.txt" using 1:2 with lines lw 2 title "ea-10p1", \
  "results/nk/ea-1c10/ecdf.txt" using 1:2 with lines lw 2 title "ea-1c10", \
  "results/nk/ga/ecdf.txt" using 1:2 with lines lw 2 title "ga", \
  "results/nk/pbil/ecdf.txt" using 1:2 with lines lw 2 title "pbil", \
  "results/nk/umda/ecdf.txt" using 1:2 with lines lw 2 title "umda"
set term pdfcairo color enhanced
set output "graphics/nk.pdf"
replot
set term png enhanced
set output "graphics/nk.png"
replot

set term epscairo color enhanced
set output "graphics/max-sat.eps"
set key title "max-sat"
plot \
  "results/max-sat/rls/ecdf.txt" using 1:2 with lines lw 2 title "rls", \
  "results/max-sat/hc/ecdf.txt" using 1:2 with lines lw 2 title "hc", \
  "results/max-sat/sa/ecdf.txt" using 1:2 with lines lw 2 title "sa", \
  "results/max-sat/ea-1p1/ecdf.txt" using 1:2 with lines lw 2 title "ea-1p1", \
  "results/max-sat/ea-1p10/ecdf.txt" using 1:2 with lines lw 2 title "ea-1p10", \
  "results/max-sat/ea-10p1/ecdf.txt" using 1:2 with lines lw 2 title "ea-10p1", \
  "results/max-sat/ea-1c10/ecdf.txt" using 1:2 with lines lw 2 title "ea-1c10", \
  "results/max-sat/ga/ecdf.txt" using 1:2 with lines lw 2 title "ga", \
  "results/max-sat/pbil/ecdf.txt" using 1:2 with lines lw 2 title "pbil", \
  "results/max-sat/umda/ecdf.txt" using 1:2 with lines lw 2 title "umda"
set term pdfcairo color enhanced
set output "graphics/max-sat.pdf"
replot
set term png enhanced
set output "graphics/max-sat.png"
replot

set term epscairo color enhanced
set output "graphics/labs.eps"
set key title "labs"
plot \
  "results/labs/rls/ecdf.txt" using 1:2 with lines lw 2 title "rls", \
  "results/labs/hc/ecdf.txt" using 1:2 with lines lw 2 title "hc", \
  "results/labs/sa/ecdf.txt" using 1:2 with lines lw 2 title "sa", \
  "results/labs/ea-1p1/ecdf.txt" using 1:2 with lines lw 2 title "ea-1p1", \
  "results/labs/ea-1p10/ecdf.txt" using 1:2 with lines lw 2 title "ea-1p10", \
  "results/labs/ea-10p1/ecdf.txt" using 1:2 with lines lw 2 title "ea-10p1", \
  "results/labs/ea-1c10/ecdf.txt" using 1:2 with lines lw 2 title "ea-1c10", \
  "results/labs/ga/ecdf.txt" using 1:2 with lines lw 2 title "ga", \
  "results/labs/pbil/ecdf.txt" using 1:2 with lines lw 2 title "pbil", \
  "results/labs/umda/ecdf.txt" using 1:2 with lines lw 2 title "umda"
set term pdfcairo color enhanced
set output "graphics/labs.pdf"
replot
set term png enhanced
set output "graphics/labs.png"
replot

set term epscairo color enhanced
set output "graphics/ep.eps"
set key title "ep"
plot \
  "results/ep/rls/ecdf.txt" using 1:2 with lines lw 2 title "rls", \
  "results/ep/hc/ecdf.txt" using 1:2 with lines lw 2 title "hc", \
  "results/ep/sa/ecdf.txt" using 1:2 with lines lw 2 title "sa", \
  "results/ep/ea-1p1/ecdf.txt" using 1:2 with lines lw 2 title "ea-1p1", \
  "results/ep/ea-1p10/ecdf.txt" using 1:2 with lines lw 2 title "ea-1p10", \
  "results/ep/ea-10p1/ecdf.txt" using 1:2 with lines lw 2 title "ea-10p1", \
  "results/ep/ea-1c10/ecdf.txt" using 1:2 with lines lw 2 title "ea-1c10", \
  "results/ep/ga/ecdf.txt" using 1:2 with lines lw 2 title "ga", \
  "results/ep/pbil/ecdf.txt" using 1:2 with lines lw 2 title "pbil", \
  "results/ep/umda/ecdf.txt" using 1:2 with lines lw 2 title "umda"
set term pdfcairo color enhanced
set output "graphics/ep.pdf"
replot
set term png enhanced
set output "graphics/ep.png"
replot

set term epscairo color enhanced
set output "graphics/cancel.eps"
set key title "cancel"
plot \
  "results/cancel/rls/ecdf.txt" using 1:2 with lines lw 2 title "rls", \
  "results/cancel/hc/ecdf.txt" using 1:2 with lines lw 2 title "hc", \
  "results/cancel/sa/ecdf.txt" using 1:2 with lines lw 2 title "sa", \
  "results/cancel/ea-1p1/ecdf.txt" using 1:2 with lines lw 2 title "ea-1p1", \
  "results/cancel/ea-1p10/ecdf.txt" using 1:2 with lines lw 2 title "ea-1p10", \
  "results/cancel/ea-10p1/ecdf.txt" using 1:2 with lines lw 2 title "ea-10p1", \
  "results/cancel/ea-1c10/ecdf.txt" using 1:2 with lines lw 2 title "ea-1c10", \
  "results/cancel/ga/ecdf.txt" using 1:2 with lines lw 2 title "ga", \
  "results/cancel/pbil/ecdf.txt" using 1:2 with lines lw 2 title "pbil", \
  "results/cancel/umda/ecdf.txt" using 1:2 with lines lw 2 title "umda"
set term pdfcairo color enhanced
set output "graphics/cancel.pdf"
replot
set term png enhanced
set output "graphics/cancel.png"
replot

set term epscairo color enhanced
set output "graphics/trap.eps"
set key title "trap"
plot \
  "results/trap/rls/ecdf.txt" using 1:2 with lines lw 2 title "rls", \
  "results/trap/hc/ecdf.txt" using 1:2 with lines lw 2 title "hc", \
  "results/trap/sa/ecdf.txt" using 1:2 with lines lw 2 title "sa", \
  "results/trap/ea-1p1/ecdf.txt" using 1:2 with lines lw 2 title "ea-1p1", \
  "results/trap/ea-1p10/ecdf.txt" using 1:2 with lines lw 2 title "ea-1p10", \
  "results/trap/ea-10p1/ecdf.txt" using 1:2 with lines lw 2 title "ea-10p1", \
  "results/trap/ea-1c10/ecdf.txt" using 1:2 with lines lw 2 title "ea-1c10", \
  "results/trap/ga/ecdf.txt" using 1:2 with lines lw 2 title "ga", \
  "results/trap/pbil/ecdf.txt" using 1:2 with lines lw 2 title "pbil", \
  "results/trap/umda/ecdf.txt" using 1:2 with lines lw 2 title "umda"
set term pdfcairo color enhanced
set output "graphics/trap.pdf"
replot
set term png enhanced
set output "graphics/trap.png"
replot

set term epscairo color enhanced
set output "graphics/hiff.eps"
set key title "hiff"
plot \
  "results/hiff/rls/ecdf.txt" using 1:2 with lines lw 2 title "rls", \
  "results/hiff/hc/ecdf.txt" using 1:2 with lines lw 2 title "hc", \
  "results/hiff/sa/ecdf.txt" using 1:2 with lines lw 2 title "sa", \
  "results/hiff/ea-1p1/ecdf.txt" using 1:2 with lines lw 2 title "ea-1p1", \
  "results/hiff/ea-1p10/ecdf.txt" using 1:2 with lines lw 2 title "ea-1p10", \
  "results/hiff/ea-10p1/ecdf.txt" using 1:2 with lines lw 2 title "ea-10p1", \
  "results/hiff/ea-1c10/ecdf.txt" using 1:2 with lines lw 2 title "ea-1c10", \
  "results/hiff/ga/ecdf.txt" using 1:2 with lines lw 2 title "ga", \
  "results/hiff/pbil/ecdf.txt" using 1:2 with lines lw 2 title "pbil", \
  "results/hiff/umda/ecdf.txt" using 1:2 with lines lw 2 title "umda"
set term pdfcairo color enhanced
set output "graphics/hiff.pdf"
replot
set term png enhanced
set output "graphics/hiff.png"
replot

set term epscairo color enhanced
set output "graphics/plateau.eps"
set key title "plateau"
plot \
  "results/plateau/rls/ecdf.txt" using 1:2 with lines lw 2 title "rls", \
  "results/plateau/hc/ecdf.txt" using 1:2 with lines lw 2 title "hc", \
  "results/plateau/sa/ecdf.txt" using 1:2 with lines lw 2 title "sa", \
  "results/plateau/ea-1p1/ecdf.txt" using 1:2 with lines lw 2 title "ea-1p1", \
  "results/plateau/ea-1p10/ecdf.txt" using 1:2 with lines lw 2 title "ea-1p10", \
  "results/plateau/ea-10p1/ecdf.txt" using 1:2 with lines lw 2 title "ea-10p1", \
  "results/plateau/ea-1c10/ecdf.txt" using 1:2 with lines lw 2 title "ea-1c10", \
  "results/plateau/ga/ecdf.txt" using 1:2 with lines lw 2 title "ga", \
  "results/plateau/pbil/ecdf.txt" using 1:2 with lines lw 2 title "pbil", \
  "results/plateau/umda/ecdf.txt" using 1:2 with lines lw 2 title "umda"
set term pdfcairo color enhanced
set output "graphics/plateau.pdf"
replot
set term png enhanced
set output "graphics/plateau.png"
replot

set term epscairo color enhanced
set output "graphics/walsh2.eps"
set key title "walsh2"
plot \
  "results/walsh2/rls/ecdf.txt" using 1:2 with lines lw 2 title "rls", \
  "results/walsh2/hc/ecdf.txt" using 1:2 with lines lw 2 title "hc", \
  "results/walsh2/sa/ecdf.txt" using 1:2 with lines lw 2 title "sa", \
  "results/walsh2/ea-1p1/ecdf.txt" using 1:2 with lines lw 2 title "ea-1p1", \
  "results/walsh2/ea-1p10/ecdf.txt" using 1:2 with lines lw 2 title "ea-1p10", \
  "results/walsh2/ea-10p1/ecdf.txt" using 1:2 with lines lw 2 title "ea-10p1", \
  "results/walsh2/ea-1c10/ecdf.txt" using 1:2 with lines lw 2 title "ea-1c10", \
  "results/walsh2/ga/ecdf.txt" using 1:2 with lines lw 2 title "ga", \
  "results/walsh2/pbil/ecdf.txt" using 1:2 with lines lw 2 title "pbil", \
  "results/walsh2/umda/ecdf.txt" using 1:2 with lines lw 2 title "umda"
set term pdfcairo color enhanced
set output "graphics/walsh2.pdf"
replot
set term png enhanced
set output "graphics/walsh2.png"
replot

