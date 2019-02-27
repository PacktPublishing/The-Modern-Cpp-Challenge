#include <cmath>
#include <iostream>
#include <random>
#include <set>
#include <utility>

void func(std::set<std::pair<int, int> >& vec);

int sum_proper_divisors(int const number) {
  int result = 1;
  for (int i = 2; i <= std::sqrt(number); i++) {
    if (number % i == 0) {
      result += (i == (number / i)) ? i : (i + number / i);
    }
  }
  return result;
}

void print_amicables(int const limit) {
  for (int number = 4; number < limit; ++number) {
    auto sum1 = sum_proper_divisors(number);

    if (sum1 < limit) {
      auto sum2 = sum_proper_divisors(sum1);

      if (sum2 == number && number != sum1) {
	std::cout << number << "," << sum1 << std::endl;
      }
    }
  }
}

void store_amicables_once(std::set<std::pair<int, int> >& set) {
  std::set<int> printed;
  int limit = 1000000;
  for (int number = 4; number < limit; ++number) {
    if (printed.find(number) != printed.end()) continue;

    auto sum1 = sum_proper_divisors(number);

    if (sum1 < limit) {
      auto sum2 = sum_proper_divisors(sum1);

      if (sum2 == number && number != sum1) {
	printed.insert(number);
	printed.insert(sum1);
	set.insert(std::make_pair(number, sum1));
      }
    }
  }
}

int main() {
  std::random_device rand_dev;

  int cnt = 0;
  std::set<std::pair<int, int> > actual, expected;
  func(actual);
  store_amicables_once(expected);

  if (actual == expected)
    std::cout << "Accepted\n";
  else
    std::cout << "Wrong Answer\n";

  return 0;
}
