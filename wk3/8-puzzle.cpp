#include <bits/stdc++.h>
#define xx first
#define yy second
#define NROW 3
#define NCOL 3
#define HOLE 0
using namespace std;
typedef vector<vector<int>> state;

// show a state
void show(state &s) {
    for (int i = 0 ; i < NROW; ++i) {
        for (int j = 0 ; j < NCOL; ++j) {
            printf("%d ", s[i][j]);
        }
        printf("\n");
    }
}

// generate the goal state
// 1 2 3
// 4 5 6
// 7 8 0
state goal() {
    int i, j;
    state ret;
    for (i = 0 ; i < NROW; ++i) {
        ret.push_back(vector<int>());
        for (j = 0 ; j < NCOL; ++j) {
            ret[i].push_back(i * NCOL + j + 1);
        }
    }

    ret[NROW - 1][NCOL - 1] = 0;
    
    return ret;
}
// check if (x, y) is a valid coordinate
bool valid(int x, int y) {
    return x >= 0 && x < NROW &&  y >= 0 && y < NCOL;
}

// get the number of the (row, column) on the board 
int get_element(state &current, int row, int column) {
    if (!valid(row, column)) return -1;
    return current[row][column];
}

// the expected location of an element
pair<int, int> expected_location(int element) {
    return make_pair((element - 1) / NCOL, (element - 1) % NCOL);
}

// get the location of the hole
pair<int, int> get_hole(state current) {
    for (int i = 0 ; i < NROW; ++i) {
        for (int j = 0 ; j < NCOL; ++j) {
            if (get_element(current, i, j) == HOLE) return make_pair(i, j);
        }
    }

    return make_pair(-1, -1);
}
// get all possible next states
vector<state> next_states(state current) {
    int i;
    vector<state> ret;
    auto hole = get_hole(current);
    int dx[4] = {0, -1, 1, 0}, dy[4] = {-1, 0, 0, 1};
    for (i = 0 ; i < 4; ++i) {
        int nx = hole.xx + dx[i], ny = hole.yy + dy[i];
        if (valid(nx, ny)) {
            state nxt = current;
            swap(nxt[hole.xx][hole.yy], nxt[nx][ny]);
            ret.push_back(nxt);
        }
    }

    return ret;
}

// compute the hcost of a position

int hcost(state current) {
    int i, j, cost = 0;
    for (i = 0; i < NROW; ++i) {
        for (j = 0 ; j < NCOL; ++j) {
            if (get_element(current, i, j) != HOLE) {
                int element = get_element(current, i, j);
                int expected_row = expected_location(element).xx, expected_col = expected_location(element).yy;
                cost = cost + abs(expected_row - i) + abs(expected_col - j);
            }
        }
    }

    return cost;
}

// check if current state is the goal position
bool reached(state &current) {
    int i, j;
    for (i = 0; i < NROW; ++i) {
        for (j = 0 ; j < NCOL; ++j) {
            if (get_element(current, i, j) == HOLE) continue;
            int element = get_element(current, i, j);
            if (expected_location(element).xx != i || expected_location(element).yy != j) return false;
        }
    }

    return true;
}

void A_star(state &src) {
    priority_queue<pair<int, state>, vector<pair<int, state>>, greater<pair<int, state>>> q;
    set<state> visited;
    map<state, int> gcost;
    map<state, state> pre;
    q.emplace(hcost(src), src);
    gcost[src] = 0;

    while (!q.empty()) {
        auto current = q.top();
        state s = current.second;
        q.pop();
        // if the node has been visited do not expand it again
        if (visited.find(s) != visited.end()) continue;
        // if we reach the goal, exit the loop
        if (reached(s)) break;
        visited.insert(s);
        auto nxt_state = next_states(s);
        // for all possible next states
        for (state &nxt : nxt_state) {
            if (visited.find(nxt) == visited.end()) {
                // if it has not been visited
                // consider the next gcost
                int new_gcost = gcost[s] + 1;
                // if the new gcost is better
                if (gcost.find(nxt) == gcost.end() || gcost[nxt] > new_gcost) {
                    // update the gcost of the next state
                    gcost[nxt] = new_gcost;
                    // calculate the hcost of the next state
                    int h = hcost(nxt);
                    // push the next state with fcost = gcost + hcost into the priority queue
                    q.emplace(new_gcost + h, nxt);
                    // set the previous state of next to s
                    pre[nxt] = s;
                }
            }
        }
    } 

    state target = goal();

    if (pre.find(target) == pre.end() && target != src) {
        printf("NO SOLUTION\n");
    } else {
        // construct the path backward
        vector<state> result;
        while (1) {
            result.push_back(target);
            if (pre.find(target) == pre.end()) break;
            target = pre[target];
        }

        reverse(result.begin(), result.end());
        int step = 0;
        for (auto s : result) {
            printf("Step %d: hcost=%d\n", step++, hcost(s));
            show(s);
        }
    }
}


int main() {
    int i, j;
    state curr;
    for (i = 0 ; i < NROW; ++i) {
        curr.push_back(vector<int>(NCOL, 0));
        for (j = 0 ; j < NCOL; ++j) {
            scanf("%d", &curr[i][j]);
        }
    }

    A_star(curr);
    return 0;
}