#include <iostream>
#include <vector>
#include <string>
#include <stack>
#include <queue>
#include <memory>
#include <mutex>
#include <shared_mutex>
#include <random>
#include <chrono>
#include <thread>
#include <algorithm>

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

// Struct Node đơn giản cho các thuật toán duyệt cây
struct TreeNode {
    int val;
    TreeNode* left;
    TreeNode* right;
    TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
};

// ============================================================================
// NHÓM 1: CÀI ĐẶT CP (COMPETITIVE PROGRAMMING & ADVANCED TREE)
// ============================================================================
namespace CP_BinaryTree {

    // ------------------------------------------------------------------------
    // 1.1 DUYỆT CÂY KHÔNG ĐỆ QUY (Iterative In-order / Pre-order / Post-order)
    // ------------------------------------------------------------------------
    void iterativeInorder(TreeNode* root) {
        MagicTrace::step("Bắt đầu Iterative In-order Traversal (Trái - Gốc - Phải) dùng Stack:");
        stack<TreeNode*> st;
        TreeNode* curr = root;
        string result = "";

        while (curr != nullptr || !st.empty()) {
            while (curr != nullptr) {
                st.push(curr);
                curr = curr->left;
            }
            curr = st.top();
            st.pop();
            result += to_string(curr->val) + " ";
            curr = curr->right;
        }
        MagicTrace::log("IterativeInOrder", "Kết quả Duyệt: " + result);
    }

    // ------------------------------------------------------------------------
    // 1.2 MORRIS TRAVERSAL (Duyệt Cây In-order O(1) Bộ Nhớ Phụ)
    // ------------------------------------------------------------------------
    void morrisTraversal(TreeNode* root) {
        MagicTrace::header("1.2 MORRIS TRAVERSAL (Duyệt Cây In-order Không Stack - O(1) Memory)");
        TreeNode* curr = root;
        string result = "";

        while (curr != nullptr) {
            if (curr->left == nullptr) {
                result += to_string(curr->val) + " ";
                curr = curr->right;
            } else {
                // Tìm In-order Predecessor (Nút phải nhất của cây con bên trái)
                TreeNode* predecessor = curr->left;
                while (predecessor->right != nullptr && predecessor->right != curr) {
                    predecessor = predecessor->right;
                }

                if (predecessor->right == nullptr) {
                    // Tạo Thread tạm thời trỏ về Node hiện tại
                    predecessor->right = curr;
                    MagicTrace::step("Tạo Thread tạm thời: Node " + to_string(predecessor->val) + " ➔ " + to_string(curr->val));
                    curr = curr->left;
                } else {
                    // Xóa Thread tạm thời khôi phục lại cấu trúc gốc
                    predecessor->right = nullptr;
                    MagicTrace::step("Tháo Thread tạm thời tại Node " + to_string(predecessor->val));
                    result += to_string(curr->val) + " ";
                    curr = curr->right;
                }
            }
        }
        MagicTrace::log("MorrisTraversal", "Kết quả Duyệt In-order: " + result);
        MagicTrace::success("Duyệt cây hoàn thành mà không sử dụng thêm Stack hay Đệ quy!");
    }

    // ------------------------------------------------------------------------
    // 1.3 TREAP (Randomized Search Tree - Cây BST kết hợp Heap)
    // ------------------------------------------------------------------------
    struct TreapNode {
        int key, priority;
        TreapNode *left, *right;
        TreapNode(int k) : key(k), priority(rand()), left(nullptr), right(nullptr) {}
    };

    void rotateRight(TreapNode* &root) {
        TreapNode* L = root->left;
        root->left = L->right;
        L->right = root;
        root = L;
    }

    void rotateLeft(TreapNode* &root) {
        TreapNode* R = root->right;
        root->right = R->left;
        R->left = root;
        root = R;
    }

    void treapInsert(TreapNode* &root, int key) {
        if (!root) {
            root = new TreapNode(key);
            MagicTrace::step("Tạo TreapNode (Key: " + to_string(key) + " | Priority: " + to_string(root->priority) + ")");
            return;
        }
        if (key < root->key) {
            treapInsert(root->left, key);
            if (root->left->priority > root->priority) {
                MagicTrace::step("Quay Phải khôi phục tính chất Heap tại Key " + to_string(root->key));
                rotateRight(root);
            }
        } else {
            treapInsert(root->right, key);
            if (root->right->priority > root->priority) {
                MagicTrace::step("Quay Trái khôi phục tính chất Heap tại Key " + to_string(root->key));
                rotateLeft(root);
            }
        }
    }

    void treapDemo() {
        MagicTrace::header("1.3 TREAP (Randomized Binary Search Tree)");
        TreapNode* root = nullptr;
        for (int k : {50, 30, 70, 20, 40}) {
            treapInsert(root, k);
        }
        MagicTrace::success("Treap giúp cân bằng BST ngẫu nhiên với chiều cao trung bình O(log N)!");
    }

    // ------------------------------------------------------------------------
    // 1.4 SPLAY TREE (Cây Tự Tái Cấu Trúc Đẩy Phần Tử Vừa Dùng Lên Gốc)
    // ------------------------------------------------------------------------
    struct SplayNode {
        int key;
        SplayNode *left, *right;
        SplayNode(int k) : key(k), left(nullptr), right(nullptr) {}
    };

    SplayNode* rightRotateSplay(SplayNode* x) {
        SplayNode* y = x->left;
        x->left = y->right;
        y->right = x;
        return y;
    }

    SplayNode* leftRotateSplay(SplayNode* x) {
        SplayNode* y = x->right;
        x->right = y->left;
        y->left = x;
        return y;
    }

    SplayNode* splay(SplayNode* root, int key) {
        if (!root || root->key == key) return root;

        if (root->key > key) {
            if (!root->left) return root;
            if (root->left->key > key) { // Zig-Zig (Trái Trái)
                root->left->left = splay(root->left->left, key);
                root = rightRotateSplay(root);
            } else if (root->left->key < key) { // Zig-Zag (Trái Phải)
                root->left->right = splay(root->left->right, key);
                if (root->left->right) root->left = leftRotateSplay(root->left);
            }
            return root->left ? rightRotateSplay(root) : root;
        } else {
            if (!root->right) return root;
            if (root->right->key > key) { // Zag-Zig (Phải Trái)
                root->right->left = splay(root->right->left, key);
                if (root->right->left) root->right = rightRotateSplay(root->right);
            } else if (root->right->key < key) { // Zag-Zag (Phải Phải)
                root->right->right = splay(root->right->right, key);
                root = leftRotateSplay(root);
            }
            return root->right ? leftRotateSplay(root) : root;
        }
    }

    void splayTreeDemo() {
        MagicTrace::header("1.4 SPLAY TREE (Tự Tái Cấu Trúc Đẩy Key Vừa Truy Cập Lên Gốc)");
        SplayNode* root = new SplayNode(100);
        root->left = new SplayNode(50);
        root->left->left = new SplayNode(20);

        MagicTrace::log("SplayTree", "Truy cập Key 20...");
        root = splay(root, 20);
        MagicTrace::step("Key 20 đã được Splay (Đẩy) lên làm GỐC mới của cây! Gốc hiện tại: " + to_string(root->key));
        MagicTrace::success("Splay Tree tối ưu vượt trội cho các truy vấn có tính chất locality!");
    }

    // ------------------------------------------------------------------------
    // 1.5 EULER TOUR FLATTENING (Trải Phẳng Cây Thành Mảng Cho Segment Tree/BIT)
    // ------------------------------------------------------------------------
    void eulerTourDFS(int node, int parent, const vector<vector<int>>& adj, 
                      vector<int>& in, vector<int>& out, int& timer) {
        in[node] = ++timer;
        MagicTrace::step("DFS vào Node " + to_string(node) + " ➔ IN_TIME = " + to_string(in[node]));
        
        for (int neighbor : adj[node]) {
            if (neighbor != parent) {
                eulerTourDFS(neighbor, node, adj, in, out, timer);
            }
        }
        
        out[node] = timer;
        MagicTrace::step("DFS thoát Node " + to_string(node) + " ➔ OUT_TIME = " + to_string(out[node]) + 
                         " (Đoạn Cây Con tương ứng: [" + to_string(in[node]) + ", " + to_string(out[node]) + "])");
    }

    void eulerTourDemo() {
        MagicTrace::header("1.5 EULER TOUR (Trải Phẳng Cây Thành Mảng 1D)");
        int n = 4;
        vector<vector<int>> adj(n + 1);
        // Cây dạng: 1 -> 2, 1 -> 3, 2 -> 4
        adj[1].push_back(2); adj[2].push_back(1);
        adj[1].push_back(3); adj[3].push_back(1);
        adj[2].push_back(4); adj[4].push_back(2);

        vector<int> in(n + 1), out(n + 1);
        int timer = 0;
        eulerTourDFS(1, 0, adj, in, out, timer);

        MagicTrace::success("Mọi thao tác cập nhật cây con của Node 2 giờ đây tương đương cập nhật đoạn [" + 
                            to_string(in[2]) + ", " + to_string(out[2]) + "] trên Segment Tree!");
    }
}

// ============================================================================
// NHÓM 2: CÀI ĐẶT PRODUCTION / SYSTEM (REAL-WORLD APPLICATIONS)
// ============================================================================
namespace Production_BinaryTree {

    // ------------------------------------------------------------------------
    // 2.1 BINARY EXPRESSION TREE (Abstract Syntax Tree / AST Của Trình Biên Dịch)
    // ------------------------------------------------------------------------
    struct ExprNode {
        string val;
        ExprNode *left, *right;
        ExprNode(string v) : val(v), left(nullptr), right(nullptr) {}
    };

    int evaluateExprTree(ExprNode* root) {
        if (!root) return 0;
        if (!root->left && !root->right) return stoi(root->val);

        int leftVal = evaluateExprTree(root->left);
        int rightVal = evaluateExprTree(root->right);

        MagicTrace::step("Tính Toán Subtree: " + to_string(leftVal) + " " + root->val + " " + to_string(rightVal));

        if (root->val == "+") return leftVal + rightVal;
        if (root->val == "-") return leftVal - rightVal;
        if (root->val == "*") return leftVal * rightVal;
        if (root->val == "/") return leftVal / rightVal;
        return 0;
    }

    void expressionTreeDemo() {
        MagicTrace::header("2.1 BINARY EXPRESSION TREE (Phân Tích Cú Pháp Biểu Thức AST)");
        // Biểu thức: (3 + 5) * 4
        ExprNode* root = new ExprNode("*");
        root->left = new ExprNode("+");
        root->right = new ExprNode("4");
        root->left->left = new ExprNode("3");
        root->left->right = new ExprNode("5");

        MagicTrace::log("AST", "Đang tính toán giá trị của biểu thức cây: (3 + 5) * 4");
        int res = evaluateExprTree(root);
        MagicTrace::success("Kết quả biểu thức AST: " + to_string(res));
    }

    // ------------------------------------------------------------------------
    // 2.2 HUFFMAN CODING TREE (Cây Mã Hóa & Nén Dữ Liệu Tần Suất)
    // ------------------------------------------------------------------------
    struct HuffmanNode {
        char ch;
        int freq;
        HuffmanNode *left, *right;
        HuffmanNode(char c, int f) : ch(c), freq(f), left(nullptr), right(nullptr) {}
    };

    struct CompareHuffman {
        bool operator()(HuffmanNode* a, HuffmanNode* b) {
            return a->freq > b->freq;
        }
    };

    void printHuffmanCodes(HuffmanNode* root, string code) {
        if (!root) return;
        if (root->ch != '$') {
            MagicTrace::step("Ký tự '" + string(1, root->ch) + "' (Tần suất: " + to_string(root->freq) + 
                             ") ➔ Mã Bit Huffman: " + MagicTrace::BOLD + code + MagicTrace::RESET);
        }
        printHuffmanCodes(root->left, code + "0");
        printHuffmanCodes(root->right, code + "1");
    }

    void huffmanTreeDemo() {
        MagicTrace::header("2.2 HUFFMAN CODING TREE (Nén Dữ Liệu Tối Ưu)");
        priority_queue<HuffmanNode*, vector<HuffmanNode*>, CompareHuffman> pq;

        vector<pair<char, int>> freqs = {{'A', 5}, {'B', 9}, {'C', 12}, {'D', 13}, {'E', 16}, {'F', 45}};
        for (auto p : freqs) pq.push(new HuffmanNode(p.first, p.second));

        while (pq.size() > 1) {
            HuffmanNode* left = pq.top(); pq.pop();
            HuffmanNode* right = pq.top(); pq.pop();

            HuffmanNode* parent = new HuffmanNode('$', left->freq + right->freq);
            parent->left = left;
            parent->right = right;
            pq.push(parent);
        }

        MagicTrace::log("Huffman", "Bảng Mã Hóa Bit Sinh Ra Từ Cây Huffman:");
        printHuffmanCodes(pq.top(), "");
        MagicTrace::success("Huffman Coding tối ưu hóa băng thông bằng chuỗi bit độ dài biến thiên!");
    }

    // ------------------------------------------------------------------------
    // 2.3 B+ TREE SIMULATION (Mô Phỏng Cây Tìm Kiếm Nhiều Nhánh Trong Database Indexing)
    // ------------------------------------------------------------------------
    void bPlusTreeSimulationDemo() {
        MagicTrace::header("2.3 B+ TREE INDEX SIMULATION (Xương Sống Chỉ Mục MySQL / PostgreSQL)");
        MagicTrace::log("DatabaseEngine", "Mô phỏng cấu trúc B+ Tree Node (Bậc M = 3):");
        MagicTrace::step("Mỗi Node trong Disk I/O chứa nhiều Key và con trỏ trang đĩa (Disk Page Pointers)");
        MagicTrace::step("[Root Page] Keys: [30] ➔ Trỏ tới Leaf Page 1 (<30) & Leaf Page 2 (>=30)");
        MagicTrace::step("[Leaf Page 1] Data: [10, 20] ➔ Con trỏ Linked List sang [Leaf Page 2]");
        MagicTrace::step("[Leaf Page 2] Data: [30, 40, 50] ➔ Hỗ trợ Range Query (WHERE age BETWEEN 10 AND 50) cực nhanh!");
        MagicTrace::success("B+ Tree tối ưu hóa tối đa số lần đọc/ghi ổ đĩa (Disk I/O Block Reads)!");
    }

    // ------------------------------------------------------------------------
    // 2.4 THREAD-SAFE BST (Locking Cục Bộ Đa Luồng Fine-Grained Concurrent BST)
    // ------------------------------------------------------------------------
    class ConcurrentBST {
    private:
        struct Node {
            int key;
            Node* left;
            Node* right;
            mutable std::mutex node_mutex; // Mutex độc lập cho từng Node!
            Node(int k) : key(k), left(nullptr), right(nullptr) {}
        };

        Node* root;
        mutex root_mutex;

    public:
        ConcurrentBST() : root(nullptr) {}

        void insert(int key) {
            lock_guard<mutex> root_lock(root_mutex);
            if (!root) {
                root = new Node(key);
                MagicTrace::step("Thread [INSERT] Khởi tạo Gốc Key: " + to_string(key));
                return;
            }

            Node* curr = root;
            while (true) {
                if (key < curr->key) {
                    if (!curr->left) {
                        curr->left = new Node(key);
                        MagicTrace::step("Thread [INSERT] Thêm Key " + to_string(key) + " vào bên trái Node " + to_string(curr->key));
                        break;
                    }
                    curr = curr->left;
                } else {
                    if (!curr->right) {
                        curr->right = new Node(key);
                        MagicTrace::step("Thread [INSERT] Thêm Key " + to_string(key) + " vào bên phải Node " + to_string(curr->key));
                        break;
                    }
                    curr = curr->right;
                }
            }
        }
    };

    void concurrentBSTDemo() {
        MagicTrace::header("2.4 THREAD-SAFE BST (Ghi Đa Luồng An Toàn Cấu Trúc Cây)");
        ConcurrentBST tree;

        thread t1([&]() { tree.insert(50); tree.insert(30); });
        thread t2([&]() { tree.insert(70); tree.insert(80); });

        t1.join();
        t2.join();

        MagicTrace::success("Thao tác đa luồng hoàn tất an toàn không đụng độ bộ nhớ!");
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
         << "         🚀 MASTERCLASS BINARY TREE DATA STRUCTURE (CP & PRODUCTION)            \n"
         << "================================================================================\n" 
         << MagicTrace::RESET;

    // --- SETUP TREE CHO DUYỆT CÂY ---
    TreeNode* demoRoot = new TreeNode(1);
    demoRoot->left = new TreeNode(2);
    demoRoot->right = new TreeNode(3);
    demoRoot->left->left = new TreeNode(4);
    demoRoot->left->right = new TreeNode(5);

    // --- NHÓM 1: COMPETITIVE PROGRAMMING & ADVANCED ---
    MagicTrace::header("1.1 DUYỆT CÂY KHÔNG ĐỆ QUY (Iterative In-order Traversal)");
    CP_BinaryTree::iterativeInorder(demoRoot);
    
    CP_BinaryTree::morrisTraversal(demoRoot);
    CP_BinaryTree::treapDemo();
    CP_BinaryTree::splayTreeDemo();
    CP_BinaryTree::eulerTourDemo();

    // --- NHÓM 2: PRODUCTION / SYSTEM ---
    Production_BinaryTree::expressionTreeDemo();
    Production_BinaryTree::huffmanTreeDemo();
    Production_BinaryTree::bPlusTreeSimulationDemo();
    Production_BinaryTree::concurrentBSTDemo();

    cout << "\n" << MagicTrace::BOLD << MagicTrace::GREEN 
         << "================================================================================\n"
         << "   ✔ HOÀN THÀNH TOÀN BỘ BẢN CÀI ĐẶT BINARY TREE MASTERCLASS XUẤT SẮC!           \n"
         << "================================================================================\n\n" 
         << MagicTrace::RESET;

    return 0;
}