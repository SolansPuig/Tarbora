#pragma once
#include "Logger.hpp"
#include "Resource.hpp"
#include <string>
#include <fstream>

namespace Tarbora {
    struct Settings {
        inline static std::string window_title = "Tarbora Game Engine";
        inline static int window_width = 1280, window_height = 720;
        inline static float clear_r = 0, clear_g = 0, clear_b = 0;

        static void Load(const char *path)
        {
            auto J = GET_RESOURCE(JsonResource, path);
            if (J)
            {
                json j = J->GetJson();

                if (j.find("window") != j.end() &&
                    j["window"].find("title") != j["window"].end())
                {
                    window_title = j["window"]["title"];
                } else LOG_WARN("Settings: Could not find a valid window title. Using default.");

                if (j.find("window") != j.end() &&
                    j["window"].find("size") != j["window"].end() &&
                    j["window"]["size"].size() == 2)
                {
                    window_width = j["window"]["size"][0];
                    window_height = j["window"]["size"][1];
                } else LOG_WARN("Settings: Could not find a valid window size. Using default");

                if (j.find("window") != j.end() &&
                    j["window"].find("clear_color") != j["window"].end() &&
                    j["window"]["clear_color"].size() == 3)
                {
                    clear_r = j["window"]["clear_color"][0];
                    clear_g = j["window"]["clear_color"][1];
                    clear_b = j["window"]["clear_color"][2];
                } else LOG_WARN("Settings: Could not find a valid window clear color. Using default.");
            } else LOG_WARN("Settings: Could not find a valid Settings file. Using default. Your settings file must be: %s", path);
        }
    };
}
