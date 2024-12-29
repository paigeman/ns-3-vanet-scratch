//
// Created by fade on 2024/12/7.
//

#ifndef RSU_APP_H
#define RSU_APP_H

#include "ns3/application.h"
#include "ns3/socket.h"

#include <unordered_map>

namespace ns3
{
// RSU应用
class RsuApp : public Application
{
    // RSU应用的服务端socket
    Ptr<Socket> m_serverSocket;
    // RSU应用的服务端port
    uint16_t m_serverPort;
    // 车辆应用的服务端端口
    uint16_t m_vehiclePort;
    // RSU的IPv4地址
    Ipv4Address m_ipv4Address;
    void StartApplication() override;
    void StopApplication() override;
    void HandleRead(Ptr<Socket> socket);

  public:
    // Register this type，包含一些元数据
    static TypeId GetTypeId();
    RsuApp(uint16_t serverPort, uint16_t vehiclePort);
};
} // namespace ns3
#endif // RSU_APP_H
