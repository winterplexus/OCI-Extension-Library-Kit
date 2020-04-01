/*
**  @(#)changepassword.c
**
**  libociext - OCI client application to login and change password
**  ---------------------------------------------------------------
**
**  copyright 2001-2020 Code Construct Systems (CCS)
*/
#include <locale.h>
#include <stdio.h>
#include <string.h>
#include "ociext.h"

/*
** Local function prototypes
*/
static int Logon(char **);
static int ChangePassword(char **);

/*
** Driver
*/
void main(int argc, char **argv) {
    /*
    ** Check command line arguments count; display usage message if required
    */
    if (argc < 5) {
        printf("usage: ociext-changepassword <user ID> <password> <password replacement> <sid>\n");
        return;
    }

    /*
    ** Set locale
    */
    setlocale(LC_ALL, "en-US");

    /*
    ** Test logon capability and change password
    */
    printf("test logon return code: %d\n", Logon(argv));
    printf("change password return code: %d\n", ChangePassword(argv));
}

/*
** Test logon capability
*/
static int Logon(char **argv) {
    int status = 0;
    char username[_MAX_USERNAME_LEN];
    char password[_MAX_PASSWORD_LEN];
    char sid[_MAX_DBNAME_LEN];

    /*
    ** Clear username, password, password (replacement) and SID
    */
    memset(username, 0, sizeof(username));
    memset(password, 0, sizeof(password));
    memset(sid, 0, sizeof(sid));

    /*
    ** Copy command line arguments to username, password, password (replacement) and SID
    */
    strncpy_s(username, sizeof(username), argv[1], strlen(argv[1]));
    strncpy_s(password, sizeof(password), argv[2], strlen(argv[2]));
    strncpy_s(sid, sizeof(sid), argv[4], strlen(argv[4]));

    /*
    ** Test logon capability to Oracle
    */
    status = OCIEXTTestOracleLogon(username, password, sid);
    return (status);
}

/*
** Change password
*/
static int ChangePassword(char **argv) {
    int status = 0;
    char username[_MAX_USERNAME_LEN];
    char password[_MAX_PASSWORD_LEN];
    char password_replacement[_MAX_PASSWORD_LEN];
    char sid[_MAX_DBNAME_LEN];

    /*
    ** Clear username, password, password (replacement) and SID
    */
    memset(username, 0, sizeof(username));
    memset(password, 0, sizeof(password));
    memset(password_replacement, 0, sizeof(password_replacement));
    memset(sid, 0, sizeof(sid));

    /*
    ** Copy command line arguments to username, password, password (replacement) and SID
    */
    strncpy_s(username, sizeof(username), argv[1], strlen(argv[1]));
    strncpy_s(password, sizeof(password), argv[2], strlen(argv[2]));
    strncpy_s(password_replacement, sizeof(password_replacement), argv[3], strlen(argv[3]));
    strncpy_s(sid, sizeof(sid), argv[4], strlen(argv[4]));

    /*
    ** Change Oracle password
    */
    status = OCIEXTChangeOraclePassword(username, password, password_replacement, sid);
    return (status);
}