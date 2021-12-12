/*
 * =====================================================================================
 *
 *       Filename:  builtins.h
 *
 *    Description:  builtins shell commands 
 *
 *        Version:  1.0
 *        Created:  12/09/21 09:32:34
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Pedro Botsaris
 *   Organization:  
 *
 * =====================================================================================
 */


#ifndef BUILTINS_H
#define BUILTINS_H

/* ENV OPTIONS */
#define IGNORE_SINGLE  "-i"
#define IGNORE_DOUBLE  "--ignore-environment"
#define UNSET_SINGLE   "-u"
#define UNSET_DOUBLE   "--unset"
#define NILL_SINGLE    "-0"
#define NILL_DOUBLE    "--null"
#define CDIR_SINGLE    "-C"
#define CDIR_DOUBLE    "--chdir"


/* CD SHORTCUTS */
#define HOME "~"
#define PREVPWD "-"

typedef enum envflags{
  INIT,                /* 0 */
  IGNORE,              /* 1 */
  UNSET,               /* 2 */
  NILL,                /* 3 */
  CDIR,                /* 4 */
  ERR,                 /* 5 */
}envflag_t;

#endif
