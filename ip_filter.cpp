#include "ipaddr.h"

#include <iostream>
#include <stdexcept>
#include <set>
#include <ranges>
#include <algorithm>


const int parsing_errorcode = 101;
const int generic_errorcode = 102;

// NOLINTNEXTLINE(hicpp-named-parameter,readability-named-parameter)
int main(int, char const *[])
{
  try
  {
    std::multiset<ipaddr> ip_pool;
    int lineno{0};
    for(std::string line; std::getline(std::cin, line);++lineno)
    {
      try {
        ip_pool.emplace(line.substr(0, line.find_first_of('\t')));
      }
      catch(const std::exception &e)
      {
        std::cerr << "Error in line " << lineno << ": " << e.what() << std::endl;
        return parsing_errorcode;
      }
    }

      // lexicographic order output
    for(auto && ip : ip_pool)
    {
      std::cout << ip.str() << "\n";
    }

      // filter by first byte = 1 and output
    std::for_each(
      ip_pool.lower_bound(ipaddr("1.255.255.255")),
      ip_pool.upper_bound(ipaddr("1.0.0.0"      )),
      [](const ipaddr &ip){std::cout << ip.str() << "\n";});

      // filter by first and second bytes and output
      // ip = filter(46, 70)
    std::for_each(
      ip_pool.lower_bound(ipaddr("46.70.255.255")),
      ip_pool.upper_bound(ipaddr("46.70.0.0"    )),
      [](const ipaddr &ip){std::cout << ip.str() << "\n";});


      // filter by any byte and output
      // ip = filter_any(46)
    const unsigned search_byte = 46;
    std::for_each(
      ip_pool.begin(), ip_pool.end(),
      [](const ipaddr &ip){
        const unsigned uip = ip.uint();
        for(unsigned ii = 0; ii < 4; ++ii)
        {
          if(((uip >> (ipaddr::bytesize() * ii)) & ipaddr::maxbyte()) == search_byte)
          {
            std::cout << ip.str() << "\n";
            break;
          }
        }
      });
  }
  catch(const std::exception &e)
  {
    std::cerr << e.what() << std::endl;
    return generic_errorcode;
  }
  return 0;
}
