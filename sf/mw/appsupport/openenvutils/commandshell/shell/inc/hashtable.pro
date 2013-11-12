/* Generated automatically */
static int hnamcmp _((const void*ap,const void*bp));
static void expandhashtable _((HashTable ht));
static void resizehashtable _((HashTable ht,int newsize));
#ifdef ZSH_HASH_DEBUG
static void printhashtabinfo _((HashTable ht));
#endif /* ZSH_HASH_DEBUG */
static void emptycmdnamtable _((HashTable ht));
static void fillcmdnamtable _((UNUSED(HashTable ht)));
static void freecmdnamnode _((HashNode hn));
static void printcmdnamnode _((HashNode hn,int printflags));
static HashNode removeshfuncnode _((UNUSED(HashTable ht),char*nam));
static void disableshfuncnode _((HashNode hn,UNUSED(int flags)));
static void enableshfuncnode _((HashNode hn,UNUSED(int flags)));
static void freeshfuncnode _((HashNode hn));
static void printshfuncnode _((HashNode hn,int printflags));
static void printreswdnode _((HashNode hn,int printflags));
static void freealiasnode _((HashNode hn));
static void printaliasnode _((HashNode hn,int printflags));
static void emptynameddirtable _((HashTable ht));
static void fillnameddirtable _((UNUSED(HashTable ht)));
static void addnameddirnode _((HashTable ht,char*nam,void*nodeptr));
static HashNode removenameddirnode _((HashTable ht,char*nam));
static void freenameddirnode _((HashNode hn));
static void printnameddirnode _((HashNode hn,int printflags));
