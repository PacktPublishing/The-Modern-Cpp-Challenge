#include <iostream>
#include <vector>
#include <array>
#include <functional>
#include <chrono>
#include <assert.h>
#include <random>
#include <future>

template <class RandomIt>
RandomIt partition(RandomIt first, RandomIt last)
{
   auto pivot = *first;
   auto i = first + 1;
   auto j = last - 1;
   while (i <= j)
   {
      while (i <= j && *i <= pivot) i++;
      while (i <= j && *j > pivot) j--;
      if (i < j) std::iter_swap(i, j);
   }
   
   std::iter_swap(i - 1, first);
   
   return i - 1;
}

template <class RandomIt, class Compare>
RandomIt partitionc(RandomIt first, RandomIt last, Compare comp)
{
   auto pivot = *first;
   auto i = first + 1;
   auto j = last - 1;
   while (i <= j)
   {
      while (i <= j && comp(*i, pivot)) i++;
      while (i <= j && !comp(*j, pivot)) j--;
      if (i < j) std::iter_swap(i, j);
   }
   
   std::iter_swap(i - 1, first);
   
   return i - 1;
}

template <class RandomIt>
void quicksort(RandomIt first, RandomIt last)
{
   if (first < last)
   {
      auto p = partition(first, last);
      quicksort(first, p);
      quicksort(p + 1, last);
   }
}

template <class RandomIt, class Compare>
void quicksort(RandomIt first, RandomIt last, Compare comp)
{
   if (first < last)
   {
      auto p = partitionc(first, last, comp);
      quicksort(first, p, comp);
      quicksort(p + 1, last, comp);
   }
}

template <class RandomIt>
void pquicksort(RandomIt first, RandomIt last)
{
   if (first < last)
   {
      auto p = partition(first, last);
      
      if(last - first <= 100000)
      {
         pquicksort(first, p);
         pquicksort(p + 1, last);
      }
      else
      {
         auto f1 = std::async(std::launch::async,
                              [first, p](){ pquicksort(first, p);});
         auto f2 = std::async(std::launch::async,
                              [last, p]() { pquicksort(p+1, last);});
         f1.wait();
         f2.wait();
      }
   }
}

int main()
{
   const size_t count = 1000000;
   std::vector<int> data(count);
   
   std::random_device rd;
   std::mt19937 mt;
   auto seed_data = std::array<int, std::mt19937::state_size> {};
   std::generate(std::begin(seed_data), std::end(seed_data), std::ref(rd));
   std::seed_seq seq(std::begin(seed_data), std::end(seed_data));
   mt.seed(seq);
   std::uniform_int_distribution<> ud(1, 1000);
   
   std::cout << "generating..." << std::endl;
   std::generate_n(std::begin(data), count, [&mt, &ud]() {return ud(mt); });

   auto d1 = data;
   auto d2 = data;
   
   std::cout << "sorting..." << std::endl;
   auto start = std::chrono::system_clock::now();
   quicksort(std::begin(d1), std::end(d1));
   auto end = std::chrono::system_clock::now();
   auto t1 = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
   std::cout << "time: " << t1.count() << "ms" << std::endl;
   
   start = std::chrono::system_clock::now();
   pquicksort(std::begin(d2), std::end(d2));
   end = std::chrono::system_clock::now();
   auto t2 = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
   std::cout << "time: " << t2.count() << "ms" << std::endl;
   
   std::cout << "verifying..." << std::endl;
   assert(d1 == d2);
}
