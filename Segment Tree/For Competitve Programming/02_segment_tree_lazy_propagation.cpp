#include <iostream>
#include <vector>

using namespace std;

/**
 * 2. SEGMENT TREE WITH LAZY PROPAGATION (RANGE UPDATE - RANGE QUERY)
 * - Tối ưu cho Lập trình thi đấu (CP)
 * - Bài toán mẫu: Range Add (cộng val vào [u, v]) & Range Sum Query (tính tổng [u, v])
 * - Kỹ thuật Lazy: "Nợ" giá trị cập nhật ở mảng lazy[], chỉ đẩy xuống con (push) khi cần
 * - Độ phức tạp: Build O(N), Update Đoạn O(log N), Query Đoạn O(log N)
 */

const int MAXN = 100000;
long long a[MAXN];          // Mảng dữ liệu gốc
long long tree[4 * MAXN];   // Mảng lưu tổng của Segment Tree
long long lazy[4 * MAXN];   // Mảng lưu giá trị hoãn lại (Lazy values)
int n;

// 1. DỰNG CÂY (BUILD)
void build(int id, int l, int r) {
    lazy[id] = 0; // Khởi tạo mảng lazy bằng 0
    if (l == r) {
        tree[id] = a[l];
        return;
    }
    int mid = (l + r) / 2;
    build(2 * id, l, mid);
    build(2 * id + 1, mid + 1, r);
    tree[id] = tree[2 * id] + tree[2 * id + 1];
}

// 2. ĐẨY GIÁ TRỊ LAZY XUỐNG CÁC NÚT CON (PUSH DOWN)
// Gọi hàm này trước khi đi xuống các con ở cả hàm update và query
void push(int id, int l, int r) {
    if (lazy[id] == 0) return; // Không có giá trị nợ -> Bỏ qua

    int mid = (l + r) / 2;

    // 1. Đẩy nợ xuống con trái và cập nhật tổng con trái
    lazy[2 * id] += lazy[id];
    tree[2 * id] += lazy[id] * (mid - l + 1);

    // 2. Đẩy nợ xuống con phải và cập nhật tổng con phải
    lazy[2 * id + 1] += lazy[id];
    tree[2 * id + 1] += lazy[id] * (r - (mid + 1) + 1);

    // 3. Xóa nợ ở nút hiện tại vì đã bàn giao cho con
    lazy[id] = 0;
}

// 3. CẬP NHẬT CẢ ĐOẠN [u, v] (RANGE UPDATE)
// Cộng 'val' vào tất cả phần tử a[i] với i thuộc [u, v]
void update_range(int id, int l, int r, int u, int v, long long val) {
    // TH 1: [l, r] nằm ngoài [u, v] -> Bỏ qua
    if (v < l || u > r) return;

    // TH 2: [l, r] nằm hoàn toàn trong [u, v] -> Cập nhật nút này & hoãn đệ quy
    if (u <= l && r <= v) {
        tree[id] += val * (r - l + 1); // Tổng tăng thêm val * số phần tử
        lazy[id] += val;               // Đánh dấu nợ cho các con
        return;
    }

    // TH 3: Giao một phần -> Đẩy nợ cũ xuống trước khi đệ quy xuống 2 con
    push(id, l, r);

    int mid = (l + r) / 2;
    update_range(2 * id, l, mid, u, v, val);
    update_range(2 * id + 1, mid + 1, r, u, v, val);

    // Cập nhật lại nút cha từ các nút con
    tree[id] = tree[2 * id] + tree[2 * id + 1];
}

// 4. TRUY VẤN TỔNG ĐOẠN [u, v] (RANGE QUERY)
long long query_range(int id, int l, int r, int u, int v) {
    // TH 1: [l, r] nằm ngoài [u, v]
    if (v < l || u > r) return 0;

    // TH 2: [l, r] nằm hoàn toàn trong [u, v]
    if (u <= l && r <= v) return tree[id];

    // TH 3: Giao một phần -> Đẩy nợ cũ xuống trước khi đi tiếp
    push(id, l, r);

    int mid = (l + r) / 2;
    return query_range(2 * id, l, mid, u, v) + 
           query_range(2 * id + 1, mid + 1, r, u, v);
}

// HÀM PRINT TRẠNG THÁI MẢNG TREE & LAZY (Dùng để Trace)
void print_status(int max_id) {
    cout << "  [Array tree[]]: ";
    for (int i = 1; i <= max_id; i++) {
        if (tree[i] != 0) cout << "t[" << i << "]=" << tree[i] << " ";
    }
    cout << "\n  [Array lazy[]]: ";
    for (int i = 1; i <= max_id; i++) {
        if (lazy[i] != 0) cout << "lz[" << i << "]=" << lazy[i] << " ";
    }
    cout << "\n";
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    vector<long long> input = {1, 3, 5, 7, 9, 11};
    n = input.size();
    for (int i = 0; i < n; i++) a[i] = input[i];

    cout << "================ TRACE BƯỚC 1: DỰNG CÂY (BUILD) ================\n";
    cout << "Mang ban dau a: ";
    for (int i = 0; i < n; i++) cout << a[i] << " ";
    cout << "\n\n";

    build(1, 0, n - 1);
    print_status(13);

    cout << "\n================ TRACE BƯỚC 2: CẬP NHẬT ĐOẠN (RANGE UPDATE) ================\n";
    int u = 1, v = 4;
    long long val = 10;
    cout << "Thuc hien: Cong " << val << " tao tat ca phan tu trong doan [" << u << ", " << v << "]\n";
    
    update_range(1, 0, n - 1, u, v, val);
    print_status(13);
    cout << "(Luu y: Cac nut con sau hon van chua bi tinh lai, gia tri cap nhat dang duoc hoan lai o lazy[])\n";

    cout << "\n================ TRACE BƯỚC 3: TRUY VẤN ĐOẠN (RANGE QUERY) ================\n";
    int q_u = 1, q_v = 2;
    cout << "Query tong doan [" << q_u << ", " << q_v << "]:\n";
    
    long long ans = query_range(1, 0, n - 1, q_u, q_v);
    cout << "=> Ket qua query: " << ans << "\n";
    print_status(13);
    cout << "(Luu y: Khi truy van cham den doan [1, 2], ham push() da kich hoat va day lazy xuong các nut con!)\n";

    /* 
    ===========================================================================
    OUTPUT TRACE TỰ ĐỘNG CHẠY BỞI CHƯƠNG TRÌNH:
    ===========================================================================
    ================ TRACE BƯỚC 1: DỰNG CÂY (BUILD) ================
    Mang ban dau a: 1 3 5 7 9 11 

      [Array tree[]]: t[1]=36 t[2]=9 t[3]=27 t[4]=4 t[5]=5 t[6]=16 t[7]=11 t[8]=1 t[9]=3 t[12]=7 t[13]=9 
      [Array lazy[]]: 

    ================ TRACE BƯỚC 2: CẬP NHẬT ĐOẠN (RANGE UPDATE) ================
    Thuc hien: Cong 10 tao tat ca phan tu trong doan [1, 4]
      [Array tree[]]: t[1]=76 t[2]=29 t[3]=47 t[4]=4 t[5]=25 t[6]=26 t[7]=11 t[8]=1 t[9]=3 t[12]=7 t[13]=9 
      [Array lazy[]]: lz[5]=10 lz[6]=10 
    (Luu y: Cac nut con sau hon van chua bi tinh lai, gia tri cap nhat dang duoc hoan lai o lazy[])

    ================ TRACE BƯỚC 3: TRUY VẤN ĐOẠN (RANGE QUERY) ================
    Query tong doan [1, 2]:
    => Ket qua query: 28
      [Array tree[]]: t[1]=76 t[2]=29 t[3]=47 t[4]=4 t[5]=25 t[6]=26 t[7]=11 t[8]=1 t[9]=13 t[10]=15 t[12]=7 t[13]=9 
      [Array lazy[]]: lz[6]=10 lz[9]=10 lz[10]=10 
    (Luu y: Khi truy van cham den doan [1, 2], ham push() da kich hoat va day lazy xuong các nut con!)
    ===========================================================================
    */

    return 0;
}