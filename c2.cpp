#include <iostream>
#include <vector>
using namespace std;

struct Connection
{
    int line, station1, station2;
};

int main()
{
    int n, m, l;
    cin >> n >> m >> l;

    // armazena as conexões
    vector<Connection> connections;

    for (int i = 0; i < m; ++i)
    {
        int line, station1, station2;
        cin >> line >> station1 >> station2;
        connections.push_back({line, station1, station2});
    }

    // (Opcional) Verificar se a leitura foi feita corretamente
    cout << "Número de estações (n): " << n << endl;
    cout << "Número de ligações (m): " << m << endl;
    cout << "Número de linhas (l): " << l << endl;
    cout << "Conexões:" << endl;
    for (const auto &conn : connections)
    {
        cout << "Linha " << conn.line << ": Estação " << conn.station1 << " <-> Estação " << conn.station2 << endl;
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
*/