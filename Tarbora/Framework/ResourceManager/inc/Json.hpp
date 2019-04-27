#pragma once
#include "Resource.hpp"
#include "../../Utility/inc/json.hpp"
typedef nlohmann::json json;

namespace Tarbora {
    struct JsonOptions
    {
        JsonOptions(bool optional=false, bool silent=false) :
            optional(optional), silent(silent) {}
        bool optional;
        bool silent;
    };

    class Json : public Resource
    {
    public:
        Json(std::string name, json j) : Resource(name), m_JSON(j), m_ErrName("") {}
        const json GetJson() const { return m_JSON; }

        void PushErrName(std::string name);
        void PopErrName();

        void Get(const char * key, json *target, JsonOptions options={});
        void Get(const char * key, bool *target, JsonOptions options={});
        void Get(const char * key, int *target, JsonOptions options={});
        void Get(const char * key, float *target, JsonOptions options={});
        void Get(const char * key, unsigned int *target, JsonOptions options={});
        void Get(const char * key, std::string *target, JsonOptions options={});

        void Get(json j, const char * key, json *target, JsonOptions options={});
        void Get(json j, const char * key, bool *target, JsonOptions options={});
        void Get(json j, const char * key, int *target, JsonOptions options={});
        void Get(json j, const char * key, float *target, JsonOptions options={});
        void Get(json j, const char * key, unsigned int *target, JsonOptions options={});
        void Get(json j, const char * key, std::string *target, JsonOptions options={});

        void Get(json j, int key, json *target, JsonOptions options={});
        void Get(json j, int key, bool *target, JsonOptions options={});
        void Get(json j, int key, int *target, JsonOptions options={});
        void Get(json j, int key, float *target, JsonOptions options={});
        void Get(json j, int key, unsigned int *target, JsonOptions options={});
        void Get(json j, int key, std::string *target, JsonOptions options={});

        json GetJson(const char * key, JsonOptions options={});
        bool GetBool(const char * key, JsonOptions options={});
        int GetInt(const char * key, JsonOptions options={});
        float GetFloat(const char * key, JsonOptions options={});
        unsigned int GetUInt(const char * key, JsonOptions options={});
        std::string GetString(const char * key, JsonOptions options={});

        json GetJson(json j, const char * key, JsonOptions options={});
        bool GetBool(json j, const char * key, JsonOptions options={});
        int GetInt(json j, const char * key, JsonOptions options={});
        float GetFloat(json j, const char * key, JsonOptions options={});
        unsigned int GetUInt(json j, const char * key, JsonOptions options={});
        std::string GetString(json j, const char * key, JsonOptions options={});

        json GetJson(json j, int key, JsonOptions options={});
        bool GetBool(json j, int key, JsonOptions options={});
        int GetInt(json j, int key, JsonOptions options={});
        float GetFloat(json j, int key, JsonOptions options={});
        unsigned int GetUInt(json j, int key, JsonOptions options={});
        std::string GetString(json j, int key, JsonOptions options={});

        void GetArray(const char * key, int i, json *target, JsonOptions options={});
        void GetArray(const char * key, int i, bool *target, JsonOptions options={});
        void GetArray(const char * key, int i, int *target, JsonOptions options={});
        void GetArray(const char * key, int i, float *target, JsonOptions options={});
        void GetArray(const char * key, int i, unsigned int *target, JsonOptions options={});
        void GetArray(const char * key, int i, std::string *target, JsonOptions options={});

        void GetArray(json j, const char * key, int i, json *target, JsonOptions options={});
        void GetArray(json j, const char * key, int i, bool *target, JsonOptions options={});
        void GetArray(json j, const char * key, int i, int *target, JsonOptions options={});
        void GetArray(json j, const char * key, int i, float *target, JsonOptions options={});
        void GetArray(json j, const char * key, int i, unsigned int *target, JsonOptions options={});
        void GetArray(json j, const char * key, int i, std::string *target, JsonOptions options={});

        json GetJsonArray(const char * key, int i, JsonOptions options={});
        bool GetBoolArray(const char * key, int i, JsonOptions options={});
        int GetIntArray(const char * key, int i, JsonOptions options={});
        float GetFloatArray(const char * key, int i, JsonOptions options={});
        unsigned int GetUIntArray(const char * key, int i, JsonOptions options={});
        std::string GetStringArray(const char * key, int i, JsonOptions options={});

        json GetJsonArray(json j, const char * key, int i, JsonOptions options={});
        bool GetBoolArray(json j, const char * key, int i, JsonOptions options={});
        int GetIntArray(json j, const char * key, int i, JsonOptions options={});
        float GetFloatArray(json j, const char * key, int i, JsonOptions options={});
        unsigned int GetUIntArray(json j, const char * key, int i, JsonOptions options={});
        std::string GetStringArray(json j, const char * key, int i, JsonOptions options={});

    protected:
        json m_JSON;
        std::string m_ErrName;
    };

    class JsonResourceLoader : public ResourceLoader
    {
    public:
        virtual const std::string GetPattern() override { return "*.json"; };
        virtual ResourcePtr Load(std::string path) override;
    };

    typedef std::shared_ptr<Json> JsonPtr;
}
