#! /usr/bin/gnuplot
#set terminal postscript eps enhanced
reset
set term gif animate delay 4
set output "res.gif"
stats './res.dat' nooutput
set size ratio -1
unset key
set xlabel "X" 
set ylabel "Y"
set xrange [-100:100]
set yrange [-100:100]
set colorbox

do for [i = 1:int(STATS_blocks)] {
        set title 'Time '.i
        #plot './res.dat' index (i-1) using 1:2:3 with p lt 1 pt 7 ps 0.4 #variable 
        plot './res.dat' index (i-1) using 1:2:3 with p lt 1 pt 7 ps 0.6 lc pal z
}
