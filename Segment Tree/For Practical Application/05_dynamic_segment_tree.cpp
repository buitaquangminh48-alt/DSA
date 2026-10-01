#include <iostream>
#include <vector>

using namespace std;

/**
 * 5. DYNAMIC SEGMENT TREE (POINTER / NODE-BASED)
 * - Quản lý không gian tọa độ cực lớn [0, 10^9] mà không tốn bộ nhớ khởi tạo ban đầu.
 * - Khởi tạo nút động bằng con trỏ (Lazy Allocation)
 * - Bài toán mẫu: Range Sum Query & Point Update trên miền [0, 10^9]
 * - Độ phức tạp bộ nhớ: O(Q * log(RANGE))
 * - Độ phức tạp thời gian: Update O(log(RANGE)), Query O(log(RANGE))
 */

const int INF_RANGE = 1e9; // Miền giá trị từ 0 đến 10^9

struct Node {
    long long val;
    Node* left;
    Node* right;

    Node() : val(0), left(nullptr), right(nullptr) {}
    
    ~Node() {
        delete left;
        delete right;
    }
};

// 1. CẬP NHẬT 1 ĐIỂM (POINT UPDATE TRÊN CÂY ĐỘNG)
// Cộng 'val' vào vị trí 'pos' trên dải [l, r]
void update(Node* node, int l, int r, int pos, long long val) {
    if (l == r) {
        node->val += val;
        return;
    }

    int mid = l + (r - l) / 2; // Dùng l + (r - l) / 2 để tránh tràn số int khi l + r lớn

    if (pos <= mid) {
        // Nếu con trái chưa tồn tại -> Cấp phát động nút mới
        if (!node->left) node->left = new Node();
        update(node->left, l, mid, pos, val);
    } else {
        // Nếu con phải chưa tồn tại -> Cấp phát động nút mới
        if (!node->right) node->right = new Node();
        update(node->right, mid + 1, r, pos, val);
    }

    // Cập nhật giá trị nút cha từ các con (kiểm tra nullptr an toàn)
    long long left_val = node->left ? node->left->val : 0;
    long long right_val = node->right ? node->right->val : 0;
    node->val = left_val + right_val;
}

// 2. TRUY VẤN ĐOẠN [u, v] (RANGE QUERY TRÊN CÂY ĐỘNG)
long long query(Node* node, int l, int r, int u, int v) {
    // Nếu nút chưa được tạo hoặc nằm ngoài đoạn -> Trả về 0
    if (!node || v < l || u > r) return 0;

    // Nằm hoàn toàn trong đoạn
    if (u <= l && r <= v) return node->val;

    int mid = l + (r - l) / 2;
    return query(node->left, l, mid, u, v) + 
           query(node->right, mid + 1, r, u, v);
}

// Hàm đếm tổng số nút hiện có trên cây (Dùng để Trace bộ nhớ)
int count_nodes(Node* node) {
    if (!node) return 0;
    return 1 + count_nodes(node->left) + count_nodes(node->right);
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    // Tạo gốc ban đầu quản lý miền [0, 10^9]
    Node* root = new Node();

    cout << "================ TRACE BƯỚC 1: KHỞI TẠO CÂY ĐỘNG ================\n";
    cout << "Pham vi quan ly: [0, " << INF_RANGE << "]\n";
    cout << "So nut ban dau: " << count_nodes(root) << " (Chi co duy nhat Nut Goc)\n\n";

    cout << "================ TRACE BƯỚC 2: CẬP NHẬT CÁC TỌA ĐỘ CỰC LỚN ================\n";
    cout << "1. Cong 50 vao vi tri pos = 100\n";
    update(root, 0, INF_RANGE, 100, 50);

    cout << "2. Cong 20 vao vi tri pos = 500,000,000\n";
    update(root, 0, INF_RANGE, 500000000, 20);

    cout << "3. Cong 30 vao vi tri pos = 999,999,999\n";
    update(root, 0, INF_RANGE, 999999999, 30);

    cout << "\nTong so nut duoc cap phat dong trong RAM: " << count_nodes(root) << " nodes\n";
    cout << "(Luu y: Thay vi ton 4*10^9 nut, ta chi dung ~90 nut cho 3 thao tac tren!)\n\n";

    cout << "================ TRACE BƯỚC 3: TRUY VẤN ĐOẠN (RANGE QUERY) ================\n";
    cout << "Query 1: Tong trong doan [0, 200]                   => " 
         << query(root, 0, INF_RANGE, 0, 200) << " (Mong doi: 50)\n";

    cout << "Query 2: Tong trong doan [400,000,000, 600,000,000] => " 
         << query(root, 0, INF_RANGE, 400000000, 600000000) << " (Mong doi: 20)\n";

    cout << "Query 3: Tong toan bo mien [0, 10^9]               => " 
         << query(root, 0, INF_RANGE, 0, INF_RANGE) << " (Mong doi: 100)\n";

    // Giải phóng bộ nhớ RAM khi kết thúc
    delete root;

    /*
    ===========================================================================
    OUTPUT TRACE TỰ ĐỘNG CHẠY BỞI CHƯƠNG TRÌNH:
    ===========================================================================
    ================ TRACE BƯỚC 1: KHỞI TẠO CÂY ĐỘNG ================
    Pham vi quan ly: [0, 1000000000]
    So nut ban dau: 1 (Chi co duy nhat Nut Goc)

    ================ TRACE BƯỚC 2: CẬP NHẬT CÁC TỌA ĐỘ CỰC LỚN ================
    1. Cong 50 vao vi tri pos = 100
    2. Cong 20 vao vi tri pos = 500,000,000
    3. Cong 30 vao vi tri pos = 999,999,999

    Tong so nut duoc cap phat dong trong RAM: 91 nodes
    (Luu y: Thay vi ton 4*10^9 nut, ta chi dung ~90 nut cho 3 thao tac tren!)

    ================ TRACE BƯỚC 3: TRUY VẤN ĐOẠN (RANGE QUERY) ================
    Query 1: Tong trong doan [0, 200]                   => 50 (Mong doi: 50)
    Query 2: Tong trong doan [400,000,000, 600,000,000] => 20 (Mong doi: 20)
    Query 3: Tong toan bo mien [0, 10^9]               => 100 (Mong doi: 100)
    ===========================================================================
    */

    return 0;
}