#include <vector>
#include <iostream>
#include <algorithm>
#include <thread>
#include <chrono>
#include <mutex>
#include <iterator>

template <typename T>
class double_buffer
{
   typedef T         value_type;
   typedef T&        reference;
   typedef T const & const_reference;
   typedef T*        pointer;
public:
   explicit double_buffer(size_t const size) :
      rdbuf(size), wrbuf(size)
   {}

   size_t size() const noexcept { return rdbuf.size(); }

   void write(T const * const ptr, size_t const size)
   {
      std::unique_lock<std::mutex> lock(mt);
      auto length = std::min(size, wrbuf.size());
      std::copy(ptr, ptr + length, std::begin(wrbuf));
      wrbuf.swap(rdbuf);
   }
   
   template <class Output>
   void read(Output it) const
   {
      std::unique_lock<std::mutex> lock(mt);
      std::copy(std::cbegin(rdbuf), std::cend(rdbuf), it);
   }
   
   pointer data() const
   {
       std::unique_lock<std::mutex> lock(mt);
       return rdbuf.data();
   }

   reference operator[](size_t const pos)
   {
      std::unique_lock<std::mutex> lock(mt);
      return rdbuf[pos];
   }
   
   const_reference operator[](size_t const pos) const
   {
      std::unique_lock<std::mutex> lock(mt);
      return rdbuf[pos];
   }

   void swap(double_buffer other)
   {
      std::swap(rdbuf, other.rdbuf);
      std::swap(wrbuf, other.wrbuf);
   }

private:
   std::vector<T> rdbuf;
   std::vector<T> wrbuf;
   mutable std::mutex mt;
};

template <typename T>
void print_buffer(double_buffer<T> const & buf)
{
   buf.read(std::ostream_iterator<T>(std::cout, " "));
   std::cout << std::endl;
}

int main()
{
   double_buffer<int> buf(10);

   std::thread t([&buf]() {
      for (int i = 1; i < 1000; i += 10)
      {
         int data[] = { i, i + 1, i + 2, i + 3, i + 4, i + 5, i + 6,i + 7,i + 8,i + 9 };
         buf.write(data, 10);

         using namespace std::chrono_literals;
         std::this_thread::sleep_for(100ms);
      }
   });

   auto start = std::chrono::system_clock::now();
   do
   {
      print_buffer(buf);

      using namespace std::chrono_literals;
      std::this_thread::sleep_for(150ms);
   } while (std::chrono::duration_cast<std::chrono::seconds>(std::chrono::system_clock::now() - start).count() < 12);

   t.join();
}
