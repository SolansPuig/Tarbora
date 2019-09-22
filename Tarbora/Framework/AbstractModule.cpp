#include "AbstractModule.hpp"

namespace Tarbora {
    AbstractModule::~AbstractModule()
    {
        if (m_UsingThread) m_Thread.join();
    }

    void AbstractModule::RunThread()
    {
        m_UsingThread = true;
        m_Thread = std::thread(&AbstractModule::Run, this);
    }

    void AbstractModule::Close()
    {
        AbstractModule::m_Run = false;
    }

    bool AbstractModule::m_Run = true;
}
