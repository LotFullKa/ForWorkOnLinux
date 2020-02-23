#include <iostream>
#include <vector>
#include <set>
#include <queue>

using namespace std;

class Graphs{
private:

    vector<set <int>>  letter;              // под цифрой лежат цифры куда можной пойти
    vector<int> colors;                     // 0 - белый   1 - серый    2 - черный
public:
    queue <int>  FinishStuck;

    Graphs(){
        colors.resize(10,0);
        letter.resize(10);
    }

    void add(int from, int to){
        letter[from].insert(to);
    }

    void print(){
        for (set <int> i : letter) {
            for (int g : i) cout << g << ",  ";
            cout << endl;
        }
    }

    void DFS(int s) {
        if(colors[s] != 0) return;
        colors[s] = 1;
        for (auto i : letter[s]) DFS(i);
        colors[s] = 2;
        FinishStuck.push(s);
    }
};

int main() {
    Graphs Letters;
    string BeforeWord, NowWord;

    cin >> BeforeWord;
    int i = 0;

    while(cin >> NowWord){
        i = 0;
        while(BeforeWord[i] == NowWord[i] and i < 4) i++;
        Letters.add(BeforeWord[i] - 'A', NowWord[i] - 'A');
        BeforeWord = NowWord;
    }

    Letters.print();

    for (int i = 0; i < 10; ++i) {
        Letters.DFS(i);
    }

    for (int i = 0; i < 10; ++i) {
        cout << char(Letters.FinishStuck.front() + 'A') << "  ";
        Letters.FinishStuck.pop();
    }
    cout << "check! \n";
    return 0;
}