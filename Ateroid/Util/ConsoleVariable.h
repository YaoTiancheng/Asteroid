#pragma once

#include "PlayerPrefs.h"
#include "Pointers.h"
#include "String.h"

namespace ASTEROID_NAMESPACE
{
    /**
     *  Base class for abstract console variable manipulation.
     */
    class BaseConsoleVariable
    {
        friend class ConsoleVariableManager;

    public:
        typedef std::shared_ptr<BaseConsoleVariable> SharedPtrType;

    public:
        BaseConsoleVariable(const String& name, bool isPersistent)
            : m_Name(name), m_IsPersistent(isPersistent)
        {
        }
        virtual ~BaseConsoleVariable() = 0 {}

        /** Read value from a input stream. */
        virtual void ReadValue(std::istream& is) = 0;
        /** Write value to an output stream. */
        virtual void WriteValue(std::ostream& os) = 0;

        const String&   Name() const { return m_Name; }
        bool            IsPersistent() const { return m_IsPersistent; }

    private:
        /** Called when this variable is unregistered from ConsoleVariableManager. */
        virtual void OnUnregister(PlayerPrefs* playerPrefs) const = 0;

    protected:
        String  m_Name;
        bool    m_IsPersistent;
    };


    /**
     *  A global manager class that manages all registered console variable in the game.
     *  Every console variable is registered, searched and unregistered by a unique name.
     */
    class ConsoleVariableManager
    {
    public:
        /**
         *  Create a singleton of ConsoleVariableManager
         */
        static ConsoleVariableManager* Create(PlayerPrefs* playerPrefs)
        {
            ASTEROID_ASSERT_F(_Singleton == nullptr, "There is already a ConsoleVariableManager singleton created.");
            _Singleton = ASTEROID_NEW ConsoleVariableManager(playerPrefs);
            return _Singleton;
        }

        /**
         *  Destroy the previous created ConsoleVariableManager singleton.
         */
        static void Destroy()
        {
            ASTEROID_DELETE(_Singleton);
            _Singleton = nullptr;
        }

        /**
         *  Get the previous created singleton.
         *  @return
         *      Instance of the singleton. nullptr if not created or destroyed.
         */
        static ConsoleVariableManager* Singleton() { return _Singleton; }

        /**
         *  Register a variable with a unique name.
         *  @param pVar
         *      The variable going to be registered.
         *  @remark
         *      The name of the variable must not be a name already used by another registered variable.
         *      Otherwise this function does nothing.
         */
        void Register(BaseConsoleVariable::SharedPtrType pVar)
        {
            ASTEROID_ASSERT_F(m_Variables.find(pVar->Name()) == m_Variables.end(), 
                "There is already a variable with name \"%s\" registered.", pVar->Name());
            m_Variables.insert(std::make_pair(pVar->Name(), pVar));
        }

        /**
         *  Unregister a variable with a unique name.
         *  @param name
         *      The variable's name.
         *  @remark
         *      If variable with this given name doesn't exist, this function does nothing.
         *      Otherwise, it will remove the variable from the global variable registry.\n
         *      Calling this function will cause a persistent variable to save its value, and any
         *      afterwards changes of it will not be saved.\n
         *      The variable being unregistered will not be searched globally through 
         *      ConsoleVariableManager any longer.
         */
        void Unregister(const String& name)
        {
            auto it = m_Variables.find(name);
            if (it != m_Variables.end())
            {
                it->second->OnUnregister(m_PlayerPrefs);
                m_Variables.erase(it);
            }
        }

        /**
         *  Check if a variable with the given name is registered.
         */
        bool HasVariable(const String& name) const
        {
            return m_Variables.find(name) != m_Variables.end();
        }

        /**
         *  Find a registered variable with given name.
         *  @return
         *      Registered variable with given name. nullptr if not found.
         */
        BaseConsoleVariable::SharedPtrType FindVariable(const String& name) const
        {
            BaseConsoleVariable::SharedPtrType pVar = nullptr;
            auto it = m_Variables.find(name);
            if (it != m_Variables.end())
                pVar = it->second;
            return pVar;
        }

        /**
         *  Numbers of currently registered variables.
         */
        size_t VariablesCount() const { return m_Variables.size(); }

        /**
         *  Unregister all registered variables.
         *  @remarks
         *      Calling this function is equivalent with calling ConsoleVariableManager::Unregister on each registered variable.
         */
        void UnregisterAllVariables() 
        { 
            for (auto& it : m_Variables)
                it.second->OnUnregister(m_PlayerPrefs);
            m_Variables.clear(); 
        }

        /**
         *  The PlayerPrefs instance this manager used to save persistent variables.
         */
        PlayerPrefs* GetPlayerPrefs() const { return m_PlayerPrefs; }

    private:
        explicit ConsoleVariableManager(PlayerPrefs* playerPrefs)
            : m_PlayerPrefs(playerPrefs)
        {
        }

    private:
        static ConsoleVariableManager* _Singleton;

        using VariableMap = UnorderedMap<String, BaseConsoleVariable::SharedPtrType>;
        VariableMap     m_Variables;
        PlayerPrefs*    m_PlayerPrefs;
    };


    /**
     *  Class template for class inherited from BaseConsoleVariable which wraps different type.
     */
    template<typename T>
    class ConsoleVariable : public BaseConsoleVariable
    {
    public:
        typedef SharedPtr<ConsoleVariable<T>> SharedPtrType;

    public:
        ASTEROID_NON_COPYABLE(ConsoleVariable)

        /**
         *  Create and register a console variable.
         *  @return
         *      Registered matching console variable if it is found. nullptr if name is registered by another console variable with
         *      different persistency or type. New console variable if the name is not used by any registered console variable.
         *  @remarks
         *      If the name, persistency and type is matching with a current registered console variable,
         *      calling this function will not create a new one but simply get the previously registered one.\n
         *      If the name is matching with a current registered console variable but persistency or type is different,
         *      calling this function will not create any console variable and returns nullptr.\n
         *      If the name doesn't match any registered console variables, calling this function will create a new console variable,
         *      assign it a default value and register it.
         */
        static SharedPtrType Create(const String& name, bool isPersistent = false, const T& defaultValue = T())
        {
            return Create(name, isPersistent, defaultValue, ConsoleVariableManager::Singleton());
        }

        /**
         *  ConsoleVariable<T>::Create implementation
         */
        static SharedPtrType Create(const String& name, bool isPersistent, const T& defaultValue, ConsoleVariableManager* consoleVariableManager)
        {
            BaseConsoleVariable::SharedPtrType regVar = consoleVariableManager->FindVariable(name);
            if (regVar == nullptr)
            {
                SharedPtrType var = ASTEROID_ALLOCATE_SHARED(ConsoleVariable<T>, name, isPersistent, defaultValue);
                consoleVariableManager->Register(var);
                var->OnRegister(consoleVariableManager->GetPlayerPrefs(), defaultValue);
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

        /**
         *  Construct a console variable without registration
         */
        ConsoleVariable(const String& name, bool isPersistent, const T& value)
            : BaseConsoleVariable(name, isPersistent), m_Value(value)
        {
        }

        virtual ~ConsoleVariable() 
        {
        }

        /**
         *  Implicit conversion to the internal value type.
         */
        operator T&() { return m_Value; }

        /**
         *  Assign a value of type T to this console variable.
         */
        ConsoleVariable<T>& operator=(const T& other) 
        {
            m_Value = other; 
            return *this;
        }

        /**
         *  Override Base::ReadValue
         */
        virtual void ReadValue(std::istream& is) override { is >> m_Value; }
        /**
         *  Override Base::WriteValue
         */
        virtual void WriteValue(std::ostream& os) override { os << m_Value; }

        /**
         *  Called when this console variable is registered.
         *  @remarks
         *      For persistent console variables, calling this function restores the persistent value
         *      from player prefs.
         */
        void OnRegister(PlayerPrefs* playerPrefs, const T& defaultValue)
        {
            if (m_IsPersistent)
                m_Value = playerPrefs->GetValue<T>(m_Name, defaultValue);
        }
        /**
         *  Called when this console variable is unregistered.
         *  @remarks
         *      For persistent console variables, calling this function saves the value to player prefs.
         */
        virtual void OnUnregister(PlayerPrefs* playerPrefs) const override
        {
            if (m_IsPersistent)
                playerPrefs->SetValue<T>(m_Name, m_Value);
        }

    private:
        T m_Value;
    };


    


    
}