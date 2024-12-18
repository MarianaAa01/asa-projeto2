#include <iostream>
#include <stack>
#include <vector>
#include <tuple>
#include <queue>
#include <unordered_map>
#include <limits>
#include <unordered_set> 

using namespace std;

class DSU {
private:
    vector<int> parent;
    vector<int> rank;

public:
    DSU(int n) : parent(n + 1), rank(n + 1, 0) {
        for (int i = 0; i <= n; ++i) {
            parent[i] = i;
        }
    }

    int find(int x) {
        if (parent[x] != x) {
            parent[x] = find(parent[x]);
        }
        return parent[x];
    }

    bool unite(int x, int y) {
        int rx = find(x);
        int ry = find(y);

        if (rx == ry) return false;

        if (rank[rx] < rank[ry]) {
            parent[rx] = ry;
        } else if (rank[rx] > rank[ry]) {
            parent[ry] = rx;
        } else {
            parent[ry] = rx;
            ++rank[rx];
        }
        return true;
    }
};

struct pair_hash {
    template <class T1, class T2>
    size_t operator () (const pair<T1, T2>& p) const {
        return hash<T1>{}(p.first) ^ (hash<T2>{}(p.second) << 1);
    }
};

unordered_map<pair<int, int>, int, pair_hash> memo;

int metroConnectivity(int numStations, int numLines, const vector<tuple<int,int,int>>& connections) {
    memo.clear();
    vector<vector<pair<int, int>>> graph(numStations + 1);
    DSU dsu(numStations);
    unordered_map<int, unordered_set<int>> station_lines;

    for (const auto& conn : connections) {
        int station1 = get<0>(conn), station2 = get<1>(conn), line = get<2>(conn);
        graph[station1].push_back(make_pair(station2, line));
        graph[station2].push_back(make_pair(station1, line));
        dsu.unite(station1, station2);
        station_lines[station1].insert(line);
        station_lines[station2].insert(line);
    }
    
    // Check if all stations are connected
    int root = dsu.find(1);
    for (int i = 2; i <= numStations; i++) {
        if (dsu.find(i) != root) return -1;
    }

    unordered_set<int> connected_stations;
    for (int line = 1; line <= numLines; ++line) {
        connected_stations.clear();
        for (int i = 1; i <= numStations; ++i) {
            if (station_lines[i].count(line)) {
                connected_stations.insert(i);
            }
        }
        // If this line connects all stations, no line change is needed
        if ((int)connected_stations.size() == numStations) {
            return 0;  // No line change required
        }
    }

    unordered_map<int, int> min_line_changes;
    int mc = 0;

    // Run Dijkstra from each station to find minimum line changes to all other stations
    for (int start = 1; start <= numStations; start++) {
        priority_queue<tuple<int, int, int>, vector<tuple<int, int, int>>, greater<tuple<int, int, int>>> pq;
        min_line_changes.clear();
        for (const auto& start_edge : graph[start]) {
            pq.push({0, start_edge.first, start_edge.second});
        }

        while (!pq.empty()) {
            int changes, current_station, current_line;
            tie(changes, current_station, current_line) = pq.top();
            pq.pop();

            // If we've already found a better path, skip
            if (min_line_changes.count(current_station) && changes > min_line_changes[current_station]) continue;

            min_line_changes[current_station] = changes;
            min_line_changes[start] = 0;

            for (const auto& next_edge : graph[current_station]) {
                int next_station = next_edge.first;
                int next_line = next_edge.second;

                int new_changes = changes + (next_line != current_line);  // Increment if the line changes
                if (!min_line_changes.count(next_station) || new_changes < min_line_changes[next_station]) {
                    min_line_changes[next_station] = new_changes;
                    pq.push(make_tuple(new_changes, next_station, next_line));
                }
            }
        }


        for (const auto& entry : min_line_changes) {
            mc = max(mc, entry.second);
        }

        // Early exit if we've found the maximum possible line changes
        if (mc == numLines - 1) {
            return mc;
        }
    }

    return mc;
}

int main() {
    ios::sync_with_stdio(0);
    cin.tie(0);
    int numStations, numConnections, numLines;
    cin >> numStations >> numConnections >> numLines;

    vector<tuple<int, int, int>> connections(numConnections);
    for (int i = 0; i < numConnections; ++i) {
        int line, station1, station2;
        cin >> station1 >> station2 >> line;
        connections[i] = make_tuple(station1, station2, line);
    }

    if (numLines == 1 || numStations == 1) {
        cout << 0 << endl;
        return 0;
    }

    if (numConnections < numStations - 1) {
        cout << -1 << endl;
        return 0;
    }

    int mc = metroConnectivity(numStations, numLines, connections);
    cout << mc << endl;
    return 0;
}
