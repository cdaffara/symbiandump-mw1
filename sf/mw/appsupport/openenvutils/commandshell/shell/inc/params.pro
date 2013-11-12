/* Generated automatically */
static HashNode getparamnode _((HashTable ht,char*nam));
static void scancopyparams _((HashNode hn,UNUSED(int flags)));
static char**getvaluearr _((Value v));
static void assigngetset _((Param pm));
static zlong getarg _((char**str,int*inv,Value v,int a2,zlong*w));
static void intsetfn _((Param pm,zlong x));
static double floatgetfn _((Param pm));
static void floatsetfn _((Param pm,double x));
static void strsetfn _((Param pm,char*x));
static void arrhashsetfn _((Param pm,char**val,int augment));
static void setlang _((char*x));
static char**pipestatgetfn _((UNUSED(Param pm)));
static void pipestatsetfn _((UNUSED(Param pm),char**x));
static int findenv _((char*name,int*pos));
static void copyenvstr _((char*s,char*value,int flags));
static char*mkenvstr _((char*name,char*value,int flags));
static void scanendscope _((HashNode hn,UNUSED(int flags)));
