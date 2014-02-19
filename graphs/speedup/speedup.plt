reset
set term pdfcairo size 29.7cm, 21cm
set output "ywc110_speedup.pdf"
set key left box
set title "Comparison of Execution Times for Various Implementations\nUbuntu 13.10; Intel Core i5-2500 (4 physical/logical Cores)"
set xlabel 'Bin Size (n)'
set xrange [16:]
set ylabel 'Execution Time/s'
set datafile separator ","
set logscale x 2
set logscale y
set grid
plot "dft.txt" every ::1 using 4:6 with lines title "DFT",\
	"fft.txt" every ::1 using 4:6 with lines title "FFT",\
	"dft_parfor.txt" every ::1 using 4:6 with lines title "DFT Parfor",\
	"fft_taskgroup.txt" every ::1 using 4:6 with lines title "FFT Taskgroup",\
	"fft_parfor.txt" every ::1 using 4:6 with lines title "FFT Parfor",\
	"fft_combined.txt" every ::1 using 4:6 with lines title "FFT Combined",\
	"fft_opt.txt" every ::1 using 4:6 with lines title "FFT Optimised"
