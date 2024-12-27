//
// Created by fade on 2024/12/7.
//
#include "rsu-app.h"
#include "typeinfo"

#include "ns3/ipv4.h"
#include "ns3/mobility-model.h"
#include "ns3/simulator.h"
#include "ns3/udp-socket-factory.h"
using namespace ns3;

NS_LOG_COMPONENT_DEFINE ("RsuApp");

TypeId RsuApp::GetTypeId()
{
    static TypeId tid = TypeId("ns3::RsuApp")
        .SetParent<Application>().SetGroupName("experiment");
    return tid;
}

void RsuApp::StartApplication()
{
    m_serverSocket = Socket::CreateSocket(GetNode(), UdpSocketFactory::GetTypeId());
    if (const auto local = InetSocketAddress(Ipv4Address::GetAny(), m_serverPort);
        m_serverSocket->Bind(local))
    {
        NS_FATAL_ERROR("Failed to bind socket");
    }
    m_serverSocket->SetRecvCallback(MakeCallback(&RsuApp::HandleRead, this));
}

void RsuApp::StopApplication()
{
    if (m_serverSocket)
    {
        m_serverSocket->Close();
    }
}

RsuApp::
RsuApp(const uint16_t serverPort, const uint16_t vehiclePort)
    : m_serverPort(serverPort),
      m_vehiclePort(vehiclePort)
{
}

void RsuApp::HandleRead(Ptr<Socket> socket)
{
    Address from;
    if (Ptr<Packet> packet = socket->RecvFrom(from))
    {
        
    }
}
