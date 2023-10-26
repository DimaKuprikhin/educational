#include <iomanip>
#include <long_integer.h>
#include <sstream>
#include <stdexcept>

LongInteger::LongInteger()
    : sign(1)
    {}

LongInteger::LongInteger(int64_t value)
    : sign(value >= 0 ? 1 : -1)
{
    while(value) {
        blocks.push_back(std::abs(value % base));
        value /= base;
    }
}

LongInteger::LongInteger(uint64_t value)
    : sign(1)
{
    while(value) {
        blocks.push_back(value % base);
        value /= base;
    }
}

LongInteger::LongInteger(const std::string_view& number)
    : sign(1)
    , blocks(0)
{
    blocks.reserve(number.size() / 9 + 1);
    uint64_t powerOfTen = 1;
    for(int i = 0; i < (int)number.size(); ++i) {
        int stringPosition = number.size() - 1 - i;
        if(stringPosition == 0 && (number[0] == '-' || number[0] == '+')) {
            sign = number[0] == '-' ? -1 : 1;
            continue;
        }
        powerOfTen *= 10;
        if(i % 9 == 0) {
            powerOfTen = 1;
            blocks.push_back(0);
        }
        if(number[stringPosition] < '0' || number[stringPosition] > '9') {
            throw std::runtime_error("The given string is not a number");
        }
        blocks.back() += (number[stringPosition] - '0') * powerOfTen;
    }
    Trim();
}

LongInteger::LongInteger(const LongInteger& other)
    : sign(other.sign)
    , blocks(other.blocks)
    {}

LongInteger::LongInteger(LongInteger&& other) noexcept
    : sign(other.sign)
    , blocks(std::move(other.blocks))
    {}

LongInteger& LongInteger::operator=(const LongInteger& other) {
    sign = other.sign;
    blocks = other.blocks;
    return *this;
}

LongInteger& LongInteger::operator=(LongInteger&& other) noexcept {
    sign = other.sign;
    blocks = std::move(other.blocks);
    return *this;
}

LongInteger LongInteger::operator+() const {
    return *this;
}

LongInteger LongInteger::operator-() const {
    auto result(*this);
    result.sign *= -1;
    return result;
}

LongInteger LongInteger::operator+(const LongInteger& rhs) const {
    auto result = *this;
    result += rhs;
    return result;
}

LongInteger& LongInteger::operator+=(const LongInteger& rhs) {
    if(sign == rhs.sign) {
        ElementWiseAddition(*this, rhs, *this);
    }
    else {
        if(LessAbsoluteValue(rhs, *this)) {
            ElementWiseSubtraction(*this, rhs, *this);
        }
        else {
            ElementWiseSubtraction(rhs, *this, *this);
            sign = -sign;
        }
    }
    return *this;
}

LongInteger LongInteger::operator-(const LongInteger& rhs) const {
    LongInteger result = *this;
    result -= rhs;
    return result;
}

LongInteger& LongInteger::operator-=(const LongInteger& rhs) {
    if(sign != rhs.sign) {
        ElementWiseAddition(*this, rhs, *this);
    }
    else {
        if(LessAbsoluteValue(rhs, *this)) {
            ElementWiseSubtraction(*this, rhs, *this);
        }
        else {
            ElementWiseSubtraction(rhs, *this, *this);
            sign = -sign;
        }
    }
    return *this;
}

LongInteger LongInteger::operator*(const LongInteger& rhs) const {
    auto result = *this;
    result *= rhs;
    return result;
}

LongInteger& LongInteger::operator*=(const LongInteger& rhs) {
    LongInteger result;
    for(size_t i = 0; i < rhs.blocks.size(); ++i) {
        auto summand = *this;
        summand.MultiplyByPowerOfBase(i);
        summand.MultiplyByLessThanBase(rhs.blocks[i]);
        result += summand;
    }
    result.sign = sign * rhs.sign;
    return *this = std::move(result);
}

LongInteger LongInteger::operator/(const LongInteger& rhs) const {
    auto result = *this;
    result /= rhs;
    return result;
}

LongInteger& LongInteger::operator/=(const LongInteger& rhs) {
    LongInteger result, temp;
    DivideWithRemainder(rhs, result, temp);
    return *this = std::move(result);
}

LongInteger LongInteger::operator%(const LongInteger& rhs) const {
    auto result = *this;
    result %= rhs;
    return result;
}

LongInteger& LongInteger::operator%=(const LongInteger& rhs) {
    LongInteger result, temp;
    DivideWithRemainder(rhs, temp, result);
    return *this = std::move(result);
}

bool LongInteger::operator<(const LongInteger& rhs) const noexcept {
    if(IsZero() && rhs.IsZero()) {
        return false;
    }
    if(sign != rhs.sign) {
        return sign == -1;
    }
    return (sign == 1 && LessAbsoluteValue(*this, rhs))
            || (sign == -1 && LessAbsoluteValue(rhs, *this));
}

bool LongInteger::operator>(const LongInteger& rhs) const noexcept {
    return rhs < *this;
}

bool LongInteger::operator<=(const LongInteger& rhs) const noexcept {
    return !(*this > rhs);
}

bool LongInteger::operator>=(const LongInteger& rhs) const noexcept {
    return !(*this < rhs);
}

bool LongInteger::operator==(const LongInteger& rhs) const noexcept {
    return !(*this < rhs || *this > rhs);
}

LongInteger LongInteger::Abs() const {
    auto result(*this);
    result.sign *= result.sign;
    return result;
}

bool LongInteger::IsZero() const noexcept {
    return !blocks.size();
}

std::string LongInteger::ToString() const {
    if(IsZero()) {
        return "0";
    }
    std::stringstream result("");
    if(sign == -1) {
        result << '-';
    }
    result << std::to_string(blocks.back());
    for(int i = blocks.size() - 2; i >= 0; --i) {
        result << std::setw(9) << std::setfill('0') << blocks[i];
    }
    return result.str();
}

int64_t LongInteger::At(size_t index) const noexcept {
    if(index < blocks.size()) {
        return blocks[index];
    }
    return 0;
}

void LongInteger::ElementWiseAddition(
    const LongInteger& first,
    const LongInteger& second,
    LongInteger& result) 
{
    result.blocks.resize(std::max(first.blocks.size(), second.blocks.size()) + 1, 0);
    int carry = 0;
    for(int i = 0; i < static_cast<int>(result.blocks.size()); ++i) {
        result.blocks[i] = carry + first.At(i) + second.At(i);
        carry = 0;
        if(result.blocks[i] >= base) {
            result.blocks[i] -= base;
            carry = 1;
        }
    }
    result.Trim();
}

void LongInteger::ElementWiseSubtraction(
    const LongInteger& lhs,
    const LongInteger& rhs,
    LongInteger& result)
{
    result.blocks.resize(std::max(lhs.blocks.size(), rhs.blocks.size()), 0);
    int carry = 0;
    for(int i = 0; i < static_cast<int>(result.blocks.size()); ++i) {
        result.blocks[i] = lhs.At(i) - rhs.At(i) - carry;
        carry = 0;
        if(result.blocks[i] < 0) {
            result.blocks[i] += base;
            carry = 1;
        }
    }
    result.Trim();
}

void LongInteger::DivideByLessThanBase(const int64_t value) {
    if(value >= base) {
        throw std::runtime_error("Value must be less than base");
    }
    int64_t carry = 0;
    for(int i = blocks.size() - 1; i >= 0; --i) {
        int64_t block = (carry * base + blocks[i]) / value;
        carry = (carry * base + blocks[i]) % value;
        blocks[i] = block;
    }
    Trim();
}

void LongInteger::DivideWithRemainder(const LongInteger& divisor,
                         LongInteger& quotient,
                         LongInteger& remainder) const
{
    if(divisor.IsZero()) {
        throw std::runtime_error("Division by zero");
    }
    if(blocks.size() < divisor.blocks.size()) {
        quotient = 0;
        remainder = *this;
        return;
    }

    remainder = this->Abs();
    // subtrahend - вычитаемое, использующиеся для деления в столбик вида
    // |divisor| * 10 ^ n : |divisor| * 10 ^ n <= *this && |divisor| * 10 ^ (n + 1) > *this.
    LongInteger subtrahend = divisor.Abs();
    subtrahend.MultiplyByPowerOfBase(blocks.size() - divisor.blocks.size());
    quotient = 0;
    // quotientSummand - число, на которое увеличится quotient при вычитании
    // subtrahend из remainder. quotientSummand = subtrahend / divisor.
    LongInteger quotientSummand(1);
    quotientSummand.MultiplyByPowerOfBase(blocks.size() - divisor.blocks.size());
    // Умножаем subtrahend на максимальную степень 10, чтобы выполнялось условие выше.
    while(subtrahend < remainder) {
        subtrahend.MultiplyByLessThanBase(10);
        quotientSummand.MultiplyByLessThanBase(10);
    }
    subtrahend.DivideByLessThanBase(10);
    quotientSummand.DivideByLessThanBase(10);

    // Производим деление в столбик.
    while(remainder >= divisor && quotientSummand > 0) {
        while(remainder >= subtrahend) {
            remainder -= subtrahend;
            quotient += quotientSummand;
        }
        subtrahend.DivideByLessThanBase(10);
        quotientSummand.DivideByLessThanBase(10);
    }
    remainder.sign = sign;
    quotient.sign = sign * divisor.sign;
}

bool LongInteger::LessAbsoluteValue(
    const LongInteger& lhs,
    const LongInteger& rhs) noexcept
{
    if(lhs.blocks.size() != rhs.blocks.size()) {
        return lhs.blocks.size() < rhs.blocks.size();
    }
    for(int i = lhs.blocks.size() - 1; i >= 0; --i) {
        if(lhs.At(i) != rhs.At(i)) {
            return lhs.At(i) < rhs.At(i);
        }
    }
    return false;
}

void LongInteger::MultiplyByLessThanBase(const int64_t value) {
    if(value > base) {
        throw std::runtime_error("Value must be less than base");
    }
    int64_t carry = 0;
    for(auto& block : blocks) {
        block = block * value + carry;
        carry = 0;
        if(block >= base) {
            carry = block / base;
            block %= base;
        }
    }
    if(carry > 0) {
        blocks.push_back(carry);
    }
}

void LongInteger::MultiplyByPowerOfBase(const uint64_t power) {
    for(int i = 0; i < static_cast<int>(power); ++i) {
        blocks.push_back(0);
    }
    for(int i = blocks.size() - 1; i >= 0; --i) {
        if(i >= static_cast<int>(power)) {
            blocks[i] = blocks[i - power];
        }
        else {
            blocks[i] = 0;
        }
    }
}

void LongInteger::Trim() noexcept {
    while(blocks.size() && !blocks.back()) {
        blocks.pop_back();
    }
}

std::ostream& operator<<(std::ostream& stream, const LongInteger& value) {
    return stream << value.ToString();
}

std::istream& operator>>(std::istream& stream, LongInteger& value) {
    std::string str;
    stream >> str;
    value = LongInteger(str);
    return stream;
}

LongInteger operator"" _li(unsigned long long int value) {
    return LongInteger(value);
}

LongInteger operator"" _li(const char* string, size_t size) {
    LongInteger result(std::string(string, size));
    return result;
}
