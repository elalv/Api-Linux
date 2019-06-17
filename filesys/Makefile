include ../Makefile.inc

GEN_EXE = t_statvfs

LINUX_EXE = t_statfs t_mount

EXE = ${GEN_EXE} ${LINUX_EXE} 

all : ${EXE}

allgen : ${GEN_EXE}

clean : 
	${RM} ${EXE} *.o

showall :
	@ echo ${EXE}

${EXE} : ${TLPI_LIB}		# True as a rough approximation
