#include <charconv>
#include <cstdint>
#include <stdexcept>
#include "ipaddr.h"

ipaddr::ipaddr(std::string &&str)
  : m_str(str), m_uint(str2ip(m_str))
{
}


unsigned ipaddr::str2ip(const std::string &ipstr)
{
  unsigned uint = 0;
  const char *curpos = ipstr.data();
  const char *endpos = ipstr.data() + ipstr.size(); // NOLINT(cppcoreguidelines-pro-bounds-pointer-arithmetic)
  for(int ii = 3; ii >= 0; --ii)
  {
    uint8_t byte{0};
    auto rez = std::from_chars(curpos, endpos, byte);
    if (rez.ec != std::errc()) { throw std::logic_error("can not convert IP component"); } // TODO: detailed error message
    curpos = rez.ptr;
    if(curpos != endpos)
    {
      if(*curpos != '.') { throw std::logic_error("IP component delimiter must be '.'"); }
      ++curpos; // NOLINT(cppcoreguidelines-pro-bounds-pointer-arithmetic)
    }
    uint += ((unsigned)byte) << ((unsigned)ii*bytesize());
  }
  return uint;
}
