#pragma once

#include <phnt/phnt_windows.h>
#include <phnt/phnt.h>

namespace wdm
{
    class srw_exclusive_lock : RTL_SRWLOCK
    {
    public:
        srw_exclusive_lock() noexcept
        {
            RtlInitializeSRWLock(this);
        }

        bool try_lock() noexcept
        {
            return RtlTryAcquireSRWLockExclusive(this);
        }

        void lock() noexcept
        {
            RtlAcquireSRWLockExclusive(this);
        }

        void unlock() noexcept
        {
            RtlReleaseSRWLockExclusive(this);
        }
    };
}
