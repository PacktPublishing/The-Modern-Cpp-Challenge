#include <iostream>
#include <ostream>
#include <string>
#include <string_view>
#include <map>

#include "curl_easy.h"
#include "curl_exception.h"

#include "json.hpp"

using json = nlohmann::json;

struct exchange_info
{
   double delay_15m_price;
   double latest_price;
   double buying_price;
   double selling_price;
   std::string symbol;
};

using blockchain_rates = std::map<std::string, exchange_info>;

void from_json(const json& jdata, exchange_info& info)
{
   info.delay_15m_price = jdata.at("15m").get<double>();
   info.latest_price = jdata.at("last").get<double>();
   info.buying_price = jdata.at("buy").get<double>();
   info.selling_price = jdata.at("sell").get<double>();
   info.symbol = jdata.at("symbol").get<std::string>();
}

std::stringstream get_json_document(std::string_view url)
{
   std::stringstream str;

   try 
   {
      curl::curl_ios<std::stringstream> writer(str);
      curl::curl_easy easy(writer);

      easy.add<CURLOPT_URL>(url.data());
      easy.add<CURLOPT_FOLLOWLOCATION>(1L);

      easy.perform();
   }
   catch (curl::curl_easy_exception const & error) 
   {
      auto errors = error.get_traceback();
      error.print_traceback();
   }
   
   return str;
}

int main() 
{
   auto doc = get_json_document("https://blockchain.info/ticker");

   json jdata;
   doc >> jdata;

   blockchain_rates rates = jdata;

   for (auto const & kvp : rates)
   {
      std::cout << "1BPI = " << kvp.second.latest_price 
                << " " << kvp.first << std::endl;
   }
}
