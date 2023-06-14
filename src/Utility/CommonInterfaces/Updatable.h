#ifndef VISIONS_UPDATABLE_H
#define VISIONS_UPDATABLE_H

#include <chrono>
#include <mutex>

class Updatable {
public:
    // aliases
    using Mutex = std::mutex;

private:
    std::chrono::steady_clock::time_point m_lastTick;

    std::mutex m_mutex; // for changing critical data

protected:
    void refreshTick();
    [[nodiscard]] double getDuration() const;

    virtual bool mainUpdate() = 0;

public:
    Updatable();

    Updatable(const Updatable &u);
    Updatable(Updatable &&u) noexcept;

    ~Updatable() = default;

    virtual bool update(); // true if to remove

    Mutex &getMutex();
};


#endif //VISIONS_UPDATABLE_H
