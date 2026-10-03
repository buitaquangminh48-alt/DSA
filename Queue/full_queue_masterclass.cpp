#include <iostream>
#include <vector>
#include <string>
#include <queue>
#include <deque>
#include <stack>
#include <unordered_map>
#include <list>
#include <algorithm>
#include <iomanip>

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

    template <typename T>
    string printContainer(T container) {
        string res = "[ ";
        for (const auto& item : container) {
            res += to_string(item) + " ";
        }
        res += "]";
        return res;
    }
}

// ============================================================================
// NHÓM 1: CÀI ĐẶT CP (COMPETITIVE PROGRAMMING)
// ============================================================================
namespace CP_Queue {

    // ------------------------------------------------------------------------
    // 1.1 BFS TRÊN ĐỒ THỊ KHÔNG TRỌNG SỐ (Loang theo từng tầng)
    // ------------------------------------------------------------------------
    void standardBFSDemo() {
        MagicTrace::header("1.1 STANDARD BFS (Tìm đường đi ngắn nhất không trọng số)");
        int num_nodes = 6;
        vector<vector<int>> adj(num_nodes + 1);
        adj[1] = {2, 3};
        adj[2] = {1, 4, 5};
        adj[3] = {1, 6};
        adj[4] = {2}; adj[5] = {2}; adj[6] = {3};

        MagicTrace::log("StandardBFS", "Duyệt BFS từ đỉnh 1 bằng std::queue...");
        queue<int> q;
        vector<int> dist(num_nodes + 1, -1);

        q.push(1);
        dist[1] = 0;

        while (!q.empty()) {
            int u = q.front();
            q.pop();
            MagicTrace::step("Pop đỉnh " + to_string(u) + " | Khoảng cách hiện tại: " + to_string(dist[u]));

            for (int v : adj[u]) {
                if (dist[v] == -1) {
                    dist[v] = dist[u] + 1;
                    q.push(v);
                    MagicTrace::step("  └─ Khám phá đỉnh " + to_string(v) + " ➔ Cập nhật dist[" + to_string(v) + "] = " + to_string(dist[v]) + " & Push vào Queue");
                }
            }
        }

        string result = "";
        for (int i = 1; i <= num_nodes; ++i) {
            result += "Đỉnh " + to_string(i) + ":" + to_string(dist[i]) + " | ";
        }
        MagicTrace::success("Khoảng cách ngắn nhất từ đỉnh 1: " + result);
    }

    // ------------------------------------------------------------------------
    // 1.2 0-1 BFS BẰNG STD::DEQUE (Tối ưu thay thế Dijkstra trong O(V + E))
    // ------------------------------------------------------------------------
    struct Edge {
        int to, weight;
    };

    void zeroOneBFSDemo() {
        MagicTrace::header("1.2 0-1 BFS (Tối ưu đường đi ngắn nhất với trọng số 0 và 1)");
        int num_nodes = 5;
        vector<vector<Edge>> adj(num_nodes + 1);
        adj[1] = {{2, 1}, {3, 0}};
        adj[2] = {{4, 1}};
        adj[3] = {{2, 0}, {5, 1}};
        adj[4] = {}; adj[5] = {{4, 0}};

        MagicTrace::log("0-1 BFS", "Cạnh 0 ➔ Push FRONT | Cạnh 1 ➔ Push BACK (dùng std::deque)...");
        deque<int> dq;
        vector<int> dist(num_nodes + 1, 1e9);

        dq.push_back(1);
        dist[1] = 0;

        while (!dq.empty()) {
            int u = dq.front();
            dq.pop_front();
            MagicTrace::step("Pop FRONT đỉnh " + to_string(u) + " | dist = " + to_string(dist[u]));

            for (auto& edge : adj[u]) {
                int v = edge.to;
                int w = edge.weight;

                if (dist[u] + w < dist[v]) {
                    dist[v] = dist[u] + w;
                    if (w == 0) {
                        dq.push_front(v);
                        MagicTrace::step("  └─ Cạnh 0 ➔ Cập nhật dist[" + to_string(v) + "]=" + to_string(dist[v]) + " ➔ Push FRONT " + to_string(v));
                    } else {
                        dq.push_back(v);
                        MagicTrace::step("  └─ Cạnh 1 ➔ Cập nhật dist[" + to_string(v) + "]=" + to_string(dist[v]) + " ➔ Push BACK " + to_string(v));
                    }
                }
            }
        }
        MagicTrace::success("Khoảng cách ngắn nhất 0-1 BFS từ đỉnh 1 đến đỉnh 4: " + to_string(dist[4]));
    }

    // ------------------------------------------------------------------------
    // 1.3 MONOTONIC QUEUE (Sliding Window Maximum trong O(N))
    // ------------------------------------------------------------------------
    void slidingWindowMaxDemo() {
        MagicTrace::header("1.3 MONOTONIC QUEUE (Sliding Window Maximum)");
        vector<int> nums = {1, 3, -1, -3, 5, 3, 6, 7};
        int k = 3;
        int n = nums.size();
        deque<int> dq; // Lưu index các phần tử giảm dần theo giá trị
        vector<int> result;

        MagicTrace::log("MonotonicQueue", "Duyệt mảng và duy trì Deque giảm dần giá trị...");

        for (int i = 0; i < n; ++i) {
            // Loại bỏ các chỉ số nằm ngoài cửa sổ hiện tại [i - k + 1, i]
            if (!dq.empty() && dq.front() == i - k) {
                MagicTrace::step("Cửa sổ trượt qua ➔ Loại chỉ số " + to_string(dq.front()) + " (giá trị " + to_string(nums[dq.front()]) + ") ở đầu Deque");
                dq.pop_front();
            }

            // Loại bỏ các phần tử ở đuôi Deque nhỏ hơn hoặc bằng nums[i]
            while (!dq.empty() && nums[dq.back()] <= nums[i]) {
                MagicTrace::step("Pop đuôi Deque index " + to_string(dq.back()) + " (giá trị " + to_string(nums[dq.back()]) + " <= " + to_string(nums[i]) + ")");
                dq.pop_back();
            }

            dq.push_back(i);
            MagicTrace::step("Push BACK index " + to_string(i) + " (giá trị " + to_string(nums[i]) + ")");

            if (i >= k - 1) {
                result.push_back(nums[dq.front()]);
                MagicTrace::step("  ★ Max cửa sổ [" + to_string(i - k + 1) + ".." + to_string(i) + "] là: " + to_string(nums[dq.front()]));
            }
        }
        MagicTrace::success("Kết quả Sliding Window Maximum: " + MagicTrace::printContainer(result));
    }

    // ------------------------------------------------------------------------
    // 1.4 QUEUE USING TWO STACKS (Khấu hao Amortized O(1))
    // ------------------------------------------------------------------------
    class QueueTwoStacks {
    private:
        stack<int> in_stack, out_stack;

        void transfer() {
            if (out_stack.empty()) {
                MagicTrace::step("out_stack RỖNG ➔ Đổ toàn bộ " + to_string(in_stack.size()) + " phần tử từ in_stack sang out_stack!");
                while (!in_stack.empty()) {
                    out_stack.push(in_stack.top());
                    in_stack.pop();
                }
            }
        }

    public:
        void enqueue(int val) {
            in_stack.push(val);
            MagicTrace::step("ENQUEUE(" + to_string(val) + ") ➔ Push vào in_stack | Size in_stack: " + to_string(in_stack.size()));
        }

        int dequeue() {
            transfer();
            if (out_stack.empty()) return -1;
            int val = out_stack.top();
            out_stack.pop();
            MagicTrace::step("DEQUEUE() ➔ Lấy " + to_string(val) + " từ out_stack");
            return val;
        }
    };

    void queueTwoStacksDemo() {
        MagicTrace::header("1.4 QUEUE USING TWO STACKS (Mô phỏng FIFO bằng 2 Stack)");
        QueueTwoStacks q;
        q.enqueue(10);
        q.enqueue(20);
        q.enqueue(30);
        q.dequeue();
        q.enqueue(40);
        q.dequeue();
        q.dequeue();
        MagicTrace::success("Mô phỏng Queue bằng 2 Stack thành công!");
    }
}

// ============================================================================
// NHÓM 2: CÀI ĐẶT PRODUCTION / SYSTEM (REAL-WORLD APPLICATIONS)
// ============================================================================
namespace Production_Queue {

    // ------------------------------------------------------------------------
    // 2.1 LRU CACHE (Least Recently Used Cache)
    // ------------------------------------------------------------------------
    class LRUCache {
    private:
        int capacity;
        list<pair<int, int>> cache_list; // Doubly Linked List chứa {key, value}
        unordered_map<int, list<pair<int, int>>::iterator> cache_map;

    public:
        LRUCache(int cap) : capacity(cap) {}

        int get(int key) {
            if (cache_map.find(key) == cache_map.end()) {
                MagicTrace::step("GET(" + to_string(key) + ") ➔ CACHE MISS ❌");
                return -1;
            }
            // Di chuyển phần tử vừa được truy cập lên đầu danh sách (Most Recently Used)
            cache_list.splice(cache_list.begin(), cache_list, cache_map[key]);
            MagicTrace::step("GET(" + to_string(key) + ") ➔ CACHE HIT ✔ | Giá trị: " + to_string(cache_map[key]->second) + " ➔ Đẩy lên ĐẦU Cache");
            return cache_map[key]->second;
        }

        void put(int key, int value) {
            if (cache_map.find(key) != cache_map.end()) {
                cache_map[key]->second = value;
                cache_list.splice(cache_list.begin(), cache_list, cache_map[key]);
                MagicTrace::step("PUT(" + to_string(key) + ", " + to_string(value) + ") ➔ Cập nhật giá trị cũ & Đẩy lên ĐẦU Cache");
                return;
            }

            if (cache_list.size() == capacity) {
                int evict_key = cache_list.back().first;
                MagicTrace::step("CACHE ĐẦY! Loại bỏ phần tử LRU ở ĐUÔI Cache (Key: " + to_string(evict_key) + ")");
                cache_map.erase(evict_key);
                cache_list.pop_back();
            }

            cache_list.push_front({key, value});
            cache_map[key] = cache_list.begin();
            MagicTrace::step("PUT(" + to_string(key) + ", " + to_string(value) + ") ➔ Thêm thành công vào ĐẦU Cache");
        }
    };

    void lruCacheDemo() {
        MagicTrace::header("2.1 LRU CACHE (Hash Map + Doubly Linked List / Deque)");
        LRUCache cache(2);
        cache.put(1, 100);
        cache.put(2, 200);
        cache.get(1);      // Key 1 được dùng ➔ Key 2 thành LRU
        cache.put(3, 300); // Tràn bộ nhớ ➔ Xóa Key 2
        cache.get(2);      // Returns -1 (MISS)
        cache.get(3);      // Returns 300 (HIT)
        MagicTrace::success("Vận hành LRU Cache chính xác!");
    }

    // ------------------------------------------------------------------------
    // 2.2 IN-MEMORY MESSAGE QUEUE (Mô hình Producer - Consumer)
    // ------------------------------------------------------------------------
    struct Message {
        int id;
        string payload;
    };

    class MessageQueue {
    private:
        queue<Message> msg_queue;
        size_t max_size;

    public:
        MessageQueue(size_t limit) : max_size(limit) {}

        bool produce(const Message& msg) {
            if (msg_queue.size() >= max_size) {
                MagicTrace::step("PRODUCER: Message Queue ĐẦY (" + to_string(msg_queue.size()) + "/" + to_string(max_size) + ") ➔ Bị nghẽn (Backpressure)!");
                return false;
            }
            msg_queue.push(msg);
            MagicTrace::step("PRODUCER 📤: Đã đẩy Msg ID " + to_string(msg.id) + " [\"" + msg.payload + "\"] vào Queue");
            return true;
        }

        void consume() {
            if (msg_queue.empty()) {
                MagicTrace::step("CONSUMER: Queue RỖNG ➔ Chờ tin nhắn mới...");
                return;
            }
            Message msg = msg_queue.front();
            msg_queue.pop();
            MagicTrace::step("CONSUMER 📥: Đã xử lý thành công Msg ID " + to_string(msg.id) + " [\"" + msg.payload + "\"]");
        }
    };

    void messageQueueDemo() {
        MagicTrace::header("2.2 IN-MEMORY MESSAGE QUEUE (Mô hình Producer - Consumer)");
        MessageQueue mq(2);
        mq.produce({101, "SEND_EMAIL_OTP"});
        mq.produce({102, "GENERATE_PDF_REPORT"});
        mq.produce({103, "PROCESS_PAYMENT"}); // Tràn queue
        mq.consume(); // Nhường chỗ
        mq.produce({103, "PROCESS_PAYMENT"}); // Đẩy lại
        mq.consume();
        mq.consume();
        MagicTrace::success("Mô phỏng Producer-Consumer hoàn tất!");
    }

    // ------------------------------------------------------------------------
    // 2.3 CIRCULAR QUEUE / RING BUFFER (Hàng đợi vòng lặp cố định cho Embedded)
    // ------------------------------------------------------------------------
    class RingBuffer {
    private:
        vector<int> buffer;
        int head = 0;
        int tail = 0;
        int count = 0;
        int capacity;

    public:
        RingBuffer(int cap) : capacity(cap), buffer(cap) {}

        bool push(int val) {
            if (count == capacity) {
                MagicTrace::step("RING BUFFER FULL! Không thể push " + to_string(val));
                return false;
            }
            buffer[tail] = val;
            MagicTrace::step("PUSH(" + to_string(val) + ") tại vị trí index " + to_string(tail));
            tail = (tail + 1) % capacity; // Quay vòng chỉ số
            count++;
            return true;
        }

        int pop() {
            if (count == 0) {
                MagicTrace::step("RING BUFFER EMPTY!");
                return -1;
            }
            int val = buffer[head];
            MagicTrace::step("POP() ➔ Lấy giá trị " + to_string(val) + " tại index " + to_string(head));
            head = (head + 1) % capacity; // Quay vòng chỉ số
            count--;
            return val;
        }
    };

    void ringBufferDemo() {
        MagicTrace::header("2.3 CIRCULAR QUEUE / RING BUFFER (Hàng đợi tĩnh chia lấy dư)");
        RingBuffer rb(3);
        rb.push(10);
        rb.push(20);
        rb.push(30);
        rb.pop(); // Giải phóng vị trí index 0
        rb.push(40); // Tái sử dụng index 0 thành công nhờ toán tử %
        rb.pop();
        rb.pop();
        rb.pop();
        MagicTrace::success("Quản lý bộ nhớ Ring Buffer hoàn hảo!");
    }

    // ------------------------------------------------------------------------
    // 2.4 PRIORITY QUEUE TASK SCHEDULER (Bộ điều phối tiến trình OS)
    // ------------------------------------------------------------------------
    struct Task {
        int id;
        string name;
        int priority; // Priority càng cao xử lý càng trước

        bool operator<(const Task& other) const {
            return priority < other.priority; // Max-Heap cho Priority Queue
        }
    };

    void priorityTaskSchedulerDemo() {
        MagicTrace::header("2.4 PRIORITY QUEUE TASK SCHEDULER (Bộ điều phối tiến trình OS)");
        priority_queue<Task> task_pq;

        MagicTrace::log("TaskScheduler", "Thêm các tiến trình hệ thống vào Priority Queue...");
        task_pq.push({1, "Background Disk Cleanup", 1});
        task_pq.push({2, "User UI Mouse Click Response", 10});
        task_pq.push({3, "Network Audio Packet Stream", 8});
        task_pq.push({4, "Render Video Frame", 5});

        while (!task_pq.empty()) {
            Task top_task = task_pq.top();
            task_pq.pop();
            MagicTrace::step("EXECUTE Task ID " + to_string(top_task.id) + " [\"" + top_task.name + "\"] | Priority level: " + to_string(top_task.priority));
        }
        MagicTrace::success("Toàn bộ tiến trình được điều phối đúng thứ tự ưu tiên!");
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
         << "         🚀 MASTERCLASS QUEUE DATA STRUCTURE (CP & PRODUCTION)                  \n"
         << "================================================================================\n" 
         << MagicTrace::RESET;

    // --- NHÓM 1: COMPETITIVE PROGRAMMING ---
    CP_Queue::standardBFSDemo();
    CP_Queue::zeroOneBFSDemo();
    CP_Queue::slidingWindowMaxDemo();
    CP_Queue::queueTwoStacksDemo();

    // --- NHÓM 2: PRODUCTION / SYSTEM ---
    Production_Queue::lruCacheDemo();
    Production_Queue::messageQueueDemo();
    Production_Queue::ringBufferDemo();
    Production_Queue::priorityTaskSchedulerDemo();

    cout << "\n" << MagicTrace::BOLD << MagicTrace::GREEN 
         << "================================================================================\n"
         << "   ✔ HOÀN THÀNH TOÀN BỘ CÁC BẢN CÀI ĐẶT QUEUE MASTERCLASS XUẤT SẮC!           \n"
         << "================================================================================\n\n" 
         << MagicTrace::RESET;

    return 0;
}