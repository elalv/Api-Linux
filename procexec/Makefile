include ../Makefile.inc

GEN_EXE = child_status exit_handlers \
	footprint fork_file_sharing fork_sig_sync fork_stdio_buf fork_whos_on_first \
	make_zombie multi_SIGCHLD multi_wait t_fork t_vfork vfork_fd_test

LINUX_EXE =t_fork

EXE = ${GEN_EXE} ${LINUX_EXE}

all : ${EXE}

allgen : ${GEN_EXE}

clean : 
	${RM} ${EXE} *.o

showall :
	@ echo ${EXE}

${EXE} : ${TLPI_LIB}		# True as a rough approximation