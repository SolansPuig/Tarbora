#pragma once
#include "Resource.hpp"
#include "../../External/json.hpp"
typedef nlohmann::json raw_json;

namespace Tarbora {
    //! Options used when retrieving data from a Json.
    //! \see Json
    struct JsonOptions
    {
        JsonOptions(bool optional=false, bool silent=false) :
            optional(optional), silent(silent) {}
        bool optional; //!< Set this to avoid stopping the execution if the data is not found. Default: false
        bool silent; //!< Set this to avoid displaying any warnings. Default: false
    };

    //! Secure access Json files
    /*!
        The class raw_json is from the package nlohmann::json, you can find its documentation here: https://nlohmann.github.io/json/

        \code{.cpp}
            // The file "resource.json" contains the json:
            //      { "shapes": [
            //          { "name": "box", "position": [1.0, 0.0, 0.0], "size": 3 },
            //          { "name": "sphere", "position": [0.0, 5.0, 0.0],"size": 4.2 },
            //          { "name": "pyramid", "position": [1.0, 5.0, 0.0]}
            //      ] }

            // We want to retrieve the name and optionally the size from all the shapes

            // Load and parse the file
            Json resource = GET_RESOURCE(Json, "resource.json");

            // Read the file
            raw_json shapes resource->GetJson("shapes");        // Get the "shapes" list
            resource->PushErrName("shapes");                // Push to the error path

            for (int i = 0; i < shapes.size(); i++)
            {
                resource->PushErrName("%d", i);
                raw_json node = GetJson(shapes, i);
                std::string name = GetString(node, "name"); // If there's no string "name", the execution will stop.
                // Read the position as values of an array
                float x = GetFloatArray(node, "position", 0);
                float y = GetFloatArray(node, "position", 1);
                float z = GetFloatArray(node, "position", 2);
                float size = 1.0f;                          // If there's no number "size", it will default to 1.0f.
                Get(node, "size", &size, {true});           // If there's no number "size", a warning will be logged.

                // Note that in the case of the "box", a warning will also be logged informing the automatic conversion from int to float.
                // Use Get(node, "size", &size, {true, true}); to avoid all sorts of warnings.

                // Log the results
                LOG_INFO("Name: %s, Size: %f, x: %f, y: %f, z: %f", name.c_str(), size, x, y, z);
                resource->PopErrName();
            }
            resource->PopErrName();
        \endcode

        \see Resource
        \see ResourceManager
    */
    class Json : public Resource
    {
        friend class JsonResourceLoader;
    public:
        //! Get the raw Json. Not recommended.
        const raw_json GetJson() const { return m_JSON; }

        //! Add a name to the error path that will be shown if an error occurs.
        /*!
            See the \ref Json class description for an example.

            Use it along with \ref PopErrName.
        */
        void PushErrName(std::string name);

        //! Remove the last name from the error path.
        /*!
            See the \ref Json class description for an example.

            Use it along with \ref PushErrName.
        */
        void PopErrName();

        //! Store the value of \a key into \a taget, from the root of the file.
        /*!
            If the value is incorrect or missing, leaves \a target as is.
            Use \a options to configure what happens in case of error.
            Also, the use of \ref PushErrName and \ref PopErrName is encouraged.
        */
        void Get(const char * key, raw_json *target, JsonOptions options={});
        //! \overload
        void Get(const char * key, bool *target, JsonOptions options={});
        //! \overload
        void Get(const char * key, int *target, JsonOptions options={});
        //! \overload
        void Get(const char * key, float *target, JsonOptions options={});
        //! \overload
        void Get(const char * key, unsigned int *target, JsonOptions options={});
        //! \overload
        void Get(const char * key, std::string *target, JsonOptions options={});

        //! Store the value of \a key into \a taget, from a raw_json subfile \a j.
        /*!
            If the value is incorrect or missing, leaves \a target as is.
            Use \a options to configure what happens in case of error.
            Also, the use of \ref PushErrName and \ref PopErrName is encouraged.
        */
        void Get(raw_json j, const char * key, raw_json *target, JsonOptions options={});
        //! \overload
        void Get(raw_json j, const char * key, bool *target, JsonOptions options={});
        //! \overload
        void Get(raw_json j, const char * key, int *target, JsonOptions options={});
        //! \overload
        void Get(raw_json j, const char * key, float *target, JsonOptions options={});
        //! \overload
        void Get(raw_json j, const char * key, unsigned int *target, JsonOptions options={});
        //! \overload
        void Get(raw_json j, const char * key, std::string *target, JsonOptions options={});
        //! \overload
        void Get(raw_json j, int key, raw_json *target, JsonOptions options={});
        //! \overload
        void Get(raw_json j, int key, bool *target, JsonOptions options={});
        //! \overload
        void Get(raw_json j, int key, int *target, JsonOptions options={});
        //! \overload
        void Get(raw_json j, int key, float *target, JsonOptions options={});
        //! \overload
        void Get(raw_json j, int key, unsigned int *target, JsonOptions options={});
        //! \overload
        void Get(raw_json j, int key, std::string *target, JsonOptions options={});

        //! Get the raw_json value of \a key, from the root of the file.
        /*!
            If the value is incorrect or missing, stops the execution.
            Also, the use of \ref PushErrName and \ref PopErrName is encouraged.
        */
        raw_json GetJson(const char * key, JsonOptions options={});

        //! Get the bool value of \a key, from the root of the file.
        /*!
            If the value is incorrect or missing, stops the execution.
            Also, the use of \ref PushErrName and \ref PopErrName is encouraged.
        */
        bool GetBool(const char * key, JsonOptions options={});

        //! Get the int value of \a key, from the root of the file.
        /*!
            If the value is incorrect or missing, stops the execution.
            Also, the use of \ref PushErrName and \ref PopErrName is encouraged.
        */
        int GetInt(const char * key, JsonOptions options={});

        //! Get the float value of \a key, from the root of the file.
        /*!
            If the value is incorrect or missing, stops the execution.
            Also, the use of \ref PushErrName and \ref PopErrName is encouraged.
        */
        float GetFloat(const char * key, JsonOptions options={});

        //! Get the unsigned int value of \a key, from the root of the file.
        /*!
            If the value is incorrect or missing, stops the execution.
            Also, the use of \ref PushErrName and \ref PopErrName is encouraged.
        */
        unsigned int GetUInt(const char * key, JsonOptions options={});

        //! Get the string int value of \a key, from the root of the file.
        /*!
            If the value is incorrect or missing, stops the execution.
            Also, the use of \ref PushErrName and \ref PopErrName is encouraged.
        */
        std::string GetString(const char * key, JsonOptions options={});

        //! Get the raw_json value of \a key, from a raw_json subfile \a j.
        /*!
            If the value is incorrect or missing, stops the execution.
            Also, the use of \ref PushErrName and \ref PopErrName is encouraged.
        */
        raw_json GetJson(raw_json j, const char * key, JsonOptions options={});

        //! \overload
        raw_json GetJson(raw_json j, int key, JsonOptions options={});

        //! Get the bool value of \a key, from a raw_json subfile \a j.
        /*!
            If the value is incorrect or missing, stops the execution.
            Also, the use of \ref PushErrName and \ref PopErrName is encouraged.
        */
        bool GetBool(raw_json j, const char * key, JsonOptions options={});
        //! \overload
        bool GetBool(raw_json j, int key, JsonOptions options={});
        //! Get the int value of \a key, from a raw_json subfile \a j.
        /*!
            If the value is incorrect or missing, stops the execution.
            Also, the use of \ref PushErrName and \ref PopErrName is encouraged.
        */
        int GetInt(raw_json j, const char * key, JsonOptions options={});
        //! \overload
        int GetInt(raw_json j, int key, JsonOptions options={});
        //! Get the float value of \a key, from a raw_json subfile \a j.
        /*!
            If the value is incorrect or missing, stops the execution.
            Also, the use of \ref PushErrName and \ref PopErrName is encouraged.
        */
        float GetFloat(raw_json j, const char * key, JsonOptions options={});
        //! \overload
        float GetFloat(raw_json j, int key, JsonOptions options={});
        //! Get the unsigned int value of \a key, from a raw_json subfile \a j.
        /*!
            If the value is incorrect or missing, stops the execution.
            Also, the use of \ref PushErrName and \ref PopErrName is encouraged.
        */
        unsigned int GetUInt(raw_json j, const char * key, JsonOptions options={});
        //! \overload
        unsigned int GetUInt(raw_json j, int key, JsonOptions options={});
        //! Get the string value of \a key, from a raw_json subfile \a j.
        /*!
            If the value is incorrect or missing, stops the execution.
            Also, the use of \ref PushErrName and \ref PopErrName is encouraged.
        */
        std::string GetString(raw_json j, const char * key, JsonOptions options={});
        //! \overload
        std::string GetString(raw_json j, int key, JsonOptions options={});

        //! Store the value of index \a i from the array \a key into \a taget, from the root of the file.
        /*!
            If the value is incorrect or missing, leaves \a target as is.
            Use \a options to configure what happens in case of error.
            Also, the use of \ref PushErrName and \ref PopErrName is encouraged.
        */
        void GetArray(const char * key, int i, raw_json *target, JsonOptions options={});
        //! \overload
        void GetArray(const char * key, int i, bool *target, JsonOptions options={});
        //! \overload
        void GetArray(const char * key, int i, int *target, JsonOptions options={});
        //! \overload
        void GetArray(const char * key, int i, float *target, JsonOptions options={});
        //! \overload
        void GetArray(const char * key, int i, unsigned int *target, JsonOptions options={});
        //! \overload
        void GetArray(const char * key, int i, std::string *target, JsonOptions options={});

        //! Store the value of index \a i from the array \a key into \a taget, from a raw_json subfile \a j.
        /*!
            If the value is incorrect or missing, leaves \a target as is.
            Use \a options to configure what happens in case of error.
            Also, the use of \ref PushErrName and \ref PopErrName is encouraged.
        */
        void GetArray(raw_json j, const char * key, int i, raw_json *target, JsonOptions options={});
        //! \overload
        void GetArray(raw_json j, const char * key, int i, bool *target, JsonOptions options={});
        //! \overload
        void GetArray(raw_json j, const char * key, int i, int *target, JsonOptions options={});
        //! \overload
        void GetArray(raw_json j, const char * key, int i, float *target, JsonOptions options={});
        //! \overload
        void GetArray(raw_json j, const char * key, int i, unsigned int *target, JsonOptions options={});
        //! \overload
        void GetArray(raw_json j, const char * key, int i, std::string *target, JsonOptions options={});

        //! Get the raw_json value of index \a i from the array \a key into \a taget, from the root of the file.
        /*!
            If the value is incorrect or missing, leaves \a target as is.
            Use \a options to configure what happens in case of error.
            Also, the use of \ref PushErrName and \ref PopErrName is encouraged.
        */
        raw_json GetJsonArray(const char * key, int i, JsonOptions options={});
        //! Get the bool value of index \a i from the array \a key into \a taget, from the root of the file.
        /*!
            If the value is incorrect or missing, leaves \a target as is.
            Use \a options to configure what happens in case of error.
            Also, the use of \ref PushErrName and \ref PopErrName is encouraged.
        */
        bool GetBoolArray(const char * key, int i, JsonOptions options={});
        //! Get the int value of index \a i from the array \a key into \a taget, from the root of the file.
        /*!
            If the value is incorrect or missing, leaves \a target as is.
            Use \a options to configure what happens in case of error.
            Also, the use of \ref PushErrName and \ref PopErrName is encouraged.
        */
        int GetIntArray(const char * key, int i, JsonOptions options={});
        //! Get the float value of index \a i from the array \a key into \a taget, from the root of the file.
        /*!
            If the value is incorrect or missing, leaves \a target as is.
            Use \a options to configure what happens in case of error.
            Also, the use of \ref PushErrName and \ref PopErrName is encouraged.
        */
        float GetFloatArray(const char * key, int i, JsonOptions options={});
        //! Get the unsigned int value of index \a i from the array \a key into \a taget, from the root of the file.
        /*!
            If the value is incorrect or missing, leaves \a target as is.
            Use \a options to configure what happens in case of error.
            Also, the use of \ref PushErrName and \ref PopErrName is encouraged.
        */
        unsigned int GetUIntArray(const char * key, int i, JsonOptions options={});
        //! Get the string value of index \a i from the array \a key into \a taget, from the root of the file.
        /*!
            If the value is incorrect or missing, leaves \a target as is.
            Use \a options to configure what happens in case of error.
            Also, the use of \ref PushErrName and \ref PopErrName is encouraged.
        */
        std::string GetStringArray(const char * key, int i, JsonOptions options={});

        //! Get the raw_json value of index \a i from the array \a key into \a taget, from a raw_json subfile \a j.
        /*!
            If the value is incorrect or missing, leaves \a target as is.
            Use \a options to configure what happens in case of error.
            Also, the use of \ref PushErrName and \ref PopErrName is encouraged.
        */
        raw_json GetJsonArray(raw_json j, const char * key, int i, JsonOptions options={});
        //! Get the bool value of index \a i from the array \a key into \a taget, from a raw_json subfile \a j.
        /*!
            If the value is incorrect or missing, leaves \a target as is.
            Use \a options to configure what happens in case of error.
            Also, the use of \ref PushErrName and \ref PopErrName is encouraged.
        */
        bool GetBoolArray(raw_json j, const char * key, int i, JsonOptions options={});
        //! Get the int value of index \a i from the array \a key into \a taget, from a raw_json subfile \a j.
        /*!
            If the value is incorrect or missing, leaves \a target as is.
            Use \a options to configure what happens in case of error.
            Also, the use of \ref PushErrName and \ref PopErrName is encouraged.
        */
        int GetIntArray(raw_json j, const char * key, int i, JsonOptions options={});
        //! Get the float value of index \a i from the array \a key into \a taget, from a raw_json subfile \a j.
        /*!
            If the value is incorrect or missing, leaves \a target as is.
            Use \a options to configure what happens in case of error.
            Also, the use of \ref PushErrName and \ref PopErrName is encouraged.
        */
        float GetFloatArray(raw_json j, const char * key, int i, JsonOptions options={});
        //! Get the unsigned int value of index \a i from the array \a key into \a taget, from a raw_json subfile \a j.
        /*!
            If the value is incorrect or missing, leaves \a target as is.
            Use \a options to configure what happens in case of error.
            Also, the use of \ref PushErrName and \ref PopErrName is encouraged.
        */
        unsigned int GetUIntArray(raw_json j, const char * key, int i, JsonOptions options={});
        //! Get the string value of index \a i from the array \a key into \a taget, from a raw_json subfile \a j.
        /*!
            If the value is incorrect or missing, leaves \a target as is.
            Use \a options to configure what happens in case of error.
            Also, the use of \ref PushErrName and \ref PopErrName is encouraged.
        */
        std::string GetStringArray(raw_json j, const char * key, int i, JsonOptions options={});

    private:
        Json(std::string name, raw_json j) : Resource(name), m_JSON(j), m_ErrName("") {}

        raw_json m_JSON;
        std::string m_ErrName;
    };

    //! \cond HIDDEN_SYMBOLS
    class JsonResourceLoader : public ResourceLoader
    {
        friend class ResourceManager;
    private:
        virtual const std::string GetPattern() override { return "*.json"; };
        virtual std::shared_ptr<Resource> Load(std::string path) override;
    };

    typedef std::shared_ptr<Json> JsonPtr;
    //! \endcond
}
