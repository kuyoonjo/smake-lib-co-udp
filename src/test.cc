#include <cassert>
#include <co/co.h>
#include <co/fastring.h>
#include <ex/udp.h>
#include <iostream>

DEF_main(argc, argv) {
  ex::ipaddr<ex::v4> ia1("127.0.0.1", 8080);
  ex::ipaddr<ex::v4> ia2("127.0.0.1", 8081);
  ex::ipaddr<ex::v4> ia3("127.0.0.2", 8080);
  ex::ipaddr<ex::v4> ia4("127.0.0.1", 8080);
  ex::ipaddr<ex::v6> i6a1("::1", 8080);
  ex::ipaddr<ex::v6> i6a2("::1", 8081);
  ex::ipaddr<ex::v6> i6a3("::2", 8080);
  ex::ipaddr<ex::v6> i6a4("::1", 8080);

  assert(ia1 != ia2);
  assert(ia1 != ia3);
  assert(ia1 == ia4);

  assert(i6a1 != i6a2);
  assert(i6a1 != i6a3);
  assert(i6a1 == i6a4);

  std::cout << "IP Pass!" << std::endl;

  go([] {
    ex::udp<ex::v6> u1;
    ex::ipaddr<ex::v6> i6a1("::1", 8081);
    u1.bind(i6a1);

    while (1) {
      u1.recvfrom();
      auto rb = u1.recv_buffer();
      std::cout << rb << std::endl;
      u1.sendto(rb, u1.rmt_ipaddr());
      u1.sendto({ 0x31, 0x32, 0x33 }, u1.rmt_ipaddr());
      u1.sendto("456", u1.rmt_ipaddr());
      u1.sendto(fastring("789"), u1.rmt_ipaddr());
    }
  });

  ex::udp<ex::v4> u1;
  u1.bind(ia1);

  while (1) {
    u1.recvfrom();
    auto rb = u1.recv_buffer();
    std::cout << rb << std::endl;
    u1.sendto(rb, u1.rmt_ipaddr());
    u1.sendto({2, 1, 1}, u1.rmt_ipaddr());
    u1.sendto("456", u1.rmt_ipaddr());
  }
  return 0;
}