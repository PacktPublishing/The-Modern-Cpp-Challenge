#include <iostream>
#include <string_view>
#include <string>

#include "pngwriter.h"

void create_flag(int const width, int const height, std::string_view filepath)
{
   pngwriter flag{ width, height, 0, filepath.data() };

   int const size = width / 3;
   // red rectangle
   flag.filledsquare(0, 0, size, 2 * size, 65535, 0, 0);
   // yellow rectangle
   flag.filledsquare(size, 0, 2 * size, 2 * size, 65535, 65535, 0);
   // blue rectangle
   flag.filledsquare(2 * size, 0, 3 * size, 2 * size, 0, 0, 65535);

   flag.close();
}

int main()
{
   int width = 0, height = 0;
   std::string filepath;

   std::cout << "Width: "; 
   std::cin >> width;

   std::cout << "Heigh: ";
   std::cin >> height;

   std::cout << "Output: ";
   std::cin >> filepath;

   create_flag(width, height, filepath);
}
