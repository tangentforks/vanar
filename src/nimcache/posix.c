/* Generated by Nimrod Compiler v0.9.2 */
/*   (c) 2012 Andreas Rumpf */
/* The generated code is subject to the original license. */
/* Compiled for: Linux, amd64, gcc */
/* Command for C compiler:
   gcc -c  -w  -I/home/claude/Projekte/Nimrod/lib -o /home/claude/Projekte/vanar/src/nimcache/posix.o /home/claude/Projekte/vanar/src/nimcache/posix.c */
#define NIM_INTBITS 64
#include "nimbase.h"

#include <errno.h>

#include <netdb.h>

#include <time.h>

#include <aio.h>

#include <dlfcn.h>

#include <fcntl.h>

#include <fenv.h>

#include <fmtmsg.h>

#include <fnmatch.h>

#include <ftw.h>

#include <glob.h>

#include <langinfo.h>

#include <locale.h>

#include <pthread.h>

#include <unistd.h>

#include <semaphore.h>

#include <sys/ipc.h>

#include <sys/stat.h>

#include <sys/statvfs.h>

#include <sys/mman.h>

#include <sys/wait.h>

#include <signal.h>

#include <nl_types.h>

#include <sched.h>

#include <sys/select.h>

#include <sys/socket.h>

#include <net/if.h>

#include <netinet/in.h>

#include <netinet/tcp.h>

#include <poll.h>

#include <spawn.h>
static N_INLINE(void, nimFrame)(TFrame* s);
static N_INLINE(void, popFrame)(void);
extern TFrame* frameptr_11625;

static N_INLINE(void, nimFrame)(TFrame* s) {
	(*s).prev = frameptr_11625;
	frameptr_11625 = s;
}

static N_INLINE(void, popFrame)(void) {
	frameptr_11625 = (*frameptr_11625).prev;
}
N_NOINLINE(void, posixInit)(void) {
	nimfr("posix", "posix.nim")
	popFrame();
}

N_NOINLINE(void, posixDatInit)(void) {
}

