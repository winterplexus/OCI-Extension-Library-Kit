/*
**  @(#)common.h
**
**  libociext - common definitions
**  ------------------------------
**
**  copyright 2001-2020 Code Construct Systems (CCS)
*/
#ifndef _COMMON_H
#define _COMMON_H

/*
** Define maximum buffer size
*/
#ifndef _MAX_BUFFER_SIZE
#define _MAX_BUFFER_SIZE 512
#else
#error  _MAX_BUFFER_SIZ is defined in another include file
#endif

/*
** Undefine TRUE and FALSE if defined in another include file
*/
#ifdef TRUE
#undef TRUE
#endif
#ifdef FALSE
#undef FALSE
#endif

/*
** Check if data type is defined in another include file
*/
#ifdef bool_c_t
#error bool_c_t is defined in another include file
#endif

/*
** Define Boolean data type
*/
typedef enum BooleanType {
        FALSE = 0, TRUE = 1
} bool_c_t;

#endif /* _COMMON_H */