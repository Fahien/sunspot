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
set ylabel "Computation time (milliseconds)"
set yrange[0:0.3]
set xrange[0:500]
set style fill transparent solid 0.5 noborder
plot "output.dat" using 1:($5/1000000) title 'Total' with filledcu x1, \
     '' using 1:($2/1000000) title 'Pass 1' with filledcurves x1, \
     '' using 1:($3/1000000) title 'Pass 2.1' with filledcu x1, \
     '' using 1:($4/1000000) title 'Pass 2.2' with filledcu x1
pause .125
reread
