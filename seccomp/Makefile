include ../Makefile.inc

GEN_EXE =

LINUX_EXE = dump_seccomp_filter libseccomp_demo seccomp_arg64 seccomp_bench \
	seccomp_control_open seccomp_deny_open seccomp_deny_syscall seccomp_launch \
	seccomp_logging

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
