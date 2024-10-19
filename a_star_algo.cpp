#include <iostream>
#include <unordered_map>
#include <vector>
#include <queue>
#include <cmath>
#include <limits>
#include <map>
#include <algorithm>

#include <windows.h>

using namespace std;
// Node structure
struct Node
{
    int id;
    string path;
    int gCost;                                  // Cost from start to current node
    int hCost;                                  // Heuristic cost to goal
    int fCost() const { return gCost + hCost; } // Total cost

    bool operator>(const Node &other) const
    {
        return fCost() > other.fCost();
    }
};

// Graph structure
struct Graph
{
    unordered_map<int, unordered_map<int, int>> adjacencyList;
    unordered_map<int, int> heuristics;

    void addEdge(int from, int to, int weight)
    {
        adjacencyList[from][to] = weight;
        adjacencyList[to][from] = weight; // Assuming undirected graph
    }
    //
    void calculateHeuristics(int goalNode, const unordered_map<int, pair<double, double>> &coordinates)
    {
        for (const auto &entry : coordinates)
        {                                                  // Using entry instead of structured binding
            int nodeId = entry.first;                      // Get the node ID
            pair<double, double> coord = entry.second;     // Get the coordinates
            double lat1 = coord.first;                     // Latitude of the current node
            double lon1 = coord.second;                    // Longitude of the current node
            double lat2 = coordinates.at(goalNode).first;  // Latitude of the goal node
            double lon2 = coordinates.at(goalNode).second; // Longitude of the goal node

            // Calculate the Euclidean distance (straight-line distance)
            double distance = sqrt(pow(lat2 - lat1, 2) + pow(lon2 - lon1, 2));
            heuristics[nodeId] = static_cast<int>(distance); // Convert to int if necessary
        }
    }
};

// Function to implement A* Algorithm
vector<int> aStar(const Graph &graph, int startNode, int goalNode)
{
    priority_queue<Node, vector<Node>, greater<Node>> openSet;
    unordered_map<int, int> gScore;
    unordered_map<int, int> cameFrom;

    for (const auto &entry : graph.adjacencyList)
    {
        gScore[entry.first] = numeric_limits<int>::max();
    }
    gScore[startNode] = 0;

    openSet.push({startNode, to_string(startNode), 0, graph.heuristics.at(startNode)});

    while (!openSet.empty())
    {
        Node currentNode = openSet.top();
        openSet.pop();

        if (currentNode.id == goalNode)
        {
            vector<int> path;
            int temp = goalNode;
            while (temp != startNode)
            {
                path.push_back(temp);
                temp = cameFrom[temp];
            }
            path.push_back(startNode);
            reverse(path.begin(), path.end());
            return path;
        }

        for (const auto &neighbor : graph.adjacencyList.at(currentNode.id))
        {
            int tentativeGScore = gScore[currentNode.id] + neighbor.second;

            if (tentativeGScore < gScore[neighbor.first])
            {
                cameFrom[neighbor.first] = currentNode.id;
                gScore[neighbor.first] = tentativeGScore;
                openSet.push({neighbor.first, currentNode.path + " -> " + to_string(neighbor.first), tentativeGScore, graph.heuristics.at(neighbor.first)});
            }
        }
    }

    return {}; // No path found
}
void sendToArduino(const int result[], int size)
{
    // Open the serial port
    HANDLE hSerial = CreateFile("\\\\.\\COM7", GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    if (hSerial == INVALID_HANDLE_VALUE)
    {
        std::cerr << "Error opening COM7" << std::endl;
        return;
    }

    // Set serial port parameters
    DCB dcbSerialParams = {0};
    dcbSerialParams.DCBlength = sizeof(dcbSerialParams);
    if (!GetCommState(hSerial, &dcbSerialParams))
    {
        std::cerr << "Error getting COM state" << std::endl;
        CloseHandle(hSerial);
        return;
    }
    dcbSerialParams.BaudRate = CBR_9600;
    dcbSerialParams.ByteSize = 8;
    dcbSerialParams.StopBits = ONESTOPBIT;
    dcbSerialParams.Parity = NOPARITY;
    if (!SetCommState(hSerial, &dcbSerialParams))
    {
        std::cerr << "Error setting COM state" << std::endl;
        CloseHandle(hSerial);
        return;
    }

    // Send data to Arduino
    DWORD bytesWritten;
    for (int i = 0; i < size; i++)
    {
        char data = result[i] ? '1' : '0';
        if (!WriteFile(hSerial, &data, 1, &bytesWritten, NULL))
        {
            std::cerr << "Error writing to COM7" << std::endl;
            break;
        }
    }

    // Read response from Arduino (optional)
    char buffer[256];
    DWORD bytesRead;
    if (ReadFile(hSerial, buffer, sizeof(buffer), &bytesRead, NULL))
    {
        buffer[bytesRead] = '\0'; // Null-terminate the response string
        std::cout << "Received from Arduino: " << buffer << std::endl;
    }
    else
    {
        std::cerr << "Error reading from Arduino" << std::endl;
    }

    // Close the serial port
    CloseHandle(hSerial);
}

void sendDataToArduino(const std::string &data)
{
    HANDLE hSerial = CreateFileA("\\\\.\\COM8", GENERIC_WRITE, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
    if (hSerial == INVALID_HANDLE_VALUE)
    {
        std::cerr << "Error opening COM port" << std::endl;
        return;
    }

    // Set serial port parameters
    DCB dcbSerialParams = {0};
    dcbSerialParams.DCBlength = sizeof(dcbSerialParams);
    GetCommState(hSerial, &dcbSerialParams);
    dcbSerialParams.BaudRate = CBR_9600; // Match the baud rate with Arduino
    dcbSerialParams.ByteSize = 8;
    dcbSerialParams.StopBits = ONESTOPBIT;
    dcbSerialParams.Parity = NOPARITY;
    SetCommState(hSerial, &dcbSerialParams);

    // Add newline character to the data before sending
    std::string dataWithNewline = data + "\n";

    // Send data
    DWORD bytesWritten;
    WriteFile(hSerial, dataWithNewline.c_str(), dataWithNewline.length(), &bytesWritten, NULL);

    CloseHandle(hSerial);
}

int main()
{
    Graph graph;

    // Add edges (remapped connections based on the renaming)
    graph.addEdge(15, 6, 9);  // Mantripukhri <-> Imphal Airport
    graph.addEdge(15, 16, 4); // Mantripukhri <-> Khuman Lampak
    graph.addEdge(15, 7, 5);  // Mantripukhri <-> Uripok
    graph.addEdge(15, 17, 3); // Mantripukhri <-> Lamlong
    graph.addEdge(15, 10, 7); // Mantripukhri <-> Singjamei
    graph.addEdge(15, 14, 2); // Mantripukhri <-> Porompat
    graph.addEdge(15, 13, 8); // Mantripukhri <-> Lilong
    graph.addEdge(15, 9, 4);  // Mantripukhri <-> Thangmeiband
    graph.addEdge(6, 10, 5);  // Imphal Airport <-> Singjamei
    graph.addEdge(6, 5, 2);   // Imphal Airport <-> Moirangkhom
    graph.addEdge(7, 9, 2);   // Uripok <-> Thangmeiband
    graph.addEdge(17, 19, 4); // Lamlong <-> Khurai
    graph.addEdge(17, 14, 3); // Lamlong <-> Porompat
    graph.addEdge(10, 13, 6); // Singjamei <-> Lilong

    // Second Layer
    graph.addEdge(5, 10, 2);  // Moirangkhom <-> Singjamei
    graph.addEdge(5, 6, 3);   // Moirangkhom <-> Imphal Airport
    graph.addEdge(8, 7, 4);   // Langol <-> Uripok
    graph.addEdge(8, 1, 6);   // Langol <-> Sekmai
    graph.addEdge(8, 9, 3);   // Langol <-> Thangmeiband
    graph.addEdge(4, 6, 8);   // Nambol <-> Imphal Airport
    graph.addEdge(4, 10, 7);  // Nambol <-> Singjamei
    graph.addEdge(4, 11, 5);  // Nambol <-> Kakawa
    graph.addEdge(11, 10, 3); // Kakawa <-> Singjamei
    graph.addEdge(11, 4, 5);  // Kakawa <-> Nambol
    graph.addEdge(11, 12, 6); // Kakawa <-> Thoubal
    graph.addEdge(18, 14, 4); // Andro <-> Porompat
    graph.addEdge(18, 17, 6); // Andro <-> Lamlong

    // Third Layer
    graph.addEdge(1, 8, 6);    // Sekmai <-> Langol
    graph.addEdge(1, 19, 7);   // Sekmai <-> Khurai
    graph.addEdge(19, 17, 4);  // Khurai <-> Lamlong
    graph.addEdge(19, 1, 7);   // Khurai <-> Sekmai
    graph.addEdge(12, 13, 6);  // Thoubal <-> Lilong
    graph.addEdge(12, 11, 6);  // Thoubal <-> Kakawa
    graph.addEdge(3, 4, 10);   // Bishnupur <-> Nambol
    graph.addEdge(3, 12, 8);   // Bishnupur <-> Thoubal
    graph.addEdge(3, 2, 12);   // Bishnupur <-> Lamdan
    graph.addEdge(2, 4, 11);   // Lamdan <-> Nambol
    graph.addEdge(2, 8, 10);   // Lamdan <-> Langol
    graph.addEdge(20, 19, 11); // Lamlai <-> Khurai
    graph.addEdge(20, 14, 13); // Lamlai <-> Porompat

    // Coordinates for each node (you'll need to provide the actual coordinates)
    unordered_map<int, pair<double, double>> coordinates = {
        {15, {24.8159, 93.9387}}, // Mantripukhri
        {6, {24.7954, 93.9430}},  // Imphal Airport
        {16, {24.8121, 93.9252}}, // Khuman Lampak
        {7, {24.8218, 93.9062}},  // Uripok
        {17, {24.7995, 93.9275}}, // Lamlong
        {10, {24.8154, 93.9487}}, // Singjamei
        {14, {24.8170, 93.9048}}, // Porompat
        {13, {24.8044, 93.9328}}, // Lilong
        {9, {24.8174, 93.9310}},  // Thangmeiband
        {5, {24.8160, 93.9405}},  // Moirangkhom
        {8, {24.8211, 93.8960}},  // Langol
        {4, {24.8181, 93.9304}},  // Nambol
        {11, {24.8186, 93.9147}}, // Kakawa
        {18, {24.8248, 93.9264}}, // Andro
        {1, {24.8202, 93.8700}},  // Sekmai
        {19, {24.7994, 93.9402}}, // Khurai
        {12, {24.8035, 93.9210}}, // Thoubal
        {3, {24.8181, 93.9512}},  // Bishnupur
        {2, {24.8250, 93.9256}},  // Lamdan
        {20, {25.0748, 94.2311}}  // Lamlai
    };

    map<int, string> m = {
        {15, "Mantripukhri"},
        {6, "Imphal Airport"},
        {16, "Khuman Lampak"},
        {7, "Uripok"},
        {17, "Lamlong"},
        {10, "Singjamei"},
        {14, "Porompat"},
        {13, "Lilong"},
        {9, "Thangmeiband"},
        {5, "Moirangkhom"},
        {8, "Langol"},
        {4, "Nambol"},
        {11, "Kakawa"},
        {18, "Andro"},
        {1, "Sekmai"},
        {19, "Khurai"},
        {12, "Thoubal"},
        {3, "Bishnupur"},
        {2, "Lamdan"},
        {20, "Lamlai"}};
    // Get the goal node from the user
    int goalNode;
    cout << "Select your universal destination node from the following list:" << endl;
    for (const auto &i : m)
    {
        cout << " Node ->  " << i.first << " -> " << i.second << endl;
    }
    // Get start node from user
    int startNode;
    cout << "Enter the start node: ";
    cin >> startNode;
    cout << "Enter the goal node: ";
    cin >> goalNode;

    // Calculate heuristics based on the selected goal node
    graph.calculateHeuristics(goalNode, coordinates);

    // Find the path
    vector<int> path = aStar(graph, startNode, goalNode);

    // Output the result
    if (!path.empty())
    {
        cout << "Path from " << startNode << " to " << goalNode << ": ";
        for (auto node : path)
        {
            cout << node << "." << m[node] << " -> ";
        }
        cout << endl;

        int totalDistance = 0;
        for (size_t i = 0; i < path.size() - 1; ++i)
        {
            totalDistance += graph.adjacencyList[path[i]][path[i + 1]];
        }
        cout << "Shortest distance from Node " << startNode << " to Node " << goalNode << ": " << totalDistance << " units" << endl;
        
        string s = m[startNode];
        string e = m[goalNode];
        string st = "Cost from";

        string resultString = "Cost from " + s + " to " + e + " is " + to_string(totalDistance) + " Units.";
        sendDataToArduino(resultString);

        // Result to send in arduino
        int result[20] = {0};
        for (auto i : path)
        {
            result[i - 1] = 1;
            sendToArduino(result, 20);
            // std::this_thread::sleep_for(std::chrono::milliseconds(1)); // Wait for 1 MILLIsecond
        }
        return 0;
    }
}