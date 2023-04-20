#pragma once
#include <ctype.h>

#include <algorithm>
#include <iostream>
#include <string>
#include <vector>

class BigInt {
 public:
  BigInt();
  BigInt(std::string number);
  BigInt(int64_t number);
  BigInt(const BigInt& number);

  BigInt operator+(const BigInt& number2);
  BigInt& operator+=(const BigInt& number2);
  BigInt operator-(const BigInt& number2);
  BigInt& operator-=(const BigInt& number2);
  BigInt operator*(const BigInt& number2);
  BigInt& operator*=(const BigInt& number2);
  BigInt operator/(const BigInt& number2);
  BigInt& operator/=(const BigInt& number2);
  BigInt operator%(const BigInt& number2);
  BigInt& operator%=(const BigInt& number2);

  friend bool operator>(const BigInt& number1, const BigInt& number2);
  bool operator<(const BigInt& number2);
  bool operator>=(const BigInt& number2);
  bool operator<=(const BigInt& number2) const;
  bool operator==(const BigInt& number2);
  bool operator!=(const BigInt& number2);

  BigInt& operator--();
  BigInt operator--(int);
  BigInt& operator++();
  BigInt operator++(int);
  BigInt& operator=(const BigInt& number2);
  BigInt operator-();

  friend std::ostream& operator<<(std::ostream& os, const BigInt& number);
  friend std::istream& operator>>(std::istream& in, const BigInt& number);

  void division_continuation(BigInt res, BigInt& positive_number2,
                             const BigInt& number2);
  friend int SumNegativeNumber(const BigInt& number1, const BigInt& number2,
                               BigInt& res);
  void delete_front_zero();

 private:
  std::vector<int> num_;
  bool IsNegative_ = false;
  int start_ = 0;
  int const kMod = 1000000000;

  std::string ToString() const;
};