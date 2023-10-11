/*
**  @(#)ocilogon.c
**
**  libociext - OCI extension logon functions
**  -----------------------------------------
**
**  Copyright (c) Wiregrass Code Technology 2001-2024
*/
#include <stdio.h>
#include <string.h>
#include <windows.h>
#include <oci.h>
#include "common.h"
#include "ociext.h"

/*
** Local function prototypes
*/
static BOOL CheckLogonParameters(OCILOGONPARMS *);
static BOOL CheckStringLength(char *, size_t);

/*
** Test logon capability to Oracle
*/
int OCIEXTTestOracleLogon(const char *username, const char *password, const char *dbname) {
    OCIEnv *envhp;
    OCIError *errhp;
    OCISvcCtx *svchp;
    sword status = 0;
    OCILOGONPARMS lparms;
    ub4 code = 0;

    /*
    ** Store passed arguments as logon parameters
    */
    strncpy(lparms.username, username, _MAX_USERNAME_LEN);
    strncpy(lparms.password, password, _MAX_PASSWORD_LEN);
    strncpy(lparms.dbname, dbname, _MAX_DBNAME_LEN);

    /*
    ** Check logon parameters
    */
    if (CheckLogonParameters(&lparms) == FALSE)
        return (EXIT_FAILURE);

    /*
    ** Create and initialize OCI environment
    */
    status = OCIEnvCreate((OCIEnv **)&envhp, \
                          (ub4)OCI_DEFAULT, (dvoid *)0, \
                          (dvoid * (*)(dvoid *, size_t))0, \
                          (dvoid * (*)(dvoid *, dvoid *, size_t))0, \
                          (void (*)(dvoid *, dvoid *))0, \
                          (size_t)0, (dvoid **)0);

    /*
    ** Exit if unable to create and initialize OCI environment
    */
    if (status != 0) {
        return (-1);
    }

    /*
    ** Allocate and initialize the environment handle
    */
    OCIEnvInit((OCIEnv **)&envhp, (ub4)OCI_DEFAULT, (size_t)0, (dvoid **)0);

    /*
    ** Allocate error and service context handles
    */
    OCIHandleAlloc((dvoid *)envhp, (dvoid **)&errhp, (ub4)OCI_HTYPE_ERROR, (size_t)0, (dvoid **)0);
    OCIHandleAlloc((dvoid *)envhp, (dvoid **)&svchp, (ub4)OCI_HTYPE_SVCCTX, (size_t)0, (dvoid **)0);

    /*
    ** Logon to Oracle using a single session connection
    */
    status = OCILogon((OCIEnv *)envhp, \
                      (OCIError *)errhp, \
                      (OCISvcCtx **)&svchp, \
                      (CONST text *) lparms.username, \
                      (ub4)strlen(lparms.username), \
                      (CONST text *) lparms.password, \
                      (ub4)strlen(lparms.password), \
                      (CONST text *) lparms.dbname, \
                      (ub4)strlen(lparms.dbname));

    /*
    ** Exit if unable to logon
    */
    code = OCIEXTErrorHandler(errhp, status);
    if (status != OCI_SUCCESS) {
        return (code);
    }

    /*
    ** Logoff from Oracle
    */
    OCIEXTErrorHandler(errhp, OCILogoff((dvoid *)svchp, (dvoid *)errhp));

    /*
    ** Free the service context, error and environment handles
    */
    OCIHandleFree((dvoid *)svchp, (ub4)OCI_HTYPE_SVCCTX);
    OCIHandleFree((dvoid *)errhp, (ub4)OCI_HTYPE_ERROR);
    OCIHandleFree((dvoid *)envhp, (ub4)OCI_HTYPE_ENV);

    /*
    ** Return condition code of logon status
    */
    if (status != OCI_SUCCESS) {
        return (code);
    }
    else {
        return (EXIT_SUCCESS);
    }
}

/*
** Check logon parameters
*/
static BOOL CheckLogonParameters(OCILOGONPARMS *lparms) {
    if (!CheckStringLength(lparms->username, _MAX_USERNAME_LEN)) {
        OCIEXTDisplayErrorMessage(_MSG_ERROR_USERNAME);
        return (FALSE);
    }
    if (!CheckStringLength(lparms->password, _MAX_PASSWORD_LEN)) {
        OCIEXTDisplayErrorMessage(_MSG_ERROR_PASSWORD_1);
        return (FALSE);
    }
    if (!CheckStringLength(lparms->dbname, _MAX_DBNAME_LEN)) {
        OCIEXTDisplayErrorMessage(_MSG_ERROR_DBNAME);
        return (FALSE);
    }
    return (TRUE);
}

/*
** Check string length
*/
static BOOL CheckStringLength(char *s, size_t len) {
    size_t i;
    BOOL terminator = FALSE;

    for (i = 0; i < len; i++) {
        if (s[i] == (char)NULL) {
            terminator = TRUE;
        }
    }

    if (terminator) {
        return (TRUE);
    }
    else {
        return (FALSE);
    }
}
