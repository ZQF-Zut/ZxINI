#pragma once
#include <string>


namespace ZQF::ZxINI
{
    class Value
    {
    private:
        std::string m_msValue;

    public:
        Value()
        {

        }

        template <class T>
        Value(T&& rfData) noexcept
        {
            this->operator= <T>(std::forward<T>(rfData));
        }

        template <class T>
        auto operator=(T&& rfData) noexcept -> Value&
        {
            if constexpr (std::is_same_v<T, Value>)
            {
                m_msValue = std::move(rfData.m_msValue);
            }
            else if constexpr (std::is_same_v<T, std::string>)
            {
                m_msValue = std::move(rfData);
            }
            else if constexpr (std::is_same_v<std::remove_cvref_t<T>, Value>)
            {
                m_msValue = rfData.m_msValue;
            }
            else if constexpr (std::is_same_v<std::remove_cvref_t<T>, std::string> || std::is_bounded_array_v<std::remove_cvref_t<T>> || std::is_same_v<std::remove_cvref_t<T>, std::string_view>)
            {
                m_msValue = rfData;
            }
            else if constexpr (std::is_same_v<std::remove_cvref_t<T>, bool>)
            {
                m_msValue = rfData ? "true" : "false";
            }
            else if constexpr (std::is_integral_v<std::remove_cvref_t<T>> || std::is_floating_point_v<std::remove_cvref_t<T>>)
            {
                m_msValue = std::to_string(rfData);
            }
            else
            {
               static_assert(false, "ZxINI::Value<>(): error type!");
            }

            return *this;
        }

        template <class T>
        auto Get() const -> T
        {
            if constexpr (std::is_same_v<std::remove_cvref_t<T>, bool>)
            {
                if (m_msValue == "true")
                {
                    return true;
                }
                else if (m_msValue == "false")
                {
                    return false;
                }
            }
            else if constexpr (std::is_integral_v<std::remove_cvref_t<T>>)
            {
                char* end_ptr{};
                const auto c_str = m_msValue.data();
                const auto radix = m_msValue.starts_with("0x") ? 16 : 10;
                return static_cast<T>(std::is_unsigned_v<T> ? std::strtoull(c_str, &end_ptr, radix) : std::strtoll(c_str, &end_ptr, radix));
            }
            else if constexpr (std::is_floating_point_v<std::remove_cvref_t<T>>)
            {
                return static_cast<T>(std::stod(m_msValue.data(), nullptr));
            }
            else if constexpr (std::is_same_v<std::remove_cvref_t<T>, std::string> || std::is_same_v<std::remove_cvref_t<T>, std::string_view>)
            {
                return m_msValue;
            }
            else if constexpr (std::is_same_v<T, const char*>)
            {
                return m_msValue.c_str();
            }
            else
            {
                static_assert(false, "ZxINI::Get<>(): error type!");
            }
        }
    };
}
