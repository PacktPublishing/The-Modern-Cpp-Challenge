#include <iostream>
#include <vector>
#include <array>
#include <algorithm>
#include <functional>
#include <random>
#include <chrono>
#include <thread>
#include <assert.h>

template <typename T, typename F>
std::vector<T> alter(std::vector<T> data, F&& f)
{
   std::transform(
      std::begin(data), std::end(data), std::begin(data),
      std::forward<F>(f));

   return data;
}

template <typename T, typename F>
std::vector<T> palter(std::vector<T> data, F&& f)
{
   if (data.size() <= 10000)
   {
      std::transform(
         std::begin(data), std::end(data), std::begin(data),
         std::forward<F>(f));
   }
   else
   {
      std::vector<std::thread> threads;
      int thread_count = std::thread::hardware_concurrency();
      auto first = std::begin(data);
      auto last = first;
      auto size = data.size() / thread_count;
      for (int i = 0; i < thread_count; ++i)
      {
         first = last;
         last = i == thread_count - 1 ? std::end(data) : first + size;

         threads.emplace_back([first, last, &f]() {
            std::transform(first, last, first,
               std::forward<F>(f));
         });
      }

      for (int i = 0; i < thread_count; ++i)
         threads[i].join();
   }

   return data;
}

template <typename RandomAccessIterator, typename F>
void ptransform(RandomAccessIterator begin, RandomAccessIterator end, F&& f)
{
   auto size = std::distance(begin, end);
   if (size <= 10000)
   {
      std::transform(begin, end, begin, std::forward<F>(f));
   }
   else
   {
      std::vector<std::thread> threads;
      int thread_count = 10;
      auto first = begin;
      auto last = first;
      size /= thread_count;
      for (int i = 0; i < thread_count; ++i)
      {
         first = last;
         if (i == thread_count - 1) last = end;
         else std::advance(last, size);

         threads.emplace_back([first, last, &f]() {
            std::transform(first, last, first, std::forward<F>(f));
         });
      }

      for (auto & t : threads) t.join();
   }
}

template <typename T, typename F>
std::vector<T> palter2(std::vector<T> data, F&& f)
{
   ptransform(
      std::begin(data), std::end(data),
      std::forward<F>(f));

   return data;
}

int main()
{
   const size_t count = 10000000;
   std::vector<int> data(count);
   
   std::random_device rd;
   std::mt19937 mt;
   auto seed_data = std::array<int, std::mt19937::state_size> {};
   std::generate(std::begin(seed_data), std::end(seed_data), std::ref(rd));
   std::seed_seq seq(std::begin(seed_data), std::end(seed_data));
   mt.seed(seq);
   std::uniform_int_distribution<> ud(1, 100);

   std::generate_n(std::begin(data), count, [&mt, &ud]() {return ud(mt); });
   
   auto start = std::chrono::system_clock::now();
   auto r1 = alter(data, [](int const e) {return e * e; });
   auto end = std::chrono::system_clock::now();
   auto t1 = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
   std::cout << "time: " << t1.count() << "ms" << std::endl;
   
   start = std::chrono::system_clock::now();
   auto r2 = palter(data, [](int const e) {return e * e; });
   end = std::chrono::system_clock::now();
   auto t2 = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
   std::cout << "time: " << t2.count() << "ms" << std::endl;

   start = std::chrono::system_clock::now();
   auto r3 = palter2(data, [](int const e) {return e * e; });
   end = std::chrono::system_clock::now();
   auto t3 = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
   std::cout << "time: " << t3.count() << "ms" << std::endl;

   assert(r1 == r2);
   assert(r1 == r3);
}
