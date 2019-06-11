#ifndef __LINUX_SPINLOCK_H__
#define __LINUX_SPINLOCK_H__

#include <pkgconf/system.h>

#ifdef CYGPKG_KERNEL

#include <cyg/kernel/kapi.h>

typedef cyg_mutex_t spinlock_t;
#define SPIN_LOCK_UNLOCKED (spinlock_t) { 0 }

#define spin_lock_init(lock)             \
CYG_MACRO_START;                         \
cyg_mutex_init(lock);    \
CYG_MACRO_END

#define spin_lock(lock)                  \
CYG_MACRO_START;                         \
cyg_mutex_lock(lock);                    \
CYG_MACRO_END

#define spin_unlock(lock)                \
CYG_MACRO_START;                         \
cyg_mutex_unlock(lock);                  \
CYG_MACRO_END

#define spin_lock_bh(lock)   spin_lock_bh(lock)
#define spin_unlock_bh(lock) spin_unlock_bh(lock)

#else
typedef struct { } spinlock_t;
#define SPIN_LOCK_UNLOCKED (spinlock_t) { }

#define spin_lock_init(lock)             \
CYG_MACRO_START;                         \
CYG_UNUSED_PARAM(spinlock_t *, lock);    \
CYG_MACRO_END

#define spin_lock(lock)                  \
CYG_MACRO_START;                         \
CYG_UNUSED_PARAM(spinlock_t *, lock);    \
CYG_MACRO_END

#define spin_unlock(lock)                \
CYG_MACRO_START;                         \
CYG_UNUSED_PARAM(spinlock_t *, lock);    \
CYG_MACRO_END

#define spin_lock_bh(lock)               \
CYG_MACRO_START;                         \
CYG_UNUSED_PARAM(spinlock_t *, lock);    \
CYG_MACRO_END

#define spin_unlock_bh(lock)             \
CYG_MACRO_START;                         \
CYG_UNUSED_PARAM(spinlock_t *, lock);    \
CYG_MACRO_END

#endif

#endif /* __LINUX_SPINLOCK_H__ */
