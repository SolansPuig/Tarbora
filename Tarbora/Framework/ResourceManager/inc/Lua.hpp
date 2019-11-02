#pragma once
#include "Resource.hpp"
#define SOL_ALL_SAFETIES_ON 1
#include <sol/sol.hpp>

namespace Tarbora {
    class LuaTable;

    template <class T>
    class LuaType {
    public:
        static T GetDefault() { return 0; }
        static std::string GetErrorName() { return "Invalid type"; }

        static T Get(LuaTable *table, const std::string &name, T def, bool silent);
        static T Get(LuaTable *table, unsigned int index, T def, bool silent);
    };

    template <class T>
    class LuaFunction {
    public:
        LuaFunction() {}
        LuaFunction(sol::state *state, sol::protected_function function, const std::string &name, bool silent)
            : m_Lua(state), m_Function(function), m_Name(name == "" ? "" : name + ".")
        {
            m_Function.error_handler = (*m_Lua)["error_handler"];
        }

        bool Valid() { return !(!m_Function); }

        template <class... Args>
        T operator()(Args&&... args)
        {
            return 0;
        }

    private:
        sol::state *m_Lua;
        sol::protected_function m_Function;
        const std::string m_Name;
        bool m_Silent;

        void PrintError(const std::string &name, const std::string &reason)
        {
            std::string path = m_Name + name;
            LOG_ERR("Lua: Could not get [%s]. %s", path.c_str(), reason.c_str());
        }

        void PrintError(unsigned int index, const std::string &reason)
        {
            PrintError(std::to_string(index), reason);
        }
    };

    class LuaTable {
    public:
        LuaTable() {}
        LuaTable(sol::state *state, sol::table table, const std::string &name)
            : m_Lua(state), m_Table(table), m_Name(name == "" ? "" : name + ".") {}

        template <class T>
        void Set(const std::string &name, T value)
        {
            m_Table[name] = value;
        }
        template <class T>
        void Set(unsigned int index, T value)
        {
            m_Table[index] = value;
        }

        template <class T=LuaTable>
        T Get(const std::string &name, T def, bool silent=false) { return LuaType<T>::Get(this, name, def, silent); }
        template <class T=LuaTable>
        T Get(const std::string &name, bool silent=false) { return Get<T>(name, LuaType<T>::GetDefault(), silent); }
        template <class T=LuaTable>
        T Get(unsigned int index, T def, bool silent=false) { return LuaType<T>::Get(this, index, def, silent); }
        template <class T=LuaTable>
        T Get(unsigned int index, bool silent=false) { return Get<T>(index, LuaType<T>::GetDefault(), silent); }

        bool Valid() { return !(!m_Table); }
        unsigned int Size() {return Valid() ? m_Table.size() : 0; }

        void PrintError(const std::string &name, const std::string &reason)
        {
            std::string path = m_Name + name;
            LOG_ERR("Lua: Could not get [%s]. %s", path.c_str(), reason.c_str());
        }

        void PrintError(unsigned int index, const std::string &reason)
        {
            PrintError(std::to_string(index), reason);
        }

        sol::state *m_Lua;
        sol::table m_Table;
        const std::string m_Name;
    };

    class LuaScript : public Resource {
        friend class LuaLoader;
    public:
        LuaScript(Module *m, const std::string &file)
            : Resource(m, file)
        {
            m_Lua.script_file(file);
            m_Lua.open_libraries(sol::lib::base, sol::lib::math);
        }

        template <class T>
        void Set(const std::string &name, T value)
        {
            return LuaTable(&m_Lua, (sol::table)m_Lua.globals(), "").Set(name, value);
        }
        template <class T>
        void Set(unsigned int index, T value)
        {
            return LuaTable(&m_Lua, (sol::table)m_Lua.globals(), "").Set(index, value);
        }

        template<class T=LuaTable>
        T Get(const std::string &name, T def, bool silent=false)
        {
            return LuaTable(&m_Lua, (sol::table)m_Lua.globals(), "").Get<T>(name, def, silent);
        }
        template<class T=LuaTable>
        T Get(const std::string &name, bool silent=false)
        {
            return LuaTable(&m_Lua, (sol::table)m_Lua.globals(), "").Get<T>(name, silent);
        }
        template<class T=LuaTable>
        T Get(unsigned int index, T def, bool silent=false)
        {
            return LuaTable(&m_Lua, (sol::table)m_Lua.globals(), "").Get<T>(index, def, silent);
        }
        template<class T=LuaTable>
        T Get(unsigned int index, bool silent=false)
        {
            return LuaTable(&m_Lua, (sol::table)m_Lua.globals(), "").Get<T>(index, silent);
        }
    private:
        sol::state m_Lua;
    };

    class LuaLoader : public ResourceLoader
    {
        friend class ResourceManager;
    private:
        virtual const std::string GetPattern() override { return "*.lua"; };
        virtual std::shared_ptr<Resource> Load(std::string path) override
        {
            return std::shared_ptr<Resource>(new LuaScript(m_Module, path));
        }
    };


    // -----------------------------------------------------------------------------
    // IMPLEMENTATION
    // -----------------------------------------------------------------------------

    template <>
    template <class... Args>
    void LuaFunction<void>::operator()(Args&&... args)
    {
        if (m_Function)
        {
            auto execution = m_Function(std::forward<Args>(args)...);
            if (!execution.valid())
            {
                sol::error err = execution;
                if (!m_Silent) PrintError(m_Name, err.what());
            }
        }
    }

    template <>
    template <class... Args>
    bool LuaFunction<bool>::operator()(Args&&... args)
    {
        if (m_Function)
        {
            auto execution = m_Function(std::forward<Args>(args)...);
            if (execution.valid())
            {
                sol::optional<bool> value = execution;
                if (value)
                    return value.value();
                else
                    if (!m_Silent) PrintError(m_Name, "Not a bool");
            }
            sol::error err = execution;
            if (!m_Silent) PrintError(m_Name, err.what());
        }
        return false;
    }

    template <>
    template <class... Args>
    int LuaFunction<int>::operator()(Args&&... args)
    {
        if (m_Function)
        {
            auto execution = m_Function(std::forward<Args>(args)...);
            if (execution.valid())
            {
                sol::optional<int> value = execution;
                if (value)
                    return value.value();
                else
                    if (!m_Silent) PrintError(m_Name, "Not an int");
            }
            sol::error err = execution;
            if (!m_Silent) PrintError(m_Name, err.what());
        }
        return 0;
    }

    template <>
    template <class... Args>
    float LuaFunction<float>::operator()(Args&&... args)
    {
        if (m_Function)
        {
            auto execution = m_Function(std::forward<Args>(args)...);
            if (execution.valid())
            {
                sol::optional<float> value = execution;
                if (value)
                    return value.value();
                else
                    if (!m_Silent) PrintError(m_Name, "Not a float");
            }
            sol::error err = execution;
            if (!m_Silent) PrintError(m_Name, err.what());
        }
        return 0;
    }

    template <>
    template <class... Args>
    std::string LuaFunction<std::string>::operator()(Args&&... args)
    {
        if (m_Function)
        {
            auto execution = m_Function(std::forward<Args>(args)...);
            if (execution.valid())
            {
                sol::optional<std::string> value = execution;
                if (value)
                    return value.value();
                else
                    if (!m_Silent) PrintError(m_Name, "Not a string");
            }
            sol::error err = execution;
            if (!m_Silent) PrintError(m_Name, err.what());
        }
        return "";
    }

    template <>
    template <class... Args>
    LuaTable LuaFunction<LuaTable>::operator()(Args&&... args)
    {
        if (m_Function)
        {
            auto execution = m_Function(std::forward<Args>(args)...);
            if (execution.valid())
            {
                sol::optional<sol::table> value = execution;
                if (value)
                    return LuaTable(m_Lua, value.value(), m_Name);
                else
                    if (!m_Silent) PrintError(m_Name, "Not a table");
            }
            sol::error err = execution;
            if (!m_Silent) PrintError(m_Name, err.what());
        }
        return LuaTable();
    }

    template <class T>
    class LuaFunction<std::vector<T>> {
    public:
        LuaFunction() {}
        LuaFunction(sol::state *state, sol::protected_function function, const std::string &name, bool m_Silent)
            : m_Lua(state), m_Function(function), m_Name(name == "" ? "" : name + ".")
        {
            m_Function.error_handler = (*m_Lua)["error_handler"];
        }

        bool Valid() { return !(!m_Function); }

        template <class... Args>
        std::vector<T> operator()(Args&&... args)
        {
            if (m_Function)
            {
                auto execution = m_Function(std::forward<Args>(args)...);
                if (execution.valid())
                {
                    sol::optional<sol::table> value = execution;
                    if (value)
                    {
                        LuaTable table = LuaTable(m_Lua, value.value(), m_Name);
                        if (table.m_Table)
                        {
                            std::vector<T> vec;
                            for (unsigned int i = 1; i <= table.Size(); i++)
                            {
                                vec.push_back(table.Get<T>(i, m_Silent));
                            }
                            return vec;
                        }
                    }
                    else
                        if (!m_Silent) PrintError(m_Name, "Not a list");
                }
                sol::error err = execution;
                if (!m_Silent) PrintError(m_Name, err.what());
            }
            return std::vector<T>();
        }

    private:
        sol::state *m_Lua;
        sol::protected_function m_Function;
        const std::string m_Name;
        bool m_Silent;

        void PrintError(const std::string &name, const std::string &reason)
        {
            std::string path = m_Name + name;
            LOG_ERR("Lua: Could not get [%s]. %s", path.c_str(), reason.c_str());
        }

        void PrintError(unsigned int index, const std::string &reason)
        {
            PrintError(std::to_string(index), reason);
        }
    };

    template <class T>
    class LuaType<LuaFunction<T>>
    {
    public:
        static LuaFunction<T> GetDefault() { return LuaFunction<T>(); }
        static std::string GetErrorName() { return "Not a function"; }

        static LuaFunction<T> Get(LuaTable *table, const std::string &name, LuaFunction<T> def, bool silent)
        {
            if (table->m_Table)
            {
                sol::optional<sol::protected_function> value = table->m_Table[name];
                if (value)
                    return LuaFunction<T>(table->m_Lua, value.value(), table->m_Name + name, silent);
                if (!silent) table->PrintError(name, GetErrorName());
            }
            return LuaFunction<T>();
        }

        static LuaFunction<T> Get(LuaTable *table, unsigned int index, LuaFunction<T> def, bool silent)
        {
            if (table->m_Table)
            {
                sol::optional<sol::protected_function> value = table->m_Table[index];
                if (value)
                    return LuaFunction<T>(table->m_Lua, value.value(), table->m_Name + std::to_string(index), silent);
                if (!silent) table->PrintError(index, GetErrorName());
            }
            return LuaFunction<T>();
        }
    };

    template <class T>
    T LuaType<T>::Get(LuaTable *table, const std::string &name, T def, bool silent)
    {
        if (table->m_Table)
        {
            sol::optional<T> value = table->m_Table[name];
            if (value)
                return value.value();

            // If its not the expected type, maybe is a function
            LuaFunction<T> function = table->Get<LuaFunction<T>>(name, silent);
            if (function.Valid())
                return function();
            if (!silent) table->PrintError(name, GetErrorName());
        }
        return def;
    }

    template <class T>
    T LuaType<T>::Get(LuaTable *table, unsigned int index, T def, bool silent)
    {
        if (table->m_Table)
        {
            sol::optional<T> value = table->m_Table[index];
            if (value)
                return value.value();
            // If its not the expected type, maybe is a function
            LuaFunction<T> function = table->Get<LuaFunction<T>>(index, silent);
            if (function.Valid())
                return function();
            if (!silent) table->PrintError(index, GetErrorName());
        }
        return def;
    }

    template <>
    inline std::string LuaType<bool>::GetErrorName() { return "Not a bool"; }
    template <>
    inline std::string LuaType<int>::GetErrorName() { return "Not an int"; }
    template <>
    inline std::string LuaType<float>::GetErrorName() { return "Not a float"; }
    template <>
    inline std::string LuaType<std::string>::GetDefault() { return ""; }
    template <>
    inline std::string LuaType<std::string>::GetErrorName() { return "Not a string"; }

    template <>
    class LuaType<LuaTable>
    {
    public:
        static LuaTable GetDefault() { return LuaTable(); }
        static std::string GetErrorName() { return "Not a table"; }

        static LuaTable Get(LuaTable *table, const std::string &name, LuaTable def, bool silent)
        {
            if (table->m_Table)
            {
                sol::optional<sol::table> value = table->m_Table[name];
                if (value)
                    return LuaTable(table->m_Lua, value.value(), table->m_Name + name);
                // If its not the expected type, maybe is a function
                LuaFunction<LuaTable> function = table->Get<LuaFunction<LuaTable>>(name, silent);
                if (function.Valid())
                    return function();
                if (!silent) table->PrintError(name, GetErrorName());
            }
            return def;
        }

        static LuaTable Get(LuaTable *table, unsigned int index, LuaTable def, bool silent)
        {
            if (table->m_Table)
            {
                sol::optional<sol::table> value = table->m_Table[index];
                if (value)
                    return LuaTable(table->m_Lua, value.value(), table->m_Name + std::to_string(index));
                // If its not the expected type, maybe is a function
                LuaFunction<LuaTable> function = table->Get<LuaFunction<LuaTable>>(index, silent);
                if (function.Valid())
                    return function();
                if (!silent) table->PrintError(index, GetErrorName());
            }
            return def;
        }
    };

    template <class T>
    class LuaType<std::vector<T>>
    {
    public:
        static std::vector<T> GetDefault() { return std::vector<T>(); }
        static std::string GetErrorName() { return "Not a list"; }

        static std::vector<T> Get(LuaTable *table, const std::string &name, std::vector<T> def, bool silent)
        {
            if (table->m_Table)
            {
                LuaTable table2 = table->Get<LuaTable>(name, silent);
                if (table2.m_Table)
                {
                    std::vector<T> vec;
                    for (unsigned int i = 1; i <= table2.Size(); i++)
                    {
                        vec.push_back(table2.Get<T>(i, silent));
                    }
                    return vec;
                }
            }
            return def;
        }

        static std::vector<T> Get(LuaTable *table, unsigned int index, std::vector<T> def, bool silent)
        {
            if (table->m_Table)
            {
                LuaTable table2 = table->Get<LuaTable>(index, silent);
                if (table2.m_Table)
                {
                    std::vector<T> vec;
                    for (unsigned int i = 1; i <= table2.Size(); i++)
                    {
                        vec.push_back(table2.Get<T>(i, silent));
                    }
                    return vec;
                }
            }
            return def;
        }
    };
}
