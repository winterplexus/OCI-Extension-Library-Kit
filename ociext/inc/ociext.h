/*
**  @(#)ociext.h
**
**  libociext - OCI extension functions
**  -----------------------------------
**
**  copyright 2001-2020 Code Construct Systems (CCS)
*/
#ifndef _OCIEXT_H
#define _OCIEXT_H

#include <oci.h>

/*
** Define maximum user name size
*/
#ifndef _MAX_USERNAME_LEN
#define _MAX_USERNAME_LEN 32
#else
#error  _MAX_USERNAME_LEN is defined in another include file
#endif

/*
** Define maximum user password size
*/
#ifndef _MAX_PASSWORD_LEN
#define _MAX_PASSWORD_LEN 32
#else
#error  _MAX_PASSWORD_LEN is defined in another include file
#endif

/*
** Define maximum database name or SID size
*/
#ifndef _MAX_DBNAME_LEN
#define _MAX_DBNAME_LEN 16
#else
#error  _MAX_DBNAME_LENis defined in another include file
#endif

/*
** Define message title
*/
#ifndef _MSG_TITLE_HEADING
#define _MSG_TITLE_HEADING "Database Error"
#else
#error  _MSG_TITLE_HEADING defined in another include file
#endif

/*
** Define user name error message
*/
#ifndef _MSG_ERROR_USERNAME
#define _MSG_ERROR_USERNAME "Username is missing."
#else
#error  _MSG_ERROR_USERNAME defined in another include file
#endif

/*
** Define user password error message
*/
#ifndef _MSG_ERROR_PASSWORD_1
#define _MSG_ERROR_PASSWORD_1 "Password is missing."
#else
#error  _MSG_ERROR_PASSWORD_1 defined in another include file
#endif

/*
** Define user replacement password error message
*/
#ifndef _MSG_ERROR_PASSWORD_2
#define _MSG_ERROR_PASSWORD_2 "Replacement password is missing."
#else
#error  _MSG_ERROR_PASSWORD_2 defined in another include file
#endif

/*
** Define database name or SID error message
*/
#ifndef _MSG_ERROR_DBNAME
#define _MSG_ERROR_DBNAME "Oracle database name or SID is missing."
#else
#error  _MSG_ERROR_DBNAME defined in another include file
#endif

/*
** Define OCI logon parameters structure and type
*/
typedef struct OCILogonParms {
        char username[_MAX_USERNAME_LEN + 1];
        char password[_MAX_PASSWORD_LEN + 1];
        char password_rep[_MAX_PASSWORD_LEN + 1];
        char dbname[_MAX_DBNAME_LEN + 1];
} OCILOGONPARMS;

/*
** Function prototypes
*/
extern int OCIEXTTestOracleLogon(const char *, const char *, const char *);
extern int OCIEXTChangeOraclePassword(const char *, const char *, const char *, const char *);
extern int OCIEXTChangeExpiredOraclePassword(const char *, const char *, const char *, const char *);
extern ub4 OCIEXTErrorHandler(OCIError *, sword);
extern ub4 OCIEXTDisplayErrorMessage(const char *, ...);

#endif /* _OCIEXT_H */