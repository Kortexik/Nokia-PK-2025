#include <Messages/PhoneNumber.hpp>
#include <iomanip>

namespace common
{

constexpr const PhoneNumber::Value PhoneNumber::INVALID_VALUE;
constexpr const PhoneNumber::Value PhoneNumber::MIN_VALUE;
constexpr const PhoneNumber::Value PhoneNumber::MAX_VALUE;
constexpr const std::size_t PhoneNumber::DIGITS;


std::istream& operator >> (std::istream& is, PhoneNumber& obj)
{
    std::uint32_t value;
    if (is >> value)
    {
        static_assert(sizeof(value) >= sizeof(obj.value), "select different type for value");

        obj.value = static_cast<decltype(obj.value)>(value);
    }
    return is;
}

std::ostream& operator << (std::ostream& os, const PhoneNumber& obj)
{
    std::ios originalState(nullptr);
    originalState.copyfmt(os);

    os << std::dec << std::setfill('0') << std::setw(PhoneNumber::DIGITS) << 0u + obj.value;

    os.copyfmt(originalState);
    return os;
}

std::string to_string(const PhoneNumber &obj)
{
    return std::to_string(0u + obj.value);
}

}
