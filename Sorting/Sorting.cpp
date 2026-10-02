#include <iostream>
#include <vector>
#include <algorithm>
#include <iomanip>

using namespace std;

// In mảng phục vụ Trace
void printArray(const vector<int>& a) {
    for (int x : a) cout << x << " ";
    cout << "\n";
}

// ----------------------------------------------------------------------------
// 1. BUBBLE SORT (SẮP XẾP NỔI BỘT)
// ----------------------------------------------------------------------------
void bubbleSortTrace(vector<int> a) {
    int n = a.size();
    cout << "\n===================================================\n";
    cout << " 1. BUBBLE SORT (SẮP XẾP NỔI BỘT)\n";
    cout << "===================================================\n";
    cout << "Ban dau: "; printArray(a);

    for (int i = 0; i < n - 1; i++) {
        bool swapped = false;
        cout << "-- Pass " << i + 1 << " --\n";
        for (int j = 0; j < n - i - 1; j++) {
            if (a[j] > a[j + 1]) {
                cout << "  Swap (" << a[j] << ", " << a[j+1] << ") => ";
                swap(a[j], a[j + 1]);
                swapped = true;
                printArray(a);
            }
        }
        if (!swapped) {
            cout << "  (Mang da sap xep, dung som!)\n";
            break;
        }
    }
}

// ----------------------------------------------------------------------------
// 2. SELECTION SORT (SẮP XẾP CHỌN)
// ----------------------------------------------------------------------------
void selectionSortTrace(vector<int> a) {
    int n = a.size();
    cout << "\n===================================================\n";
    cout << " 2. SELECTION SORT (SẮP XẾP CHỌN)\n";
    cout << "===================================================\n";
    cout << "Ban dau: "; printArray(a);

    for (int i = 0; i < n - 1; i++) {
        int min_idx = i;
        for (int j = i + 1; j < n; j++) {
            if (a[j] < a[min_idx]) {
                min_idx = j;
            }
        }
        if (min_idx != i) {
            cout << "Pass " << i + 1 << ": Doi a[" << i << "]=" << a[i] 
                 << " voi min a[" << min_idx << "]=" << a[min_idx] << " => ";
            swap(a[i], a[min_idx]);
            printArray(a);
        } else {
            cout << "Pass " << i + 1 << ": Min van la a[" << i << "]=" << a[i] << " => ";
            printArray(a);
        }
    }
}

// ----------------------------------------------------------------------------
// 3. INSERTION SORT (SẮP XẾP CHÈN)
// ----------------------------------------------------------------------------
void insertionSortTrace(vector<int> a) {
    int n = a.size();
    cout << "\n===================================================\n";
    cout << " 3. INSERTION SORT (SẮP XẾP CHÈN)\n";
    cout << "===================================================\n";
    cout << "Ban dau: "; printArray(a);

    for (int i = 1; i < n; i++) {
        int key = a[i];
        int j = i - 1;
        cout << "Chen key = " << key << ": ";
        while (j >= 0 && a[j] > key) {
            a[j + 1] = a[j];
            j--;
        }
        a[j + 1] = key;
        printArray(a);
    }
}

// ----------------------------------------------------------------------------
// 4. QUICK SORT (SẮP XẾP NHANH - LOMUTO)
// ----------------------------------------------------------------------------
int partitionTrace(vector<int>& a, int low, int high) {
    int pivot = a[high];
    cout << "  [Partition [" << low << ".." << high << "]] Pivot = " << pivot << "\n";
    int i = low - 1;

    for (int j = low; j < high; j++) {
        if (a[j] < pivot) {
            i++;
            swap(a[i], a[j]);
        }
    }
    swap(a[i + 1], a[high]);
    cout << "  -> Mang sau khi chia mien quanh Pivot: ";
    printArray(a);
    return i + 1;
}

void quickSortTrace(vector<int>& a, int low, int high) {
    if (low < high) {
        int pi = partitionTrace(a, low, high);
        quickSortTrace(a, low, pi - 1);
        quickSortTrace(a, pi + 1, high);
    }
}

// ----------------------------------------------------------------------------
// 5. MERGE SORT (SẮP XẾP TRỘN)
// ----------------------------------------------------------------------------
void mergeTrace(vector<int>& a, int left, int mid, int right) {
    vector<int> L(a.begin() + left, a.begin() + mid + 1);
    vector<int> R(a.begin() + mid + 1, a.begin() + right + 1);

    int i = 0, j = 0, k = left;
    while (i < L.size() && j < R.size()) {
        if (L[i] <= R[j]) a[k++] = L[i++];
        else a[k++] = R[j++];
    }

    while (i < L.size()) a[k++] = L[i++];
    while (j < R.size()) a[k++] = R[j++];

    cout << "  Tron doan [" << left << ".." << right << "]: ";
    for (int idx = left; idx <= right; idx++) cout << a[idx] << " ";
    cout << "\n";
}

void mergeSortTrace(vector<int>& a, int left, int right) {
    if (left < right) {
        int mid = left + (right - left) / 2;
        mergeSortTrace(a, left, mid);
        mergeSortTrace(a, mid + 1, right);
        mergeTrace(a, left, mid, right);
    }
}

// ----------------------------------------------------------------------------
// 6. HEAP SORT (SẮP XẾP VUN ĐỐNG)
// ----------------------------------------------------------------------------
void heapifyTrace(vector<int>& a, int n, int i) {
    int largest = i;
    int left = 2 * i + 1;
    int right = 2 * i + 2;

    if (left < n && a[left] > a[largest]) largest = left;
    if (right < n && a[right] > a[largest]) largest = right;

    if (largest != i) {
        swap(a[i], a[largest]);
        heapifyTrace(a, n, largest);
    }
}

void heapSortTrace(vector<int> a) {
    int n = a.size();
    cout << "\n===================================================\n";
    cout << " 6. HEAP SORT (SẮP XẾP VUN ĐỐNG)\n";
    cout << "===================================================\n";
    cout << "Ban dau: "; printArray(a);

    // Build Max-Heap
    for (int i = n / 2 - 1; i >= 0; i--) {
        heapifyTrace(a, n, i);
    }
    cout << "  [Heapify] Max-Heap ban dau: "; printArray(a);

    // Extract elements from Heap
    for (int i = n - 1; i > 0; i--) {
        swap(a[0], a[i]);
        heapifyTrace(a, i, 0);
        cout << "  -> Dua max " << a[i] << " ve cuoi [size=" << i << "]: ";
        printArray(a);
    }
}

// ----------------------------------------------------------------------------
// 7. RADIX SORT (SẮP XẾP THEO HÀNG CHỮ SỐ)
// ----------------------------------------------------------------------------
void countingSortForRadix(vector<int>& a, int exp) {
    int n = a.size();
    vector<int> output(n);
    int count[10] = {0};

    for (int i = 0; i < n; i++) {
        int digit = (a[i] / exp) % 10;
        count[digit]++;
    }

    for (int i = 1; i < 10; i++) {
        count[i] += count[i - 1];
    }

    for (int i = n - 1; i >= 0; i--) {
        int digit = (a[i] / exp) % 10;
        output[count[digit] - 1] = a[i];
        count[digit]--;
    }

    for (int i = 0; i < n; i++) a[i] = output[i];
}

void radixSortTrace(vector<int> a) {
    cout << "\n===================================================\n";
    cout << " 7. RADIX SORT (SẮP XẾP THEO HÀNG CHỮ SỐ)\n";
    cout << "===================================================\n";
    cout << "Ban dau: "; printArray(a);

    if (a.empty()) return;
    int max_val = *max_element(a.begin(), a.end());

    for (int exp = 1; max_val / exp > 0; exp *= 10) {
        countingSortForRadix(a, exp);
        cout << "  -> Sap xep theo hang " << exp << ": ";
        printArray(a);
    }
}

// ----------------------------------------------------------------------------
// MAIN FUNCTION DEMO FULL 7 SORTING ALGORITHMS
// ----------------------------------------------------------------------------
int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    cout << "***************************************************\n";
    cout << "*  BỘ SỐ CÂU LẠC BỘ 7 THUẬT TOÁN SẮP XẾP C++      *\n";
    cout << "***************************************************\n";

    vector<int> std_arr = {64, 34, 25, 12, 22, 11, 90};
    vector<int> radix_arr = {170, 45, 75, 90, 802, 24, 2, 66};

    // 1. Bubble Sort
    bubbleSortTrace(std_arr);

    // 2. Selection Sort
    selectionSortTrace(std_arr);

    // 3. Insertion Sort
    insertionSortTrace(std_arr);

    // 4. Quick Sort
    cout << "\n===================================================\n";
    cout << " 4. QUICK SORT (SẮP XẾP NHANH)\n";
    cout << "===================================================\n";
    cout << "Ban dau: "; printArray(std_arr);
    vector<int> a4 = std_arr;
    quickSortTrace(a4, 0, a4.size() - 1);
    cout << "Ket qua: "; printArray(a4);

    // 5. Merge Sort
    cout << "\n===================================================\n";
    cout << " 5. MERGE SORT (SẮP XẾP TRỘN)\n";
    cout << "===================================================\n";
    cout << "Ban dau: "; printArray(std_arr);
    vector<int> a5 = std_arr;
    mergeSortTrace(a5, 0, a5.size() - 1);
    cout << "Ket qua: "; printArray(a5);

    // 6. Heap Sort
    heapSortTrace(std_arr);

    // 7. Radix Sort
    radixSortTrace(radix_arr);

    cout << "\n===================================================\n";
    cout << " HOÀN THÀNH TRACE TOÀN BỘ 7 THUẬT TOÁN!\n";
    cout << "===================================================\n";

    return 0;
}