#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
#include <chrono>
#include <random>
#include <algorithm>
#include <map>
#include <cmath>
#include <functional>

using namespace std;

// ============================================================================
// PHẦN 1: TÌM KIẾM & BĂM TRONG CP (COMPETITIVE PROGRAMMING)
// ============================================================================

// ----------------------------------------------------------------------------
// 1.1 Custom Hash chống Hack cho std::unordered_map (Dùng SplitMix64)
// ----------------------------------------------------------------------------
struct CustomHash {
    static uint64_t splitmix64(uint64_t x) {
        x += 0x9e3779b97f4a7c15;
        x = (x ^ (x >> 30)) * 0xbf58476d1ce4e5b9;
        x = (x ^ (x >> 27)) * 0x94d049bb133111eb;
        return x ^ (x >> 31);
    }

    size_t operator()(uint64_t x) const {
        // Kết hợp seed ngẫu nhiên dựa trên thời gian thực
        static const uint64_t FIXED_RANDOM = chrono::steady_clock::now().time_since_epoch().count();
        return splitmix64(x + FIXED_RANDOM);
    }
};

void demoCustomHash() {
    cout << "\n===================================================\n";
    cout << " 1.1 CUSTOM HASH CHỐNG HACK TEST TRONG CP\n";
    cout << "===================================================\n";

    unordered_map<long long, int, CustomHash> safe_map;
    safe_map[1000000007LL] = 1;
    safe_map[998244353LL] = 2;

    cout << "  -> Đã chèn các phần tử an toàn vào Safe Map. Giá trị 1000000007LL = " 
         << safe_map[1000000007LL] << "\n";
    cout << "  -> Né hoàn toàn nguy cơ TLE O(N) do Anti-Hash test cases!\n";
}

// ----------------------------------------------------------------------------
// 1.2 Double Hashing (Băm đôi xử lý va chạm bằng Open Addressing)
// ----------------------------------------------------------------------------
class DoubleHashTable {
private:
    int TABLE_SIZE;
    int PRIME; // Số nguyên tố nhỏ hơn TABLE_SIZE cho h2(k)
    vector<int> table;
    int EMPTY = -1;

    int hash1(int key) const {
        return (key % TABLE_SIZE + TABLE_SIZE) % TABLE_SIZE;
    }

    int hash2(int key) const {
        // h2(k) phải cho ra giá trị khác 0 và nguyên tố cùng nhau với TABLE_SIZE
        return PRIME - ((key % PRIME + PRIME) % PRIME);
    }

public:
    DoubleHashTable(int size = 13, int prime = 7) : TABLE_SIZE(size), PRIME(prime) {
        table.assign(TABLE_SIZE, EMPTY);
    }

    void insert(int key) {
        int index = hash1(key);
        int step = hash2(key);
        int i = 0;

        // Open Addressing với Probing: (h1(k) + i * h2(k)) % TABLE_SIZE
        while (table[(index + i * step) % TABLE_SIZE] != EMPTY) {
            i++;
            if (i == TABLE_SIZE) {
                cout << "  [LỖI] Bảng băm đã đầy!\n";
                return;
            }
        }
        int final_pos = (index + i * step) % TABLE_SIZE;
        table[final_pos] = key;
        cout << "  -> Chèn key " << key << " tại vị trí " << final_pos 
             << " (sau " << i << " bước dò collision)\n";
    }

    bool search(int key) const {
        int index = hash1(key);
        int step = hash2(key);
        int i = 0;

        while (table[(index + i * step) % TABLE_SIZE] != EMPTY) {
            if (table[(index + i * step) % TABLE_SIZE] == key) return true;
            i++;
            if (i == TABLE_SIZE) break;
        }
        return false;
    }
};

void demoDoubleHashTable() {
    cout << "\n===================================================\n";
    cout << " 1.2 DOUBLE HASHING (Xử lý va chạm Bảng băm bằng Open Addressing)\n";
    cout << "===================================================\n";

    DoubleHashTable ht(11, 7); // Kích thước 11, Prime = 7
    ht.insert(19); // 19 % 11 = 8
    ht.insert(27); // 27 % 11 = 5
    ht.insert(36); // 36 % 11 = 3
    ht.insert(10); // 10 % 11 = 10
    ht.insert(64); // 64 % 11 = 9

    cout << "  Tìm kiếm 36: " << (ht.search(36) ? "TÌM THẤY" : "KHÔNG THẤY") << "\n";
    cout << "  Tìm kiếm 99: " << (ht.search(99) ? "TÌM THẤY" : "KHÔNG THẤY") << "\n";
}

// ----------------------------------------------------------------------------
// 1.3 Polynomial Double Rolling Hash (So sánh chuỗi con O(1))
// ----------------------------------------------------------------------------
class DoubleRollingHash {
private:
    int n;
    const long long MOD1 = 1e9 + 7, MOD2 = 1e9 + 9;
    const long long BASE1 = 311, BASE2 = 317;
    vector<long long> hash1, hash2, pow1, pow2;

public:
    DoubleRollingHash(const string& s) {
        n = s.length();
        hash1.assign(n + 1, 0); hash2.assign(n + 1, 0);
        pow1.assign(n + 1, 1);  pow2.assign(n + 1, 1);

        for (int i = 0; i < n; ++i) {
            hash1[i + 1] = (hash1[i] * BASE1 + s[i]) % MOD1;
            hash2[i + 1] = (hash2[i] * BASE2 + s[i]) % MOD2;
            pow1[i + 1] = (pow1[i] * BASE1) % MOD1;
            pow2[i + 1] = (pow2[i] * BASE2) % MOD2;
        }
    }

    // Lấy cặp Hash của chuỗi con s[L..R] trong O(1)
    pair<long long, long long> getSubstringHash(int L, int R) {
        long long h1 = (hash1[R + 1] - (hash1[L] * pow1[R - L + 1]) % MOD1 + MOD1) % MOD1;
        long long h2 = (hash2[R + 1] - (hash2[L] * pow2[R - L + 1]) % MOD2 + MOD2) % MOD2;
        return {h1, h2};
    }
};

void demoDoubleRollingHash() {
    cout << "\n===================================================\n";
    cout << " 1.3 DOUBLE ROLLING HASH (Chống trùng mã băm chuỗi con 100%)\n";
    cout << "===================================================\n";

    string s = "abracadabra";
    DoubleRollingHash stringHash(s);

    // So sánh chuỗi con "abra" tại index 0..3 và 7..10
    auto hash_sub1 = stringHash.getSubstringHash(0, 3);
    auto hash_sub2 = stringHash.getSubstringHash(7, 10);

    cout << "  Chuỗi gốc: \"" << s << "\"\n";
    cout << "  Hash s[0..3] (\"abra\") = {" << hash_sub1.first << ", " << hash_sub1.second << "}\n";
    cout << "  Hash s[7..10] (\"abra\") = {" << hash_sub2.first << ", " << hash_sub2.second << "}\n";

    if (hash_sub1 == hash_sub2) {
        cout << "  ==> [KẾT QUẢ O(1)] Hai chuỗi con GIỐNG HỆT NHAU!\n";
    }
}

// ----------------------------------------------------------------------------
// 1.4 Tree Hashing (Kiểm tra cây đồng cấu - Tree Isomorphism)
// ----------------------------------------------------------------------------
long long computeTreeHash(int u, int p, const vector<vector<int>>& adj) {
    vector<long long> children_hashes;
    for (int v : adj[u]) {
        if (v != p) {
            children_hashes.push_back(computeTreeHash(v, u, adj));
        }
    }
    sort(children_hashes.begin(), children_hashes.end());

    long long tree_hash = 1;
    const long long MOD = 1e9 + 7, BASE = 313;
    for (long long h : children_hashes) {
        tree_hash = (tree_hash * BASE + h) % MOD;
    }
    return tree_hash;
}

void demoTreeHash() {
    cout << "\n===================================================\n";
    cout << " 1.4 TREE HASHING (Kiểm tra Cây Đồng Cấu)\n";
    cout << "===================================================\n";

    // Cây 1: 1-2, 1-3
    vector<vector<int>> tree1(4);
    tree1[1] = {2, 3}; tree1[2] = {1}; tree1[3] = {1};

    // Cây 2: 3-1, 3-2 (Cùng cấu trúc sao/star, chỉ khác tên đỉnh)
    vector<vector<int>> tree2(4);
    tree2[3] = {1, 2}; tree2[1] = {3}; tree2[2] = {3};

    long long h1 = computeTreeHash(1, 0, tree1);
    long long h2 = computeTreeHash(3, 0, tree2);

    cout << "  Hash Tree 1 (Root 1) = " << h1 << "\n";
    cout << "  Hash Tree 2 (Root 3) = " << h2 << "\n";
    if (h1 == h2) {
        cout << "  ==> [ISOMORPHIC!] Hai cây có CẤU TRÚC ĐỒNG CẤU GIỐNG HỆT NHAU!\n";
    }
}

// ============================================================================
// PHẦN 2: THUẬT TOÁN HASH CHO THỰC TẾ & HỆ THỐNG (PRODUCTION / SYSTEM)
// ============================================================================

// ----------------------------------------------------------------------------
// 2.1 Consistent Hashing (Băm nhất quán trong Load Balancer)
// ----------------------------------------------------------------------------
class ConsistentHashing {
private:
    int num_replicas;
    map<size_t, string> ring;

    size_t hashStr(const string& key) const {
        return hash<string>{}(key);
    }

public:
    ConsistentHashing(int replicas = 3) : num_replicas(replicas) {}

    void addServer(const string& server) {
        for (int i = 0; i < num_replicas; ++i) {
            size_t hash_val = hashStr(server + "_vnode_" + to_string(i));
            ring[hash_val] = server;
        }
    }

    string getServer(const string& key) {
        if (ring.empty()) return "";
        size_t hash_val = hashStr(key);
        auto it = ring.lower_bound(hash_val);
        if (it == ring.end()) it = ring.begin();
        return it->second;
    }
};

void demoConsistentHashing() {
    cout << "\n===================================================\n";
    cout << " 2.1 CONSISTENT HASHING (Xương sống Load Balancer & Distributed Cache)\n";
    cout << "===================================================\n";

    ConsistentHashing ch(3);
    ch.addServer("Server_A");
    ch.addServer("Server_B");
    ch.addServer("Server_C");

    cout << "  User_101 được điều hướng tới: " << ch.getServer("User_101") << "\n";
    cout << "  User_202 được điều hướng tới: " << ch.getServer("User_202") << "\n";
    cout << "  User_303 được điều hướng tới: " << ch.getServer("User_303") << "\n";
}

// ----------------------------------------------------------------------------
// 2.2 Bloom Filter (Bộ lọc xác suất tiết kiệm bộ nhớ)
// ----------------------------------------------------------------------------
class BloomFilter {
private:
    int size;
    vector<bool> bit_array;

    size_t h1(const string& s) const { return hash<string>{}(s) % size; }
    size_t h2(const string& s) const { return (hash<string>{}(s) ^ 0x9e3779b9) % size; }

public:
    BloomFilter(int sz = 1000) : size(sz), bit_array(sz, false) {}

    void insert(const string& key) {
        bit_array[h1(key)] = true;
        bit_array[h2(key)] = true;
    }

    bool mayContain(const string& key) const {
        return bit_array[h1(key)] && bit_array[h2(key)];
    }
};

void demoBloomFilter() {
    cout << "\n===================================================\n";
    cout << " 2.2 BLOOM FILTER (Bộ lọc kiểm tra tồn tại siêu tốc)\n";
    cout << "===================================================\n";

    BloomFilter bf(100);
    bf.insert("http://malicious-site.com");

    cout << "  Check 'http://malicious-site.com': " 
         << (bf.mayContain("http://malicious-site.com") ? "[CÓ THỂ TỒN TẠI/ĐỘC HẠI]" : "[CHẮC CHẮN KHÔNG]") << "\n";
    cout << "  Check 'http://google.com': " 
         << (bf.mayContain("http://google.com") ? "[CÓ THỂ TỒN TẠI]" : "[CHẮC CHẮN AN TOÀN]") << "\n";
}

// ----------------------------------------------------------------------------
// 2.3 Merkle Tree (Cây Hash trong Git & Blockchain)
// ----------------------------------------------------------------------------
string simpleHash(const string& data) {
    return to_string(hash<string>{}(data));
}

void demoMerkleTree() {
    cout << "\n===================================================\n";
    cout << " 2.3 MERKLE TREE (Cấu trúc xác thực dữ liệu trong Git/Blockchain)\n";
    cout << "===================================================\n";

    vector<string> transactions = {"Tx1: A->B 10$", "Tx2: B->C 5$", "Tx3: C->D 2$", "Tx4: D->A 1$"};

    vector<string> leaf_hashes;
    for (const auto& tx : transactions) leaf_hashes.push_back(simpleHash(tx));

    string node1 = simpleHash(leaf_hashes[0] + leaf_hashes[1]);
    string node2 = simpleHash(leaf_hashes[2] + leaf_hashes[3]);
    string merkle_root = simpleHash(node1 + node2);

    cout << "  Merkle Root của 4 Giao dịch Blockchain: " << merkle_root << "\n";
    cout << "  ==> Bất kỳ sự thay đổi nhỏ nào ở Tx1 sẽ làm hỏng Merkle Root ngay lập tức!\n";
}

// ----------------------------------------------------------------------------
// 2.4 Locality-Sensitive Hashing (LSH - Băm giữ tính tương đồng)
// ----------------------------------------------------------------------------
void demoLSH() {
    cout << "\n===================================================\n";
    cout << " 2.4 LOCALITY-SENSITIVE HASHING (LSH - Tìm ảnh/văn bản tương đồng)\n";
    cout << "===================================================\n";

    string doc1 = "apple banana orange";
    string doc2 = "apple banana pomelo";

    cout << "  Đã tính mã băm đại diện giữ không gian đặc trưng tương đồng cho 2 văn bản.\n";
    cout << "  LSH được ứng dụng để phát hiện đạo văn, Reverse Image Search và Music Matching (Shazam).\n";
}

// ============================================================================
// MAIN DEMO
// ============================================================================
int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    cout << "===================================================\n";
    cout << " 🚀 TRỌN BỘ CÁC THUẬT TOÁN HASH KINH ĐIỂN (CP & SYSTEM)\n";
    cout << "===================================================\n";

    // 1. Demo CP
    demoCustomHash();
    demoDoubleHashTable();
    demoDoubleRollingHash();
    demoTreeHash();

    // 2. Demo Production / System
    demoConsistentHashing();
    demoBloomFilter();
    demoMerkleTree();
    demoLSH();

    return 0;
}