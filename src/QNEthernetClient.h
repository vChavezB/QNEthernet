// SPDX-FileCopyrightText: (c) 2021 Shawn Silverman <shawn@pobox.com>
// SPDX-License-Identifier: MIT

// QNEthernetClient.h defines the TCP client interface.
// This file is part of the QNEthernet library.

#ifndef QNE_ETHERNETCLIENT_H_
#define QNE_ETHERNETCLIENT_H_

// C++ includes
#include <memory>

#include <Client.h>
#include <IPAddress.h>
#include <Print.h>
#include <WString.h>
#include <lwip/ip_addr.h>
#include "ConnectionHolder.h"

namespace qindesign {
namespace network {

class EthernetServer;

class EthernetClient final : public Client {
 public:
  EthernetClient();
  ~EthernetClient();

  // Ideally, we only want move semantics because the state should only be owned
  // by one client at a time. However, user code may need to copy and the writer
  // may not get that compiler errors are because std::move was required.

  EthernetClient(const EthernetClient &) = default;
  EthernetClient &operator=(const EthernetClient &) = default;
  EthernetClient(EthernetClient &&other) = default;
  EthernetClient &operator=(EthernetClient &&other) = default;

  int connect(IPAddress ip, uint16_t port) override;
  int connect(const char *host, uint16_t port) override;

  uint8_t connected() override;
  operator bool() override;

  void setConnectionTimeout(uint16_t timeout);

  void stop() override;

  uint16_t localPort() const;
  IPAddress remoteIP() const;
  uint16_t remotePort() const;

  // Bring Print::write functions into scope
  using Print::write;

  size_t write(uint8_t b) override;
  size_t write(const uint8_t *buf, size_t size) override;
  int availableForWrite() override;
  void flush() override;

  int available() override;
  int read() override;
  int read(uint8_t *buf, size_t size) override;
  int peek() override;

 private:
  // Set up an already-connected client. If the holder is NULL then a new
  // unconnected client will be created.
  EthernetClient(std::shared_ptr<ConnectionHolder> holder);

  static void dnsFoundFunc(const char *name, const ip_addr_t *ipaddr,
                           void *callback_arg);

  // Connection state
  uint16_t connTimeout_;

  // DNS lookups
  String lookupHost_;   // For matching DNS lookups
  IPAddress lookupIP_;  // Set by a DNS lookup
  volatile bool lookupFound_;

  std::shared_ptr<ConnectionHolder> conn_;

  friend class EthernetServer;
};

}  // namespace network
}  // namespace qindesign

#endif  // QNE_ETHERNETCLIENT_H_
