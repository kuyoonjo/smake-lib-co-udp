#pragma once

#include "ipaddr.h"
#include <co/def.h>
#include <ex/shared_buffer.h>

namespace ex {
template <typename T> class udp {
  static_assert(std::is_base_of<ipv, T>::value, "T must inherit from ipv");

public:
  explicit udp() : m_socket(co::udp_socket(T::domain)), m_recv_buffer(1024) {}
  int bind(const ipaddr<T> &ia) {
    return co::bind(m_socket, &ia.sockaddr, sizeof(ia.sockaddr));
  }

  int recvfrom(int ms = -1) {
    m_recv_buffer_len =
        co::recvfrom(m_socket, m_recv_buffer.data(), m_recv_buffer.size(),
                     &m_rmt_ipaddr.sockaddr, &m_rmt_ipaddr_len, ms);
    return m_recv_buffer_len;
  }

  int sendto(char const *str, const ipaddr<T> &dst_ipaddr, int ms = -1) {
    return co::sendto(m_socket, str, strlen(str), &dst_ipaddr.sockaddr,
                      sizeof(dst_ipaddr.sockaddr), ms);
  }

  template <typename U>
  int sendto(U *buf, size_t size, const ipaddr<T> &dst_ipaddr, int ms = -1) {
    return co::sendto(m_socket, buf, size, &dst_ipaddr.sockaddr,
                      sizeof(dst_ipaddr.sockaddr), ms);
  }

  template <typename U>
  int sendto(const U &t, const ipaddr<T> &dst_ipaddr, int ms = -1) const {
    return co::sendto(m_socket, t.data(), t.size(), &dst_ipaddr.sockaddr,
                      sizeof(dst_ipaddr.sockaddr), ms);
  }

  template <typename U>
  int sendto(U &&t, const ipaddr<T> &dst_ipaddr, int ms = -1) const {
    return co::sendto(m_socket, t.data(), t.size(), &dst_ipaddr.sockaddr,
                      sizeof(dst_ipaddr.sockaddr), ms);
  }

  template <typename U>
  int sendto(std::initializer_list<U> t, const ipaddr<T> &dst_ipaddr,
             int ms = -1) {
    ex::buffer v(t.size());
    v.fill(t);
    return co::sendto(m_socket, v.data(), t.size(), &dst_ipaddr.sockaddr,
                      sizeof(dst_ipaddr.sockaddr), ms);
  }

  ex::shared_buffer recv_buffer() {
    return ex::shared_buffer(m_recv_buffer, 0, m_recv_buffer_len);
  }

  ipaddr<T> &rmt_ipaddr() { return m_rmt_ipaddr; }

private:
  ex::buffer m_recv_buffer;
  int m_recv_buffer_len = 0;
  sock_t m_socket;
  ipaddr<T> m_rmt_ipaddr;
  int m_rmt_ipaddr_len = sizeof(m_rmt_ipaddr.sockaddr);
};

} // namespace ex
