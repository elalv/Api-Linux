For instructions on building the programs, see the file [BUILDING](https://github.com/elalv/Api-Linux/blob/main/BUILDING).

Directory       Features

lib             This contains library routines used by other
                programs. The tlpi_hdr.h and error_functions.*
                files are located here.

progconc        3 (System Programming Concepts)

fileio          4 and 5 (File I/O)

proc            6 (Processes)

memalloc        7 (Memory Allocation)

users_groups    8 (Users and Groups)

proccred        9 (Process Credentials)

time            10 (Time)

syslim          11 (System Limits and Options)

sysinfo         12 (System and Process Information)

filebuff        13 (File I/O Buffering)

filesys         14 (File Systems)

files           15 (File Attributes)

xattr           16 (Extended Attributes)

acl             17 (Access Control Lists)

dirs_links      18 (Directories and Links)

inotify         19 (Monitoring File Events)

signals         20 to 22 (Signals)

timers          23 (Timers and Sleeping)

procexec        24 (Process Creation), 25 (Process Termination),
                26 (Monitoring Child Processes), 27 (Program Execution),
                and 28 (Further Details on Process Creation and Program
                Execution)

threads         29 to 33 (POSIX Threads)

pgsjc           34 (Process Groups, Sessions, and Job Control)

procpri         35 (Process Priorities and Scheduling)

procres         36 (Process Resources)

daemons         37 (Daemons)

cap             39 (Capabilities)

loginacct       40 (Login Accounting)

shlibs          41 and 42 (Shared Libraries)

pipes           44 (Pipes and FIFOs)

svipc           45 (System V IPC)

svmsg           46 (System V Message Queues)

svsem           47 (System V Semaphores)

svshm           48 (System V Shared Memory)

mmap            49 (Memory Mappings)

vmem            50 (Virtual Memory Operations)

pmsg            52 (POSIX Message Queues)

psem            53 (POSIX Semaphores)

pshm            54 (POSIX Shared Memory)

filelock        55 (File Locking)

sockets         56 to 61 (Sockets and Network Programming)

tty             62 (Terminals)

altio           63 (Alternative I/O Models)

pty             64 (Pseudoterminals)

getopt          B.1: Parsing Command-Line Options

cgroups         A.1: (Control Groups) Linux kernel feature that limits, 
                accounts for, and isolates the resource usage
                (CPU, memory, disk, I/O) of a collection of processes

namespaces      A.2: Code examples for namespaces, mainly related to my LWN.net
                article series starting at https://lwn.net/Articles/531114/

seccomp         A.3: Code examples demonstrating the use of the seccomp
                (Secure Computing) facility.

vdso            A.4: Some example code relating to the VDSO (Virtual Dynamic
                Shared Object); see vdso(7)