#include <iostream>
#include <vector>
#include <tuple>
using namespace std;

int main()
{
    int n, m, l;
    cin >> n >> m >> l;

    // vetor de tuplos que guarda as conexões
    vector<tuple<int, int, int>> connections;

    for (int i = 0; i < m; ++i)
    {
        int line, station1, station2;
        cin >> line >> station1 >> station2;
        connections.emplace_back(line, station1, station2);
    }

    cout << "\n" << endl;
    cout << "Número de estações (n): " << n << endl;
    cout << "Número de ligações (m): " << m << endl;
    cout << "Número de linhas (l): " << l << endl;
    cout << "\n" << endl;
    cout << "Conexões:" << endl;
    for (const auto& conn : connections) {
        cout << "Linha " << get<0>(conn) << ": Estação " << get<1>(conn) << " <-> Estação " << get<2>(conn) << endl;
    }
    cout << "\n" << endl;
    cout << "Como as conexões estão guardadas num vetor de tuplos:" << endl;
    for (const auto& conn : connections) {
    cout << "(" << get<0>(conn) << ", " << get<1>(conn) << ", " << get<2>(conn) << ")" << endl;
    }


    return 0;
}

/*
n: número de estações (n>=2)
m: número de ligações (m>=0)
l: número de linhas da rede de metro (l>=0)

cada linha (i) do ficheiro tem l, x, y (x está diretamente ligada a y na linha l)

entre duas estações podem existir múltiplas conexões, correspondendo a diferentes linhas de metro

Exemplo 1:

    7 8 3    n=7; m=8; l=3
    3 2 1    a estação 3 está diretamente ligada à estação 2 na linha 1
    2 7 1
    7 5 1
    2 6 2
    6 4 2
    4 1 2
    4 1 3
    1 5 3


Exemplo 2:

    7 6 3 n=7; m=6; l=3
    3 2 1
    2 7 1
    7 5 1
    2 6 2
    6 4 2
    1 5 3


Para testar:
7 8 3
3 2 1
2 7 1
7 5 1
2 6 2
6 4 2
4 1 2
4 1 3
1 5 3


7 6 3
3 2 1
2 7 1
7 5 1
2 6 2
6 4 2
1 5 3
*/