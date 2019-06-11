/* 
 * $Revision: $
 * $Id: $
 */

#ifndef _SVNREV_H
#define _SVNREV_H

#ifdef __cplusplus
extern "C" {
#endif

#if MAKE_OPT_REV == 1
#define SVN_REV		    6551
#define SVN_REVSTR	    "6551M"
#define SVN_REVDATE	    "2018/04/11 13:58:47"
#define BUILD_DATE   	__DATE__" "__TIME__
#else
#define SVN_REV		    00
#define SVN_REVSTR	    "00M"
#define SVN_REVDATE	    "2000-01-01"
#define BUILD_DATE   	__DATE__" "__TIME__
#endif

#ifdef __cplusplus
}
#endif

#endif /* SVN_REV_H */
