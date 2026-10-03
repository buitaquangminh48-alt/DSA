#include <iostream>
#include <vector>
#include <string>
#include <stack>
#include <sstream>
#include <cctype>
#include <algorithm>
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

    // Trực quan hóa nội dung Stack dưới dạng dạng [ top -> bottom ]
    template <typename T>
    string printStack(stack<T> st) {
        string res = "[ Top: ";
        if (st.empty()) return "[ EMPTY ]";
        while (!st.empty()) {
            res += to_string(st.top()) + " ";
            st.pop();
        }
        res += "]";
        return res;
    }
}

// ============================================================================
// NHÓM 1: CÀI ĐẶT CP (COMPETITIVE PROGRAMMING)
// ============================================================================
namespace CP_Stack {

    // ------------------------------------------------------------------------
    // 1.1 MONOTONIC STACK (Next Greater Element trong O(N))
    // ------------------------------------------------------------------------
    void nextGreaterElementDemo() {
        MagicTrace::header("1.1 MONOTONIC STACK (Next Greater Element - NGE)");
        vector<int> arr = {4, 5, 2, 25, 7, 8};
        int n = arr.size();
        vector<int> nge(n, -1);
        stack<int> st; // Lưu index của các phần tử

        MagicTrace::log("MonotonicStack", "Duyệt mảng từ TỪ PHẢI SANG TRÁI để duy trì Stack giảm dần...");

        for (int i = n - 1; i >= 0; --i) {
            // Loại bỏ các phần tử nhỏ hơn hoặc bằng phần tử hiện tại
            while (!st.empty() && arr[st.top()] <= arr[i]) {
                MagicTrace::step("Pop " + to_string(arr[st.top()]) + " ra khỏi Stack (vì <= " + to_string(arr[i]) + ")");
                st.pop();
            }

            if (!st.empty()) {
                nge[i] = arr[st.top()];
            }

            st.push(i);
            MagicTrace::step("Xử lý arr[" + to_string(i) + "] = " + to_string(arr[i]) 
                            + " | NGE = " + (nge[i] == -1 ? "None" : to_string(nge[i])) 
                            + " | Stack index size: " + to_string(st.size()));
        }

        string result_str = "";
        for (int i = 0; i < n; ++i) {
            result_str += to_string(arr[i]) + "➔" + (nge[i] == -1 ? "N/A" : to_string(nge[i])) + " | ";
        }
        MagicTrace::success("Kết quả Next Greater Element: " + result_str);
    }

    // ------------------------------------------------------------------------
    // 1.2 SHUNTING-YARD ALGORITHM (Đổi Infix -> Postfix & Đánh giá biểu thức)
    // ------------------------------------------------------------------------
    int precedence(char op) {
        if (op == '+' || op == '-') return 1;
        if (op == '*' || op == '/') return 2;
        return 0;
    }

    void shuntingYardDemo() {
        MagicTrace::header("1.2 SHUNTING-YARD ALGORITHM (Infix to Postfix Evaluator)");
        string infix = "3 + 4 * 2 / ( 1 - 5 )";
        MagicTrace::log("ShuntingYard", "Chuyển biểu thức Trung Tố (Infix): \"" + infix + "\" sang Hậu Tố (Postfix)");

        stringstream ss(infix);
        string token;
        string postfix = "";
        stack<char> opStack;

        while (ss >> token) {
            if (isdigit(token[0])) {
                postfix += token + " ";
                MagicTrace::step("Số '" + token + "' ➔ Đẩy thẳng vào đầu ra Postfix: " + postfix);
            } else if (token == "(") {
                opStack.push('(');
                MagicTrace::step("Ngoặc mở '(' ➔ Push vào Operator Stack");
            } else if (token == ")") {
                while (!opStack.empty() && opStack.top() != '(') {
                    postfix += string(1, opStack.top()) + " ";
                    opStack.pop();
                }
                if (!opStack.empty()) opStack.pop(); // Pop '('
                MagicTrace::step("Ngoặc đóng ')' ➔ Pop toán tử cho đến '(': " + postfix);
            } else { // Toán tử +, -, *, /
                char op = token[0];
                while (!opStack.empty() && precedence(opStack.top()) >= precedence(op)) {
                    postfix += string(1, opStack.top()) + " ";
                    opStack.pop();
                }
                opStack.push(op);
                MagicTrace::step("Toán tử '" + string(1, op) + "' ➔ Đẩy vào Stack | Postfix hiện tại: " + postfix);
            }
        }

        while (!opStack.empty()) {
            postfix += string(1, opStack.top()) + " ";
            opStack.pop();
        }

        MagicTrace::success("Biểu thức Postfix hoàn chỉnh: " + postfix);
    }

    // ------------------------------------------------------------------------
    // 1.3 KHỬ ĐỆ QUY DFS BẰNG STACK (Chống Tràn Bộ Nhớ Call Stack System)
    // ------------------------------------------------------------------------
    void iterativeDFSDemo() {
        MagicTrace::header("1.3 ITERATIVE DFS (Duyệt Đồ Thị Không Đệ Quy Chống Stack Overflow)");
        int num_nodes = 5;
        vector<vector<int>> adj(num_nodes + 1);
        // Dựng đồ thị mẫu
        adj[1] = {2, 3};
        adj[2] = {4};
        adj[3] = {5};

        MagicTrace::log("IterativeDFS", "Khởi tạo Stack thủ công trên Heap thay vì dùng Đệ Quy hệ thống...");
        stack<int> st;
        vector<bool> visited(num_nodes + 1, false);

        st.push(1);
        string traversal_order = "";

        while (!st.empty()) {
            int u = st.top();
            st.pop();

            if (!visited[u]) {
                visited[u] = true;
                traversal_order += to_string(u) + " ";
                MagicTrace::step("Thăm Đỉnh " + to_string(u) + " | Pop khỏi Stack");

                // Đẩy các đỉnh kề vào Stack (duyệt ngược để đảm bảo thứ tự)
                for (auto it = adj[u].rbegin(); it != adj[u].rend(); ++it) {
                    if (!visited[*it]) {
                        st.push(*it);
                        MagicTrace::step("  └─ Push đỉnh kề " + to_string(*it) + " vào Stack");
                    }
                }
            }
        }
        MagicTrace::success("Thứ tự duyệt DFS không đệ quy: " + traversal_order);
    }
}

// ============================================================================
// NHÓM 2: CÀI ĐẶT PRODUCTION / SYSTEM (REAL-WORLD APPLICATIONS)
// ============================================================================
namespace Production_Stack {

    // ------------------------------------------------------------------------
    // 2.1 UNDO / REDO MANAGER (Hệ Thống Hoàn Tác Song Song 2 Stack)
    // ------------------------------------------------------------------------
    class UndoRedoManager {
    private:
        stack<string> undo_stack;
        stack<string> redo_stack;
        string current_text = "";

    public:
        void type(const string& new_text) {
            undo_stack.push(current_text);
            current_text += new_text;
            // Xóa sạch Redo Stack khi có hành động gõ mới
            while (!redo_stack.empty()) redo_stack.pop();
            MagicTrace::step("GÕ BÀN PHÍM: \"" + new_text + "\" | Văn bản hiện tại: \"" + current_text + "\"");
        }

        void undo() {
            if (undo_stack.empty()) {
                MagicTrace::step("Không thể Undo (Undo Stack rỗng)!");
                return;
            }
            redo_stack.push(current_text);
            current_text = undo_stack.top();
            undo_stack.pop();
            MagicTrace::step("CTRL + Z (UNDO) ➔ Khôi phục về: \"" + current_text + "\"");
        }

        void redo() {
            if (redo_stack.empty()) {
                MagicTrace::step("Không thể Redo (Redo Stack rỗng)!");
                return;
            }
            undo_stack.push(current_text);
            current_text = redo_stack.top();
            redo_stack.pop();
            MagicTrace::step("CTRL + Y (REDO) ➔ Tiến tới: \"" + current_text + "\"");
        }
    };

    void undoRedoDemo() {
        MagicTrace::header("2.1 UNDO / REDO MANAGER (Dual Stack System)");
        UndoRedoManager editor;
        editor.type("Hello ");
        editor.type("World!");
        editor.undo();
        editor.redo();
        editor.type(" C++17 Stack!");
        editor.undo();
        MagicTrace::success("Mô phỏng Undo/Redo hoàn tất!");
    }

    // ------------------------------------------------------------------------
    // 2.2 SYNTAX VALIDATOR (Linter/Parser Kiểm Tra Cặp Thẻ / Ngoặc)
    // ------------------------------------------------------------------------
    void syntaxValidatorDemo() {
        MagicTrace::header("2.2 SYNTAX VALIDATOR (Linter Kiểm Tra Cú Pháp Ngoặc)");
        string code = "function main() { vector<int> v = {1, (2 + 3)}; }";
        MagicTrace::log("SyntaxValidator", "Kiểm tra cú pháp chuỗi Code: \"" + code + "\"");

        stack<char> st;
        map<char, char> matching = {{')', '('}, {'}', '{'}, {']', '['}};
        bool is_valid = true;

        for (size_t i = 0; i < code.length(); ++i) {
            char ch = code[i];
            if (ch == '(' || ch == '{' || ch == '[') {
                st.push(ch);
                MagicTrace::step("Thẻ mở '" + string(1, ch) + "' tại vị trí " + to_string(i) + " ➔ Push vào Stack");
            } else if (ch == ')' || ch == '}' || ch == ']') {
                if (st.empty() || st.top() != matching[ch]) {
                    MagicTrace::step("LỖI CÚ PHÁP! Thẻ đóng '" + string(1, ch) + "' không khớp!");
                    is_valid = false;
                    break;
                } else {
                    MagicTrace::step("Thẻ đóng '" + string(1, ch) + "' KHỚP với '" + string(1, st.top()) + "' ➔ Pop Stack");
                    st.pop();
                }
            }
        }

        if (!st.empty()) is_valid = false;

        if (is_valid) {
            MagicTrace::success("Mã nguồn CHUẨN CÚ PHÁP (Valid Syntax)!");
        } else {
            MagicTrace::step("Mã nguồn SAI CÚ PHÁP (Invalid Syntax)!");
        }
    }

    // ------------------------------------------------------------------------
    // 2.3 BROWSER HISTORY NAVIGATION (Tính năng Back / Forward Trình Duyệt)
    // ------------------------------------------------------------------------
    class BrowserHistory {
    private:
        stack<string> back_stack;
        stack<string> forward_stack;
        string current_url = "about:blank";

    public:
        void visit(const string& url) {
            if (current_url != "about:blank") {
                back_stack.push(current_url);
            }
            current_url = url;
            while (!forward_stack.empty()) forward_stack.pop();
            MagicTrace::step("TRUY CẬP: " + current_url);
        }

        void back() {
            if (back_stack.empty()) {
                MagicTrace::step("Không thể Back!");
                return;
            }
            forward_stack.push(current_url);
            current_url = back_stack.top();
            back_stack.pop();
            MagicTrace::step("NÚT BACK ⬅️ Quay lại: " + current_url);
        }

        void forward() {
            if (forward_stack.empty()) {
                MagicTrace::step("Không thể Forward!");
                return;
            }
            back_stack.push(current_url);
            current_url = forward_stack.top();
            forward_stack.pop();
            MagicTrace::step("NÚT FORWARD ➡️ Tiến tới: " + current_url);
        }
    };

    void browserNavigationDemo() {
        MagicTrace::header("2.3 BROWSER HISTORY NAVIGATION (Back & Forward Stack)");
        BrowserHistory browser;
        browser.visit("google.com");
        browser.visit("github.com");
        browser.visit("leetcode.com");
        browser.back();
        browser.back();
        browser.forward();
        browser.visit("youtube.com");
        MagicTrace::success("Mô phỏng điều hướng trình duyệt hoàn thành!");
    }

    // ------------------------------------------------------------------------
    // 2.4 CALL STACK & FUNCTION EXECUTION TRACKER (Mô phỏng Call Stack Runtime)
    // ------------------------------------------------------------------------
    struct Frame {
        string function_name;
        int line_number;
    };

    void callStackTrackerDemo() {
        MagicTrace::header("2.4 CALL STACK & FUNCTION TRACKER (Trình Mô Phỏng Call Stack Runtime)");
        stack<Frame> call_stack;

        auto pushFrame = [&](const string& func, int line) {
            call_stack.push({func, line});
            MagicTrace::step("CALL FUNCTION: " + func + "() [Line " + to_string(line) + "] ➔ Push Frame vào Call Stack | Depth = " + to_string(call_stack.size()));
        };

        auto popFrame = [&]() {
            if (!call_stack.empty()) {
                MagicTrace::step("RETURN FUNCTION: " + call_stack.top().function_name + "() ➔ Pop Frame khỏi Call Stack");
                call_stack.pop();
            }
        };

        // Giả lập luồng thực thi: main() -> calculate() -> multiply()
        pushFrame("main", 10);
        pushFrame("calculateSum", 25);
        pushFrame("multiply", 42);
        
        popFrame(); // Hoàn thành multiply
        pushFrame("divide", 50); // Gọi tiếp divide
        popFrame(); // Hoàn thành divide
        popFrame(); // Hoàn thành calculateSum
        popFrame(); // Hoàn thành main

        MagicTrace::success("Call Stack đã rỗng. Chương trình thoát an toàn!");
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
         << "         🚀 MASTERCLASS STACK DATA STRUCTURE (CP & PRODUCTION)                  \n"
         << "================================================================================\n" 
         << MagicTrace::RESET;

    // --- NHÓM 1: COMPETITIVE PROGRAMMING ---
    CP_Stack::nextGreaterElementDemo();
    CP_Stack::shuntingYardDemo();
    CP_Stack::iterativeDFSDemo();

    // --- NHÓM 2: PRODUCTION / SYSTEM ---
    Production_Stack::undoRedoDemo();
    Production_Stack::syntaxValidatorDemo();
    Production_Stack::browserNavigationDemo();
    Production_Stack::callStackTrackerDemo();

    cout << "\n" << MagicTrace::BOLD << MagicTrace::GREEN 
         << "================================================================================\n"
         << "   ✔ HOÀN THÀNH TOÀN BỘ CÁC BẢN CÀI ĐẶT STACK MASTERCLASS XUẤT SẮC!            \n"
         << "================================================================================\n\n" 
         << MagicTrace::RESET;

    return 0;
}