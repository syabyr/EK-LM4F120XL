# driverlib CMake file
project(lwip)

# find . -name '*.c' | sort -u
add_library(${PROJECT_NAME}
./src/core/ipv4/autoip.c
./src/core/ipv4/dhcp.c
./src/core/ipv4/etharp.c
./src/core/ipv4/icmp.c
./src/core/ipv4/igmp.c
./src/core/ipv4/ip4_addr.c
./src/core/ipv4/ip4.c
./src/core/ipv4/ip4_frag.c

./src/core/ipv6/dhcp6.c
./src/core/ipv6/ethip6.c
./src/core/ipv6/icmp6.c
./src/core/ipv6/inet6.c
./src/core/ipv6/ip6_addr.c
./src/core/ipv6/ip6.c
./src/core/ipv6/ip6_frag.c
./src/core/ipv6/mld6.c
./src/core/ipv6/nd6.c

./src/core/altcp.c
./src/core/altcp_alloc.c
./src/core/altcp_tcp.c
./src/core/def.c
./src/core/dns.c
./src/core/inet_chksum.c
./src/core/init.c
./src/core/ip.c
./src/core/mem.c
./src/core/memp.c
./src/core/netif.c
./src/core/pbuf.c
./src/core/raw.c
./src/core/stats.c
./src/core/sys.c
./src/core/tcp_in.c
./src/core/tcp_out.c
./src/core/tcp.c
./src/core/timeouts.c
./src/core/udp.c


./src/netif/ethernet.c
./src/netif/slipif.c


./src/apps/http/httpd.c
./src/apps/http/fs.c)

target_include_directories(${PROJECT_NAME} PUBLIC ../../
                                ./src/include
                                ${WORKING_BASE_DIRECTORY}
                                ${WORKING_BASE_DIRECTORY}/inc/
                                )

#target_link_libraries(${PROJECT_NAME} device)