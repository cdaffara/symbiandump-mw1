/* Generated automatically */
static void patadd _((char*add,int ch,long n,int paflags));
static long patcompswitch _((int paren,int*flagp));
static long patcompbranch _((int*flagp));
static long patcomppiece _((int*flagp));
static long patcompnot _((int paren,int*flagsp));
static long patnode _((long op));
static void patinsert _((long op,int opnd,char*xtra,int sz));
static void pattail _((long p,long val));
static void patoptail _((long p,long val));
static int patmatch _((Upat prog));
static int patmatchrange _((char*range,int ch));
static int patrepeat _((Upat p));
#ifdef ZSH_PAT_DEBUG
static void patdump _((Patprog r));
static char*patprop _((Upat op));
#endif /* ZSH_PAT_DEBUG */
