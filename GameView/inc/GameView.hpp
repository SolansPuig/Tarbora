#pragma once
#include <list>
#include <memory>

namespace Tarbora {
    typedef unsigned int GameViewId;
    enum GameViewType {
        GV_Human, GV_Remote, GV_AI, GV_Recorder, GV_Other
    };

    class GameView
    {
    public:
        virtual ~GameView() {};

        virtual void OnCreate(GameViewId id) = 0;
        virtual void OnDestroy() = 0;
        virtual void Update(float elapsed_time) = 0;
        virtual void Draw() = 0;

        virtual GameViewId GetId() const = 0;
        virtual GameViewType GetType() const = 0;
    };

    typedef std::list<std::shared_ptr<GameView>> GameViewList;
    typedef std::shared_ptr<GameView> GameViewPtr;
}
