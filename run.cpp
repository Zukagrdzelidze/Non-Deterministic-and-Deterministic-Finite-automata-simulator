#include <bits/stdc++.h>

using namespace std;

struct automata
{
    int idx;
    map<char, unordered_set<automata*> > moves;
};
map<int, automata*> con;
unordered_set<int> acceptStates;
int stateAmount, accAmount, moveAmount, acc, mv;
string inp;


int main(void){
    cin >> inp;
    cin >> stateAmount;
    cin >> accAmount;
    cin >> moveAmount;
    for(int i = 0; i < accAmount; i++){
        cin >> acc;
        if(con.find(acc) == con.end()){
            automata* node = new automata;
            node->idx = acc;
            con[acc] = node;
        }
        acceptStates.insert(con[acc]->idx);
    }
    for(int i = 0; i < stateAmount; i++){
        cin >> mv;
        char ch;
        int index;
        automata* au = new automata;
        au->idx = i;
        con[i] = au;
        for(int j = 0; j < mv; j++){
            cin >> ch;
            cin >> index;
            if(au->moves.find(ch) == au->moves.end()){
                unordered_set<automata*> unordSet;
                au->moves[ch] = unordSet;
            }
            if(con.find(index) == con.end()){
                automata* automat = new automata;
                automat->idx = index;
                con[index] = automat;
            }
            au->moves[ch].insert(con[index]);
        }
    }


    string str = "";
    queue<automata*> q;
    int num = 1;
    int k = 0;
    for(auto const& var : con[0]->moves[inp[0]]){
        q.push(var);
        k++;
    }
    while(num <= inp.size()){
        char c = 'N';
        int sec = 0;
        for(int i = 0; i < k; i++){
            if(acceptStates.find(q.front()->idx) != acceptStates.end()){
                c = 'Y';
            }
            if(num == inp.size()){
                q.pop();
                continue;
            }
            for(auto const& v : con[q.front()->idx]->moves[inp[num]]){
                q.push(v);
                sec++;
            }
            q.pop();
        }
        str += c;
        num++;
        k = sec;

    }

    cout << str << endl;
}