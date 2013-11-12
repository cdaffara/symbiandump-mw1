/* Generated automatically */
#ifdef HAVE_GETRUSAGE
static struct rusage child_usage;
#else
static struct tms shtms;
#endif
static struct timeval*dtime _((struct timeval*dt,struct timeval*t1,struct timeval*t2));
static int super_job _((int sub));
static int handle_sub _((int job,int fg));
static void setprevjob _((void));
#ifndef HAVE_GETRUSAGE
static void set_clktck _((void));
#endif
static void printhhmmss _((double secs));
static void dumptime _((Job jn));
static int should_report_time _((Job j));
static void zwaitjob _((int job,int sig));
static int isanum _((char*s));
static void setcurjob _((void));
static int getjob _((char*s,char*prog));
