#include "AbstractModule.hpp"

namespace Tarbora {
    AbstractModule::~AbstractModule()
    {
        if (using_thread_) thread_.join();
    }

    void AbstractModule::runThread(const std::string &name)
    {
        using_thread_ = true;
        thread_ = std::thread(&AbstractModule::run, this, name);
        tracy::SetThreadName(thread_, name.c_str());
    }

    void AbstractModule::close()
    {
        AbstractModule::run_ = false;
    }

    bool AbstractModule::run_ = true;
}
