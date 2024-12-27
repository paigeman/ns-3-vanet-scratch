#include "header-example.h"
using namespace ns3;

TypeId HeaderExample::GetTypeId() {
    static TypeId tid = TypeId("ns3::HeaderExample").SetParent<Header>()
        .SetGroupName("experiment");
    return tid;
}
