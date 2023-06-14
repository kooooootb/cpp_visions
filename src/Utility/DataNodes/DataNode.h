#ifndef VISIONS_DATANODE_H
#define VISIONS_DATANODE_H

#include <filesystem>
#include <memory>
#include <map>

#include "Point.h"

/**
 * Unifying accessing data
 */
class DataNode {
public:
    // destructor
    virtual ~DataNode() = default;

    // folders
    [[nodiscard]] virtual std::filesystem::path getFolder() const = 0;
    [[nodiscard]] virtual std::filesystem::path getFolder(const std::string &key) const = 0;

    // get by key
    [[nodiscard]] virtual double getNumber(const std::string &key) const = 0;
    [[nodiscard]] virtual std::string getString(const std::string &key) const = 0;
    [[nodiscard]] virtual std::vector<std::shared_ptr<DataNode>> getNodesList(const std::string &key) const = 0;
    [[nodiscard]] virtual std::vector<std::string> getStringsList(const std::string &key) const = 0;

    // get map or vector
    [[nodiscard]] virtual std::map<int, std::pair<int, int>> getPairsMap(const std::string &key) const = 0;
    [[nodiscard]] virtual std::vector<Point> getPointsVector(const std::string &key) const = 0;

    // check presence
    [[nodiscard]] virtual bool contains(const std::string &key) const = 0;

    // getting inner node
    [[nodiscard]] virtual std::unique_ptr<DataNode> getDerived(const std::string &key) const = 0; // can be used to rewrite values

    // saving to file
    virtual void refreshFile() const = 0;

    // changing string by key
    virtual void updateStringValue(const std::string &key, std::string newValue) = 0;

    // obtaining keys
    [[nodiscard]] virtual std::vector<std::string> getKeys() const = 0;
};


#endif //VISIONS_DATANODE_H
