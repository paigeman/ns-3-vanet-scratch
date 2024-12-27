//
// Created by fade on 2024/12/7.
//
#include "vehicle-app.h"

#include "ns3/ipv4.h"
#include "ns3/mobility-model.h"
#include "ns3/simulator.h"
#include "ns3/udp-socket-factory.h"
using namespace ns3;

NS_LOG_COMPONENT_DEFINE ("VehicleApp");

TypeId VehicleApp::GetTypeId()
{
    static TypeId tid = TypeId("ns3::VehicleApp")
        .SetParent<Application>().SetGroupName("experiment");
    return tid;
}

void VehicleApp::StartApplication()
{
    // 为了时延考虑，使用UDP
    m_serverSocket = Socket::CreateSocket(GetNode(), UdpSocketFactory::GetTypeId());
    // 地址可改为车辆的IP地址
    const auto local = InetSocketAddress(Ipv4Address::GetAny(), m_serverPort);
    if (m_serverSocket->Bind(local))
    {
        NS_FATAL_ERROR("Failed to bind socket");
    }
    m_serverSocket->SetRecvCallback(MakeCallback(&VehicleApp::HandleRead, this));
}

void VehicleApp::StopApplication()
{
    if (m_serverSocket)
    {
        m_serverSocket->Close();
    }
}

VehicleApp::
VehicleApp(const uint16_t rsuServerPort,
           const NodeContainer& rsuNodes,
           const uint16_t m_serverPort)
    : m_rsuServerPort(rsuServerPort),
      m_rsuNodes(rsuNodes),
      m_serverPort(m_serverPort)
{
}

void VehicleApp::HandleRead(Ptr<Socket> socket) const{
    Address from;
    if (Ptr<Packet> packet = socket->RecvFrom(from))
    {
        
    }
}


