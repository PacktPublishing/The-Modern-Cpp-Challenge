#ifdef _WIN32
#include <windows.h>
#else
typedef void *HANDLE;

#define DWORD     unsigned long
#ifdef _LP64
#define LONG_PTR  long long
#define ULONG_PTR unsigned long long
#else
#define LONG_PTR  long
#define ULONG_PTR unsigned long
#endif
#define INVALID_HANDLE_VALUE ((HANDLE)(LONG_PTR)-1)

struct SECURITY_ATTRIBUTES
{
    DWORD nLength;
    void* lpSecurityDescriptor;
    int bInheritHandle;
};

struct OVERLAPPED
{
    ULONG_PTR Internal;
    ULONG_PTR InternalHigh;
    union {
        struct {
            DWORD Offset;
            DWORD OffsetHigh;
        } DUMMYSTRUCTNAME;
        
        void* Pointer;
    } DUMMYUNIONNAME;
    
    HANDLE hEvent;
};

int CloseHandle(HANDLE hObject) {return 0;}
HANDLE CreateFile(char const *,
                  DWORD,
                  DWORD,
                  SECURITY_ATTRIBUTES*,
                  DWORD,
                  DWORD,
                  HANDLE)
{return INVALID_HANDLE_VALUE;}

int ReadFile(HANDLE,
             void*,
             DWORD,
             DWORD *,
             OVERLAPPED*)
{return 0;}

#define GENERIC_READ           0x80000000L
#define GENERIC_WRITE          0x40000000L
#define CREATE_NEW             1
#define CREATE_ALWAYS          2
#define OPEN_EXISTING          3
#define OPEN_ALWAYS            4
#define TRUNCATE_EXISTING      5
#define FILE_SHARE_READ        1
#define FILE_ATTRIBUTE_NORMAL  0x00000080
#endif

#include <algorithm>
#include <vector>

template <typename Traits>
class unique_handle
{
   using pointer = typename Traits::pointer;

   pointer m_value;

public:
   unique_handle(unique_handle const &) = delete;
   unique_handle& operator=(unique_handle const &) = delete;

   explicit unique_handle(pointer value = Traits::invalid()) noexcept
      :m_value{ value }
   {}

   unique_handle(unique_handle && other) noexcept
      : m_value{ other.release() }
   {}

   unique_handle& operator=(unique_handle && other) noexcept
   {
      if (this != &other)
      {
         reset(other.release());
      }

      return *this;
   }

   ~unique_handle() noexcept
   {
      Traits::close(m_value);
   }

   explicit operator bool() const noexcept
   {
      return m_value != Traits::invalid();
   }

   pointer get() const noexcept
   {
      return m_value;
   }

   pointer release() noexcept
   {
      auto value = m_value;
      m_value = Traits::invalid();
      return value;
   }

   bool reset(pointer value = Traits::invalid()) noexcept
   {
      if (m_value != value)
      {
         Traits::close(m_value);
         m_value = value;
      }

      return static_cast<bool>(*this);
   }

   void swap(unique_handle<Traits> & other) noexcept
   {
      std::swap(m_value, other.m_value);
   }
};

template <typename Traits>
void swap(unique_handle<Traits> & left,
   unique_handle<Traits> & right) noexcept
{
   left.swap(right);
}

template <typename Traits>
bool operator==(
   unique_handle<Traits> const & left,
   unique_handle<Traits> const & right) noexcept
{
   return left.get() == right.get();
}

template <typename Traits>
bool operator!=(
   unique_handle<Traits> const & left,
   unique_handle<Traits> const & right) noexcept
{
   return left.get() != right.get();
}

struct null_handle_traits
{
   using pointer = HANDLE;

   static pointer invalid() noexcept
   {
      return nullptr;
   }

   static void close(pointer value) noexcept
   {
      CloseHandle(value);
   }
};

struct invalid_handle_traits
{
   using pointer = HANDLE;

   static pointer invalid() noexcept
   {
      return INVALID_HANDLE_VALUE;
   }

   static void close(pointer value) noexcept
   {
      CloseHandle(value);
   }
};

using null_handle = unique_handle<null_handle_traits>;
using invalid_handle = unique_handle<invalid_handle_traits>;

void function_that_throws()
{
    throw std::runtime_error("an error has occurred");
}

void bad_handle_example()
{
    bool condition1 = false;
    bool condition2 = true;
    HANDLE handle = CreateFile("sample.txt",
                               GENERIC_READ,
                               FILE_SHARE_READ,
                               nullptr,
                               OPEN_EXISTING,
                               FILE_ATTRIBUTE_NORMAL,
                               nullptr);
    
    if (handle == INVALID_HANDLE_VALUE)
        return;
    
    if (condition1)
    {
        CloseHandle(handle);
        return;
    }
    
    std::vector<char> buffer(1024);
    unsigned long bytesRead = 0;
    ReadFile(handle, buffer.data(), buffer.size(), &bytesRead, nullptr);
    
    if (condition2)
    {
        // oops, forgot to close handle
        return;
    }
    
    // throws exception; the next line will not execute
    function_that_throws();
    
    CloseHandle(handle);
}

void good_handle_example()
{
    bool condition1 = false;
    bool condition2 = true;
    
    invalid_handle handle{
        CreateFile("sample.txt",
                   GENERIC_READ,
                   FILE_SHARE_READ,
                   nullptr,
                   OPEN_EXISTING,
                   FILE_ATTRIBUTE_NORMAL,
                   nullptr) };
    
    if (!handle) return;
    
    if (condition1) return;
    
    std::vector<char> buffer(1024);
    unsigned long bytesRead = 0;
    ReadFile(handle.get(),
             buffer.data(),
             buffer.size(),
             &bytesRead,
             nullptr);
    
    if (condition2) return;
    
    function_that_throws();
}

int main()
{
    try
    {
        bad_handle_example();
    }
    catch (...) {}
    
    try
    {
        good_handle_example();
    }
    catch (...) {}
    
    return 0;
}
