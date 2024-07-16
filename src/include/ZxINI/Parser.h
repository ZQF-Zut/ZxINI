#pragma once
#include <string>
#include <optional>
#include <string_view>
#include <unordered_map>
#include <ZxINI/Value.h>


namespace ZQF::ZxINI
{
    using Key = std::string;
    using NodeName = std::string;
    using KeysMap = std::unordered_map<ZxINI::Key, ZxINI::Value>;
    using NodesMap = std::unordered_map<NodeName, KeysMap>;


    class Parser
    {
    private:
        NodesMap m_mpNodes;

    public:
        Parser();
        Parser(const std::string_view msPath);

    public:
        auto Dump() const -> std::string;
        auto Parse(const std::string_view msPath) -> void;

    public:
        auto Save(const std::string_view msPath, bool isForceSave) const -> void;
        
        //auto Get(const std::string_view msNode) -> std::optional<std::reference_wrapper<KeysMap>>
        //{
        //    auto ite = m_mpNodes.find(msNode.data());
        //    return (ite != m_mpNodes.end()) ? std::optional<KeysMap&>{ite->second} : std::nullopt;
        //}

        //auto Get(const std::string_view msNode, const std::string_view msKey) -> std::optional<std::reference_wrapper<Value>>
        //{
        //    if (auto keys_map_opt = this->Get(msNode))
        //    {
        //        auto& keys_map = *keys_map_opt;
        //        auto ite = keys_map.get().find(msKey.data());
        //        return (ite != keys_map.get().end()) ? std::optional<Value&>{ite->second} : std::nullopt;
        //    }
        //    return std::nullopt;
        //}

        //auto operator[] (size_t)->KeysMap & = delete;
        //auto operator[] (const std::string_view msNode)->KeysMap&
        //{
        //    if (auto keys_map_opt = this->Get(msNode))
        //    {
        //        return (*keys_map_opt);
        //    }

        //    throw std::runtime_error("not find node");
        //}
    };
}
