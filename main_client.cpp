#include "Tarbora/Views/GraphicViews/inc/HumanView.hpp"

using namespace Tarbora;

int main() {
    LOG_LEVEL(DEBUG);

    std::shared_ptr<HumanView> human_view(new HumanView("localhost:50051"));
    human_view->Run();

    return 0;
}
