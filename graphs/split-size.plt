reset
set key left box
set xlabel 'n'
set ylabel 'Time'
set datafile separator ","
set logscale x 2
set logscale y
plot "fft.txt" every ::1 using 4:6 with lines,\
	"k1024.txt" every ::1 using 4:6 with lines,\
	"k1024split1024.txt" every ::1 using 4:6 with lines,\
	"k1024split32768.txt" every ::1 using 4:6 with lines,\
	"k1024split65536.txt" every ::1 using 4:6 with lines,\
	"if.txt" every ::1 using 4:6 with lines

#	"split32.txt" every ::1 using 4:6 with lines,\
#	 "split64.txt" every ::1 using 4:6 with lines,\
#	 "split128.txt" every ::1 using 4:6 with lines,\
#	 "split256.txt" every ::1 using 4:6 with lines,\
#	 "split512.txt" every ::1 using 4:6 with lines,\
#	 "split1024.txt" every ::1 using 4:6 with lines,\
#	"k2048.txt" every ::1 using 4:6 with lines,\
#	"k4096.txt" every ::1 using 4:6 with lines,\
