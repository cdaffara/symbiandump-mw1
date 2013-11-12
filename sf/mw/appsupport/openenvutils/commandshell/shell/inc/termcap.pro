/* Generated automatically */
static int ztgetflag _((char*s));
static int bin_echotc _((char*name,char**argv,UNUSED(Options ops),UNUSED(int func)));
#ifdef HAVE_TGETENT
static void shempty _((void));
static Param createtchash _(());
static HashNode gettermcap _((UNUSED(HashTable ht),char*name));
static void scantermcap _((UNUSED(HashTable ht),ScanFunc func,int flags));
#endif /* HAVE_TGETENT */
