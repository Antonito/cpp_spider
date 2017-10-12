#include "MacAddr.h"
#include <iostream>
#include <fstream>

#include <cstring>
#include <memory>

#if defined _WIN32
#include <SDKDDKVer.h>
#include <stdio.h>
#include <winsock2.h>
#include <iphlpapi.h>
#include <Assert.h>
#include <memory>
#include <iostream>
#include <new>
#include <string>
#include <iostream>
#include <WS2tcpip.h>
#pragma comment(lib, "IPHLPAPI.lib")
#pragma comment(lib, "ws2_32.lib")
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

void MacAddress::get(spider::network::tcp::MacAddrArray &addr)
{
  std::array<std::uint8_t, 6> res;

  addr.fill(0);
  if (!getAddress(res))
    {
      addr = res;
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
  PIP_ADAPTER_ADDRESSES       pAddresses = nullptr;
  ULONG                       outBufLen = 15000;
  ULONG                       flags = GAA_FLAG_INCLUDE_PREFIX;
  ULONG                       family = AF_INET;
  PIP_ADAPTER_ADDRESSES       pCurrAddresses;
  PIP_ADAPTER_UNICAST_ADDRESS pUnicast = nullptr;
  std::array<char, 100> buff;
  DWORD bufflen = 100;
  pAddresses = (IP_ADAPTER_ADDRESSES *)::operator new(outBufLen);

  if (GetAdaptersAddresses(family, flags, nullptr, pAddresses, &outBufLen) ==
      ERROR_BUFFER_OVERFLOW)
    {
      delete pAddresses;
      return (-1);
    }
  pCurrAddresses = pAddresses;
  while (pCurrAddresses)
    {
      pUnicast = pCurrAddresses->FirstUnicastAddress;
      if (pUnicast != nullptr)
	{
	  for (int i = 0; pUnicast != nullptr; i++)
	    {
	      if (pUnicast->Address.lpSockaddr->sa_family == AF_INET)
		{
		  sockaddr_in *sa_in =
		      (sockaddr_in *)pUnicast->Address.lpSockaddr;
		  std::string ip(inet_ntop(AF_INET, &(sa_in->sin_addr),
		                           buff.data(), bufflen));
		  if (ip != "127.0.0.1")
		    {
		      if (pCurrAddresses->PhysicalAddressLength != 0)
			{
			  for (int j = 0;
			       j < pCurrAddresses->PhysicalAddressLength; j++)
			    {
			      result[j] = pCurrAddresses->PhysicalAddress[j];
			    }
			  delete pAddresses;
			  return (0);
			}
		    }
		}
	      pUnicast = pUnicast->Next;
	    }
	}
      pCurrAddresses = pCurrAddresses->Next;
    }
  delete pAddresses;
  return (-1);
}

#elif defined __APPLE__

std::int32_t MacAddress::getAddressMacOS(std::array<std::uint8_t, 6> &result)
{
  std::string         interfaceName;
  std::string         ip;
  std::string         macAddress;
  struct ifaddrs *    ifap, *ifa;
  struct sockaddr_in *sa;
  char *              addr;
  std::array<int, 6> mib;
  size_t              len;
  char *              buf;
  unsigned char *     ptr;
  struct if_msghdr *  ifm;
  struct sockaddr_dl *sdl;

  // gettings active interface
  getifaddrs(&ifap);
  for (ifa = ifap; ifa; ifa = ifa->ifa_next)
    {
      if (ifa->ifa_addr->sa_family == AF_INET)
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

  // getting Mac address
  mib[0] = CTL_NET;
  mib[1] = AF_ROUTE;
  mib[2] = 0;
  mib[3] = AF_LINK;
  mib[4] = NET_RT_IFLIST;
  if ((mib[5] = static_cast<int>(if_nametoindex(interfaceName.c_str()))) == 0)
    {
      return (-1);
    }
  if (sysctl(mib.data(), 6, nullptr, &len, nullptr, 0) < 0)
    {
      return (-1);
    }
  if ((buf = new char[len]) == nullptr)
    {
      return (-1);
    }
  if (sysctl(mib.data(), 6, buf, &len, nullptr, 0) < 0)
    {
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
  std::string         interfaceName;
  std::string         ip;
  std::string         macAddress;
  struct ifaddrs *    ifap, *ifa;
  struct sockaddr_in *sa;
  char *              addr;

  // gettings active interface
  getifaddrs(&ifap);
  for (ifa = ifap; ifa; ifa = ifa->ifa_next)
    {
      if (ifa->ifa_addr->sa_family == AF_INET)
	{
	  sa = (struct sockaddr_in *)ifa->ifa_addr;
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

  // getting mac address
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
