/* Generated automatically */
static LinkNode stringsubst _((LinkList list,LinkNode node,int ssub,int asssub));
static int mult_isarr;
static int multsub _((char**s,char***a,int*isarr,UNUSED(char*sep)));
static char*strcatsub _((char**d,char*pb,char*pe,char*src,int l,char*s,int glbsub,int copied));
static char*dopadding _((char*str,int prenum,int postnum,char*preone,char*postone,char*premul,char*postmul));
static int get_intarg _((char**s));
static char*arithsubst _((char*a,char**bptr,char*rest));
static char*dstackent _((char ch,int val));
