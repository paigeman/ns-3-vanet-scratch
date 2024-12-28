//
// Created by fade on 2024/12/28.
//
#ifndef TAG_EXAMPLE_H
#define TAG_EXAMPLE_H
#include "ns3/tag.h"

#include <cstdint>

namespace ns3
{
class TagExample : public Tag
{
    uint32_t m_data;

  public:
    // Register this type，包含一些元数据
    static TypeId GetTypeId();
    // Inherited from header:
    TypeId GetInstanceTypeId() const override;
    // 序列化与反序列化相关
    uint32_t GetSerializedSize() const override;
    void Serialize(TagBuffer i) const override;
    void Deserialize(TagBuffer i) override;
    void Print(std::ostream& os) const override;
    // 构造器
    TagExample() = default;
    TagExample(uint32_t data);

    // getter和setter
    void SetData(uint32_t data)
    {
        m_data = data;
    }

    uint32_t GetData() const
    {
        return m_data;
    }
};
} // namespace ns3

#endif
