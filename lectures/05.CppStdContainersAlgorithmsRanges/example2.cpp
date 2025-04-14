#include <ranges>
#include <iostream>
#include <array>
#include <string>


/*

Filter only non default values (example – nonzero for numbers) – e.g.:{1,0,2,3}  {1,2,3}
Transform this filtered range to duplication of input elements – e.g.:{1, 2, 3, 4}  {2, 4, 6, 8} 
{“1”s, “2”s, “3”s, “4”s}  {“11”s, “22”s, “33”s, “44”s} 
Prints 20 elements, at most, from that filtered/transformed range

*/

template <std::ranges::range R>
void double_print(R const&& range)
{
   auto non_zero = []<typename T>(const T& p) { return p != T();  };
   auto double_value = [](const auto& p) { return p + p;  };
   using namespace std::views;

   for (auto&& e : range | filter(non_zero) | take(20) | transform(double_value))
   {
      std::cout << '[' << e << ']';
   }
   std::cout << '\n';
}

template <std::ranges::range R>
void double_print_classic(R const&& range)
{
   using value_type = std::ranges::range_value_t<R>;
   size_t counter{0};
   for (auto&& e : range)
   {
      if (e != value_type())
      {
         if (++counter > 20) break;
         std::cout << '[' << (e + e) << ']';
      }
   }
   std::cout << '\n';
}

int main() 
{
   using namespace std::string_literals;
   double_print(std::array{"1"s,"2"s,"3"s,""s,""s,"4"s,"5"s,"6"s,"7"s,"8"s,"9"s,"10"s,"11"s,"12"s,"13"s,"14"s,"15"s,"16"s,"17"s,"18"s});
   double_print(std::array{1,2,3,0,0,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18});

   double_print_classic(std::array{"1"s,"2"s,"3"s,""s,""s,"4"s,"5"s,"6"s,"7"s,"8"s,"9"s,"10"s,"11"s,"12"s,"13"s,"14"s,"15"s,"16"s,"17"s,"18"s});
   double_print_classic(std::array{1,2,3,0,0,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18});
}
