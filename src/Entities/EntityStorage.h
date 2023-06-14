#ifndef VISIONS_ENTITYSTORAGE_H
#define VISIONS_ENTITYSTORAGE_H

#include <vector>
#include <queue>

#include "Alive.h"
#include "Pickable.h"
#include "Watcher.h"

class EntityStorage {
public:
    // aliases
    using Index = Watcher::Index;

    using Alive_ptr = std::unique_ptr<Alive>;
    using Pickable_ptr = std::unique_ptr<Pickable>;

    using AlivesArray = std::map<Index, Alive_ptr>;
    using PickablesArray = std::map<Index, std::unique_ptr<Pickable>>;

    using Alives_cit = AlivesArray::const_iterator;
    using Pickables_cit = PickablesArray::const_iterator;

    // storage doesn't mind changing properties of its elements
    using Alive_mt = std::pair<Index, Alive &>; // mapped type, returned by dereferencing iterator
    using Pickable_mt = std::pair<Index, Pickable &>;
    using Entity_mt = std::pair<Index, Entity &>;

private:
    // arrays
    AlivesArray m_alives;
    PickablesArray m_pickables;
    Index m_alivesIndex{0}, m_pickablesIndex{0};

    // late managing
    std::queue<Pickable_ptr> m_addPickablesQ;
    std::queue<Alive_ptr> m_addAlivesQ;
    std::queue<Index> m_removeAlivesQ, m_removePickablesQ;
    bool changed = false;

    // addings
    Index addAlive(Alive_ptr &&ptr);
    Index addPickable(Pickable_ptr &&ptr);
    // removings
    Alive_ptr removeAlive(Index index);
    Pickable_ptr removePickable(Index index);
    // add who needs to be add, remove who to be removed
    void manageAlives();
    void managePickables();

public:
    EntityStorage() = default;

    ~EntityStorage() = default;

    // iterating
    // over alives
    class CAlivesIterator{
    public:
        explicit CAlivesIterator(Alives_cit it, Alives_cit end);

        Alive_mt operator*() const;
        Alives_cit &operator++();
        bool operator!=(const CAlivesIterator &cait) const;

    private:
        Alives_cit m_it;
        Alives_cit m_end;
    };

    class CAlivesIteratorHelper{
    public:
        explicit CAlivesIteratorHelper(const AlivesArray &aa);

        [[nodiscard]] CAlivesIterator begin() const;
        [[nodiscard]] CAlivesIterator end() const;
    private:
        const AlivesArray &m_ref;
    };

    // over pickables
    class CPickablesIterator{
    public:
        explicit CPickablesIterator(Pickables_cit it, Pickables_cit end);

        Pickable_mt operator*() const;
        Pickables_cit &operator++();
        bool operator!=(const CPickablesIterator &cpit) const;

    private:
        Pickables_cit m_it;
        Pickables_cit m_end;
    };

    class CPickablesIteratorHelper{
    public:
        explicit CPickablesIteratorHelper(const PickablesArray &pa);

        [[nodiscard]] CPickablesIterator begin() const;
        [[nodiscard]] CPickablesIterator end() const;
    private:
        const PickablesArray &m_ref;
    };

    // over entities
    class CEntitiesIterator{
    public:
        explicit CEntitiesIterator(Alives_cit ait, Alives_cit aend, Pickables_cit pit);

        Entity_mt operator*() const;
        void operator++();
        bool operator!=(const CEntitiesIterator &cpit) const;

    private:
        Alives_cit m_ait;
        Alives_cit m_aend;
        Pickables_cit m_pit;
    };

    class CEntitiesIteratorHelper{
    public:
        explicit CEntitiesIteratorHelper(const AlivesArray &aa, const PickablesArray &pa);

        [[nodiscard]] CEntitiesIterator begin() const;
        [[nodiscard]] CEntitiesIterator end() const;
    private:
        const AlivesArray &m_ar;
        const PickablesArray &m_pr;
    };

    // calling iterators
    [[nodiscard]] CAlivesIteratorHelper iterateAlives() const;
    [[nodiscard]] CPickablesIteratorHelper iteratePickables() const;
    [[nodiscard]] CEntitiesIteratorHelper iterateEntities() const;

    // queue adding
    void queueAddAlive(Alive_ptr &&alive);
    void queueAddPickable(Pickable_ptr &&pickable);

    // queue removing
    void queueRemoveAlive(Index key);
    void queueRemovePickable(Index key);

    // immediate removing
    Alive_ptr popAlive(Index index);
    Pickable_ptr popPickable(Index index);

    // direct accessing
    [[nodiscard]] AlivesArray &getAlives();
    [[nodiscard]] PickablesArray &getPickables();

    // end cycle, perform addings and removings, clear queues
    void cleanup();

    // checking modifications
    bool hasChanged() const; // getting condition
    void resetHasChanged(); // reseting condition
};

#endif //VISIONS_ENTITYSTORAGE_H
