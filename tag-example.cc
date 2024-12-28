//
// Created by fade on 2024/12/28.
//
#include "tag-example.h"

#include <cstdint>
using namespace ns3;

TypeId
TagExample::GetTypeId()
{
    static TypeId tid = TypeId("ns3::TagExample").SetParent<Tag>().SetGroupName("experiment");
    return tid;
}

TypeId
TagExample::GetInstanceTypeId() const
{
    return GetTypeId();
}

uint32_t
TagExample::GetSerializedSize() const
{
    // uint32_t is 4 bytes
    return 4;
}

void
TagExample::Serialize(TagBuffer i) const
{
    i.WriteU32(m_data);
}

void
TagExample::Deserialize(TagBuffer i)
{
    m_data = i.ReadU32();
}

void
TagExample::Print(std::ostream& os) const
{
    os << "data: " << m_data << "\n";
}

TagExample::TagExample(uint32_t data)
    : m_data(data)
{
}
