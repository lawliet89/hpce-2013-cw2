reset
set term pdfcairo
set output "ywc110_scalability.pdf"
set key left box
set title "Comparison of Execution Times of FFT Optimised with Varying Processors\nUbuntu 13.10; Intel Core i5-2500 (4 physical/logical Cores)"
set xlabel 'Bin Size (n)'
set xrange [16:]
set ylabel 'Execution Time/s'
set datafile separator ","
set logscale x 2
set logscale y
set grid
plot "fft_opt_1.txt" every ::1 using 4:6 with lines title "1 Core",\
	"fft_opt_2.txt" every ::1 using 4:6 with lines title "2 Cores",\
	"fft_opt_3.txt" every ::1 using 4:6 with lines title "3 Cores",\
	"fft_opt_4.txt" every ::1 using 4:6 with lines title "4 Cores"
