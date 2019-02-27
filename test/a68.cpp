#include <iostream>
#include <string>
#include <memory>
#include <random>
#include <array>
#include <algorithm>

class password_generator
{
public:
   virtual std::string generate() = 0;
   
   virtual std::string allowed_chars() const = 0;
   virtual size_t length() const = 0;
   
   virtual void add(std::unique_ptr<password_generator> generator) = 0;
   
   virtual ~password_generator(){}
};

class basic_password_generator : public password_generator
{
   size_t len;
public:
   explicit basic_password_generator(size_t const len) noexcept : len(len) {}
   
   virtual std::string generate() override
   { throw std::runtime_error("not implemented"); }
   
   virtual void add(std::unique_ptr<password_generator>) override
   { throw std::runtime_error("not implemented"); }
   
   virtual size_t length() const noexcept override final
   {return len;}
};

class digit_generator : public basic_password_generator
{
public:
   explicit digit_generator(size_t const len) noexcept
   : basic_password_generator(len) {}
   
   virtual std::string allowed_chars() const override
   {return "0123456789";}
};

class symbol_generator : public basic_password_generator
{
public:
   explicit symbol_generator(size_t const len) noexcept
   : basic_password_generator(len) {}
   
   virtual std::string allowed_chars() const override
   {return "!@#$%^&*(){}[]?<>";}
};

class upper_letter_generator : public basic_password_generator
{
public:
   explicit upper_letter_generator(size_t const len) noexcept
   : basic_password_generator(len) {}
   
   virtual std::string allowed_chars() const override
   {return "ABCDEFGHIJKLMNOPQRSTUVXYWZ";}
};

class lower_letter_generator : public basic_password_generator
{
public:
   explicit lower_letter_generator(size_t const len) noexcept
   : basic_password_generator(len) {}
   
   virtual std::string allowed_chars() const override
   {return "abcdefghijklmnopqrstuvxywz";}
};

class composite_password_generator : public password_generator
{
   virtual std::string allowed_chars() const override
   { throw std::runtime_error("not implemented"); };
   virtual size_t length() const override
   { throw std::runtime_error("not implemented"); };
public:
   composite_password_generator()
   {
      auto seed_data = std::array<int, std::mt19937::state_size> {};
      std::generate(std::begin(seed_data), std::end(seed_data), std::ref(rd));
      std::seed_seq seq(std::begin(seed_data), std::end(seed_data));
      eng.seed(seq);
   }
   
   virtual std::string generate() override
   {
      std::string password;
      for(auto & generator : generators)
      {
         std::string chars = generator->allowed_chars();
         std::uniform_int_distribution<> ud(0, static_cast<int>(chars.length() - 1));
         
         for(size_t i = 0; i < generator->length(); ++i)
            password += chars[ud(eng)];
      }
      
      std::shuffle(std::begin(password), std::end(password), eng);
      
      return password;
   }
   
   virtual void add(std::unique_ptr<password_generator> generator) override
   {
      generators.push_back(std::move(generator));
   }
   
private:
   std::random_device rd;
   std::mt19937 eng;
   std::vector<std::unique_ptr<password_generator>> generators;
};

int main()
{
   composite_password_generator generator;
   generator.add(std::make_unique<symbol_generator>(2));
   generator.add(std::make_unique<digit_generator>(2));
   generator.add(std::make_unique<upper_letter_generator>(2));
   generator.add(std::make_unique<lower_letter_generator>(4));
   
   auto password = generator.generate();
   std::cout << password << std::endl;
}

