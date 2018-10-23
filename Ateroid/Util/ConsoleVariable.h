#pragma once

#include "PlayerPrefs.h"

namespace ASTEROID_NAMESPACE
{
    class BaseConsoleVariable
    {
    public:
        typedef std::shared_ptr<BaseConsoleVariable> SharedPtrType;

    public:
        BaseConsoleVariable(const std::string& name, bool isPersistent)
            : m_Name(name), m_IsPersistent(isPersistent)
        {
        }
        virtual ~BaseConsoleVariable() = 0 {}

        virtual void ReadValue(std::istream& is) = 0;
        virtual void WriteValue(std::ostream& os) = 0;

        const std::string& Name() const { return m_Name; }
        bool IsPersistent() const { return m_IsPersistent; }

        virtual void OnUnregister() const = 0;

    protected:
        std::string m_Name;
        bool        m_IsPersistent;
    };


    class ConsoleVariableManager
    {
    public:
        static void Register(BaseConsoleVariable::SharedPtrType pVar)
        {
            ASTEROID_ASSERT_F(_Variables.find(pVar->Name()) == _Variables.end(), 
                "There is already a variable with name \"%s\" registered.", pVar->Name());
            _Variables.insert(std::make_pair(pVar->Name(), pVar));
        }

        static void Unregister(const std::string& name)
        {
            auto it = _Variables.find(name);
            if (it != _Variables.end())
            {
                it->second->OnUnregister();
                _Variables.erase(it);
            }
        }

        static bool HasVariable(const std::string& name)
        {
            return _Variables.find(name) != _Variables.end();
        }

        static BaseConsoleVariable::SharedPtrType FindVariable(const std::string& name)
        {
            BaseConsoleVariable::SharedPtrType pVar = nullptr;
            auto it = _Variables.find(name);
            if (it != _Variables.end())
                pVar = it->second;
            return pVar;
        }

        static size_t VariablesCount() { return _Variables.size(); }

        static void UnregisterAllVariables() 
        { 
            for (auto& it : _Variables)
                it.second->OnUnregister();
            _Variables.clear(); 
        }

    private:
        using VariableMap = std::unordered_map<std::string, BaseConsoleVariable::SharedPtrType>;
        static VariableMap  _Variables;
    };


    template<typename T>
    class ConsoleVariable : public BaseConsoleVariable
    {
    public:
        typedef std::shared_ptr<ConsoleVariable<T>> SharedPtrType;

    public:
        

        static SharedPtrType Create(const std::string& name, bool isPersistent, const T& defaultValue)
        {
            BaseConsoleVariable::SharedPtrType regVar = ConsoleVariableManager::FindVariable(name);
            if (regVar == nullptr)
            {
                SharedPtrType var = std::make_shared<ConsoleVariable<T>>(name, isPersistent, defaultValue);
                ConsoleVariableManager::Register(var);
                var->OnRegister(defaultValue);
                return var;
            }
            else
            {
                SharedPtrType var = nullptr;
                if (regVar->IsPersistent() == isPersistent)
                {
                    var = std::dynamic_pointer_cast<ConsoleVariable<T>>(regVar);
                    if (var == nullptr)
                        ASTEROID_LOG_ERROR_F("There is already a variable with name \"%s\" registered but it's a different type.", name);
                }
                else
                {
                    ASTEROID_LOG_ERROR_F("There is already a variable with name \"%s\" registered but it has different persistency property.", name);
                }
                return var;
            }
        }

        ConsoleVariable(const std::string& name, bool isPersistent, const T& value)
            : BaseConsoleVariable(name, isPersistent), m_Value(value)
        {
        }

        virtual ~ConsoleVariable() 
        {
        }

        operator T&() { return m_Value; }

        virtual void ReadValue(std::istream& is) override { is >> m_Value; }
        virtual void WriteValue(std::ostream& os) override { os << m_Value; }

        void OnRegister(const T& defaultValue) 
        {
            if (m_IsPersistent)
                m_Value = PlayerPrefs::GetValue<T>(m_Name, defaultValue); 
        }
        virtual void OnUnregister() const override 
        {
            if (m_IsPersistent)
                PlayerPrefs::SetValue<T>(m_Name, m_Value); 
        }

    private:
        T m_Value;
    };


    


    
}