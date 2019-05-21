
set terminal png size 800,600 enhanced
do for [generator in "random reversed sorted"] {
# do for [generator in "random"] {
    filename='../full_data/full_'.generator.'.dat'

    do for [size in "100000000"] {
        set output '../images/speedup_'.generator.'_size_'.size.'.png'
        set title "Generator: ".generator." with array size ".size
        set xlabel "Thread pool size"
        set ylabel "Speedup"
	set key top left
	set grid
	set xrange [1:32]

        plot filename using (($1==size)?$2:1/0):($3/$4) with linespoints title "Raw J" linecolor 1, \
        filename using (($1==size)?$2:1/0):($3/$5) with linespoints title "Raw JC" linecolor 2, \
        filename using (($1==size)?$2:1/0):($3/$9) with linespoints title "Raw Adaptive" linecolor 8, \
        filename using (($1==size)?$2:1/0):($3/$6) with linespoints title "No copy J" linecolor 3, \
        filename using (($1==size)?$2:1/0):($3/$7) with linespoints title "No copy JC" linecolor 4, \
        filename using (($1==size)?$2:1/0):($3/$8) with linespoints title "Rayon" linecolor 6, \
        filename using (($1==size)?$2:1/0):($3/$10) with linespoints title "OMP" linecolor 7, \
        filename using (($1==size)?$2:1/0):($3/$3) with linespoints title "Seq" linecolor 5

    }
}
