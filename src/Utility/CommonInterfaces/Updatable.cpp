#include "Updatable.h"

Updatable::Updatable() {
    refreshTick();
}

Updatable::Updatable(const Updatable &u) : m_lastTick(u.m_lastTick) {}

Updatable::Updatable(Updatable &&u) noexcept : m_lastTick(u.m_lastTick) {}

void Updatable::refreshTick() {
    m_lastTick = std::chrono::steady_clock::now();
}

double Updatable::getDuration() const {
    return std::chrono::duration<double>(std::chrono::steady_clock::now() - m_lastTick).count();
}

bool Updatable::update() {
    bool result = mainUpdate();

    refreshTick();

    return result;
}

Updatable::Mutex &Updatable::getMutex() {
    return m_mutex;
}
