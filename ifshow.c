#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ifaddrs.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

void afficher_interfaces() {
    struct ifaddrs *ifaddr, *ifa;
    char addr[INET6_ADDRSTRLEN];

    if (getifaddrs(&ifaddr) == -1) {
        perror("getifaddrs");
        exit(EXIT_FAILURE);
    }

    for (ifa = ifaddr; ifa != NULL; ifa = ifa->ifa_next) {
        if (ifa->ifa_addr == NULL) continue;

        int family = ifa->ifa_addr->sa_family;

        if (family == AF_INET || family == AF_INET6) {
            void *addr_ptr;
            int prefix = (family == AF_INET) ? 32 : 64; // Hypothèse des préfixes

            if (family == AF_INET) {
                addr_ptr = &((struct sockaddr_in *)ifa->ifa_addr)->sin_addr;
            } else {
                addr_ptr = &((struct sockaddr_in6 *)ifa->ifa_addr)->sin6_addr;
            }

            inet_ntop(family, addr_ptr, addr, sizeof(addr));
            printf("%s: %s/%d\n", ifa->ifa_name, addr, prefix);
        }
    }

    freeifaddrs(ifaddr);
}

void afficher_interface_unique(const char *ifname) {
    struct ifaddrs *ifaddr, *ifa;
    char addr[INET6_ADDRSTRLEN];

    if (getifaddrs(&ifaddr) == -1) {
        perror("getifaddrs");
        exit(EXIT_FAILURE);
    }

    for (ifa = ifaddr; ifa != NULL; ifa = ifa->ifa_next) {
        if (ifa->ifa_addr == NULL || strcmp(ifa->ifa_name, ifname) != 0)
            continue;

        int family = ifa->ifa_addr->sa_family;
        if (family == AF_INET || family == AF_INET6) {
            void *addr_ptr;
            int prefix = (family == AF_INET) ? 32 : 64;

            if (family == AF_INET) {
                addr_ptr = &((struct sockaddr_in *)ifa->ifa_addr)->sin_addr;
            } else {
                addr_ptr = &((struct sockaddr_in6 *)ifa->ifa_addr)->sin6_addr;
            }

            inet_ntop(family, addr_ptr, addr, sizeof(addr));
            printf("%s: %s/%d\n", ifa->ifa_name, addr, prefix);
        }
    }

    freeifaddrs(ifaddr);
}

int main(int argc, char *argv[]) {
    if (argc == 1) {
        fprintf(stderr, "Usage: %s -a | -i ifname\n", argv[0]);
        return EXIT_FAILURE;
    }

    if (strcmp(argv[1], "-a") == 0) {
        afficher_interfaces();
    } else if (strcmp(argv[1], "-i") == 0) {
        if (argc != 3) {
            fprintf(stderr, "Usage: %s -i ifname\n", argv[0]);
            return EXIT_FAILURE;
        }
        afficher_interface_unique(argv[2]);
    } else {
        fprintf(stderr, "Option inconnue : %s\n", argv[1]);
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
