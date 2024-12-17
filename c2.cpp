#include <iostream>
#include <vector>
#include <set>
#include <map>
#include <algorithm>
using namespace std;

class MetroNetwork {
public:
    // Grafo de linhas (onde cada linha contém um set de estações)
    vector<set<int>> lines;
    // Mapa que mapeia cada estação para as linhas em que ela aparece
    map<int, set<int>> station_to_lines;

    // Adiciona a conexão entre duas estações na linha l
    void addConnection(int l, int x, int y) {
        if (lines.size() <= l) {
            lines.resize(l + 1);  // Garante que o índice da linha l existe
        }
        
        // Adiciona a estação x e y à linha l
        lines[l].insert(x);
        lines[l].insert(y);

        // Adiciona as estações ao mapa que associa estação com linhas
        station_to_lines[x].insert(l);
        station_to_lines[y].insert(l);
    }

    // Verifica se o grafo de linhas é conexo
    bool isLineGraphConnected() {
        // Vamos considerar a primeira linha como referência
        vector<bool> visited(lines.size(), false);

        // Verifica se o número de linhas é maior que 1
        if (lines.empty() || lines.size() == 1) {
            return true; // Se há 0 ou 1 linha, o grafo de linhas é conexo
        }

        // DFS para verificar a conectividade das linhas
        dfs(0, visited);

        // Se todas as linhas foram visitadas, o grafo de linhas é conexo
        for (bool v : visited) {
            if (!v) {
                return false;
            }
        }
        return true;
    }

private:
    void dfs(int line, vector<bool>& visited) {
        visited[line] = true;
        
        // Explora todas as linhas que compartilham uma estação com a linha atual
        for (int station : lines[line]) {
            for (int connected_line : station_to_lines[station]) {
                if (!visited[connected_line]) {
                    dfs(connected_line, visited);
                }
            }
        }
    }
};

int main() {
    int n, m, l;
    cin >> n >> m >> l;

    MetroNetwork network;

    // Lê as ligações entre estações
    for (int i = 0; i < m; i++) {
        int line, x, y;
        cin >> line >> x >> y;
        line--;  // Para ajustar para o índice de 0
        network.addConnection(line, x, y);
    }

    // Verifica se o grafo de linhas é conexo
    if (network.isLineGraphConnected()) {
        cout << "O grafo de linhas é conexo." << endl;
    } else {
        cout << "O grafo de linhas não é conexo." << endl;
    }

    return 0;
}
