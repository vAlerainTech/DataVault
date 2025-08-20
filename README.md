# BigInteger 超高精度计算库

## 概述
### BigInteger 是一个基于 C++20 的超高精度整数计算库，使用字符串存储数字以实现任意精度运算。

# 主要特性
## ✅ 任意精度：支持任意大小的整数运算

## ⚡ 高性能：采用 Karatsuba 快速乘法算法

## 📦 功能完整：支持所有基本算术和比较操作

## 🛡️ 异常安全：完善的错误处理机制

## 🎯 现代接口：完全兼容 C++20 标准

# ⚖️实战样例

    #include "BigInteger.h"
    #include <iostream>

    int main() {
    // 创建大整数对象
    BigInteger a("12345678901234567890");
    BigInteger b("98765432109876543210");

    // 执行算术运算
    std::cout << "加法结果: " << (a + b) << std::endl;
    std::cout << "乘法结果: " << (a * b) << std::endl;
    
    return 0;
    }

# 🐦‍🔥复合赋值运算符

    BigInteger a("100");
    BigInteger b("50");

    a += b;  // 等价于 a = a + b
    a -= b;  // 等价于 a = a - b
    a *= b;  // 等价于 a = a * b
    a /= b;  // 等价于 a = a / b
    a %= b;  // 等价于 a = a % b

# ❌错误处理&异常类型
## 异常类型	触发条件	处理建议
    std::invalid_argument	无效的数字字符串	检查输入格式
    std::runtime_error	除零错误	检查除数是否为零
    std::exception	类型转换溢出	检查转换范围

# 📋注意事项
### ❌ 不支持浮点数运算：仅支持整数运算

### ❌ 不支持复数运算：仅支持实数运算

### ❌ 不支持小数运算：仅支持整数运算


### ⚠️ 超大数性能：超过 10^6 位的数字运算可能较慢
