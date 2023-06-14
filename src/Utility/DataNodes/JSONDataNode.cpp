#include <JSON/Parser.h>

#include <cassert>
#include <string>
#include <fstream>
#include <utility>

#include "JSONDataNode.h"

JSONDataNode::JSONDataNode(JSON::Object data, std::filesystem::path file, std::shared_ptr<JSON::Node> eldestNode) :
    m_data(std::move(data)) , m_file(std::move(file)) , m_eldestNode(std::move(eldestNode))
{}

JSONDataNode::JSONDataNode(std::filesystem::path file_) : m_file(std::move(file_)) {
    std::ifstream ifs(m_file);
    assert(ifs.is_open() && "Can't open file for JSONDataNode");

    JSON::Parser parser(ifs);

    m_eldestNode = parser.getRoot();
    m_data = m_eldestNode->getValue<JSON::Object>();
}

double JSONDataNode::getNumber(const std::string &key) const {
    return (*m_data)[key]->getValue<double>();
}

std::string JSONDataNode::getString(const std::string &key) const {
    return *(*m_data)[key]->getValue<JSON::String>();
}

bool JSONDataNode::contains(const std::string &key) const{
    return m_data->find(key) != m_data->end();
}

std::map<int, std::pair<int, int>> JSONDataNode::getPairsMap(const std::string &key) const {
    auto pointsObject = (*m_data)[key]->getValue<JSON::Object>();

    std::map<int, std::pair<int, int>> result;
    for(const auto &node : *pointsObject){
        const JSON::Array &array = node.second->getValue<JSON::Array>();
        int x = static_cast<int>(array->front()->getValue<double>());
        int y = static_cast<int>(array->back()->getValue<double>());

        result.insert({std::stoi(node.first), {x, y}});
    }

    return result;
}

std::vector<Point> JSONDataNode::getPointsVector(const std::string &key) const {
    std::vector<Point> result;

    const auto &pointsArray = (*m_data)[key]->getValue<JSON::Array>();

    for(const auto &node : *pointsArray){
        auto pointCoords = node->getValue<JSON::Array>();
        auto x = pointCoords->front()->getValue<double>();
        auto y = pointCoords->back()->getValue<double>();

        result.emplace_back(x, y);
    }

    return result;
}

std::vector<std::string> JSONDataNode::getStringsList(const std::string &key) const {
    const auto &stringArray = (*m_data)[key]->getValue<JSON::Array>();
    std::vector<std::string> result;

    for(const auto &node : *stringArray){
        result.push_back(*node->getValue<JSON::String>());
    }

    return result;
}

std::string JSONDataNode::formatNumber(const std::string &key, double value) {
    return "\"" + key + "\":" + std::to_string(value);
}

std::string JSONDataNode::formatString(const std::string &key, const std::string &value) {
    return "\"" + key + "\":" + value;
}

std::string JSONDataNode::formatPairsMap(const std::string &key, const std::map<int, std::pair<int, int>> &map){
    std::string result;

    // add key and open object
    result += "\"";
    result += key;
    result += "\":{";

    // add every item
    for(auto it = std::begin(map);;){
        result += "\"";
        result += std::to_string(it->first);
        result += "\":[";
        result += std::to_string(it->second.first);
        result += ",";
        result += std::to_string(it->second.second);
        result += "]";
        if(++it != std::end(map)){
            result += ",";
        } else{
            break;
        }
    }

    // close object
    result += "}";

    return result;
}

std::string JSONDataNode::formatPointsVector(const std::string &key, const std::vector<Point> &points) {
    std::string result;

    // add key and open array
    result += "\"";
    result += key;
    result += "\":[";

    // add every point
    for(auto it = std::begin(points);;){
        result += "[";
        result += std::to_string(it->x);
        result += ",";
        result += std::to_string(it->y);
        result += "]";
        if(++it != std::end(points)){
            result += ",";
        } else{
            break;
        }
    }

    // close array
    result += "]";

    return result;
}

std::filesystem::path JSONDataNode::getFolder() const {
    return m_file.parent_path();
}

std::filesystem::path JSONDataNode::getFolder(const std::string &key) const {
    const std::filesystem::path &path = getString(key);

    if(path.is_absolute()){
        return path;
    } else{
        return m_file.parent_path() / path;
    }
}

std::unique_ptr<DataNode> JSONDataNode::getDerived(const std::string &key) const {
    return std::make_unique<JSONDataNode>((*m_data)[key]->getValue<JSON::Object>(), m_file, m_eldestNode);
}

std::vector<std::shared_ptr<DataNode>> JSONDataNode::getNodesList(const std::string &key) const {
    std::vector<std::shared_ptr<DataNode>> result;

    auto array = (*m_data)[key]->getValue<JSON::Array>();
    for(const auto &object : *array){
        result.push_back(std::make_shared<JSONDataNode>(object->getValue<JSON::Object>(), m_file, m_eldestNode));
    }

    return result;
}

void JSONDataNode::refreshFile() const {
    std::ofstream ofs(m_file, std::ios_base::trunc | std::ios_base::out);
    ofs << m_eldestNode;
    ofs.close();
}

void JSONDataNode::updateStringValue(const std::string &key, std::string newValue) {
    (*m_data)[key]->setValue(std::make_shared<std::string>(std::move(newValue)));
}

std::vector<std::string> JSONDataNode::getKeys() const {
    std::vector<std::string> keys;
    for(const auto &keyPair : *m_data){
        keys.push_back(keyPair.first);
    }

    return keys;
}
