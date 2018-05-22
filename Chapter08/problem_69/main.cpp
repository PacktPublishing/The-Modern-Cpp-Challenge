#include <iostream>
#include <string>
#include <sstream>
#include <string_view>
#include <random>
#include <array>
#include <map>
#include <memory>
#include <algorithm>
#include <numeric>
#include <assert.h>

enum class sex_type {female, male};

class social_number_generator
{
protected:
   virtual int sex_digit(sex_type const sex) const noexcept = 0;
   virtual int next_random(unsigned const year, unsigned const month, unsigned const day) = 0;
   virtual int modulo_value() const noexcept = 0;
   
   social_number_generator(int const min, int const max):ud(min, max)
   {
      std::random_device rd;
      auto seed_data = std::array<int, std::mt19937::state_size> {};
      std::generate(std::begin(seed_data), std::end(seed_data), std::ref(rd));
      std::seed_seq seq(std::begin(seed_data), std::end(seed_data));
      eng.seed(seq);
   }
  
public:
   std::string generate(
      sex_type const sex,
      unsigned const year, unsigned const month, unsigned const day)
   {
      std::stringstream snumber;
      
      snumber << sex_digit(sex);
      
      snumber << year << month << day;
      
      snumber << next_random(year, month, day);
      
      auto number = snumber.str();
      
      auto index = number.length();
      auto sum = std::accumulate(std::begin(number), std::end(number), 0u,
                                 [&index](unsigned int const s, char const c) {
                                    return s + static_cast<unsigned int>(index-- * (c-'0'));});
      
      auto rest = sum % modulo_value();
      snumber << modulo_value() - rest;
      
      return snumber.str();
   }
   
   virtual ~social_number_generator() {}
   
protected:
   std::map<unsigned, int> cache;
   std::mt19937 eng;
   std::uniform_int_distribution<> ud;
};

class southeria_social_number_generator final : public social_number_generator
{
public:
   southeria_social_number_generator():
      social_number_generator(1000, 9999)
   {
   }

protected:
   virtual int sex_digit(sex_type const sex) const noexcept override
   {
      if(sex == sex_type::female) return 1;
      else return 2;
   }
   
   virtual int next_random(unsigned const year, unsigned const month, unsigned const day) override
   {
      auto key = year * 10000 + month * 100 + day;
      while(true)
      {
         auto number = ud(eng);
         auto pos = cache.find(number);
         if(pos == std::end(cache))
         {
            cache[key] = number;
            return number;
         }
      }
   }
   
   virtual int modulo_value() const noexcept override
   {
      return 11;
   }
};

class northeria_social_number_generator final : public social_number_generator
{
public:
   northeria_social_number_generator():
      social_number_generator(10000, 99999)
   {
   }

protected:
   virtual int sex_digit(sex_type const sex) const noexcept override
   {
      if(sex == sex_type::female) return 9;
      else return 7;
   }
   
   virtual int next_random(unsigned const year, unsigned const month, unsigned const day) override
   {
      auto key = year * 10000 + month * 100 + day;
      while(true)
      {
         auto number = ud(eng);
         auto pos = cache.find(number);
         if(pos == std::end(cache))
         {
            cache[key] = number;
            return number;
         }
      }
   }
   
   virtual int modulo_value() const noexcept override
   {
      return 11;
   }
};

class social_number_generator_factory
{
public:
   social_number_generator_factory()
   {
      generators["northeria"] = std::make_unique<northeria_social_number_generator>();
      generators["southeria"] = std::make_unique<southeria_social_number_generator>();
   }
   
   social_number_generator* get_generator(std::string_view country) const
   {
      auto it = generators.find(country.data());
      if(it != std::end(generators))
         return it->second.get();
      
      throw std::runtime_error("invalid country");
   }
   
private:
   std::map<std::string, std::unique_ptr<social_number_generator>> generators;
};

int main()
{
   social_number_generator_factory factory;
   
   auto sn1 = factory.get_generator("northeria")->generate(sex_type::female, 2017, 12, 25);
   auto sn2 = factory.get_generator("northeria")->generate(sex_type::female, 2017, 12, 25);
   auto sn3 = factory.get_generator("northeria")->generate(sex_type::male, 2017, 12, 25);
   
   auto ss1 = factory.get_generator("southeria")->generate(sex_type::female, 2017, 12, 25);
   auto ss2 = factory.get_generator("southeria")->generate(sex_type::female, 2017, 12, 25);
   auto ss3 = factory.get_generator("southeria")->generate(sex_type::male, 2017, 12, 25);
}
