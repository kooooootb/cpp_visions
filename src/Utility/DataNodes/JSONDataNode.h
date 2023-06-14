#ifndef VISIONS_JSONDATANODE_H
#define VISIONS_JSONDATANODE_H

#include <JSON/Node.h>

#include <filesystem>
#include <memory>
#include <map>

#include "Point.h"
#include "DataNode.h"

/**
 * Access data within config.json file
 */
class JSONDataNode : public DataNode {
private:
    JSON::Object m_data;

    std::filesystem::path m_file;
    std::shared_ptr<JSON::Node> m_eldestNode;
public:
    JSONDataNode(JSON::Object data_, std::filesystem::path file_, std::shared_ptr<JSON::Node> eldestNode);
    explicit JSONDataNode(std::filesystem::path file);

    ~JSONDataNode() override = default;

    [[nodiscard]] std::filesystem::path getFolder() const override;
    [[nodiscard]] std::filesystem::path getFolder(const std::string &key) const override;

    [[nodiscard]] double getNumber(const std::string &key) const override;
    [[nodiscard]] std::string getString(const std::string &key) const override;
    [[nodiscard]] std::vector<std::shared_ptr<DataNode>> getNodesList(const std::string &key) const override;
    [[nodiscard]] std::vector<std::string> getStringsList(const std::string &key) const override;

    [[nodiscard]] std::map<int, std::pair<int, int>> getPairsMap(const std::string &key) const override;
    [[nodiscard]] std::vector<Point> getPointsVector(const std::string &key) const override;

    [[nodiscard]] bool contains(const std::string &key) const override;

    [[nodiscard]] std::unique_ptr<DataNode> getDerived(const std::string &key) const override;

    static std::string formatNumber(const std::string &key, double value);
    static std::string formatString(const std::string &key, const std::string &value);
    static std::string formatPairsMap(const std::string &key, const std::map<int, std::pair<int, int>> &map);
    static std::string formatPointsVector(const std::string &key, const std::vector<Point> &points);

    // saving to file
    void refreshFile() const override;

    // changing string by key
    void updateStringValue(const std::string &key, std::string newValue) override;

    // obtaining keys
    [[nodiscard]] std::vector<std::string> getKeys() const override;
};

using MainDataNode = JSONDataNode;

#endif //VISIONS_JSONDATANODE_H
