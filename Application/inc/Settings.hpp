#pragma once
#include "json.hpp"
#include "Logger.hpp"
#include <string>
#include <fstream>

typedef nlohmann::json json;

namespace Tarbora {
    struct Settings {
        inline static std::string window_title = "Tarbora Game Engine";
        inline static int window_width = 1280, window_height = 720;
        inline static float clear_r = 0, clear_g = 0, clear_b = 0;

        static void Load(const char *path)
        {
            std::ifstream ifs(path);
            if (!ifs.fail())
            {
                json j = json::parse(ifs);
                json window = j["window"];

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
