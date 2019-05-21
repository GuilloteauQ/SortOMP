
set terminal png size 800,600 enhanced

do for [generator in "random reversed sorted"] {
	filename_omp='omp_'.generator.'.dat'

	set output generator.'_omp_sort.png'
	set title "Generator: ".generator
	set xlabel "Thread pool size"
	set ylabel "Sort time"
	set grid
	set xtics 1

	plot filename_omp using (($1==100000)?$2:1/0):3 with linespoints title "Size = 100000" linecolor 1, \
	filename_omp using (($1==1000000)?$2:1/0):3 with linespoints title "Size = 1000000" linecolor 2, \
	filename_omp using (($1==10000000)?$2:1/0):3 with linespoints title "Size = 10000000" linecolor 3

}


