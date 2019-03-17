#include "Application.hpp"

int main() {
    Tarbora::Application* app = new Tarbora::Application();

    app->Run();

    delete app;
    return 0;
}
