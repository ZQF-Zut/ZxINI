#include <ZxINI/Platform.h>

#include <span>
#include <format>

#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#elif __linux__
#include <fcntl.h>
#include <unistd.h>
#endif


namespace ZQF::ZxINI::Private
{
#ifdef _WIN32
    static auto PathUtf8ToWide(const std::string_view msPath) -> std::unique_ptr<wchar_t[]>
    {
        const size_t buffer_max_chars = (msPath.size() * sizeof(char) + 1) * 2;
        auto buffer = std::make_unique_for_overwrite<wchar_t[]>(buffer_max_chars);
        const auto char_count_real = static_cast<size_t>(::MultiByteToWideChar(CP_UTF8, MB_ERR_INVALID_CHARS, msPath.data(), static_cast<int>(msPath.size()), buffer.get(), static_cast<int>(buffer_max_chars)));
        buffer[char_count_real] = {};
        return std::move(buffer);
    }

    auto ReadAllBytes(const std::string_view msPath) -> std::pair<size_t, std::unique_ptr<char[]>>
    {
        const HANDLE hfile = ::CreateFileW(PathUtf8ToWide(msPath).get(), GENERIC_READ, FILE_SHARE_READ, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);
        if (hfile == INVALID_HANDLE_VALUE) { throw std::runtime_error(std::format("ZxINI::RreadAllBytes(): open file error!. msPath: {}", msPath)); }
        const auto file_size = static_cast<size_t>(::GetFileSize(hfile, nullptr));
        auto file_buffer = std::make_unique_for_overwrite<char[]>(file_size + 2);
        DWORD read{};
        (void)::ReadFile(hfile, file_buffer.get(), static_cast<DWORD>(file_size), &read, nullptr);
        (void)::CloseHandle(hfile);
        file_buffer[file_size + 0] = '\n';
        file_buffer[file_size + 1] = '\0';
        return { file_size, std::move(file_buffer) };
    }

    auto WriteAllBytes(const std::string_view msPath, const std::span<const char> spData, bool isForceCreate) -> void
    {
        const HANDLE hfile = ::CreateFileW(PathUtf8ToWide(msPath).get(), GENERIC_WRITE, FILE_SHARE_READ, nullptr, isForceCreate ? CREATE_ALWAYS : CREATE_NEW, FILE_ATTRIBUTE_NORMAL, nullptr); // NOLINT
        if (hfile == INVALID_HANDLE_VALUE) { throw std::runtime_error(std::format("ZxINI::WriteAllBytes(): create file error!. msPath: {}", msPath)); }
        DWORD write{};
        (void)::WriteFile(hfile, spData.data(), static_cast<DWORD>(spData.size_bytes()), &write, nullptr);
        (void)::CloseHandle(hfile);
    }
#else
    auto ReadAllBytes(const std::string_view msPath) -> std::pair<size_t, std::unique_ptr<char[]>>
    {
        const auto file_handle = ::open(msPath.data(), O_RDONLY, 0666);
        if (file_handle == -1) { throw std::runtime_error(std::format("ZxINI::RreadAllBytes(): open file error!. msPath: {}", msPath)); }
        const auto file_size = ::lseek(file_handle, 0, SEEK_END);
        auto buffer = std::make_unique_for_overwrite<char[]>(file_size + 2);
        ::lseek(file_handle, 0, SEEK_SET);
        ::read(file_handle, buffer.get(), file_size);
        ::close(file_handle);
        buffer[file_size + 0] = '\n';
        buffer[file_size + 1] = '\0';
        return { file_size, std::move(buffer) };
    }

    auto WriteAllBytes(const std::string_view msPath, const std::span<const char> spData, bool isForceCreate) -> void
    {
        constexpr auto create_always = O_CREAT | O_WRONLY | O_TRUNC;
        constexpr auto create_new = O_CREAT | O_WRONLY | O_EXCL;
        const auto file_handle = ::open(msPath.data(), isForceCreate ? create_always : create_new, 0666);  // NOLINT
        if (file_handle == -1) { throw std::runtime_error(std::format("ZxINI::WriteAllBytes(): create file failed!", msPath)); }
        ::write(file_handle, spData.data(), spData.size_bytes());
        ::close(file_handle);
    }
#endif
} // namespace ZQF::ZxINI::Private
