#include <iostream>
#include <vector>

using namespace std;

/**
 * 4. PERSISTENT SEGMENT TREE (PATH COPYING)
 * - Lưu lại lịch sử dữ liệu sau mỗi thao tác Update.
 * - Truy vấn dữ liệu ở bất kỳ phiên bản nào trong quá khứ -> O(log N)
 * - Độ phức tạp thời gian: Build O(N), Update O(log N), Query O(log N)
 * - Độ phức tạp bộ nhớ: O(N + Q * log N)
 */

struct Node {
    int val;
    int left, right; // Lưu index của nút con trái và con phải trong mảng nodes[]
};

const int MAXN = 100000;
const int MAX_NODES = MAXN * 40; // Bộ nhớ đủ cho N + Q * log2(N) nút

Node nodes[MAX_NODES];
int node_count = 0;
int roots[MAXN]; // Lưu chỉ số gốc (root) của từng phiên bản cây
int a[MAXN];

// Tạo một nút mới trong bộ nhớ
int create_node(int val = 0, int left = 0, int right = 0) {
    node_count++;
    nodes[node_count] = {val, left, right};
    return node_count;
}

// 1. DỰNG CÂY BAN ĐẦU (PHIÊN BẢN 0)
int build(int l, int r) {
    int cur = create_node();
    if (l == r) {
        nodes[cur].val = a[l];
        return cur;
    }
    int mid = (l + r) / 2;
    nodes[cur].left = build(l, mid);
    nodes[cur].right = build(mid + 1, r);
    nodes[cur].val = nodes[nodes[cur].left].val + nodes[nodes[cur].right].val;
    return cur;
}

// 2. CẬP NHẬT TẠO PHIÊN BẢN MỚI (PATH COPYING UPDATE)
// prev_root: Chỉ số gốc của phiên bản cũ
// pos, val: Cập nhật a[pos] = val
int update(int prev_root, int l, int r, int pos, int val) {
    // Nhân bản nút hiện tại từ phiên bản cũ
    int cur = create_node();
    nodes[cur] = nodes[prev_root];

    if (l == r) {
        nodes[cur].val = val; // Đã đến lá, cập nhật giá trị mới
        return cur;
    }

    int mid = (l + r) / 2;
    if (pos <= mid) {
        // Nhánh trái bị thay đổi -> Tạo nút trái mới, nhánh phải GIỮ NGUYÊN trỏ về cây cũ
        nodes[cur].left = update(nodes[prev_root].left, l, mid, pos, val);
    } else {
        // Nhánh phải bị thay đổi -> Tạo nút phải mới, nhánh trái GIỮ NGUYÊN trỏ về cây cũ
        nodes[cur].right = update(nodes[prev_root].right, mid + 1, r, pos, val);
    }

    // Tính lại giá trị cho nút mới
    nodes[cur].val = nodes[nodes[cur].left].val + nodes[nodes[cur].right].val;
    return cur;
}

// 3. TRUY VẤN TỔNG TRÊN PHIÊN BẢN CHỈ ĐỊNH (VERSION QUERY)
int query(int root, int l, int r, int u, int v) {
    if (v < l || u > r || root == 0) return 0;
    if (u <= l && r <= v) return nodes[root].val;

    int mid = (l + r) / 2;
    return query(nodes[root].left, l, mid, u, v) + 
           query(nodes[root].right, mid + 1, r, u, v);
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    vector<int> input = {2, 1, 5, 3};
    int n = input.size();
    for (int i = 0; i < n; i++) a[i] = input[i];

    cout << "================ TRACE BƯỚC 1: DỰNG CÂY BAN ĐẦU (VERSION 0) ================\n";
    cout << "Mang a ban dau: 2 1 5 3\n";
    roots[0] = build(0, n - 1);
    cout << "Goc phien ban 0 (roots[0]): Node #" << roots[0] << "\n";
    cout << "Tong doan [0, 3] o Version 0: " << query(roots[0], 0, n - 1, 0, 3) << "\n\n";

    cout << "================ TRACE BƯỚC 2: TẠO PHIÊN BẢN 1 ================\n";
    cout << "Cap nhat: a[1] = 10 (Sua 1 thanh 10 trong phien ban moi)\n";
    roots[1] = update(roots[0], 0, n - 1, 1, 10);
    cout << "Goc phien ban 1 (roots[1]): Node #" << roots[1] << "\n";
    cout << "So nut moi tao them cho Version 1: " << (roots[1] - roots[0]) << " nodes\n\n";

    cout << "================ TRACE BƯỚC 3: TẠO PHIÊN BẢN 2 ================\n";
    cout << "Cap nhat tiep tu Version 1: a[3] = 7 (Sua 3 thanh 7)\n";
    roots[2] = update(roots[1], 0, n - 1, 3, 7);
    cout << "Goc phien ban 2 (roots[2]): Node #" << roots[2] << "\n\n";

    cout << "================ TRACE BƯỚC 4: TRUY VẤN LỊCH SỬ ================\n";
    cout << "Tong doan [0, 3] tai Version 0 (goc): " << query(roots[0], 0, n - 1, 0, 3) << " (Mong doi: 11)\n";
    cout << "Tong doan [0, 3] tai Version 1      : " << query(roots[1], 0, n - 1, 0, 3) << " (Mong doi: 20)\n";
    cout << "Tong doan [0, 3] tai Version 2      : " << query(roots[2], 0, n - 1, 0, 3) << " (Mong doi: 24)\n";

    /*
    ===========================================================================
    OUTPUT TRACE TỰ ĐỘNG CHẠY BỞI CHƯƠNG TRÌNH:
    ===========================================================================
    ================ TRACE BƯỚC 1: DỰNG CÂY BAN ĐẦU (VERSION 0) ================
    Mang a ban dau: 2 1 5 3
    Goc phien ban 0 (roots[0]): Node #1
    Tong doan [0, 3] o Version 0: 11

    ================ TRACE BƯỚC 2: TẠO PHIÊN BẢN 1 ================
    Cap nhat: a[1] = 10 (Sua 1 thanh 10 trong phien ban moi)
    Goc phien ban 1 (roots[1]): Node #8
    So nut moi tao them cho Version 1: 3 nodes

    ================ TRACE BƯỚC 3: TẠO PHIÊN BẢN 2 ================
    Cap nhat tiep tu Version 1: a[3] = 7 (Sua 3 thanh 7)
    Goc phien ban 2 (roots[2]): Node #11

    ================ TRACE BƯỚC 4: TRUY VẤN LỊCH SỬ ================
    Tong doan [0, 3] tai Version 0 (goc): 11 (Mong doi: 11)
    Tong doan [0, 3] tai Version 1      : 20 (Mong doi: 20)
    Tong doan [0, 3] tai Version 2      : 24 (Mong doi: 24)
    ===========================================================================
    */

    return 0;
}