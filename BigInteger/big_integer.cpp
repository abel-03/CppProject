#include "big_integer.hpp"

const int kMaxDigitsInElement = 9;
const int kIncreaseLen = 10;

BigInt::BigInt() = default;

BigInt::BigInt(std::string number) {
  if (number.length() == 0 || isdigit(number[0]) != 0) {
    IsNegative_ = false;
  } else {
    IsNegative_ = true;
    start_ = 1;
  }
  for (int i = number.length() - 1; i >= start_; i -= kMaxDigitsInElement) {
    int res = 0;
    if (i < kMaxDigitsInElement) {
      int k = i;
      for (int j = start_; j < i + 1; j++) {
        int sum = number[j] - '0';
        res += sum;
        if (j != i) {
          res *= kIncreaseLen;
        }
      }
    } else {
      for (int j = kMaxDigitsInElement - 1; j >= 0; j--) {
        int sum = number[i - j] - '0';
        res += sum;
        if (j != 0) {
          res *= kIncreaseLen;
        }
      }
    }
    num_.push_back(res);
  }
}

BigInt::BigInt(int64_t number) {
  if (number < 0) {
    IsNegative_ = true;
    number *= -1;
  }
  do {
    num_.push_back(number % kMod);
    number /= kMod;
  } while (number != 0);
}

BigInt::BigInt(const BigInt& number) {
  IsNegative_ = number.IsNegative_;
  for (size_t i = 0; i < number.num_.size(); i++) {
    num_.push_back(number.num_[i]);
  }
}

BigInt BigInt::operator+(const BigInt& number2) {
  BigInt res;
  int remain = 0;
  int sum = 0;
  size_t max =
      num_.size() > number2.num_.size() ? num_.size() : number2.num_.size();
  if (IsNegative_ == number2.IsNegative_) {
    res.IsNegative_ = IsNegative_;
    for (size_t j = 0; j < max; j++) {
      sum = (num_[j] + number2.num_[j] + remain) % kMod;
      remain = sum / kMod;
      res.num_.push_back(sum);
    }
  } else {
    if (*this > number2) {
      SumNegativeNumber(*this, number2, res);
    } else {
      SumNegativeNumber(number2, *this, res);
    }
    res.num_.push_back(sum);
  }
  return res;
}

int SumNegativeNumber(const BigInt& number1, const BigInt& number2,
                      BigInt& res) {
  int sum = 0;
  int remain = 0;
  size_t max = number1.num_.size() > number2.num_.size() ? number1.num_.size()
                                                         : number2.num_.size();
  BigInt de_number2 = number2;
  de_number2.IsNegative_ = !number2.IsNegative_;
  res.IsNegative_ =
      number1 > de_number2 ? number1.IsNegative_ : number2.IsNegative_;
  for (size_t j = 0; j < max; j++) {
    sum = (number1.num_[j] - number2.num_[j] - remain) % number1.kMod;
    remain = sum / number1.kMod;
  }
  return sum;
}
BigInt& BigInt::operator+=(const BigInt& number2) {
  *this = (*this + number2);
  return *this;
}

BigInt BigInt::operator-(const BigInt& number2) {
  BigInt buffer = *this;
  buffer.IsNegative_ = !IsNegative_;
  return buffer + number2;
}

BigInt& BigInt::operator-=(const BigInt& number2) {
  *this = *this - number2;
  return *this;
}

BigInt BigInt::operator*(const BigInt& number2) {
  BigInt res;
  res.IsNegative_ = IsNegative_ ^ number2.IsNegative_;
  res.num_.resize(num_.size(), number2.num_.size());
  for (size_t i = 0; i < res.num_.size(); ++i) {
    int elem = 0;
    size_t index = res.num_.size() - i - 1;
    for (size_t j = 0; j <= index; ++j) {
      size_t dif = index - j;
      if (j < num_.size() && dif < number2.num_.size()) {
        elem += num_[j] * number2.num_[dif];
      }
    }
    res.num_[index] = elem;
  }
  int rem = 0;
  for (auto& digital : num_) {
    digital += rem;
    rem = digital / kMod;
    digital %= kMod;
  }
  while (rem > 0) {
    num_.push_back(rem % kMod);
    rem /= kMod;
  }
  res.delete_front_zero();
  return res;
}

BigInt& BigInt::operator*=(const BigInt& number2) {
  *this = *this * number2;
  return *this;
}

BigInt BigInt::operator/(const BigInt& number2) {
  if (number2.num_.size() == 1 && number2.num_[0] == 0) {
    throw("Error: Cannot be divided by 0");
  }
  BigInt res;
  res.IsNegative_ = IsNegative_ ^ number2.IsNegative_;
  BigInt positive_number1 = *this;
  BigInt positive_number2 = number2;
  positive_number1.IsNegative_ = false;
  positive_number2.IsNegative_ = false;
  if (positive_number2 > positive_number1) {
    res = 0;
    res.IsNegative_ = false;
    return res;
  }
  if (*this == number2) {
    res = IsNegative_ == number2.IsNegative_ ? 1 : -1;
    return res;
  }
  division_continuation(res, positive_number2, number2);
  res.delete_front_zero();
  return res;
}

void BigInt::division_continuation(BigInt res, BigInt& positive_number2,
                                   const BigInt& number2) {
  BigInt del = 0;
  for (int iter = num_.size() - 1; iter >= 0; --iter) {
    del = del * kIncreaseLen + BigInt(num_[iter]);
    int i = 0;
    while (del > (positive_number2 * i)) {
      i++;
    }
    res = res * kIncreaseLen + i;
    del -= (positive_number2 * i);
  }
}

BigInt& BigInt::operator/=(const BigInt& number2) {
  *this = *this / number2;
  return *this;
}

BigInt BigInt::operator%(const BigInt& number2) {
  BigInt res = *this - *this / number2 * number2;
  return res;
}

BigInt& BigInt::operator%=(const BigInt& number2) {
  *this = *this % number2;
  return *this;
}

bool operator>(const BigInt& number1, const BigInt& number2) {
  if (number1.IsNegative_ != number2.IsNegative_) {
    return !number1.IsNegative_;
  }
  if (number1.num_.size() > number2.num_.size()) {
    return !number1.IsNegative_;
  }
  if (number1.num_.size() < number2.num_.size()) {
    return !number1.IsNegative_;
  }
  for (size_t i = 0; i < number1.num_.size(); i++) {
    if (number1.num_[number1.num_.size() - 1 - i] >
        number2.num_[number2.num_.size() - 1 - i]) {
      return !number1.IsNegative_;
    }
    if (number1.num_[number1.num_.size() - 1 - i] <
        number2.num_[number2.num_.size() - 1 - i]) {
      return number1.IsNegative_;
    }
  }
  return false;
}

bool BigInt::operator==(const BigInt& number2) {
  if (IsNegative_ == number2.IsNegative_ &&
      num_.size() == number2.num_.size()) {
    for (size_t i = 0; i < num_.size(); i++) {
      if (num_[num_.size() - 1 - i] !=
          number2.num_[number2.num_.size() - 1 - i]) {
        return false;
      }
    }
    return true;
  }
  return false;
}

bool BigInt::operator!=(const BigInt& number2) { return !(*this == number2); }

bool BigInt::operator>=(const BigInt& number2) {
  return *this == number2 || *this > number2;
}

bool BigInt::operator<(const BigInt& number2) { return !(*this >= number2); }

bool BigInt::operator<=(const BigInt& number2) const {
  return !(*this > number2);
}

BigInt& BigInt::operator++() {
  *this += 1;
  return *this;
}

BigInt BigInt::operator++(int) {
  BigInt buffer = *this;
  *this += 1;
  return buffer;
}

BigInt& BigInt::operator--() {
  *this -= 1;
  return *this;
}

BigInt BigInt::operator--(int) {
  BigInt buffer = *this;
  *this -= 1;
  return buffer;
}

BigInt BigInt::operator-() {
  BigInt res = *this;
  res.IsNegative_ = !IsNegative_;
  return res;
}

std::ostream& operator<<(std::ostream& out, const BigInt& number) {
  std::string s = number.ToString();
  out << s;
  return out;
}

std::string BigInt::ToString() const {
  if (num_.empty()) {
    return "0";
  }
  std::string res;
  size_t log_base = 9;
  for (size_t i = 0; i < num_.size(); ++i) {
    int digital = num_[i];
    std::string strDigital = std::to_string(digital);
    std::reverse(strDigital.begin(), strDigital.end());
    while (i != num_.size() - 1 && strDigital.size() != log_base) {
      strDigital += "0";
    }
    res += strDigital;
  }
  if (IsNegative_) {
    res += '-';
    std::reverse(res.begin(), res.end());
  }
  return res;
}

std::istream& operator>>(std::istream& in, BigInt& number) {
  std::string str;
  in >> str;
  number = str;
  return in;
}

BigInt& BigInt::operator=(const BigInt& number2) {
  IsNegative_ = number2.IsNegative_;
  for (size_t i = 0; i < number2.num_.size(); i++) {
    num_.push_back(number2.num_[i]);
  }
  return *this;
}

void BigInt::delete_front_zero() {
  while (num_.size() > 1 && num_.back() == 0) {
    num_.pop_back();
    if (num_.size() == 1 && num_[0] == 0) {
      IsNegative_ = false;
    }
  }
  if (num_.empty()) {
    IsNegative_ = false;
  }
}