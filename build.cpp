#include <bits/stdc++.h>
#include <stack>

using namespace std;

struct automata
{
    int idx;
    bool isAccState;
    map<char, set<automata*>> nextPositions;
};


struct nfa{
    automata* start;
    automata* end;
};

int automataAmount, accAmount, moveAmount;
set<automata*> visited;
vector<automata*> vec;
set<pair<automata*, automata*>> sets;



int getIntfromChar(char a){
    switch (a)
    {
    case '*': return 3;
    case '+': return 2;
    case '|': return 1;
    case '(': return 0;
    }
}

bool isMoreImportant(char x, char y){
    return (getIntfromChar(x) >= getIntfromChar(y));
}

bool isChar(char ch){
    
    if((ch >= 'a' && ch <= 'z') || (ch >= '0' && ch <= '9') || ch == '@') {
        return true;
    }
    return false;
}



void deleteEpsilons(automata* automat){
    if(visited.count(automat) == 1) return;

    if(automat->nextPositions['@'].size() > 0){
        set<automata*> automataSets = automat->nextPositions['@'];
        automata* automataPosition = *(automataSets.begin());
        if(automataPosition->isAccState){
            automat->isAccState = true;
        }
        automat->nextPositions['@'].erase(automataPosition);

        for(auto const& var : automataPosition->nextPositions){
            char sym = var.first;
            set<automata*> set = automataPosition->nextPositions[sym];

            for(auto const& au : set){
                if(sym != '@' || au != automat){
                    if(sym == '@'){
                        pair<automata*, automata*> pr;
                        pr.first = automat;
                        pr.second = au;
                        if(sets.count(pr) == 0){
                            automat->nextPositions[sym].insert(au);
                            sets.insert(pr);
                        }
                    }else{
                        automat->nextPositions[sym].insert(au);
                    }
                }
            }
        }


        deleteEpsilons(automat);
    }else{
        visited.insert(automat);
        vec.push_back(automat);
        automataAmount++;
        if(automat->isAccState){
            accAmount++;
        }
        for(auto const& var : automat->nextPositions){
            char symbol = var.first;
            set<automata*> automataSets = automat->nextPositions[symbol];
            moveAmount += automataSets.size();

            for(auto const& v : automataSets){
                deleteEpsilons(v);
            }
        } 
    }
}

nfa createNFA(string str){
    stack<nfa> stack;
    for(int i = 0; i < str.length(); i++){
        char ch = str[i];
        if(isChar(ch)){
            automata*  fir = new automata;
            fir->isAccState = false;
            automata* sec = new automata;
            sec->isAccState = true;
            fir->nextPositions[ch].insert(sec);

            nfa temp;
            temp.start = fir;
            temp.end = sec;

            stack.push(temp);
        }else if(ch == '+'){
            nfa secNFA = stack.top();
            stack.pop();
            nfa firNFA = stack.top();
            stack.pop();
            firNFA.end->isAccState = false;
            firNFA.end->nextPositions['@'].insert(secNFA.start);
            nfa temp;
            temp.start = firNFA.start;
            temp.end = secNFA.end;

            stack.push(temp);         
        }else if(ch == '|'){
            nfa secNFA = stack.top();
            stack.pop();
            nfa firNFA = stack.top();
            stack.pop();

            automata* start = new automata;
            start->isAccState = false;
            start->nextPositions['@'].insert(firNFA.start);
            start->nextPositions['@'].insert(secNFA.start);
            automata* end = new automata;
            end->isAccState = true;
            firNFA.end->isAccState = false;
            firNFA.end->nextPositions['@'].insert(end);
            secNFA.end->isAccState = false;
            secNFA.end->nextPositions['@'].insert(end);

            nfa temp;
            temp.start = start;
            temp.end = end;
            
            stack.push(temp);   
        }else if(ch == '*'){
            nfa nf = stack.top();
            stack.pop();
            automata* start = new automata;
            automata* end = new automata;

            start->isAccState = false;
            start->nextPositions['@'].insert(nf.start);
            start->nextPositions['@'].insert(end);

            nf.end->isAccState = false;
            nf.end->nextPositions['@'].insert(nf.start);
            nf.end->nextPositions['@'].insert(end);
            end->isAccState = true;

            nfa temp;
            temp.start = start;
            temp.end = end;

            stack.push(temp);

        }
    }
    nfa toReturn = stack.top();
    deleteEpsilons(toReturn.start);
    return toReturn;
}

string toString(int i){
    return to_string(i);
}

void printPositions(automata* toPrint){
    int n = 0;
    string str = "";
    for(auto const& var : toPrint->nextPositions){
        char symbol = var.first;
        set<automata*> toSet = toPrint->nextPositions[symbol];

        n += toSet.size();

        for(auto const& au : toSet){
            str += symbol;
            str += " ";
            str += toString(au->idx);
            str += " ";
        }
    }
    str = toString(n) + " " + str;
    cout << str << endl;
}


int main(){

    string inp;
    cin >> inp;

    for(int i = 0; i < inp.length(); i++){
        if(i != inp.length() - 1){
            if(inp[i] == '(' && inp[i + 1] == ')'){
                //@ will be epsilon
                inp = inp.substr(0, i) + '@' + inp.substr(i + 2);
                i--;
                continue;
            }
            if(inp[i] != '|' && inp[i] != '(' && inp[i + 1] != ')' && inp[i + 1] != '|' && inp[i + 1] != '*'){
                // + will be operator that tells us to concatenate
                inp = inp.substr(0, i + 1) + '+' + inp.substr(i + 1);
                i++;
            }
        }
    }
    string str = "";
    stack<char> temp;

    for(int i = 0; i < inp.length(); i++){
        if(isChar(inp[i])){
            str = str + inp[i];
        }else{
            if(inp[i] == '('){
                temp.push(inp[i]);
            }else if(inp[i] == ')'){
                while(temp.top() != '('){
                    str = str + temp.top();
                    temp.pop();
                }
                temp.pop();
            }else{
                while(!temp.empty() && isMoreImportant(temp.top(), inp[i])){
                    str = str + temp.top();
                    temp.pop();
                }
                temp.push(inp[i]);
            }
        }
    }

    while(!temp.empty()){
        str += temp.top();
        temp.pop();
    }

    inp = str;

    nfa automat = createNFA(inp);
    
    int idx = 1;
    for(int i = 0; i < vec.size(); i++){
        if(vec[i] == automat.start){
            automat.start->idx = 0;
        }else{
            vec[i]->idx = idx;
            idx++;
        }
    }
    cout << automataAmount << " " << accAmount << " " << moveAmount << endl;
    for(int i = 0; i < vec.size(); i++){
        if(vec[i]->isAccState){
            cout << vec[i]->idx << " ";
        }
    }
    cout << endl;
    printPositions(automat.start);
    for(int i = 0; i < vec.size(); i++){
        if(vec[i] != automat.start){
            printPositions(vec[i]);
        }
    }
    return 0;
}