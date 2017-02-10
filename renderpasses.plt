#
# Gnuplot script file for plotting data in file "output.dat"
# This file is called "renderpasses.plt"
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
set ylabel "Computation time (nanoseconds)"
set yrange[0:300000]
set style fill transparent solid 0.5 noborder
plot "output.dat" using 1:5 title 'Total' with filledcu x1, \
     '' using 1:2 title 'Pass 1' with filledcurves x1, \
     '' using 1:3 title 'Pass 2.1' with filledcu x1, \
     '' using 1:4 title 'Pass 2.2' with filledcu x1
pause .125
reread
