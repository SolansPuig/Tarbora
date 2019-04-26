#include "../inc/Json.hpp"
#include "../../EventManager/inc/EventManager.hpp"

namespace Tarbora {
    void Json::Get(const char * key, json *target, bool optional, bool silent)
    {
        Get(m_JSON, key, target, optional, silent);
    }

    void Json::Get(const char * key, bool *target, bool optional, bool silent)
    {
        Get(m_JSON, key, target, optional, silent);
    }

    void Json::Get(const char * key, int *target, bool optional, bool silent)
    {
        Get(m_JSON, key, target, optional, silent);
    }

    void Json::Get(const char * key, float *target, bool optional, bool silent)
    {
        Get(m_JSON, key, target, optional, silent);
    }

    void Json::Get(const char * key, unsigned int *target, bool optional, bool silent)
    {
        Get(m_JSON, key, target, optional, silent);
    }

    void Json::Get(const char * key, std::string *target, bool optional, bool silent)
    {
        Get(m_JSON, key, target, optional, silent);
    }

    void Json::Get(json j, const char * key, json *target, bool optional, bool silent)
    {
        json r = j[key];

        if (r.is_null())
        {
            if (optional)
            {
                if (!silent)
                    LOG_WARN("Json: Could not find key \"%s\" in file \"%s\". This is marked as optional.", key, m_Name.c_str());
            }
            else
            {
                LOG_ERR("Json: Could not find key \"%s\" in file \"%s\".", key, m_Name.c_str());
                WindowCloseEvent ev;
                EventManager::Trigger(EventType::WindowClose, &ev);
            }
        }
        else
        {
            *target = r;
        }
    }

    void Json::Get(json j, const char * key, bool *target, bool optional, bool silent)
    {
        json r;
        Get(j, key, &r, optional, silent);

        if (!r.is_null())
        {
            if (r.is_boolean())
            {
                *target = r;
            }
            else
            {
                if (optional)
                {
                    if (!silent)
                        LOG_WARN("Json: Key \"%s\" in file \"%s\" should be boolean but is \"%s\". This is marked as optional.", key, m_Name.c_str(), r.type_name());
                }
                else
                {
                    LOG_ERR("Json: Key \"%s\" in file \"%s\" should be boolean but is \"%s\".", key, m_Name.c_str(), r.type_name());
                    WindowCloseEvent ev;
                    EventManager::Trigger(EventType::WindowClose, &ev);
                }
            }
        }
    }

    void Json::Get(json j, const char * key, int *target, bool optional, bool silent)
    {
        json r;
        Get(j, key, &r, optional, silent);

        if (!r.is_null())
        {
            if (r.is_number())
            {
                if (r.is_number_float())
                {
                    if (!silent)
                        LOG_WARN("Json: Key \"%s\" in file \"%s\" should be an integer but is \"float\". An automatic conversion to integer has been made.", key, m_Name.c_str());
                }
                *target = r;
            }
            else
            {
                if (optional)
                {
                    if (!silent)
                        LOG_WARN("Json: Key \"%s\" in file \"%s\" should be an integer but is \"%s\". This is marked as optional.", key, m_Name.c_str(), r.type_name());
                }
                else
                {
                    LOG_ERR("Json: Key \"%s\" in file \"%s\" should be an integer but is \"%s\".", key, m_Name.c_str(), r.type_name());
                    WindowCloseEvent ev;
                    EventManager::Trigger(EventType::WindowClose, &ev);
                }
            }
        }
    }

    void Json::Get(json j, const char * key, float *target, bool optional, bool silent)
    {
        json r;
        Get(j, key, &r, optional, silent);

        if (!r.is_null())
        {
            if (r.is_number())
            {
                *target = r;
            }
            else
            {
                if (optional)
                {
                    if (!silent)
                        LOG_WARN("Json: Key \"%s\" in file \"%s\" should be a float but is \"%s\". This is marked as optional.", key, m_Name.c_str(), r.type_name());
                }
                else
                {
                    LOG_ERR("Json: Key \"%s\" in file \"%s\" should be a float but is \"%s\".", key, m_Name.c_str(), r.type_name());
                    WindowCloseEvent ev;
                    EventManager::Trigger(EventType::WindowClose, &ev);
                }
            }
        }
    }

    void Json::Get(json j, const char * key, unsigned int *target, bool optional, bool silent)
    {
        json r;
        Get(j, key, &r, optional, silent);

        if (!r.is_null())
        {
            if (!r.is_number())
            {
                if (r.is_number_float())
                {
                    if (!silent)
                        LOG_WARN("Json: Key \"%s\" in file \"%s\" should be an unsigned integer but is \"float\". An automatic conversion to unsigned integer has been made.", key, m_Name.c_str());
                }
                else if (!r.is_number_unsigned())
                {
                    if (!silent)
                        LOG_WARN("Json: Key \"%s\" in file \"%s\" should be an unsigned integer but is \"integer\". An automatic conversion to unsigned integer has been made.", key, m_Name.c_str());
                }
                *target = r;
            }
            else
            {
                if (optional)
                {
                    if (!silent)
                        LOG_WARN("Json: Key \"%s\" in file \"%s\" should be an unsigned integer but is \"%s\". This is marked as optional.", key, m_Name.c_str(), r.type_name());
                }
                else
                {
                    LOG_ERR("Json: Key \"%s\" in file \"%s\" should be an unsigned integer but is \"%s\".", key, m_Name.c_str(), r.type_name());
                    WindowCloseEvent ev;
                    EventManager::Trigger(EventType::WindowClose, &ev);
                }
            }
        }
    }

    void Json::Get(json j, const char * key, std::string *target, bool optional, bool silent)
    {
        json r;
        Get(j, key, &r, optional, silent);

        if (!r.is_null())
        {
            if (r.is_string())
            {
                *target = r;
            }
            else
            {
                if (optional)
                {
                    if (!silent)
                        LOG_WARN("Json: Key \"%s\" in file \"%s\" should be a string but is \"%s\". This is marked as optional.", key, m_Name.c_str(), r.type_name());
                }
                else
                {
                    LOG_ERR("Json: Key \"%s\" in file \"%s\" should be a string but is \"%s\".", key, m_Name.c_str(), r.type_name());
                    WindowCloseEvent ev;
                    EventManager::Trigger(EventType::WindowClose, &ev);
                }
            }
        }
    }

    void Json::Get(json j, int key, json *target, bool optional, bool silent, std::string name)
    {
        if (j.is_array())
        {
            if (j.size() > key)
            {
                *target = j[key];
            }
            else
            {
                if (optional)
                {
                    if (!silent)
                        LOG_WARN("Json: Key \"%s\" in file \"%s\" has no key \"%d\". This is marked as optional.", name.c_str(), m_Name.c_str(), key);
                }
                else
                {
                    LOG_ERR("Json: Key \"%s\" in file \"%s\" has no key \"%d\".", name.c_str(), m_Name.c_str(), key);
                    WindowCloseEvent ev;
                    EventManager::Trigger(EventType::WindowClose, &ev);
                }
            }
        }
        else
        {
            if (optional)
            {
                if (!silent)
                    LOG_WARN("Json: Key \"%s\" in file \"%s\" should be an array but is \"%s\". This is marked as optional.", name.c_str(), m_Name.c_str(), j.type_name());
            }
            else
            {
                LOG_ERR("Json: Key \"%s\" in file \"%s\" should be an array but is \"%s\".", name.c_str(), m_Name.c_str(), j.type_name());
                WindowCloseEvent ev;
                EventManager::Trigger(EventType::WindowClose, &ev);
            }
        }

        json r = j[key];

        if (r.is_null())
        {
            if (optional)
            {
                if (!silent)
                    LOG_WARN("Json: Could not find key \"%s.%d\" in file \"%s\". This is marked as optional.", name.c_str(), key, m_Name.c_str());
            }
            else
            {
                LOG_ERR("Json: Could not find key \"%s.%d\" in file \"%s\".", name.c_str(), key, m_Name.c_str());
                WindowCloseEvent ev;
                EventManager::Trigger(EventType::WindowClose, &ev);
            }
        }
        else
        {
            *target = r;
        }
    }

    void Json::Get(json j, int key, bool *target, bool optional, bool silent, std::string name)
    {
        json r;
        Get(j, key, &r, optional, silent, name);

        if (!r.is_null())
        {
            if (r.is_boolean())
            {
                *target = r;
            }
            else
            {
                if (optional)
                {
                    if (!silent)
                        LOG_WARN("Json: Key \"%s.%d\" in file \"%s\" should be boolean but is \"%s\". This is marked as optional.", name.c_str(), key, m_Name.c_str(), r.type_name());
                }
                else
                {
                    LOG_ERR("Json: Key \"%s.%d\" in file \"%s\" should be boolean but is \"%s\".", name.c_str(), key, m_Name.c_str(), r.type_name());
                    WindowCloseEvent ev;
                    EventManager::Trigger(EventType::WindowClose, &ev);
                }
            }
        }
    }

    void Json::Get(json j, int key, int *target, bool optional, bool silent, std::string name)
    {
        json r;
        Get(j, key, &r, optional, silent, name);

        if (!r.is_null())
        {
            if (r.is_number())
            {
                if (r.is_number_float())
                {
                    if (!silent)
                        LOG_WARN("Json: Key \"%s.%d\" in file \"%s\" should be an integer but is \"float\". An automatic conversion to integer has been made.", name.c_str(), key, m_Name.c_str());
                }
                *target = r;
            }
            else
            {
                if (optional)
                {
                    if (!silent)
                        LOG_WARN("Json: Key \"%s.%d\" in file \"%s\" should be an integer but is \"%s\". This is marked as optional.", name.c_str(), key, m_Name.c_str(), r.type_name());
                }
                else
                {
                    LOG_ERR("Json: Key \"%s.%d\" in file \"%s\" should be an integer but is \"%s\".", name.c_str(), key, m_Name.c_str(), r.type_name());
                    WindowCloseEvent ev;
                    EventManager::Trigger(EventType::WindowClose, &ev);
                }
            }
        }
    }

    void Json::Get(json j, int key, float *target, bool optional, bool silent, std::string name)
    {
        json r;
        Get(j, key, &r, optional, silent, name);

        if (!r.is_null())
        {
            if (r.is_number())
            {
                *target = r;
            }
            else
            {
                if (optional)
                {
                    if (!silent)
                        LOG_WARN("Json: Key \"%s.%d\" in file \"%s\" should be a float but is \"%s\". This is marked as optional.", name.c_str(), key, m_Name.c_str(), r.type_name());
                }
                else
                {
                    LOG_ERR("Json: Key \"%s.%d\" in file \"%s\" should be a float but is \"%s\".", name.c_str(), key, m_Name.c_str(), r.type_name());
                    WindowCloseEvent ev;
                    EventManager::Trigger(EventType::WindowClose, &ev);
                }
            }
        }
    }

    void Json::Get(json j, int key, unsigned int *target, bool optional, bool silent, std::string name)
    {
        json r;
        Get(j, key, &r, optional, silent, name);

        if (!r.is_null())
        {
            if (!r.is_number())
            {
                if (r.is_number_float())
                {
                    if (!silent)
                        LOG_WARN("Json: Key \"%s.%d\" in file \"%s\" should be an unsigned integer but is \"float\". An automatic conversion to unsigned integer has been made.", name.c_str(), key, m_Name.c_str());
                }
                else if (!r.is_number_unsigned())
                {
                    if (!silent)
                        LOG_WARN("Json: Key \"%s.%d\" in file \"%s\" should be an unsigned integer but is \"integer\". An automatic conversion to unsigned integer has been made.", name.c_str(), key, m_Name.c_str());
                }
                *target = r;
            }
            else
            {
                if (optional)
                {
                    if (!silent)
                        LOG_WARN("Json: Key \"%s.%d\" in file \"%s\" should be an unsigned integer but is \"%s\". This is marked as optional.", name.c_str(), key, m_Name.c_str(), r.type_name());
                }
                else
                {
                    LOG_ERR("Json: Key \"%s.%d\" in file \"%s\" should be an unsigned integer but is \"%s\".", name.c_str(), key, m_Name.c_str(), r.type_name());
                    WindowCloseEvent ev;
                    EventManager::Trigger(EventType::WindowClose, &ev);
                }
            }
        }
    }

    void Json::Get(json j, int key, std::string *target, bool optional, bool silent, std::string name)
    {
        json r;
        Get(j, key, &r, optional, silent, name);

        if (!r.is_null())
        {
            if (r.is_string())
            {
                *target = r;
            }
            else
            {
                if (optional)
                {
                    if (!silent)
                        LOG_WARN("Json: Key \"%s.%d\" in file \"%s\" should be a string but is \"%s\". This is marked as optional.", name.c_str(), key, m_Name.c_str(), r.type_name());
                }
                else
                {
                    LOG_ERR("Json: Key \"%s.%d\" in file \"%s\" should be a string but is \"%s\".", name.c_str(), key, m_Name.c_str(), r.type_name());
                    WindowCloseEvent ev;
                    EventManager::Trigger(EventType::WindowClose, &ev);
                }
            }
        }
    }

    void Json::GetArray(const char * key, int i, json *target, bool optional, bool silent)
    {
        GetArray(m_JSON, key, i, target, optional, silent);
    }

    void Json::GetArray(const char * key, int i, bool *target, bool optional, bool silent)
    {
        GetArray(m_JSON, key, i, target, optional, silent);
    }

    void Json::GetArray(const char * key, int i, int *target, bool optional, bool silent)
    {
        GetArray(m_JSON, key, i, target, optional, silent);
    }

    void Json::GetArray(const char * key, int i, float *target, bool optional, bool silent)
    {
        GetArray(m_JSON, key, i, target, optional, silent);
    }

    void Json::GetArray(const char * key, int i, unsigned int *target, bool optional, bool silent)
    {
        GetArray(m_JSON, key, i, target, optional, silent);
    }

    void Json::GetArray(const char * key, int i, std::string *target, bool optional, bool silent)
    {
        GetArray(m_JSON, key, i, target, optional, silent);
    }

    void Json::GetArray(json j, const char * key, int i, json *target, bool optional, bool silent)
    {
        json r;
        Get(j, key, &r, optional, silent);

        if (!r.is_null())
        {
            if (r.is_array())
            {
                if (r.size() > i)
                {
                    *target = r[i];
                }
                else
                {
                    if (optional)
                    {
                        if (!silent)
                            LOG_WARN("Json: Array \"%s\" in file \"%s\" has no index \"%d\". This is marked as optional.", key, m_Name.c_str(), i);
                    }
                    else
                    {
                        LOG_ERR("Json: Array \"%s\" in file \"%s\" has no index \"%d\".", key, m_Name.c_str(), i);
                        WindowCloseEvent ev;
                        EventManager::Trigger(EventType::WindowClose, &ev);
                    }
                }
            }
            else
            {
                if (optional)
                {
                    if (!silent)
                        LOG_WARN("Json: Key \"%s\" in file \"%s\" should be an array but is \"%s\". This is marked as optional.", key, m_Name.c_str(), r.type_name());
                }
                else
                {
                    LOG_ERR("Json: Key \"%s\" in file \"%s\" should be an array but is \"%s\".", key, m_Name.c_str(), r.type_name());
                    WindowCloseEvent ev;
                    EventManager::Trigger(EventType::WindowClose, &ev);
                }
            }
        }
    }

    void Json::GetArray(json j, const char * key, int i, bool *target, bool optional, bool silent)
    {
        json r;
        GetArray(j, key, i, &r, optional, silent);

        if (!r.is_null())
        {
            if (r.is_boolean())
            {
                *target = r;
            }
            else
            {
                if (optional)
                {
                    if (!silent)
                        LOG_WARN("Json: Array item \"%s.%d\" in file \"%s\" should be boolean but is \"%s\". This is marked as optional.", key, i, m_Name.c_str(), r.type_name());
                }
                else
                {
                    LOG_ERR("Json: Array item \"%s.%d\" in file \"%s\" should be boolean but is \"%s\".", key, i, m_Name.c_str(), r.type_name());
                    WindowCloseEvent ev;
                    EventManager::Trigger(EventType::WindowClose, &ev);
                }
            }
        }
    }

    void Json::GetArray(json j, const char * key, int i, int *target, bool optional, bool silent)
    {
        json r;
        GetArray(j, key, i, &r, optional, silent);

        if (!r.is_null())
        {
            if (r.is_number())
            {
                if (r.is_number_float())
                {
                    if (!silent)
                        LOG_WARN("Json: Array item \"%s.%d\" in file \"%s\" should be an integer but is \"float\". An automatic conversion to integer has been made.", key, i, m_Name.c_str());
                }
                *target = r;
            }
            else
            {
                if (optional)
                {
                    if (!silent)
                        LOG_WARN("Json: Array item \"%s.%d\" in file \"%s\" should be an integer but is \"%s\". This is marked as optional.", key, i, m_Name.c_str(), r.type_name());
                }
                else
                {
                    LOG_ERR("Json: Array item \"%s.%d\" in file \"%s\" should be an integer but is \"%s\".", key, i, m_Name.c_str(), r.type_name());
                    WindowCloseEvent ev;
                    EventManager::Trigger(EventType::WindowClose, &ev);
                }
            }
        }
    }

    void Json::GetArray(json j, const char * key, int i, float *target, bool optional, bool silent)
    {
        json r;
        GetArray(j, key, i, &r, optional, silent);

        if (!r.is_null())
        {
            if (r.is_number())
            {
                *target = r;
            }
            else
            {
                if (optional)
                {
                    if (!silent)
                        LOG_WARN("Json: Array item \"%s.%d\" in file \"%s\" should be a float but is \"%s\". This is marked as optional.", key, i, m_Name.c_str(), r.type_name());
                }
                else
                {
                    LOG_ERR("Json: Array item \"%s.%d\" in file \"%s\" should be a float but is \"%s\".", key, i, m_Name.c_str(), r.type_name());
                    WindowCloseEvent ev;
                    EventManager::Trigger(EventType::WindowClose, &ev);
                }
            }
        }
    }

    void Json::GetArray(json j, const char * key, int i, unsigned int *target, bool optional, bool silent)
    {
        json r;
        GetArray(j, key, i, &r, optional, silent);

        if (!r.is_null())
        {
            if (!r.is_number())
            {
                if (r.is_number_float())
                {
                    if (!silent)
                        LOG_WARN("Json: Array item \"%s.%d\" in file \"%s\" should be an unsigned integer but is \"float\". An automatic conversion to unsigned integer has been made.", key, i, m_Name.c_str());
                }
                else if (!r.is_number_unsigned())
                {
                    if (!silent)
                        LOG_WARN("Json: Array item \"%s.%d\" in file \"%s\" should be an unsigned integer but is \"integer\". An automatic conversion to unsigned integer has been made.", key, i, m_Name.c_str());
                }
                *target = r;
            }
            else
            {
                if (optional)
                {
                    if (!silent)
                        LOG_WARN("Json: Array item \"%s.%d\" in file \"%s\" should be an unsigned integer but is \"%s\". This is marked as optional.", key, i, m_Name.c_str(), r.type_name());
                }
                else
                {
                    LOG_ERR("Json: Array item \"%s.%d\" in file \"%s\" should be an unsigned integer but is \"%s\".", key, i, m_Name.c_str(), r.type_name());
                    WindowCloseEvent ev;
                    EventManager::Trigger(EventType::WindowClose, &ev);
                }
            }
        }
    }

    void Json::GetArray(json j, const char * key, int i, std::string *target, bool optional, bool silent)
    {
        json r;
        GetArray(j, key, i, &r, optional, silent);

        if (!r.is_null())
        {
            if (r.is_string())
            {
                *target = r;
            }
            else
            {
                if (optional)
                {
                    if (!silent)
                        LOG_WARN("Json: Array item \"%s.%d\" in file \"%s\" should be a string but is \"%s\". This is marked as optional.", key, i, m_Name.c_str(), r.type_name());
                }
                else
                {
                    LOG_ERR("Json: Array item \"%s.%d\" in file \"%s\" should be a string but is \"%s\".", key, i, m_Name.c_str(), r.type_name());
                    WindowCloseEvent ev;
                    EventManager::Trigger(EventType::WindowClose, &ev);
                }
            }
        }
    }

    json Json::GetJson(const char * key)
    {
        json r;
        Get(key, &r, false, false);
        return r;
    }

    bool Json::GetBool(const char * key)
    {
        bool r;
        Get(key, &r, false, false);
        return r;
    }

    int Json::GetInt(const char * key)
    {
        int r;
        Get(key, &r, false, false);
        return r;
    }

    float Json::GetFloat(const char * key)
    {
        float r;
        Get(key, &r, false, false);
        return r;
    }

    unsigned int Json::GetUInt(const char * key)
    {
        unsigned int r;
        Get(key, &r, false, false);
        return r;
    }

    std::string Json::GetString(const char * key)
    {
        std::string r;
        Get(key, &r, false, false);
        return r;
    }

    json Json::GetJson(json j, const char * key)
    {
        json r;
        Get(j, key, &r, false, false);
        return r;
    }

    bool Json::GetBool(json j, const char * key)
    {
        bool r;
        Get(j, key, &r, false, false);
        return r;
    }

    int Json::GetInt(json j, const char * key)
    {
        int r;
        Get(j, key, &r, false, false);
        return r;
    }

    float Json::GetFloat(json j, const char * key)
    {
        float r;
        Get(j, key, &r, false, false);
        return r;
    }

    unsigned int Json::GetUInt(json j, const char * key)
    {
        unsigned int r;
        Get(j, key, &r, false, false);
        return r;
    }

    std::string Json::GetString(json j, const char * key)
    {
        std::string r;
        Get(j, key, &r, false, false);
        return r;
    }

    json Json::GetJson(json j, int key, std::string name)
    {
        json r;
        Get(j, key, &r, false, false, name);
        return r;
    }

    bool Json::GetBool(json j, int key, std::string name)
    {
        bool r;
        Get(j, key, &r, false, false, name);
        return r;
    }

    int Json::GetInt(json j, int key, std::string name)
    {
        int r;
        Get(j, key, &r, false, false, name);
        return r;
    }

    float Json::GetFloat(json j, int key, std::string name)
    {
        float r;
        Get(j, key, &r, false, false, name);
        return r;
    }

    unsigned int Json::GetUInt(json j, int key, std::string name)
    {
        unsigned int r;
        Get(j, key, &r, false, false, name);
        return r;
    }

    std::string Json::GetString(json j, int key, std::string name)
    {
        std::string r;
        Get(j, key, &r, false, false, name);
        return r;
    }

    json Json::GetJsonArray(const char * key, int i)
    {
        json r;
        GetArray(key, i, &r);
        return r;
    }

    bool Json::GetBoolArray(const char * key, int i)
    {
        bool r;
        GetArray(key, i, &r);
        return r;
    }

    int Json::GetIntArray(const char * key, int i)
    {
        int r;
        GetArray(key, i, &r);
        return r;
    }

    float Json::GetFloatArray(const char * key, int i)
    {
        float r;
        GetArray(key, i, &r);
        return r;
    }

    unsigned int Json::GetUIntArray(const char * key, int i)
    {
        unsigned int r;
        GetArray(key, i, &r);
        return r;
    }

    std::string Json::GetStringArray(const char * key, int i)
    {
        std::string r;
        GetArray(key, i, &r);
        return r;
    }

    json Json::GetJsonArray(json j, const char * key, int i)
    {
        json r;
        GetArray(j, key, i, &r);
        return r;
    }

    bool Json::GetBoolArray(json j, const char * key, int i)
    {
        bool r;
        GetArray(j, key, i, &r);
        return r;
    }

    int Json::GetIntArray(json j, const char * key, int i)
    {
        int r;
        GetArray(j, key, i, &r);
        return r;
    }

    float Json::GetFloatArray(json j, const char * key, int i)
    {
        float r;
        GetArray(j, key, i, &r);
        return r;
    }

    unsigned int Json::GetUIntArray(json j, const char * key, int i)
    {
        unsigned int r;
        GetArray(j, key, i, &r);
        return r;
    }

    std::string Json::GetStringArray(json j, const char * key, int i)
    {
        std::string r;
        GetArray(j, key, i, &r);
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
