#include <iostream>
#include <vector>
#include <functional>
#include <algorithm>
#include <climits>
#include <string>

using namespace std;

/**
 * 6. GENERIC / ABSTRACT SEGMENT TREE (C++ TEMPLATE + OOP)
 * - Tái sử dụng code cho mọi kiểu dữ liệu và mọi phép toán kết hợp (Sum, Min, Max, GCD, Matrix, Struct...)
 * - Dùng std::function linh hoạt truyền hàm merge (Lambda)
 * - Độ phức tạp: Build O(N), Update O(log N), Query O(log N)
 */

template <typename T>
class SegmentTree {
private:
    int n;
    vector<T> tree;
    T identity;                           // Giá trị trung hòa (Neutral Value)
    function<T(const T&, const T&)> merge; // Phép toán gộp 2 nút con (Lambda Function)

    void build(const vector<T>& a, int id, int l, int r) {
        if (l == r) {
            tree[id] = a[l];
            return;
        }
        int mid = (l + r) / 2;
        build(a, 2 * id, l, mid);
        build(a, 2 * id + 1, mid + 1, r);
        tree[id] = merge(tree[2 * id], tree[2 * id + 1]);
    }

    void update(int id, int l, int r, int pos, const T& val) {
        if (l == r) {
            tree[id] = val;
            return;
        }
        int mid = (l + r) / 2;
        if (pos <= mid) update(2 * id, l, mid, pos, val);
        else update(2 * id + 1, mid + 1, r, pos, val);
        tree[id] = merge(tree[2 * id], tree[2 * id + 1]);
    }

    T query(int id, int l, int r, int u, int v) const {
        if (v < l || u > r) return identity; // Trả về giá trị trung hòa khi nằm ngoài đoạn
        if (u <= l && r <= v) return tree[id];
        int mid = (l + r) / 2;
        return merge(query(2 * id, l, mid, u, v), 
                     query(2 * id + 1, mid + 1, r, u, v));
    }

public:
    // Constructor nhận vào:
    // - Mảng dữ liệu ban đầu
    // - Phép toán gộp (merge function)
    // - Giá trị trung hòa (identity value)
    SegmentTree(const vector<T>& a, 
                function<T(const T&, const T&)> merge_fn, 
                T id_val) 
        : merge(merge_fn), identity(id_val) {
        n = a.size();
        tree.resize(4 * n);
        if (n > 0) build(a, 1, 0, n - 1);
    }

    void update(int pos, const T& val) {
        update(1, 0, n - 1, pos, val);
    }

    T query(int u, int v) const {
        return query(1, 0, n - 1, u, v);
    }
};

// STRUCT MẪU DÙNG CHO DẠNG DỮ LIỆU PHỨC TẠP
struct NodeData {
    int min_val;
    int max_val;
};

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    cout << "================ TRACE 1: SEGMENT TREE TÍNH TỔNG (INT) ================\n";
    vector<int> nums = {3, 1, 4, 1, 5, 9};
    // Phép cộng (+), giá trị trung hòa = 0
    SegmentTree<int> sum_tree(
        nums, 
        [](int a, int b) { return a + b; }, 
        0
    );

    cout << "Tong doan [1, 4] (1 + 4 + 1 + 5): " << sum_tree.query(1, 4) << "\n";
    sum_tree.update(2, 10); // nums[2] = 10 -> {3, 1, 10, 1, 5, 9}
    cout << "Tong doan [1, 4] sau update nums[2]=10: " << sum_tree.query(1, 4) << "\n\n";


    cout << "================ TRACE 2: SEGMENT TREE TÌM MIN (DOUBLE) ================\n";
    vector<double> prices = {10.5, 3.2, 8.7, 1.5, 9.9};
    // Phép lấy min, giá trị trung hòa = INF
    SegmentTree<double> min_tree(
        prices, 
        [](double a, double b) { return min(a, b); }, 
        1e18
    );

    cout << "Gia nho nhat doan [0, 2] (10.5, 3.2, 8.7): " << min_tree.query(0, 2) << "\n\n";


    cout << "================ TRACE 3: SEGMENT TREE VỚI STRUCT PHỨC TẠP ================\n";
    // Mảng lưu các Struct chứa cả Min và Max đồng thời
    vector<NodeData> complex_data = {{5, 5}, {2, 2}, {9, 9}, {1, 1}, {7, 7}};
    
    // Phép merge gộp cả Min lẫn Max của 2 con
    auto merge_data = [](const NodeData& a, const NodeData& b) -> NodeData {
        return {min(a.min_val, b.min_val), max(a.max_val, b.max_val)};
    };
    NodeData identity_data = {INT_MAX, INT_MIN};

    SegmentTree<NodeData> struct_tree(complex_data, merge_data, identity_data);

    NodeData res = struct_tree.query(0, 3); // Đoạn [0, 3] bao gồm {5, 2, 9, 1}
    cout << "Doan [0, 3] -> Min: " << res.min_val << " | Max: " << res.max_val << "\n";

    /*
    ===========================================================================
    OUTPUT TRACE TỰ ĐỘNG CHẠY BỞI CHƯƠNG TRÌNH:
    ===========================================================================
    ================ TRACE 1: SEGMENT TREE TÍNH TỔNG (INT) ================
    Tong doan [1, 4] (1 + 4 + 1 + 5): 11
    Tong doan [1, 4] sau update nums[2]=10: 17

    ================ TRACE 2: SEGMENT TREE TÌM MIN (DOUBLE) ================
    Gia nho nhat doan [0, 2] (10.5, 3.2, 8.7): 3.2

    ================ TRACE 3: SEGMENT TREE VỚI STRUCT PHỨC TẠP ================
    Doan [0, 3] -> Min: 1 | Max: 9
    ===========================================================================
    */

    return 0;
}