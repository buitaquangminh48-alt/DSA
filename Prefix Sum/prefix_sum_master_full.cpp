#include <iostream>
#include <vector>
#include <string>
#include <cmath>
#include <algorithm>
#include <unordered_map>
#include <iomanip>
#include <cstdint>

using namespace std;

// ============================================================================
// NHÓM 1: BẢN CÀI ĐẶT KINH ĐIỂN CHO CP (COMPETITIVE PROGRAMMING)
// ============================================================================

namespace CP_PrefixSum {

    // ------------------------------------------------------------------------
    // 1.1 Model 1: 2D Prefix Sum (Inclusion-Exclusion Principle)
    // ------------------------------------------------------------------------
    class PrefixSum2D {
    private:
        vector<vector<int>> pref;
    public:
        PrefixSum2D(const vector<vector<int>>& matrix) {
            if (matrix.empty() || matrix[0].empty()) return;
            int R = matrix.size();
            int C = matrix[0].size();
            pref.assign(R + 1, vector<int>(C + 1, 0));

            for (int r = 0; r < R; ++r) {
                for (int c = 0; c < C; ++c) {
                    pref[r + 1][c + 1] = matrix[r][c] 
                                       + pref[r][c + 1] 
                                       + pref[r + 1][c] 
                                       - pref[r][c];
                }
            }
        }

        // Truy vấn tổng hình chữ nhật từ (r1, c1) tới (r2, c2) - 0-indexed - O(1)
        int query(int r1, int c1, int r2, int c2) const {
            return pref[r2 + 1][c2 + 1] 
                 - pref[r1][c2 + 1] 
                 - pref[r2 + 1][c1] 
                 + pref[r1][c1];
        }
    };

    // ------------------------------------------------------------------------
    // 1.2 Model 2: Difference Array + Prefix Sum (Range Update O(1))
    // ------------------------------------------------------------------------
    class DifferenceArray {
    private:
        vector<int> diff;
        int n;
    public:
        DifferenceArray(int size) : n(size), diff(size + 1, 0) {}

        // Cộng val vào tất cả phần tử trong đoạn [L, R] - O(1)
        void updateRange(int L, int R, int val) {
            diff[L] += val;
            if (R + 1 < n) {
                diff[R + 1] -= val;
            }
        }

        // Tích hợp Prefix Sum để khôi phục mảng ban đầu - O(N)
        vector<int> getFinalArray() {
            vector<int> result(n, 0);
            int current_sum = 0;
            for (int i = 0; i < n; ++i) {
                current_sum += diff[i];
                result[i] = current_sum;
            }
            return result;
        }
    };

    // ------------------------------------------------------------------------
    // 1.3 Model 3: Prefix XOR (Tìm subsegment có tổng XOR = K) O(N)
    // ------------------------------------------------------------------------
    int countSubarraysWithXorK(const vector<int>& arr, int K) {
        unordered_map<int, int> pref_freq;
        pref_freq[0] = 1; // Base case: prefix XOR = 0 xuất hiện 1 lần

        int current_xor = 0;
        int count = 0;

        for (int x : arr) {
            current_xor ^= x;
            int target = current_xor ^ K;
            
            if (pref_freq.count(target)) {
                count += pref_freq[target];
            }
            pref_freq[current_xor]++;
        }
        return count;
    }
}

void demoCPGroup() {
    cout << "\n===================================================\n";
    cout << " 1. NHÓM THUẬT TOÁN CP (COMPETITIVE PROGRAMMING)\n";
    cout << "===================================================\n";

    // Demo 2D Prefix Sum
    vector<vector<int>> grid = {
        {3, 0, 1, 4, 2},
        {5, 6, 3, 2, 1},
        {1, 2, 0, 1, 5},
        {4, 1, 0, 1, 7},
        {1, 0, 3, 0, 5}
    };
    CP_PrefixSum::PrefixSum2D ps2d(grid);
    cout << "  [2D Prefix Sum O(1)] Tổng vùng (2,1) -> (4,3): " 
         << ps2d.query(2, 1, 4, 3) << " (Expected: 8)\n";

    // Demo Difference Array
    CP_PrefixSum::DifferenceArray diffArr(5);
    diffArr.updateRange(1, 3, 10); // Cộng 10 vào [1..3]
    diffArr.updateRange(2, 4, 5);  // Cộng 5 vào [2..4]
    auto finalArr = diffArr.getFinalArray();
    cout << "  [Difference Array] Mảng sau 2 thao tác Range Update: ";
    for (int x : finalArr) cout << x << " ";
    cout << "\n";

    // Demo Prefix XOR
    vector<int> xorArr = {4, 2, 2, 6, 4};
    int K = 6;
    cout << "  [Prefix XOR O(N)] Số đoạn con có XOR = " << K << ": " 
         << CP_PrefixSum::countSubarraysWithXorK(xorArr, K) << "\n";
}

// ============================================================================
// NHÓM 2: BẢN CÀI ĐẶT KINH ĐIỂN CHO THỰC TẾ (PRODUCTION / SYSTEM)
// ============================================================================

namespace Production_PrefixSum {

    // ------------------------------------------------------------------------
    // 2.1 Integral Image / Summed-Area Table (Box Blur Filter)
    // ------------------------------------------------------------------------
    void applyBoxBlur(const vector<vector<int>>& image, int radius) {
        CP_PrefixSum::PrefixSum2D integralImage(image);
        int R = image.size();
        int C = image[0].size();
        vector<vector<int>> blurred(R, vector<int>(C, 0));

        for (int r = 0; r < R; ++r) {
            for (int c = 0; c < C; ++c) {
                int r1 = max(0, r - radius);
                int c1 = max(0, c - radius);
                int r2 = min(R - 1, r + radius);
                int c2 = min(C - 1, c + radius);

                int sum = integralImage.query(r1, c1, r2, c2);
                int area = (r2 - r1 + 1) * (c2 - c1 + 1);
                blurred[r][c] = sum / area; // Lấy giá trị trung bình pixel
            }
        }

        cout << "  [Integral Image] Kết quả Box Blur 3x3 pixel góc (1,1): " 
             << blurred[1][1] << "\n";
    }

    // ------------------------------------------------------------------------
    // 2.2 Radix Sort (Sử dụng Prefix Sum trên Counting Array) O(N)
    // ------------------------------------------------------------------------
    void countSortByDigit(vector<int>& arr, int exp) {
        int n = arr.size();
        vector<int> output(n);
        vector<int> count(10, 0);

        for (int i = 0; i < n; ++i) {
            count[(arr[i] / exp) % 10]++;
        }

        for (int i = 1; i < 10; ++i) {
            count[i] += count[i - 1];
        }

        for (int i = n - 1; i >= 0; --i) {
            int digit = (arr[i] / exp) % 10;
            output[count[digit] - 1] = arr[i];
            count[digit]--;
        }

        for (int i = 0; i < n; ++i) {
            arr[i] = output[i];
        }
    }

    void radixSort(vector<int>& arr) {
        if (arr.empty()) return;
        int max_val = *max_element(arr.begin(), arr.end());

        for (int exp = 1; max_val / exp > 0; exp *= 10) {
            countSortByDigit(arr, exp);
        }
    }

    // ------------------------------------------------------------------------
    // 2.3 Parallel Scan Concept (Exclusive Scan / Blelloch Scan)
    // ------------------------------------------------------------------------
    vector<int> exclusiveScan(const vector<int>& input) {
        int n = input.size();
        vector<int> output(n, 0);
        int acc = 0;
        for (int i = 0; i < n; ++i) {
            output[i] = acc;
            acc += input[i];
        }
        return output;
    }

    // ------------------------------------------------------------------------
    // 2.4 Stream Cipher Stream Generation (Prefix XOR Cryptography cho IoT)
    // ------------------------------------------------------------------------
    class LightweightStreamCipher {
    private:
        uint8_t key;
    public:
        LightweightStreamCipher(uint8_t secretKey) : key(secretKey) {}

        // Mã hóa: Ciphertext[i] = Plaintext[i] ^ Key ^ Ciphertext[i-1] (Chế độ Prefix XOR)
        vector<uint8_t> encrypt(const string& plaintext) {
            vector<uint8_t> ciphertext(plaintext.length());
            uint8_t pref_xor = key; // Khởi tạo với Secret Key (hoặc IV)

            for (size_t i = 0; i < plaintext.length(); ++i) {
                uint8_t encrypted_byte = plaintext[i] ^ pref_xor;
                ciphertext[i] = encrypted_byte;
                // Cập nhật Prefix XOR bằng cách tích lũy Ciphertext vừa tạo
                pref_xor ^= encrypted_byte;
            }
            return ciphertext;
        }

        // Giải mã: Plaintext[i] = Ciphertext[i] ^ Key ^ Ciphertext[i-1]
        string decrypt(const vector<uint8_t>& ciphertext) {
            string plaintext = "";
            uint8_t pref_xor = key;

            for (size_t i = 0; i < ciphertext.size(); ++i) {
                uint8_t decrypted_byte = ciphertext[i] ^ pref_xor;
                plaintext += (char)decrypted_byte;
                // Tích lũy lại Prefix XOR tương tự để đảo ngược quá trình
                pref_xor ^= ciphertext[i];
            }
            return plaintext;
        }
    };
}

void demoProductionGroup() {
    cout << "\n===================================================\n";
    cout << " 2. NHÓM THUẬT TOÁN PRODUCTION (THỰC TẾ & SYSTEM)\n";
    cout << "===================================================\n";

    // Demo Integral Image (Box Blur Filter)
    vector<vector<int>> image = {
        {10, 20, 30, 40},
        {50, 60, 70, 80},
        {90, 100, 110, 120},
        {130, 140, 150, 160}
    };
    Production_PrefixSum::applyBoxBlur(image, 1);

    // Demo Radix Sort
    vector<int> rawData = {170, 45, 75, 90, 802, 24, 2, 66};
    Production_PrefixSum::radixSort(rawData);
    cout << "  [Radix Sort O(N)] Mảng sau khi sắp xếp bằng Prefix Sum Index: ";
    for (int x : rawData) cout << x << " ";
    cout << "\n";

    // Demo Exclusive Scan
    vector<int> stream = {3, 1, 7, 0, 4, 1, 6, 3};
    auto exScan = Production_PrefixSum::exclusiveScan(stream);
    cout << "  [Exclusive Scan (GPU Stream Concept)] Mảng cộng dồn loại trừ: ";
    for (int x : exScan) cout << x << " ";
    cout << "\n";

    // Demo Stream Cipher Cryptography
    string secretMessage = "HelloIoTWorld!";
    uint8_t secretKey = 0xAA; // 10101010 in binary
    Production_PrefixSum::LightweightStreamCipher cipher(secretKey);

    auto encrypted = cipher.encrypt(secretMessage);
    string decrypted = cipher.decrypt(encrypted);

    cout << "  [Lightweight Stream Cipher - Prefix XOR Crypto]:\n";
    cout << "    - Message Gốc: " << secretMessage << "\n";
    cout << "    - Ciphertext (HEX): ";
    for (uint8_t b : encrypted) {
        cout << hex << uppercase << setw(2) << setfill('0') << (int)b << " ";
    }
    cout << dec << "\n";
    cout << "    - Decrypted Message: " << decrypted << "\n";
}

// ============================================================================
// MAIN DEMO
// ============================================================================
int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    cout << "===================================================\n";
    cout << " 🚀 TRỌN BỘ CÁC BẢN CÀI ĐẶT PREFIX SUM KINH ĐIỂN (FULL)\n";
    cout << "===================================================\n";

    demoCPGroup();
    demoProductionGroup();

    return 0;
}