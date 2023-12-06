/******************************************************************************

                              Online C++ Compiler.
               Code, Compile, Run and Debug C++ program online.
Write your code in this editor and press "Run" button to compile and execute it.

*******************************************************************************/

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <algorithm>
#include <sstream>

struct Node {
    std::string name;
    std::vector<std::string> neighbors;
    mutable bool locked;
    
    int cut ;
    int wire;
    
    // Default constructor
    Node() = default;

    // Constructor with string argument
    Node(const std::string& s) : name(s), locked(false) {
        // neighbors vector暫時為空
    }
    
    void reverse() const{
        locked = !locked; // 切換 locked 的狀態為相反值
    }
};


void initializePartitions(const std::map<std::string, Node>& nodes, std::map<std::string, Node>& partition1, std::map<std::string, Node>& partition2) {
    std::vector<std::pair<std::string, int>> nodeDegrees;

    // Calculate degrees of nodes
    for (const auto& node : nodes) {
        nodeDegrees.push_back({node.first, node.second.neighbors.size()});
    }

    // Sort nodes based on degrees
    // std::sort(nodeDegrees.begin(), nodeDegrees.end(), [](const auto& a, const auto& b) {
    //     return a.second > b.second;
    // });

    // Assign nodes alternately to partitions based on sorted degrees
    bool assignToPartition1 = false;
    for (const auto& nodeDegree : nodeDegrees) {
        Node node = nodes.at(nodeDegree.first);
        if (assignToPartition1) {
            partition1[nodeDegree.first] = node;
            // Calculate cut for nodes in partition1
            for (const auto& neighbor : node.neighbors) {
                if (partition2.find(neighbor) != partition2.end()) {
                    partition1[nodeDegree.first].cut++;
                }else{
                    partition1[nodeDegree.first].wire++;
                }
            }
        } else {
            partition2[nodeDegree.first] = node;
            // Calculate cut for nodes in partition2
            for (const auto& neighbor : node.neighbors) {
                if (partition1.find(neighbor) != partition1.end()) {
                    partition2[nodeDegree.first].cut++;
                }
                else{
                    partition2[nodeDegree.first].wire++;
                }
            }
        }

        assignToPartition1 = !assignToPartition1;
    }

}



std::pair<std::string, std::string> findMax(const std::map<std::string, Node>& partition1, const std::map<std::string, Node>& partition2) {
    std::pair<std::string, std::string> maxNodes;
    int maxCutChange = -999;

    for (const auto& node1 : partition1) {
        for (const auto& node2 : partition2) {
            const Node& partition1Node = node1.second;
            const Node& partition2Node = node2.second;

            // Calculate cut changes
            int cutChange = partition1Node.cut + partition2Node.cut - partition1Node.wire - partition2Node.wire;

            // Update maxNodes if a better pair is found
            if (cutChange > maxCutChange && !partition1Node.locked && !partition2Node.locked) {
                maxCutChange = cutChange;
                maxNodes = {partition1Node.name, partition2Node.name};
            }
        }
    }

    return maxNodes;
}

void swapNode(std::map<std::string, Node>& partition1, std::map<std::string, Node>& partition2) {
    std::pair<std::string, std::string> nodesToSwap = findMax(partition1, partition2);
    std::string nodeName1 = nodesToSwap.first;
    std::string nodeName2 = nodesToSwap.second;
    if (!nodesToSwap.first.empty() && !nodesToSwap.second.empty()) {
        

        if (partition1.find(nodeName1) != partition1.end() && partition2.find(nodeName2) != partition2.end()) {
            Node& node1 = partition1[nodeName1];
            Node& node2 = partition2[nodeName2];
            
            
            for (const auto& neighbor : node1.neighbors) {
                if (partition2.find(neighbor) != partition2.end()) { 
                    partition2[neighbor].cut--;
                    partition1[nodeName1].cut--;
                    partition2[neighbor].wire++;
                    partition1[nodeName1].wire++;
                }else{
                    partition2[neighbor].cut++;
                    partition1[nodeName1].cut++;
                    partition2[neighbor].wire--;
                    partition1[nodeName1].wire--;
                }
            }
            node1.locked = true;
            partition2[nodeName1] = node1;
            partition1.erase(nodeName1);
            // std::cout << nodeName1 << std::endl;

            for (const auto& neighbor : node2.neighbors) {
                if (partition1.find(neighbor) != partition1.end()) {
                    partition1[neighbor].cut--;
                    partition2[nodeName2].cut--;
                    partition1[neighbor].wire++;
                    partition2[nodeName2].wire++;
                }else{
                    partition1[neighbor].cut++;
                    partition2[nodeName2].cut++;
                    partition1[neighbor].wire--;
                    partition2[nodeName2].wire--;
                }
            }
            node2.locked = true;
            partition1[nodeName2] = node2;
            partition2.erase(nodeName2);
            // std::cout << nodeName2 << std::endl;
        }
        
    }
    else if(!nodesToSwap.first.empty()){
        if(partition1.find(nodeName1) != partition1.end()){
            Node& node1 = partition1[nodeName1];
            
            for (const auto& neighbor : node1.neighbors) {
                if (partition2.find(neighbor) != partition2.end()) {
                    partition2[neighbor].cut--;
                    partition1[nodeName1].cut--;
                    partition2[neighbor].wire++;
                    partition1[nodeName1].wire++;
                }else{
                    partition2[neighbor].cut++;
                    partition1[nodeName1].cut++;
                    partition2[neighbor].wire--;
                    partition1[nodeName1].wire--;
                }
            }
            node1.locked = true;
            partition2[nodeName1] = node1;
            partition1.erase(nodeName1);
            // std::cout << nodeName1 << std::endl;
        }
    }    
    else if(!nodesToSwap.second.empty()){
        if(partition2.find(nodeName2)!= partition2.end()){
    
            Node& node2 = partition2[nodeName2];
            
            for (const auto& neighbor : node2.neighbors) {
                if (partition1.find(neighbor) != partition1.end()) {
                    partition1[neighbor].cut--;
                    partition2[nodeName2].cut--;
                    partition1[neighbor].wire++;
                    partition2[nodeName2].wire++;
                }else{
                    partition1[neighbor].cut++;
                    partition2[nodeName2].cut++;
                    partition1[neighbor].wire--;
                    partition2[nodeName2].wire--;
                }
            }
            node2.locked = true;
            partition1[nodeName2] = node2;
            partition2.erase(nodeName2);
            // std::cout << nodeName2 << std::endl;
        }
    }
}

int calculateCutSize(const std::map<std::string, Node>& partition1, const std::map<std::string, Node>& partition2) {
    int cutSize = 0;
    
    // 遍歷 partition1 中的節點
    for (const auto& node : partition1) {
        const Node& currentNode = node.second;

        // 檢查節點的鄰居是否在 partition2 中，如果是，則增加切割大小
        for (const std::string& neighbor : currentNode.neighbors) {
            if (partition2.find(neighbor) != partition2.end()) {
                cutSize++;
            }
        }
    }

    return cutSize;
}

int KLalgorithm(int k,std::map<std::string, Node> nodes){
    if(nodes.size()<=k) return 0;
    std::map<std::string, Node> partition1,partition2;
    
    initializePartitions(nodes,partition1,partition2);
    
    int initialCutsize = calculateCutSize(partition1,partition2);
    std::cout<<"Initial Cuts: " <<initialCutsize<< std::endl;
    
    bool cutDecrease;
    do{
        initialCutsize = calculateCutSize(partition1,partition2);
        
        swapNode(partition1,partition2);
        
        int currentCutSize = calculateCutSize(partition1, partition2);
        std::cout<<"Cuts after swap: " <<currentCutSize<< std::endl;
        cutDecrease = (currentCutSize < initialCutsize );
    }while(cutDecrease);
    
    
    int endCutSize = calculateCutSize(partition1,partition2);
    std::cout << endCutSize << std::endl;
    
    std::cout <<"Inside partition1" << std::endl;
    for (const auto& pair : partition1) {
        std::cout << "Node: " << pair.first <<std::endl;
    }
    
    std::cout <<"Inside partition2" << std::endl;
    for (const auto& pair : partition2) {
        std::cout << "Node: " << pair.first <<std::endl;
    }
    
    return KLalgorithm(k,partition1) + KLalgorithm(k,partition2) + 1;
}

int main() {
    std::string inputString = R"(
          # 10 inputs
# 1 outputs
# 8 D-type flipflops
# 38 inverters
# 66 gates 
INPUT(P.0)
INPUT(C.8)
INPUT(C.7)
INPUT(C.6)
INPUT(C.5)
INPUT(C.4)
INPUT(C.3)
INPUT(C.2)
INPUT(C.1)
INPUT(C.0)
OUTPUT(Z)
X.4 = DFF(I12)
X.3 = DFF(I13)
X.2 = DFF(I14)
X.1 = DFF(I15)
X.8 = DFF(I110)
X.7 = DFF(I111)
X.6 = DFF(I112)
X.5 = DFF(I113)
I73.1 = NOT(I69)
I73.2 = NOT(X.3)
I7.1 = NOT(I66)
I7.2 = NOT(X.2)
I88.1 = NOT(X.1)
I88.2 = NOT(P.0)
I48 = NOT(P.0)
I49 = NOT(X.4)
I50 = NOT(X.3)
I68 = NOT(I69)
I105.1 = NOT(I163)
I105.2 = NOT(X.6)
I182.1 = NOT(X.5)
I182.2 = NOT(I1.2)
I148 = NOT(X.7)
I149 = NOT(X.6)
I161 = NOT(I162)
I164 = NOT(I163)
I212 = NOT(P.0)
I213 = NOT(X.1)
I214 = NOT(X.2)
I215 = NOT(X.3)
I216 = NOT(X.4)
I225 = NOT(I224)
I240 = NOT(P.0)
I241 = NOT(X.5)
I242 = NOT(X.6)
I243 = NOT(X.7)
I244 = NOT(X.8)
I252 = NOT(I251)
I282 = NOT(P.2)
I283 = NOT(P.3)
I286 = NOT(C.2)
I287 = NOT(C.3)
I306 = NOT(P.6)
I307 = NOT(P.7)
I310 = NOT(C.6)
I311 = NOT(C.7)
I73.3 = AND(I69, I73.2)
I73.4 = AND(X.3, I73.1)
I7.3 = AND(I66, I7.2)
I7.4 = AND(X.2, I7.1)
I88.3 = AND(X.1, I88.2)
I88.4 = AND(P.0, I88.1)
I105.3 = AND(I163, I105.2)
I105.4 = AND(X.6, I105.1)
I182.3 = AND(X.5, I182.2)
I182.4 = AND(I1.2, I182.1)
I191.1 = AND(I164, X.6)
I1.2 = AND(I2.1, P.0)
P.5 = AND(I209.1, I205.2)
P.6 = AND(I209.1, I206.2)
P.7 = AND(I209.1, I207.2)
P.8 = AND(I209.1, I208.2)
I295.1 = AND(P.1, C.1)
I295.2 = AND(P.0, C.0)
I319.1 = AND(P.5, C.5)
I319.2 = AND(P.4, C.4)
I270.3 = AND(P.8, C.8)
I70.1 = OR(I68, X.4, I50)
I13 = OR(I73.3, I73.4)
I15 = OR(I88.3, I88.4)
I95.1 = OR(I64, I50, I48)
I167.1 = OR(I165, X.8, I148)
I170.1 = OR(I165, X.7)
I113 = OR(I182.3, I182.4)
I188.1 = OR(I163, I149, I148)
I291.1 = OR(I283, I287)
I291.2 = OR(I282, I286)
I315.1 = OR(I307, I311)
I315.2 = OR(I306, I310)
I270.2 = OR(I269.1, I269.2)
Z = OR(I270.2, I270.3)
I12 = NAND(I70.1, I62)
I62 = NAND(I95.1, X.4)
I64 = NAND(X.1, X.2)
I66 = NAND(X.1, P.0)
I110 = NAND(I167.1, I159)
I111 = NAND(I170.1, I161)
I159 = NAND(I188.1, X.8)
I163 = NAND(X.5, I1.2)
I165 = NAND(I164, X.6)
I222 = NAND(I225, I214)
I224 = NAND(I213, P.0)
I249 = NAND(I252, I242)
I251 = NAND(I241, P.0)
I269.1 = NAND(I291.1, I291.2, I290)
I269.2 = NAND(I315.1, I315.2, I314)
I14 = NOR(I7.3, I7.4)
I2.1 = NOR(I64, I49, I50)
I69 = NOR(I64, I48)
I112 = NOR(I105.3, I105.4)
I162 = NOR(I148, I191.1)
P.1 = NOR(I212, I213)
P.2 = NOR(I214, I224)
P.3 = NOR(I215, I222)
P.4 = NOR(X.3, I222, I216)
I209.1 = NOR(X.4, X.2, X.3, X.1)
I205.2 = NOR(I240, I241)
I206.2 = NOR(I242, I251)
I207.2 = NOR(I243, I249)
I208.2 = NOR(X.7, I249, I244)
I290 = NOR(I295.1, I295.2)
I314 = NOR(I319.1, I319.2)
    )";

    std::map<std::string, Node> nodes;

    // Create a stringstream from the inputString
    std::stringstream ss(inputString);
    std::string line;

    while (std::getline(ss, line, '\n')) {
        if (line.find("INPUT(") != std::string::npos || line.find("OUTPUT(") != std::string::npos) {
            size_t startPos = line.find("(");
            size_t endPos = line.find(")");
            if (startPos != std::string::npos && endPos != std::string::npos) {
                std::string nodeName = line.substr(startPos + 1, endPos - startPos - 1);
                // nodeName.erase(std::remove_if(nodeName.begin(), nodeName.end(), ::isspace), nodeName.end());
                // nodeName.erase(std::remove(nodeName.begin(), nodeName.end(), '.'), nodeName.end());

                nodes[nodeName].name = nodeName;
            }
        }
        else if (line.find("= AND(") != std::string::npos || line.find("= OR(") != std::string::npos || line.find("= NAND(") != std::string::npos || line.find("= NOR(") != std::string::npos || line.find("= NOT(") != std::string::npos||line.find("= DFF(") != std::string::npos) {
            size_t equalsPos = line.find(" = ");
            size_t openParenPos = line.find("(");
            size_t closeParenPos = line.find(")");
            size_t camma = line.find(",");
            
            std::string nodeName = line.substr(0, equalsPos);
            
            // Check if the line exceeds 20 characters
            if (line.length() > 20) {
                // Code to handle inputNodes greater than 20 characters
                std::string nodeName = line.substr(0, equalsPos);
                std::vector<std::string> inputNodes;
                size_t startPos = openParenPos + 1;
                
                while (camma != std::string::npos) {
                    std::string inputNode = line.substr(startPos, camma - startPos);
                    inputNodes.push_back(inputNode);
                    startPos = camma + 2;
                    camma = line.find(",", startPos);
                }
                
                std::string lastInputNode = line.substr(startPos, closeParenPos - startPos);
                inputNodes.push_back(lastInputNode);
                
                // Check if the nodes exist, if not, create them
                if (nodes.find(nodeName) == nodes.end()) {
                    nodes[nodeName] = Node(nodeName);
                }
                
                // Process inputNodes and set neighbors
                for (const auto& inputNode : inputNodes) {
                    if (nodes.find(inputNode) == nodes.end()) {
                        nodes[inputNode] = Node(inputNode);
                    }
                    nodes[nodeName].neighbors.push_back(inputNode);
                    nodes[inputNode].neighbors.push_back(nodeName);
                }
            } else {
                // Code for handling inputNode less than or equal to 20 characters
                std::string inputNode = line.substr(openParenPos + 1, closeParenPos - openParenPos - 1);
                
                // Proceed with the rest of the code using inputNode
                if (nodes.find(nodeName) == nodes.end()) {
                    nodes[nodeName] = Node(nodeName);
                }
                if (nodes.find(inputNode) == nodes.end()) {
                    nodes[inputNode] = Node(inputNode);
                }
                nodes[nodeName].neighbors.push_back(inputNode);
                nodes[inputNode].neighbors.push_back(nodeName);
            }

        }
    }

    
    //KL algorithm 
    
    
    int k = 5;
    int partitionTimes = KLalgorithm(k, nodes);
    std::cout<<std::endl<<"k = " <<k<<"時切了 " <<partitionTimes << " 次" <<std::endl;
    
    
    return 0;
}

//https://www.onlinegdb.com/online_c++_compiler
//請選用C++14 or > 14以上版本
