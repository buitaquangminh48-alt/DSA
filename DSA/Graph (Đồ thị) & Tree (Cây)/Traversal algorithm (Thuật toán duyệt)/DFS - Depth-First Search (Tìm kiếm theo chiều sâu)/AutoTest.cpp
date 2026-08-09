//AutoTest.cpp
#include<bits/stdc++.h>
using namespace std;

// Name 
const string NAME = "DFS"; 

// Number of test cases to generate
const int TEST = 10;

// Use a fixed seed to reproduce previous failing test cases
mt19937 rng(7405);

// If you want random test cases, use the following line instead
mt19937 rng2(chrono::steady_clock::now().time_since_epoch().count());

// This function returns a random integer in the range [l, r]
int rnd(int l, int r){
    // rng() returns a random integer in the range [0, 2^32 - 1]
    return abs((int)rng() % (r - l + 1)) + l;
}

// This function is used to generate test cases
void generate_test(){
    // We generate test cases and output them to a .inp file
    // Each time we output to the .inp file, we will overwrite the previous input
    ofstream inp(NAME + ".inp");

    // Input test cases here
    
    int n = rnd(1, 10);
    int m = rnd(0, min(20, n * (n - 1) / 2));

    inp << n << ' ' << m << '\n';

    set<pair<int, int>> edges;

    while ((int)edges.size() < m) {
        int u = rnd(1, n);
        int v = rnd(1, n);

        if (u == v) continue;
        if (u > v) swap(u, v);

        edges.insert({u, v});
    }

    for (auto [u, v] : edges)
        inp << u << ' ' << v << '\n';

    inp.close();
    
}

// In quite a few problems, checking the output is simply a matter of comparing whether the output of the .out file and the .ans file are identical.
// This is one of those problems
bool check_test(){
    // The following command checks if the two files are identical
    if (system(("diff " + NAME + ".out " + NAME + ".ans").c_str())) { // 2 different 
        return 0;
    }
    else{
        return 1;
    }
}

void process(){
    for(int itest = 1; itest <= TEST; itest++){\
        // Display the test number
        cout << "========== TEST " << itest << " ==========\n";

        // Generate test cases
        generate_test();

        // Run the two programs to generate output and answer files
        system(("./" + NAME +
               " < " + NAME + ".inp > " + NAME + ".out").c_str());

        system(("./" + NAME + "_trau" +
               " < " + NAME + ".inp > " + NAME + ".ans").c_str());

        // Print input
        cout << "Input:\n";
        ifstream fin(NAME + ".inp");
        string s;
        while (getline(fin, s))
            cout << s << '\n';
        fin.close();

        // Print output
        cout << "\nOutput:\n";
        ifstream fout(NAME + ".out");
        while (getline(fout, s))
            cout << s << '\n';
        fout.close();

        // Print answer
        cout << "\nAnswer:\n";
        ifstream fans(NAME + ".ans");
        while (getline(fans, s))
            cout << s << '\n';
        fans.close();

        // Check if the output and answer files are identical
        bool ok = check_test();

        if(!ok){
            cout << "TEST " << itest << ": WA\n";
            exit(0);
        }
        else{
            cout << "TEST " << itest << ": AC\n";
        }
    }
}

signed main(){
    // Set the name of the problem here
    process();
}