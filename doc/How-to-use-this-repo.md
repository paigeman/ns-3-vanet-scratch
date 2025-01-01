# How to use this repo

## Git clone this project in the `scratch` directory of ns-3 project

```shell
cd /path/to/ns-3/project/scratch
git clone https://github.com/paigeman/ns-3-vanet-scratch.git
```

## Rebuild the ns-3 project

In Visual Studio Code, you can achieve it by doing this:

![image](resources/ca5ad227a5b432afd9576746c1457889aa760265e0e47feb293ae8d917d9c6e5.png)

## How to configure SUMO related parameters

For some parameters like simulation area, vehicle density, and vehicle speed, these are provided by Eclipse SUMO.

SUMO offers a script named `traceExporter.py` to generates a ns2 mobility file and generates a ns2 configuration file. You can follow Ref. [1-17] to generate necessary input files for this repo.

For example, if you use `traceExporter.py` like this:

```shell
python traceExporter.py --fcd-input sumoTrace.xml --ns2mobility-output ns2mobility.tcl --ns2config-output config.tcl
```

This repo need `ns2mobility.tcl` and `config.tcl` .

Then you can replace the values of `traceFile` and `configFile` in `.vscode/settings.json` file:

![image](resources/308eff8b68ec768aca1af19100e66782be8d4427e76e7aa3ac4b6e4ec78f956f.png)

The part where the parameters `traceFile` and `configFile` are defined and parsed in the code `experiment.cc` is:

```cpp
// experiment.cc
std::string traceFile;
std::string configFile;
CommandLine cmd(__FILE__);
cmd.AddValue("traceFile", "Ns2 movement trace file", traceFile);
cmd.AddValue("configFile", "Configuration file", configFile);
cmd.Parse(argc, argv);
```

After the program receives the values of these two parameters, `traceFile` can be processed using the file parsing methods provided by ns-3. For `configFile`, however, you need to define your own corresponding parsing method.

Here, we first introduce how `configFile` is parsed, as most parameters are set by parsing this file, while `traceFile` only stores the movement trajectories of vehicles.

```cpp
// experiment.cc

uint32_t nodeNum;
double duration;
double start;
std::unordered_map<std::string, std::string> configMap;
ParseConfigFile(configFile, configMap);
// set the number of nodes
nodeNum = std::stoi(configMap["opt(nn)"]);
nodeNum = 1;
// start time of simulation
start = std::stod(configMap["opt(start)"]);
// end time of simulation
duration = std::stod(configMap["opt(stop)"]);

// parse configFile
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
        // ignore comments
        size_t pos_sharp = line.find_first_of('#');
        if (pos_sharp != std::string::npos)
        {
            line = line.substr(0, pos_sharp);
        }
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
        // if line is not starting with set, skip this line
        if (keyword != "set")
        {
            continue;
        }
        iss >> key >> value;
        configMap[key] = value;
    }
    file.close();
}
```

In the code above, the `ParseConfigFile` section should not require any modifications unless your requirements involve lines that do not start with `set` or cases that are not accounted for in this repository. Additionally, note that the `nodeNum` here is set to 1 after being retrieved from the configuration file. This is because, for simplicity, this repository only demonstrates communication between one vehicle and one RSU.

The `duration` can also be considered the simulation end time, as the start time is generally 0, making the end time equal to the duration.

The parsing of `traceFile` will not be elaborated further, as this process is coupled with the configuration of the mobility model.

## How to set the number of RSUs

Originally, the plan was to set the number of RSUs by passing a command-line argument to the program. However, considering that having multiple RSUs would require designing a more reasonable placement strategy, the decision was made to simplify the scenario. Because a reasonable RSU placement strategy would not only involve determining optimal RSU positions but also selecting an appropriate communication propagation model.

Therefore, although `rsuNum` is defined in `experiment.cc` to accept command-line arguments, its value is forcibly set to `1` in the subsequent code, meaning there will always be only one RSU.

In other words, you are required to pass the `rsuNum` parameter via the command line, but the value you provide will have no effect. Users are free to modify this logic if desired.

```cpp
// experiment.cc
int rsuNum;
cmd.AddValue("rsuNum", "Number of RSUs", rsuNum);
cmd.Parse(argc, argv);
rsuNum = 1;
```

## How to configure mobility models and create node containers

The following code configures the mobility model for vehicles and creates the node container:

```cpp
// experiment.cc
// Create Ns2MobilityHelper with the specified trace log file as parameter
auto ns2 = Ns2MobilityHelper(traceFile);
// Must add the following two lines code
// Create all vehicle nodes.
NodeContainer stas;
stas.Create(nodeNum);
ns2.Install(); // configure movements for each node, while reading trace file
```

The following code configures the mobility model for RSUs and creates the node container:

```cpp
// experiment.cc
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
```

The mobility model for the RSU is configured to be stationary, i.e., `ConstantPositionMobilityModel`. It also retrieves the size of the simulation area from the previously parsed configuration file, calculates the center of the area, and places the RSU at the center. Of course, this is just a simple approach and should be adjusted according to the size of the simulation area in conjunction with the propagation model.

Of course, you can also use other position allocators, such as replacing `ListPositionAllocator` with `GridPositionAllocator` . However, if you do so, some additional configurations will also be required.

The relevant references are 18 to 26.

## How to configure channel and physical layer

```cpp
// experiment.cc
YansWifiChannelHelper channel = YansWifiChannelHelper::Default();
channel.SetPropagationDelay("ns3::ConstantSpeedPropagationDelayModel");
channel.AddPropagationLoss("ns3::FixedRssLossModel", "Rss", DoubleValue(rss));
YansWifiPhyHelper phy;
// This is one parameter that matters when using FixedRssLossModel
// set it to zero; otherwise, gain will be added
phy.Set("RxGain", DoubleValue(0));
phy.SetChannel(channel.Create());
WifiHelper wifi;
wifi.SetRemoteStationManager("ns3::ConstantRateWifiManager",
                                "DataMode",
                                StringValue("OfdmRate6Mbps"),
                                "ControlMode",
                                StringValue("OfdmRate6Mbps"));
```

In the code above, `ConstantSpeedPropagationDelayModel` is the propagation delay model, `FixedRssLossModel` is the propagation loss model, `ConstantRateWifiManager` is the rate manager, and `OfdmRate6Mbps` indicates that the channel bandwidth is 6 Mbps. These parameters usually have default values if not specified. To find out about the default values, there are generally two approaches: one is to look at the code, and the other is to consult the documentation. Taking `YansWifiChannelHelper` as an example, Ref. 27 briefly mentions what its default propagation loss model and propagation delay model are.

> By default, we create a channel model with a propagation delay equal to a constant, the speed of light, and a propagation loss based on a log distance model with a reference loss of 46.6777 dB at reference distance of 1m.

In Ref. 28, it explicitly states what its default propagation loss model and propagation delay model are.

> Specifically, the default is a channel model with a propagation delay equal to a constant, the speed of light (ns3::ConstantSpeedPropagationDelayModel), and a propagation loss based on a default log distance model (ns3::LogDistancePropagationLossModel), using a default exponent of 3. Please note that the default log distance model is configured with a reference loss of 46.6777 dB at reference distance of 1m.

As for which propagation loss models and propagation delay models are implemented in ns-3, you can refer to Ref. 29.

If you want to change the propagation model, you just need to change the corresponding value in the code to the one you wish to use, for example, changing `ns3::ConstantSpeedPropagationDelayModel` to `ns3::RandomPropagationDelayModel` .

Each propagation model also has configurable attributes; take `FixedRssLossModel` as an example. You can find the attributes that can be set in the `Detailed Description` section of reference 30. Typically, these can also be obtained by looking at the `GetTypeId` function of the corresponding code:

```cpp
// experiment.cc
TypeId
FixedRssLossModel::GetTypeId()
{
    static TypeId tid = TypeId("ns3::FixedRssLossModel")
                            .SetParent<PropagationLossModel>()
                            .SetGroupName("Propagation")
                            .AddConstructor<FixedRssLossModel>()
                            .AddAttribute("Rss",
                                          "The fixed receiver Rss.",
                                          DoubleValue(-150.0),
                                          MakeDoubleAccessor(&FixedRssLossModel::m_rss),
                                          MakeDoubleChecker<double>());
    return tid;
}
```

Among them, `Rss` in the `AddAttribute` is an attribute that is allowed to be set, and this determines what string identifier you should use when setting the attribute. Note that this method also applies to other classes that have a `GetTypeId` function. When you need to set attributes through their `set` methods or the `SetAttribute` method of a `Ptr`, you either have to consult the documentation to find the settable attributes or look into the code.

As for the value of `RxGain`, this also depends on the propagation model used.

The default value of `ConstantRateWifiManager` is `OfdmRate6Mbps`, meaning the channel bandwidth is 6 Mbps. If you want to verify whether it is indeed 6 Mbps, you can enable pcap in the code:

```cpp
// experiment.cc
phy.SetPcapDataLinkType(WifiPhyHelper::DLT_IEEE802_11_RADIO);
phy.EnablePcap("experiment", vehicleDevices.Get(0));
phy.EnablePcap("experiment", rsuDevices.Get(0));
```

Then, open the generated file with Wireshark, select a packet, and examine the corresponding fields at the Physical layer:

![image](resources/ce4d45909eddcab75882ce1e2ac26c3c4401be61ebd005f3464e64798eeb014f.png)

If you want to change the channel bandwidth, you can modify the values of `DataMode` and `ControlMode`. However, the rates permitted by each IEEE 802.11 standard are predefined, for which you can refer to reference 31. Regarding the representation of each rate in the code, the author has not found any documentation that fully and accurately describes this point, but you can take a look at reference 32, which provides some description. Alternatively, readers can examine the class `WifiPhy` and other parts of the code to see if they can find corresponding rules.

## How to configure the data link layer

## References

1. [SUMO 从入门到基础 SUMO入门一篇就够了](https://blog.csdn.net/qilie_32/article/details/127201612)
2. [使用OSM生成路网](https://www.bilibili.com/video/BV1H7411F76B/?p=4&vd_source=95cbfe5a1adb7fe10e7dfdf77f3ea7d8)
3. [SUMO仿真教程（4）—— 由openstreetmap生成路网文件](https://blog.csdn.net/weixin_50632459/article/details/115449397)
4. [（二）osm格式文件如何导入sumo（包括建筑物边界的生成）](https://blog.csdn.net/lianginging/article/details/137744327)
5. [polyconvert](https://sumo.dlr.de/docs/polyconvert.html)
6. [How do I maintain a constant number of vehicles in the net?](https://sumo.dlr.de/docs/FAQ.html#how_do_i_maintain_a_constant_number_of_vehicles_in_the_net)
7. [how can I set the vehicle to drive at a constant speed in sumo file?](https://stackoverflow.com/questions/64533948/how-can-i-set-the-vehicle-to-drive-at-a-constant-speed-in-sumo-file)
8. [Automatically generating a vehicle type](https://sumo.dlr.de/docs/Tools/Trip.html#automatically_generating_a_vehicle_type)
9. [`***.sumo.cfg` 配置文件生成](https://github.com/Internet-of-Vehicles-Code/Veins_SUMO_OMNeTpp?tab=readme-ov-file#211-%E4%BD%BF%E7%94%A8netconvert%E8%BD%AC%E6%8D%A2%E5%B7%A5%E5%85%B7)
10. [Strange behavior when using randomTrips.py](https://github.com/eclipse-sumo/sumo/issues/15306)
11. [DFROUTER speed conflict](https://github.com/eclipse-sumo/sumo/issues/6951#issuecomment-620675996)
12. [sumo+ns3](https://blog.csdn.net/LUYAO_LY/article/details/117383835)
13. [Trace File Generation](https://sumo.dlr.de/docs/Tutorials/Trace_File_Generation.html)
14. [Vanet：生成ns3仿真所需的车辆移动文件（*.tcl）](https://blog.csdn.net/hanweixiao/article/details/122407194)
15. [SUMO产生trace并在NS3中调用实例](https://blog.csdn.net/lovehuishouzan/article/details/96751782)
16. [sdsxpln/IoTWNT](https://github.com/sdsxpln/IoTWNT)
17. [TraceExporter](https://sumo.dlr.de/docs/Tools/TraceExporter.html)
18. [Thread: [sumo-user] Add RSU on road map](https://sourceforge.net/p/sumo/mailman/sumo-user/thread/CAHJk9CcoH3XHojVvfe+JHcUX+zsLYcnOxd14VW1daJd2nKObeg@mail.gmail.com/)
19. [ns3：搭建vanet的移动模型](https://blog.csdn.net/hanweixiao/article/details/122407904)
20. <https://github.com/ms-van3t-devs/ms-van3t/blob/a40ccaa833b8e06569c92c594703c4239ddc2a29/src/automotive/examples/v2i-areaSpeedAdvisor-80211p.cc#L1-L368>
21. [VANET projects examples using ns3](https://ns3simulation.com/vanet-projects-examples-using-ns3/)
22. <https://github.com/cqu-bdsc/NS3-example/blob/30f4d87fdb45c43305df4e7c4b6534ec0a2c9bcd/scratch/vanet-demo-case-3.cc#L1-L729>
23. [How can I add a Road Side Unit into a NS-2 and Sumo Simulation](https://stackoverflow.com/questions/47545100/how-can-i-add-a-road-side-unit-into-a-ns-2-and-sumo-simulation)
24. [RSU (road side unit) implementation in VANET using ns-3](https://groups.google.com/g/ns-3-users/c/GU5yhRKYKwc)
25. [物联网与无线网络实验资源](https://github.com/sdsxpln/IoTWNT)
26. [ns3::PositionAllocator Class Reference](https://www.nsnam.org/docs/release/3.42/doxygen/df/d7b/classns3_1_1_position_allocator.html)
27. [Default()](https://www.nsnam.org/docs/release/3.42/doxygen/de/d95/classns3_1_1_yans_wifi_channel_helper.html#abf7de2e0916e7188a8434a363cb96183)
28. [34.2.1.1. YansWifiChannelHelper](https://www.nsnam.org/docs/models/html/wifi-user.html#yanswifichannelhelper)
29. [28. Propagation](https://www.nsnam.org/docs/models/html/propagation.html)
30. [Detailed Description](https://www.nsnam.org/docs/release/3.42/doxygen/d0/d39/classns3_1_1_fixed_rss_loss_model.html#details)
31. [802.11物理层技术讲解](https://blog.csdn.net/weixin_42353331/article/details/86504529)
32. [transmission rate](https://groups.google.com/g/ns-3-users/c/ymNOB59mNqU/m/n8GvsoY95MYJ)
