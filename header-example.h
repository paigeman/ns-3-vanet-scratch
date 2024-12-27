//
// Created by fade on 2024/12/27.
//
#ifndef HEADER_EXAMPLE_H
#define HEADER_EXAMPLE_H
#include "ns3/header.h"
namespace ns3
{
class HeaderExample : public Header {
  uint32_t m_data;
public:
  // Register this type，包含一些元数据
  static TypeId GetTypeId();
  // Inherited from header:
  TypeId GetInstanceTypeId() const override;
  // 序列化与反序列化相关
  uint32_t GetSerializedSize() const override;
  void Serialize(Buffer::Iterator start) const override;
  uint32_t Deserialize(Buffer::Iterator start) override;
  void Print(std::ostream& os) const override;
  // 构造器
  HeaderExample() = default;
  HeaderExample(int data);
  // getter和setter
  uint32_t GetData() { return m_data; }
  void SetData(uint32_t data) { m_data = data; }  
};
} // namespace ns3
#endif