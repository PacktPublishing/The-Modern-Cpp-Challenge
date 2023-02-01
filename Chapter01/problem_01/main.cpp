#include <iostream>
using namespace std;

int main()
{
   unsigned int limit;
   cout << "Upper limit: ";
   cin >> limit;

   unsigned long long sum = 0;
   for (unsigned int i = 3; i < limit; ++i)
   {
      if (i % 3 == 0 || i % 5 == 0)
         sum += i;
   }

   cout << "sum=" << sum << endl;
   return 0;
}
