#include "split.h"

#include <cassert>
#include <cstdlib>
#include <iostream>
#include <stdexcept>
#include <string>
#include <charconv>
#include <vector>
#include <set>
#include <ranges>
#include <algorithm>

class ipaddr
{
public:
  ipaddr(const std::string &str)
    : m_str(str), m_uint(str2ip(m_str))
    {
    }

  static unsigned str2ip(const std::string &ipstr)
  {
    unsigned uint = 0;
    const char *curpos = ipstr.data();
    const char *endpos = ipstr.data() +ipstr.size();
    for(int ii = 3; ii >= 0; --ii)
    {
      unsigned byte;
      auto rez = std::from_chars(curpos, endpos, byte);
      if (rez.ec != std::errc()) throw std::logic_error("can not convert IP component");
      curpos = rez.ptr;
      if(byte > 255) throw std::logic_error("component of the IP address is larger than 255");
      if(curpos != endpos)
      {
        if(*curpos != '.') throw std::logic_error("IP component delimiter must be '.'");
        ++curpos;
      }
      uint += byte << (ii*8);
    }
    return uint;
  }

  bool operator<(const ipaddr &other) const
  {
    return m_uint > other.m_uint;
  }

  std::string str() const
  {
      // for memory optimization convert from m_uint
    return m_str;
  }

  unsigned uint() const
  {
    return m_uint;
  }


private:
  std::string m_str;
  unsigned m_uint;
};



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
        return 101;
      }
    }

      // lexicographical sorted output
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

    std::for_each(
      ip_pool.begin(), ip_pool.end(),
      [](const ipaddr &ip){
        unsigned uip = ip.uint();
        for(int ii = 0; ii < 4; ++ii)
        {
          if(((uip >> (8 * ii)) & 255) == 46)
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
    return 102;
  }
  return 0;
}
