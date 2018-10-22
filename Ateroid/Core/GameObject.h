#pragma once

#include "Object.h"

namespace ASTEROID_NAMESPACE
{
    class GameObject : public Object
    {
    public:
        const std::string& Name() const { return m_Name; }
        std::string& Name() { return m_Name; }

    private:
        std::string m_Name;
    };
}
