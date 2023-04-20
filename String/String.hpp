#pragma once
#include <cstdint>
#include <cstring>
#include <iostream>
#include <vector>

class String {
 public:
  String();
  String(size_t size, char character);
  String(const String&);
  String(const char* str);
  String& operator=(const String& str2);
  ~String();

  void Clear();
  void PushBack(char character);
  void PopBack();
  void Resize(size_t new_size);
  void Resize(size_t new_size, char character);
  void Reserve(size_t new_cap);
  void ShrinkToFit();
  void Swap(String& other);
  const char& operator[](int index) const;
  char& operator[](int index);
  const char& Front() const;
  char& Front();
  const char& Back() const;
  char& Back();
  bool Empty() const;
  size_t Size() const;
  size_t Capacity() const;
  char* Data();
  const char* Data() const;

  String& operator+=(const String& str2);
  String& operator+=(char str);
  String& operator*=(size_t n);
  String operator*(const char* str, int n);
  void NewSizeAndCap(const String& str1, const String& str2);
  char* ReturnStr() const;
  size_t ReturnSize() const;

  std::vector<String> Split(const String& delim = " ");
  String Join(const std::vector<String>& strings) const;
  std::vector<String> Split2(std::vector<String>& res, const String& delim);

 private:
  char* str_;
  size_t size_;
  size_t capacity_;
};

bool operator>(const String& str1, const String& str2);
bool operator<(const String& str1, const String& str2);
bool operator<=(const String& str1, const String& str2);
bool operator>=(const String& str1, const String& str2);
bool operator==(const String& str1, const String& str2);
bool operator!=(const String& str1, const String& str2);
String operator+(const String& str1, const String& str2);
String operator*(const String& str, size_t n);
std::ostream& operator<<(std::ostream& ostream, const String& str2);
std::istream& operator>>(std::istream& istream, String& str2);