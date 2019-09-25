#include "AbstractModule.hpp"

namespace Tarbora {
    AbstractModule::~AbstractModule()
    {
        if (m_UsingThread) m_Thread.join();
    }

    void AbstractModule::RunThread(std::string name)
    {
        m_UsingThread = true;
        m_Thread = std::thread(&AbstractModule::Run, this, name);
        tracy::SetThreadName(m_Thread, name.c_str());
    }

    void AbstractModule::Close()
    {
        AbstractModule::m_Run = false;
    }

    bool AbstractModule::m_Run = true;
}
