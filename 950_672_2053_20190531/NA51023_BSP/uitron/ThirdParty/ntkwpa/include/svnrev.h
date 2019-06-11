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
#define SVN_REV		    $WCREV$
#define SVN_REVSTR	    "$WCREV$$WCMODS?M:$"
#define SVN_REVDATE	    "$WCDATE$"
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
