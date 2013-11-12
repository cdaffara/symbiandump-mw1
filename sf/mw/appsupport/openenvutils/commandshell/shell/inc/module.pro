/* Generated automatically */
static void printmodalias _((Module m,Options ops));
#ifdef DYNAMIC
#ifdef AIXDYNAMIC
#else
#ifdef HPUXDYNAMIC
#endif
#endif /* !AIXDYNAMIC */
static void*try_load_module _((char const*name));
static void*do_load_module _((char const*name));
#else /* !DYNAMIC */
static void*do_load_module _((char const*name));
#endif /* !DYNAMIC */
static LinkNode find_module _((const char*name,int aliasp,const char**namep));
static void delete_module _((LinkNode node));
#ifdef DYNAMIC
#ifdef AIXDYNAMIC
static int dyn_setup_module _((Module m));
static int dyn_boot_module _((Module m));
static int dyn_cleanup_module _((Module m));
static int dyn_finish_module _((Module m));
#else
static int dyn_setup_module _((Module m));
static int dyn_boot_module _((Module m));
static int dyn_cleanup_module _((Module m));
static int dyn_finish_module _((Module m));
#endif /* !AIXDYNAMIC */
static int setup_module _((Module m));
static int boot_module _((Module m));
static int cleanup_module _((Module m));
static int finish_module _((Module m));
#else /* !DYNAMIC */
static int setup_module _((Module m));
static int boot_module _((Module m));
static int cleanup_module _((Module m));
static int finish_module _((Module m));
#endif /* !DYNAMIC */
static int modname_ok _((char const*p));
static void autoloadscan _((HashNode hn,int printflags));
static int bin_zmodload_alias _((char*nam,char**args,Options ops));
static int bin_zmodload_exist _((UNUSED(char*nam),char**args,Options ops));
static int bin_zmodload_dep _((UNUSED(char*nam),char**args,Options ops));
static int bin_zmodload_auto _((char*nam,char**args,Options ops));
static int bin_zmodload_cond _((char*nam,char**args,Options ops));
static int bin_zmodload_math _((char*nam,char**args,Options ops));
static int bin_zmodload_param _((char*nam,char**args,Options ops));
static int bin_zmodload_load _((char*nam,char**args,Options ops));
static void removemathfunc _((MathFunc previous,MathFunc current));
