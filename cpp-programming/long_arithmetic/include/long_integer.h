#pragma once
#include <string>
#include <type_traits>
#include <vector>

class LongInteger {
public:
    LongInteger();

    LongInteger(int64_t value);

    LongInteger(uint64_t value);

    template<typename T, std::enable_if_t<std::is_integral_v<T> && std::is_signed_v<T>, bool> = true>
    LongInteger(T value) : LongInteger(static_cast<int64_t>(value)) {}

    template<typename T, std::enable_if_t<std::is_integral_v<T> && std::is_unsigned_v<T>, bool> = true>
    LongInteger(T value) : LongInteger(static_cast<uint64_t>(value)) {}

    LongInteger(const std::string_view& number);

    LongInteger(const LongInteger& other);

    LongInteger(LongInteger&& other) noexcept;

    LongInteger& operator=(const LongInteger& other);

    LongInteger& operator=(LongInteger&& other) noexcept;

    LongInteger operator+() const;

    LongInteger operator-() const;

    LongInteger operator+(const LongInteger& rhs) const;

    LongInteger& operator+=(const LongInteger& rhs);

    LongInteger operator-(const LongInteger& rhs) const;

    LongInteger& operator-=(const LongInteger& rhs);

    // Алгоритмаическая сложность операций *, *=, /, /=, %, %=:
    // O(n ^ 2), где n - max(this->blocks.size(), rhs.blocks.size()).
    LongInteger operator*(const LongInteger& rhs) const;

    LongInteger& operator*=(const LongInteger& rhs);

    LongInteger operator/(const LongInteger& rhs) const;

    LongInteger& operator/=(const LongInteger& rhs);

    LongInteger operator%(const LongInteger& rhs) const;

    LongInteger& operator%=(const LongInteger& rhs);

    bool operator<(const LongInteger& rhs) const noexcept;

    bool operator>(const LongInteger& rhs) const noexcept;

    bool operator<=(const LongInteger& rhs) const noexcept;

    bool operator>=(const LongInteger& rhs) const noexcept;

    bool operator==(const LongInteger& rhs) const noexcept;

    LongInteger Abs() const;

    bool IsZero() const noexcept;

    std::string ToString() const;

private:
    // Возвращает значение по индексу из blocks, если index < blocks.size(),
    // иначе возвращает 0.
    int64_t At(size_t index) const noexcept;

    // Поэлементно суммирует first.blocks и second.blocks с учетом переносов
    // и записывает результат в result.blocks.
    static void ElementWiseAddition(
        const LongInteger& first,
        const LongInteger& second,
        LongInteger& result);

    // Поэлементно вычитает rhs.blocks из lhs.blocks с учетом переносов и
    // записывает результат в result.blocks. Корректно работает только если
    // lhs >= rhs.
    static void ElementWiseSubtraction(
        const LongInteger& lhs,
        const LongInteger& rhs,
        LongInteger& result);

    // Деление за O(blocks.size()) числа на value < base.
    void DivideByLessThanBase(const int64_t value);

    // Деление за O(blocks.size() + power) числа на base ^ power.
    void DivideByPowerOfBase(const uint64_t power);

    // Делит число с отатком на divisor. Результат деления записывается в
    // quotient и remainder. Верно, что *this = divisor * quotient + remainder.
    void DivideWithRemainder(const LongInteger& divisor,
                             LongInteger& quotient,
                             LongInteger& remainder) const;

    // Сравнивает числа по абсолютному значению без копирования.
    static bool LessAbsoluteValue(
        const LongInteger& lhs,
        const LongInteger& rhs) noexcept;

    // Умножает число за O(blocks.size()) на value < base.
    void MultiplyByLessThanBase(const int64_t value);

    // Умножает число за O(blocks.size() + power) на base ^ power.
    void MultiplyByPowerOfBase(const uint64_t power);

    // Удаляет нулевые старшие разряды.
    void Trim() noexcept;

private:
    // Основание использующийся системы счисления для блоков.
    // Максимальное невключительно число, которое может храниться в блоке.
    static const int64_t base = 1e9;
    // Знак числа. Может быть либо 1, либо -1.
    int32_t sign;
    // Разряды хранимого числа. Младший разряд числа записан в blocks[0].
    // Если хранимое число 0, blocks.size() равен 0.
    std::vector<int64_t> blocks;
};

std::ostream& operator<<(std::ostream& stream, const LongInteger& value);

std::istream& operator>>(std::istream& stream, LongInteger& value);

// Литералы для длинного числа.
LongInteger operator"" _li(unsigned long long int value);

LongInteger operator"" _li(const char* string, size_t size);
