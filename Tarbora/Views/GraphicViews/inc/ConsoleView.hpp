#pragma once
#include "Console.hpp"
#include "GraphicView.hpp"

namespace Tarbora {
    class ConsoleView : public GraphicView
    {
    public:
        ConsoleView(std::string server_address);
        ~ConsoleView();

        virtual void GetInput() override;
        virtual void Update(float elapsed_time) override;
        virtual void Draw() override;

        std::shared_ptr<ConsoleImpl> Console() { return m_Console; }

    private:
        std::shared_ptr<ConsoleImpl> m_Console;
        EventId m_All;
    };
}
