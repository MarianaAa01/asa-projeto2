#include <iostream>
#include <vector>
#include <tuple>
#include <queue>
#include <unordered_map>
#include <unordered_set>
#include <limits>

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

unordered_map<int, unordered_set<int>> buildLineGraph(const vector<tuple<int, int, int>>& connections) {
    unordered_map<int, unordered_set<int>> lineGraph;
    unordered_map<int, unordered_set<int>> stationLines;

    for (const auto& conn : connections) {
        int station1 = get<0>(conn), station2 = get<1>(conn), line = get<2>(conn);
        stationLines[station1].insert(line);
        stationLines[station2].insert(line);
    }

    for (const auto& stationEntry : stationLines) {
        const auto& lines = stationEntry.second;
        vector<int> lineList(lines.begin(), lines.end());

        for (int i = 0; i < (int)lineList.size(); ++i) {
            for (int j = i + 1; j < (int)lineList.size(); ++j) {
                int line1 = lineList[i], line2 = lineList[j];
                lineGraph[line1].insert(line2);
                lineGraph[line2].insert(line1);
            }
        }
    }

    return lineGraph;
}

int minLineChangesPath(int startLine, int endLine, const unordered_map<int, unordered_set<int>>& lineGraph) {
    unordered_map<int, int> line_changes;
    for (const auto& line : lineGraph) {
        line_changes[line.first] = numeric_limits<int>::max();
    }
    line_changes[startLine] = 0;

    priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> pq;
    pq.push({0, startLine});

    while (!pq.empty()) {
        int changes = pq.top().first;
        int currentLine = pq.top().second;
        pq.pop();

        if (changes > line_changes[currentLine]) continue;

        for (int neighbor : lineGraph.at(currentLine)) {
            int newChanges = changes + 1;
            if (newChanges < line_changes[neighbor]) {
                line_changes[neighbor] = newChanges;
                pq.push({newChanges, neighbor});
            }
        }
    }

    return line_changes[endLine];
}

int metroConnectivity(int numStations, int numConnections, int numLines, const vector<tuple<int, int, int>>& connections) {
    if (numLines == 1 || numStations == 1) {
        return 0;
    }

    if (numConnections < numStations - 1) {
        return -1;
    }

    DSU dsu(numStations);
    for (const auto& conn : connections) {
        int station1 = get<0>(conn), station2 = get<1>(conn);
        dsu.unite(station1, station2);
    }

    int root = dsu.find(1);
    for (int i = 2; i <= numStations; i++) {
        if (dsu.find(i) != root) {
            return -1;
        }
    }

    unordered_map<int, unordered_set<int>> lineStationsMap;

    for (const auto& conn : connections) {
        int station1 = get<0>(conn), station2 = get<1>(conn), line = get<2>(conn);
        lineStationsMap[line].insert(station1);
        lineStationsMap[line].insert(station2);
    }

    for (const auto& entry : lineStationsMap) {
        if ((int)entry.second.size() == numStations) {
            return 0;
        }
    }

    auto lineGraph = buildLineGraph(connections);

    int maxChanges = 0;
    for (int startLine = 1; startLine <= numLines; ++startLine) {
        for (int endLine = startLine + 1; endLine <= numLines; ++endLine) {
            int changes = minLineChangesPath(startLine, endLine, lineGraph);
            if(changes == numLines - 1){
                return changes;
            }
            maxChanges = max(maxChanges, changes);
        }
    }
    return maxChanges;
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

    int mc = metroConnectivity(numStations, numConnections, numLines, connections);
    cout << mc << endl;
    return 0;
}