#include <iostream>
#include <vector>
#include <string>
#include <chrono>
#include <thread>
#include <mutex>
#include <shared_mutex>
#include <iomanip>
#include <algorithm>
#include <cstdint>

// Thư viện PBDS mở rộng cho CP
#include <ext/pb_ds/assoc_container.hpp>

using namespace std;

// ============================================================================
// BỘ CÔNG CỤ TRACE "PHÉP THUẬT" IN MÀU ANSI
// ============================================================================
namespace MagicTrace {
    const string RESET   = "\033[0m";
    const string BOLD    = "\033[1m";
    const string CYAN    = "\033[36m";
    const string GREEN   = "\033[32m";
    const string YELLOW  = "\033[33m";
    const string MAGENTA = "\033[35m";
    const string RED     = "\033[31m";

    void header(const string& title) {
        cout << "\n" << BOLD << MAGENTA << "================================================================================\n";
        cout << "  " << title << "\n";
        cout << "================================================================================\n" << RESET;
    }

    void log(const string& tag, const string& msg) {
        cout << CYAN << "  [" << tag << "] " << RESET << msg << "\n";
    }

    void step(const string& step_info) {
        cout << YELLOW << "    ➜ " << RESET << step_info << "\n";
    }

    void success(const string& msg) {
        cout << GREEN << "    ✔ " << BOLD << msg << RESET << "\n";
    }
}

// ============================================================================
// NHÓM 1: CÀI ĐẶT CP (COMPETITIVE PROGRAMMING)
// ============================================================================
namespace CP_HashMap {

    // ------------------------------------------------------------------------
    // 1.1 STATIC HASH MAP VỚI LINEAR PROBING (Tối ưu Cache Locality & Tốc độ)
    // ------------------------------------------------------------------------
    template <size_t SIZE = 10007>
    class StaticLinearProbingMap {
    private:
        int keys[SIZE];
        int values[SIZE];
        bool occupied[SIZE];

        size_t hash(int key) const {
            return (size_t)(key % SIZE + SIZE) % SIZE;
        }

    public:
        StaticLinearProbingMap() {
            fill(occupied, occupied + SIZE, false);
        }

        void insert(int key, int val) {
            size_t idx = hash(key);
            size_t start_idx = idx;
            int steps = 0;

            while (occupied[idx] && keys[idx] != key) {
                idx = (idx + 1) % SIZE;
                steps++;
                if (idx == start_idx) {
                    MagicTrace::step("StaticMap ĐẦY! Không thể chèn.");
                    return;
                }
            }

            keys[idx] = key;
            values[idx] = val;
            occupied[idx] = true;

            MagicTrace::step("INSERT key " + to_string(key) + " ➔ val " + to_string(val) + 
                             " tại index " + to_string(idx) + " (Đụng độ/Probing steps: " + to_string(steps) + ")");
        }

        int get(int key) const {
            size_t idx = hash(key);
            size_t start_idx = idx;

            while (occupied[idx]) {
                if (keys[idx] == key) return values[idx];
                idx = (idx + 1) % SIZE;
                if (idx == start_idx) break;
            }
            return -1; // Not found
        }
    };

    void staticLinearProbingDemo() {
        MagicTrace::header("1.1 STATIC HASH MAP (Linear Probing trên Mảng Tĩnh)");
        StaticLinearProbingMap<13> map; // Dùng size nhỏ 13 để dễ kích hoạt đụng độ
        map.insert(10, 100);
        map.insert(23, 230); // 23 % 13 = 10 -> Đụng độ với 10!
        map.insert(36, 360); // 36 % 13 = 10 -> Tiếp tục đụng độ!

        MagicTrace::log("StaticMap", "Tra cứu Key 23 ➔ Val: " + to_string(map.get(23)));
        MagicTrace::log("StaticMap", "Tra cứu Key 36 ➔ Val: " + to_string(map.get(36)));
        MagicTrace::success("Static Map chạy mượt mà trên mảng tĩnh không cấp phát động!");
    }

    // ------------------------------------------------------------------------
    // 1.2 CUSTOM HASH CHỐNG HACK (SplitMix64 + Time-Based Seed)
    // ------------------------------------------------------------------------
    struct CustomAntiHackHash {
        static uint64_t splitmix64(uint64_t x) {
            x += 0x9e3779b97f4a7c15;
            x = (x ^ (x >> 30)) * 0xbf58476d1ce4e5b9;
            x = (x ^ (x >> 27)) * 0x94d049bb133111eb;
            return x ^ (x >> 31);
        }

        size_t operator()(uint64_t x) const {
            // Random Seed theo thời gian thực mỗi lần chạy chương trình
            static const uint64_t FIXED_RANDOM = chrono::steady_clock::now().time_since_epoch().count();
            return splitmix64(x + FIXED_RANDOM);
        }
    };

    void customHashDemo() {
        MagicTrace::header("1.2 CUSTOM HASH CHỐNG HACK (SplitMix64 + Random Seed)");
        unordered_map<long long, int, CustomAntiHackHash> safe_map;

        MagicTrace::log("AntiHack", "Sinh Hash ngẫu nhiên theo Time-Based Seed cho các Key:");
        CustomAntiHackHash hasher;
        for (long long key : {1LL, 2LL, 1000000007LL}) {
            MagicTrace::step("Key: " + to_string(key) + " ➔ Hash Code: " + to_string(hasher(key)));
            safe_map[key] = key * 2;
        }
        MagicTrace::success("Chống hoàn toàn các bộ test Anti-Hash cố tình ép O(N) trên Codeforces!");
    }

    // ------------------------------------------------------------------------
    // 1.3 GP_HASH_TABLE (__gnu_pbds::gp_hash_table Vũ Khí Bí Mật CP)
    // ------------------------------------------------------------------------
    void gpHashTableDemo() {
        MagicTrace::header("1.3 POLICY-BASED DATA STRUCTURES (gp_hash_table)");
        
        // Khai báo gp_hash_table kết hợp Custom Hash
        __gnu_pbds::gp_hash_table<long long, int, CustomAntiHackHash> fast_map;

        MagicTrace::log("PBDS", "Thêm dữ liệu vào gp_hash_table...");
        fast_map[100] = 1;
        fast_map[200] = 2;

        MagicTrace::step("Tra cứu fast_map[100] = " + to_string(fast_map[100]));
        MagicTrace::step("Tra cứu fast_map[200] = " + to_string(fast_map[200]));
        MagicTrace::success("gp_hash_table đạt tốc độ tra cứu nhanh gấp 3-5 lần std::unordered_map!");
    }
}

// ============================================================================
// NHÓM 2: CÀI ĐẶT PRODUCTION / SYSTEM (REAL-WORLD APPLICATIONS)
// ============================================================================
namespace Production_HashMap {

    // ------------------------------------------------------------------------
    // 2.1 CHAINING HASH MAP VỚI DYNAMIC RESIZING & REHASHING
    // ------------------------------------------------------------------------
    class DynamicChainingHashMap {
    private:
        struct Node {
            string key;
            int val;
            Node* next;
            Node(string k, int v) : key(k), val(v), next(nullptr) {}
        };

        vector<Node*> buckets;
        size_t capacity;
        size_t size;
        const float max_load_factor = 0.75f;

        size_t hash(const string& key) const {
            size_t h = 0;
            for (char c : key) h = h * 31 + c;
            return h % capacity;
        }

        void rehash() {
            size_t old_cap = capacity;
            capacity *= 2;
            vector<Node*> new_buckets(capacity, nullptr);

            MagicTrace::step("⚡ LOAD FACTOR VƯỢT NGLƯỠNG 0.75 ➔ Tiến hành REHASHING (Dung tích: " + 
                             to_string(old_cap) + " ➔ " + to_string(capacity) + ")");

            for (size_t i = 0; i < old_cap; ++i) {
                Node* curr = buckets[i];
                while (curr) {
                    Node* next_node = curr->next;
                    size_t new_idx = hash(curr->key);
                    curr->next = new_buckets[new_idx];
                    new_buckets[new_idx] = curr;
                    curr = next_node;
                }
            }
            buckets = move(new_buckets);
        }

    public:
        DynamicChainingHashMap(size_t initial_cap = 4) : capacity(initial_cap), size(0) {
            buckets.resize(capacity, nullptr);
        }

        ~DynamicChainingHashMap() {
            for (size_t i = 0; i < capacity; ++i) {
                Node* curr = buckets[i];
                while (curr) {
                    Node* temp = curr;
                    curr = curr->next;
                    delete temp;
                }
            }
        }

        void insert(const string& key, int val) {
            if ((float)(size + 1) / capacity > max_load_factor) {
                rehash();
            }

            size_t idx = hash(key);
            Node* curr = buckets[idx];
            while (curr) {
                if (curr->key == key) {
                    curr->val = val;
                    return;
                }
                curr = curr->next;
            }

            Node* new_node = new Node(key, val);
            new_node->next = buckets[idx];
            buckets[idx] = new_node;
            size++;

            MagicTrace::step("INSERT key \"" + key + "\" ➔ Bucket " + to_string(idx) + 
                             " | Size hiện tại: " + to_string(size) + "/" + to_string(capacity) + 
                             " (Load Factor: " + to_string((float)size/capacity).substr(0,4) + ")");
        }

        int get(const string& key) const {
            size_t idx = hash(key);
            Node* curr = buckets[idx];
            while (curr) {
                if (curr->key == key) return curr->val;
                curr = curr->next;
            }
            return -1;
        }
    };

    void dynamicChainingDemo() {
        MagicTrace::header("2.1 CHAINING HASH MAP (Dynamic Resizing & Rehashing)");
        DynamicChainingHashMap map(2); // Khởi tạo dung tích siêu nhỏ 2 để test Rehashing
        map.insert("Apple", 10);
        map.insert("Banana", 20);
        map.insert("Cherry", 30); // Kích hoạt Rehash!
        map.insert("Date", 40);   // Kích hoạt Rehash tiếp!

        MagicTrace::success("Mô phỏng HashMap cơ chế Java/Python tự động mở rộng thành công!");
    }

    // ------------------------------------------------------------------------
    // 2.2 THREAD-SAFE CONCURRENT HASH MAP (Lock Striping / Segment Locks)
    // ------------------------------------------------------------------------
    template <typename K, typename V, size_t SEGMENTS = 4>
    class ConcurrentHashMap {
    private:
        struct Segment {
            unordered_map<K, V> map;
            mutable std::shared_mutex rw_mutex; // Read-Write Lock
        };

        Segment segments[SEGMENTS];
        hash<K> hasher;

        size_t getSegmentIndex(const K& key) const {
            return hasher(key) % SEGMENTS;
        }

    public:
        void put(const K& key, const V& val) {
            size_t seg_idx = getSegmentIndex(key);
            // Chỉ Lock đúng Segment tương ứng, các Segment khác vẫn hoạt động bình thường!
            unique_lock<std::shared_mutex> lock(segments[seg_idx].rw_mutex);
            segments[seg_idx].map[key] = val;
            MagicTrace::step("THREAD [PUT] Key ➔ Khóa Segment " + to_string(seg_idx) + " | Thêm dữ liệu thành công");
        }

        V get(const K& key) const {
            size_t seg_idx = getSegmentIndex(key);
            shared_lock<std::shared_mutex> lock(segments[seg_idx].rw_mutex); // Lock Đọc (Shared Lock)
            auto it = segments[seg_idx].map.find(key);
            if (it != segments[seg_idx].map.end()) return it->second;
            return V();
        }
    };

    void concurrentHashMapDemo() {
        MagicTrace::header("2.2 CONCURRENT HASH MAP (Lock Striping / Segment Locking)");
        ConcurrentHashMap<int, string, 4> cmap;

        MagicTrace::log("ConcurrentMap", "Mô phỏng 2 Thread ghi đồng thời vào các Segment khác nhau:");
        thread t1([&]() { cmap.put(1, "User_Data_1"); });
        thread t2([&]() { cmap.put(2, "User_Data_2"); });

        t1.join();
        t2.join();

        MagicTrace::step("Thread Đọc ➔ Get Key 1: " + cmap.get(1));
        MagicTrace::step("Thread Đọc ➔ Get Key 2: " + cmap.get(2));
        MagicTrace::success("Lock Striping giúp loại bỏ nút thắt cổ chai (Bottleneck) khi xử lý đa luồng!");
    }

    // ------------------------------------------------------------------------
    // 2.3 TTL HASH MAP (Time-To-Live / Map Tự Hủy Phiên Đăng Nhập)
    // ------------------------------------------------------------------------
    class TTLHashMap {
    private:
        struct Entry {
            string value;
            chrono::steady_clock::time_point expire_at;
        };

        unordered_map<string, Entry> store;

    public:
        void put(const string& key, const string& val, int ttl_milliseconds) {
            auto expire = chrono::steady_clock::now() + chrono::milliseconds(ttl_milliseconds);
            store[key] = {val, expire};
            MagicTrace::step("PUT Session \"" + key + "\" ➔ TTL: " + to_string(ttl_milliseconds) + "ms");
        }

        string get(const string& key) {
            auto it = store.find(key);
            if (it == store.end()) {
                MagicTrace::step("GET \"" + key + "\" ➔ KHÔNG TỒN TẠI ❌");
                return "";
            }

            if (chrono::steady_clock::now() > it->second.expire_at) {
                MagicTrace::step("GET \"" + key + "\" ➔ SESSION HẾT HẠN (EXPIRED) ⏰ ➔ Xóa khỏi Map!");
                store.erase(it);
                return "";
            }

            MagicTrace::step("GET \"" + key + "\" ➔ SESSION HỢP LỆ ✔ | Val: " + it->second.value);
            return it->second.value;
        }
    };

    void ttlHashMapDemo() {
        MagicTrace::header("2.3 TTL HASH MAP (Time-To-Live Session Storage)");
        TTLHashMap session_store;
        
        session_store.put("token_abc", "User_Admin", 300); // Sống 300ms

        MagicTrace::log("TTLMap", "Kiểm tra ngay lập tức:");
        session_store.get("token_abc");

        MagicTrace::log("TTLMap", "Chờ 400ms để Token hết hạn...");
        this_thread::sleep_for(chrono::milliseconds(400));

        session_store.get("token_abc"); // Lấy lại sau khi hết hạn
        MagicTrace::success("Cơ chế TTL In-Memory Session hoạt động chuẩn xác!");
    }

    // ------------------------------------------------------------------------
    // 2.4 ROBIN HOOD HASH MAP (Băm Dịch Chuyển Cướp Của Người Giàu Cứu Người Nghèo)
    // ------------------------------------------------------------------------
    class RobinHoodHashMap {
    private:
        struct Element {
            int key;
            int val;
            int psl; // Probe Sequence Length (Độ xa so với vị trí gốc)
            bool occupied = false;
        };

        vector<Element> table;
        size_t capacity;

        size_t hash(int key) const {
            return (size_t)(key % capacity + capacity) % capacity;
        }

    public:
        RobinHoodHashMap(size_t cap = 8) : capacity(cap), table(cap) {}

        void insert(int key, int val) {
            Element incoming = {key, val, 0, true};
            size_t idx = hash(key);

            while (true) {
                if (!table[idx].occupied) {
                    table[idx] = incoming;
                    MagicTrace::step("INSERT Key " + to_string(incoming.key) + 
                                     " tại index " + to_string(idx) + " | PSL = " + to_string(incoming.psl));
                    return;
                }

                // Nếu phần tử đang chèn "nghèo hơn" (ở xa vị trí gốc hơn PSL hiện tại) ➔ Cướp chỗ!
                if (incoming.psl > table[idx].psl) {
                    MagicTrace::step("⚔ ROBIN HOOD SWAP! Key mới " + to_string(incoming.key) + 
                                     " (PSL=" + to_string(incoming.psl) + ") \"NGHÈO HƠN\" Key cũ " + 
                                     to_string(table[idx].key) + " (PSL=" + to_string(table[idx].psl) + 
                                     ") tại index " + to_string(idx) + " ➔ CƯỚP CHỖ!");
                    swap(incoming, table[idx]);
                }

                idx = (idx + 1) % capacity;
                incoming.psl++;
            }
        }
    };

    void robinHoodDemo() {
        MagicTrace::header("2.4 ROBIN HOOD HASH MAP (Tối ưu Variance/Worst-Case Probing)");
        RobinHoodHashMap rh_map(7);
        rh_map.insert(1, 100);
        rh_map.insert(8, 200);  // 8 % 7 = 1 -> Đụng độ với 1 (PSL tăng lên 1)
        rh_map.insert(15, 300); // 15 % 7 = 1 -> Đụng độ tiếp! Cướp chỗ nếu cần thiết!

        MagicTrace::success("Robin Hood Hashing giúp phân bổ dữ liệu cực kỳ đồng đều!");
    }
}

// ============================================================================
// MAIN DEMO RUNNER
// ============================================================================
int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    cout << MagicTrace::BOLD << MagicTrace::CYAN 
         << "================================================================================\n"
         << "         🚀 MASTERCLASS HASH MAP DATA STRUCTURE (CP & PRODUCTION)               \n"
         << "================================================================================\n" 
         << MagicTrace::RESET;

    // --- NHÓM 1: COMPETITIVE PROGRAMMING ---
    CP_HashMap::staticLinearProbingDemo();
    CP_HashMap::customHashDemo();
    CP_HashMap::gpHashTableDemo();

    // --- NHÓM 2: PRODUCTION / SYSTEM ---
    Production_HashMap::dynamicChainingDemo();
    Production_HashMap::concurrentHashMapDemo();
    Production_HashMap::ttlHashMapDemo();
    Production_HashMap::robinHoodDemo();

    cout << "\n" << MagicTrace::BOLD << MagicTrace::GREEN 
         << "================================================================================\n"
         << "   ✔ HOÀN THÀNH TOÀN BỘ CÁC BẢN CÀI ĐẶT HASH MAP MASTERCLASS XUẤT SẮC!         \n"
         << "================================================================================\n\n" 
         << MagicTrace::RESET;

    return 0;
}