#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <cmath>

using namespace std;

// Struct giả lập danh sách liên kết cho bài toán Floyd's Cycle
struct ListNode {
    int val;
    ListNode* next;
    ListNode(int x) : val(x), next(nullptr) {}
};

// ============================================================================
// NHÓM 1: BẢN CÀI ĐẶT KINH ĐIỂN CHO CP (COMPETITIVE PROGRAMMING)
// ============================================================================

namespace CP_TwoPointers {

    // ------------------------------------------------------------------------
    // 1.1 Model 1: Two Sum / 3Sum - Chạy ngược chiều hội tụ O(N^2)
    // ------------------------------------------------------------------------
    vector<vector<int>> threeSum(vector<int>& nums) {
        vector<vector<int>> result;
        sort(nums.begin(), nums.end()); // Bắt buộc phải Sort trước

        int n = nums.size();
        for (int i = 0; i < n - 2; ++i) {
            if (i > 0 && nums[i] == nums[i - 1]) continue; // Bỏ qua trùng lặp

            int left = i + 1, right = n - 1;
            while (left < right) {
                int sum = nums[i] + nums[left] + nums[right];
                if (sum == 0) {
                    result.push_back({nums[i], nums[left], nums[right]});
                    while (left < right && nums[left] == nums[left + 1]) left++;   // Tránh trùng
                    while (left < right && nums[right] == nums[right - 1]) right--; // Tránh trùng
                    left++;
                    right--;
                } else if (sum < 0) {
                    left++;
                } else {
                    right--;
                }
            }
        }
        return result;
    }

    // ------------------------------------------------------------------------
    // 1.2 Model 2: Floyd's Tortoise and Hare (Phát hiện chu trình) O(N) Time, O(1) Space
    // ------------------------------------------------------------------------
    ListNode* detectCycle(ListNode* head) {
        if (!head || !head->next) return nullptr;

        ListNode* slow = head;
        ListNode* fast = head;

        // BƯỚC 1: Tìm điểm giao nhau trong chu trình
        bool hasCycle = false;
        while (fast && fast->next) {
            slow = slow->next;          // Rùa nhảy 1 bước
            fast = fast->next->next;    // Thỏ nhảy 2 bước
            if (slow == fast) {
                hasCycle = true;
                break;
            }
        }

        if (!hasCycle) return nullptr;

        // BƯỚC 2: Tìm điểm BẮT ĐẦU của chu trình
        ListNode* ptr1 = head;
        ListNode* ptr2 = slow;
        while (ptr1 != ptr2) {
            ptr1 = ptr1->next;
            ptr2 = ptr2->next;
        }
        return ptr1; // Trả về node bắt đầu chu trình
    }

    // ------------------------------------------------------------------------
    // 1.3 Model 3: Merge Two Sorted Arrays - Hai mảng khác nhau O(N + M)
    // ------------------------------------------------------------------------
    vector<int> mergeSortedArrays(const vector<int>& A, const vector<int>& B) {
        int p1 = 0, p2 = 0;
        int n = A.size(), m = B.size();
        vector<int> result;
        result.reserve(n + m);

        while (p1 < n && p2 < m) {
            if (A[p1] <= B[p2]) {
                result.push_back(A[p1++]);
            } else {
                result.push_back(B[p2++]);
            }
        }

        while (p1 < n) result.push_back(A[p1++]);
        while (p2 < m) result.push_back(B[p2++]);

        return result;
    }
}

void demoCPGroup() {
    cout << "\n===================================================\n";
    cout << " 1. NHÓM THUẬT TOÁN CP (COMPETITIVE PROGRAMMING)\n";
    cout << "===================================================\n";

    // Demo 3Sum
    vector<int> nums = {-1, 0, 1, 2, -1, -4};
    auto triplets = CP_TwoPointers::threeSum(nums);
    cout << "  [3Sum O(N^2)] Các bộ 3 có tổng = 0: ";
    for (const auto& t : triplets) {
        cout << "[" << t[0] << "," << t[1] << "," << t[2] << "] ";
    }
    cout << "\n";

    // Demo Floyd Cycle Detection
    ListNode* n1 = new ListNode(3);
    ListNode* n2 = new ListNode(2);
    ListNode* n3 = new ListNode(0);
    ListNode* n4 = new ListNode(-4);
    n1->next = n2; n2->next = n3; n3->next = n4; n4->next = n2; // Tạo cycle tại n2
    ListNode* cycleStart = CP_TwoPointers::detectCycle(n1);
    cout << "  [Floyd Cycle] Chu trình bắt đầu tại Node có giá trị: " 
         << (cycleStart ? to_string(cycleStart->val) : "No Cycle") << "\n";

    // Demo Merge Step
    vector<int> A = {1, 3, 5, 7}, B = {2, 4, 6, 8};
    auto merged = CP_TwoPointers::mergeSortedArrays(A, B);
    cout << "  [Merge Two Arrays] Mảng hợp nhất: ";
    for (int x : merged) cout << x << " ";
    cout << "\n";
}

// ============================================================================
// NHÓM 2: BẢN CÀI ĐẶT KINH ĐIỂN CHO THỰC TẾ (PRODUCTION / SYSTEM)
// ============================================================================

namespace Production_TwoPointers {

    // ------------------------------------------------------------------------
    // 2.1 In-place Data Deduplication (Loại bỏ trùng lặp tại chỗ O(1) Space)
    // ------------------------------------------------------------------------
    int removeDuplicates(vector<int>& nums) {
        if (nums.empty()) return 0;

        int write = 0; // Con trỏ GHI (Write Pointer)
        for (int read = 1; read < nums.size(); ++read) { // Con trỏ ĐỌC (Read Pointer)
            if (nums[read] != nums[write]) {
                write++;
                nums[write] = nums[read];
            }
        }
        return write + 1; // Độ dài mảng mới sau khi lọc
    }

    // ------------------------------------------------------------------------
    // 2.2 Sort-Merge Join Simulator (Mô phỏng JOIN 2 Bảng trong Database SQL)
    // ------------------------------------------------------------------------
    struct Record {
        int id;
        string value;
    };

    void sortMergeJoin(vector<Record>& tableA, vector<Record>& tableB) {
        // BƯỚC 1: Sort 2 bảng theo Key (id)
        auto comp = [](const Record& a, const Record& b) { return a.id < b.id; };
        sort(tableA.begin(), tableA.end(), comp);
        sort(tableB.begin(), tableB.end(), comp);

        // BƯỚC 2: dùng 2 con trỏ Merge Join
        int pA = 0, pB = 0;
        cout << "  --- SQL SORT-MERGE JOIN RESULTS ---\n";
        while (pA < tableA.size() && pB < tableB.size()) {
            if (tableA[pA].id == tableB[pB].id) {
                int matched_id = tableA[pA].id;
                // Ghép tất cả các cặp có cùng ID
                int tempB = pB;
                while (pA < tableA.size() && tableA[pA].id == matched_id) {
                    pB = tempB;
                    while (pB < tableB.size() && tableB[pB].id == matched_id) {
                        cout << "    [MATCH] ID=" << matched_id 
                             << " | TableA: " << tableA[pA].value 
                             << " <==> TableB: " << tableB[pB].value << "\n";
                        pB++;
                    }
                    pA++;
                }
            } else if (tableA[pA].id < tableB[pB].id) {
                pA++;
            } else {
                pB++;
            }
        }
    }

    // ------------------------------------------------------------------------
    // 2.3 Run-Length Encoding (RLE - Thuật toán Nén chuỗi dữ liệu)
    // ------------------------------------------------------------------------
    string compressRLE(const string& src) {
        if (src.empty()) return "";

        string compressed = "";
        int start = 0;

        for (int current = 0; current < src.length(); ++current) {
            // Khi gặp ký tự khác hoặc tới cuối chuỗi
            if (current + 1 == src.length() || src[current] != src[current + 1]) {
                int count = current - start + 1;
                compressed += to_string(count) + src[start];
                start = current + 1; // Nhảy con trỏ bắt đầu sang khối tiếp theo
            }
        }
        return compressed;
    }

    // ------------------------------------------------------------------------
    // 2.4 Container With Most Water (Tối ưu hóa thiết kế hình học thực tế)
    // ------------------------------------------------------------------------
    int maxArea(const vector<int>& height) {
        int left = 0, right = height.size() - 1;
        int max_water = 0;

        while (left < right) {
            int h = min(height[left], height[right]);
            int w = right - left;
            max_water = max(max_water, h * w);

            // Dịch con trỏ ở cột ngắn hơn để tìm cơ hội có cột cao hơn
            if (height[left] < height[right]) {
                left++;
            } else {
                right--;
            }
        }
        return max_water;
    }
}

void demoProductionGroup() {
    cout << "\n===================================================\n";
    cout << " 2. NHÓM THUẬT TOÁN PRODUCTION (THỰC TẾ & SYSTEM)\n";
    cout << "===================================================\n";

    // Demo In-place Deduplication
    vector<int> dupArray = {1, 1, 2, 2, 3, 4, 4, 5};
    int newLen = Production_TwoPointers::removeDuplicates(dupArray);
    cout << "  [In-place Deduplication O(1) Space] Mảng sau lọc: ";
    for (int i = 0; i < newLen; ++i) cout << dupArray[i] << " ";
    cout << "\n";

    // Demo Sort-Merge Join
    vector<Production_TwoPointers::Record> t1 = {{2, "Alice"}, {1, "Bob"}, {3, "Charlie"}};
    vector<Production_TwoPointers::Record> t2 = {{1, "Dev"}, {2, "HR"}, {2, "Manager"}};
    Production_TwoPointers::sortMergeJoin(t1, t2);

    // Demo RLE Compression
    string rawData = "AAAAABBBCCDAA";
    cout << "  [RLE Compression] Chuỗi gốc: " << rawData 
         << " -> Chuỗi nén: " << Production_TwoPointers::compressRLE(rawData) << "\n";

    // Demo Container With Most Water
    vector<int> heights = {1, 8, 6, 2, 5, 4, 8, 3, 7};
    cout << "  [Container With Most Water] Lượng nước tối đa chứa được: " 
         << Production_TwoPointers::maxArea(heights) << "\n";
}

// ============================================================================
// MAIN DEMO
// ============================================================================
int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    cout << "===================================================\n";
    cout << " 🚀 TRỌN BỘ CÁC BẢN CÀI ĐẶT TWO POINTERS KINH ĐIỂN\n";
    cout << "===================================================\n";

    demoCPGroup();
    demoProductionGroup();

    return 0;
}