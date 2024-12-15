#include <iostream>
#include <vector>
#include <tuple>
#include <stack>
#include <limits>
#include <queue>
#include <unordered_map>

using namespace std;
struct Edge {
    int station;
    int line;

    // Define a custom equality operator for the unordered_map key comparison
    bool operator==(const Edge& other) const {
        return station == other.station && line == other.line;
    }
};

// Define a custom hash function for the Edge struct
namespace std {
    template <>
    struct hash<Edge> {
        size_t operator()(const Edge& e) const {
            return hash<int>()(e.station) ^ (hash<int>()(e.line) << 1);
        }
    };
}

unordered_map<Edge, int> memo;

//_______________________________________________FUNÇÕES AUXILIARES_______________________________________________

// verifica conectividade com o BFS -- não sei se isto é a maneira mais eficiente??? maybe rever
bool isConnected(int numStations, const vector<vector<Edge>>& graph) {
    queue<int> q;
    vector<bool> visited(numStations + 1, false);
    int visitedCount = 0; // Count of visited nodes

    q.push(1);
    visited[1] = true;

    while (!q.empty()) {
        int current = q.front();
        q.pop();
        visitedCount++;

        // Traverse neighbors
        for (const auto& edge : graph[current]) {
            if (!visited[edge.station]) {
                visited[edge.station] = true;
                q.push(edge.station);
            }
        }
    }

    return visitedCount == numStations;
}

int bfs(int start, int end, const vector<vector<Edge>>& graph, int numStations, int numLines) {
    if (start == end) return 0;

    vector<vector<int>> line_changes(numStations + 1, vector<int>(numLines + 1, numeric_limits<int>::max()));
    
    priority_queue<tuple<int, int, int>, vector<tuple<int, int, int>>, greater<tuple<int, int, int>>> pq;
    
    // Initialize with all possible starting lines
    for (const auto& edge : graph[start]) {
        pq.push({0, edge.station, edge.line});
        line_changes[edge.station][edge.line] = 0;
    }

    while (!pq.empty()) {
        tuple<int, int, int> top = pq.top();
        pq.pop();
        int changes = get<0>(top);
        int current_station = get<1>(top);  
        int current_line = get<2>(top);

        Edge currentEdge = {current_station, current_line};
        if (memo.find(currentEdge) != memo.end()) {
            // Use the memoized result to avoid reprocessing
            changes = memo[currentEdge];

            // Skip this station-line pair if its result is already computed and stored
            if (changes >= line_changes[current_station][current_line]) continue; 
        }


        if (current_station == end) {
            return changes;
        }

        // Skip if we've found a better path
        if (changes > line_changes[current_station][current_line]) {
            continue;
        }

        for (const auto& edge : graph[current_station]) {
            int next_station = edge.station;
            int next_line = edge.line;

            // Calculate new changes
            int new_changes = changes + (next_line != current_line ? 1 : 0);

            // Only update if we've found a better path
            if (new_changes < line_changes[next_station][next_line]) {
                line_changes[next_station][next_line] = new_changes;
                pq.push({new_changes, next_station, next_line});
            }
        }
    }
    return -1; 
}

int metroConnectivity(int numStations, int numConnections, int numLines, const vector<tuple<int,int,int>>&connections){
    vector<vector<Edge>> graph(numStations + 1); 
    memo.clear();

    for (const auto& conn : connections) {
        int station1 = get<0>(conn);
        int station2 = get<1>(conn);
        int line = get<2>(conn);
        graph[station1].push_back({station2, line});
        graph[station2].push_back({station1, line});
    }

    if(!isConnected(numStations, graph)){    
        return -1;
    }
  
    int mc = 0;
    for (int i = 1; i <= numStations; i++) {
        for (int j = i + 1; j <= numStations; j++) {
            // Create an Edge to use for memoization
            Edge key = {i, j};

            // If result is already memoized, use it
            if (memo.find(key) == memo.end()) {
                int changes = bfs(i, j, graph, numStations, numLines);
                memo[key] = changes; // Store result in memo
                if (changes == numLines - 1) {
                    return changes;
                }
            }

            mc = max(mc, memo[key]);
        }
    }

    return mc;
}

//________________________________________________________________________________________________________________
int main()
{
    ios::sync_with_stdio(0);
    cin.tie(0);
    int numStations, numConnections, numLines;
    cin >> numStations >> numConnections >> numLines;

    //temos as conexões aqui neste vetor de tuplos
    vector<tuple<int, int, int>> connections(numConnections);
    for (int i = 0; i < numConnections; ++i) //este for é para guardar as conexões todas
    {
        int line, station1, station2;
        cin >> station1 >> station2 >> line;
        connections[i] = make_tuple(station1, station2, line);
    }

    if(numLines == 1 || numStations == 1){
        cout << 0 << endl;
        return 0; 
    }

    if(numConnections >= numStations - 1){
        int mc = metroConnectivity(numStations, numConnections, numLines, connections);
        cout << mc << endl;
        return 0; 
    }
    else{
        cout << -1 << endl;
        return 0; 
    }
}
