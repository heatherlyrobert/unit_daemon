#include "unit_daemon.h"


typedef struct tm        tTIME;
static char    x_term    = '-';
static char    x_cpu     = '-';



void             /* [------] receive signals ---------------------------------*/
incomming          (int a_signal, siginfo_t *a_info, char *a_name, char *a_desc)
{
   /*---(locals)-----------+-----+-----+-*/
   FILE       *f           = NULL;
   int         x_pid       = 0;
   char        t           [LEN_DESC]  = "";
   char        x_recd      [LEN_HUND]  = "";
   /*---(message)-------------------------------*/
   x_pid     = getpid ();
   f = fopen ("/tmp/signal_local.log", "at");
   sprintf (t, "local  %-7.7s (%2d) %s", a_name, a_signal, a_desc);
   yEXEC_heartquiet (x_pid, 0, t, NULL, x_recd);
   if (f != NULL) {
      fprintf (f, "%s\n", x_recd);
      fflush (f);
      fclose (f);
      f = NULL;
   }
   /*---(check signal)-------------------*/
   switch (a_signal) {
   case  SIGCHLD:
      break;
   case  SIGHUP:
      break;
   case  SIGUSR1:
      break;
   case  SIGUSR2:
      break;
   case  SIGALRM:
      if (x_cpu  != '-')  _exit (0);
      break;
   case  SIGSEGV:
      break;
   case  SIGTERM:
      sleep (1);
      if (x_term == 'y')  _exit (SIGTERM);
      break;
   case  SIGABRT:
      break;
   case  SIGTSTP:
      break;
   case  SIGCONT:
      break;
   case  SIGINT :
      break;
   case  SIGQUIT:
      break;
   default      :
      break;
   }
   /*---(complete)-----------------------*/
   return;
}

int
main               (int a_argc, char *a_argv [])
{
   /*---(locals)-----------+-----+-----+-*/
   char       *p           =  0x2;
   char        x_daemon    =  'y';
   int         x_loop      =    2;
   char        rc          =    0;
   int         i           =    0;
   char        x_bullet    = YEXEC_SOFT;
   char        x_inter     = YEXEC_NO;
   char        x_child     = YEXEC_WAIT;
   FILE       *f           = NULL;
   int         x_pid       =   -1;
   int         x_ppid      =   -1;
   int         x_uid       =   -1;
   char        x_root      =  '-';
   char        x_who       [LEN_LABEL] = "";
   char        x_proj      [LEN_LABEL] = "";
   char        x_home      [LEN_HUND]  = "";
   int         x_hup       =    0;
   int         a           =    0;
   int         b           =    0;
   char        x_mem       =  '-';
   char        x_cycle     =  '-';
   char        x_secs      [LEN_HUND]  = "";
   char        x_altname   [LEN_HUND]  = "";
   int         x_max       =    0;
   int         x_len       =    0;
   /*---(set up signal handling)---------*/
   for (i = 1; i < a_argc; ++i) {
      if (strcmp (a_argv [i], "--bulletproof"   ) == 0)   x_bullet = YEXEC_HARD;
      if (strcmp (a_argv [i], "--interactive"   ) == 0)   x_inter  = YEXEC_YES;
      if (strcmp (a_argv [i], "--children"      ) == 0)   x_child  = YEXEC_YES;
   }
   /*---(determine looping)--------------*/
   for (i = 1; i < a_argc; ++i) {
      if      (strcmp (a_argv [i], "--nodaemon") == 0)    x_daemon = '-';
      else if (strcmp (a_argv [i], "--graceful") == 0)    x_term   = 'y';
      else if (strcmp (a_argv [i], "--infinite") == 0)    x_loop   = 99999;
      else if (strcmp (a_argv [i], "--super"   ) == 0)    x_loop   = 60;
      else if (strcmp (a_argv [i], "--longer"  ) == 0)    x_loop   = 20;
      else if (strcmp (a_argv [i], "--long"    ) == 0)    x_loop   = 10;
      else if (strcmp (a_argv [i], "--4s"      ) == 0)    x_loop   = 4;
      else if (strcmp (a_argv [i], "--2s"      ) == 0)    x_loop   = 2;
      else if (strcmp (a_argv [i], "--short"   ) == 0)    x_loop   = 1;
      else if (strcmp (a_argv [i], "--instant" ) == 0)    x_loop   = 0;
      else if (strcmp (a_argv [i], "--cycle"   ) == 0)    x_cycle  = 'y';
      else if (strcmp (a_argv [i], "--hup"     ) == 0)    x_hup    = atoi (a_argv [++i]);
      else if (strcmp (a_argv [i], "--Lcpu"    ) == 0)    x_cpu    = 'L';
      else if (strcmp (a_argv [i], "--Mcpu"    ) == 0)    x_cpu    = 'M';
      else if (strcmp (a_argv [i], "--Hcpu"    ) == 0)    x_cpu    = 'H';
      else if (strcmp (a_argv [i], "--normal"  ) == 0)    x_loop   = 2;
      else if (strcmp (a_argv [i], "--Lmem"    ) == 0)    x_mem    = 'L';
      else if (strcmp (a_argv [i], "--Mmem"    ) == 0)    x_mem    = 'M';
      else if (strcmp (a_argv [i], "--Hmem"    ) == 0)    x_mem    = 'H';
      /*---(complicated)-----------------*/
      else if (strcmp (a_argv [i], "--hobo"    ) == 0) {
         strcpy (x_altname, "hobo-rider"    );
      }
      else if (strcmp (a_argv [i], "--gypsy"   ) == 0) {
         strcpy (x_altname, "gypsy-wanderer");
      }
      else if (strcmp (a_argv [i], "--piker"   ) == 0) {
         strcpy (x_altname, "piker-traveler");
      }
      else if (strcmp (a_argv [i], "--segv"    ) == 0) {
         printf ("%s", p);
         x_loop = 2;
      }
      else if (strcmp (a_argv [i], "--badrc"   ) == 0) {
         x_daemon = '-';
         x_loop = 1;
         rc     = -12;
      }
      else if (strcmp (a_argv [i], "--whoami"  ) == 0) {
         rc = yEXEC_whoami (&x_pid, &x_ppid, &x_uid, NULL, &x_root, x_who, 'n', NULL, NULL, NULL);
         getcwd (x_home, LEN_HUND);
         rc = ystrlproj (x_home, x_proj);
         f  = fopen ("/tmp/unit_whoami.txt", "wt");
         if (f != NULL) {
            fprintf (f, "pid  : %d\n" , x_pid);
            fprintf (f, "ppid : %d\n" , x_ppid);
            fprintf (f, "uid  : %d\n" , x_uid);
            fprintf (f, "root : %c\n" , x_root);
            fprintf (f, "who  : %s\n" , x_who);
            fprintf (f, "home : %s\n" , x_home);
            fprintf (f, "name : %s\n" , x_proj);
            fflush  (f);
            fclose  (f);
         }
         rc = chmod ("/tmp/unit_whoami.txt", 0666);
         return 0;
      }
   }
   /*---(daemonize)----------------------*/
   if (x_daemon == 'y')  yEXEC_daemon (-1, (int *) 0);
   yEXEC_signal (x_bullet, x_inter, x_child, incomming, "unit");
   /*---(name-change)--------------------*/
   x_len = strlen (x_altname);
   if (x_len > 0) {
      rc = yEXEC_maxname (a_argc, a_argv, &x_max);
      ystrlcat (x_altname, YSTR_EMPTY, LEN_HUND);
      ystrlcpy (a_argv [0], x_altname, x_max);
      sync ();
   }
   /*---(loop)---------------------------*/
   f = fopen ("/tmp/unit_ticker.txt", "wt");
   i = 0;
   if (x_cpu == '-') {
      while (i++ < x_loop) {
         if (f != NULL) {
            fprintf (f, ".");
            fflush  (f);
         }
         if (x_cycle == 'y') {
            sprintf (x_secs, "%s [%2ds]", P_NAMESAKE, i);
            strcpy (a_argv [0], x_secs);
         }
         sleep (1);
         switch (x_mem) {
         case 'L' :
            for (a = 0; a <  10 ; ++a)  b *= a;
            break;
         case 'M' :
            for (a = 0; a < 100 ; ++a)  malloc (1024);
            break;
         case 'H' :
            for (a = 0; a < 200 ; ++a)  b *= a;
            break;
         }
      }
   } else {
      alarm (x_loop);
      while (1) {
         switch (x_cpu) {
         case 'L' :
            ++a;
            usleep (100);
            break;
         case 'M' :
            for (a = 0; a < 150 ; ++a)  b *= a;
            usleep (20);
            break;
         case 'H' :
            for (a = 0; a < 3000; ++a)  b *= a;
            usleep (2);
            break;
         }
      }
   }
   if (f != NULL)  fclose (f);
   chmod ("/tmp/unit_ticker.txt", 0666);
   if (x_hup >  1) {
      /*> printf ("hupping %d\n", x_hup);                                             <*/
      kill (x_hup, 1);
   }
   /*---(complete)-----------------------*/
   return rc;
}


