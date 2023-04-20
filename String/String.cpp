#include "String.hpp"

String::String() {
  size_ = 0;
  capacity_ = 0;
  str_ = nullptr;
}

String::String(size_t size, char character) : size_(size) {
  capacity_ = 0;
  str_ = new char[size_ + 1];
  for (size_t i = 0; i < size_; i++) {
    str_[i] = character;
    if (size_ > capacity_) {
      capacity_ == 0 ? capacity_ += 1 : capacity_ *= 2;
    }
  }
  str_[size_] = '\0';
}

String::String(const char* str) {
  capacity_ = 0;
  size_ = strlen(str);
  if (size_ != 0) {
    capacity_ = 1;
    while (capacity_ <= size_) {
      this->capacity_ *= 2;
    }
  }
  str_ = new char[capacity_ + 1];
  std::memcpy(str_, str, size_);
  str_[size_] = '\0';
}

String::String(const String& str2) {
  size_ = str2.size_;
  capacity_ = str2.capacity_;
  str_ = new char[capacity_ + 1];
  std::memcpy(str_, str2.str_, size_);
  str_[size_] = '\0';
}

String& String::operator=(const String& str2) {
  if (&str2 == this) {
    return *this;
  }
  delete[] str_;
  size_ = str2.size_;
  capacity_ = str2.capacity_;
  str_ = new char[capacity_ + 1];
  std::memcpy(str_, str2.str_, size_);
  str_[size_] = '\0';
  return *this;
}

String::~String() { delete[] str_; }

void String::Clear() { size_ = 0; }

void String::PushBack(char character) {
  (size_ == 0 && capacity_ == 0) ? capacity_ += 1 : capacity_ *= 2;
  char* new_str = new char[capacity_ + 1];
  std::memcpy(new_str, str_, size_);
  new_str[size_] = character;
  size_++;
  delete[] str_;
  str_ = new char[capacity_ + 1];
  std::memcpy(str_, new_str, size_);
  str_[size_] = '\0';
  delete[] new_str;
}

void String::PopBack() {
  if (size_ == 0) {
    return;
  }
  size_--;
  str_[size_] = '\0';
}

void String::Resize(size_t new_size) {
  if (new_size <= capacity_) {
    size_ = new_size;
    return;
  }
  char* new_str = new char[new_size + 1];
  std::memcpy(new_str, str_, size_);
  new_str[new_size] = '\0';
  for (size_t i = size_; i < new_size; i++) {
    if (new_size > capacity_) {
      capacity_ == 0 ? capacity_ += 1 : capacity_ *= 2;
    }
  }
  delete[] str_;
  str_ = new char[capacity_ + 1];
  std::memcpy(str_, new_str, size_);
  str_[size_] = '\0';
  size_ = new_size;
  delete[] new_str;
}

void String::Resize(size_t new_size, char character) {
  size_t size_before = size_;
  Resize(new_size);
  if (new_size <= size_before) {
    return;
  }
  for (size_t i = size_before; i < new_size; i++) {
    str_[i] = character;
  }
  str_[new_size] = '\0';
}

void String::Reserve(size_t new_cap) {
  capacity_ = new_cap > capacity_ ? new_cap : capacity_;
}

void String::ShrinkToFit() {
  capacity_ = capacity_ > size_ ? size_ : capacity_;
}

void String::Swap(String& other) {
  std::swap(capacity_, other.capacity_);
  std::swap(size_, other.size_);
  std::swap(str_, other.str_);
}

const char& String::operator[](int index) const { return str_[index]; }

char& String::operator[](int index) { return str_[index]; }

const char& String::Front() const { return str_[0]; }

char& String::Front() { return str_[0]; }

const char& String::Back() const { return str_[size_ - 1]; }

char& String::Back() { return str_[size_ - 1]; }

bool String::Empty() const { return size_ == 0; }

size_t String::Size() const { return size_; }

size_t String::Capacity() const { return capacity_; }

char* String::Data() { return str_; }

const char* String::Data() const { return str_; }

bool operator>(const String& str1, const String& str2) {
  if (str1.ReturnSize() > str2.ReturnSize()) {
    return true;
  }
  if (str1.ReturnSize() < str2.ReturnSize()) {
    return false;
  }
  for (size_t i = 0; i < str1.ReturnSize(); i++) {
    if (str1.ReturnStr()[i] > str2.ReturnStr()[i]) {
      return true;
    }
    if (str1.ReturnStr()[i] < str2.ReturnStr()[i]) {
      return false;
    }
  }
  return false;
}

bool operator<(const String& str1, const String& str2) {
  return (!(str1 > str2) && str2 != str1);
}

bool operator<=(const String& str1, const String& str2) {
  return !(str1 > str2);
}

bool operator>=(const String& str1, const String& str2) {
  return !(str1 < str2);
}

bool operator==(const String& str1, const String& str2) {
  if (str1.ReturnSize() != str2.ReturnSize()) {
    return false;
  }
  for (size_t i = 0; i < str1.ReturnSize(); i++) {
    if (str1.ReturnStr()[i] != str2.ReturnStr()[i]) {
      return false;
    }
  }
  return true;
}

bool operator!=(const String& str1, const String& str2) {
  return !(str1 == str2);
}

void String::NewSizeAndCap(const String& str1, const String& str2) {
  this->size_ = str2.size_ + str1.size_;
  this->capacity_ = str2.capacity_ + str1.capacity_;
  this->str_ = new char[this->capacity_ + 1];
}

String operator+(const String& str1, const String& str2) {
  String res;
  res.NewSizeAndCap(str1, str2);
  std::memcpy(res.ReturnStr(), str1.ReturnStr(), str1.ReturnSize());
  std::memcpy(res.ReturnStr() + str1.ReturnSize(), str2.ReturnStr(),
              str2.ReturnSize());
  res.ReturnStr()[res.ReturnSize()] = '\0';
  return res;
}

String& String::operator+=(const String& str2) {
  *this = *this + str2;
  return *this;
}

String& String::operator+=(char str) {
  PushBack(str);
  return *this;
}

String operator*(const String& str, size_t num) {
  String res;
  size_t new_num = num;
  if (num < 2) {
    if (num == 0) {
      return res;
    }
    return str;
  }
  num /= 2;
  res = str * num;
  res += res;
  if (new_num % 2 != 0) {
    return res += str;
  }
  return res;
}

String& String::operator*=(size_t n) {
  *this = *this * n;
  return *this;
}

std::ostream& operator<<(std::ostream& ostream, const String& str2) {
  ostream << str2.ReturnStr();
  return ostream;
}

std::istream& operator>>(std::istream& istream, String& str2) {
  char str;
  while (istream.get(str)) {
    str2.PushBack(str);
  }
  return istream;
}
bool SubIsDel(size_t iter, const String& str, const String& delim) {
  size_t word_size = delim.Size() + 1;
  char* word = new char[word_size];
  if (delim.Size() <= str.Size() - iter + 1) {
    std::memcpy(word, str.Data() + iter, delim.Size());
  }
  word[delim.Size()] = '\0';
  bool res = (delim == word);
  delete[] word;
  return res;
}

std::vector<String> String::Split(const String& delim) {
  std::vector<String> result;
  size_t begin = 0;
  size_t iter = 0;
  while (iter < size_) {
    if (SubIsDel(iter, *this, delim)) {
      char* with_word = new char[iter - begin + 1];
      std::memcpy(with_word, str_ + begin, iter - begin);
      with_word[iter - begin] = '\0';
      result.push_back(with_word);
      begin = iter;
      begin += delim.size_;
      delete[] with_word;
    }
    iter++;
  }
  char* with_word = new char[iter - begin + 1];
  std::memcpy(with_word, str_ + begin, iter - begin);
  with_word[iter - begin] = '\0';
  result.push_back(String(with_word));
  delete[] with_word;
  return result;
}

String String::Join(const std::vector<String>& strings) const {
  String res;
  if (!strings.empty()) {
    for (size_t i = 0; i < strings.size(); i++) {
      res += strings[i];
      if (i < (strings.size() - 1)) {
        res = res + (*this);
      }
    }
  }
  return res;
}

char* String::ReturnStr() const { return this->str_; }

size_t String::ReturnSize() const { return this->size_; }

String String::operator*(const char* str, int n) { 
  return String(str) * n;
}

int main() {
  String a = "kek" * 5;
  std::cout << a;
}