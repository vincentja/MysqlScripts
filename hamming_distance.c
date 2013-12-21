#ifdef STANDARD
/* STANDARD is defined, don't use any mysql functions */
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#ifdef __WIN__
typedef unsigned __int64 ulonglong; /* Microsofts 64 bit types */
typedef __int64 longlong;
#else
typedef unsigned long long ulonglong;
typedef long long longlong;
#endif /*__WIN__*/
#else
#include "my_global.h"
#include "my_sys.h"
#if defined(MYSQL_SERVER)
#include <m_string.h> /* To get strmov() */
#else
/* when compiled as standalone */
#include <string.h>
#define strmov(a,b) stpcpy(a,b)
#define bzero(a,b) memset(a,0,b)
#endif
#endif
#include <mysql.h>
#include <ctype.h>

#if defined( _MSC_VER) || defined(_BORLANDC_)
typedef unsigned _uint64 ulong64;
typedef signed _int64 long64;
#else
typedef unsigned long long ulong64;
typedef signed long long long64;
#endif


#ifdef _WIN32
/* inet_aton needs winsock library */
#pragma comment(lib, "ws2_32")
#endif
#ifdef HAVE_DLOPEN

#if !defined(HAVE_GETHOSTBYADDR_R) || !defined(HAVE_SOLARIS_STYLE_GETHOST)
static pthread_mutex_t LOCK_hostname;
#endif

/* These must be right or mysqld will not find the symbol! */

my_bool had_init( UDF_INIT *initid, UDF_ARGS *args, char *message );
int had( UDF_INIT *initid __attribute__((unused)), UDF_ARGS *args, char *is_null, char *error );
void had_deinit( UDF_INIT *initid );


my_bool had_init( UDF_INIT *initid, UDF_ARGS *args, char *message )
{
if( args->arg_count != 2 )
{
strcpy( message, "Wrong number of arguments to HAMMING_DISTANCE; accepts exactly two arguments." );
return 1;
}
/*
if( ((args->arg_type[0] != INT_RESULT) && (args->args[0] != NULL)) || ((args->arg_type[1] != INT_RESULT) && (args->args[0] != NULL)) )
{
strcpy( message, "One or both arguments to HAMMING_DISTANCE is not an integer." );
return 1;
}*/
initid->maybe_null = 1;
return 0;
}
void had_deinit( UDF_INIT *initid __attribute__((unused)) )
{
return;
}

int had( UDF_INIT *initid __attribute__((unused)), UDF_ARGS *args, char *is_null, char *error ){
if( (args->args[0] == NULL) || (args->args[1] == NULL) )
{
*is_null = 1;
return 0;
}
ulong64 hash1 = *((ulong64*) args->args[0]);
ulong64 hash2 = *((ulong64*) args->args[1]);
 /*printf("%llu",hash1);
    printf("%llu",hash2); */
   ulong64 x = hash1^hash2;
   /*printf("%llu",x); */
    const ulong64 m1  = 0x5555555555555555ULL;
    const ulong64 m2  = 0x3333333333333333ULL;
    const ulong64 h01 = 0x0101010101010101ULL;
    const ulong64 m4  = 0x0f0f0f0f0f0f0f0fULL;
    x -= (x >> 1) & m1;
    x = (x & m2) + ((x >> 2) & m2);
    x = (x + (x >> 4)) & m4;
    return (x * h01)>>56;

}

#endif /* HAVE_DLOPEN */
