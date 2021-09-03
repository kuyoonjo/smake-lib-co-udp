#pragma once

#include <co/co.h>
#include <type_traits>

namespace ex {

struct ipv {};

struct v4 : ipv {
  using sockaddr_t = sockaddr_in;
  static inline auto domain = AF_INET;
};
struct v6 : ipv {
  using sockaddr_t = sockaddr_in6;
  static inline auto domain = AF_INET6;
};

template <typename T> struct ipaddr {
  static_assert(std::is_base_of<ipv, T>::value, "T must inherit from ipv");
  explicit ipaddr() {}
  explicit ipaddr(const fastring &ip, uint16 port) {
    co::init_ip_addr(&sockaddr, ip.c_str(), port);
  }
  explicit ipaddr(const typename T::sockaddr_t &addr) : sockaddr(addr) {}

  fastring ip() const { return co::ip_str(&sockaddr); }

  uint16 port() const { return ntoh16(sockaddr.sin_port); }
  typename T::sockaddr_t sockaddr;
};

inline bool operator==(const ex::ipaddr<ex::v4> &c1,
                       const ex::ipaddr<ex::v4> &c2) {
  return (c1.sockaddr.sin_port == c2.sockaddr.sin_port &&
          c1.sockaddr.sin_addr.s_addr == c2.sockaddr.sin_addr.s_addr);
}

inline bool operator!=(const ex::ipaddr<ex::v4> &c1,
                       const ex::ipaddr<ex::v4> &c2) {
  return (c1.sockaddr.sin_port != c2.sockaddr.sin_port ||
          c1.sockaddr.sin_addr.s_addr != c2.sockaddr.sin_addr.s_addr);
}
inline bool operator==(const ex::ipaddr<ex::v6> &c1,
                       const ex::ipaddr<ex::v6> &c2) {
  return (c1.sockaddr.sin6_port == c2.sockaddr.sin6_port &&
          c1.sockaddr.sin6_scope_id == c2.sockaddr.sin6_scope_id &&
          c1.sockaddr.sin6_flowinfo == c2.sockaddr.sin6_flowinfo &&
          std::memcmp(&c1.sockaddr.sin6_addr, &c2.sockaddr.sin6_addr,
                      sizeof(c1.sockaddr.sin6_addr)) == 0);
}

inline bool operator!=(const ex::ipaddr<ex::v6> &c1,
                       const ex::ipaddr<ex::v6> &c2) {
  return (c1.sockaddr.sin6_port != c2.sockaddr.sin6_port ||
          c1.sockaddr.sin6_scope_id != c2.sockaddr.sin6_scope_id ||
          c1.sockaddr.sin6_flowinfo != c2.sockaddr.sin6_flowinfo ||
          std::memcmp(&c1.sockaddr.sin6_addr, &c2.sockaddr.sin6_addr,
                      sizeof(c1.sockaddr.sin6_addr)) != 0);
}
} // namespace ex