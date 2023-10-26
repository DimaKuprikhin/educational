// #include <tp/thread_pool.hpp>

#include <iostream>
#include <thread>

int main() {
  std::thread t([](){
    std::cout << "helllo\n";
  });
  // t.join();
  int n;
  std::cin >> n;
  std::cout << n + 1 << std::endl;
  t.join();

  return 0;
}
