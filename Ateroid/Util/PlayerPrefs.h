#pragma once

#include "Containers.h"
#include "Debug.h"
#include "String.h"

namespace ASTEROID_NAMESPACE
{
    /**
     *  A local key-value storage for simple player preference settings.\n
     *  Integer, float and string value type are supported.\n
     *  The values in local storage will be loaded to memory at some time of the engine start stage,
     *  so those Get/Set functions actually get/set values from/to memory instead of local storage.\n
     *  The values cached in memory will finally be written to local storage when the engine is finalizing.\n
     *  Multiple different type values with same name are allowed.
     */
    class PlayerPrefs
    {
    public:
        template<typename T>
        using NamedValueMap = UnorderedMap<String, T>;

    public:
        ASTEROID_NON_COPYABLE(PlayerPrefs)

        /**
         *  Create a PlayerPrefs singleton.
         */
        static PlayerPrefs* Create()
        {
            ASTEROID_ASSERT(_Singleton == nullptr, "There is already a PlayerPrefs singleton created.");
            _Singleton = ASTEROID_NEW PlayerPrefs();
            return _Singleton;
        }

        /**
         *  Destroy the current PlayerPrefs singleton.
         */
        static void Destroy()
        {
            ASTEROID_DELETE _Singleton;
            _Singleton = nullptr;
        }

        /**
         *  Current created singleton.
         *  @return
         *      Instance of current created singleton. nullptr if no instance created or singleton was destroyed.
         */
        static PlayerPrefs* Singleton() { return _Singleton; }

        /**
         *  Load all values from local storage.\n
         *  All values present in memory will be updated to the values from the local storage.
         *  @return
         *      True if succeeded. Otherwise false.
         */
        bool Load();

        /**
         *  Write all values to local storage.
         */
        void Save();

        /**
         *  Get a integer value by name.
         *  @param name
         *      The unique name of this value.
         *  @param defaultValue
         *      The default value of this value if the name is not exist.
         *  @return
         *      The value with the given name if exist. Otherwise the default value.
         *  @remarks
         *      If the function failed to find a value by the given name then a new value with the given
         *      name is created and initialized to the defauled value.
         */
        int32_t GetInteger(const String& name, int32_t defaultValue = 0)
        {
            int32_t value = defaultValue;
            auto it = m_IntegerValues.find(name);
            if (it != m_IntegerValues.end())
                value = it->second;
            else
                m_IntegerValues.insert(std::make_pair(name, defaultValue));
            return value;
        }
        /**
         *  Set a integer value by name.
         *  @param name
         *      The unique name of this value.
         *  @param value
         *      The value being assigned.
         *  @remarks
         *      If the function failed to find a value by the given name then a new value with the given
         *      name is created and initialized to the given value.
         */
        void SetInteger(const String& name, int32_t value)
        {
            auto it = m_IntegerValues.find(name);
            if (it != m_IntegerValues.end())
                it->second = value;
            else
                m_IntegerValues.insert(std::make_pair(name, value));
        }

        /**
         *  Get a single type value by name.
         *  @param name
         *      The unique name of this value.
         *  @param defaultValue
         *      The default value of this value if the name is not exist.
         *  @return
         *      The value with the given name if exist. Otherwise the default value.
         *  @remarks
         *      If the function failed to find a value by the given name then a new value with the given
         *      name is created and initialized to the defauled value.
         */
        float GetSingle(const String& name, float defaultValue = 0.0f)
        {
            float value = defaultValue;
            auto it = m_SingleValues.find(name);
            if (it != m_SingleValues.end())
                value = it->second;
            else
                m_SingleValues.insert(std::make_pair(name, defaultValue));
            return value;
        }
        /**
         *  Set a single type value by name.
         *  @param name
         *      The unique name of this value.
         *  @param value
         *      The value being assigned.
         *  @remarks
         *      If the function failed to find a value by the given name then a new value with the given
         *      name is created and initialized to the given value.
         */
        void SetSingle(const String& name, float value)
        {
            auto it = m_SingleValues.find(name);
            if (it != m_SingleValues.end())
                it->second = value;
            else
                m_SingleValues.insert(std::make_pair(name, value));
        }

        /**
         *  Get a string value by name.
         *  @param name
         *      The unique name of this value.
         *  @param defaultValue
         *      The default value of this value if the name is not exist.
         *  @return
         *      The value with the given name if exist. Otherwise the default value.
         *  @remarks
         *      If the function failed to find a value by the given name then a new value with the given
         *      name is created and initialized to the defauled value.
         */
        String GetString(const String& name, const String& defaultValue = "")
        {
            String value = defaultValue;
            auto it = m_StringValues.find(name);
            if (it != m_StringValues.end())
                value = it->second;
            else
                m_StringValues.insert(std::make_pair(name, defaultValue));
            return value;
        }
        /**
         *  Set a string value by name.
         *  @param name
         *      The unique name of this value.
         *  @param value
         *      The value being assigned.
         *  @remarks
         *      If the function failed to find a value by the given name then a new value with the given
         *      name is created and initialized to the given value.
         */
        void SetString(const String& name, const String& value)
        {
            auto it = m_StringValues.find(name);
            if (it != m_StringValues.end())
                it->second = value;
            else
                m_StringValues.insert(std::make_pair(name, value));
        }

        /**
         *  A function template for get value of type T by name.
         *  This function template is specialized for each specific supported types,
         *  Instantiation using any other non-supported type will result a compile time error.
         */
        template <typename T>
        T GetValue(const String& name, const T& defaultValue)
        {
            static_assert(false, "GetValue with type T is not supported.");
        }
        /**
         *  A function template for set value of type T by name.
         *  This function template is specialized for each specific supported types,
         *  Instantiation using any other non-supported type will result a compile time error.
         */
        template <typename T>
        void SetValue(const String& name, const T& value)
        {
            static_assert(false, "SetValue with type T is not supported.");
        }

        /**
         *  Generic version of PlayerPrefs::GetInteger
         */
        template <>
        int GetValue<int>(const String& name, const int& defaultValue)
        {
            return GetInteger(name, defaultValue);
        }
        /**
         *  Generic version of PlayerPrefs::SetInteger
         */
        template <>
        void SetValue<int>(const String& name, const int& value)
        {
            SetInteger(name, value);
        }

        /**
         *  Generic version of PlayerPrefs::GetSingle
         */
        template <>
        float GetValue<float>(const String& name, const float& defaultValue)
        {
            return GetSingle(name, defaultValue);
        }
        /**
         *  Generic version of PlayerPrefs::SetSingle
         */
        template <>
        void SetValue<float>(const String& name, const float& value)
        {
            SetSingle(name, value);
        }

        /**
         *  Generic version of PlayerPrefs::GetString
         */
        template <>
        String GetValue<String>(const String& name, const String& defaultValue)
        {
            return GetString(name, defaultValue);
        }
        /**
         *  Generic version of PlayerPrefs::SetString
         */
        template <>
        void SetValue<String>(const String& name, const String& value)
        {
            SetString(name, value);
        }

    private:
        PlayerPrefs() = default;

    private:
        static PlayerPrefs* _Singleton;

    private:
        NamedValueMap<int32_t>  m_IntegerValues;
        NamedValueMap<float>    m_SingleValues;
        NamedValueMap<String>   m_StringValues;
    };
    
}