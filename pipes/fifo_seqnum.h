/* fifo_seqnum.h

   Header file used by fifo_seqnum_server.c and fifo_seqnum_client.c

   These programs create FIFOS in /tmp. This makes it easy to compile and
   run the programs. However, for a security reasons, a real-world
   application should never create sensitive files in /tmp. (As a simple of
   example of the kind of security problems that can result, a malicious
   user could create a FIFO using the name defined in SERVER_FIFO, and
   thereby cause a denial of service attack against this application.)
*/