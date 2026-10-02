#include <iostream>
#include <vector>

using namespace std;

/**
 * 3. ITERATIVE SEGMENT TREE (NON-RECURSIVE)
 * - Tối ưu cực hạn cho CP: Không đệ quy, chạy nhanh gấp 2-3 lần bản thông thường.
 * - Bài toán mẫu: Point Update & Range Sum Query (0-indexed cho mảng đầu vào).
 * - Kích thước mảng tree: 2 * N.
 * - Độ phức tạp: Build O(N), Update O(log N), Query O(log N) với hằng số cực nhỏ.
 */

const int MAXN = 100000;
int n;
int tree[2 * MAXN]; // Nút lá nằm từ index n đến 2n - 1. Nút gốc tại index 1.

// 1. DỰNG CÂY BẰNG VÒNG LẶP (BUILD)
void build(const vector<int>& a) {
    n = a.size();
    // Gán dữ liệu vào các nút lá
    for (int i = 0; i < n; i++) {
        tree[n + i] = a[i];
    }
    // Dựng các nút cha từ dưới lên
    for (int i = n - 1; i > 0; --i) {
        tree[i] = tree[i << 1] + tree[i << 1 | 1]; // tree[i] = tree[2*i] + tree[2*i + 1]
    }
}

// 2. CẬP NHẬT 1 ĐIỂM (POINT UPDATE)
// Thay đổi a[pos] = val
void update(int pos, int val) {
    // Sửa giá trị tại nút lá tương ứng
    for (tree[pos += n] = val; pos > 1; pos >>= 1) {
        // Cập nhật lại nút cha (pos >> 1) từ 2 con (pos và pos ^ 1)
        tree[pos >> 1] = tree[pos] + tree[pos ^ 1];
    }
}

// 3. TRUY VẤN ĐOẠN [l, r] ĐÓNG (RANGE QUERY)
// Tính tổng các phần tử từ a[l] đến a[r]
int query(int l, int r) {
    int res = 0;
    // Chuyển l, r về chỉ số nút lá
    for (l += n, r += n + 1; l < r; l >>= 1, r >>= 1) {
        // Nếu l là con phải của cha nó -> l không thể đóng góp cho cha, ta phải cộng trực tiếp tree[l]
        if (l & 1) res += tree[l++];
        // Nếu r là con phải -> (r-1) là con trái, ta phải cộng trực tiếp tree[r-1]
        if (r & 1) res += tree[--r];
    }
    return res;
}

// HÀM PRINT TRẠNG THÁI CÂY TẠI CÁC TẦNG (Dùng để Trace)
void print_tree_status() {
    cout << "  [Array tree[]]:\n";
    cout << "  - Nut la (a[0.." << n-1 << "]): ";
    for (int i = 0; i < n; i++) cout << "t[" << n + i << "]=" << tree[n + i] << " ";
    cout << "\n  - Nut cha trong cây : ";
    for (int i = 1; i < n; i++) cout << "t[" << i << "]=" << tree[i] << " ";
    cout << "\n";
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    vector<int> a = {2, 1, 5, 3, 4};

    cout << "================ TRACE BƯỚC 1: DỰNG CÂY BẰNG VÒNG LẶP (BUILD) ================\n";
    cout << "Mang ban dau a: ";
    for (int x : a) cout << x << " ";
    cout << "\n\n";

    build(a);
    print_tree_status();

    cout << "\n================ TRACE BƯỚC 2: TRUY VẤN ĐOẠN [1, 3] (QUERY) ================\n";
    int L = 1, R = 3;
    cout << "Query tong doan [" << L << ", " << R << "] (Cac phan tu: " 
         << a[1] << " + " << a[2] << " + " << a[3] << "):\n";
    
    int ans = query(L, R);
    cout << "=> Ket qua query: " << ans << "\n";

    cout << "\n================ TRACE BƯỚC 3: CẬP NHẬT A[2] = 10 (UPDATE) ================\n";
    int pos = 2, new_val = 10;
    cout << "Update a[" << pos << "] tu " << a[pos] << " thanh " << new_val << "\n";
    
    a[pos] = new_val;
    update(pos, new_val);
    print_tree_status();

    cout << "\nQuery lai tong doan [" << L << ", " << R << "] sau update:\n";
    ans = query(L, R);
    cout << "=> Ket qua query moi: " << ans << "\n";

    /*
    ===========================================================================
    OUTPUT TRACE TỰ ĐỘNG CHẠY BỞI CHƯƠNG TRÌNH:
    ===========================================================================
    ================ TRACE BƯỚC 1: DỰNG CÂY BẰNG VÒNG LẶP (BUILD) ================
    Mang ban dau a: 2 1 5 3 4 

      [Array tree[]]:
      - Nut la (a[0..4]): t[5]=2 t[6]=1 t[7]=5 t[8]=3 t[9]=4 
      - Nut cha trong cây : t[1]=15 t[2]=8 t[3]=7 t[4]=3 

    ================ TRACE BƯỚC 2: TRUY VẤN ĐOẠN [1, 3] (QUERY) ================
    Query tong doan [1, 3] (Cac phan tu: 1 + 5 + 3):
    => Ket qua query: 9

    ================ TRACE BƯỚC 3: CẬP NHẬT A[2] = 10 (UPDATE) ================
    Update a[2] tu 5 thanh 10
      [Array tree[]]:
      - Nut la (a[0..4]): t[5]=2 t[6]=1 t[7]=10 t[8]=3 t[9]=4 
      - Nut cha trong cây : t[1]=20 t[2]=8 t[3]=12 t[4]=3 

    Query lai tong doan [1, 3] sau update:
    => Ket qua query moi: 14
    ===========================================================================
    */

    return 0;
}