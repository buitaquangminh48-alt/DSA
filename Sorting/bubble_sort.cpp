#include "raylib.h"
#include <vector>
#include <cstdlib>
#include <ctime>

int main() {
    // 1. Khởi tạo màn hình
    const int screenWidth = 800;
    const int screenHeight = 450;
    InitWindow(screenWidth, screenHeight, "DSA Visualizer voi Raylib - Bubble Sort");

    // 2. Tạo mảng dữ liệu ngẫu nhiên (Vẽ thành các cột chữ nhật)
    const int barCount = 40;
    const int barWidth = screenWidth / barCount;
    std::vector<int> arr(barCount);
    
    srand(time(0));
    for (int i = 0; i < barCount; i++) {
        arr[i] = rand() % (screenHeight - 50) + 10; // Chiều cao ngẫu nhiên
    }

    // Biến quản lý trạng thái thuật toán
    int i = 0, j = 0;
    bool sorted = false;

    SetTargetFPS(30); // Đặt tốc độ 60 khung hình/giây

    // 3. Vòng lặp đồ họa chính
    while (!WindowShouldClose()) {
        
        // --- PHẦN LOGIC THUẬT TOÁN (Chạy từng bước một mỗi frame) ---
        if (!sorted) {
            if (arr[j] > arr[j + 1]) {
                // Hoán đổi
                int temp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = temp;
            }
            
            j++;
            if (j >= barCount - i - 1) {
                j = 0;
                i++;
                if (i >= barCount - 1) {
                    sorted = true; // Thuật toán hoàn thành
                }
            }
        }

        // --- PHẦN VẼ GIAO DIỆN (RENDER) ---
        BeginDrawing();
        ClearBackground(BLACK); // Nền đen huyền bí

        for (int k = 0; k < barCount; k++) {
            Color barColor = WHITE;
            
            // Đổi màu sinh động để biết thuật toán đang làm gì
            if (!sorted) {
                if (k == j || k == j + 1) barColor = RED;       // Cột đang được so sánh
                else if (k > barCount - i - 1) barColor = GREEN; // Cột đã nằm đúng vị trí
            } else {
                barColor = GREEN; // Hoàn thành tất cả màu xanh lá
            }

            // Vẽ cột chữ nhật tương ứng với giá trị phần tử
            DrawRectangle(k * barWidth, screenHeight - arr[k], barWidth - 2, arr[k], barColor);
        }

        DrawText(sorted ? "DA SAP XEP XONG!" : "Dang chay Bubble Sort...", 20, 20, 20, LIGHTGRAY);
        
        EndDrawing();
    }

    CloseWindow();
    return 0;
}