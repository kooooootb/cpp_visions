#ifndef VISIONS_UPDATER_H
#define VISIONS_UPDATER_H

#include <memory>
#include <thread>
#include <atomic>
#include <condition_variable>
#include <vector>

#include "Updatable.h"

class Updater {
private:
    // main updatable
    std::vector<Updatable *> m_updatables;

    // worker
    std::thread m_worker;
    std::condition_variable m_workerCv;
    std::mutex m_workerMtx;
    std::condition_variable m_taskWaiterCv;
    std::mutex m_taskWaiterMtx;
    std::atomic<bool> m_taskDone{false};
    std::atomic<bool> m_stopping{false};

    void work();
public:
    Updater();

    ~Updater();

    void resetUpdatables();
    void addUpdatable(Updatable *updatable);

    void runUpdate();

    void waitUntilDone();
};


#endif //VISIONS_UPDATER_H
