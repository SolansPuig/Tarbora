/*********************************************************************
 * Copyright (C) 2020 Roger Solans Puig
 * Email: roger@solanspuig.cat
 *
 * This file is part of Tarbora. You can obtain a copy at
 * https://github.com/SolansPuig/Tarbora
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 *********************************************************************/

#ifndef __TARBORALUA_H_
#define __TARBORALUA_H_

#include "Resource.hpp"
#define SOL_ALL_SAFETIES_ON 1
#include "sol/sol.hpp"

namespace Tarbora {
  class LuaTable;
  class LuaFile;

  typedef const sol::basic_object<sol::basic_reference<false>> sol_object;
  typedef sol::basic_table_core<false, sol::basic_reference<false> >::iterator sol_iterator;

  inline void printError(const std::string &name, const std::string &reason)
  {
    LOG_WARN("Error: Could not get [%s]. %s", name.c_str(), reason.c_str());
  }

  template <class T>
  class LuaType {
  public:
    static const T getDefault() { return 0; }
    static const std::string getErrorName() { return "Invalid type"; }

    static LuaTable& set(LuaTable *table, const std::string &name, const T &value);
    static LuaTable& set(LuaTable *table, unsigned int index, const T &value);

    static T get(
      const LuaTable *table, const std::string &name, const T &def, bool silent
    );
    static T get(const LuaTable *table, unsigned int index, const T &def, bool silent);

    static bool is(const sol_object &object) { return object.is<T>(); }
    static T as(
      std::shared_ptr<sol::state> state, const sol_object &object,
      const std::string &name, const T &def, bool silent
    );

    static void write(LuaFile *file, const std::string &name, const T &value, bool comma);
    static void write(LuaFile *file, const T &value, bool comma);
  };

  template <class T>
  class LuaFunction {
  public:
    LuaFunction() {}
    LuaFunction(
      std::shared_ptr<sol::state> state, const sol::protected_function &function,
      const std::string &name, bool silent
    )
      : lua_(state), function_(function), name_(name), silent_(silent)
    {
      function_.error_handler = (*lua_)["error_handler"];
    }

    bool valid() const { return !(!function_); }

    template <class... Args>
    T operator()(Args&&... args);

  private:
    std::shared_ptr<sol::state> lua_;
    sol::protected_function function_;
    std::string name_;
    bool silent_;
  };

  class LuaFile {
  public:
    LuaFile(const std::string &path)
    {
      file.open(path.c_str());
    }

    void indent() { indentation += tab_width; }
    void unindent() { indentation -= tab_width; }

    template <class T>
    void writeGlobal(const std::string &name, const T &value)
    {
      LuaType<T>::write(this, name, value, false);
    }

    template <class T>
    void writeGlobal(const T &value)
    {
      LuaType<T>::write(this, value, false);
    }

    template <class T>
    void write(const std::string &name, const T &value)
    {
      LuaType<T>::write(this, name, value, true);
    }

    template <class T>
    void write(const T &value)
    {
      LuaType<T>::write(this, value, true);
    }

    void beginTable(const std::string &name)
    {
      file << std::string(indentation, ' ') << name << " = {" << std::endl;
      indent();
    }

    void beginTable()
    {
      file << std::string(indentation, ' ') << "{" << std::endl;
      indent();
    }

    void closeTable(bool comma=true)
    {
      unindent();
      file << std::string(indentation, ' ') << (comma ? "}," : "}") << std::endl;
    }

    inline void beginGlobalTable(const std::string &name) { beginTable(name); }
    inline void beginGlobalTable() { beginTable(); }
    inline void closeGlobalTable() { closeTable(false); }

    std::ofstream file;
    unsigned int indentation{0};
    unsigned int tab_width{2};

  };

  class LuaObject {
  public:
    LuaObject() {}
    LuaObject(
      std::shared_ptr<sol::state> state, const sol_object &obj, const std::string &name
    )
      : lua_(state), object_(obj), name_(name) {}
    template <class T>
    T getAs(const T &def, bool silent=false);
    template <class T>
    T getAs(bool silent=false) { return getAs(LuaType<T>::getDefault(), silent); }
    template <class T>
    bool is() const
    {
      return LuaType<T>::is(object_) || LuaType<LuaFunction<T>>::is(object_);
    }

    bool valid() const { return !(!object_); }

  private:
    std::shared_ptr<sol::state> lua_;
    sol_object object_;
    std::string name_;
  };

  class LuaIterator {
  public:
    // This is bad... But given the implementation of sol::table_iterator,
    // I can't find a better way.
    LuaIterator(const LuaTable *table, const sol::table *t, bool end=false);

    std::pair<LuaObject, LuaObject> operator*();

    bool operator==(const LuaIterator &other) const { return iter_ == other.iter_; }
    bool operator!=(const LuaIterator &other) const { return iter_ != other.iter_; }

    LuaIterator& operator++()
    {
      ++iter_;
      return *this;
    }

  private:
    sol_iterator iter_;
    const LuaTable *table_;
  };

  class LuaTable {
  public:
    LuaTable() {}
    LuaTable(
      std::shared_ptr<sol::state> state, const sol::table &table, const std::string &name
    )
      : lua_(state), table_(table), name_(name == "" ? "" : name + ".") {}

    LuaTable createTable(const std::string &name);
    LuaTable createTable(unsigned int &index);

    template <class T>
    LuaTable& set(const std::string &name, const T &value)
    {
      return LuaType<T>::set(this, name, value);
    }

    template <class T>
    LuaTable& set(unsigned int index, const T &value)
    {
      return LuaType<T>::set(this, index, value);
    }

    template <class T=LuaTable>
    T get(const std::string &name, T def, bool silent=false) const
    {
      return LuaType<T>::get(this, name, def, silent);
    }

    template <class T=LuaTable>
    T get(const std::string &name, bool silent=false) const
    {
      return get<T>(name, LuaType<T>::getDefault(), silent);
    }

    template <class T=LuaTable>
    T get(unsigned int index, T def, bool silent=false) const
    {
      return LuaType<T>::get(this, index, def, silent);
    }

    template <class T=LuaTable>
    T get(unsigned int index, bool silent=false) const
    {
      return get<T>(index, LuaType<T>::getDefault(), silent);
    }

    // This is bad... But given the implementation of sol::table_iterator,
    // I can't find a better way.
    LuaIterator begin() const { return LuaIterator(this, &table_); }
    LuaIterator end() const { return LuaIterator(this, &table_, true); }

    bool valid() const { return !(!table_); }
    unsigned int size() const {return valid() ? table_.size() : 0; }

    std::shared_ptr<sol::state> lua_;
    sol::table table_;
    std::string name_;
  };

  class LuaScript : public Resource {
    friend class LuaLoader;
  public:
    LuaScript(const std::string &name, bool is_file = true) : Resource(name)
    {
      lua_ = std::make_shared<sol::state>();
      lua_->require_file("p", "../Resources/LuaParameters.lua");
      lua_->open_libraries(sol::lib::base, sol::lib::math, sol::lib::table);
      if (is_file)
      {
        lua_->safe_script_file(name, [](lua_State*, sol::protected_function_result pfr) {
          const sol::error err = pfr;
          LOG_ERR("Error: In file %s", err.what());
          return pfr;
        });
      }
      else
      {
        lua_->safe_script(name, [](lua_State*, sol::protected_function_result pfr) {
          const sol::error err = pfr;
          LOG_ERR("Error: In script %s", err.what());
          return pfr;
        });
      }
    }

    LuaTable createTable(const std::string &name)
    {
      return LuaTable(lua_, (sol::table)lua_->globals(), "").createTable(name);
    }

    LuaTable createTable(unsigned int &index)
    {
      return LuaTable(lua_, (sol::table)lua_->globals(), "").createTable(index);
    }

    template <class T>
    LuaTable& set(const std::string &name, const T &value)
    {
      return LuaTable(lua_, (sol::table)lua_->globals(), "").set(name, value);
    }
    template <class T>
    LuaTable& set(unsigned int index, const T &value)
    {
      return LuaTable(lua_, (sol::table)lua_->globals(), "").set(index, value);
    }

    template<class T=LuaTable>
    T get(const std::string &name, const T &def, bool silent=false)
    {
      return LuaTable(lua_, (sol::table)lua_->globals(), "").get<T>(name, def, silent);
    }
    template<class T=LuaTable>
    T get(const std::string &name, bool silent=false)
    {
      return LuaTable(lua_, (sol::table)lua_->globals(), "").get<T>(name, silent);
    }
    template<class T=LuaTable>
    T get(unsigned int index, const T &def, bool silent=false)
    {
      return LuaTable(lua_, (sol::table)lua_->globals(), "").get<T>(index, def, silent);
    }
    template<class T=LuaTable>
    T get(unsigned int index, bool silent=false)
    {
      return LuaTable(lua_, (sol::table)lua_->globals(), "").get<T>(index, silent);
    }
  private:
    std::shared_ptr<sol::state> lua_;
  };

  class LuaLoader : public ResourceLoader
  {
    friend class ResourceManager;
  public:
    virtual ~LuaLoader() {}
  private:
    virtual const std::string getPattern() override { return ".lua"; };
    virtual std::shared_ptr<Resource> load(const std::string &path) override
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
  inline const std::string LuaType<bool>::getErrorName() { return "Not a bool"; }
  template <>
  inline const std::string LuaType<int>::getErrorName() { return "Not an int"; }
  template <>
  inline const std::string LuaType<float>::getErrorName() { return "Not a float"; }
  template <>
  inline const std::string LuaType<std::string>::getDefault() { return ""; }
  template <>
  inline const std::string LuaType<std::string>::getErrorName() { return "Not a string"; }
  template <>
  inline const LuaTable LuaType<LuaTable>::getDefault() { return LuaTable(); }
  template <>
  inline const std::string LuaType<LuaTable>::getErrorName() { return "Not a table"; }
  template <>
  inline const LuaObject LuaType<LuaObject>::getDefault() { return LuaObject(); }
  template <>
  inline const std::string LuaType<LuaObject>::getErrorName() { return "Not an object"; }
  template <>
  inline const glm::vec2 LuaType<glm::vec2>::getDefault() { return glm::vec2(0.0f); }
  template <>
  inline const std::string LuaType<glm::vec2>::getErrorName() { return "Not a vec2"; }
  template <>
  inline const glm::vec3 LuaType<glm::vec3>::getDefault() { return glm::vec3(0.0f); }
  template <>
  inline const std::string LuaType<glm::vec3>::getErrorName() { return "Not a vec3"; }
  template <>
  inline const glm::vec4 LuaType<glm::vec4>::getDefault() { return glm::vec4(0.0f); }
  template <>
  inline const std::string LuaType<glm::vec4>::getErrorName() { return "Not a vec4"; }
  template <>
  inline const glm::mat3 LuaType<glm::mat3>::getDefault() { return glm::mat3(1.0f); }
  template <>
  inline const std::string LuaType<glm::mat3>::getErrorName() { return "Not a mat3"; }
  template <>
  inline const glm::mat4 LuaType<glm::mat4>::getDefault() { return glm::mat4(1.0f); }
  template <>
  inline const std::string LuaType<glm::mat4>::getErrorName() { return "Not a mat4"; }

  template <class T>
  void LuaType<T>::write(LuaFile *, const std::string &, const T &, bool)
  {}

  template <class T>
  void LuaType<T>::write(LuaFile *, const T &, bool)
  {}

  template <>
  inline void LuaType<bool>::write(
    LuaFile *file, const std::string &name, const bool &value, bool comma
  )
  {
    file->file << std::string(file->indentation, ' ') << name << " = "
      << (value ? "true" : "false") << (comma ? "," : "") << std::endl;
  }

  template <>
  inline void LuaType<bool>::write(LuaFile *file, const bool &value, bool comma)
  {
    file->file << std::string(file->indentation, ' ')
      << (value ? "true" : "false") << (comma ? "," : "") << std::endl;
  }

  template <>
  inline void LuaType<int>::write(
    LuaFile *file, const std::string &name, const int &value, bool comma
  )
  {
    file->file << std::string(file->indentation, ' ') << name << " = "
      << std::to_string(value) << (comma ? "," : "") << std::endl;
  }

  template <>
  inline void LuaType<int>::write(LuaFile *file, const int &value, bool comma)
  {
    file->file << std::string(file->indentation, ' ')
      << std::to_string(value) << (comma ? "," : "") << std::endl;
  }

  template <>
  inline void LuaType<float>::write(
    LuaFile *file, const std::string &name, const float &value, bool comma
  )
  {
    file->file << std::string(file->indentation, ' ') << name << " = "
      << std::fixed << std::setprecision(2) << value << (comma ? "," : "") << std::endl;
  }

  template <>
  inline void LuaType<float>::write(
    LuaFile *file, const float &value, bool comma
  )
  {
    file->file << std::string(file->indentation, ' ')
      << std::fixed << std::setprecision(2) << value << (comma ? "," : "") << std::endl;
  }

  template <>
  inline void LuaType<std::string>::write(
    LuaFile *file, const std::string &name, const std::string &value, bool comma
  )
  {
    file->file << std::string(file->indentation, ' ') << name << " = \""
      << value << (comma ? "\"," : "\"") << std::endl;
  }

  template <>
  inline void LuaType<std::string>::write(
    LuaFile *file, const std::string &value, bool comma
  )
  {
    file->file << std::string(file->indentation, ' ') << "\""
      << value << (comma ? "\"," : "\"") << std::endl;
  }

  template <class T>
  template <class... Args>
  T LuaFunction<T>::operator()(Args&&... args)
  {
    if (function_)
    {
      const auto execution = function_(std::forward<Args>(args)...);
      if (execution.valid())
      {
        return LuaType<T>::as(lua_, execution, name_, LuaType<T>::getDefault(), silent_);
      }
      const sol::error err = execution;
      printError(name_, err.what());
    }
    return LuaType<T>::getDefault();
  }

  template <>
  template <class... Args>
  void LuaFunction<void>::operator()(Args&&... args)
  {
    if (function_)
    {
      const auto execution = function_(std::forward<Args>(args)...);
      if (!execution.valid())
      {
        const sol::error err = execution;
        printError(name_, err.what());
      }
    }
  }

  template <class T>
  class LuaFunction<std::vector<T>> {
  public:
    LuaFunction() {}
    LuaFunction(
      std::shared_ptr<sol::state> state, const sol::protected_function &function,
      const std::string &name, bool
    )
      : lua_(state), function_(function), name_(name)
    {
      function_.error_handler = (*lua_)["error_handler"];
    }

    bool valid() const { return !(!function_); }

    template <class... Args>
    std::vector<T> operator()(Args&&... args)
    {
      if (function_)
      {
        const auto execution = function_(std::forward<Args>(args)...);
        if (execution.valid())
        {
          const sol::optional<sol::table> value = execution;
          if (value)
          {
            LuaTable table = LuaTable(lua_, value.value(), name_);
            if (table.table_)
            {
              std::vector<T> vec;
              for (unsigned int i = 1; i <= table.size(); i++)
              {
                vec.push_back(table.get<T>(i));
              }
              return vec;
            }
          }
          else
            printError(name_, "Not a list");
        }
        const sol::error err = execution;
        printError(name_, err.what());
      }
      return std::vector<T>();
    }

  private:
    std::shared_ptr<sol::state> lua_;
    sol::protected_function function_;
    std::string name_;
    bool silent_;
  };

  template <class T>
  class LuaType<LuaFunction<T>>
  {
  public:
    static const LuaFunction<T> getDefault() { return LuaFunction<T>(); }
    static const std::string getErrorName() { return "Not a function"; }

    LuaTable& set(LuaTable *table, const std::string &name, const LuaFunction<T> &value)
    {
      if (table->table_)
        table->table_[name] = value->function_;
      return *table;
    }

    LuaTable& set(LuaTable *table, unsigned int index, const LuaFunction<T> &value)
    {
      if (table->table_)
        table->table_[index] = value->function_;
      return *table;
    }

    static LuaFunction<T> get(
      const LuaTable *table, const std::string &name,
      const LuaFunction<T> &def, bool silent
    )
    {
      if (table->table_)
        return as(table->lua_, table->table_[name], table->name_ + name, def, silent);
      return def;
    }

    static LuaFunction<T> get(
      const LuaTable *table, unsigned int index, const LuaFunction<T> &def, bool silent
    )
    {
      if (table->table_)
        return as(
          table->lua_, table->table_[index],
          table->name_ + std::to_string(index), def, silent
        );
      return def;
    }

    static bool is(const sol_object &object)
    {
      return object.is<sol::protected_function>();
    }

    static LuaFunction<T> as(
      std::shared_ptr<sol::state> state, const sol_object &object, const std::string &name,
      const LuaFunction<T> &def, bool silent
    )
    {
      if (object.is<sol::protected_function>())
        return LuaFunction<T>(state, object.as<sol::protected_function>(), name, silent);

      if (!silent) printError(name, getErrorName());
      return def;
    }
  };

  inline LuaTable LuaTable::createTable(const std::string &name)
  {
    sol::table t = lua_->create_named_table(name);
    table_[name] = t;
    return LuaTable(lua_, t, name_ + name);
  }

  inline LuaTable LuaTable::createTable(unsigned int &index)
  {
    sol::table t = lua_->create_named_table(index);
    table_[index] = t;
    return LuaTable(lua_, t, name_ + std::to_string(index));;
  }

  template <class T>
  LuaTable& LuaType<T>::set(LuaTable *table, const std::string &name, const T &value)
  {
    if (table->table_)
      table->table_[name] = value;
    return *table;
  }

  template <class T>
  LuaTable& LuaType<T>::set(LuaTable *table, unsigned int index, const T &value)
  {
    if (table->table_)
      table->table_[index] = value;
    return *table;
  }

  template <class T>
  T LuaType<T>::get(
    const LuaTable *table, const std::string &name, const T &def, bool silent
  )
  {
    if (table->table_)
      return as(table->lua_, table->table_[name], table->name_ + name, def, silent);
    return def;
  }

  template <class T>
  T LuaType<T>::get(const LuaTable *table, unsigned int index, const T &def, bool silent)
  {
    if (table->table_)
      return as(
        table->lua_, table->table_[index],
        table->name_ + std::to_string(index), def, silent
      );
    return def;
  }

  template <class T>
  T LuaType<T>::as(
    std::shared_ptr<sol::state> state, const sol_object &object, const std::string &name,
    const T &def, bool silent
  )
  {
    if (object.is<T>())
      return object.as<T>();

    // If its not the expected type, maybe is a function
    if (object.is<sol::protected_function>())
    {
      LuaFunction<T> function(state, object.as<sol::protected_function>(), name, silent);
      if (function.valid())
        return function();
    }

    if (!silent) printError(name, LuaType<T>::getErrorName());
    return def;
  }

  template <>
  inline LuaTable& LuaType<LuaTable>::set(
    LuaTable *table, const std::string &name, const LuaTable &value
  )
  {
    if (table->table_)
      table->table_[name] = value.table_;
    return *table;
  }

  template <>
  inline LuaTable& LuaType<LuaTable>::set(
    LuaTable *table, unsigned int index, const LuaTable &value
  )
  {
    if (table->table_)
      table->table_[index] = value.table_;
    return *table;
  }

  template <>
  inline bool LuaType<LuaTable>::is(const sol_object &object)
  {
    return object.is<sol::table>();
  }

  template <>
  inline LuaTable LuaType<LuaTable>::as(
    std::shared_ptr<sol::state> state, const sol_object &object, const std::string &name,
    const LuaTable &def, bool silent
  )
  {
    if (object.is<sol::table>())
      return LuaTable(state, object.as<sol::table>(), name);

    // If its not the expected type, maybe is a function
    if (object.is<sol::protected_function>())
    {
      LuaFunction<LuaTable> function(
        state, object.as<sol::protected_function>(), name, silent
      );
      if (function.valid())
        return function();
    }

    if (!silent) printError(name, getErrorName());
    return def;
  }

  template <>
  inline LuaTable& LuaType<glm::vec2>::set(
    LuaTable *table, const std::string &name, const glm::vec2 &value
  )
  {
    if (table->table_)
    {
      LuaTable t = table->createTable(name);
      for (unsigned int i = 0; i < 2; i++)
        t.set(i+1, value[i]);
    }
    return *table;
  }

  template <>
  inline LuaTable& LuaType<glm::vec2>::set(
    LuaTable *table, unsigned int index, const glm::vec2 &value
  )
  {
    if (table->table_)
    {
      LuaTable t = table->createTable(index);
      for (unsigned int i = 0; i < 2; i++)
        t.set(i+1, value[i]);
    }
    return *table;
  }


  template <>
  inline LuaTable& LuaType<glm::vec3>::set(
    LuaTable *table, const std::string &name, const glm::vec3 &value
  )
  {
    if (table->table_)
    {
      LuaTable t = table->createTable(name);
      for (unsigned int i = 0; i < 3; i++)
        t.set(i+1, value[i]);
    }
    return *table;
  }

  template <>
  inline LuaTable& LuaType<glm::vec3>::set(
    LuaTable *table, unsigned int index, const glm::vec3 &value
  )
  {
    if (table->table_)
    {
      LuaTable t = table->createTable(index);
      for (unsigned int i = 0; i < 3; i++)
        t.set(i+1, value[i]);
    }
    return *table;
  }

  template <>
  inline LuaTable& LuaType<glm::vec4>::set(
    LuaTable *table, const std::string &name, const glm::vec4 &value
  )
  {
    if (table->table_)
    {
      LuaTable t = table->createTable(name);
      for (unsigned int i = 0; i < 4; i++)
        t.set(i+1, value[i]);
    }
    return *table;
  }

  template <>
  inline LuaTable& LuaType<glm::vec4>::set(
    LuaTable *table, unsigned int index, const glm::vec4 &value
  )
  {
    if (table->table_)
    {
      LuaTable t = table->createTable(index);
      for (unsigned int i = 0; i < 4; i++)
        t.set(i+1, value[i]);
    }
    return *table;
  }

  template <>
  inline bool LuaType<glm::vec2>::is(const sol_object &object)
  {
    return object.is<sol::table>();
  }

  template <>
  inline bool LuaType<glm::vec3>::is(const sol_object &object)
  {
    return object.is<sol::table>();
  }

  template <>
  inline bool LuaType<glm::vec4>::is(const sol_object &object)
  {
    return object.is<sol::table>();
  }

  template <>
  inline glm::vec2 LuaType<glm::vec2>::as(
    std::shared_ptr<sol::state> state, const sol_object &object, const std::string &name,
    const glm::vec2 &def, bool silent
  )
  {
    LuaTable table = LuaType<LuaTable>::as(state, object, name, LuaTable(), silent);
    glm::vec2 vec;
    for (int i = 0; i < 2; i++)
      vec[i] = table.get<float>(i+1, def[i], silent);
    return vec;
  }

  template <>
  inline glm::vec3 LuaType<glm::vec3>::as(
    std::shared_ptr<sol::state> state, const sol_object &object, const std::string &name,
    const glm::vec3 &def, bool silent
  )
  {
    LuaTable table = LuaType<LuaTable>::as(state, object, name, LuaTable(), silent);
    glm::vec3 vec;
    for (int i = 0; i < 3; i++)
      vec[i] = table.get<float>(i+1, def[i], silent);
    return vec;
  }

  template <>
  inline glm::vec4 LuaType<glm::vec4>::as(
    std::shared_ptr<sol::state> state, const sol_object &object, const std::string &name,
    const glm::vec4 &def, bool silent
  )
  {
    LuaTable table = LuaType<LuaTable>::as(state, object, name, LuaTable(), silent);
    glm::vec4 vec;
    for (int i = 0; i < 4; i++)
      vec[i] = table.get<float>(i+1, def[i], silent);
    return vec;
  }

  template <>
  inline LuaObject LuaType<LuaObject>::as(
    std::shared_ptr<sol::state> state, const sol_object &object, const std::string &name,
    const LuaObject &def, bool silent
  )
  {
    UNUSED(def);
    UNUSED(silent);
    return LuaObject(state, object, name);
  }

  template <>
  inline void LuaType<glm::vec2>::write(
    LuaFile *file, const std::string &name, const glm::vec2 &value, bool comma
  )
  {
    file->file << std::string(file->indentation, ' ') << name << " = {"
      << std::fixed << std::setprecision(2) << value.x << ", "
      << std::fixed << std::setprecision(2) << value.y << (comma ? "}," : "}")
      << std::endl;
  }

  template <>
  inline void LuaType<glm::vec2>::write(
    LuaFile *file, const glm::vec2 &value, bool comma
  )
  {
    file->file << std::string(file->indentation, ' ') << "{"
      << std::fixed << std::setprecision(2) << value.x << ", "
      << std::fixed << std::setprecision(2) << value.y << (comma ? "}," : "}")
      << std::endl;
  }

  template <>
  inline void LuaType<glm::vec3>::write(
    LuaFile *file, const std::string &name, const glm::vec3 &value, bool comma
  )
  {
    file->file << std::string(file->indentation, ' ') << name << " = {"
      << std::fixed << std::setprecision(2) << value.x << ", "
      << std::fixed << std::setprecision(2) << value.y << ", "
      << std::fixed << std::setprecision(2) << value.z << (comma ? "}," : "}")
      << std::endl;
  }

  template <>
  inline void LuaType<glm::vec3>::write(
    LuaFile *file, const glm::vec3 &value, bool comma
  )
  {
    file->file << std::string(file->indentation, ' ') << "{"
      << std::fixed << std::setprecision(2) << value.x << ", "
      << std::fixed << std::setprecision(2) << value.y << ", "
      << std::fixed << std::setprecision(2) << value.z << (comma ? "}," : "}")
      << std::endl;
  }

  template <>
  inline void LuaType<glm::vec4>::write(
    LuaFile *file, const std::string &name, const glm::vec4 &value, bool comma
  )
  {
    file->file << std::string(file->indentation, ' ') << name << " = {"
      << std::fixed << std::setprecision(2) << value.x << ", "
      << std::fixed << std::setprecision(2) << value.y << ", "
      << std::fixed << std::setprecision(2) << value.z << ", "
      << std::fixed << std::setprecision(2) << value.w << (comma ? "}," : "}")
      << std::endl;
  }

  template <>
  inline void LuaType<glm::vec4>::write(
    LuaFile *file, const glm::vec4 &value, bool comma
  )
  {
    file->file << std::string(file->indentation, ' ') << "{"
      << std::fixed << std::setprecision(2) << value.x << ", "
      << std::fixed << std::setprecision(2) << value.y << ", "
      << std::fixed << std::setprecision(2) << value.z << ", "
      << std::fixed << std::setprecision(2) << value.w << (comma ? "}," : "}")
      << std::endl;
  }


  template <class T>
  class LuaType<std::vector<T>>
  {
  public:
    static const std::vector<T> getDefault() { return std::vector<T>(); }
    static const std::string getErrorName() { return "Not a list"; }

    LuaTable& set(LuaTable *table, const std::string &name, const std::vector<T> &value)
    {
      if (table->table_)
      {
        LuaTable t = table->createTable(name);
        for (unsigned int i = 0; i < value.size(); i++)
        {
          t.set(i+1, value[i]);
        }
      }
      return *table;
    }

    LuaTable &set(LuaTable *table, unsigned int index, const std::vector<T> &value)
    {
      if (table->table_)
      {
        LuaTable t = table->createTable(index);
        for (unsigned int i = 0; i < value.size(); i++)
        {
          t.set(i+1, value[i]);
        }
      }
      return *table;
    }

    static std::vector<T> get(
      const LuaTable *table, const std::string &name,
      const std::vector<T> &def, bool silent
    )
    {
      if (table->table_)
      {
        return as(
          table->lua_, table->table_[name], table->name_ + name, def, silent
        );
      }
      return def;
    }

    static std::vector<T> get(
      const LuaTable *table, unsigned int index, const std::vector<T> &def, bool silent
    )
    {
      if (table->table_)
      {
        return as(
          table->lua_, table->table_[index],
          table->name_ + std::to_string(index), def, silent
        );
      }
      return def;
    }

    static bool is(const sol_object &object) { return LuaType<LuaTable>::is(object); }

    static std::vector<T> as(
      std::shared_ptr<sol::state> state, const sol_object &object, const std::string &name,
      const std::vector<T> &def, bool silent
    )
    {
      LuaTable table2 = LuaType<LuaTable>::as(state, object, name, LuaTable(), silent);
      if (table2.table_)
      {
        std::vector<T> vec;
        for (unsigned int i = 1; i <= table2.size(); i++)
        {
          vec.push_back(table2.get<T>(i));
        }
        return vec;
      }
      return def;
    }

    static void write(LuaFile *file, const std::vector<T> &value, bool comma)
    {
      file->beginTable();
      for (auto v : value)
      {
        file->beginTable();
        file->write(v.second);
        file->closeTable();
      }
      file->closeTable(comma);
    }

    static void write(
      LuaFile *file, const std::string &name, const std::vector<T> &value, bool comma
    )
    {
      file->beginTable(name);
      for (auto v : value)
      {
        file->beginTable();
        file->write(v.second);
        file->closeTable();
      }
      file->closeTable(comma);
    }
  };

  inline LuaIterator::LuaIterator(const LuaTable *table, const sol::table *t, bool end)
    : iter_(table->valid() ? end ? t->end() : t->begin() : sol_iterator()), table_(table)
  {}

  template <class T>
  T LuaObject::getAs(const T &def, bool silent)
  {
    if (valid())
      return LuaType<T>::as(lua_, object_, name_, def, silent);
    return def;
  }

  inline std::pair<LuaObject, LuaObject> LuaIterator::operator*()
  {
    return std::pair<LuaObject, LuaObject>(
      LuaObject(table_->lua_, (*iter_).first, table_->name_ + "iterable"),
      LuaObject(table_->lua_, (*iter_).second, table_->name_ + "iterable")
    );
  }
}

#endif // __TARBORA_LUA_H_
