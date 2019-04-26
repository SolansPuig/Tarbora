#pragma once
#include "Resource.hpp"
#include "../../Utility/inc/json.hpp"
typedef nlohmann::json json;

namespace Tarbora {
    class Json : public Resource
    {
    public:
        Json(std::string name, json j) : Resource(name), m_JSON(j) {}
        const json GetJson() const { return m_JSON; }

        void Get(const char * key, json *target, bool optional=false, bool silent=false);
        void Get(const char * key, bool *target, bool optional=false, bool silent=false);
        void Get(const char * key, int *target, bool optional=false, bool silent=false);
        void Get(const char * key, float *target, bool optional=false, bool silent=false);
        void Get(const char * key, unsigned int *target, bool optional=false, bool silent=false);
        void Get(const char * key, std::string *target, bool optional=false, bool silent=false);

        void Get(json j, const char * key, json *target, bool optional=false, bool silent=false);
        void Get(json j, const char * key, bool *target, bool optional=false, bool silent=false);
        void Get(json j, const char * key, int *target, bool optional=false, bool silent=false);
        void Get(json j, const char * key, float *target, bool optional=false, bool silent=false);
        void Get(json j, const char * key, unsigned int *target, bool optional=false, bool silent=false);
        void Get(json j, const char * key, std::string *target, bool optional=false, bool silent=false);

        void Get(json j, int key, json *target, bool optional=false, bool silent=false, std::string name="");
        void Get(json j, int key, bool *target, bool optional=false, bool silent=false, std::string name="");
        void Get(json j, int key, int *target, bool optional=false, bool silent=false, std::string name="");
        void Get(json j, int key, float *target, bool optional=false, bool silent=false, std::string name="");
        void Get(json j, int key, unsigned int *target, bool optional=false, bool silent=false, std::string name="");
        void Get(json j, int key, std::string *target, bool optional=false, bool silent=false, std::string name="");

        void GetArray(const char * key, int i, json *target, bool optional=false, bool silent=false);
        void GetArray(const char * key, int i, bool *target, bool optional=false, bool silent=false);
        void GetArray(const char * key, int i, int *target, bool optional=false, bool silent=false);
        void GetArray(const char * key, int i, float *target, bool optional=false, bool silent=false);
        void GetArray(const char * key, int i, unsigned int *target, bool optional=false, bool silent=false);
        void GetArray(const char * key, int i, std::string *target, bool optional=false, bool silent=false);

        void GetArray(json j, const char * key, int i, json *target, bool optional=false, bool silent=false);
        void GetArray(json j, const char * key, int i, bool *target, bool optional=false, bool silent=false);
        void GetArray(json j, const char * key, int i, int *target, bool optional=false, bool silent=false);
        void GetArray(json j, const char * key, int i, float *target, bool optional=false, bool silent=false);
        void GetArray(json j, const char * key, int i, unsigned int *target, bool optional=false, bool silent=false);
        void GetArray(json j, const char * key, int i, std::string *target, bool optional=false, bool silent=false);

        json GetJson(const char * key);
        bool GetBool(const char * key);
        int GetInt(const char * key);
        float GetFloat(const char * key);
        unsigned int GetUInt(const char * key);
        std::string GetString(const char * key);

        json GetJson(json j, const char * key);
        bool GetBool(json j, const char * key);
        int GetInt(json j, const char * key);
        float GetFloat(json j, const char * key);
        unsigned int GetUInt(json j, const char * key);
        std::string GetString(json j, const char * key);

        json GetJson(json j, int key, std::string name="");
        bool GetBool(json j, int key, std::string name="");
        int GetInt(json j, int key, std::string name="");
        float GetFloat(json j, int key, std::string name="");
        unsigned int GetUInt(json j, int key, std::string name="");
        std::string GetString(json j, int key, std::string name="");

        json GetJsonArray(const char * key, int i);
        bool GetBoolArray(const char * key, int i);
        int GetIntArray(const char * key, int i);
        float GetFloatArray(const char * key, int i);
        unsigned int GetUIntArray(const char * key, int i);
        std::string GetStringArray(const char * key, int i);

        json GetJsonArray(json j, const char * key, int i);
        bool GetBoolArray(json j, const char * key, int i);
        int GetIntArray(json j, const char * key, int i);
        float GetFloatArray(json j, const char * key, int i);
        unsigned int GetUIntArray(json j, const char * key, int i);
        std::string GetStringArray(json j, const char * key, int i);

    protected:
        json m_JSON;
    };

    class JsonResourceLoader : public ResourceLoader
    {
    public:
        virtual const std::string GetPattern() override { return "*.json"; };
        virtual ResourcePtr Load(std::string path) override;
    };

    typedef std::shared_ptr<Json> JsonPtr;
}
