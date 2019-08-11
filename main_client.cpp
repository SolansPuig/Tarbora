#include "Tarbora/Views/GraphicViews/inc/HumanView.hpp"

using namespace Tarbora;

int main() {
    Logger::Init(stdout);
    LOG_LEVEL(Logger::LogLevel::DEBUG);

    std::shared_ptr<HumanView> human_view(new HumanView("localhost:50051"));
    human_view->Run();

    Logger::Close();
    return 0;
}
