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

// 帧率相关定义
#define DEFAULT_CAPTURE_FPS 20000  // 默认采集帧率 500KHz
#define DEFAULT_DISPLAY_FPS 60      // 默认显示帧率 60Hz

// 窗口相关定义
#define WINDOW_CLASS_NAME "SequenceCaptureWindow"
#define WINDOW_TITLE "video display"
HWND g_hwnd = NULL;
BITMAPINFO* g_bmi = NULL;
unsigned char* g_displayBuffer = NULL;
int g_width = 0;
int g_height = 0;
BOOL g_running = TRUE;

// 全局变量
int g_capture_fps = DEFAULT_CAPTURE_FPS;  // 采集帧率
int g_display_fps = DEFAULT_DISPLAY_FPS;  // 显示帧率

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
        case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hwnd, &ps);
            
            if (g_displayBuffer) {
                RECT rect;
                GetClientRect(hwnd, &rect);
                StretchDIBits(hdc,
                    0, 0, rect.right, rect.bottom,
                    0, 0, g_width, g_height,
                    g_displayBuffer, g_bmi,
                    DIB_RGB_COLORS, SRCCOPY);
            }
            
            EndPaint(hwnd, &ps);
            return 0;
        }
            
        case WM_CLOSE:
            g_running = FALSE;
            return 0;
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

    printf("Please open the EPIX PIXCI frame capture card...\n");
    
    driverparms[sizeof(driverparms)-1] = 0;
    _snprintf(driverparms, sizeof(driverparms)-1, "-DM 0x%x %s", UNITSOPENMAP, DRIVERPARMS);

    #if defined(FORMATFILE)
        printf("Using format file: %s\n", FORMATFILE);
        r = pxd_PIXCIopen(driverparms, "", FORMATFILE);
    #endif

    if (r < 0) {
        printf("Open error %s(%d)\n", pxd_mesgErrorCode(r), r);
        pxd_mesgFault(UNITSMAP);
        return r;
    }

    // 获取图像尺寸并初始化显示缓冲区
    g_width = pxd_imageXdim();
    g_height = pxd_imageYdim();
    g_displayBuffer = (unsigned char*)malloc(g_width * g_height);
    g_bmi = create_bitmap_info(g_width, g_height);

    if (!g_displayBuffer || !g_bmi) {
        printf("Memory allocation failed\n");
        return -1;
    }

    printf("Camera initialization succeeded\n");
    return r;
}

// 修改capture_and_display_sequence函数
int capture_and_display_sequence(void)
{
    int err;
    DWORD start_time = GetTickCount();
    DWORD frames = 0;
    
    printf("Entering capture loop...\n");
    
    const int DISPLAY_INTERVAL = 1000 / g_display_fps;
    const int FRAMES_PER_DISPLAY = g_capture_fps / g_display_fps;
    
    printf("Display interval: %d ms, Frames per display: %d\n", 
           DISPLAY_INTERVAL, FRAMES_PER_DISPLAY);

    // 确保停止之前的采集
    pxd_goUnLive(UNITSMAP);
    
    // 序列采集参数
    int startbuf = 1;         // 起始缓冲区
    int endbuf = 4;           // 结束缓冲区
    int incbuf = 1;           // 缓冲区增量
    int numbuf = 0;           // 0表示连续循环采集
    int videoperiod = 1;      // 每帧之间的间隔，1表示视频速率采集
    
    // 启动序列采集
    err = pxd_goLiveSeq(UNITSMAP, startbuf, endbuf, incbuf, numbuf, videoperiod);
    if (err < 0) {
        printf("pxd_goLiveSeq error: %s\n", pxd_mesgErrorCode(err));
        return err;
    }
    printf("Sequence capture started...\n");

    int current_buffer = startbuf;
    DWORD last_display_time = GetTickCount();
    BOOL need_repaint = TRUE;

    while (g_running) {
        // 只处理重绘消息
        MSG msg;
        if (need_repaint) {
            if (PeekMessage(&msg, NULL, WM_PAINT, WM_PAINT, PM_REMOVE)) {
                TranslateMessage(&msg);
                DispatchMessage(&msg);
                need_repaint = FALSE;
            }
            // else if (PeekMessage(&msg, NULL, WM_QUIT, WM_QUIT, PM_REMOVE)) {
            //     g_running = FALSE;
            //     break;
            // }
        }
        

        // 等待当前缓冲区采集完成
        int captured_buffer;
        DWORD wait_start = GetTickCount();
        const DWORD TIMEOUT = 100;
        
        do {
            captured_buffer = pxd_capturedBuffer(UNITSMAP);
            if (GetTickCount() - wait_start > TIMEOUT) {
                printf("Wait for buffer timeout, last captured: %d\n", captured_buffer);
                break;
            }
        } while (captured_buffer != current_buffer && g_running);

        if (!g_running) break;

        // 读取当前缓冲区的图像数据
        err = pxd_readuchar(UNITSMAP, current_buffer, 0, 0, -1, -1, 
                           g_displayBuffer, g_width * g_height, "Grey");
        if (err < 0) {
            printf("Read image error: %s\n", pxd_mesgErrorCode(err));
            return err;
        }

        // 更新显示（基于时间间隔）
        DWORD current_time = GetTickCount();
        if (current_time - last_display_time >= DISPLAY_INTERVAL) {
            need_repaint = TRUE;  // 标记需要重绘
            InvalidateRect(g_hwnd, NULL, FALSE);
            last_display_time = current_time;
            
            // 增强的状态显示
            if (current_time - start_time >= 1000) {
                float fps = (float)frames * 1000 / (current_time - start_time);
                float display_fps = 1000.0f / DISPLAY_INTERVAL;
                printf("\rStats: Capture: %.2f fps | Display: %.2f fps | Buffer: %d/%d | Memory: %d/%d KB", 
                    fps,                          // 采集帧率
                    display_fps,                  // 显示帧率
                    captured_buffer,              // 当前缓冲区
                    endbuf,                       // 总缓冲区数
                    g_width * g_height,           // 当前帧大小
                    g_width * g_height * endbuf   // 总缓冲区大小
                );
                fflush(stdout);
                frames = 0;
                start_time = current_time;
            }
        }

        // 更新到下一个缓冲区
        current_buffer++;
        if (current_buffer > endbuf) {
            current_buffer = startbuf;
        }
        frames++;
    }

    cleanup:
    // 停止采集时显示最终统计信息
    printf("\n\nFinal Statistics:\n");
    printf("Total frames captured: %d\n", frames);
    printf("Average capture rate: %.2f fps\n", (float)frames * 1000 / (GetTickCount() - start_time));
    printf("Total running time: %.2f seconds\n", (float)(GetTickCount() - start_time) / 1000);
    
    // 停止采集
    pxd_goUnLive(UNITSMAP);
    printf("\nCapture stopped\n");
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

    printf("Starting program...\n");  // 添加调试信息

    // 创建显示窗口
    if (!create_display_window()) {
        printf("Failed to create window\n");
        return 1;
    }
    printf("Window created successfully\n");  // 添加调试信息

    // 初始化相机
    if (initialize_camera() < 0) {
        cleanup();
        return 1;
    }
    printf("Camera initialized successfully\n");  // 添加调试信息

    // 显示图像参数
    printf("\nImage information:\n");
    printf("Width: %d\n", g_width);
    printf("Height: %d\n", g_height);
    printf("Bit depth: %d\n", pxd_imageBdim());
    printf("Buffer count: %d\n", pxd_imageZdim());

    printf("Starting capture sequence...\n");  // 添加调试信息
    
    // 执行连续采集和显示
    if (capture_and_display_sequence() < 0) {
        printf("Capture sequence failed\n");  // 添加调试信息
        cleanup();
        return 1;
    }

    // 清理资源
    cleanup();
    printf("Program closed\n");

    return 0;
} 