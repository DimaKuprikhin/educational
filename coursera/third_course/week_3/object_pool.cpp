#include "test_runner.h"

#include <algorithm>
#include <iostream>
#include <string>
#include <queue>
#include <stdexcept>
#include <unordered_set>
using namespace std;

int destructedCount = 0;

template <class T>
class ObjectPool {
public:
  ObjectPool() { destructedCount = 0; }

  T* Allocate() {
    T* result = TryAllocate();
    if(result == nullptr) {
      result = new T;
      allocated.insert(result);
    }
    return result;
  }

  T* TryAllocate() {
    if(availableToAllocate.empty()) {
      return nullptr;
    }
    T* result = availableToAllocate.front();
    availableToAllocate.pop();
    allocated.insert(result);
    return result;
  }

  void Deallocate(T* object) {
    if(allocated.find(object) == allocated.end()) {
      throw std::invalid_argument("Tried to deallocate a pointer wasn't allocated by ObjectPool.");
    }
    availableToAllocate.push(object);
    allocated.erase(object);
  }

  ~ObjectPool() {
    while(!availableToAllocate.empty()) {
      delete availableToAllocate.front();
      availableToAllocate.pop();
      ++destructedCount;
    }
    for(T* ptr : allocated) {
      delete ptr;
      ++destructedCount;
    }
  }

private:
  std::queue<T*> availableToAllocate;
  std::unordered_set<T*> allocated;

};

void TestObjectPool() {
  ObjectPool<string> pool;

  auto p1 = pool.Allocate();
  auto p2 = pool.Allocate();
  auto p3 = pool.Allocate();

  *p1 = "first";
  *p2 = "second";
  *p3 = "third";

  pool.Deallocate(p2);
  ASSERT_EQUAL(*pool.Allocate(), "second");

  pool.Deallocate(p3);
  pool.Deallocate(p1);
  ASSERT_EQUAL(*pool.Allocate(), "third");
  ASSERT_EQUAL(*pool.Allocate(), "first");

  pool.Deallocate(p1);
}

void MyTest() {
    {
        ObjectPool<int> pool;
        std::vector<int*> pointers(500, nullptr);
        for(int i = 0; i < 500; ++i) {
            pointers[i] = pool.Allocate();
        }
        for(int i = 0; i < 300; ++i) {
            pool.Deallocate(pointers[i]);
        }
    }
    std::cout << destructedCount << "\n";
}

int main() {
  TestRunner tr;
  RUN_TEST(tr, TestObjectPool);
  RUN_TEST(tr, MyTest);
  return 0;
}