#include "Tarbora/Views/GraphicViews/inc/ConsoleView.hpp"

using namespace Tarbora;

int main() {
    Logger::Init(stdout);
    LOG_LEVEL(Logger::LogLevel::DEBUG);

    std::shared_ptr<ConsoleView> console_view(new ConsoleView("localhost:50051"));

    console_view->Run();

    Logger::Close();
    return 0;
}
