/* Generated automatically */
#ifdef HAVE_GETRLIMIT
#endif /* HAVE_GETRLIMIT */
static pid_t zfork _((struct timeval*tv));
static int execcursh _((Estate state,int do_exec));
static int zexecve _((char*pth,char**argv));
static int isgooderr _((int e,char*dir));
static int execsimple _((Estate state));
static int execpline _((Estate state,wordcode slcode,int how,int last1));
static void execpline2 _((Estate state,wordcode pcode,int how,int input,int output,int last1));
static char**makecline _((LinkList list));
static int clobber_open _((struct redir*f));
static void closemn _((struct multio**mfds,int fd));
static void closemnodes _((struct multio**mfds));
static void closeallelse _((struct multio*mn));
static void addfd _((int forked,int*save,struct multio**mfds,int fd1,int fd2,int rflag));
static void addvars _((Estate state,Wordcode pc,int export));
static void execcmd _((Estate state,int input,int output,int how,int last1));
static void save_params _((Estate state,Wordcode pc,LinkList*restore_p,LinkList*remove_p));
static void restore_params _((LinkList restorelist,LinkList removelist));
static void fixfds _((int*save));
static void entersubsh _((int how,int cl,int fake,int revertpgrp));
static int getherestr _((struct redir*fn));
static Eprog parsecmd _((char*cmd));
static int getpipe _((char*cmd,int nullexec));
static void mpipe _((int*pp));
static void spawnpipes _((LinkList l,int nullexec));
static int execcond _((Estate state,UNUSED(int do_exec)));
static int execarith _((Estate state,UNUSED(int do_exec)));
static int exectime _((Estate state,UNUSED(int do_exec)));
static int execfuncdef _((Estate state,UNUSED(int do_exec)));
static void execshfunc _((Shfunc shf,LinkList args));
static int execautofn _((Estate state,UNUSED(int do_exec)));
static char*cancd _((char*s));
static int cancd2 _((char*s));
