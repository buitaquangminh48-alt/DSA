#include <iostream>
#include <vector>
#include <list>
#include <string>
#include <chrono>
#include <thread>
#include <mutex>
#include <algorithm>
#include <shared_mutex>
#include <unordered_map>

using namespace std;

// ============================================================================
// NHÓM 1: BẢN CÀI ĐẶT KINH ĐIỂN CHO CP (COMPETITIVE PROGRAMMING)
// ============================================================================

// ----------------------------------------------------------------------------
// 1.1 Static Hash Table với Linear Probing (Code siêu ngắn, mảng tĩnh)
// ----------------------------------------------------------------------------
namespace CP_Static {
    const int SIZE = 100003; // Số nguyên tố lớn hơn 2 * N
    int keys[SIZE];
    int values[SIZE];
    bool occupied[SIZE];

    void init() {
        fill(occupied, occupied + SIZE, false);
    }

    int hashFunc(int key) {
        return (key % SIZE + SIZE) % SIZE;
    }

    void insert(int key, int val) {
        int idx = hashFunc(key);
        while (occupied[idx]) {
            if (keys[idx] == key) {
                values[idx] = val; // Update
                return;
            }
            idx = (idx + 1) % SIZE; // Dò tuyến tính
        }
        occupied[idx] = true;
        keys[idx] = key;
        values[idx] = val;
    }

    int get(int key) {
        int idx = hashFunc(key);
        while (occupied[idx]) {
            if (keys[idx] == key) return values[idx];
            idx = (idx + 1) % SIZE;
        }
        return -1; // Not Found
    }
}

void demoCPStatic() {
    cout << "\n===================================================\n";
    cout << " 1.1 STATIC HASH TABLE (CP - Mảng tĩnh, Linear Probing)\n";
    cout << "===================================================\n";

    CP_Static::init();
    CP_Static::insert(101, 500);
    CP_Static::insert(202, 600);
    CP_Static::insert(101 + CP_Static::SIZE, 700); // Đụng độ index với 101

    cout << "  Get Key 101: " << CP_Static::get(101) << "\n";
    cout << "  Get Key 202: " << CP_Static::get(202) << "\n";
    cout << "  Get Collision Key: " << CP_Static::get(101 + CP_Static::SIZE) << "\n";
}

// ----------------------------------------------------------------------------
// 1.2 Custom Anti-Hack Hash Map (Random Time-based Seed + SplitMix64)
// ----------------------------------------------------------------------------
struct AntiHackHash {
    static uint64_t splitmix64(uint64_t x) {
        x += 0x9e3779b97f4a7c15;
        x = (x ^ (x >> 30)) * 0xbf58476d1ce4e5b9;
        x = (x ^ (x >> 27)) * 0x94d049bb133111eb;
        return x ^ (x >> 31);
    }

    size_t operator()(uint64_t x) const {
        static const uint64_t RND = chrono::steady_clock::now().time_since_epoch().count();
        return splitmix64(x + RND);
    }
};

void demoCPCustomHash() {
    cout << "\n===================================================\n";
    cout << " 1.2 CUSTOM ANTI-HACK HASH MAP (Né TLE O(N))\n";
    cout << "===================================================\n";

    unordered_map<long long, int, AntiHackHash> safe_map;
    safe_map[1e9 + 7] = 42;
    cout << "  Map hoạt động an toàn với Time-based Random Seed!\n";
    cout << "  Key 1e9 + 7: " << safe_map[1e9 + 7] << "\n";
}

// ============================================================================
// NHÓM 2: BẢN CÀI ĐẶT KINH ĐIỂN CHO THỰC TẾ (PRODUCTION / SYSTEM)
// ============================================================================

// ----------------------------------------------------------------------------
// 2.1 Chaining Hash Map với Resizing / Rehashing Tự Động
// ----------------------------------------------------------------------------
class DynamicChainingHashMap {
private:
    struct Node {
        int key;
        string val;
    };

    vector<list<Node>> buckets;
    size_t capacity;
    size_t element_count;
    const double MAX_LOAD_FACTOR = 0.75;

    int hashFunc(int key, size_t cap) const {
        return (key % cap + cap) % cap;
    }

    void rehash() {
        size_t new_cap = capacity * 2;
        vector<list<Node>> new_buckets(new_cap);

        for (const auto& chain : buckets) {
            for (const auto& node : chain) {
                int new_idx = hashFunc(node.key, new_cap);
                new_buckets[new_idx].push_back(node);
            }
        }

        buckets = move(new_buckets);
        capacity = new_cap;
        cout << "  [REHASH] Đã mở rộng Capacity lên: " << capacity << "\n";
    }

public:
    DynamicChainingHashMap(size_t init_cap = 4) : capacity(init_cap), element_count(0), buckets(init_cap) {}

    void insert(int key, const string& val) {
        int idx = hashFunc(key, capacity);
        for (auto& node : buckets[idx]) {
            if (node.key == key) {
                node.val = val;
                return;
            }
        }

        buckets[idx].push_back({key, val});
        element_count++;

        // Kiểm tra hệ số tải (Load Factor)
        if ((double)element_count / capacity > MAX_LOAD_FACTOR) {
            rehash();
        }
    }

    string get(int key) {
        int idx = hashFunc(key, capacity);
        for (const auto& node : buckets[idx]) {
            if (node.key == key) return node.val;
        }
        return "NOT_FOUND";
    }
};

void demoDynamicChaining() {
    cout << "\n===================================================\n";
    cout << " 2.1 DYNAMIC CHAINING HASH MAP (Auto Resizing / Rehashing)\n";
    cout << "===================================================\n";

    DynamicChainingHashMap map(4); // Khởi tạo capacity nhỏ = 4
    map.insert(1, "Alpha");
    map.insert(2, "Beta");
    map.insert(3, "Gamma"); // Vượt ngưỡng 0.75 -> Tự động Rehash lên 8!
    map.insert(4, "Delta");

    cout << "  Get Key 3: " << map.get(3) << "\n";
}

// ----------------------------------------------------------------------------
// 2.2 Robin Hood Hashing ("Lấy của người giàu chia cho người nghèo")
// ----------------------------------------------------------------------------
class RobinHoodHashMap {
private:
    struct Element {
        int key = -1;
        string val = "";
        int psl = -1; // Probe Sequence Length (Độ nghèo khổ - Khoảng cách tới vị trí gốc)
    };

    int capacity;
    vector<Element> table;

    int hashFunc(int key) const {
        return (key % capacity + capacity) % capacity;
    }

public:
    RobinHoodHashMap(int cap = 7) : capacity(cap), table(cap) {}

    void insert(int key, const string& val) {
        Element curr = {key, val, 0};
        int idx = hashFunc(key);

        while (true) {
            if (table[idx].psl == -1) {
                table[idx] = curr;
                return;
            }

            // Nếu phần tử mới "nghèo hơn" (PSL lớn hơn) phần tử hiện tại -> Cướp chỗ!
            if (curr.psl > table[idx].psl) {
                swap(curr, table[idx]); // Phần tử cũ bị đẩy đi tiếp
            }

            idx = (idx + 1) % capacity;
            curr.psl++;
        }
    }

    void display() {
        cout << "  Trạng thái bảng Robin Hood Hashing:\n";
        for (int i = 0; i < capacity; ++i) {
            if (table[i].psl != -1) {
                cout << "   Slot [" << i << "]: Key=" << table[i].key 
                     << ", Val=" << table[i].val << ", PSL (Độ nghèo)=" << table[i].psl << "\n";
            }
        }
    }
};

void demoRobinHood() {
    cout << "\n===================================================\n";
    cout << " 2.2 ROBIN HOOD HASHING (Tối ưu khoảng cách dò tìm)\n";
    cout << "===================================================\n";

    RobinHoodHashMap rh(7);
    rh.insert(1, "A");  // Hash = 1
    rh.insert(8, "B");  // Hash = 1 (Collision với A) -> Bị đẩy sang 2 (PSL=1)
    rh.insert(15, "C"); // Hash = 1 -> Va chạm A, va chạm B -> Đẩy tiếp
    rh.display();
}

// ----------------------------------------------------------------------------
// 2.3 Cuckoo Hashing (Cam kết Worst-Case Lookup O(1))
// ----------------------------------------------------------------------------
class CuckooHashMap {
private:
    static const int CAP = 11;
    vector<int> t1, t2;

    int h1(int key) const { return (key % CAP + CAP) % CAP; }
    int h2(int key) const { return ((key / CAP) % CAP + CAP) % CAP; }

public:
    CuckooHashMap() : t1(CAP, -1), t2(CAP, -1) {}

    bool insert(int key, int max_displace = 10) {
        int cur_key = key;
        for (int count = 0; count < max_displace; ++count) {
            // Thử chèn vào Bảng 1
            int pos1 = h1(cur_key);
            if (t1[pos1] == -1) {
                t1[pos1] = cur_key;
                return true;
            }
            // Nếu đã có -> Đá phần tử cũ văng ra
            swap(cur_key, t1[pos1]);

            // Thử đem phần tử bị đá sang Bảng 2 với h2
            int pos2 = h2(cur_key);
            if (t2[pos2] == -1) {
                t2[pos2] = cur_key;
                return true;
            }
            // Nếu Bảng 2 cũng có -> Đá tiếp!
            swap(cur_key, t2[pos2]);
        }
        cout << "  [CUCKOO CYCLE DETECTED] Cần Rehash lại toàn bộ!\n";
        return false;
    }

    bool lookup(int key) const {
        // Chỉ cần kiểm tra ĐÚNG 2 VỊ TRÍ -> Luôn là O(1) Tuyệt Đối!
        return (t1[h1(key)] == key || t2[h2(key)] == key);
    }
};

void demoCuckoo() {
    cout << "\n===================================================\n";
    cout << " 2.3 CUCKOO HASHING (Lookup O(1) Tuyệt đối)\n";
    cout << "===================================================\n";

    CuckooHashMap ck;
    ck.insert(20);
    ck.insert(31);
    ck.insert(42);

    cout << "  Lookup 31: " << (ck.lookup(31) ? "FOUND" : "NOT FOUND") << "\n";
    cout << "  Lookup 99: " << (ck.lookup(99) ? "FOUND" : "NOT FOUND") << "\n";
}

// ----------------------------------------------------------------------------
// 2.4 Thread-Safe Concurrent Hash Map (Lock Striping / Segments)
// ----------------------------------------------------------------------------
class ConcurrentHashMap {
private:
    struct Segment {
        unordered_map<int, string> map;
        mutable shared_mutex mutex; // Lock riêng cho từng Segment
    };

    size_t num_segments;
    vector<Segment> segments;

    size_t getSegmentIndex(int key) const {
        return (hash<int>{}(key)) % num_segments;
    }

public:
    ConcurrentHashMap(size_t segs = 4) : num_segments(segs), segments(segs) {}

    void insert(int key, const string& val) {
        size_t idx = getSegmentIndex(key);
        // Chỉ khóa duy nhất Segment tương ứng (Exclusive Write Lock)
        unique_lock<shared_mutex> lock(segments[idx].mutex);
        segments[idx].map[key] = val;
    }

    string get(int key) const {
        size_t idx = getSegmentIndex(key);
        // Nhiều luồng có thể cùng ĐỌC đồng thời (Shared Read Lock)
        shared_lock<shared_mutex> lock(segments[idx].mutex);
        auto it = segments[idx].map.find(key);
        if (it != segments[idx].map.end()) return it->second;
        return "NOT_FOUND";
    }
};

void demoConcurrent() {
    cout << "\n===================================================\n";
    cout << " 2.4 THREAD-SAFE CONCURRENT HASH MAP (Lock Striping)\n";
    cout << "===================================================\n";

    ConcurrentHashMap chm(4);

    // Giả lập 2 Threads ghi dữ liệu đồng thời
    thread t1([&]() { chm.insert(10, "Thread 1 Data"); });
    thread t2([&]() { chm.insert(20, "Thread 2 Data"); });

    t1.join();
    t2.join();

    cout << "  Get Key 10: " << chm.get(10) << "\n";
    cout << "  Get Key 20: " << chm.get(20) << "\n";
    cout << "  An toàn đa luồng tuyệt đối, tối ưu hiệu năng ghi/đọc song song!\n";
}

// ============================================================================
// MAIN DEMO
// ============================================================================
int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    cout << "===================================================\n";
    cout << " 🚀 TRỌN BỘ 6 CẤU TRÚC HASH TABLE KINH ĐIỂN\n";
    cout << "===================================================\n";

    // Demo CP Group
    demoCPStatic();
    demoCPCustomHash();

    // Demo Production Group
    demoDynamicChaining();
    demoRobinHood();
    demoCuckoo();
    demoConcurrent();

    return 0;
}