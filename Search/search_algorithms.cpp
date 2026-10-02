#include <iostream>
#include <vector>
#include <algorithm>
#include <unordered_map>
#include <iomanip>
#include <cmath>

using namespace std;

// ============================================================================
// NHÓM 1: TÌM KIẾM KINH ĐIỂN (LINEAR & BINARY SEARCH)
// ============================================================================

// 1.1 Linear Search (Tìm kiếm tuyến tính) - O(n)
int linearSearchTrace(const vector<int>& arr, int target) {
    cout << "\n---------------------------------------------------\n";
    cout << " 1.1 LINEAR SEARCH (Tuyến tính) | Target = " << target << "\n";
    cout << "---------------------------------------------------\n";

    for (size_t i = 0; i < arr.size(); ++i) {
        cout << "  [Bước " << i + 1 << "] So sánh arr[" << i << "] = " << arr[i];
        if (arr[i] == target) {
            cout << " ==> [FOUND!] Tìm thấy tại vị trí (index) " << i << "\n";
            return i;
        }
        cout << " (Không khớp, tiếp tục...)\n";
    }
    cout << "  ==> [NOT FOUND] Không tìm thấy phần tử trong mảng!\n";
    return -1;
}

// 1.2 Binary Search (Tìm kiếm nhị phân) - O(log n)
int binarySearchTrace(const vector<int>& arr, int target) {
    cout << "\n---------------------------------------------------\n";
    cout << " 1.2 BINARY SEARCH (Nhị phân) | Target = " << target << "\n";
    cout << "---------------------------------------------------\n";

    int left = 0, right = (int)arr.size() - 1;
    int step = 1;

    while (left <= right) {
        int mid = left + (right - left) / 2;
        cout << "  [Bước " << step++ << "] Phạm vi [" << left << ".." << right << "] | Mid index = " << mid << " (Giá trị = " << arr[mid] << ")";

        if (arr[mid] == target) {
            cout << " ==> [FOUND!] Tìm thấy tại vị trí " << mid << "\n";
            return mid;
        }

        if (arr[mid] < target) {
            cout << " -> Target lớn hơn, thu hẹp sang NỬA PHẢI\n";
            left = mid + 1;
        } else {
            cout << " -> Target nhỏ hơn, thu hẹp sang NỬA TRÁI\n";
            right = mid - 1;
        }
    }
    cout << "  ==> [NOT FOUND] Không tìm thấy phần tử trong mảng!\n";
    return -1;
}

// ============================================================================
// NHÓM 2: BIẾN THỂ VÀ NÂNG CAO (INTERPOLATION & TREE SEARCH)
// ============================================================================

// 2.1 Interpolation Search (Tìm kiếm nội suy) - Average O(log log n), Worst O(n)
int interpolationSearchTrace(const vector<int>& arr, int target) {
    cout << "\n---------------------------------------------------\n";
    cout << " 2.1 INTERPOLATION SEARCH (Nội suy) | Target = " << target << "\n";
    cout << "---------------------------------------------------\n";

    int low = 0, high = (int)arr.size() - 1;
    int step = 1;

    while (low <= high && target >= arr[low] && target <= arr[high]) {
        if (low == high) {
            if (arr[low] == target) return low;
            return -1;
        }

        // Công thức ước lượng vị trí nội suy (Dự đoán điểm lật trang)
        int pos = low + (((double)(high - low) / (arr[high] - arr[low])) * (target - arr[low]));

        cout << "  [Bước " << step++ << "] Phạm vi [" << low << ".." << high 
             << "] | Dự đoán vị trí pos = " << pos << " (Giá trị = " << arr[pos] << ")";

        if (arr[pos] == target) {
            cout << " ==> [FOUND!] Dự đoán chính xác tại index " << pos << "\n";
            return pos;
        }

        if (arr[pos] < target) {
            cout << " -> Target nằm phía sau, dịch low = pos + 1\n";
            low = pos + 1;
        } else {
            cout << " -> Target nằm phía trước, dịch high = pos - 1\n";
            high = pos - 1;
        }
    }

    cout << "  ==> [NOT FOUND] Không tìm thấy phần tử!\n";
    return -1;
}

// 2.2 Binary Search Tree (Cây tìm kiếm nhị phân đơn giản) - Average O(log n)
struct BSTNode {
    int val;
    BSTNode* left;
    BSTNode* right;
    BSTNode(int v) : val(v), left(nullptr), right(nullptr) {}
};

BSTNode* insertBST(BSTNode* root, int val) {
    if (!root) return new BSTNode(val);
    if (val < root->val) root->left = insertBST(root->left, val);
    else if (val > root->val) root->right = insertBST(root->right, val);
    return root;
}

bool searchBSTTrace(BSTNode* root, int target, int level = 1) {
    if (!root) {
        cout << "  [Nấc " << level << "] Gặp node NULL -> [NOT FOUND]\n";
        return false;
    }

    cout << "  [Nấc " << level << "] Đang xét Node(" << root->val << ")";
    if (root->val == target) {
        cout << " ==> [FOUND!] Tìm thấy giá trị trên Cây BST!\n";
        return true;
    }

    if (target < root->val) {
        cout << " -> " << target << " < " << root->val << " (Rẽ TRÁI)\n";
        return searchBSTTrace(root->left, target, level + 1);
    } else {
        cout << " -> " << target << " > " << root->val << " (Rẽ PHẢI)\n";
        return searchBSTTrace(root->right, target, level + 1);
    }
}

// ============================================================================
// NHÓM 3: HASH SEARCH (BẢNG BĂM) - Average O(1)
// ============================================================================

void hashSearchTrace(const vector<pair<string, string>>& dataset, const string& search_key) {
    cout << "\n---------------------------------------------------\n";
    cout << " 3. HASH SEARCH (Bảng băm / Hash Map) | Key = \"" << search_key << "\"\n";
    cout << "---------------------------------------------------\n";

    unordered_map<string, string> hashMap;

    // Nạp dữ liệu vào Hash Table - O(1) trung bình cho mỗi insert
    for (const auto& item : dataset) {
        hashMap[item.first] = item.second;
    }

    cout << "Đã nạp " << dataset.size() << " phần tử vào std::unordered_map.\n";
    cout << "Thực hiện tính Hash value cho key \"" << search_key << "\":\n";

    size_t hash_code = hash<string>{}(search_key);
    cout << "  -> Hash Code tính được = " << hash_code << "\n";

    // Tìm kiếm trong Hash Table - O(1)
    auto it = hashMap.find(search_key);
    if (it != hashMap.end()) {
        cout << "  ==> [FOUND IN O(1)!] Key: \"" << it->first << "\" | Value: \"" << it->second << "\"\n";
    } else {
        cout << "  ==> [NOT FOUND] Key không tồn tại trong Hash Table!\n";
    }
}

// ============================================================================
// MAIN DEMO
// ============================================================================
int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    cout << "===================================================\n";
    cout << "   BỘ CÀI ĐẶT CÁC THUẬT TOÁN TÌM KIẾM (SEARCHING)\n";
    cout << "===================================================\n";

    // Mảng dữ liệu mẫu (đã sắp xếp cho Binary & Interpolation Search)
    vector<int> sorted_arr = {10, 20, 30, 45, 60, 80, 100, 130, 170, 200, 250};
    int target = 130;

    // 1. Nhóm Kinh điển
    linearSearchTrace(sorted_arr, target);
    binarySearchTrace(sorted_arr, target);

    // 2. Nhóm Biến thể & Nâng cao
    interpolationSearchTrace(sorted_arr, target);

    // Demo Cây BST
    cout << "\n---------------------------------------------------\n";
    cout << " 2.2 TREE SEARCH (Cây tìm kiếm nhị phân BST)\n";
    cout << "---------------------------------------------------\n";
    BSTNode* bst_root = nullptr;
    for (int x : {50, 30, 70, 20, 40, 60, 80}) bst_root = insertBST(bst_root, x);
    cout << "Dự tính tìm giá trị 60 trên Cây BST:\n";
    searchBSTTrace(bst_root, 60);

    // 3. Nhóm Bảng băm
    vector<pair<string, string>> user_db = {
        {"user_101", "Nguyen Van A"},
        {"user_102", "Tran Thi B"},
        {"user_103", "Le Van C"}
    };
    hashSearchTrace(user_db, "user_102");

    return 0;
}