#pragma once

#include "Debug.h"

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
        using NamedValueMap = std::unordered_map<std::string, T>;

    public:
        ASTEROID_NO_DEFAULT_CTOR(PlayerPrefs)
        ASTEROID_NON_COPYABLE(PlayerPrefs)

        /**
         *  Load all values from local storage.\n
         *  All values present in memory will be updated to the values from the local storage.
         *  @return
         *      True if succeeded. Otherwise false.
         */
        static bool Load();

        /**
         *  Write all values to local storage.
         */
        static void Save();

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
        static int32_t GetInteger(const std::string& name, int32_t defaultValue = 0)
        {
            int32_t value = defaultValue;
            auto it = _IntegerValues.find(name);
            if (it != _IntegerValues.end())
                value = it->second;
            else
                _IntegerValues.insert(std::make_pair(name, defaultValue));
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
        static void SetInteger(const std::string& name, int32_t value)
        {
            auto it = _IntegerValues.find(name);
            if (it != _IntegerValues.end())
                it->second = value;
            else
                _IntegerValues.insert(std::make_pair(name, value));
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
        static float GetSingle(const std::string& name, float defaultValue = 0.0f)
        {
            float value = defaultValue;
            auto it = _SingleValues.find(name);
            if (it != _SingleValues.end())
                value = it->second;
            else
                _SingleValues.insert(std::make_pair(name, defaultValue));
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
        static void SetSingle(const std::string& name, float value)
        {
            auto it = _SingleValues.find(name);
            if (it != _SingleValues.end())
                it->second = value;
            else
                _SingleValues.insert(std::make_pair(name, value));
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
        static std::string GetString(const std::string& name, const std::string& defaultValue = "")
        {
            std::string value = defaultValue;
            auto it = _StringValues.find(name);
            if (it != _StringValues.end())
                value = it->second;
            else
                _StringValues.insert(std::make_pair(name, defaultValue));
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
        static void SetString(const std::string& name, const std::string& value)
        {
            auto it = _StringValues.find(name);
            if (it != _StringValues.end())
                it->second = value;
            else
                _StringValues.insert(std::make_pair(name, value));
        }

    private:
        static NamedValueMap<int32_t>      _IntegerValues;
        static NamedValueMap<float>        _SingleValues;
        static NamedValueMap<std::string>  _StringValues;
    };
    
}