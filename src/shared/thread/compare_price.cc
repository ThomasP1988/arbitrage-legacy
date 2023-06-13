#include <Disruptor/IEventHandler.h>
#include <mutex>
#include <condition_variable>
#include <iostream>
#include "shared/thread/compare_price.h"

struct PrintingEventHandler : Disruptor::IEventHandler< LongEvent >
{
    explicit PrintingEventHandler(int toProcess) : m_actuallyProcessed(0), m_toProcess(toProcess)
    {}

    void onEvent(LongEvent& event, int64_t, bool) override
    {
        std::cout << "Event: " << event.value << std::endl;

        if (++m_actuallyProcessed == m_toProcess)
            m_allDone.notify_all();
    }

    void waitEndOfProcessing()
    {
        std::unique_lock<decltype(m_mutex)> lk(m_mutex);
        m_allDone.wait(lk);
    }

private:
    std::mutex m_mutex;
    std::condition_variable m_allDone;
    int m_actuallyProcessed;
    int m_toProcess;
};

