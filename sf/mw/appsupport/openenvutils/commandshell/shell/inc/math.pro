/* Generated automatically */
static int zzlex _((void));
static void push _((mnumber val,char*lval,int getme));
static mnumber pop _((int noget));
static mnumber getcvar _((char*s));
static mnumber setvar _((char*s,mnumber v));
static mnumber callmathfunc _((char*o));
static int notzero _((mnumber a));
static void bop _((int tk));
static mnumber mathevall _((char*s,int prek,char**ep));
static void checkunary _((int mtokc,char*mptr));
static void mathparse _((int pc));
