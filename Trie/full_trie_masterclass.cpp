#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
#include <queue>
#include <algorithm>
#include <iomanip>
#include <bitset>

using namespace std;

// ============================================================================
// BỘ CÔNG CỤ TRACE "PHÉP THUẬT" IN MÀU VỚI ANSI ESCAPE CODES
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
// NHÓM 1: CÀI ĐẶT CP (FLAT ARRAY - MẢNG 2 CHIỀU PHẲNG)
// ============================================================================
namespace CP_Trie {

    string binaryString5(int x) {
        string s;
        for (int i = 4; i >= 0; --i) {
            s.push_back((x >> i & 1) ? '1' : '0');
        }
        return s;
    }

    // 1. ALPHABET STRING TRIE
    namespace StringTrie {
        const int MAX_NODES = 1e5 + 5;
        const int ALPHABET_SIZE = 26;

        int trie[MAX_NODES][ALPHABET_SIZE];
        int is_end_of_word[MAX_NODES];
        int node_count = 1;

        void clear() {
            for (int i = 0; i <= node_count; i++) {
                fill(trie[i], trie[i] + ALPHABET_SIZE, 0);
                is_end_of_word[i] = 0;
            }
            node_count = 1;
        }

        void insert(const string& word) {
            MagicTrace::log("StringTrie", "Chèn từ: " + MagicTrace::BOLD + word + MagicTrace::RESET);
            int u = 1;
            for (char c : word) {
                int v = c - 'a';
                if (!trie[u][v]) {
                    trie[u][v] = ++node_count;
                    MagicTrace::step("Tạo nút mới: " + to_string(u) + " --('" + c + "')--> " + to_string(node_count));
                } else {
                    MagicTrace::step("Đi theo nhánh sẵn có: " + to_string(u) + " --('" + c + "')--> " + to_string(trie[u][v]));
                }
                u = trie[u][v];
            }
            is_end_of_word[u]++;
            MagicTrace::success("Đã đánh dấu kết thúc từ tại Nút " + to_string(u));
        }

        bool search(const string& word) {
            MagicTrace::log("StringTrie", "Tìm kiếm từ: " + MagicTrace::BOLD + word + MagicTrace::RESET);
            int u = 1;
            for (char c : word) {
                int v = c - 'a';
                if (!trie[u][v]) {
                    MagicTrace::step("Gặp ngõ tịt tại nút " + to_string(u) + " với ký tự '" + c + "'");
                    return false;
                }
                MagicTrace::step("Duyệt qua: " + to_string(u) + " --('" + c + "')--> " + to_string(trie[u][v]));
                u = trie[u][v];
            }
            return is_end_of_word[u] > 0;
        }
    }

    // 2. BINARY TRIE (MAXIMUM XOR PAIR)
    namespace BinaryTrie {
        const int MAX_NODES = 3e5 + 5;
        int trie_bin[MAX_NODES][2];
        int bin_node_count = 1;

        void clear() {
            for (int i = 0; i <= bin_node_count; i++) {
                trie_bin[i][0] = trie_bin[i][1] = 0;
            }
            bin_node_count = 1;
        }

        void insert_num(int num) {
            MagicTrace::log("BinaryTrie", "Chèn số: " + to_string(num) + " (Nhị phân 5-bit: " + binaryString5(num) + ")");
            int u = 1;
            for (int i = 4; i >= 0; i--) { // Demo 5 bit cho dễ nhìn trace
                int bit = (num >> i) & 1;
                if (!trie_bin[u][bit]) {
                    trie_bin[u][bit] = ++bin_node_count;
                    MagicTrace::step("Bit " + to_string(i) + " (" + to_string(bit) + "): Tạo nút " + to_string(u) + " -> " + to_string(bin_node_count));
                } else {
                    MagicTrace::step("Bit " + to_string(i) + " (" + to_string(bit) + "): Đi theo nút " + to_string(u) + " -> " + to_string(trie_bin[u][bit]));
                }
                u = trie_bin[u][bit];
            }
        }

        int find_max_xor(int num) {
            MagicTrace::log("BinaryTrie", "Tìm XOR tối đa với số: " + to_string(num) + " (" + binaryString5(num) + ")");
            int u = 1;
            int max_xor = 0;
            for (int i = 4; i >= 0; i--) {
                int bit = (num >> i) & 1;
                int wanted_bit = 1 - bit; // Tham lam: Ưu tiên nhánh ngược bit
                
                if (trie_bin[u][wanted_bit]) {
                    max_xor |= (1 << i);
                    MagicTrace::step("Bit " + to_string(i) + " = " + to_string(bit) + " | Thấy bit ngược (" + to_string(wanted_bit) + ") -> Nhảy nhánh " + to_string(trie_bin[u][wanted_bit]) + " [THÊM BIT 1]");
                    u = trie_bin[u][wanted_bit];
                } else {
                    MagicTrace::step("Bit " + to_string(i) + " = " + to_string(bit) + " | Không có bit ngược -> Buộc đi nhánh cùng bit (" + to_string(bit) + ") " + to_string(trie_bin[u][bit]) + " [THÊM BIT 0]");
                    u = trie_bin[u][bit];
                }
            }
            return max_xor;
        }
    }

    // 3. AHO-CORASICK AUTOMATON
    namespace AhoCorasick {
        const int MAX_NODES = 1000;
        int trie[MAX_NODES][26];
        int fail[MAX_NODES];
        vector<int> output[MAX_NODES]; // Lưu id của các từ khớp tại nút này
        int node_count = 1;

        void clear() {
            for (int i = 0; i <= node_count; i++) {
                fill(trie[i], trie[i] + 26, 0);
                fail[i] = 0;
                output[i].clear();
            }
            node_count = 1;
        }

        void insert(const string& pattern, int pattern_id) {
            int u = 1;
            for (char c : pattern) {
                int v = c - 'a';
                if (!trie[u][v]) trie[u][v] = ++node_count;
                u = trie[u][v];
            }
            output[u].push_back(pattern_id);
        }

        void build_failure_links(const vector<string>& patterns) {
            MagicTrace::log("AhoCorasick", "Đang xây dựng Failure Links (BFS)...");
            queue<int> q;
            // Khởi tạo hàng đợi cho các nút cấp 1 (con trực tiếp của Root)
            for (int c = 0; c < 26; c++) {
                if (trie[1][c]) {
                    fail[trie[1][c]] = 1;
                    q.push(trie[1][c]);
                } else {
                    trie[1][c] = 1; // Trie ngầm thành Automaton
                }
            }

            while (!q.empty()) {
                int u = q.front(); q.pop();

                for (int c = 0; c < 26; c++) {
                    if (trie[u][c] && trie[u][c] != 1) {
                        int v = trie[u][c];
                        int f = fail[u];
                        while (!trie[f][c]) f = fail[f];
                        fail[v] = trie[f][c];
                        
                        // Ghép output của nút fail vào nút hiện tại
                        output[v].insert(output[v].end(), output[fail[v]].begin(), output[fail[v]].end());
                        MagicTrace::step("Nút " + to_string(v) + " có Fail Link -> Nút " + to_string(fail[v]));
                        q.push(v);
                    } else if (!trie[u][c]) {
                        trie[u][c] = trie[fail[u]][c]; // Chuyển trạng thái
                    }
                }
            }
            MagicTrace::success("Xây dựng Automaton hoàn tất!");
        }

        void search_text(const string& text, const vector<string>& patterns) {
            MagicTrace::log("AhoCorasick", "Quét văn bản: " + MagicTrace::BOLD + text + MagicTrace::RESET);
            int u = 1;
            for (int i = 0; i < text.length(); i++) {
                int c = text[i] - 'a';
                u = trie[u][c];
                MagicTrace::step("Vị trí " + to_string(i) + " ('" + text[i] + "'): Đang ở trạng thái " + to_string(u));

                if (!output[u].empty()) {
                    for (int p_id : output[u]) {
                        MagicTrace::success("PHÁT HIỆN MẪU: '" + patterns[p_id] + "' kết thúc tại vị trí " + to_string(i));
                    }
                }
            }
        }
    }
}

// ============================================================================
// NHÓM 2: CÀI ĐẶT PRODUCTION / SYSTEM (OBJECT-ORIENTED)
// ============================================================================
namespace Production_Trie {

    // 1. AUTOCOMPLETE & SEARCH SUGGESTIONS
    class AutocompleteSystem {
    private:
        struct TrieNode {
            unordered_map<char, TrieNode*> children;
            bool is_end = false;
            int weight = 0;
        };
        TrieNode* root;

        void collect(TrieNode* node, string prefix, vector<pair<int, string>>& res) {
            if (node->is_end) res.push_back({node->weight, prefix});
            for (auto& [ch, child] : node->children) {
                collect(child, prefix + ch, res);
            }
        }

    public:
        AutocompleteSystem() { root = new TrieNode(); }

        void insert(const string& word, int weight) {
            TrieNode* curr = root;
            for (char ch : word) {
                if (!curr->children.count(ch)) curr->children[ch] = new TrieNode();
                curr = curr->children[ch];
            }
            curr->is_end = true;
            curr->weight = weight;
        }

        void get_suggestions(const string& prefix) {
            MagicTrace::log("Autocomplete", "Gợi ý cho tiền tố: " + MagicTrace::BOLD + prefix + MagicTrace::RESET);
            TrieNode* curr = root;
            for (char ch : prefix) {
                if (!curr->children.count(ch)) {
                    MagicTrace::step("Không tìm thấy tiền tố!");
                    return;
                }
                curr = curr->children[ch];
            }

            vector<pair<int, string>> candidates;
            collect(curr, prefix, candidates);
            sort(candidates.begin(), candidates.end(), greater<>());

            for (auto& [w, word] : candidates) {
                MagicTrace::success("Gợi ý: " + word + " (Lượt click/Score: " + to_string(w) + ")");
            }
        }
    };

    // 2. PROFANITY FILTER (BỘ LỌC TỪ CẤM)
    class ProfanityFilter {
    private:
        struct Node {
            unordered_map<char, Node*> children;
            bool is_bad = false;
        };
        Node* root;

    public:
        ProfanityFilter() { root = new Node(); }

        void add_bad_word(const string& word) {
            Node* curr = root;
            for (char ch : word) {
                if (!curr->children.count(ch)) curr->children[ch] = new Node();
                curr = curr->children[ch];
            }
            curr->is_bad = true;
        }

        string filter_text(string text) {
            MagicTrace::log("ProfanityFilter", "Kiểm duyệt văn bản: " + text);
            string result = text;
            int n = text.length();

            for (int i = 0; i < n; i++) {
                Node* curr = root;
                int match_len = 0;
                for (int j = i; j < n; j++) {
                    char ch = tolower(text[j]);
                    if (!curr->children.count(ch)) break;
                    curr = curr->children[ch];
                    if (curr->is_bad) {
                        match_len = j - i + 1;
                    }
                }
                if (match_len > 0) {
                    string bad = text.substr(i, match_len);
                    MagicTrace::step("Phát hiện từ cấm: '" + bad + "' từ vị trí " + to_string(i));
                    for (int k = i; k < i + match_len; k++) result[k] = '*';
                    i += match_len - 1; // Bỏ qua phần đã che
                }
            }
            MagicTrace::success("Văn bản sau khi lọc: " + result);
            return result;
        }
    };

    // 3. IP ROUTER LONGEST PREFIX MATCHING
    class IPRouter {
    private:
        struct Node {
            Node* children[2] = {nullptr, nullptr};
            string port = "";
        };
        Node* root;

    public:
        IPRouter() { root = new Node(); }

        void add_route(const string& bit_prefix, const string& port) {
            MagicTrace::log("IPRouter", "Thêm tuyến: Tiền tố " + bit_prefix + " -> Cổng " + port);
            Node* curr = root;
            for (char b : bit_prefix) {
                int bit = b - '0';
                if (!curr->children[bit]) curr->children[bit] = new Node();
                curr = curr->children[bit];
            }
            curr->port = port;
        }

        string route_packet(const string& ip_bits) {
            MagicTrace::log("IPRouter", "Định tuyến gói tin IP (Bitstream): " + ip_bits);
            Node* curr = root;
            string best_port = "DEFAULT_GATEWAY";
            string matched_prefix = "";
            string current_path = "";

            for (char b : ip_bits) {
                int bit = b - '0';
                if (!curr->children[bit]) break;
                curr = curr->children[bit];
                current_path += b;
                if (!curr->port.empty()) {
                    best_port = curr->port;
                    matched_prefix = current_path;
                    MagicTrace::step("Khớp tiền tố dài hơn: " + matched_prefix + " -> Cổng " + best_port);
                }
            }
            MagicTrace::success("KẾT QUẢ ROUTE: Chuyển gói tin tới cổng [" + best_port + "]");
            return best_port;
        }
    };

    // 4. COMPRESSED TRIE / RADIX TREE
    class RadixTree {
    private:
        struct Node {
            unordered_map<string, Node*> children;
            bool is_end = false;
        };
        Node* root;

        void print_tree(Node* node, string indent) {
            for (auto& [edge, child] : node->children) {
                cout << indent << " └── [" << edge << "]" << (child->is_end ? " (END)" : "") << "\n";
                print_tree(child, indent + "      ");
            }
        }

    public:
        RadixTree() { root = new Node(); }

        void insert(const string& word) {
            MagicTrace::log("RadixTree", "Chèn từ (Nén nút): " + word);
            Node* curr = root;
            string w = word;

            while (!w.empty()) {
                bool matched = false;
                for (auto& [edge, child] : curr->children) {
                    // Tìm tiền tố chung
                    int len = 0;
                    while (len < w.length() && len < edge.length() && w[len] == edge[len]) len++;

                    if (len > 0) {
                        matched = true;
                        if (len == edge.length()) {
                            // Khớp hoàn toàn nhãn cạnh, đi xuống nút con
                            curr = child;
                            w = w.substr(len);
                            break;
                        } else {
                            // Tách cạnh (Split Edge)
                            string common = edge.substr(0, len);
                            string remain_edge = edge.substr(len);

                            Node* split_node = new Node();
                            split_node->children[remain_edge] = child;

                            curr->children.erase(edge);
                            curr->children[common] = split_node;

                            if (len == w.length()) {
                                split_node->is_end = true;
                            } else {
                                Node* new_leaf = new Node();
                                new_leaf->is_end = true;
                                split_node->children[w.substr(len)] = new_leaf;
                            }
                            MagicTrace::step("Tách cạnh '" + edge + "' thành '" + common + "' và '" + remain_edge + "'");
                            return;
                        }
                    }
                }
                if (!matched) {
                    // Tạo cạnh mới hoàn toàn
                    Node* new_node = new Node();
                    new_node->is_end = true;
                    curr->children[w] = new_node;
                    MagicTrace::step("Tạo cạnh nén mới: [" + w + "]");
                    break;
                }
            }
        }

        void display() {
            MagicTrace::log("RadixTree", "Cấu trúc cây Radix Tree đã nén:");
            print_tree(root, "");
        }
    };
}

// ============================================================================
// HÀM MAIN CHẠY DEMO TOÀN BỘ 7 BẢN CÀI ĐẶT
// ============================================================================
int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    // --- 1. STRING TRIE ---
    MagicTrace::header("1. STRING TRIE (ALPHABET CP)");
    CP_Trie::StringTrie::insert("thuat");
    CP_Trie::StringTrie::insert("thuattoan");
    CP_Trie::StringTrie::search("thuat");

    // --- 2. BINARY TRIE ---
    MagicTrace::header("2. BINARY TRIE (MAXIMUM XOR PAIR)");
    CP_Trie::BinaryTrie::insert_num(25); // 11001
    CP_Trie::BinaryTrie::insert_num(5);  // 00101
    CP_Trie::BinaryTrie::find_max_xor(5);

    // --- 3. AHO-CORASICK AUTOMATON ---
    MagicTrace::header("3. AHO-CORASICK AUTOMATON (MULTI-PATTERN MATCHING)");
    vector<string> patterns = {"he", "she", "his", "hers"};
    for (int i = 0; i < patterns.size(); i++) {
        CP_Trie::AhoCorasick::insert(patterns[i], i);
    }
    CP_Trie::AhoCorasick::build_failure_links(patterns);
    CP_Trie::AhoCorasick::search_text("ahishers", patterns);

    // --- 4. AUTOCOMPLETE ENGINE ---
    MagicTrace::header("4. AUTOCOMPLETE ENGINE (PRODUCTION)");
    Production_Trie::AutocompleteSystem auto_sys;
    auto_sys.insert("thuattoan", 100);
    auto_sys.insert("thuyien", 50);
    auto_sys.insert("thuocbac", 85);
    auto_sys.get_suggestions("thu");

    // --- 5. PROFANITY FILTER ---
    MagicTrace::header("5. PROFANITY FILTER (CONTENT CENSORSHIP)");
    Production_Trie::ProfanityFilter filter;
    filter.add_bad_word("badword");
    filter.add_bad_word("hack");
    filter.filter_text("This is a badword and a dangerous hack inside system!");

    // --- 6. IP ROUTER LONGEST PREFIX MATCHING ---
    MagicTrace::header("6. IP ROUTER (LONGEST PREFIX MATCHING)");
    Production_Trie::IPRouter router;
    router.add_route("101", "PORT_A (Mạng nội bộ)");
    router.add_route("10110", "PORT_B (Mạng ưu tiên)");
    router.route_packet("10110111");

    // --- 7. RADIX TREE (COMPRESSED TRIE) ---
    MagicTrace::header("7. RADIX TREE / COMPRESSED TRIE");
    Production_Trie::RadixTree radix;
    radix.insert("repository");
    radix.insert("report");
    radix.insert("rest");
    radix.display();

    cout << "\n" << MagicTrace::BOLD << MagicTrace::GREEN << "=== TOÀN BỘ DEMO ĐÃ HOÀN THÀNH XUẤT SẮC! ===\n\n" << MagicTrace::RESET;
    return 0;
}