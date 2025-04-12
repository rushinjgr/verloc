#include "NetIntLooper.h"
#include <ifaddrs.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <cstring>
#include <iostream>

NetIntLooper::NetIntLooper()
    : BaseLooper("NetInt", std::chrono::seconds(10)) {}

void NetIntLooper::run() {
    // Optionally print a header
    std::cout << "[" << name << "] Loop running every " << interval.count()
              << "ms... Press Ctrl+C to exit." << std::endl;
    
    // NetIntLooper-specific network enumeration logic.
    while (!exitFlag) {
        std::this_thread::sleep_for(interval); // Mimic a ticker.
        std::cout << "[" << name << "] Enumerating network interfaces..." << std::endl;
        
        struct ifaddrs *interfaces = nullptr;
        if (getifaddrs(&interfaces) == -1) {
            std::cerr << "[" << name << "] Error retrieving interfaces: " << strerror(errno) << std::endl;
            continue;
        }
        for (struct ifaddrs *iface = interfaces; iface != nullptr; iface = iface->ifa_next) {
            if (iface->ifa_addr == nullptr) continue; // Skip if no address.
            std::cout << "[" << name << "] Interface: " << iface->ifa_name << std::endl;
            if (iface->ifa_addr->sa_family == AF_INET) { // IPv4.
                char ip[INET_ADDRSTRLEN];
                auto *addr = reinterpret_cast<struct sockaddr_in *>(iface->ifa_addr);
                inet_ntop(AF_INET, &(addr->sin_addr), ip, INET_ADDRSTRLEN);
                std::cout << "[" << name << "] - Address: " << ip << std::endl;
            } else if (iface->ifa_addr->sa_family == AF_INET6) { // IPv6.
                char ip[INET6_ADDRSTRLEN];
                auto *addr = reinterpret_cast<struct sockaddr_in6 *>(iface->ifa_addr);
                inet_ntop(AF_INET6, &(addr->sin6_addr), ip, INET6_ADDRSTRLEN);
                std::cout << "[" << name << "] - Address: " << ip << std::endl;
            }
        }
        freeifaddrs(interfaces); // Free allocated memory.
    }
    std::cout << "[" << name << "] NetInt received signal to exit. Cleaning up..." << std::endl;
    teardown();
}
