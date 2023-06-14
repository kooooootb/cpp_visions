#include "EntityStorage.h"

EntityStorage::Index EntityStorage::addAlive(std::unique_ptr<Alive> &&alive) {
    m_alives.insert({m_alivesIndex, std::move(alive)});
    return m_alivesIndex++;
}

EntityStorage::Index EntityStorage::addPickable(std::unique_ptr<Pickable> &&pickable) {
    m_pickables.insert({m_pickablesIndex, std::move(pickable)});
    return m_pickablesIndex++;
}

EntityStorage::Alive_ptr EntityStorage::removeAlive(EntityStorage::Index index) {
    auto it = m_alives.find(index);
    auto result = std::move(it->second);
    m_alives.erase(it);
    return result;
}

EntityStorage::Pickable_ptr EntityStorage::removePickable(EntityStorage::Index index) {
    auto it = m_pickables.find(index);
    auto result = std::move(it->second);
    m_pickables.erase(it);
    return result;
}

void EntityStorage::manageAlives() {
    // removing
    while(!m_removeAlivesQ.empty()){
        auto index = m_removeAlivesQ.front();
        m_removeAlivesQ.pop();
        removeAlive(index);
    }

    // adding
    while(!m_addAlivesQ.empty()){
        auto alive = std::move(m_addAlivesQ.front());
        m_addAlivesQ.pop();
        addAlive(std::move(alive));
    }
}

void EntityStorage::managePickables() {
    // removing
    while(!m_removePickablesQ.empty()){
        auto index = m_removePickablesQ.front();
        m_removePickablesQ.pop();
        removePickable(index);
    }

    // adding
    while(!m_addPickablesQ.empty()){
        auto pickable = std::move(m_addPickablesQ.front());
        m_addPickablesQ.pop();
        addPickable(std::move(pickable));
    }
}

void EntityStorage::cleanup() {
    // this is where all modifications apply
    // set changed flag
    if(!m_addAlivesQ.empty() || !m_addPickablesQ.empty() || !m_removeAlivesQ.empty() || !m_removePickablesQ.empty()){
        changed = true;
    }

    // apply
    manageAlives();
    managePickables();
}

void EntityStorage::queueAddAlive(EntityStorage::Alive_ptr &&alive) {
    m_addAlivesQ.push(std::move(alive));
}

void EntityStorage::queueAddPickable(EntityStorage::Pickable_ptr &&pickable) {
    m_addPickablesQ.push(std::move(pickable));
}

void EntityStorage::queueRemoveAlive(EntityStorage::Index key) {
    m_removeAlivesQ.push(key);
}

void EntityStorage::queueRemovePickable(EntityStorage::Index key) {
    m_removePickablesQ.push(key);
}

EntityStorage::Alive_ptr EntityStorage::popAlive(EntityStorage::Index index) {
    auto it = m_alives.find(index);
    Alive_ptr result = nullptr;

    if(it != m_alives.end()){
        result = std::move(it->second);
        m_alives.erase(it);
    }

    return result;
}

EntityStorage::Pickable_ptr EntityStorage::popPickable(EntityStorage::Index index) {
    auto it = m_pickables.find(index);
    Pickable_ptr result = nullptr;

    if(it != m_pickables.end()){
        result = std::move(it->second);
        m_pickables.erase(it);
    }

    return result;
}

EntityStorage::AlivesArray &EntityStorage::getAlives() {
    return m_alives;
}

EntityStorage::PickablesArray &EntityStorage::getPickables() {
    return m_pickables;
}

bool EntityStorage::hasChanged() const {
    return changed;
}

void EntityStorage::resetHasChanged() {
    changed = false;
}

EntityStorage::CAlivesIterator::CAlivesIterator(EntityStorage::Alives_cit it, EntityStorage::Alives_cit end) :
    m_it(it) , m_end(end)
{}

EntityStorage::Alive_mt EntityStorage::CAlivesIterator::operator*() const {
    return { m_it->first, *m_it->second };
}

EntityStorage::Alives_cit &EntityStorage::CAlivesIterator::operator++() {
//    while(++m_it != m_end && m_it->second == nullptr);
    return ++m_it;
}

bool EntityStorage::CAlivesIterator::operator!=(const CAlivesIterator &cait) const {
    return m_it != cait.m_it;
}

EntityStorage::CAlivesIteratorHelper::CAlivesIteratorHelper(const AlivesArray &aa) : m_ref(aa) {}

EntityStorage::CAlivesIterator EntityStorage::CAlivesIteratorHelper::begin() const {
    return CAlivesIterator(std::begin(m_ref), std::end(m_ref));
}

EntityStorage::CAlivesIterator EntityStorage::CAlivesIteratorHelper::end() const {
    return CAlivesIterator(std::end(m_ref), std::end(m_ref));
}

EntityStorage::CPickablesIterator::CPickablesIterator(EntityStorage::Pickables_cit it, EntityStorage::Pickables_cit end) :
    m_it(it) , m_end(end)
{}

EntityStorage::Pickable_mt EntityStorage::CPickablesIterator::operator*() const {
    return { m_it->first, *m_it->second };
}

EntityStorage::Pickables_cit &EntityStorage::CPickablesIterator::operator++() {
//    while(++m_it != m_end && m_it->second == nullptr);
    return ++m_it;
}

bool EntityStorage::CPickablesIterator::operator!=(const CPickablesIterator &cait) const {
    return m_it != cait.m_it;
}

EntityStorage::CPickablesIteratorHelper::CPickablesIteratorHelper(const PickablesArray &aa) : m_ref(aa) {}

EntityStorage::CPickablesIterator EntityStorage::CPickablesIteratorHelper::begin() const {
    return CPickablesIterator(std::begin(m_ref), std::end(m_ref));
}

EntityStorage::CPickablesIterator EntityStorage::CPickablesIteratorHelper::end() const {
    return CPickablesIterator(std::end(m_ref), std::end(m_ref));
}

EntityStorage::CEntitiesIterator::CEntitiesIterator(Alives_cit ait, Alives_cit aend, Pickables_cit pit) :
    m_ait(ait) , m_aend(aend) , m_pit(pit)
{}

EntityStorage::Entity_mt EntityStorage::CEntitiesIterator::operator*() const {
    if(m_ait == m_aend){
        return { m_pit->first, *m_pit->second };
    } else {
        return { m_ait->first, *m_ait->second };
    }
}

void EntityStorage::CEntitiesIterator::operator++() {
    if(m_ait == m_aend)
        ++m_pit;
    else
        ++m_ait;
}

bool EntityStorage::CEntitiesIterator::operator!=(const CEntitiesIterator &cait) const {
    if(m_ait == m_aend){
        if(cait.m_ait == cait.m_aend){
            return m_pit != cait.m_pit;
        } else{
            return true;
        }
    } else{
        if(cait.m_ait == cait.m_aend){
            return true;
        } else{
            return m_ait != cait.m_ait;
        }
    }
}

EntityStorage::CEntitiesIteratorHelper::CEntitiesIteratorHelper(const AlivesArray &aa, const PickablesArray &pa) :
    m_ar(aa) , m_pr(pa)
{}

EntityStorage::CEntitiesIterator EntityStorage::CEntitiesIteratorHelper::begin() const {
    return CEntitiesIterator(std::begin(m_ar), std::end(m_ar), std::begin(m_pr));
}

EntityStorage::CEntitiesIterator EntityStorage::CEntitiesIteratorHelper::end() const {
    return CEntitiesIterator(std::end(m_ar), std::end(m_ar), std::end(m_pr));
}

EntityStorage::CAlivesIteratorHelper EntityStorage::iterateAlives() const {
    return CAlivesIteratorHelper(m_alives);
}

EntityStorage::CPickablesIteratorHelper EntityStorage::iteratePickables() const {
    return CPickablesIteratorHelper(m_pickables);
}

EntityStorage::CEntitiesIteratorHelper EntityStorage::iterateEntities() const {
    return CEntitiesIteratorHelper(m_alives, m_pickables);
}
