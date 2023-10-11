/*
**  @(#)ocipassword.c
**
**  libociext - OCI extension password functions
**  --------------------------------------------
**
**  Copyright (c) Wiregrass Code Technology 2001-2024
*/
#include <stdio.h>
#include <string.h>
#include <windows.h>
#include <oci.h>
#include <oratypes.h>
#include "common.h"
#include "ociext.h"

/*
** Local function prototypes
*/
static BOOL CheckLogonParameters(OCILOGONPARMS *);
static BOOL CheckStringLength(char *, size_t);

/*
** Change Oracle password
*/
int OCIEXTChangeOraclePassword(const char *username, const char *password, const char *password_rep, const char *dbname) {
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
    strncpy(lparms.password_rep, password_rep, _MAX_PASSWORD_LEN);
    strncpy(lparms.dbname, dbname, _MAX_DBNAME_LEN);

    /*
    ** Check logon parameters
    */
    if (CheckLogonParameters(&lparms) == FALSE) {
        return (EXIT_FAILURE);
    }

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
                      (CONST text *)lparms.username, \
                      (ub4)strlen(lparms.username), \
                      (CONST text *)lparms.password, \
                      (ub4)strlen(lparms.password), \
                      (CONST text *)lparms.dbname, \
                      (ub4)strlen(lparms.dbname));

    /*
    ** Get return condition code using error handle and status
    */
    code = OCIEXTErrorHandler(errhp, status);

    /*
    ** Exit if unable to logon
    */
    if (status != OCI_SUCCESS) {
        return (code);
    }

    /*
    ** Change password
    */
    status = OCIPasswordChange((OCISvcCtx *)svchp, \
                               (OCIError *)errhp, \
                               (CONST text *)lparms.username, \
                               (ub4)strlen(lparms.username), \
                               (CONST text *)lparms.password, \
                               (ub4)strlen(lparms.password), \
                               (CONST text *)lparms.password_rep, \
                               (ub4)strlen(lparms.password_rep), \
                               (ub4)OCI_DEFAULT);

    /*
    ** Get return condition code of change password status
    */
    code = OCIEXTErrorHandler(errhp, status);

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
    ** Return condition code of change password status
    */
    if (status != OCI_SUCCESS) {
        return (code);
    }
    else {
        return (EXIT_SUCCESS);
    }
}

/*
** Change expired Oracle password
*/
int OCIEXTChangeExpiredOraclePassword(const char *username, const char *password, const char *password_rep, const char *dbname) {
    OCIEnv *envhp;
    OCIError *errhp;
    OCIServer *srvhp;
    OCISvcCtx *svchp;
    OCISession *usrhp;
    sword status = 0;
    OCILOGONPARMS lparms;
    ub4 code = 0;

    /*
    ** Store passed arguments as logon parameters
    */
    strncpy(lparms.username, username, _MAX_USERNAME_LEN);
    strncpy(lparms.password, password, _MAX_PASSWORD_LEN);
    strncpy(lparms.password_rep, password_rep, _MAX_PASSWORD_LEN);
    strncpy(lparms.dbname, dbname, _MAX_DBNAME_LEN);

    /*
    ** Check logon parameters
    */
    if (CheckLogonParameters(&lparms) == FALSE) {
        return (EXIT_FAILURE);
    }

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
    ** Allocate error and server context handles
    */
    OCIHandleAlloc((dvoid *)envhp, (dvoid **)&errhp, (ub4)OCI_HTYPE_ERROR, (size_t)0, (dvoid **)0);
    OCIHandleAlloc((dvoid *)envhp, (dvoid **)&srvhp, (ub4)OCI_HTYPE_SERVER, (size_t)0, (dvoid **)0);

    /*
    ** Attach to the server
    */
    OCIServerAttach((OCIServer *)srvhp, (OCIError *)errhp, (text *)lparms.dbname, (sb4)strlen(lparms.dbname), (ub4)OCI_DEFAULT);

    /*
    ** Allocate the service context handle and set the server attribute to the server handle
    */
    OCIHandleAlloc((dvoid *)envhp, (dvoid **)&svchp, (ub4)OCI_HTYPE_SVCCTX, (size_t)0, (dvoid **)0);
    OCIAttrSet((dvoid *)svchp, (ub4)OCI_HTYPE_SVCCTX, (dvoid *)srvhp, (ub4)0, (ub4)OCI_ATTR_SERVER, (OCIError *)errhp);

    /*
    ** Allocate the session handle and add it to the session attribute of the service context
    */
    OCIHandleAlloc((dvoid *)envhp, (dvoid **)&usrhp, (ub4)OCI_HTYPE_SESSION, (size_t)0, (dvoid **)0);
    OCIAttrSet((dvoid *)svchp, (ub4)OCI_HTYPE_SVCCTX, (dvoid *)usrhp, (ub4)0, (ub4)OCI_ATTR_SESSION, (OCIError *)errhp);

    /*
    ** Change password after Oracle establishes a session
    */
    status = OCIPasswordChange((OCISvcCtx *)svchp, \
                               (OCIError *)errhp, \
                               (CONST text *)lparms.username, \
                               (ub4)strlen(lparms.username), \
                               (CONST text *)lparms.password, \
                               (ub4)strlen(lparms.password), \
                               (CONST text *)lparms.password_rep, \
                               (ub4)strlen(lparms.password_rep), \
                               (ub4)OCI_AUTH);

    /*
    ** Get return condition code using error handle and status
    */
    code = OCIEXTErrorHandler(errhp, status);

    /*
    ** End the session and detach from the server
    */
    OCISessionEnd((OCISvcCtx *)svchp, (OCIError *)errhp, (OCISession *)usrhp, (ub4)OCI_DEFAULT);
    OCIServerDetach((OCIServer *)srvhp, (OCIError *)errhp, (ub4)OCI_DEFAULT);

    /*
    ** Free the server, service context and session handles
    */
    OCIEXTErrorHandler(errhp, OCIHandleFree((dvoid *)srvhp, (ub4)OCI_HTYPE_SERVER));
    OCIEXTErrorHandler(errhp, OCIHandleFree((dvoid *)svchp, (ub4)OCI_HTYPE_SVCCTX));
    OCIEXTErrorHandler(errhp, OCIHandleFree((dvoid *)usrhp, (ub4)OCI_HTYPE_SESSION));

    /*
    ** Free error and environment handles
    */
    OCIHandleFree((dvoid *)errhp, (ub4)OCI_HTYPE_ERROR);
    OCIHandleFree((dvoid *)envhp, (ub4)OCI_HTYPE_ENV);

    /*
    ** Return condition of change password status
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
    if (!CheckStringLength(lparms->password_rep, _MAX_PASSWORD_LEN)) {
        OCIEXTDisplayErrorMessage(_MSG_ERROR_PASSWORD_2);
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
