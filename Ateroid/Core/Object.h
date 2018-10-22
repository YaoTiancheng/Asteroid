#pragma once

#include "ObjectInstanceID.h"

namespace ASTEROID_NAMESPACE
{
    class Object
    {
    public:
        typedef ObjectInstanceID InstanceID;

    public:
        Object();
        virtual ~Object();

        ASTEROID_NON_COPYABLE(Object)

        InstanceID InstanceId() const { return m_InstanceId; }

    private:
        InstanceID m_InstanceId;
    };
}