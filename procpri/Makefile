include ../Makefile.inc

GEN_EXE = t_setpriority

LINUX_EXE =

EXE = ${GEN_EXE} ${LINUX_EXE}

LDLIBS = ${IMPL_LDLIBS} #CF[ ${LINUX_LIBRT} ]

all : ${EXE}

allgen : ${GEN_EXE}

clean : 
	${RM} ${EXE} *.o

showall :
	@ echo ${EXE}

${EXE} : ${TLPI_LIB}		# True as a rough approximation
