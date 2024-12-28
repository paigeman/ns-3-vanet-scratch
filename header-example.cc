#include "header-example.h"
using namespace ns3;

TypeId HeaderExample::GetTypeId() {
    static TypeId tid = TypeId("ns3::HeaderExample").SetParent<Header>()
        .SetGroupName("experiment");
    return tid;
}

TypeId HeaderExample::GetInstanceTypeId() const {
    return GetTypeId();
}

uint32_t HeaderExample::GetSerializedSize() const {
    // uint32_t is 4 bytes
    return 4;
}

void HeaderExample::Serialize(Buffer::Iterator start) const {
    start.WriteHtonU32(m_data);
}

uint32_t HeaderExample::Deserialize(Buffer::Iterator start) {
    Buffer::Iterator i = start;
    m_data = i.ReadNtohU32();
    return i.GetDistanceFrom(start);
}

void HeaderExample::Print(std::ostream& os) const {
    os << "data: " << m_data << "\n";
}

HeaderExample::HeaderExample(uint32_t data) : m_data(data) {}