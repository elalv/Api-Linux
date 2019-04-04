include ../Makefile.inc

GEN_EXE = display_env longjmp mem_segments necho setjmp_vars

LINUX_EXE = modify_env

EXE = ${GEN_EXE} ${LINUX_EXE}

all : ${EXE}

allgen : ${GEN_EXE}

clean : 
	${RM} ${EXE} *.o

showall :
	@ echo ${EXE}

${EXE} : ${TLPI_LIB}		# True as a rough approximation
