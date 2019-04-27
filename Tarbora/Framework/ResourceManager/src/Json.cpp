#include "../inc/Json.hpp"
#include "../../EventManager/inc/EventManager.hpp"

namespace Tarbora {
    void Json::PushErrName(std::string name)
    {
        m_ErrName += (name + ".");
    }

    void Json::PopErrName()
    {
        std::size_t found = m_ErrName.find_last_of(".", m_ErrName.size()-2);
        m_ErrName = m_ErrName.substr(0,found+1);
    }

    void Json::Get(const char * key, json *target, JsonOptions options)
    {
        Get(m_JSON, key, target, options);
    }

    void Json::Get(const char * key, bool *target, JsonOptions options)
    {
        Get(m_JSON, key, target, options);
    }

    void Json::Get(const char * key, int *target, JsonOptions options)
    {
        Get(m_JSON, key, target, options);
    }

    void Json::Get(const char * key, float *target, JsonOptions options)
    {
        Get(m_JSON, key, target, options);
    }

    void Json::Get(const char * key, unsigned int *target, JsonOptions options)
    {
        Get(m_JSON, key, target, options);
    }

    void Json::Get(const char * key, std::string *target, JsonOptions options)
    {
        Get(m_JSON, key, target, options);
    }

    void Json::Get(json j, const char * key, json *target, JsonOptions options)
    {
        json r = j[key];

        if (r.is_null())
        {
            if (options.optional)
            {
                if (!options.silent)
                    LOG_WARN("Json: Could not find key \"%s%s\" in file \"%s\". This is marked as optional.", m_ErrName.c_str(), key, m_Name.c_str());
            }
            else
            {
                LOG_ERR("Json: Could not find key \"%s%s\" in file \"%s\".", m_ErrName.c_str(), key, m_Name.c_str());
                WindowCloseEvent ev;
                EventManager::Trigger(EventType::WindowClose, &ev);
            }
        }
        else
        {
            *target = r;
        }
    }

    void Json::Get(json j, const char * key, bool *target, JsonOptions options)
    {
        json r;
        Get(j, key, &r, options);

        if (!r.is_null())
        {
            if (r.is_boolean())
            {
                *target = r;
            }
            else
            {
                if (options.optional)
                {
                    if (!options.silent)
                        LOG_WARN("Json: Key \"%s%s\" in file \"%s\" should be boolean but is \"%s\". This is marked as optional.", m_ErrName.c_str(), key, m_Name.c_str(), r.type_name());
                }
                else
                {
                    LOG_ERR("Json: Key \"%s%s\" in file \"%s\" should be boolean but is \"%s\".", m_ErrName.c_str(), key, m_Name.c_str(), r.type_name());
                    WindowCloseEvent ev;
                    EventManager::Trigger(EventType::WindowClose, &ev);
                }
            }
        }
    }

    void Json::Get(json j, const char * key, int *target, JsonOptions options)
    {
        json r;
        Get(j, key, &r, options);

        if (!r.is_null())
        {
            if (r.is_number())
            {
                if (r.is_number_float())
                {
                    if (!options.silent)
                        LOG_WARN("Json: Key \"%s%s\" in file \"%s\" should be an integer but is \"float\". An automatic conversion to integer has been made.", m_ErrName.c_str(), key, m_Name.c_str());
                }
                *target = r;
            }
            else
            {
                if (options.optional)
                {
                    if (!options.silent)
                        LOG_WARN("Json: Key \"%s%s\" in file \"%s\" should be an integer but is \"%s\". This is marked as optional.", m_ErrName.c_str(), key, m_Name.c_str(), r.type_name());
                }
                else
                {
                    LOG_ERR("Json: Key \"%s%s\" in file \"%s\" should be an integer but is \"%s\".", m_ErrName.c_str(), key, m_Name.c_str(), r.type_name());
                    WindowCloseEvent ev;
                    EventManager::Trigger(EventType::WindowClose, &ev);
                }
            }
        }
    }

    void Json::Get(json j, const char * key, float *target, JsonOptions options)
    {
        json r;
        Get(j, key, &r, options);

        if (!r.is_null())
        {
            if (r.is_number())
            {
                *target = r;
            }
            else
            {
                if (options.optional)
                {
                    if (!options.silent)
                        LOG_WARN("Json: Key \"%s%s\" in file \"%s\" should be a float but is \"%s\". This is marked as optional.", m_ErrName.c_str(), key, m_Name.c_str(), r.type_name());
                }
                else
                {
                    LOG_ERR("Json: Key \"%s%s\" in file \"%s\" should be a float but is \"%s\".", m_ErrName.c_str(), key, m_Name.c_str(), r.type_name());
                    WindowCloseEvent ev;
                    EventManager::Trigger(EventType::WindowClose, &ev);
                }
            }
        }
    }

    void Json::Get(json j, const char * key, unsigned int *target, JsonOptions options)
    {
        json r;
        Get(j, key, &r, options);

        if (!r.is_null())
        {
            if (!r.is_number())
            {
                if (r.is_number_float())
                {
                    if (!options.silent)
                        LOG_WARN("Json: Key \"%s%s\" in file \"%s\" should be an unsigned integer but is \"float\". An automatic conversion to unsigned integer has been made.", m_ErrName.c_str(), key, m_Name.c_str());
                }
                else if (!r.is_number_unsigned())
                {
                    if (!options.silent)
                        LOG_WARN("Json: Key \"%s%s\" in file \"%s\" should be an unsigned integer but is \"integer\". An automatic conversion to unsigned integer has been made.", m_ErrName.c_str(), key, m_Name.c_str());
                }
                *target = r;
            }
            else
            {
                if (options.optional)
                {
                    if (!options.silent)
                        LOG_WARN("Json: Key \"%s%s\" in file \"%s\" should be an unsigned integer but is \"%s\". This is marked as optional.", m_ErrName.c_str(), key, m_Name.c_str(), r.type_name());
                }
                else
                {
                    LOG_ERR("Json: Key \"%s%s\" in file \"%s\" should be an unsigned integer but is \"%s\".", m_ErrName.c_str(), key, m_Name.c_str(), r.type_name());
                    WindowCloseEvent ev;
                    EventManager::Trigger(EventType::WindowClose, &ev);
                }
            }
        }
    }

    void Json::Get(json j, const char * key, std::string *target, JsonOptions options)
    {
        json r;
        Get(j, key, &r, options);

        if (!r.is_null())
        {
            if (r.is_string())
            {
                *target = r;
            }
            else
            {
                if (options.optional)
                {
                    if (!options.silent)
                        LOG_WARN("Json: Key \"%s%s\" in file \"%s\" should be a string but is \"%s\". This is marked as optional.", m_ErrName.c_str(), key, m_Name.c_str(), r.type_name());
                }
                else
                {
                    LOG_ERR("Json: Key \"%s%s\" in file \"%s\" should be a string but is \"%s\".", m_ErrName.c_str(), key, m_Name.c_str(), r.type_name());
                    WindowCloseEvent ev;
                    EventManager::Trigger(EventType::WindowClose, &ev);
                }
            }
        }
    }

    void Json::Get(json j, int key, json *target, JsonOptions options)
    {
        if (j.is_array())
        {
            if (j.size() > (unsigned int)key)
            {
                *target = j[key];
            }
            else
            {
                if (options.optional)
                {
                    if (!options.silent)
                        LOG_WARN("Json: Could not find key \"%s%s\" in file \"%s\". This is marked as optional.", m_ErrName.c_str(), key, m_Name.c_str());
                }
                else
                {
                    LOG_ERR("Json: Could not find key \"%s%s\" in file \"%s\".", m_ErrName.c_str(), key, m_Name.c_str());
                    WindowCloseEvent ev;
                    EventManager::Trigger(EventType::WindowClose, &ev);
                }
            }
        }
        else
        {
            if (options.optional)
            {
                if (!options.silent)
                    LOG_WARN("Json: Key \"%s.%d\" in file \"%s\" should be an array but is \"%s\". This is marked as optional.", m_ErrName.c_str(), key, m_Name.c_str(), j.type_name());
            }
            else
            {
                LOG_ERR("Json: Key \"%s.%d\" in file \"%s\" should be an array but is \"%s\".", m_ErrName.c_str(), key, m_Name.c_str(), j.type_name());
                WindowCloseEvent ev;
                EventManager::Trigger(EventType::WindowClose, &ev);
            }
        }

        json r = j[key];

        if (r.is_null())
        {
            if (options.optional)
            {
                if (!options.silent)
                    LOG_WARN("Json: Could not find key \"%s%d\" in file \"%s\". This is marked as optional.", m_ErrName.c_str(), key, m_Name.c_str());
            }
            else
            {
                LOG_ERR("Json: Could not find key \"%s%d\" in file \"%s\".", m_ErrName.c_str(), key, m_Name.c_str());
                WindowCloseEvent ev;
                EventManager::Trigger(EventType::WindowClose, &ev);
            }
        }
        else
        {
            *target = r;
        }
    }

    void Json::Get(json j, int key, bool *target, JsonOptions options)
    {
        json r;
        Get(j, key, &r, options);

        if (!r.is_null())
        {
            if (r.is_boolean())
            {
                *target = r;
            }
            else
            {
                if (options.optional)
                {
                    if (!options.silent)
                        LOG_WARN("Json: Key \"%s%d\" in file \"%s\" should be boolean but is \"%s\". This is marked as optional.", m_ErrName.c_str(), key, m_Name.c_str(), r.type_name());
                }
                else
                {
                    LOG_ERR("Json: Key \"%s%d\" in file \"%s\" should be boolean but is \"%s\".", m_ErrName.c_str(), key, m_Name.c_str(), r.type_name());
                    WindowCloseEvent ev;
                    EventManager::Trigger(EventType::WindowClose, &ev);
                }
            }
        }
    }

    void Json::Get(json j, int key, int *target, JsonOptions options)
    {
        json r;
        Get(j, key, &r, options);

        if (!r.is_null())
        {
            if (r.is_number())
            {
                if (r.is_number_float())
                {
                    if (!options.silent)
                        LOG_WARN("Json: Key \"%s%d\" in file \"%s\" should be an integer but is \"float\". An automatic conversion to integer has been made.", m_ErrName.c_str(), key, m_Name.c_str());
                }
                *target = r;
            }
            else
            {
                if (options.optional)
                {
                    if (!options.silent)
                        LOG_WARN("Json: Key \"%s%d\" in file \"%s\" should be an integer but is \"%s\". This is marked as optional.", m_ErrName.c_str(), key, m_Name.c_str(), r.type_name());
                }
                else
                {
                    LOG_ERR("Json: Key \"%s%d\" in file \"%s\" should be an integer but is \"%s\".", m_ErrName.c_str(), key, m_Name.c_str(), r.type_name());
                    WindowCloseEvent ev;
                    EventManager::Trigger(EventType::WindowClose, &ev);
                }
            }
        }
    }

    void Json::Get(json j, int key, float *target, JsonOptions options)
    {
        json r;
        Get(j, key, &r, options);

        if (!r.is_null())
        {
            if (r.is_number())
            {
                *target = r;
            }
            else
            {
                if (options.optional)
                {
                    if (!options.silent)
                        LOG_WARN("Json: Key \"%s%d\" in file \"%s\" should be a float but is \"%s\". This is marked as optional.", m_ErrName.c_str(), key, m_Name.c_str(), r.type_name());
                }
                else
                {
                    LOG_ERR("Json: Key \"%s%d\" in file \"%s\" should be a float but is \"%s\".", m_ErrName.c_str(), key, m_Name.c_str(), r.type_name());
                    WindowCloseEvent ev;
                    EventManager::Trigger(EventType::WindowClose, &ev);
                }
            }
        }
    }

    void Json::Get(json j, int key, unsigned int *target, JsonOptions options)
    {
        json r;
        Get(j, key, &r, options);

        if (!r.is_null())
        {
            if (!r.is_number())
            {
                if (r.is_number_float())
                {
                    if (!options.silent)
                        LOG_WARN("Json: Key \"%s%d\" in file \"%s\" should be an unsigned integer but is \"float\". An automatic conversion to unsigned integer has been made.", m_ErrName.c_str(), key, m_Name.c_str());
                }
                else if (!r.is_number_unsigned())
                {
                    if (!options.silent)
                        LOG_WARN("Json: Key \"%s%d\" in file \"%s\" should be an unsigned integer but is \"integer\". An automatic conversion to unsigned integer has been made.", m_ErrName.c_str(), key, m_Name.c_str());
                }
                *target = r;
            }
            else
            {
                if (options.optional)
                {
                    if (!options.silent)
                        LOG_WARN("Json: Key \"%s%d\" in file \"%s\" should be an unsigned integer but is \"%s\". This is marked as optional.", m_ErrName.c_str(), key, m_Name.c_str(), r.type_name());
                }
                else
                {
                    LOG_ERR("Json: Key \"%s%d\" in file \"%s\" should be an unsigned integer but is \"%s\".", m_ErrName.c_str(), key, m_Name.c_str(), r.type_name());
                    WindowCloseEvent ev;
                    EventManager::Trigger(EventType::WindowClose, &ev);
                }
            }
        }
    }

    void Json::Get(json j, int key, std::string *target, JsonOptions options)
    {
        json r;
        Get(j, key, &r, options);

        if (!r.is_null())
        {
            if (r.is_string())
            {
                *target = r;
            }
            else
            {
                if (options.optional)
                {
                    if (!options.silent)
                        LOG_WARN("Json: Key \"%s%d\" in file \"%s\" should be a string but is \"%s\". This is marked as optional.", m_ErrName.c_str(), key, m_Name.c_str(), r.type_name());
                }
                else
                {
                    LOG_ERR("Json: Key \"%s%d\" in file \"%s\" should be a string but is \"%s\".", m_ErrName.c_str(), key, m_Name.c_str(), r.type_name());
                    WindowCloseEvent ev;
                    EventManager::Trigger(EventType::WindowClose, &ev);
                }
            }
        }
    }

    void Json::GetArray(const char * key, int i, json *target, JsonOptions options)
    {
        GetArray(m_JSON, key, i, target, options);
    }

    void Json::GetArray(const char * key, int i, bool *target, JsonOptions options)
    {
        GetArray(m_JSON, key, i, target, options);
    }

    void Json::GetArray(const char * key, int i, int *target, JsonOptions options)
    {
        GetArray(m_JSON, key, i, target, options);
    }

    void Json::GetArray(const char * key, int i, float *target, JsonOptions options)
    {
        GetArray(m_JSON, key, i, target, options);
    }

    void Json::GetArray(const char * key, int i, unsigned int *target, JsonOptions options)
    {
        GetArray(m_JSON, key, i, target, options);
    }

    void Json::GetArray(const char * key, int i, std::string *target, JsonOptions options)
    {
        GetArray(m_JSON, key, i, target, options);
    }

    void Json::GetArray(json j, const char * key, int i, json *target, JsonOptions options)
    {
        json r;
        Get(j, key, &r, options);

        if (!r.is_null())
        {
            if (r.is_array())
            {
                if (r.size() > (unsigned int)i)
                {
                    *target = r[i];
                }
                else
                {
                    if (options.optional)
                    {
                        if (!options.silent)
                            LOG_WARN("Json: Array \"%s%s\" in file \"%s\" has no index \"%d\". This is marked as optional.", m_ErrName.c_str(), key, m_Name.c_str(), i);
                    }
                    else
                    {
                        LOG_ERR("Json: Array \"%s%s\" in file \"%s\" has no index \"%d\".", m_ErrName.c_str(), key, m_Name.c_str(), i);
                        WindowCloseEvent ev;
                        EventManager::Trigger(EventType::WindowClose, &ev);
                    }
                }
            }
            else
            {
                if (options.optional)
                {
                    if (!options.silent)
                        LOG_WARN("Json: Key \"%s%s\" in file \"%s\" should be an array but is \"%s\". This is marked as optional.", m_ErrName.c_str(), key, m_Name.c_str(), r.type_name());
                }
                else
                {
                    LOG_ERR("Json: Key \"%s%s\" in file \"%s\" should be an array but is \"%s\".", m_ErrName.c_str(), key, m_Name.c_str(), r.type_name());
                    WindowCloseEvent ev;
                    EventManager::Trigger(EventType::WindowClose, &ev);
                }
            }
        }
    }

    void Json::GetArray(json j, const char * key, int i, bool *target, JsonOptions options)
    {
        json r;
        GetArray(j, key, i, &r, options);

        if (!r.is_null())
        {
            if (r.is_boolean())
            {
                *target = r;
            }
            else
            {
                if (options.optional)
                {
                    if (!options.silent)
                        LOG_WARN("Json: Array item \"%s%s.%d\" in file \"%s\" should be boolean but is \"%s\". This is marked as optional.", m_ErrName.c_str(), key, i, m_Name.c_str(), r.type_name());
                }
                else
                {
                    LOG_ERR("Json: Array item \"%s%s.%d\" in file \"%s\" should be boolean but is \"%s\".", m_ErrName.c_str(), key, i, m_Name.c_str(), r.type_name());
                    WindowCloseEvent ev;
                    EventManager::Trigger(EventType::WindowClose, &ev);
                }
            }
        }
    }

    void Json::GetArray(json j, const char * key, int i, int *target, JsonOptions options)
    {
        json r;
        GetArray(j, key, i, &r, options);

        if (!r.is_null())
        {
            if (r.is_number())
            {
                if (r.is_number_float())
                {
                    if (!options.silent)
                        LOG_WARN("Json: Array item \"%s%s.%d\" in file \"%s\" should be an integer but is \"float\". An automatic conversion to integer has been made.", m_ErrName.c_str(), key, i, m_Name.c_str());
                }
                *target = r;
            }
            else
            {
                if (options.optional)
                {
                    if (!options.silent)
                        LOG_WARN("Json: Array item \"%s%s.%d\" in file \"%s\" should be an integer but is \"%s\". This is marked as optional.", m_ErrName.c_str(), key, i, m_Name.c_str(), r.type_name());
                }
                else
                {
                    LOG_ERR("Json: Array item \"%s%s.%d\" in file \"%s\" should be an integer but is \"%s\".", m_ErrName.c_str(), key, i, m_Name.c_str(), r.type_name());
                    WindowCloseEvent ev;
                    EventManager::Trigger(EventType::WindowClose, &ev);
                }
            }
        }
    }

    void Json::GetArray(json j, const char * key, int i, float *target, JsonOptions options)
    {
        json r;
        GetArray(j, key, i, &r, options);

        if (!r.is_null())
        {
            if (r.is_number())
            {
                *target = r;
            }
            else
            {
                if (options.optional)
                {
                    if (!options.silent)
                        LOG_WARN("Json: Array item \"%s%s.%d\" in file \"%s\" should be a float but is \"%s\". This is marked as optional.", m_ErrName.c_str(), key, i, m_Name.c_str(), r.type_name());
                }
                else
                {
                    LOG_ERR("Json: Array item \"%s%s.%d\" in file \"%s\" should be a float but is \"%s\".", m_ErrName.c_str(), key, i, m_Name.c_str(), r.type_name());
                    WindowCloseEvent ev;
                    EventManager::Trigger(EventType::WindowClose, &ev);
                }
            }
        }
    }

    void Json::GetArray(json j, const char * key, int i, unsigned int *target, JsonOptions options)
    {
        json r;
        GetArray(j, key, i, &r, options);

        if (!r.is_null())
        {
            if (!r.is_number())
            {
                if (r.is_number_float())
                {
                    if (!options.silent)
                        LOG_WARN("Json: Array item \"%s%s.%d\" in file \"%s\" should be an unsigned integer but is \"float\". An automatic conversion to unsigned integer has been made.", m_ErrName.c_str(), key, i, m_Name.c_str());
                }
                else if (!r.is_number_unsigned())
                {
                    if (!options.silent)
                        LOG_WARN("Json: Array item \"%s%s.%d\" in file \"%s\" should be an unsigned integer but is \"integer\". An automatic conversion to unsigned integer has been made.", m_ErrName.c_str(), key, i, m_Name.c_str());
                }
                *target = r;
            }
            else
            {
                if (options.optional)
                {
                    if (!options.silent)
                        LOG_WARN("Json: Array item \"%s%s.%d\" in file \"%s\" should be an unsigned integer but is \"%s\". This is marked as optional.", m_ErrName.c_str(), key, i, m_Name.c_str(), r.type_name());
                }
                else
                {
                    LOG_ERR("Json: Array item \"%s%s.%d\" in file \"%s\" should be an unsigned integer but is \"%s\".", m_ErrName.c_str(), key, i, m_Name.c_str(), r.type_name());
                    WindowCloseEvent ev;
                    EventManager::Trigger(EventType::WindowClose, &ev);
                }
            }
        }
    }

    void Json::GetArray(json j, const char * key, int i, std::string *target, JsonOptions options)
    {
        json r;
        GetArray(j, key, i, &r, options);

        if (!r.is_null())
        {
            if (r.is_string())
            {
                *target = r;
            }
            else
            {
                if (options.optional)
                {
                    if (!options.silent)
                        LOG_WARN("Json: Array item \"%s%s.%d\" in file \"%s\" should be a string but is \"%s\". This is marked as optional.", m_ErrName.c_str(), key, i, m_Name.c_str(), r.type_name());
                }
                else
                {
                    LOG_ERR("Json: Array item \"%s%s.%d\" in file \"%s\" should be a string but is \"%s\".", m_ErrName.c_str(), key, i, m_Name.c_str(), r.type_name());
                    WindowCloseEvent ev;
                    EventManager::Trigger(EventType::WindowClose, &ev);
                }
            }
        }
    }

    json Json::GetJson(const char * key, JsonOptions options)
    {
        json r;
        Get(key, &r, options);
        return r;
    }

    bool Json::GetBool(const char * key, JsonOptions options)
    {
        bool r;
        Get(key, &r, options);
        return r;
    }

    int Json::GetInt(const char * key, JsonOptions options)
    {
        int r;
        Get(key, &r, options);
        return r;
    }

    float Json::GetFloat(const char * key, JsonOptions options)
    {
        float r;
        Get(key, &r, options);
        return r;
    }

    unsigned int Json::GetUInt(const char * key, JsonOptions options)
    {
        unsigned int r;
        Get(key, &r, options);
        return r;
    }

    std::string Json::GetString(const char * key, JsonOptions options)
    {
        std::string r;
        Get(key, &r, options);
        return r;
    }

    json Json::GetJson(json j, const char * key, JsonOptions options)
    {
        json r;
        Get(j, key, &r, options);
        return r;
    }

    bool Json::GetBool(json j, const char * key, JsonOptions options)
    {
        bool r;
        Get(j, key, &r, options);
        return r;
    }

    int Json::GetInt(json j, const char * key, JsonOptions options)
    {
        int r;
        Get(j, key, &r, options);
        return r;
    }

    float Json::GetFloat(json j, const char * key, JsonOptions options)
    {
        float r;
        Get(j, key, &r, options);
        return r;
    }

    unsigned int Json::GetUInt(json j, const char * key, JsonOptions options)
    {
        unsigned int r;
        Get(j, key, &r, options);
        return r;
    }

    std::string Json::GetString(json j, const char * key, JsonOptions options)
    {
        std::string r;
        Get(j, key, &r, options);
        return r;
    }

    json Json::GetJson(json j, int key, JsonOptions options)
    {
        json r;
        Get(j, key, &r, options);
        return r;
    }

    bool Json::GetBool(json j, int key, JsonOptions options)
    {
        bool r;
        Get(j, key, &r, options);
        return r;
    }

    int Json::GetInt(json j, int key, JsonOptions options)
    {
        int r;
        Get(j, key, &r, options);
        return r;
    }

    float Json::GetFloat(json j, int key, JsonOptions options)
    {
        float r;
        Get(j, key, &r, options);
        return r;
    }

    unsigned int Json::GetUInt(json j, int key, JsonOptions options)
    {
        unsigned int r;
        Get(j, key, &r, options);
        return r;
    }

    std::string Json::GetString(json j, int key, JsonOptions options)
    {
        std::string r;
        Get(j, key, &r, options);
        return r;
    }

    json Json::GetJsonArray(const char * key, int i, JsonOptions options)
    {
        json r;
        GetArray(key, i, &r, options);
        return r;
    }

    bool Json::GetBoolArray(const char * key, int i, JsonOptions options)
    {
        bool r;
        GetArray(key, i, &r, options);
        return r;
    }

    int Json::GetIntArray(const char * key, int i, JsonOptions options)
    {
        int r;
        GetArray(key, i, &r, options);
        return r;
    }

    float Json::GetFloatArray(const char * key, int i, JsonOptions options)
    {
        float r;
        GetArray(key, i, &r, options);
        return r;
    }

    unsigned int Json::GetUIntArray(const char * key, int i, JsonOptions options)
    {
        unsigned int r;
        GetArray(key, i, &r, options);
        return r;
    }

    std::string Json::GetStringArray(const char * key, int i, JsonOptions options)
    {
        std::string r;
        GetArray(key, i, &r, options);
        return r;
    }

    json Json::GetJsonArray(json j, const char * key, int i, JsonOptions options)
    {
        json r;
        GetArray(j, key, i, &r, options);
        return r;
    }

    bool Json::GetBoolArray(json j, const char * key, int i, JsonOptions options)
    {
        bool r;
        GetArray(j, key, i, &r, options);
        return r;
    }

    int Json::GetIntArray(json j, const char * key, int i, JsonOptions options)
    {
        int r;
        GetArray(j, key, i, &r, options);
        return r;
    }

    float Json::GetFloatArray(json j, const char * key, int i, JsonOptions options)
    {
        float r;
        GetArray(j, key, i, &r, options);
        return r;
    }

    unsigned int Json::GetUIntArray(json j, const char * key, int i, JsonOptions options)
    {
        unsigned int r;
        GetArray(j, key, i, &r, options);
        return r;
    }

    std::string Json::GetStringArray(json j, const char * key, int i, JsonOptions options)
    {
        std::string r;
        GetArray(j, key, i, &r, options);
        return r;
    }

    ResourcePtr JsonResourceLoader::Load(std::string path)
    {
        std::ifstream file;
        file.open(path.c_str());
        if (file.fail())
        {
            return ResourcePtr();
        }

        json data;
        try
        {
            data = json::parse(file);
        }
        catch (json::parse_error& e)
        {
            LOG_ERR("JsonResourceLoader: Trying to parse file \"%s\" found exception: \n \"%s\"", path.c_str(), e.what());
            file.close();
            return ResourcePtr();
        }

        ResourcePtr r = ResourcePtr(new Json(path, data));
        file.close();
        return r;
    }
}
