include ../Makefile.inc

GEN_EXE =

LINUX_EXE = dump_seccomp_filter libseccomp_demo

EXE = ${GEN_EXE} ${LINUX_EXE}

all : ${EXE}

allgen : ${GEN_EXE}

clean :
	${RM} ${EXE} *.o

showall :
	@ echo ${EXE}

libseccomp_demo : libseccomp_demo.c
	${CC} -o $@ libseccomp_demo.c ${CFLAGS} ${IMPL_LDLIBS} -lseccomp

${EXE} : ${TLPI_LIB}		# True as a rough approximation
