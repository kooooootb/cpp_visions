#include "Updater.h"

Updater::Updater() : m_worker(&Updater::work, this)
{}

Updater::~Updater() {
    m_stopping = true;

    m_workerCv.notify_one();

    m_worker.join();
}

void Updater::work() {
    while(!m_stopping){
        std::unique_lock<std::mutex> lock(m_workerMtx);
        m_workerCv.wait(lock);
        lock.unlock();

        if(!m_taskDone){
            for(auto &ptr : m_updatables){
                ptr->update();
            }

            std::lock_guard<std::mutex> lockGuard(m_taskWaiterMtx);
            m_taskDone = true;
            m_taskWaiterCv.notify_one();
        }
    }
}

void Updater::resetUpdatables() {
    m_updatables.clear();
}

void Updater::addUpdatable(Updatable *updatable){
    m_updatables.push_back(updatable);
}

void Updater::runUpdate(){
    m_taskDone = false;
    m_workerCv.notify_one();
}

void Updater::waitUntilDone() {
    std::unique_lock<std::mutex> lock(m_taskWaiterMtx);

    while(!m_taskDone){
        m_taskWaiterCv.wait(lock);
        lock.unlock();
    }
}
