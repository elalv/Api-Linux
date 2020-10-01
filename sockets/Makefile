include ../Makefile.inc

GEN_EXE = i6d_ucase_sv i6d_ucase_cl \
	is_seqnum_sv is_seqnum_cl t_gethostbyname ud_ucase_sv ud_ucase_cl us_xfr_cl us_xfr_sv

LINUX_EXE = us_abstract_bind

EXE = ${GEN_EXE} ${LINUX_EXE}

all : ${EXE}

allgen : ${GEN_EXE}

i6d_ucase_sv.o i6d_ucase_cl.o : i6d_ucase.h

is_seqnum_sv.o is_seqnum_cl.o : is_seqnum.h

us_xfr_sv.o us_xfr_cl.o : us_xfr.h

ud_ucase_sv.o ud_ucase_cl.o : ud_ucase.h

clean :
	${RM} ${EXE} *.o

showall :
	@ echo ${EXE}

${EXE} : ${TLPI_LIB}		# True as a rough approximation
