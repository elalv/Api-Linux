include ../Makefile.inc

GEN_EXE = new_intr

EXE = ${GEN_EXE} ${LINUX_EXE}

all : ${EXE}

allgen : ${GEN_EXE}

clean :
	${RM} ${EXE} *.o

showall :
	@ echo ${EXE}

${EXE} : ${TLPI_LIB}		# True as a rough approximation
