#include <iostream>
#include <stack>
#include <vector>
#include <tuple>
#include <queue>
#include <unordered_map>
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

struct pair_hash {
    template <class T1, class T2>
    size_t operator () (const pair<T1, T2> &p) const {
        return hash<T1>{}(p.first) ^ (hash<T2>{}(p.second) << 1);
    }
};

unordered_map<pair<int, int>, int, pair_hash> memo;

int metroConnectivity(int numStations, int numConnections, int numLines, const vector<tuple<int,int,int>>&connections){
    memo.clear();
    vector<vector<pair<int, int>>> graph(numStations + 1);
    DSU dsu(numStations);

    for (const auto& conn : connections) {
        int station1 = get<0>(conn), station2 = get<1>(conn), line = get<2>(conn);
        graph[station1].push_back(make_pair(station2, line));
        graph[station2].push_back(make_pair(station1, line));
        dsu.unite(station1, station2);
    }

    int root = dsu.find(1);
    for (int i = 2; i <= numStations; i++) {
        if (dsu.find(i) != root) return -1;
    }

    unordered_map<int, int> min_line_changes;
    int mc = 0;
    for (int start = 1; start <= numStations; start++) {
        for (int end = start + 1; end <= numStations; end++) {

            auto memo_key = make_pair(min(start, end), max(start, end));

            if (memo.count(memo_key)) {
                mc = max(mc, memo[memo_key]);
                //cout << "Memoized result for (" << start << ", " << end << ") = " << mc << endl;
                continue;
            }
            priority_queue<tuple<int, int, int>, vector<tuple<int, int, int>>, greater<tuple<int, int, int>>> pq;

            for(const auto& start_edge : graph[start]) {
                pq.push({0, start_edge.first, start_edge.second});
            }

            min_line_changes.clear();
            min_line_changes[start] = 0;

            int current_mc = numeric_limits<int>::max();
            //cout << "Starting BFS from station " << start << " to station " << end << endl;

            while(!pq.empty()) {
                int changes, current_station, current_line;
                tie(changes, current_station, current_line) = pq.top(); 
                pq.pop();

                auto memo_key = make_pair(min(current_station, end), max(current_station, end));
                //cout << "Visiting station " << current_station << " on line " << current_line << " with " << changes << " line changes." << endl;

                if (current_station == end) {
                    current_mc = min(current_mc, changes);
                    memo[memo_key] = current_mc; 
                    //cout << "Reached destination station " << end << " with " << current_mc << " line changes." << endl;
                    break;
                }

                if (min_line_changes.count(current_station) && changes > min_line_changes[current_station]) {
                    //cout << "Skipping station " << current_station << " since we've already found a better path." << endl;
                    continue;
                }
                
                for (const auto& next_edge : graph[current_station]) {
                    int next_station = next_edge.first;
                    int next_line = next_edge.second;

                    int new_changes = changes + (next_line != current_line);

                    if (!min_line_changes.count(next_station) || new_changes < min_line_changes[next_station]) {
                        min_line_changes[next_station] = new_changes;
                        pq.push(make_tuple(new_changes, next_station, next_line));

                        //cout << "Pushing to queue: station " << next_station << " on line " << next_line << " with " << new_changes << " line changes." << endl;
                    }
                }
            }
            memo[memo_key] = current_mc;
            mc = max(mc, current_mc);
            //cout << "Finished processing (" << start << ", " << end << ") with " << current_mc << " line changes." << endl;
            if (mc == numLines - 1) {
                //cout << "Maximum possible line changes found, exiting early." << endl;
                return mc;
            }
        }
    }
    //cout << "Final mc; "<< mc<< endl;
    return mc;
}

//________________________________________________________________________________________________________________
int main() {
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

    if(numConnections < numStations - 1){
        cout << -1 << endl;
        return 0; 
    }
    
    int mc = metroConnectivity(numStations, numConnections, numLines, connections);
    cout << mc << endl;
    return 0;
}
