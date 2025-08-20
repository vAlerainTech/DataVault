#pragma once
#include <string>
#include <vector>
#include <algorithm>
#include <functional>
#include <stdexcept>
#include <iostream>
#include <compare>
#include <cctype>

class BigInteger {
private:
    std::string digits;
    bool isNegative;

    // 移除前导零
    void removeLeadingZeros() {
        size_t nonZero = digits.find_first_not_of('0');
        if (nonZero == std::string::npos) {
            digits = "0";
            isNegative = false;
        } else {
            digits = digits.substr(nonZero);
        }
        if (digits == "0") isNegative = false;
    }

    // 比较绝对值大小
    int compareAbsolute(const BigInteger& other) const {
        if (digits.length() != other.digits.length()) {
            return digits.length() > other.digits.length() ? 1 : -1;
        }
        return digits.compare(other.digits);
    }

    // Karatsuba乘法核心算法
    static BigInteger karatsubaMultiply(const BigInteger& a, const BigInteger& b) {
        size_t n = std::max(a.digits.size(), b.digits.size());

        if (n <= 32) { // 阈值，小数字直接相乘
            return simpleMultiply(a, b);
        }

        n = (n + 1) / 2;

        BigInteger high1(a.digits.substr(0, a.digits.size() - n));
        BigInteger low1(a.digits.substr(a.digits.size() - n));
        BigInteger high2(b.digits.substr(0, b.digits.size() - n));
        BigInteger low2(b.digits.substr(b.digits.size() - n));

        BigInteger z0 = karatsubaMultiply(low1, low2);
        BigInteger z1 = karatsubaMultiply(low1 + high1, low2 + high2);
        BigInteger z2 = karatsubaMultiply(high1, high2);

        return (z2 << (2 * n)) + ((z1 - z2 - z0) << n) + z0;
    }

    // 简单乘法（用于小数字）
    static BigInteger simpleMultiply(const BigInteger& a, const BigInteger& b) {
        size_t len1 = a.digits.size();
        size_t len2 = b.digits.size();
        std::string result(len1 + len2, '0');

        for (int i = len1 - 1; i >= 0; i--) {
            int carry = 0;
            int digit1 = a.digits[i] - '0';

            for (int j = len2 - 1; j >= 0; j--) {
                int digit2 = b.digits[j] - '0';
                int sum = (result[i + j + 1] - '0') + digit1 * digit2 + carry;
                carry = sum / 10;
                result[i + j + 1] = (sum % 10) + '0';
            }

            result[i] += carry;
        }

        BigInteger res(result);
        res.removeLeadingZeros();
        return res;
    }

public:
    // 构造函数
    BigInteger() : digits("0"), isNegative(false) {}
    BigInteger(const std::string& num) {
        if (num.empty()) {
            digits = "0";
            isNegative = false;
            return;
        }

        std::string processed = num;
        if (processed[0] == '-' || processed[0] == '+') {
            isNegative = (processed[0] == '-');
            processed = processed.substr(1);
        } else {
            isNegative = false;
        }

        // 验证数字有效性
        if (processed.find_first_not_of("0123456789") != std::string::npos) {
            throw std::invalid_argument("Invalid number string");
        }

        digits = processed;
        removeLeadingZeros();
    }

    BigInteger(long long num) {
        isNegative = num < 0;
        digits = std::to_string(std::abs(num));
    }

    // 比较运算符 - 先定义这些，避免operator<=>的问题
    bool operator<(const BigInteger& other) const {
        if (isNegative != other.isNegative) {
            return isNegative;
        }

        int absCompare = compareAbsolute(other);
        return isNegative ? (absCompare > 0) : (absCompare < 0);
    }

    bool operator<=(const BigInteger& other) const {
        return !(other < *this);
    }

    bool operator>(const BigInteger& other) const {
        return other < *this;
    }

    bool operator>=(const BigInteger& other) const {
        return !(*this < other);
    }

    bool operator==(const BigInteger& other) const {
        return isNegative == other.isNegative && digits == other.digits;
    }

    bool operator!=(const BigInteger& other) const {
        return !(*this == other);
    }

    // 三路比较运算符（C++20）
    std::strong_ordering operator<=>(const BigInteger& other) const {
        if (isNegative != other.isNegative) {
            return isNegative ? std::strong_ordering::less : std::strong_ordering::greater;
        }

        int absCompare = compareAbsolute(other);
        if (absCompare < 0) {
            return isNegative ? std::strong_ordering::greater : std::strong_ordering::less;
        } else if (absCompare > 0) {
            return isNegative ? std::strong_ordering::less : std::strong_ordering::greater;
        }
        return std::strong_ordering::equal;
    }

    // 算术运算符重载
    BigInteger operator+(const BigInteger& other) const {
        if (isNegative == other.isNegative) {
            BigInteger result;
            result.isNegative = isNegative;

            std::string num1 = digits;
            std::string num2 = other.digits;
            std::string sum;

            int carry = 0;
            int i = num1.size() - 1;
            int j = num2.size() - 1;

            while (i >= 0 || j >= 0 || carry) {
                int digit1 = (i >= 0) ? num1[i--] - '0' : 0;
                int digit2 = (j >= 0) ? num2[j--] - '0' : 0;
                int total = digit1 + digit2 + carry;
                carry = total / 10;
                sum.push_back((total % 10) + '0');
            }

            std::reverse(sum.begin(), sum.end());
            result.digits = sum;
            result.removeLeadingZeros();
            return result;
        } else {
            if (isNegative) {
                return other - (-*this);
            } else {
                return *this - (-other);
            }
        }
    }

    BigInteger operator-(const BigInteger& other) const {
        if (isNegative != other.isNegative) {
            return *this + (-other);
        }

        if (compareAbsolute(other) < 0) {
            BigInteger result = other - *this;
            result.isNegative = !isNegative;
            return result;
        }

        BigInteger result;
        result.isNegative = isNegative;

        std::string num1 = digits;
        std::string num2 = other.digits;
        std::string diff;

        int borrow = 0;
        int i = num1.size() - 1;
        int j = num2.size() - 1;

        while (i >= 0 || j >= 0) {
            int digit1 = (i >= 0) ? num1[i--] - '0' : 0;
            int digit2 = (j >= 0) ? num2[j--] - '0' : 0;

            digit1 -= borrow;
            if (digit1 < digit2) {
                digit1 += 10;
                borrow = 1;
            } else {
                borrow = 0;
            }

            diff.push_back((digit1 - digit2) + '0');
        }

        std::reverse(diff.begin(), diff.end());
        result.digits = diff;
        result.removeLeadingZeros();
        return result;
    }

    BigInteger operator*(const BigInteger& other) const {
        if (*this == 0 || other == 0) return BigInteger(0);

        BigInteger result = karatsubaMultiply(*this, other);
        result.isNegative = (isNegative != other.isNegative);
        result.removeLeadingZeros();
        return result;
    }

    BigInteger operator/(const BigInteger& other) const {
        if (other == 0) {
            throw std::runtime_error("Division by zero");
        }

        if (compareAbsolute(other) < 0) {
            return BigInteger(0);
        }

        BigInteger dividend = *this;
        BigInteger divisor = other;
        dividend.isNegative = false;
        divisor.isNegative = false;

        BigInteger quotient("0");
        BigInteger current("0");

        for (char digit : dividend.digits) {
            current = current * 10 + BigInteger(std::string(1, digit));

            int count = 0;
            while (current >= divisor) {
                current = current - divisor;
                count++;
            }

            quotient = quotient * 10 + BigInteger(count);
        }

        quotient.isNegative = (isNegative != other.isNegative);
        quotient.removeLeadingZeros();
        return quotient;
    }

    BigInteger operator%(const BigInteger& other) const {
        BigInteger quotient = *this / other;
        return *this - quotient * other;
    }

    // 一元运算符
    BigInteger operator-() const {
        BigInteger result = *this;
        if (result != 0) {
            result.isNegative = !result.isNegative;
        }
        return result;
    }

    BigInteger operator+() const {
        return *this;
    }

    // 位移运算符（用于快速乘2的幂）
    BigInteger operator<<(size_t n) const {
        if (*this == 0) return *this;
        BigInteger result = *this;
        result.digits += std::string(n, '0');
        return result;
    }

    // 快捷方法
    BigInteger& operator+=(const BigInteger& other) { *this = *this + other; return *this; }
    BigInteger& operator-=(const BigInteger& other) { *this = *this - other; return *this; }
    BigInteger& operator*=(const BigInteger& other) { *this = *this * other; return *this; }
    BigInteger& operator/=(const BigInteger& other) { *this = *this / other; return *this; }
    BigInteger& operator%=(const BigInteger& other) { *this = *this % other; return *this; }

    // 快速幂算法
    BigInteger pow(const BigInteger& exponent) const {
        if (exponent == 0) return BigInteger(1);
        if (exponent == 1) return *this;
        if (exponent < 0) {
            throw std::invalid_argument("Negative exponent not supported");
        }

        BigInteger result(1);
        BigInteger base = *this;
        BigInteger exp = exponent;

        while (exp > 0) {
            if (exp % 2 == 1) {
                result = result * base;
            }
            base = base * base;
            exp = exp / 2;
        }

        return result;
    }

    // 转换函数
    std::string toString() const {
        return (isNegative ? "-" : "") + digits;
    }

    explicit operator long long() const {
        return std::stoll(toString());
    }

    // 友元函数
    friend std::ostream& operator<<(std::ostream& os, const BigInteger& num);
    friend std::istream& operator>>(std::istream& is, BigInteger& num);
};

// 流操作符
inline std::ostream& operator<<(std::ostream& os, const BigInteger& num) {
    os << num.toString();
    return os;
}

inline std::istream& operator>>(std::istream& is, BigInteger& num) {
    std::string input;
    is >> input;
    num = BigInteger(input);
    return is;
}