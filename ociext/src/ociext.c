/*
**  @(#)ociext.c
**
**  libociext - OCI extension functions
**  -----------------------------------
**
**  copyright 2001-2020 Code Construct Systems (CCS)
*/
#include <stdio.h>
#include <string.h>
#include <windows.h>
#include <oci.h>
#include "common.h"
#include "ociext.h"

/*
** OCI extension error handler
*/
ub4 OCIEXTErrorHandler(OCIError* errhp, sword status) {
    text buffer[_MAX_BUFFER_SIZE];
    ub4 code = 0;

    /*
    ** Process status code
    */
    switch (status) {
        case OCI_SUCCESS:
             break;
        case OCI_SUCCESS_WITH_INFO:
             OCIEXTDisplayErrorMessage("OCI status: OCI_SUCCESS_WITH_INFO");
             break;
        case OCI_NEED_DATA:
             OCIEXTDisplayErrorMessage("OCI status: OCI_NEED_DATA");
             break;
        case OCI_NO_DATA:
             break;
        case OCI_ERROR:
             OCIErrorGet((dvoid*)errhp, (ub4)1, (text*)NULL, (sb4*)&code, (text*)buffer, (ub4)sizeof(buffer), (ub4)OCI_HTYPE_ERROR);
             OCIEXTDisplayErrorMessage("Oracle reports -> %s", buffer);
             break;
        case OCI_INVALID_HANDLE:
             OCIEXTDisplayErrorMessage("OCI status: OCI_INVALID_HANDLE");
             break;
        case OCI_STILL_EXECUTING:
             OCIEXTDisplayErrorMessage("OCI status: OCI_STILL_EXECUTING");
             break;
        case OCI_CONTINUE:
             OCIEXTDisplayErrorMessage("OCI status: OCI_CONTINUE");
             break;
        default:
            break;
    }

    /*
    ** Return error code
    */
    return (code);
}

/*
** OCI extension display error message
*/
ub4 OCIEXTDisplayErrorMessage(const char* format, ...) {
    char* title = _MSG_TITLE_HEADING;
    char message[_MAX_BUFFER_SIZE];
    ub4 vc = 0;
    va_list vargs;

    /*
    ** Format variable arguments and store message string
    */
    va_start(vargs, format);
    vc = (ub4)vsprintf(message, format, vargs);

#ifdef WIN32
    /*
    ** Display message box
    */
    if (vc) {
        MessageBox((HWND)NULL, (LPCWSTR)message, (LPCWSTR)title, MB_OK | MB_ICONINFORMATION);
    }
    else {
        MessageBox((HWND)NULL, L"Buffer overflow while printing this message.", L"Fatal Error", MB_OK | MB_ICONEXCLAMATION);
    }
#else
    /*
    ** Display message text line
    */
    if (vc) {
        printf("%s\n%s\n", title, message);
    }
    else {
        printf("Buffer overflow while printing this message.");
    }
#endif
    /*
    ** Return variable string formatter code
    */
    return (vc);
}