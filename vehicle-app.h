//
// Created by fade on 2024/12/7.
//

#ifndef VEHICLE_APP_H
#define VEHICLE_APP_H
#include "unordered_set"

#include "ns3/application.h"
#include "ns3/node-container.h"
#include "ns3/socket.h"

namespace ns3
{
// 车辆应用
class VehicleApp : public Application
{
    // RSU的服务端port
    uint16_t m_rsuServerPort;
    // RSU的服务端IPv4地址
    Ipv4Address m_rsuIpAddress;
    // 车辆应用的服务端port
    uint16_t m_serverPort;
    // 车辆应用的服务端socket
    Ptr<Socket> m_serverSocket;
    // 车辆的IPv4地址
    Ipv4Address m_ipv4Address;
    void StartApplication() override;
    void StopApplication() override;
    // 处理来自RSU的消息
    void HandleRead(Ptr<Socket> socket) const;
    // 发送消息
    void Send();

  public:
    // Register this type，包含一些元数据
    static TypeId GetTypeId();
    VehicleApp(uint16_t rsuServerPort, const Ipv4Address& rsuIpAddress, uint16_t serverPort);
};
} // namespace ns3
#endif // VEHICLE_APP_H
