include ../Makefile.inc

GEN_EXE =

LINUX_EXE = cred_launcher demo_userns demo_uts_namespaces

EXE = ${GEN_EXE} ${LINUX_EXE}

all : ${EXE}

allgen : ${GEN_EXE}

clean :
	${RM} ${EXE} *.o

showall :
	@ echo ${EXE}

# Many of the programs here need libcap

LDLIBS = ${IMPL_LDLIBS} ${LINUX_LIBCAP}

${EXE} : ${TLPI_LIB}		# True as a rough approximation
