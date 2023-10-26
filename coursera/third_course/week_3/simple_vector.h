#pragma once

#include <cstdlib>

template <typename T>
class SimpleVector {
public:
  SimpleVector()
    : data(nullptr)
    , size(0)
    , capacity(0)
    {}

  explicit SimpleVector(size_t size)
    : data(new T[size])
    , size(size)
    , capacity(size)
    {}

  ~SimpleVector() {
    delete[] data;
  }

  T& operator[](size_t index) { return data[index]; }
  const T& operator[](size_t index) const { return data[index]; }

  T* begin() { return data; }
  T* end() { return data + size; }
  const T* begin() const { return data; }
  const T* end() const { return data + size; } 

  size_t Size() const { return size; }

  size_t Capacity() const { return capacity; }

  void PushBack(const T& value) {
    if(capacity == 0) {
      data = new T[1];
      capacity = 1;
    }
    if(size == capacity) {
      capacity *= 2;
      T* newData = new T[capacity];
      for(int i = 0; i < size; ++i) {
        newData[i] = data[i];
      }
      delete[] data;
      data = newData;
    }
    data[size++] = value;
  }

private:
  T* data;
  size_t size;
  size_t capacity;
  
};
