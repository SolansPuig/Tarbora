#pragma once
#include <list>
#include <memory>
#include "Actors.hpp"

namespace Tarbora {
    enum GameViewType {
        GV_Human, GV_Remote, GV_AI, GV_Recorder, GV_Other
    };

    class GameView
    {
    public:
        virtual void Update(float elapsed_time) = 0;
        virtual void Draw() = 0;

        virtual ActorId GetTargetId() const = 0;
        virtual GameViewType GetType() const = 0;
    };

    typedef std::list<std::shared_ptr<GameView>> GameViewList;
    typedef std::shared_ptr<GameView> GameViewPtr;
}
