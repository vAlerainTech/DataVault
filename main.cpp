#include <iostream>
#include "BigInteger.h"
#include <chrono>
#include <random>

void performanceTest() {
    std::cout << "=== ���ܲ��� ===" << std::endl;

    // ���ɴ������
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, 9);

    std::string bigNum1, bigNum2;
    for (int i = 0; i < 1000; i++) {
        bigNum1 += std::to_string(dis(gen));
        bigNum2 += std::to_string(dis(gen));
    }

    BigInteger a(bigNum1);
    BigInteger b(bigNum2);

    auto start = std::chrono::high_resolution_clock::now();
    BigInteger c = a * b; // ����Karatsuba�˷�
    auto end = std::chrono::high_resolution_clock::now();

    std::chrono::duration<double> duration = end - start;
    std::cout << "1000λ�˷���ʱ: " << duration.count() << "��" << std::endl;
}

// ������ʾ
void demonstration() {
    std::cout << "\n=== ������ʾ ===" << std::endl;

    BigInteger a("123456789012345678901234567890");
    BigInteger b("987654321098765432109876543210");

    std::cout << "a = " << a << std::endl;
    std::cout << "b = " << b << std::endl;
    std::cout << "a + b = " << a + b << std::endl;
    std::cout << "a - b = " << a - b << std::endl;
    std::cout << "a * b = " << a * b << std::endl;
    std::cout << "a / 12345 = " << a / BigInteger(12345) << std::endl;

    // ��������ʾ
    BigInteger base("2");
    BigInteger exponent("100");
    std::cout << "2^100 = " << base.pow(exponent) << std::endl;
}

int main() {
    try {
        demonstration();
        performanceTest();

        // �������
        BigInteger x("18446744073709551616"); // 2^64
        BigInteger y("340282366920938463463374607431768211456"); // 2^128
        std::cout << "\n����������:" << std::endl;
        std::cout << "x * y = " << x * y << std::endl;

    } catch (const std::exception& e) {
        std::cerr << "����: " << e.what() << std::endl;
    }

    return 0;
}