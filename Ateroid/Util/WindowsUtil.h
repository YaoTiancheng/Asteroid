#pragma once

#include "String.h"

namespace ASTEROID_NAMESPACE
{
    /**
     *  Useful utility functions to simplify some common procedual needed
     *  in windows application development.
     */
    class WindowsUtil
    {
    public:
        ASTEROID_NO_DEFAULT_CTOR(WindowsUtil)
        ASTEROID_NON_COPYABLE(WindowsUtil)

        /**
         *  Get a translated message of the last error code.
         */
        static String GetLastErrorString();
    };
}