#include <iostream>
#include <vector>
#include <algorithm>
#include <iomanip>

using namespace std;

/**
 * 1. SEGMENT TREE CƠ BẢN (POINT UPDATE - RANGE QUERY)
 * - Tối ưu cho Lập trình thi đấu (CP)
 * - Dùng mảng 1 chiều 1-indexed giúp cache-friendly và tính chỉ số nhanh
 * - Độ phức tạp: Build O(N), Update O(log N), Query O(log N)
 */

const int MAXN = 100000;
int a[MAXN];          // Mảng dữ liệu ban đầu (0-indexed)
int tree[4 * MAXN];   // Segment Tree lưu trên mảng 1 chiều (1-indexed)
int n;                // Số lượng phần tử

// 1. DỰNG CÂY (BUILD)
void build(int id, int l, int r) {
    if (l == r) {
        tree[id] = a[l];
        return;
    }
    int mid = (l + r) / 2;
    build(2 * id, l, mid);          // Con trái tại 2*id
    build(2 * id + 1, mid + 1, r);  // Con phải tại 2*id + 1
    
    // Gộp kết quả (Ví dụ này dùng TỔNG, có thể đổi thành min/max)
    tree[id] = tree[2 * id] + tree[2 * id + 1];
}

// 2. CẬP NHẬT 1 ĐIỂM (POINT UPDATE)
// Thay đổi giá trị a[pos] = val
void update(int id, int l, int r, int pos, int val) {
    if (l == r) {
        tree[id] = val;
        a[pos] = val;
        return;
    }
    int mid = (l + r) / 2;
    if (pos <= mid) {
        update(2 * id, l, mid, pos, val);         // Vị trí cần sửa ở nửa trái
    } else {
        update(2 * id + 1, mid + 1, r, pos, val); // Vị trí cần sửa ở nửa phải
    }
    
    // Cập nhật lại nút cha khi quay lui đệ quy
    tree[id] = tree[2 * id] + tree[2 * id + 1];
}

// 3. TRUY VẤN ĐOẠN [u, v] (RANGE QUERY)
int query(int id, int l, int r, int u, int v) {
    // Trường hợp 1: [l, r] nằm ngoài [u, v]
    if (v < l || u > r) {
        return 0; // Trả về phần tử trung hòa của phép cộng (Nếu là min thì trả INF, max thì trả -INF)
    }
    // Trường hợp 2: [l, r] nằm hoàn toàn trong [u, v]
    if (u <= l && r <= v) {
        return tree[id];
    }
    // Trường hợp 3: [l, r] giao một phần với [u, v]
    int mid = (l + r) / 2;
    return query(2 * id, l, mid, u, v) + query(2 * id + 1, mid + 1, r, u, v);
}

// HAM IN TRẠNG THÁI MẢNG TREE (Hỗ trợ trace & debug)
void print_tree_status(int max_id) {
    cout << "  [Array tree[]]: ";
    for (int i = 1; i <= max_id; i++) {
        if (tree[i] != 0) cout << "t[" << i << "]=" << tree[i] << " ";
    }
    cout << "\n";
}

int main() {
    // Tối ưu I/O cho CP
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    vector<int> input = {2, 1, 5, 3, 4};
    n = input.size();
    for (int i = 0; i < n; i++) a[i] = input[i];

    cout << "================ TRACE BƯỚC 1: DỰNG CÂY (BUILD) ================\n";
    cout << "Mang ban dau a: ";
    for (int i = 0; i < n; i++) cout << a[i] << " ";
    cout << "\n\n";

    build(1, 0, n - 1);
    print_tree_status(13);

    cout << "\n================ TRACE BƯỚC 2: TRUY VẤN (QUERY) ================\n";
    int u = 1, v = 3;
    cout << "Query tong doan [" << u << ", " << v << "] (Cac phan tu: ";
    for (int i = u; i <= v; i++) cout << a[i] << (i == v ? "" : " + ");
    cout << "):\n";
    
    int ans = query(1, 0, n - 1, u, v);
    cout << "=> Ket qua query: " << ans << "\n\n";

    cout << "================ TRACE BƯỚC 3: CẬP NHẬT (UPDATE) ================\n";
    int pos = 2, new_val = 10;
    cout << "Update a[" << pos << "] tu " << a[pos] << " thanh " << new_val << "\n";
    
    update(1, 0, n - 1, pos, new_val);
    cout << "Mang a sau khi update: ";
    for (int i = 0; i < n; i++) cout << a[i] << " ";
    cout << "\n";
    print_tree_status(13);

    cout << "\nQuery lai tong doan [" << u << ", " << v << "] sau khi update:\n";
    ans = query(1, 0, n - 1, u, v);
    cout << "=> Ket qua query moi: " << ans << "\n";

    /* 
    ===========================================================================
    OUTPUT TRACE TỰ ĐỘNG CHẠY BỞI CHƯƠNG TRÌNH:
    ===========================================================================
    ================ TRACE BƯỚC 1: DỰNG CÂY (BUILD) ================
    Mang ban dau a: 2 1 5 3 4 

      [Array tree[]]: t[1]=15 t[2]=8 t[3]=7 t[4]=3 t[5]=5 t[6]=3 t[7]=4 t[8]=2 t[9]=1 

    ================ TRACE BƯỚC 2: TRUY VẤN (QUERY) ================
    Query tong doan [1, 3] (Cac phan tu: 1 + 5 + 3):
    => Ket qua query: 9

    ================ TRACE BƯỚC 3: CẬP NHẬT (UPDATE) ================
    Update a[2] tu 5 thanh 10
    Mang a sau khi update: 2 1 10 3 4 
      [Array tree[]]: t[1]=20 t[2]=13 t[3]=7 t[4]=3 t[5]=10 t[6]=3 t[7]=4 t[8]=2 t[9]=1 

    Query lai tong doan [1, 3] sau khi update:
    => Ket qua query moi: 14
    ===========================================================================
    */

    return 0;
}