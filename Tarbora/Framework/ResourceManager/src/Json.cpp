#include "../inc/Json.hpp"
#include "../../MessageManager/inc/MessageManager.hpp"
#include "../../ClientApplication.hpp"

namespace Tarbora {
    void Json::PushErrName(std::string name)
    {
        // Append the name to the end of m_ErrName, and a dot.
        m_ErrName += (name + ".");
    }

    void Json::PopErrName()
    {
        // Find the second-to-last dot and remove everythin from it.
        std::size_t found = m_ErrName.find_last_of(".", m_ErrName.size()-2);
        m_ErrName = m_ErrName.substr(0,found+1);
    }

    // This is Chaos.
    void Json::Get(const char * key, raw_json *target, JsonOptions options)
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

    void Json::Get(raw_json j, const char * key, raw_json *target, JsonOptions options)
    {
        raw_json r = j[key];

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
                app->Close();
            }
        }
        else
        {
            *target = r;
        }
    }

    void Json::Get(raw_json j, const char * key, bool *target, JsonOptions options)
    {
        raw_json r;
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
                    app->Close();
                }
            }
        }
    }

    void Json::Get(raw_json j, const char * key, int *target, JsonOptions options)
    {
        raw_json r;
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
                    app->Close();
                }
            }
        }
    }

    void Json::Get(raw_json j, const char * key, float *target, JsonOptions options)
    {
        raw_json r;
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
                    app->Close();
                }
            }
        }
    }

    void Json::Get(raw_json j, const char * key, unsigned int *target, JsonOptions options)
    {
        raw_json r;
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
                    app->Close();
                }
            }
        }
    }

    void Json::Get(raw_json j, const char * key, std::string *target, JsonOptions options)
    {
        raw_json r;
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
                    app->Close();
                }
            }
        }
    }

    void Json::Get(raw_json j, int key, raw_json *target, JsonOptions options)
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
                    app->Close();
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
                app->Close();
            }
        }

        raw_json r = j[key];

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
                app->Close();
            }
        }
        else
        {
            *target = r;
        }
    }

    void Json::Get(raw_json j, int key, bool *target, JsonOptions options)
    {
        raw_json r;
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
                    app->Close();
                }
            }
        }
    }

    void Json::Get(raw_json j, int key, int *target, JsonOptions options)
    {
        raw_json r;
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
                    app->Close();
                }
            }
        }
    }

    void Json::Get(raw_json j, int key, float *target, JsonOptions options)
    {
        raw_json r;
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
                    app->Close();
                }
            }
        }
    }

    void Json::Get(raw_json j, int key, unsigned int *target, JsonOptions options)
    {
        raw_json r;
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
                    app->Close();
                }
            }
        }
    }

    void Json::Get(raw_json j, int key, std::string *target, JsonOptions options)
    {
        raw_json r;
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
                    app->Close();
                }
            }
        }
    }

    void Json::GetArray(const char * key, int i, raw_json *target, JsonOptions options)
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

    void Json::GetArray(raw_json j, const char * key, int i, raw_json *target, JsonOptions options)
    {
        raw_json r;
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
                        app->Close();
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
                    app->Close();
                }
            }
        }
    }

    void Json::GetArray(raw_json j, const char * key, int i, bool *target, JsonOptions options)
    {
        raw_json r;
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
                    app->Close();
                }
            }
        }
    }

    void Json::GetArray(raw_json j, const char * key, int i, int *target, JsonOptions options)
    {
        raw_json r;
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
                    app->Close();
                }
            }
        }
    }

    void Json::GetArray(raw_json j, const char * key, int i, float *target, JsonOptions options)
    {
        raw_json r;
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
                    app->Close();
                }
            }
        }
    }

    void Json::GetArray(raw_json j, const char * key, int i, unsigned int *target, JsonOptions options)
    {
        raw_json r;
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
                    app->Close();
                }
            }
        }
    }

    void Json::GetArray(raw_json j, const char * key, int i, std::string *target, JsonOptions options)
    {
        raw_json r;
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
                    app->Close();
                }
            }
        }
    }

    raw_json Json::GetJson(const char * key, JsonOptions options)
    {
        raw_json r;
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

    raw_json Json::GetJson(raw_json j, const char * key, JsonOptions options)
    {
        raw_json r;
        Get(j, key, &r, options);
        return r;
    }

    bool Json::GetBool(raw_json j, const char * key, JsonOptions options)
    {
        bool r;
        Get(j, key, &r, options);
        return r;
    }

    int Json::GetInt(raw_json j, const char * key, JsonOptions options)
    {
        int r;
        Get(j, key, &r, options);
        return r;
    }

    float Json::GetFloat(raw_json j, const char * key, JsonOptions options)
    {
        float r;
        Get(j, key, &r, options);
        return r;
    }

    unsigned int Json::GetUInt(raw_json j, const char * key, JsonOptions options)
    {
        unsigned int r;
        Get(j, key, &r, options);
        return r;
    }

    std::string Json::GetString(raw_json j, const char * key, JsonOptions options)
    {
        std::string r;
        Get(j, key, &r, options);
        return r;
    }

    raw_json Json::GetJson(raw_json j, int key, JsonOptions options)
    {
        raw_json r;
        Get(j, key, &r, options);
        return r;
    }

    bool Json::GetBool(raw_json j, int key, JsonOptions options)
    {
        bool r;
        Get(j, key, &r, options);
        return r;
    }

    int Json::GetInt(raw_json j, int key, JsonOptions options)
    {
        int r;
        Get(j, key, &r, options);
        return r;
    }

    float Json::GetFloat(raw_json j, int key, JsonOptions options)
    {
        float r;
        Get(j, key, &r, options);
        return r;
    }

    unsigned int Json::GetUInt(raw_json j, int key, JsonOptions options)
    {
        unsigned int r;
        Get(j, key, &r, options);
        return r;
    }

    std::string Json::GetString(raw_json j, int key, JsonOptions options)
    {
        std::string r;
        Get(j, key, &r, options);
        return r;
    }

    raw_json Json::GetJsonArray(const char * key, int i, JsonOptions options)
    {
        raw_json r;
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

    raw_json Json::GetJsonArray(raw_json j, const char * key, int i, JsonOptions options)
    {
        raw_json r;
        GetArray(j, key, i, &r, options);
        return r;
    }

    bool Json::GetBoolArray(raw_json j, const char * key, int i, JsonOptions options)
    {
        bool r;
        GetArray(j, key, i, &r, options);
        return r;
    }

    int Json::GetIntArray(raw_json j, const char * key, int i, JsonOptions options)
    {
        int r;
        GetArray(j, key, i, &r, options);
        return r;
    }

    float Json::GetFloatArray(raw_json j, const char * key, int i, JsonOptions options)
    {
        float r;
        GetArray(j, key, i, &r, options);
        return r;
    }

    unsigned int Json::GetUIntArray(raw_json j, const char * key, int i, JsonOptions options)
    {
        unsigned int r;
        GetArray(j, key, i, &r, options);
        return r;
    }

    std::string Json::GetStringArray(raw_json j, const char * key, int i, JsonOptions options)
    {
        std::string r;
        GetArray(j, key, i, &r, options);
        return r;
    }

    ResourcePtr JsonResourceLoader::Load(std::string path)
    {
        // Try to open the file.
        std::ifstream file;
        file.open(path.c_str());
        if (file.fail())
        {
            return ResourcePtr();
        }

        // Try to parse the file.
        raw_json data;
        try
        {
            data = raw_json::parse(file);
        }
        catch (raw_json::parse_error& e)
        {
            LOG_ERR("JsonResourceLoader: Trying to parse file \"%s\" found exception: \n \"%s\"", path.c_str(), e.what());
            file.close();
            return ResourcePtr();
        }

        // Crate the Resource.
        ResourcePtr r = ResourcePtr(new Json(app, path, data));
        file.close(); // Close the file.
        return r;
    }
}
