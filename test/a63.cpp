#include <iostream>
#include <vector>
#include <array>
#include <algorithm>
#include <functional>
#include <random>
#include <chrono>
#include <future>
#include <assert.h>

template <typename Iterator, typename F>
auto sprocess(Iterator begin, Iterator end, F&& f)
{
   return std::forward<F>(f)(begin, end);
}

template <typename Iterator>
auto smin(Iterator begin, Iterator end)
{
   return sprocess(begin, end,
                   [](auto b, auto e){return *std::min_element(b, e);});
}

template <typename Iterator>
auto smax(Iterator begin, Iterator end)
{
   return sprocess(begin, end,
                   [](auto b, auto e){return *std::max_element(b, e);});
}

template <typename Iterator, typename F>
auto pprocess(Iterator begin, Iterator end, F&& f)
{
   auto size = std::distance(begin, end);
   if (size <= 10000)
   {
      return std::forward<F>(f)(begin, end);
   }
   else
   {
      int task_count = std::thread::hardware_concurrency();
      std::vector<std::future<typename std::iterator_traits<Iterator>::value_type>> tasks;

      auto first = begin;
      auto last = first;
      size /= task_count;
      for (int i = 0; i < task_count; ++i)
      {
         first = last;
         if (i == task_count - 1) last = end;
         else std::advance(last, size);

         tasks.emplace_back(std::async(
            std::launch::async,
            [first, last, &f]() {
               return std::forward<F>(f)(first, last);
         }));
      }

      std::vector<typename std::iterator_traits<Iterator>::value_type> mins;
      for (auto & t : tasks) 
         mins.push_back(t.get());

      return std::forward<F>(f)(std::begin(mins), std::end(mins));
   }
}

template <typename Iterator>
auto pmin(Iterator begin, Iterator end)
{
   return pprocess(begin, end,
                   [](auto b, auto e){return *std::min_element(b, e);});
}

template <typename Iterator>
auto pmax(Iterator begin, Iterator end)
{
   return pprocess(begin, end,
                   [](auto b, auto e){return *std::max_element(b, e);});
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
   std::uniform_int_distribution<> ud(1, 1000);

   std::generate_n(std::begin(data), count, [&mt, &ud]() {return ud(mt); });

   {
      std::cout << "minimum element" << std::endl;
      
      auto start = std::chrono::system_clock::now();
      auto r1 = smin(std::begin(data), std::end(data));
      auto end = std::chrono::system_clock::now();
      auto t1 = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
      std::cout << "seq time: " << t1.count() << "ms" << std::endl;

      start = std::chrono::system_clock::now();
      auto r2 = pmin(std::begin(data), std::end(data));
      end = std::chrono::system_clock::now();
      auto t2 = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
      std::cout << "par time: " << t2.count() << "ms" << std::endl;

      assert(r1 == r2);
   }
   
   {
      std::cout << "maximum element" << std::endl;
      
      auto start = std::chrono::system_clock::now();
      auto r1 = smax(std::begin(data), std::end(data));
      auto end = std::chrono::system_clock::now();
      auto t1 = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
      std::cout << "seq time: " << t1.count() << "ms" << std::endl;
      
      start = std::chrono::system_clock::now();
      auto r2 = pmax(std::begin(data), std::end(data));
      end = std::chrono::system_clock::now();
      auto t2 = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
      std::cout << "par time: " << t2.count() << "ms" << std::endl;
      
      assert(r1 == r2);
   }
}
