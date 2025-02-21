/*
 * sequence_capture.c
 * 基于XCLIB实现序列图像采集和视频输出
 */

#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include "xcliball.h"

// 定义参数
#if !defined(FORMAT) && !defined(FORMATFILE)
    #define FORMATFILE "xcvidset.fmt"
#endif

#if !defined(UNITS)
    #define UNITS 1
#endif
#define UNITSMAP ((1<<UNITS)-1)

#if !defined(UNITSOPENMAP)
    #define UNITSOPENMAP UNITSMAP
#endif

#if !defined(DRIVERPARMS)
    #define DRIVERPARMS ""
#endif

// 窗口相关定义
#define WINDOW_CLASS_NAME "SequenceCaptureWindow"
#define WINDOW_TITLE "实时图像显示"
HWND g_hwnd = NULL;
BITMAPINFO* g_bmi = NULL;
unsigned char* g_displayBuffer = NULL;
int g_width = 0;
int g_height = 0;
BOOL g_running = TRUE;

// 错误处理函数
void signaled(int sig)
{
    g_running = FALSE;
    pxd_PIXCIclose();
    exit(1);
}

// 创建位图信息头
BITMAPINFO* create_bitmap_info(int width, int height)
{
    BITMAPINFO* bmi = (BITMAPINFO*)malloc(sizeof(BITMAPINFO));
    if (!bmi) return NULL;

    memset(bmi, 0, sizeof(BITMAPINFO));
    bmi->bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
    bmi->bmiHeader.biWidth = width;
    bmi->bmiHeader.biHeight = -height; // 负值表示从上到下的位图
    bmi->bmiHeader.biPlanes = 1;
    bmi->bmiHeader.biBitCount = 8;     // 8位灰度图
    bmi->bmiHeader.biCompression = BI_RGB;

    // 设置灰度调色板
    unsigned char* palette = (unsigned char*)&bmi->bmiColors[0];
    for(int i = 0; i < 256; i++) {
        palette[i*4 + 0] = i;  // 蓝色
        palette[i*4 + 1] = i;  // 绿色
        palette[i*4 + 2] = i;  // 红色
        palette[i*4 + 3] = 0;  // 保留
    }

    return bmi;
}

// 窗口过程
LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
        case WM_CLOSE:
            g_running = FALSE;
            DestroyWindow(hwnd);
            return 0;

        case WM_DESTROY:
            PostQuitMessage(0);
            return 0;

        case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hwnd, &ps);
            
            if (g_displayBuffer) {
                // 获取客户区大小
                RECT rect;
                GetClientRect(hwnd, &rect);
                
                // 显示图像，自动缩放到窗口大小
                StretchDIBits(hdc,
                    0, 0, rect.right, rect.bottom,
                    0, 0, g_width, g_height,
                    g_displayBuffer, g_bmi,
                    DIB_RGB_COLORS, SRCCOPY);
            }
            
            EndPaint(hwnd, &ps);
            return 0;
        }
    }
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

// 创建显示窗口
BOOL create_display_window(void)
{
    WNDCLASS wc = {0};
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = GetModuleHandle(NULL);
    wc.lpszClassName = WINDOW_CLASS_NAME;
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
    
    if (!RegisterClass(&wc)) return FALSE;

    // 创建窗口
    g_hwnd = CreateWindow(
        WINDOW_CLASS_NAME, WINDOW_TITLE,
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT,
        800, 600,  // 初始窗口大小
        NULL, NULL,
        GetModuleHandle(NULL), NULL
    );

    if (!g_hwnd) return FALSE;

    ShowWindow(g_hwnd, SW_SHOW);
    UpdateWindow(g_hwnd);
    return TRUE;
}

// 初始化相机
int initialize_camera(void)
{
    int r;
    char driverparms[80];

    printf("正在打开EPIX PIXCI帧采集卡...\n");
    
    driverparms[sizeof(driverparms)-1] = 0;
    _snprintf(driverparms, sizeof(driverparms)-1, "-DM 0x%x %s", UNITSOPENMAP, DRIVERPARMS);

    #if defined(FORMATFILE)
        printf("使用格式文件: %s\n", FORMATFILE);
        r = pxd_PIXCIopen(driverparms, "", FORMATFILE);
    #endif

    if (r < 0) {
        printf("打开错误 %s(%d)\n", pxd_mesgErrorCode(r), r);
        pxd_mesgFault(UNITSMAP);
        return r;
    }

    // 获取图像尺寸并初始化显示缓冲区
    g_width = pxd_imageXdim();
    g_height = pxd_imageYdim();
    g_displayBuffer = (unsigned char*)malloc(g_width * g_height);
    g_bmi = create_bitmap_info(g_width, g_height);

    if (!g_displayBuffer || !g_bmi) {
        printf("内存分配失败\n");
        return -1;
    }

    printf("相机初始化成功\n");
    return r;
}

// 处理Windows消息
void process_messages(void)
{
    MSG msg;
    while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
}

// 连续采集并显示图像，直到用户关闭窗口或按Ctrl+C
int capture_and_display_sequence(void)
{
    int err;
    DWORD start_time = GetTickCount();
    DWORD frames = 0;
    
    printf("开始连续采集图像...\n");
    printf("按Ctrl+C或关闭窗口停止采集\n");

    while (g_running) {
        // 采集一帧图像
        err = pxd_goLive(UNITSMAP, 1);
        if (err < 0) {
            printf("pxd_goLive 错误: %s\n", pxd_mesgErrorCode(err));
            return err;
        }

        // 等待采集完成
        while (pxd_goneLive(UNITSMAP, 0)) {
            Sleep(1);
            process_messages();
            if (!g_running) goto cleanup; // 用户请求退出
        }

        // 读取图像数据到显示缓冲区
        err = pxd_readuchar(UNITSMAP, 1, 0, 0, -1, -1, g_displayBuffer, g_width * g_height, "Grey");
        if (err < 0) {
            printf("读取图像错误: %s\n", pxd_mesgErrorCode(err));
            return err;
        }

        // 刷新显示
        InvalidateRect(g_hwnd, NULL, FALSE);
        UpdateWindow(g_hwnd);
        
        // 处理Windows消息
        process_messages();
        
        // 计算并显示帧率
        frames++;
        DWORD current_time = GetTickCount();
        DWORD elapsed_time = current_time - start_time;
        if (elapsed_time >= 1000) { // 每秒更新一次帧率
            printf("\r当前帧率: %.2f fps", (float)frames * 1000 / elapsed_time);
            fflush(stdout);
            frames = 0;
            start_time = current_time;
        }
    }

cleanup:
    printf("\n采集已停止\n");
    return 0;
}

// 清理资源
void cleanup(void)
{
    if (g_displayBuffer) {
        free(g_displayBuffer);
        g_displayBuffer = NULL;
    }
    if (g_bmi) {
        free(g_bmi);
        g_bmi = NULL;
    }
    pxd_PIXCIclose();
}

// 主函数
int main(void)
{
    // 注册信号处理
    signal(SIGINT, signaled);
    signal(SIGFPE, signaled);

    // 创建显示窗口
    if (!create_display_window()) {
        printf("创建窗口失败\n");
        return 1;
    }

    // 初始化相机
    if (initialize_camera() < 0) {
        cleanup();
        return 1;
    }

    // 显示图像参数
    printf("\n图像信息:\n");
    printf("宽度: %d\n", g_width);
    printf("高度: %d\n", g_height);
    printf("位深: %d\n", pxd_imageBdim());
    printf("缓冲区数量: %d\n", pxd_imageZdim());

    // 执行连续采集和显示
    if (capture_and_display_sequence() < 0) {
        cleanup();
        return 1;
    }

    // 清理资源
    cleanup();
    printf("程序已关闭\n");

    return 0;
} 