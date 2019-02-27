#include <iostream>
#include <random>

int func(int a);

int main() {
  std::random_device rand_dev;

  int cnt = 0;
  for (int i = 0; i < 100; i++) {
    unsigned int limit = rand_dev() % 1000;

    unsigned long long sum = 0;
    for (unsigned int i = 3; i < limit; ++i) {
      if (i % 3 == 0 || i % 5 == 0) sum += i;
    }
    if (func(limit) != sum) {
      std::cout << limit << "\texpect:" << sum << "\tactual:" << func(limit)
		<< "\n";
    } else {
      cnt++;
    }
  }
  if (cnt == 100)
    std::cout << "Accepted\n";
  else
    std::cout << "Wrong Answer\n";
}
