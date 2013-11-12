/* Generated automatically */
static char**slashsplit _((char*s));
static int xsymlinks _((char*s));
static void finddir_scan _((HashNode hn,UNUSED(int flags)));
static int dircmp _((char*s,char*t));
static void checkmailpath _((char**s));
static void spscan _((HashNode hn,UNUSED(int scanflags)));
static int ztrftimebuf _((int*bufsizeptr,int decr));
static int skipwsep _((char**s));
static int findsep _((char**s,char*sep,int quote));
static char*spname _((char*oldname));
static int mindist _((char*dir,char*mindistguess,char*mindistbest));
static int spdist _((char*s,char*t,int thresh));
static char*nicedup _((char const*s,int heap));
static int upchdir _((int n));
