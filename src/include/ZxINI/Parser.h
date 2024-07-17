#pragma once
#include <string>
#include <optional>
#include <string_view>
#include <unordered_map>
#include <ZxINI/Value.h>


namespace ZQF::ZxINI
{
    // https://www.cppstories.com/2021/heterogeneous-access-cpp20/
    struct string_hash
    {
        using is_transparent = void;
        [[nodiscard]] size_t operator()(const char* cpKey) const
        {
            return std::hash<std::string_view>{}(cpKey);
        }

        [[nodiscard]] size_t operator()(const std::string_view msKey) const
        {
            return std::hash<std::string_view>{}(msKey);
        }

        [[nodiscard]] size_t operator()(const std::string& msKey) const
        {
            return std::hash<std::string>{}(msKey);
        }
    };

    using Key = std::string;
    using NodeName = std::string;
    using KeysMap = std::unordered_map<ZxINI::Key, ZxINI::Value, string_hash, std::equal_to<>>;
    using NodesMap = std::unordered_map<NodeName, KeysMap, string_hash, std::equal_to<>>;


    class Parser
    {
    private:
        NodesMap m_mpNodes;

    public:
        Parser();
        Parser(const std::string_view msPath);

    public:
        auto Dump() const->std::string;
        auto Parse(const std::string_view msPath) -> void;

    public:
        auto Save(const std::string_view msPath, bool isForceSave) const -> void;

    public:
        auto operator[] (size_t)->KeysMap & = delete;
        auto operator[] (const std::string_view msNode)->KeysMap&;
        auto Exist(const std::string_view msNode) const -> bool;
        auto Exist(const std::string_view msNode, const std::string_view msKey) -> bool;
        auto Get(const std::string_view msNode) -> std::optional<KeysMap*>;
        auto Get(const std::string_view msNode, const std::string_view msKey) -> std::optional<Value*>;
    };
}
