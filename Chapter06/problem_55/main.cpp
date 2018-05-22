#include <iostream>
#include <vector>

template <typename Input1, typename Input2, typename Output>
void zip(
   Input1 begin1, Input1 end1, 
   Input2 begin2, Input2 end2,
   Output result)
{
   auto it1 = begin1;
   auto it2 = begin2;
   while (it1 != end1 && it2 != end2)
   {
      result++ = std::make_pair(*it1++, *it2++);
   }
}

template <typename T, typename U>
std::vector<std::pair<T, U>> zip(
   std::vector<T> const & range1, 
   std::vector<U> const & range2)
{
   std::vector<std::pair<T, U>> result;

   zip(
      std::begin(range1), std::end(range1),
      std::begin(range2), std::end(range2),
      std::back_inserter(result));

   return result;
}

int main()
{
   std::vector<int> v1{ 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
   std::vector<int> v2{ 1, 1, 3, 5, 8, 13, 21 };

   auto result = zip(v1, v2);

   for (auto const & p : result)
   {
      std::cout << '{' << p.first << ',' << p.second << '}' << std::endl;
   }
}
