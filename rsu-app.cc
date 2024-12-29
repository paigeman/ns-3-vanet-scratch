//
// Created by fade on 2024/12/7.
//
#include "rsu-app.h"

#include "header-example.h"
#include "tag-example.h"
#include "typeinfo"

#include "ns3/ipv4.h"
#include "ns3/mobility-model.h"
#include "ns3/simulator.h"
#include "ns3/udp-socket-factory.h"

#include <cstdint>
#include <sstream>
using namespace ns3;

NS_LOG_COMPONENT_DEFINE("RsuApp");

TypeId
RsuApp::GetTypeId()
{
    static TypeId tid = TypeId("ns3::RsuApp").SetParent<Application>().SetGroupName("experiment");
    return tid;
}

void
RsuApp::StartApplication()
{
    m_serverSocket = Socket::CreateSocket(GetNode(), UdpSocketFactory::GetTypeId());
    if (const auto local = InetSocketAddress(Ipv4Address::GetAny(), m_serverPort);
        m_serverSocket->Bind(local))
    {
        NS_FATAL_ERROR("Failed to bind socket");
    }
    m_serverSocket->SetRecvCallback(MakeCallback(&RsuApp::HandleRead, this));
    m_ipv4Address = GetNode()->GetObject<Ipv4>()->GetAddress(1, 0).GetLocal();
}

void
RsuApp::StopApplication()
{
    if (m_serverSocket)
    {
        m_serverSocket->Close();
    }
}

RsuApp::RsuApp(const uint16_t serverPort, const uint16_t vehiclePort)
    : m_serverPort(serverPort),
      m_vehiclePort(vehiclePort)
{
}

void
RsuApp::HandleRead(Ptr<Socket> socket)
{
    Address from;
    if (Ptr<Packet> packet = socket->RecvFrom(from))
    {
        std::ostringstream oss;
        // 读取tag和header的顺序和写入时是相反的，最好不要变动
        TagExample tag;
        if (packet->FindFirstMatchingByteTag(tag))
        {
            auto ipv4Address = InetSocketAddress::ConvertFrom(from).GetIpv4();
            oss << Simulator::Now() << " RSU: RSU "
                << "at " << m_ipv4Address << " received a packet from Vehicle at " << ipv4Address;
            NS_LOG_DEBUG(oss.str());
            HeaderExample header;
            packet->RemoveHeader(header);
            oss.str("");
            oss.clear();
            oss << Simulator::Now() << " RSU: data in tag is " << tag.GetData()
                << ", data in header is " << header.GetData();
            NS_LOG_DEBUG(oss.str());
            // RSU上的客户端套接字
            Ptr<Socket> client = Socket::CreateSocket(GetNode(), UdpSocketFactory::GetTypeId());
            const InetSocketAddress remote(ipv4Address, m_vehiclePort);
            uint32_t data = 4399;
            auto buffer = reinterpret_cast<uint8_t*>(&data);
            packet = Create<Packet>(buffer, 4);
            client->SendTo(packet, 0, remote);
            oss.str("");
            oss.clear();
            oss << Simulator::Now() << " RSU: "
                << "RSU from " << m_ipv4Address << " sent a packet to Vehicle at " << ipv4Address;
            NS_LOG_DEBUG(oss.str());
            client->Close();
        }
    }
}
