
#include <iostream>
#include <vector>

using namespace std;

int main() {
    // Thao tác nhanh với I/O
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    // Mảng dữ liệu ban đầu (chỉ số chạy từ 1)
    vector<int> A = {0, 2, -1, 3, 5}; 
    int n = A.size() - 1; // n = 4

    // Khởi tạo mảng prefix sum có kích thước n + 1, tất cả phần tử bằng 0
    vector<int> S(n + 1, 0);

    // Bước 1: Xây dựng mảng cộng dồn O(N)
    for (int i = 1; i <= n; i++) {
        cout << "S[i=" << i << "] = S[i - 1] + A[i=" << i << "] \n= "
            << S[i - 1] << " + " << A[i] << " \n= ";
        S[i] = S[i - 1] + A[i];
        cout << S[i] << endl << endl;
    }

    // In mảng Prefix Sum để kiểm tra
    cout << "Mang Prefix Sum S: ";
    for (int i = 1; i <= n; i++) {
        cout << S[i] << " ";
    }
    cout << "\n\n";

    // Bước 2: Truy vấn tính tổng đoạn từ L đến R trong O(1)
    // Ví dụ: Tính tổng từ vị trí 2 đến vị trí 4 (tức là: -1 + 3 + 5 = 7)
    int L = 2, R = 4;
    cout << "sum = S[R=" << R << "] - S[L - 1] \n= "
        << "S[R=" << R << "] - S[" << L << " - 1] \n= "
        << "S[R=" << R << "] - S[" << L - 1 << "] \n= "
        << S[R] << " - " << S[L - 1] << " \n= ";
    int sum = S[R] - S[L - 1];
    cout << sum << endl << endl;
    
    cout << "Tong cac phan tu tu vi tri " << L << " den " << R << " la: " << sum << "\n";
    /*
    Trace: 
    
    S[i=1] = S[i - 1] + A[i=1] 
    = 0 + 2 
    = 2

    S[i=2] = S[i - 1] + A[i=2] 
    = 2 + -1 
    = 1

    S[i=3] = S[i - 1] + A[i=3] 
    = 1 + 3 
    = 4

    S[i=4] = S[i - 1] + A[i=4] 
    = 4 + 5 
    = 9

    Mang Prefix Sum S: 2 1 4 9 

    sum = S[R=4] - S[L - 1] 
    = S[R=4] - S[2 - 1] 
    = S[R=4] - S[1] 
    = 9 - 2 
    = 7

    Tong cac phan tu tu vi tri 2 den 4 la: 7*/
    
    return 0;
}
