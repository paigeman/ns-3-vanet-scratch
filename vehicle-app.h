//
// Created by fade on 2024/12/7.
//

#ifndef VEHICLE_APP_H
#define VEHICLE_APP_H
#include "ns3/node-container.h"
#include "unordered_set"

#include "ns3/application.h"
#include "ns3/socket.h"

namespace ns3
{
// 车辆应用
class VehicleApp : public Application
{
  // RSU 服务端port
  uint16_t m_rsuServerPort;
  // RSU节点
  NodeContainer m_rsuNodes;
  // 服务端port
  uint16_t m_serverPort;
  // 服务端socket
  Ptr<Socket> m_serverSocket;
  void StartApplication() override;
  void StopApplication() override;
  // 处理来自RSU的消息
  void HandleRead(Ptr<Socket> socket) const;
public:
  // Register this type，包含一些元数据
  static TypeId GetTypeId();
  VehicleApp(uint16_t rsuServerPort, const NodeContainer& rsuNodes, uint16_t m_serverPort, const Time& m_delay);
};
}
#endif //VEHICLE_APP_H
