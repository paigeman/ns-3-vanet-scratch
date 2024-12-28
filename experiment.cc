#include "filesystem"
#include "rsu-app.h"
#include "vehicle-app.h"

#include "ns3/core-module.h"
#include "ns3/internet-module.h"
#include "ns3/mobility-module.h"
#include "ns3/ns2-mobility-helper.h"
#include "ns3/ssid.h"
#include "ns3/yans-wifi-helper.h"

#include <sstream>

using namespace ns3;

NS_LOG_COMPONENT_DEFINE("experiment");

// 解析配置文件
static void
ParseConfigFile(const std::string& fileName,
                std::unordered_map<std::string, std::string>& configMap)
{
    std::ifstream file(fileName);
    if (!file.is_open())
    {
        std::cerr << "Error opening file: " << fileName << std::endl;
        return;
    }
    std::string line;
    while (std::getline(file, line))
    {
        if (line.empty())
        {
            continue;
        }
        // 忽略注释
        size_t pos_sharp = line.find_first_of('#');
        if (pos_sharp != std::string::npos)
        {
            line = line.substr(0, pos_sharp);
        }
        // 去除行首和行尾的空白字符
        while (!line.empty() && isblank(line[0]))
        {
            line.erase(0, 1); // Removes blank spaces at the beginning of the line
        }
        while (!line.empty() && isblank(line[line.size() - 1]))
        {
            line.erase(line.size() - 1, 1); // Removes blank spaces from at end of line
        }
        std::istringstream iss(line);
        std::string keyword;
        std::string key;
        std::string value;
        iss >> keyword;
        // 如果不是set打头直接跳过
        if (keyword != "set")
        {
            continue;
        }
        iss >> key >> value;
        configMap[key] = value;
    }
    file.close();
}

// OutputRsuLocation 输出RSU位置
static void
OutputRsuLocation(const NodeContainer& rsuNodes)
{
    for (uint32_t i = 0; i < rsuNodes.GetN(); ++i)
    {
        Ptr<MobilityModel> mobility = rsuNodes.Get(i)->GetObject<MobilityModel>();
        const Vector pos = mobility->GetPosition();
        std::ostringstream oss;
        oss << "RSU " << i << " position: (" << pos.x << ", " << pos.y << ", " << pos.z << ")"
            << std::endl;
        NS_LOG_DEBUG(oss.str());
    }
}

// run with:
// * environments: NS_LOG=experiment=debug:VehicleApp=debug:RsuApp=debug
// * program args:
// --traceFile=E:\GitHub\ns-3-dev-git\scratch\experiment\area1ns2mobility.tcl
// --configFile=E:\GitHub\ns-3-dev-git\scratch\experiment\area1ns2config.tcl
// --rsuNum=4
int
main(int argc, char* argv[])
{
    std::string traceFile;
    std::string configFile;

    uint32_t nodeNum;
    int rsuNum;
    double duration;
    double start;
    double rss{-80}; // -dBm

    // Parse command line attribute
    CommandLine cmd(__FILE__);
    cmd.AddValue("traceFile", "Ns2 movement trace file", traceFile);
    cmd.AddValue("configFile", "Configuration file", configFile);
    cmd.AddValue("rsuNum", "Number of RSUs", rsuNum);
    cmd.Parse(argc, argv);

    // Check command line arguments
    if (traceFile.empty() || configFile.empty() || rsuNum <= 0)
    {
        std::cout << "Usage of " << argv[0]
                  << " :\n\n"
                     "./ns3 run \"scratch/experiment/experiment"
                     " --traceFile=src/mobility/examples/default.ns_movements"
                     " --rsuNum=2"
                     " --configFile=/path/to/your/config\" \n\n"
                     "NOTE: ns2-traces-file could be an absolute or relative path. You could use "
                     "the file default.ns_movements\n"
                     "      included in the same directory of this example file.\n\n"
                     "NOTE 1: Number of RSUs must\n"
                     "        be a positive number.\n\n";
        return 0;
    }

    // 解析配置文件
    std::unordered_map<std::string, std::string> configMap;
    ParseConfigFile(configFile, configMap);
    // 设置节点数量
    nodeNum = std::stoi(configMap["opt(nn)"]);
    nodeNum = 1;
    // 开始时间
    start = std::stod(configMap["opt(start)"]);
    // 设置duration
    duration = std::stod(configMap["opt(stop)"]);

    // Create Ns2MobilityHelper with the specified trace log file as parameter
    auto ns2 = Ns2MobilityHelper(traceFile);

    // Must add the following two lines code
    // Create all vehicle nodes.
    NodeContainer stas;
    stas.Create(nodeNum);
    ns2.Install(); // configure movements for each node, while reading trace file

    // 配置RSU的移动模型
    double minX = std::stod(configMap["opt(min-x)"]);
    double minY = std::stod(configMap["opt(min-y)"]);
    double maxX = std::stod(configMap["opt(x)"]);
    double maxY = std::stod(configMap["opt(y)"]);
    double width = maxX - minX;
    double height = maxY - minY;

    // 最简单的实现，只有一个RSU，部署在区域的中心
    // 创建RSU节点
    NodeContainer rsuNodes;
    rsuNum = 1;
    rsuNodes.Create(rsuNum);
    // 自定义位置分配器，把RSU放至区域的中心
    Ptr<ListPositionAllocator> positionAllocator = CreateObject<ListPositionAllocator>();
    double posX = minX + width / 2;
    double posY = minY + height / 2;
    positionAllocator->Add(Vector(posX, posY, 0.0));

    MobilityHelper mobility;
    mobility.SetPositionAllocator(positionAllocator);
    // RSU的mobility model配置为静态的
    mobility.SetMobilityModel("ns3::ConstantPositionMobilityModel");
    mobility.Install(rsuNodes);
    OutputRsuLocation(rsuNodes);

    // 配置协议
    // 信道和物理层
    YansWifiChannelHelper channel = YansWifiChannelHelper::Default();
    channel.SetPropagationDelay("ns3::ConstantSpeedPropagationDelayModel");
    channel.AddPropagationLoss("ns3::FixedRssLossModel", "Rss", DoubleValue(rss));
    YansWifiPhyHelper phy;
    // This is one parameter that matters when using FixedRssLossModel
    // set it to zero; otherwise, gain will be added
    phy.Set("RxGain", DoubleValue(0));
    phy.SetChannel(channel.Create());
    // 数据链路层
    WifiMacHelper mac;
    Ssid ssid = Ssid("experiment");
    WifiHelper wifi;
    mac.SetType("ns3::AdhocWifiMac");
    NetDeviceContainer vehicleDevices = wifi.Install(phy, mac, stas);
    NetDeviceContainer rsuDevices = wifi.Install(phy, mac, rsuNodes);
    // 网络层
    InternetStackHelper stack;
    stack.Install(stas);
    stack.Install(rsuNodes);
    // 分配IP地址 一般来说够了
    Ipv4AddressHelper address;
    address.SetBase("10.1.1.0", "255.255.255.0");
    address.Assign(vehicleDevices);
    auto rsuInterfaces = address.Assign(rsuDevices);

    uint16_t rsuServerPort = 8080;
    uint16_t vehicleServerPort = 8081;

    // RSU应用
    for (int i = 0; i < rsuNum; ++i)
    {
        Ptr<RsuApp> rsuApp = CreateObject<RsuApp>(rsuServerPort, vehicleServerPort);
        rsuNodes.Get(i)->AddApplication(rsuApp);
        rsuApp->SetStartTime(Seconds(start));
        rsuApp->SetStopTime(Seconds(duration));
    }

    // 车辆应用
    for (uint32_t i = 0; i < nodeNum; i++)
    {
        Ptr<VehicleApp> vehicleApp =
            CreateObject<VehicleApp>(rsuServerPort, rsuInterfaces.GetAddress(0), vehicleServerPort);
        stas.Get(i)->AddApplication(vehicleApp);
        vehicleApp->SetStartTime(Seconds(start));
        vehicleApp->SetStopTime(Seconds(duration));
    }

    Ipv4GlobalRoutingHelper::PopulateRoutingTables();

    Simulator::Stop(Seconds(duration));
    Simulator::Run();
    Simulator::Destroy();
}
