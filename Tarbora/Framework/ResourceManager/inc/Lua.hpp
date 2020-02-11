#pragma once
#include "Resource.hpp"
#define SOL_ALL_SAFETIES_ON 1
#include <sol/sol.hpp>

namespace Tarbora {
    class LuaTable;

    typedef const sol::basic_object<sol::basic_reference<false>> sol_object;
    typedef sol::basic_table_core<false, sol::basic_reference<false> >::iterator sol_iterator;

    inline void LuaPrintError(const std::string &name, const std::string &reason)
    {
        LOG_WARN("Error: Could not get [%s]. %s", name.c_str(), reason.c_str());
    }

    template <class T>
    class LuaType {
    public:
        static T GetDefault() { return 0; }
        static std::string GetErrorName() { return "Invalid type"; }

        static LuaTable &Set(LuaTable *table, const std::string &name, T value);
        static LuaTable &Set(LuaTable *table, unsigned int index, T value);

        static T Get(const LuaTable *table, const std::string &name, T def, bool silent);
        static T Get(const LuaTable *table, unsigned int index, T def, bool silent);

        static bool Is(const sol_object &object) { return object.is<T>(); }
        static T As(sol::state *state, const sol_object &object, const std::string &name, T def, bool silent);
    };

    template <class T>
    class LuaFunction {
    public:
        LuaFunction() {}
        LuaFunction(sol::state *state, sol::protected_function function, const std::string &name, bool silent)
            : m_Lua(state), m_Function(function), m_Name(name), m_Silent(silent)
        {
            m_Function.error_handler = (*m_Lua)["error_handler"];
        }

        bool Valid() const { return !(!m_Function); }

        template <class... Args>
        T operator()(Args&&... args);

    private:
        sol::state *m_Lua;
        sol::protected_function m_Function;
        std::string m_Name;
        bool m_Silent;
    };

    class LuaObject {
    public:
        LuaObject() {}
        LuaObject(sol::state *state, sol_object &obj, const std::string &name)
            : m_Lua(state), m_Object(obj), m_Name(name) {}
        template <class T>
        T GetAs(T def, bool silent=false);
        template <class T>
        T GetAs(bool silent=false) { return GetAs(LuaType<T>::GetDefault(), silent); }
        template <class T>
        bool Is() const { return LuaType<T>::Is(m_Object) || LuaType<LuaFunction<T>>::Is(m_Object); }

        bool Valid() const { return !(!m_Object); }

    private:
        sol::state *m_Lua;
        sol_object m_Object;
        std::string m_Name;
    };

    class LuaIterator {
    public:
        // This is bad... But given the implementation of sol::table_iterator, I can't find a better way.
        LuaIterator(const LuaTable *table, const sol::table *t, bool end=false);

        std::pair<LuaObject, LuaObject> operator*();

        bool operator==(const LuaIterator &other) const { return m_Iter == other.m_Iter; }
        bool operator!=(const LuaIterator &other) const { return m_Iter != other.m_Iter; }

        LuaIterator &operator++()
        {
            ++m_Iter;
            return *this;
        }

    private:
        sol_iterator m_Iter;
        const LuaTable *m_Table;
    };

    class LuaTable {
    public:
        LuaTable() {}
        LuaTable(sol::state *state, sol::table table, const std::string &name)
            : m_Lua(state), m_Table(table), m_Name(name == "" ? "" : name + ".") {}

        LuaTable CreateTable(const std::string &name);
        LuaTable CreateTable(unsigned int &index);

        template <class T>
        LuaTable &Set(const std::string &name, T value) { return LuaType<T>::Set(this, name, value); }
        template <class T>
        LuaTable &Set(unsigned int index, T value) { return LuaType<T>::Set(this, index, value); }
        template <class T=LuaTable>
        T Get(const std::string &name, T def, bool silent=false) const { return LuaType<T>::Get(this, name, def, silent); }
        template <class T=LuaTable>
        T Get(const std::string &name, bool silent=false) const { return Get<T>(name, LuaType<T>::GetDefault(), silent); }
        template <class T=LuaTable>
        T Get(unsigned int index, T def, bool silent=false) const { return LuaType<T>::Get(this, index, def, silent); }
        template <class T=LuaTable>
        T Get(unsigned int index, bool silent=false) const { return Get<T>(index, LuaType<T>::GetDefault(), silent); }

        // This is bad... But given the implementation of sol::table_iterator, I can't find a better way.
        LuaIterator begin() const { return LuaIterator(this, &m_Table); }
        LuaIterator end() const { return LuaIterator(this, &m_Table, true); }

        bool Valid() const { return !(!m_Table); }
        unsigned int Size() const {return Valid() ? m_Table.size() : 0; }

        sol::state *m_Lua;
        sol::table m_Table;
        std::string m_Name;
    };

    class LuaScript : public Resource {
        friend class LuaLoader;
    public:
        LuaScript(const std::string &script, bool isFile = true)
            : Resource(script)
        {
            m_Lua.require_file("p", "../Resources/LuaParameters.lua");
            if (isFile)
            {
                m_Lua.safe_script_file(script, [](lua_State*, sol::protected_function_result pfr) {
                    sol::error err = pfr;
                    LOG_ERR("Error: In file %s", err.what());
                    return pfr;
                });
            }
            else
            {
                m_Lua.safe_script(script, [](lua_State*, sol::protected_function_result pfr) {
                    sol::error err = pfr;
                    LOG_ERR("Error: In script %s", err.what());
                    return pfr;
                });
            }
            m_Lua.open_libraries(sol::lib::base, sol::lib::math);
        }

        LuaTable CreateTable(const std::string &name)
        {
            return LuaTable(&m_Lua, (sol::table)m_Lua.globals(), "").CreateTable(name);
        }

        LuaTable CreateTable(unsigned int &index)
        {
            return LuaTable(&m_Lua, (sol::table)m_Lua.globals(), "").CreateTable(index);
        }

        template <class T>
        LuaTable &Set(const std::string &name, T value)
        {
            return LuaTable(&m_Lua, (sol::table)m_Lua.globals(), "").Set(name, value);
        }
        template <class T>
        LuaTable &Set(unsigned int index, T value)
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
            {
                std::ifstream file(path);
                if (!file) return std::shared_ptr<Resource>();
            }

            return std::shared_ptr<Resource>(new LuaScript(path, true));
        }
    };


    // -----------------------------------------------------------------------------
    // IMPLEMENTATION
    // -----------------------------------------------------------------------------

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
    inline LuaTable LuaType<LuaTable>::GetDefault() { return LuaTable(); }
    template <>
    inline std::string LuaType<LuaTable>::GetErrorName() { return "Not a table"; }
    template <>
    inline LuaObject LuaType<LuaObject>::GetDefault() { return LuaObject(); }
    template <>
    inline std::string LuaType<LuaObject>::GetErrorName() { return "Not an object"; }
    template <>
    inline glm::vec3 LuaType<glm::vec3>::GetDefault() { return glm::vec3(0.0f); }
    template <>
    inline std::string LuaType<glm::vec3>::GetErrorName() { return "Not a vec3"; }
    template <>
    inline glm::vec4 LuaType<glm::vec4>::GetDefault() { return glm::vec4(0.0f); }
    template <>
    inline std::string LuaType<glm::vec4>::GetErrorName() { return "Not a vec4"; }
    template <>
    inline glm::mat3 LuaType<glm::mat3>::GetDefault() { return glm::mat3(1.0f); }
    template <>
    inline std::string LuaType<glm::mat3>::GetErrorName() { return "Not a mat3"; }
    template <>
    inline glm::mat4 LuaType<glm::mat4>::GetDefault() { return glm::mat4(1.0f); }
    template <>
    inline std::string LuaType<glm::mat4>::GetErrorName() { return "Not a mat4"; }


    template <class T>
    template <class... Args>
    T LuaFunction<T>::operator()(Args&&... args)
    {
        if (m_Function)
        {
            auto execution = m_Function(std::forward<Args>(args)...);
            if (execution.valid())
            {
                return LuaType<T>::As(m_Lua, execution, m_Name, LuaType<T>::GetDefault(), m_Silent);
            }
            sol::error err = execution;
            LuaPrintError(m_Name, err.what());
        }
        return LuaType<T>::GetDefault();
    }

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
                LuaPrintError(m_Name, err.what());
            }
        }
    }

    template <class T>
    class LuaFunction<std::vector<T>> {
    public:
        LuaFunction() {}
        LuaFunction(sol::state *state, sol::protected_function function, const std::string &name, bool silent)
            : m_Lua(state), m_Function(function), m_Name(name)
        {
            m_Function.error_handler = (*m_Lua)["error_handler"];
        }

        bool Valid() const { return !(!m_Function); }

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
                                vec.push_back(table.Get<T>(i));
                            }
                            return vec;
                        }
                    }
                    else
                        LuaPrintError(m_Name, "Not a list");
                }
                sol::error err = execution;
                LuaPrintError(m_Name, err.what());
            }
            return std::vector<T>();
        }

    private:
        sol::state *m_Lua;
        sol::protected_function m_Function;
        std::string m_Name;
        bool m_Silent;
    };

    template <class T>
    class LuaType<LuaFunction<T>>
    {
    public:
        static LuaFunction<T> GetDefault() { return LuaFunction<T>(); }
        static std::string GetErrorName() { return "Not a function"; }

        LuaTable &Set(LuaTable *table, const std::string &name, LuaFunction<T> value)
        {
            if (table->m_Table)
                table->m_Table[name] = value->m_Function;
            return *table;
        }

        LuaTable &Set(LuaTable *table, unsigned int index, LuaFunction<T> value)
        {
            if (table->m_Table)
                table->m_Table[index] = value->m_Function;
            return *table;
        }

        static LuaFunction<T> Get(const LuaTable *table, const std::string &name, LuaFunction<T> def, bool silent)
        {
            if (table->m_Table)
                return As(table->m_Lua, table->m_Table[name], table->m_Name + name, def, silent);
            return def;
        }

        static LuaFunction<T> Get(const LuaTable *table, unsigned int index, LuaFunction<T> def, bool silent)
        {
            if (table->m_Table)
                return As(table->m_Lua, table->m_Table[index], table->m_Name + std::to_string(index), def, silent);
            return def;
        }

        static bool Is(const sol_object &object) { return object.is<sol::protected_function>(); }

        static LuaFunction<T> As(sol::state *state, const sol_object &object, const std::string &name, LuaFunction<T> def, bool silent)
        {
            if (object.is<sol::protected_function>())
                return LuaFunction<T>(state, object.as<sol::protected_function>(), name, silent);

            if (!silent) LuaPrintError(name, GetErrorName());
            return def;
        }
    };

    inline LuaTable LuaTable::CreateTable(const std::string &name)
    {
    sol::table t = m_Lua->create_named_table(name);
    m_Table[name] = t;
    return LuaTable(m_Lua, t, m_Name + name);
    }

    inline LuaTable LuaTable::CreateTable(unsigned int &index)
    {
        sol::table t = m_Lua->create_named_table(index);
        m_Table[index] = t;
        return LuaTable(m_Lua, t, m_Name + std::to_string(index));;
    }

    template <class T>
    LuaTable &LuaType<T>::Set(LuaTable *table, const std::string &name, T value)
    {
        if (table->m_Table)
            table->m_Table[name] = value;
        return *table;
    }

    template <class T>
    LuaTable &LuaType<T>::Set(LuaTable *table, unsigned int index, T value)
    {
        if (table->m_Table)
            table->m_Table[index] = value;
        return *table;
    }

    template <class T>
    T LuaType<T>::Get(const LuaTable *table, const std::string &name, T def, bool silent)
    {
        if (table->m_Table)
            return As(table->m_Lua, table->m_Table[name], table->m_Name + name, def, silent);
        return def;
    }

    template <class T>
    T LuaType<T>::Get(const LuaTable *table, unsigned int index, T def, bool silent)
    {
        if (table->m_Table)
            return As(table->m_Lua, table->m_Table[index], table->m_Name + std::to_string(index), def, silent);
        return def;
    }

    template <class T>
    T LuaType<T>::As(sol::state *state, const sol_object &object, const std::string &name, T def, bool silent)
    {
        if (object.is<T>())
            return object.as<T>();

        // If its not the expected type, maybe is a function
        if (object.is<sol::protected_function>())
        {
            LuaFunction<T> function(state, object.as<sol::protected_function>(), name, silent);
            if (function.Valid())
                 return function();
        }

        if (!silent) LuaPrintError(name, LuaType<T>::GetErrorName());
        return def;
    }

    template <>
    inline LuaTable &LuaType<LuaTable>::Set(LuaTable *table, const std::string &name, LuaTable value)
    {
        if (table->m_Table)
            table->m_Table[name] = value.m_Table;
        return *table;
    }

    template <>
    inline LuaTable &LuaType<LuaTable>::Set(LuaTable *table, unsigned int index, LuaTable value)
    {
        if (table->m_Table)
            table->m_Table[index] = value.m_Table;
        return *table;
    }

    template <>
    inline bool LuaType<LuaTable>::Is(const sol_object &object) { return object.is<sol::table>(); }

    template <>
    inline LuaTable LuaType<LuaTable>::As(sol::state *state, const sol_object &object, const std::string &name, LuaTable def, bool silent)
    {
        if (object.is<sol::table>())
            return LuaTable(state, object.as<sol::table>(), name);

        // If its not the expected type, maybe is a function
        if (object.is<sol::protected_function>())
        {
            LuaFunction<LuaTable> function(state, object.as<sol::protected_function>(), name, silent);
            if (function.Valid())
                 return function();
        }

        if (!silent) LuaPrintError(name, GetErrorName());
        return def;
    }

    template <>
    inline LuaTable &LuaType<glm::vec3>::Set(LuaTable *table, const std::string &name, glm::vec3 value)
    {
        if (table->m_Table)
        {
            LuaTable t = table->CreateTable(name);
            for (unsigned int i = 0; i < 3; i++)
                t.Set(i+1, value[i]);
        }
        return *table;
    }

    template <>
    inline LuaTable &LuaType<glm::vec3>::Set(LuaTable *table, unsigned int index, glm::vec3 value)
    {
        if (table->m_Table)
        {
            LuaTable t = table->CreateTable(index);
            for (unsigned int i = 0; i < 3; i++)
                t.Set(i+1, value[i]);
        }
        return *table;
    }

    template <>
    inline LuaTable &LuaType<glm::vec4>::Set(LuaTable *table, const std::string &name, glm::vec4 value)
    {
        if (table->m_Table)
        {
            LuaTable t = table->CreateTable(name);
            for (unsigned int i = 0; i < 4; i++)
                t.Set(i+1, value[i]);
        }
        return *table;
    }

    template <>
    inline LuaTable &LuaType<glm::vec4>::Set(LuaTable *table, unsigned int index, glm::vec4 value)
    {
        if (table->m_Table)
        {
            LuaTable t = table->CreateTable(index);
            for (unsigned int i = 0; i < 4; i++)
                t.Set(i+1, value[i]);
        }
        return *table;
    }

    template <>
    inline LuaTable &LuaType<glm::mat3>::Set(LuaTable *table, const std::string &name, glm::mat3 value)
    {
        if (table->m_Table)
        {
            LuaTable t = table->CreateTable(name);
            for (unsigned int i = 0; i < 3; i++)
                t.Set(i+1, value[i]);
        }
        return *table;
    }

    template <>
    inline LuaTable &LuaType<glm::mat3>::Set(LuaTable *table, unsigned int index, glm::mat3 value)
    {
        if (table->m_Table)
        {
            LuaTable t = table->CreateTable(index);
            for (unsigned int i = 0; i < 3; i++)
                t.Set(i+1, value[i]);
        }
        return *table;
    }

    template <>
    inline LuaTable &LuaType<glm::mat4>::Set(LuaTable *table, const std::string &name, glm::mat4 value)
    {
        if (table->m_Table)
        {
            LuaTable t = table->CreateTable(name);
            for (unsigned int i = 0; i < 4; i++)
                t.Set(i+1, value[i]);
        }
        return *table;
    }

    template <>
    inline LuaTable &LuaType<glm::mat4>::Set(LuaTable *table, unsigned int index, glm::mat4 value)
    {
        if (table->m_Table)
        {
            LuaTable t = table->CreateTable(index);
            for (unsigned int i = 0; i < 4; i++)
                t.Set(i+1, value[i]);
        }
        return *table;
    }

    template <>
    inline bool LuaType<glm::vec3>::Is(const sol_object &object) { return object.is<sol::table>(); }

    template <>
    inline bool LuaType<glm::vec4>::Is(const sol_object &object) { return object.is<sol::table>(); }

    template <>
    inline bool LuaType<glm::mat3>::Is(const sol_object &object) { return object.is<sol::table>(); }

    template <>
    inline bool LuaType<glm::mat4>::Is(const sol_object &object) { return object.is<sol::table>(); }

    template <>
    inline glm::vec3 LuaType<glm::vec3>::As(sol::state *state, const sol_object &object, const std::string &name, glm::vec3 def, bool silent)
    {
        LuaTable table = LuaType<LuaTable>::As(state, object, name, LuaTable(), silent);
        glm::vec3 vec;
        for (int i = 0; i < 3; i++)
            vec[i] = table.Get<float>(i+1, def[i], silent);
        return vec;
    }

    template <>
    inline glm::vec4 LuaType<glm::vec4>::As(sol::state *state, const sol_object &object, const std::string &name, glm::vec4 def, bool silent)
    {
        LuaTable table = LuaType<LuaTable>::As(state, object, name, LuaTable(), silent);
        glm::vec4 vec;
        for (int i = 0; i < 4; i++)
            vec[i] = table.Get<float>(i+1, def[i], silent);
        return vec;
    }

    template <>
    inline glm::mat3 LuaType<glm::mat3>::As(sol::state *state, const sol_object &object, const std::string &name, glm::mat3 def, bool silent)
    {
        LuaTable table = LuaType<LuaTable>::As(state, object, name, LuaTable(), silent);
        glm::mat3 mat;
        for (int i = 0; i < 3; i++)
            mat[i] = table.Get<glm::vec3>(i+1, def[i], silent);
        return mat;
    }

    template <>
    inline glm::mat4 LuaType<glm::mat4>::As(sol::state *state, const sol_object &object, const std::string &name, glm::mat4 def, bool silent)
    {
        LuaTable table = LuaType<LuaTable>::As(state, object, name, LuaTable(), silent);
        glm::mat4 mat;
        for (int i = 0; i < 4; i++)
            mat[i] = table.Get<glm::vec4>(i+1, def[i], silent);
        return mat;
    }

    template <>
    inline LuaObject LuaType<LuaObject>::As(sol::state *state, const sol_object &object, const std::string &name, LuaObject def, bool silent)
    {
        return LuaObject(state, object, name);
    }

    template <class T>
    class LuaType<std::vector<T>>
    {
    public:
        static std::vector<T> GetDefault() { return std::vector<T>(); }
        static std::string GetErrorName() { return "Not a list"; }

        LuaTable &Set(LuaTable *table, const std::string &name, std::vector<T> value)
        {
            if (table->m_Table)
            {
                LuaTable t = table->CreateTable(name);
                for (unsigned int i = 0; i < value.size(); i++)
                {
                    t.Set(i+1, value[i]);
                }
            }
            return *table;
        }

        LuaTable &Set(LuaTable *table, unsigned int index, std::vector<T> value)
        {
            if (table->m_Table)
            {
                LuaTable t = table->CreateTable(index);
                for (unsigned int i = 0; i < value.size(); i++)
                {
                    t.Set(i+1, value[i]);
                }
            }
            return *table;
        }

        static std::vector<T> Get(const LuaTable *table, const std::string &name, std::vector<T> def, bool silent)
        {
            if (table->m_Table)
            {
                return As(table->m_Lua, table->m_Table[name], table->m_Name + name, def, silent);
            }
            return def;
        }

        static std::vector<T> Get(const LuaTable *table, unsigned int index, std::vector<T> def, bool silent)
        {
            if (table->m_Table)
            {
                return As(table->m_Lua, table->m_Table[index], table->m_Name + std::to_string(index), def, silent);
            }
            return def;
        }

        static bool Is(const sol_object &object) { return LuaType<LuaTable>::Is(object); }

        static std::vector<T> As(sol::state *state, const sol_object &object, const std::string &name, std::vector<T> def, bool silent)
        {
            LuaTable table2 = LuaType<LuaTable>::As(state, object, name, LuaTable(), silent);
            if (table2.m_Table)
            {
                std::vector<T> vec;
                for (unsigned int i = 1; i <= table2.Size(); i++)
                {
                    vec.push_back(table2.Get<T>(i));
                }
                return vec;
            }
            return def;
        }
    };

    inline LuaIterator::LuaIterator(const LuaTable *table, const sol::table *t, bool end)
        : m_Iter(table->Valid() ? end ? t->end() : t->begin() : sol_iterator()), m_Table(table) {}

    template <class T>
    T LuaObject::GetAs(T def, bool silent)
    {
        if (Valid())
            return LuaType<T>::As(m_Lua, m_Object, m_Name, def, silent);
        return def;
    }

    inline std::pair<LuaObject, LuaObject> LuaIterator::operator*()
    {
        return std::pair<LuaObject, LuaObject>(
            LuaObject(m_Table->m_Lua, (*m_Iter).first, m_Table->m_Name + "iterable"),
            LuaObject(m_Table->m_Lua, (*m_Iter).second, m_Table->m_Name + "iterable")
        );
    }
}
