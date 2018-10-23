#pragma once

#include "PlayerPrefs.h"

namespace ASTEROID_NAMESPACE
{
    class BaseConsoleVariable
    {
    public:
        virtual ~BaseConsoleVariable() = 0 {}

        virtual void SaveAs(const std::string& name) const = 0;
        virtual void LoadAs(const std::string& name) = 0;

    private:
        virtual void ReadValue(std::istream& is) = 0;
        virtual void WriteValue(std::ostream& os) = 0;
    };


    template<typename T>
    class ConsoleVariable : public BaseConsoleVariable
    {
    public:
        explicit ConsoleVariable(const T& value) : m_Value(value) {}
        virtual ~ConsoleVariable() {}

        virtual void SaveAs(const std::string& name) const override { PlayerPrefs::SetValue<T>(name, m_Value); }
        virtual void LoadAs(const std::string& name) override { m_Value = PlayerPrefs::GetValue<T>(name, m_Value); }

        T& operator T() { return m_Value; }

    private:
        virtual void ReadValue(std::istream& is) override { is >> m_Value; }
        virtual void WriteValue(std::ostream& os) override { os << m_Value; }

    private:
        T m_Value;
    };


    template<typename T>
    class SharedConsoleVariable
    {
    public:

        struct ConsoleVariableContainer
        {
            BaseConsoleVariable*    var;
            std::string             name;
            bool                    isPersistent;
            uint32_t                refCount;

            SharedConsoleVariable(BaseConsoleVariable* var, const std::string& name, bool isPersistent)
                : var(var), name(name), isPersistent(isPersistent), refCount(0)
            {
            }

            ~ConsoleVariableContainer()
            {
                ASTEROID_DELETE var;
            }
        };

        static SharedConsoleVariable Get(const std::string& name, const T& defaultValue, bool isPersistent)
        {
            auto it = _Variables.find(name);
            if (it == _Variables.end())
            {
                ConsoleVariableContainer container;
                container.var = ASTEROID_NEW ConsoleVariable<T>(defaultValue);
                container.name = name;
                container.isPersistent = isPersistent;
                container.refCount = 0;
                _Variables.insert(std::make_pair(name, container));

                if (isPersistent)
                    container.var->LoadAs(name);
            }
            else
            {
            }
        }

    private:
        explicit SharedConsoleVariable(ConsoleVariable<T>& var, uint32_t* refCount)
            : m_Var(var), m_RefCount(refCount)
        {
        }

    private:
        using VariableMap = std::unordered_map<std::string, ConsoleVariableContainer>;
        static VariableMap  _Variables;
        uint32_t*           m_RefCount;
        ConsoleVariable<T>& m_Var;
    };


    


    
}