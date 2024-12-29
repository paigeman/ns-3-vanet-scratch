//
// Created by fade on 2024/12/7.
//
#include "vehicle-app.h"

#include "header-example.h"
#include "tag-example.h"

#include "ns3/ipv4.h"
#include "ns3/mobility-model.h"
#include "ns3/simulator.h"
#include "ns3/udp-socket-factory.h"

#include <cstdint>
using namespace ns3;

NS_LOG_COMPONENT_DEFINE("VehicleApp");

TypeId
VehicleApp::GetTypeId()
{
    static TypeId tid =
        TypeId("ns3::VehicleApp").SetParent<Application>().SetGroupName("experiment");
    return tid;
}

void
VehicleApp::StartApplication()
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
    // 调度发送
    Simulator::Schedule(Seconds(0), &VehicleApp::Send, this);
    m_ipv4Address = GetNode()->GetObject<Ipv4>()->GetAddress(1, 0).GetLocal();
}

void
VehicleApp::StopApplication()
{
    if (m_serverSocket)
    {
        m_serverSocket->Close();
    }
}

VehicleApp::VehicleApp(const uint16_t rsuServerPort,
                       const Ipv4Address& rsuIpAddress,
                       const uint16_t serverPort)
    : m_rsuServerPort(rsuServerPort),
      m_rsuIpAddress(rsuIpAddress),
      m_serverPort(serverPort)
{
}

void
VehicleApp::HandleRead(Ptr<Socket> socket) const
{
    Address from;
    if (Ptr<Packet> packet = socket->RecvFrom(from))
    {
        auto ipv4Address = InetSocketAddress::ConvertFrom(from).GetIpv4();
        std::ostringstream oss;
        oss << Simulator::Now() << " Vehicle: Vehicle at " << m_ipv4Address
            << " received a packet from RSU at " << ipv4Address;
        NS_LOG_DEBUG(oss.str());
        auto* buffer = new uint8_t[2];
        packet->CopyData(buffer, 4);
        oss.str("");
        oss.clear();
        oss << Simulator::Now() << " Vehicle: data in buffer is "
            << *reinterpret_cast<uint32_t*>(buffer);
        NS_LOG_DEBUG(oss.str());
    }
}

void
VehicleApp::Send()
{
    // 客户端套接字要使用时再创建
    Ptr<Socket> socket = Socket::CreateSocket(GetNode(), UdpSocketFactory::GetTypeId());
    const InetSocketAddress remote(m_rsuIpAddress, m_rsuServerPort);
    HeaderExample header(7758);
    TagExample tag(258);
    // 创建要发送的包
    Ptr<Packet> packet = Create<Packet>();
    // 添加header和tag的顺序最好不要变动
    // 把header添加进包里
    packet->AddHeader(header);
    // 添加tag
    packet->AddByteTag(tag);
    // 发送给RSU
    socket->SendTo(packet, 0, remote);
    std::ostringstream oss;
    oss << Simulator::Now() << " Vehicle: "
        << "Vehicle "
        << "from " << m_ipv4Address << " sent a packet to RSU "
        << "at " << m_rsuIpAddress;
    NS_LOG_DEBUG(oss.str());
    socket->Close();
}
