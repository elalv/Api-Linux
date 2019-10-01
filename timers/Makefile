include ../Makefile.inc

GEN_EXE = real_timer timed_read

LINUX_EXE =

EXE = ${GEN_EXE} ${LINUX_EXE}

all : ${EXE}

allgen : ${GEN_EXE}

LDLIBS = ${IMPL_LDLIBS} ${LINUX_LIBRT} ${IMPL_THREAD_FLAGS}
	# Many of the programs in this directory need the 
	# realtime library, librt; to keep this Makefile simple,
	# we link *all* of the programs against that library.
	# Some of the programs in this directory use POSIX thread
	# and so require building with '-pthread'. To keep the Makefile
	# simple, we build all of the programs with '-pthread'

clean : 
	${RM} ${EXE} *.o

showall :
	@ echo ${EXE}

${EXE} : ${TLPI_LIB}		# True as a rough approximation
