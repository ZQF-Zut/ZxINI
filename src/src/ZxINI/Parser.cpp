#include <ZxINI/Parser.h>
#include <ZxINI/Platform.h>
#include <cassert>
#include <stdexcept>


namespace ZQF::ZxINI
{
    Parser::Parser()
    {

    }

    Parser::Parser(const std::string_view msPath)
    {
        this->Parse(msPath);
    }

    auto Parser::Dump() const -> std::string
    {
        std::string dump_str;

        for (auto& node : m_mpNodes)
        {
            dump_str.append(1, '[');
            dump_str.append(node.first);
            dump_str.append(1, ']');
            dump_str.append(1, '\n');

            for (auto& key : node.second)
            {
                dump_str.append(key.first);
                dump_str.append(1, '=');
                dump_str.append(key.second.Get<std::string_view>());
                dump_str.append(1, '\n');
            }

            dump_str.append(1, '\n');
        }

        return dump_str;
    }

    auto Parser::Parse(const std::string_view msPath) -> void
    {
        auto [ini_data_bytes, ini_data] = Private::ReadAllBytes(msPath);
        char* cur_ptr = ini_data.get();

        auto fn_skip_white = [&cur_ptr]()
            {
                while (true)
                {
                    switch (cur_ptr[0])
                    {
                    case '\t': case '\n': case '\r': case ' ': cur_ptr++; continue;
                    }
                    break;
                }
            };

        auto fn_next_line = [&cur_ptr]()
            {
                do
                {
                    cur_ptr++;
                } while ((*cur_ptr != '\n') && (*cur_ptr != '\r'));
            };

        auto fn_parse_kv = [&cur_ptr, &fn_skip_white]()-> std::pair<std::string_view, std::string_view>
            {
                std::string_view key;
                {
                    while (*cur_ptr == ' ') { cur_ptr++; }

                    const char* key_beg_ptr = cur_ptr;
                    char* key_end_ptr = cur_ptr;
                    while (*key_end_ptr != '=') { key_end_ptr++; }

                    cur_ptr = key_end_ptr + 1;

                    while (*--key_end_ptr == ' ');
                    key = { key_beg_ptr, key_end_ptr + 1 };
                }

                std::string_view val;
                {
                    while (*cur_ptr == ' ') { cur_ptr++; }

                    const char* val_beg_ptr = cur_ptr;
                    char* val_end_ptr = cur_ptr;
                    while ((*val_end_ptr != '\n') && (*val_end_ptr != '\r')) { val_end_ptr++; }

                    cur_ptr = val_end_ptr + 2;

                    while (*--val_end_ptr == ' ');
                    if ((*val_beg_ptr == '\"') && (*val_end_ptr == '\"'))
                    {
                        val = { val_beg_ptr + 1, val_end_ptr };
                    }
                    else
                    {
                        val = { val_beg_ptr, val_end_ptr + 1 };
                    }
                }

                return { key, val };
            };

        auto fn_parse_node = [&cur_ptr, &fn_skip_white]() -> std::string_view
            {
                assert(*cur_ptr == '[');
                cur_ptr++;

                std::string_view node;
                {
                    while (*cur_ptr == ' ') { cur_ptr++; }

                    const char* node_beg_ptr = cur_ptr;
                    char* node_end_ptr = cur_ptr;
                    while (*node_end_ptr != ']' && *node_end_ptr != '\n' && *node_end_ptr != '\r') { node_end_ptr++; }
                    if (*node_end_ptr != ']') { throw std::runtime_error("parse key error!"); }

                    cur_ptr = node_end_ptr + 1;

                    while (*--node_end_ptr == ' ');
                    node = { node_beg_ptr, node_end_ptr + 1 };
                }

                return node;
            };


        KeysMap* keys_map_ptr{};
        while (true)
        {
            fn_skip_white();

            // check end
            const size_t read_bytes = cur_ptr - ini_data.get();
            if (read_bytes >= ini_data_bytes) { break; }

            switch (*cur_ptr)
            {
            case L'#':case L';':case L'/': // commet
            {
                fn_next_line();
            }
            break;

            case L'[': // node
            {
                keys_map_ptr = &m_mpNodes.operator[](std::string(fn_parse_node()));
            }
            break;

            default: // key-value
            {
                if (keys_map_ptr == nullptr) { throw std::runtime_error("at least one node"); }
                keys_map_ptr->insert(fn_parse_kv());
            }
            break;
            }
        }
    }

    auto Parser::Save(const std::string_view msPath, bool isForceSave) const -> void
    {
        const std::string dump_str = this->Dump();
        Private::WriteAllBytes(msPath, dump_str, isForceSave);
    }

}
