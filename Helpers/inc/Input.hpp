#pragma once
#include <utility>

namespace Tarbora {
    namespace Input {
        void Init();
        
        bool IsKeyPressed(int keycode);
        bool IsMouseButtonPressed(int button);
        std::pair<int, int> GetMousePosition();
    }
}
