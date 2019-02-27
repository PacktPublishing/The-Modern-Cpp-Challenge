#include <cmath>
#include <iostream>
#include <random>
#include <utility>
#include <vector>

void func(std::vector<std::pair<int, int> >& vec, int limit);

int sum_proper_divisors(int const number) {
  int result = 1;
  for (int i = 2; i <= std::sqrt(number); i++) {
    if (number % i == 0) {
      result += (i == (number / i)) ? i : (i + number / i);
    }
  }

  return result;
}

void store_abundant(std::vector<std::pair<int, int> >& vec, int const limit) {
  for (int number = 10; number <= limit; ++number) {
    auto sum = sum_proper_divisors(number);
    if (sum > number) {
      vec.push_back(std::make_pair(number, sum - number));
    }
  }
}

void print_pair(std::pair<int, int>& pair) {
  std::cout << "(" << pair.first << ", " << pair.second << ")\n";
}

int main() {
  std::random_device rand_dev;

  int cnt = 0;
  for (int i = 0; i < 100; i++) {
    unsigned int limit = rand_dev() % 1000;
    std::vector<std::pair<int, int> > actual, expected;
    func(actual, limit);
    store_abundant(expected, limit);

    if (expected != actual) {
      std::cout << limit << "\n";
    } else {
      cnt++;
    }
  }
  if (cnt == 100)
    std::cout << "Accepted\n";
  else
    std::cout << "Wrong Answer\n";

  return 0;
}
