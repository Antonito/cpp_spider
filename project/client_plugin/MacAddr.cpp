#include "MacAddr.h"
#include <iostream>
#include <fstream>

#include <cstring>

#if defined _WIN32
#include <Windows.h>
#include <SDKDDKVer.h>
#include <stdio.h>
#include <stdlib.h>
#include <tchar.h>
#include <Iphlpapi.h>
#include <Assert.h>
#pragma comment(lib, "Iphlpapi.lib")
#pragma comment(lib, "rpcrt4.lib")
#elif defined __linux__
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <net/if.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <ifaddrs.h>
#include <stdio.h>
#include <stropts.h>
#elif defined __APPLE__
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <sys/sysctl.h>
#include <net/if.h>
#include <net/if_dl.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <ifaddrs.h>
#endif

void MacAddress::get(std::string &addr)
{
  std::array<std::uint8_t, 6> res;

  addr = "";
  if (!getAddress(res))
    {
      std::array<char, 64 * 8> tmp;

      std::snprintf(tmp.data(), sizeof(tmp), "%02X:%02X:%02X:%02X:%02X:%02X",
                    res[0], res[1], res[2], res[3], res[4], res[5]);
      addr = tmp.data();
    }
}

std::int32_t MacAddress::getAddress(std::array<std::uint8_t, 6> &result)
{
  std::int32_t rc = -1;

  result.fill(0);
#if defined _WIN32
  rc = getAddressWindows(result);
#elif defined __APPLE__
  rc = getAddressMacOS(result);
#elif defined __linux__
  rc = getAddressLinux(result);
#endif
  return rc;
}

#if defined  _WIN32
std::int32_t MacAddress::getAddressWindows(std::array<std::uint8_t, 6> &result)
{
  DWORD dwBufLen = sizeof(PIP_ADAPTER_INFO);
  IP_ADAPTER_INFO *AdapterInfo = new IP_ADAPTER_INFO();

  if (AdapterInfo == NULL)
  {
	  return (-1);
  }
  if (GetAdaptersInfo(AdapterInfo, &dwBufLen) == ERROR_BUFFER_OVERFLOW)
  {
	  AdapterInfo = new IP_ADAPTER_INFO();
    if (AdapterInfo == NULL)
    {
		return (-1);
    }
  }
  if (GetAdaptersInfo(AdapterInfo, &dwBufLen) == ERROR_SUCCESS)
  {
    PIP_ADAPTER_INFO pAdapterInfo = AdapterInfo;
    while (pAdapterInfo)
    {
      if (pAdapterInfo->IpAddressList.IpAddress.String != "127.0.0.1")
      {
        for (std::uint32_t count = 0; count < 6; count++)
        {
          result[count] = pAdapterInfo->Address[count];;
        }
        return (0);
        break;
      }
    }
  }
  return (-1);
}

#elif defined __APPLE__

std::int32_t MacAddress::getAddressMacOS(std::array<std::uint8_t, 6> &result)
{
  std::string interfaceName;
  std::string ip;
  std::string macAddress;
  struct ifaddrs *ifap, *ifa;
  struct sockaddr_in *sa;
  char *addr;
  std::array<int, 6> mib;
  size_t len;
  char *buf;
  unsigned char *ptr;
  struct if_msghdr *ifm;
  struct sockaddr_dl *sdl;

  //gettings active interface
  getifaddrs (&ifap);
  for (ifa = ifap; ifa; ifa = ifa->ifa_next)
  {
    if (ifa->ifa_addr->sa_family==AF_INET)
    {
      sa = reinterpret_cast<struct sockaddr_in *>(ifa->ifa_addr);
      addr = inet_ntoa(sa->sin_addr);
      interfaceName = ifa->ifa_name;
      ip = addr;
      if (ip != "127.0.0.1")
      {
        break;
      }
    }
  }
  if (!ifa)
  {
    return (-1);
  }
  freeifaddrs(ifap);

  //getting Mac address
  mib[0] = CTL_NET;
  mib[1] = AF_ROUTE;
  mib[2] = 0;
  mib[3] = AF_LINK;
  mib[4] = NET_RT_IFLIST;
  if ((mib[5] = static_cast<int>(if_nametoindex(interfaceName.c_str()))) == 0) {
    return (-1);
  }
  if (sysctl(mib.data(), 6, NULL, &len, NULL, 0) < 0) {
    return (-1);
  }
  if ((buf = new char[len]) == NULL) {
    return (-1);
  }
  if (sysctl(mib.data(), 6, buf, &len, NULL, 0) < 0) {
    return (-1);
  }
  ifm = reinterpret_cast<struct if_msghdr *>(buf);
  sdl = reinterpret_cast<struct sockaddr_dl *>(ifm + 1);
  ptr = reinterpret_cast<unsigned char *>(LLADDR(sdl));
  for (std::uint32_t count = 0; count < 6; count++)
  {
    result[count] = *(ptr + count);
  }
  return (0);
}

#elif defined __linux__

std::int32_t MacAddress::getAddressLinux(std::array<std::uint8_t, 6> &result)
{
  std::string interfaceName;
  std::string ip;
  std::string macAddress;
  struct ifaddrs *ifap, *ifa;
  struct sockaddr_in *sa;
  char *addr;

  //gettings active interface
  getifaddrs (&ifap);
  for (ifa = ifap; ifa; ifa = ifa->ifa_next)
  {
    if (ifa->ifa_addr->sa_family==AF_INET)
    {
      sa = (struct sockaddr_in *) ifa->ifa_addr;
      addr = inet_ntoa(sa->sin_addr);
      interfaceName = ifa->ifa_name;
      ip = addr;
      if (ip != "127.0.0.1")
      {
        break;
      }
    }
  }
  if (!ifa)
  {
    return (-1);
  }
  freeifaddrs(ifap);
  
  //getting mac address
  std::ifstream input("/sys/class/net/" + interfaceName + "/address");
  getline(input, macAddress);
  for (int i = 0; i < 6; i++)
  {
    char *t;  
    result[i] = std::strtol(macAddress.substr(i * 3, 2).c_str(), &t, 16);
  }
  return (0);
}
#endif
