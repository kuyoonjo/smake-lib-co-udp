# C++ UDP Class
## Dependencies
- [co](https://github.com/kuyoonjo/smake-lib-co.git)
- [buffer](https://github.com/kuyoonjo/smake-lib-buffer.git)

## ipaddr.h
```c++
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
  explicit ipaddr(const fastring &ip, uint16 port);
  explicit ipaddr(const typename T::sockaddr_t &addr);
  fastring ip() const;
  uint16 port() const;
  typename T::sockaddr_t sockaddr;
};

inline bool operator==(const ex::ipaddr<ex::v4> &c1,
                       const ex::ipaddr<ex::v4> &c2);

inline bool operator!=(const ex::ipaddr<ex::v4> &c1,
                       const ex::ipaddr<ex::v4> &c2);
inline bool operator==(const ex::ipaddr<ex::v6> &c1,
                       const ex::ipaddr<ex::v6> &c2);

inline bool operator!=(const ex::ipaddr<ex::v6> &c1,
                       const ex::ipaddr<ex::v6> &c2);
```

## udp.h
```c++
namespace ex {
template <typename T> class udp {
  static_assert(std::is_base_of<ipv, T>::value, "T must inherit from ipv");

public:
  explicit udp();

  int bind(const ipaddr<T> &ia);
  int recvfrom(int ms = -1);

  int sendto(char const *str, const ipaddr<T> &dst_ipaddr, int ms = -1) {
    return co::sendto(m_socket, str, std::strlen(str), &dst_ipaddr.sockaddr,
                      sizeof(dst_ipaddr.sockaddr), ms);
  }

  template <typename U>
  int sendto(U *buf, size_t size, const ipaddr<T> &dst_ipaddr, int ms = -1);

  template <typename U>
  int sendto(const U &t, const ipaddr<T> &dst_ipaddr, int ms = -1) const;

  template <typename U>
  int sendto(U &&t, const ipaddr<T> &dst_ipaddr, int ms = -1) const;

  template <typename U>
  int sendto(std::initializer_list<U> t, const ipaddr<T> &dst_ipaddr,
             int ms = -1);

  ex::shared_buffer recv_buffer();
  ipaddr<T> &rmt_ipaddr();
};

} // namespace ex

```