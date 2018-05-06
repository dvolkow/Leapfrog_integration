#! /usr/bin/gnuplot
#set terminal postscript eps enhanced
reset
set term gif animate delay 5
set output "res.gif"
stats './res.dat' nooutput
set size ratio -1
unset key
set xlabel "X" 
set ylabel "Y"
set xrange [-200:200]
set yrange [-200:200]

do for [i = 1:int(STATS_blocks)] {
        set title 'time '.i
        plot './res.dat' index (i-1) using 1:2:3 with p lt 1 pt 7 ps variable 
}
