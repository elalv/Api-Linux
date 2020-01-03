include ../Makefile.inc

GEN_EXE = detached_attrib simple_thread

LINUX_EXE =

EXE = ${GEN_EXE} ${LINUX_EXE}

all : ${EXE}

allgen : ${GEN_EXE}

CFLAGS = ${IMPL_CFLAGS} ${IMPL_THREAD_FLAGS}
LDLIBS = ${IMPL_LDLIBS} ${IMPL_THREAD_FLAGS}

clean : 
	${RM} ${EXE} *.o

showall :
	@ echo ${EXE}

${EXE} : ${TLPI_LIB}		# True as a rough approximation
