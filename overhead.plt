#
# Gnuplot script file for plotting data in file "output.dat"
# This file is called "overhead.plt"
# Antonio Caggiano <intoantoniocaggiano@gmail.com>
# February 2017
#
set autoscale # scale axes automatically
unset log     # remove any log-scaling
unset label   # remove any previous labels
set xtic auto # set xtics automatically
set ytic auto # set ytics automatically
set title "Sunspot Stereoscopic Rendering"
set xlabel "Frame number"
set ylabel "Second pass overhead (percent)"
set yrange [0:100]
set style fill transparent solid 0.5 noborder
plot "output.dat" using 1:6 title 'Overhead' with filledcu x1
pause .125
reread
