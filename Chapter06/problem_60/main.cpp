#include <iostream>
#include <vector>
#include <random>
#include <array>
#include <thread>
#include <chrono>

class universe
{
private:
   universe() = delete;

public:
   enum class seed
   {
      random,
      ten_cell_row,
      small_explorer,
      explorer
   };
public:
   universe(size_t const width, size_t const height):
      rows(height), columns(width),grid(width * height), dist(0, 4)
   {
      std::random_device rd;
      auto seed_data = std::array<int, std::mt19937::state_size> {};
      std::generate(std::begin(seed_data), std::end(seed_data), std::ref(rd));
      std::seed_seq seq(std::begin(seed_data), std::end(seed_data));
      mt.seed(seq);
   }

   void run(
      seed const s,
      int const generations, 
      std::chrono::milliseconds const ms = std::chrono::milliseconds(100))
   {
      reset();
      initialize(s);
      display();

      int i = 0;
      do 
      {
         next_generation();
         display();

         using namespace std::chrono_literals;
         std::this_thread::sleep_for(ms);

      } while (i++ < generations || generations == 0);
   }

private:
   void next_generation()
   {
      std::vector<unsigned char> newgrid(grid.size());

      for (size_t r = 0; r < rows; ++r)
      {
         for (size_t c = 0; c < columns; ++c)
         {
            auto count = count_neighbors(r, c);

            if (cell(c, r) == alive)
            {
               newgrid[r * columns + c] = (count == 2 || count == 3) ? alive : dead;
            }
            else 
            {
               newgrid[r * columns + c] = (count == 3) ? alive : dead;
            }
         }
      }

      grid.swap(newgrid);
   }
    
   void reset_display()
   {
#ifdef _WIN32
      system("cls");
#endif
   }

   void display()
   {
      reset_display();

      for (size_t r = 0; r < rows; ++r)
      {
         for (size_t c = 0; c < columns; ++c)
         {
            std::cout << (cell(c, r) ? '*' : ' ');
         }
         std::cout << std::endl;
      }
   }

   void initialize(seed const s)
   {
      if (s == seed::small_explorer)
      {
         auto mr = rows / 2;
         auto mc = columns / 2;

         cell(mc, mr) = alive;
         cell(mc - 1, mr + 1) = alive;
         cell(mc, mr + 1) = alive;
         cell(mc + 1, mr + 1) = alive;
         cell(mc - 1, mr + 2) = alive;
         cell(mc + 1, mr + 2) = alive;
         cell(mc, mr + 3) = alive;
      }
      else if (s == seed::explorer)
      {
         auto mr = rows / 2;
         auto mc = columns / 2;

         cell(mc - 2, mr - 2) = alive;
         cell(mc    , mr - 2) = alive;
         cell(mc + 2, mr - 2) = alive;

         cell(mc - 2, mr - 1) = alive;
         cell(mc + 2, mr - 1) = alive;
         cell(mc - 2, mr    ) = alive;
         cell(mc + 2, mr    ) = alive;
         cell(mc - 2, mr + 1) = alive;
         cell(mc + 2, mr + 1) = alive;

         cell(mc - 2, mr + 2) = alive;
         cell(mc    , mr - 2) = alive;
         cell(mc + 2, mr + 2) = alive;
      }
      else if (s == seed::ten_cell_row)
      {
         for (size_t c = columns / 2 - 5; c < columns / 2 + 5; c++)
            cell(c, rows / 2) = alive;
      }
      else
      {
         for (size_t r = 0; r < rows; ++r)
         {
            for (size_t c = 0; c < columns; ++c)
            {
               cell(c, r) = dist(mt) == 0 ? alive : dead;
            }
         }
      }
   }

   void reset()
   {
      for (size_t r = 0; r < rows; ++r)
      {
         for (size_t c = 0; c < columns; ++c)
         {
            cell(c, r) = dead;
         }
      }
   }

    
   int count_alive() { return 0; }
    
   template<typename T1, typename... T>
   auto count_alive(T1 s, T... ts) { return s + count_alive(ts...); }
    
   int count_neighbors(size_t const row, size_t const col)
   {
      if (row == 0 && col == 0) 
         return count_alive(cell(1, 0), cell(1,1), cell(0, 1));
      if (row == 0 && col == columns - 1)
         return count_alive(cell(columns - 2, 0), cell(columns - 2, 1), cell(columns - 1, 1));
      if (row == rows - 1 && col == 0)
         return count_alive(cell(0, rows - 2), cell(1, rows - 2), cell(1, rows - 1));
      if (row == rows - 1 && col == columns - 1)
         return count_alive(cell(columns - 1, rows - 2), cell(columns - 2, rows - 2), cell(columns - 2, rows - 1));
      if (row == 0 && col > 0 && col < columns - 1)
         return count_alive(cell(col - 1, 0), cell(col - 1, 1), cell(col, 1), cell(col + 1, 1), cell(col + 1, 0));
      if (row == rows - 1 && col > 0 && col < columns - 1)
         return count_alive(cell(col - 1, row), cell(col - 1, row - 1), cell(col, row - 1), cell(col + 1, row - 1), cell(col + 1, row));
      if (col == 0 && row > 0 && row < rows - 1)
         return count_alive(cell(0, row - 1), cell(1, row - 1), cell(1, row), cell(1, row + 1), cell(0, row + 1));
      if (col == columns - 1 && row > 0 && row < rows - 1)
         return count_alive(cell(col, row - 1), cell(col - 1, row - 1), cell(col - 1, row), cell(col - 1, row + 1), cell(col, row + 1));
         
      return count_alive(cell(col - 1, row - 1), cell(col, row - 1), cell(col + 1, row - 1), cell(col + 1, row), cell(col + 1, row + 1), cell(col, row + 1), cell(col - 1, row + 1), cell(col - 1, row));
   }

   unsigned char& cell(size_t const col, size_t const row)
   {
      return grid[row * columns + col];
   }

private:
   size_t rows;
   size_t columns;

   std::vector<unsigned char> grid;
   const unsigned char alive = 1;
   const unsigned char dead = 0;

   std::uniform_int_distribution<> dist;
   std::mt19937 mt;
};

int main()
{
   using namespace std::chrono_literals;

   universe u(50, 20);
   u.run(universe::seed::random, 100, 100ms);
}
