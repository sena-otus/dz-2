#include <climits>
#include <limits>
#include <string>

class ipaddr
{
public:
    /** ctor  will throw if IP is invalid
     * @param str IP in form X.X.X.X */
  explicit ipaddr(std::string &&str);

    /* convert IP from string representation to unsigned int,
     * suitable for sorting
     * @param ipstr IP in form X.X.X.X
     * @return integer representation */
  static unsigned str2ip(const std::string &ipstr);

    /** compare operator to sort in reverse lexicographic order */
  bool operator<(const ipaddr &other) const
  {
    return m_uint > other.m_uint;
  }

    /** @return string representation of the IP address */
  std::string str() const
  {
      // for memory optimization convert from m_uint and remove m_str
    return m_str;
  }

    /** @return unsigned int representation of the IP */
  unsigned uint() const
  {
    return m_uint;
  }

    /**  must be 255 */
  static size_t maxbyte() { return std::numeric_limits<uint8_t>::max();}
    /**  must be 8 */
  static size_t bytesize() { return CHAR_BIT; }

private:
  std::string m_str;
  unsigned m_uint;
};
