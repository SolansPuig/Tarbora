#include "Tarbora/Views/GraphicViews/inc/ConsoleView.hpp"

using namespace Tarbora;

int main() {
    LOG_LEVEL(DEBUG);

    std::shared_ptr<ConsoleView> console_view(new ConsoleView("localhost:50051"));

    console_view->Run();

    return 0;
}
