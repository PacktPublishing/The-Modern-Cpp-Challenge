#include <iostream>
#include <random>

bool is_prime(int a);

bool is_prime_ans(int const num) {
  if (num <= 3) {
    return num > 1;
  } else if (num % 2 == 0 || num % 3 == 0) {
    return false;
  } else {
    for (int i = 5; i * i <= num; i += 6) {
      if (num % i == 0 || num % (i + 2) == 0) {
	return false;
      }
    }

    return true;
  }
}

int main() {
  std::random_device rand_dev;

  int cnt = 0;
  for (int i = 0; i < 100; i++) {
    unsigned int limit = rand_dev() % 1000;
    int actual, expected;

    for (int j = limit; j > 1; j--) {
      if (is_prime_ans(j)) {
	expected = is_prime_ans(j);
	break;
      }
    }
    for (int j = limit; j > 1; j--) {
      if (is_prime(j)) {
	actual = is_prime_ans(j);
	break;
      }
    }

    if (expected != actual) {
      std::cout << limit << "\texpect:" << expected << "\tactual:" << actual
		<< "\n";
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
