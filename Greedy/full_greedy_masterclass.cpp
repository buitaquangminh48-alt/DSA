#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <queue>
#include <numeric>
#include <iomanip>
#include <bitset>
#include <map>

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
namespace CP_Greedy {

    // ------------------------------------------------------------------------
    // 1.1 INTERVAL SCHEDULING (Chọn nhiều công việc không trùng lịch nhất)
    // ------------------------------------------------------------------------
    struct Interval {
        int id, start, finish;
    };

    void intervalSchedulingDemo() {
        MagicTrace::header("1.1 INTERVAL SCHEDULING (Gia công theo Finish Time)");
        vector<Interval> jobs = {{1, 1, 3}, {2, 2, 5}, {3, 4, 7}, {4, 1, 8}, {5, 5, 9}, {6, 8, 10}};

        MagicTrace::log("IntervalScheduling", "Sắp xếp danh sách công việc theo thời gian KẾT THÚC tăng dần...");
        sort(jobs.begin(), jobs.end(), [](const Interval& a, const Interval& b) {
            return a.finish < b.finish;
        });

        int selected_count = 0;
        int last_finish_time = -1;

        for (const auto& job : jobs) {
            if (job.start >= last_finish_time) {
                selected_count++;
                MagicTrace::step("CHỌN Job " + to_string(job.id) + " [" + to_string(job.start) + " -> " + to_string(job.finish) + "] | Finish Time mới = " + to_string(job.finish));
                last_finish_time = job.finish;
            } else {
                MagicTrace::step("BỎ QUA Job " + to_string(job.id) + " [" + to_string(job.start) + " -> " + to_string(job.finish) + "] (Trùng lịch với công việc trước)");
            }
        }
        MagicTrace::success("Số lượng cuộc họp tối đa tổ chức được: " + to_string(selected_count));
    }

    // ------------------------------------------------------------------------
    // 1.2 INTERVAL PARTITIONING (Xếp phòng họp tối thiểu bằng Priority Queue)
    // ------------------------------------------------------------------------
    void intervalPartitioningDemo() {
        MagicTrace::header("1.2 INTERVAL PARTITIONING (Min Halls/Rooms Required)");
        vector<Interval> meetings = {{1, 0, 30}, {2, 5, 10}, {3, 15, 20}, {4, 10, 15}};

        MagicTrace::log("IntervalPartitioning", "Sắp xếp cuộc họp theo thời gian BẮT ĐẦU...");
        sort(meetings.begin(), meetings.end(), [](const Interval& a, const Interval& b) {
            return a.start < b.start;
        });

        // Min-heap chứa thời gian kết thúc của các phòng đang mở
        priority_queue<int, vector<int>, greater<int>> min_heap;

        for (const auto& m : meetings) {
            if (!min_heap.empty() && min_heap.top() <= m.start) {
                MagicTrace::step("Cuộc họp " + to_string(m.id) + " [" + to_string(m.start) + "->" + to_string(m.finish) + "] TÁI SỬ DỤNG phòng trống rảnh lúc " + to_string(min_heap.top()));
                min_heap.pop();
            } else {
                MagicTrace::step("Cuộc họp " + to_string(m.id) + " [" + to_string(m.start) + "->" + to_string(m.finish) + "] MỞ PHÒNG MỚI (Tất cả các phòng cũ còn bận)");
            }
            min_heap.push(m.finish);
        }
        MagicTrace::success("Số phòng họp tối thiểu cần mở: " + to_string(min_heap.size()));
    }

    // ------------------------------------------------------------------------
    // 1.3 FRACTIONAL KNAPSACK (Bài toán Cái túi phân thân)
    // ------------------------------------------------------------------------
    struct Item {
        int id;
        double weight, value;
        double ratio() const { return value / weight; }
    };

    void fractionalKnapsackDemo() {
        MagicTrace::header("1.3 FRACTIONAL KNAPSACK (Tham lam theo Tỷ lệ Giá trị / Khối lượng)");
        vector<Item> items = {{1, 10, 60}, {2, 20, 100}, {3, 30, 120}};
        double capacity = 50.0;

        MagicTrace::log("FractionalKnapsack", "Sắp xếp vật phẩm theo Unit Value (value/weight) giảm dần...");
        sort(items.begin(), items.end(), [](const Item& a, const Item& b) {
            return a.ratio() > b.ratio();
        });

        double total_value = 0.0;
        for (const auto& item : items) {
            if (capacity <= 0) break;
            if (item.weight <= capacity) {
                capacity -= item.weight;
                total_value += item.value;
                MagicTrace::step("Lấy 100% Vật phẩm " + to_string(item.id) + " (W: " + to_string((int)item.weight) + ", V: " + to_string((int)item.value) + ") | Sức chứa còn lại: " + to_string((int)capacity));
            } else {
                double fraction = capacity / item.weight;
                total_value += item.value * fraction;
                MagicTrace::step("Lấy " + to_string((int)(fraction * 100)) + "% Vật phẩm " + to_string(item.id) + " (W lấy: " + to_string((int)capacity) + ") | Túi ĐÃ ĐẦY!");
                capacity = 0;
            }
        }
        MagicTrace::success("Tổng giá trị tối đa thu được: " + to_string(total_value));
    }

    // ------------------------------------------------------------------------
    // 1.4 KRUSKAL MST (Cây bao trùm tối thiểu - Minimum Spanning Tree)
    // ------------------------------------------------------------------------
    struct Edge {
        int u, v, weight;
    };

    struct DSU {
        vector<int> parent;
        DSU(int n) { parent.resize(n + 1); iota(parent.begin(), parent.end(), 0); }
        int find(int i) { return (parent[i] == i) ? i : (parent[i] = find(parent[i])); }
        bool unite(int i, int j) {
            int root_i = find(i), root_j = find(j);
            if (root_i != root_j) { parent[root_i] = root_j; return true; }
            return false;
        }
    };

    void kruskalMSTDemo() {
        MagicTrace::header("1.4 KRUSKAL MST (Chọn cạnh nhỏ nhất không tạo chu trình)");
        int num_vertices = 4;
        vector<Edge> edges = {{1, 2, 10}, {1, 3, 6}, {1, 4, 5}, {2, 4, 15}, {3, 4, 4}};

        MagicTrace::log("KruskalMST", "Sắp xếp danh sách cạnh theo trọng số tăng dần...");
        sort(edges.begin(), edges.end(), [](const Edge& a, const Edge& b) {
            return a.weight < b.weight;
        });

        DSU dsu(num_vertices);
        int mst_weight = 0, edges_count = 0;

        for (const auto& edge : edges) {
            if (dsu.unite(edge.u, edge.v)) {
                mst_weight += edge.weight;
                edges_count++;
                MagicTrace::step("CHỌN cạnh (" + to_string(edge.u) + " - " + to_string(edge.v) + ") Trọng số: " + to_string(edge.weight) + " | Thêm vào MST");
            } else {
                MagicTrace::step("BỎ QUA cạnh (" + to_string(edge.u) + " - " + to_string(edge.v) + ") Trọng số: " + to_string(edge.weight) + " (Tạo thành chu trình!)");
            }
        }
        MagicTrace::success("Tổng trọng số Cây Bao Trùm Tối Thiểu (MST): " + to_string(mst_weight));
    }
}

// ============================================================================
// NHÓM 2: CÀI ĐẶT PRODUCTION / SYSTEM (HEURISTIC & REAL-WORLD)
// ============================================================================
namespace Production_Greedy {

    // ------------------------------------------------------------------------
    // 2.1 HUFFMAN COMPRESSION ENGINE (Mã hóa văn bản thành Bitstream)
    // ------------------------------------------------------------------------
    struct HuffmanNode {
        char ch;
        int freq;
        HuffmanNode *left, *right;
        HuffmanNode(char c, int f) : ch(c), freq(f), left(nullptr), right(nullptr) {}
    };

    struct CompareHuffman {
        bool operator()(HuffmanNode* l, HuffmanNode* r) { return l->freq > r->freq; }
    };

    class HuffmanEngine {
    private:
        map<char, string> huffman_codes;
        HuffmanNode* root = nullptr;

        void generateCodes(HuffmanNode* node, string code) {
            if (!node) return;
            if (!node->left && !node->right) {
                huffman_codes[node->ch] = code;
            }
            generateCodes(node->left, code + "0");
            generateCodes(node->right, code + "1");
        }

    public:
        void buildAndEncode(const string& text) {
            MagicTrace::header("2.1 HUFFMAN COMPRESSION ENGINE (Xây dựng Cây Huffman từ dưới lên)");
            map<char, int> freq_map;
            for (char c : text) freq_map[c]++;

            priority_queue<HuffmanNode*, vector<HuffmanNode*>, CompareHuffman> min_pq;
            for (auto pair : freq_map) {
                min_pq.push(new HuffmanNode(pair.first, pair.second));
            }

            MagicTrace::log("HuffmanEngine", "Gom 2 nút có tần suất nhỏ nhất liên tục qua Priority Queue...");
            while (min_pq.size() > 1) {
                HuffmanNode* left = min_pq.top(); min_pq.pop();
                HuffmanNode* right = min_pq.top(); min_pq.pop();

                HuffmanNode* parent = new HuffmanNode('$', left->freq + right->freq);
                parent->left = left;
                parent->right = right;

                MagicTrace::step("Gom cụm: Nút L(" + (left->ch == '$' ? "Gộp" : string(1, left->ch)) + ":" + to_string(left->freq) + 
                                ") + Nút R(" + (right->ch == '$' ? "Gộp" : string(1, right->ch)) + ":" + to_string(right->freq) + 
                                ") ➔ Parent freq = " + to_string(parent->freq));
                min_pq.push(parent);
            }

            root = min_pq.top();
            generateCodes(root, "");

            MagicTrace::log("HuffmanEngine", "Bảng mã hóa Bit mã hóa sinh ra:");
            for (auto pair : huffman_codes) {
                MagicTrace::step("Ký tự '" + string(1, pair.first) + "' ➔ Mã Bit: " + pair.second);
            }

            string encoded = "";
            for (char c : text) encoded += huffman_codes[c];
            MagicTrace::success("Văn bản ban đầu (" + to_string(text.length() * 8) + " bits) ➔ Văn bản nén (" + to_string(encoded.length()) + " bits): " + encoded);
        }
    };

    // ------------------------------------------------------------------------
    // 2.2 BANDWIDTH THROTTLING & NETWORK PACKET SCHEDULING (Shortest Job First)
    // ------------------------------------------------------------------------
    struct Packet {
        int id;
        int size_bytes; // Kích thước gói tin
        int latency_sensitive;
    };

    void networkPacketSchedulingDemo() {
        MagicTrace::header("2.2 BANDWIDTH THROTTLING & PACKET SCHEDULING (Shortest Job First)");
        vector<Packet> packets = {{101, 1500, 1}, {102, 64, 5}, {103, 512, 3}, {104, 128, 4}};

        MagicTrace::log("PacketScheduler", "Sắp xếp ưu tiên các gói tin có kích thước NHỎ NHẤT đi trước để tối ưu Latency trung bình...");
        sort(packets.begin(), packets.end(), [](const Packet& a, const Packet& b) {
            return a.size_bytes < b.size_bytes;
        });

        int current_time = 0;
        int total_wait_time = 0;
        for (const auto& p : packets) {
            MagicTrace::step("ĐẨY GÓI TIN " + to_string(p.id) + " (" + to_string(p.size_bytes) + " Bytes) qua Router | Thời gian chờ: " + to_string(current_time) + "ms");
            total_wait_time += current_time;
            current_time += p.size_bytes / 10; // Giả lập thời gian truyền
        }
        MagicTrace::success("Thời gian chờ trung bình trung bình của gói tin tối ưu đạt: " + to_string((double)total_wait_time / packets.size()) + "ms");
    }

    // ------------------------------------------------------------------------
    // 2.3 CLOUD VM PLACEMENT (First-Fit Decreasing heuristic)
    // ------------------------------------------------------------------------
    void cloudVMPlacementDemo() {
        MagicTrace::header("2.3 CLOUD VIRTUAL MACHINE PLACEMENT (First-Fit Decreasing Bin Packing)");
        vector<int> vm_ram_req = {8, 16, 32, 4, 16, 8, 32}; // Yêu cầu RAM (GB) của các VM
        int server_capacity = 64; // Dung lượng RAM tối đa của 1 Server vật lý

        MagicTrace::log("CloudPlacement", "Sắp xếp danh sách VM theo nhu cầu RAM GIẢM DẦN...");
        sort(vm_ram_req.rbegin(), vm_ram_req.rend());

        vector<int> servers; // Lưu dung lượng RAM đã dùng của từng server

        for (int ram : vm_ram_req) {
            bool placed = false;
            for (size_t i = 0; i < servers.size(); ++i) {
                if (servers[i] + ram <= server_capacity) {
                    servers[i] += ram;
                    MagicTrace::step("Nhét VM " + to_string(ram) + "GB vào Server [" + to_string(i + 1) + "] | RAM Server dùng: " + to_string(servers[i]) + "/" + to_string(server_capacity) + "GB");
                    placed = true;
                    break;
                }
            }
            if (!placed) {
                servers.push_back(ram);
                MagicTrace::step("MỞ SERVER MỚI [" + to_string(servers.size()) + "] nhét VM " + to_string(ram) + "GB | RAM Server dùng: " + to_string(ram) + "/" + to_string(server_capacity) + "GB");
            }
        }
        MagicTrace::success("Tối ưu hóa gom cụm thành công: Chỉ cần dùng " + to_string(servers.size()) + " Server vật lý!");
    }

    // ------------------------------------------------------------------------
    // 2.4 CASH REGISTER / CHANGE-MAKING (Hệ thống thối tiền tự động ATM/POS)
    // ------------------------------------------------------------------------
    void changeMakingDemo() {
        MagicTrace::header("2.4 CASH REGISTER / CHANGE-MAKING (Thối tiền mệnh giá lớn trước)");
        vector<int> denominations = {500, 200, 100, 50, 20, 10, 5, 2, 1}; // Các mệnh giá VND (nghìn đồng)
        int change_needed = 887; // Cần thối 887 nghìn đồng

        MagicTrace::log("CashRegister", "Ưu tiên rút các tờ tiền có MỆNH GIÁ LỚN NHẤT có thể...");
        int total_notes = 0;

        for (int coin : denominations) {
            if (change_needed >= coin) {
                int count = change_needed / coin;
                change_needed %= coin;
                total_notes += count;
                MagicTrace::step("Rút " + to_string(count) + " tờ mệnh giá [" + to_string(coin) + "k] | Tiền thối còn lại: " + to_string(change_needed) + "k");
            }
        }
        MagicTrace::success("Tổng số lượng tờ tiền tối thiểu cần đưa cho khách: " + to_string(total_notes) + " tờ");
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
         << "     🚀 MASTERCLASS GREEDY ALGORITHMS (COMPETITIVE PROGRAMMING & SYSTEM)        \n"
         << "================================================================================\n" 
         << MagicTrace::RESET;

    // --- NHÓM 1: COMPETITIVE PROGRAMMING ---
    CP_Greedy::intervalSchedulingDemo();
    CP_Greedy::intervalPartitioningDemo();
    CP_Greedy::fractionalKnapsackDemo();
    CP_Greedy::kruskalMSTDemo();

    // --- NHÓM 2: PRODUCTION / SYSTEM ---
    Production_Greedy::HuffmanEngine huffman;
    huffman.buildAndEncode("ABRACADABRA");

    Production_Greedy::networkPacketSchedulingDemo();
    Production_Greedy::cloudVMPlacementDemo();
    Production_Greedy::changeMakingDemo();

    cout << "\n" << MagicTrace::BOLD << MagicTrace::GREEN 
         << "================================================================================\n"
         << "   ✔ HOÀN THÀNH TOÀN BỘ CÁC BẢN CÀI ĐẶT GREEDY MASTERCLASS XUẤT SẮC!           \n"
         << "================================================================================\n\n" 
         << MagicTrace::RESET;

    return 0;
}