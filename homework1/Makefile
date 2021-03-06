#
# Sample Makefile for Image Processing class
#
#   George Wolberg (wolberg@cs.ccny.cuny.edu)
#
# This Makefile can be used for all homework assignments, simply
# by changing the ALL variable, and each of the target names
#
#

CC  = g++   # Use gcc compiler. Use g++ for C++ compilation.
CFLAGS  =   # Compiler flags: -g for debug, -O for optimization
LDFLAGS =   # Linker flags
LIBS    = -lm   # Libraries to link with (-lm is the math library)

# This is the Master Compile command, which can be used for
# all executables. Simply make a new entry for each one underneath,
# and change the ALL variable.
# YOU SHOULD NOT NEED TO TOUCH THIS.
#
#   $@ stands for the target name (i.e., the resulting executable)
#   $? stands for the dependency list (i.e., the .o files)
#
COMPILE_EXECUTABLE = $(CC) $(CFLAGS) $(LDFLAGS) -o $@ $? $(LIBS)

# These are all the files to be compiled.
ALL = thr qntz histo_plot histo_stretch histo_match threshold_Otsu histeq histmatch 

all:    $(ALL)

# List of executable targets
thr:    $($@.o) IPutil.o

qntz:  $($@.o) IPutil.o

histeq: $($@.o) IPutil.o

histmatch: $($@.o) IPutil.o

histo_plot: $($@.o) IPutil.o

histo_stretch: $($@.o) IPutil.o

histo_match: $($@.o) IPutil.o

threshold_Otsu: $($@.o) IPutil.o

# Clean target to remove backup, object, and core files
clean:
	rm -f *~ *.o core
	cleanx:
		rm -f *~ *.o core $(ALL)

# Dependencies
IPutil.o:   IP.h
thr.o:      IP.h
histo_plot.o: IP.h
histo_stretch: IP.h
histeq.o:   IP.h
histo_match.o:    IP.h
qntz.o:      IP.h
threshold_Otsu: IP.h