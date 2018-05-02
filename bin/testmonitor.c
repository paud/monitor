/*
Cuckoo Sandbox - Automated Malware Analysis.
Copyright (C) 2010-2015 Cuckoo Foundation.

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <stdio.h>
#include <windows.h>
#include "config.h"
#include "diffing.h"
#include "hooking.h"
#include "ignore.h"
#include "log.h"
#include "memory.h"
#include "misc.h"
#include "monitor.h"
#include "native.h"
#include "pipe.h"
#include "sleep.h"
#include "symbol.h"
#include "unhook.h"

#define IDB_ONE 3301
#define IDB_TWO 3302
#define IDB_THREE 3303

void monitor_init(HMODULE module_handle)
{
    // Sends crashes to the process rather than showing error popup boxes etc.
    SetErrorMode(SEM_FAILCRITICALERRORS | SEM_NOALIGNMENTFAULTEXCEPT |
                 SEM_NOGPFAULTERRORBOX | SEM_NOOPENFILEERRORBOX);

    config_t cfg;
    config_read(&cfg);

    // Required to be initialized before any logging starts.
    mem_init();

    // Initialize capstone without our custom allocator as it is
    // not available yet.
    hook_init(module_handle);

    pipe_init(cfg.pipe_name, cfg.pipe_pid);
    native_init();

    // Re-initialize capstone with our custom allocator which is now
    // accessible after native_init().
    hook_init2();

    misc_init(cfg.shutdown_mutex);
    diffing_init(cfg.hashes_path, cfg.diffing_enable);

    copy_init();
    log_init(cfg.logpipe, cfg.track);
    ignore_init();

    misc_init2(&monitor_hook, &monitor_unhook);

    sleep_init(cfg.first_process, cfg.force_sleep_skip, cfg.startup_time);

    // Disable the unhook detection for now. TODO Re-enable.
    // unhook_init_detection(cfg.first_process);

    hide_module_from_peb(module_handle);

    if (cfg.disguise != 0)
    {
        // Set the processor count to two.
        set_processor_count(2);

        // Pretend like we have two extra gigabytes of memory.
        add_virtual_memory(2 * 1024 * 1024 * 1024ull);
    }

    symbol_init(module_handle);

    // Should be the last as some of the other initialization routines extract
    // the image size, EAT pointers, etc while the PE header is still intact.
    destroy_pe_header(module_handle);

    misc_set_monitor_options(cfg.track, cfg.mode, cfg.trigger);
}

void monitor_hook(const char *library, void *module_handle)
{
    // Initialize data about each hook.
    for (hook_t *h = sig_hooks(); h->funcname != NULL; h++)
    {
        // If a specific library has been specified then we skip all other
        // libraries. This feature is used in the special hook for LdrLoadDll.
        if (library != NULL && stricmp(h->library, library) != 0)
        {
            continue;
        }

        // We only hook this function if the monitor mode is "hook everything"
        // or if the monitor mode matches the mode of this hook.
        if (g_monitor_mode != HOOK_MODE_ALL &&
            (g_monitor_mode & h->mode) == 0)
        {
            continue;
        }

        // Return value 1 indicates to retry the hook. This is important for
        // delay-loaded function forwarders as the delay-loaded DLL may
        // already have been loaded. In that case we want to hook the function
        // forwarder right away. (Note that the library member of the hook
        // object is updated in the case of retrying).
        while (hook(h, module_handle) == 1)
            ;
    }
}

void monitor_unhook(const char *library, void *module_handle)
{
    (void)library;

    for (hook_t *h = sig_hooks(); h->funcname != NULL; h++)
    {
        // This module was unloaded.
        if (h->module_handle == module_handle)
        {
            h->is_hooked = 0;
            h->addr = NULL;
        }

        // This is a hooked function which doesn't belong to a particular DLL.
        // Therefore the module handle is a nullptr and we simply check
        // whether the address of the original function is still in-memory.
        if (h->module_handle == NULL && range_is_readable(h->addr, 16) == 0)
        {
            h->is_hooked = 0;
            h->addr = NULL;
        }
    }
}
BOOL APIENTRY DllMain(HANDLE hModule, DWORD dwReason, LPVOID lpReserved);
LRESULT CALLBACK MainWndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    char szText[] = "hook测试程序！";
    switch (message)
    {
    case WM_CREATE:
    {
        CreateWindow("Button", "按钮一", WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,
                     35, 30, 120, 60, hwnd, (HMENU)IDB_ONE, hwnd, NULL);
        CreateWindow("Button", "按钮二", WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,
                     35, 100, 120, 60, hwnd, (HMENU)IDB_TWO, hwnd, NULL);
        CreateWindow("Button", "按钮三", WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,
                     35, 170, 120, 60, hwnd, (HMENU)IDB_THREE, hwnd, NULL);
    }
    break;
    case WM_PAINT: // 窗口客户区需要重画
    {
        HDC hdc;
        PAINTSTRUCT ps;

        // 使无效的客户区变的有效，并取得设备环境句柄
        hdc = BeginPaint(hwnd, &ps);
        // 显示文字
        TextOut(hdc, 10, 10, szText, strlen(szText));
        EndPaint(hwnd, &ps);
        return 0;
    }
    case WM_COMMAND:
    {
        switch (LOWORD(wParam))
        {
        case IDB_ONE:
            {
                //monitor_hook(NULL, NULL);
                DWORD h=LoadLibrary("user32.dll"); //关于系统dll,这里会根据便宜出的PE格式自动调用x64 或 x86的dll
                DllMain(h,DLL_PROCESS_ATTACH,NULL);
            }
            //MessageBox(hwnd, "您点击了第一个按钮。", "提示", MB_OK | MB_ICONINFORMATION);
            break;
        case IDB_TWO:
            MessageBox(hwnd, "您点击了第二个按钮。", "提示", MB_OK | MB_ICONINFORMATION);
            break;
        case IDB_THREE:
            MessageBox(hwnd, "您点击了第三个按钮。", "提示", MB_OK | MB_ICONINFORMATION);
            break;
        default:
            break;
        }
    }
    break;
    case WM_DESTROY: // 正在销毁窗口

        // 向消息队列投递一个WM_QUIT消息，促使GetMessage函数返回0，结束消息循环
        PostQuitMessage(0);
        return 0;
    }

    // 将我们不处理的消息交给系统做默认处理
    return DefWindowProc(hwnd, message, wParam, lParam);
}

BOOL APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
    char szClassName[] = "MainWClass";
    WNDCLASSEX wndclass;

    // 用描述主窗口的参数填充WNDCLASSEX结构
    wndclass.cbSize = sizeof(wndclass);       // 结构的大小
    wndclass.style = CS_HREDRAW | CS_VREDRAW; // 指定如果大小改变就重画
    wndclass.lpfnWndProc = MainWndProc;       // 窗口函数指针
    wndclass.cbClsExtra = 0;                  // 没有额外的类内存
    wndclass.cbWndExtra = 0;                  // 没有额外的窗口内存
    wndclass.hInstance = hInstance;           // 实例句柄
    wndclass.hIcon = LoadIcon(NULL,
                              IDI_APPLICATION); // 使用预定义图标
    wndclass.hCursor = LoadCursor(NULL,
                                  IDC_ARROW); // 使用预定义的光标
    wndclass.hbrBackground = (HBRUSH)
        GetStockObject(WHITE_BRUSH);      // 使用白色背景画刷
    wndclass.lpszMenuName = NULL;         // 不指定菜单
    wndclass.lpszClassName = szClassName; // 窗口类的名称
    wndclass.hIconSm = NULL;              // 没有类的小图标

    // 注册这个窗口类
    RegisterClassEx(&wndclass);

    // 创建主窗口
    HWND hwnd = CreateWindowEx(
        0,                   // dwExStyle，扩展样式
        szClassName,         // lpClassName，类名
        "My hook test Window!",  // lpWindowName，标题
        WS_OVERLAPPEDWINDOW, // dwStyle，窗口风格
        CW_USEDEFAULT,       // X，初始 X 坐标
        CW_USEDEFAULT,       // Y，初始 Y 坐标
        CW_USEDEFAULT,       // nWidth，宽度
        CW_USEDEFAULT,       // nHeight，高度
        NULL,                // hWndParent，父窗口句柄
        NULL,                // hMenu，菜单句柄
        hInstance,           // hlnstance，程序实例句柄
        NULL);               // lpParam，用户数据

    if (hwnd == NULL)
    {
        MessageBox(NULL, "创建窗口出错！", "error", MB_OK);
        return -1;
    }

    // 显示窗口，刷新窗口客户区
    ShowWindow(hwnd, nShowCmd);
    UpdateWindow(hwnd);

    // 从消息堆中取出消息
    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0))
    {
        // 转化键盘消息
        TranslateMessage(&msg);
        // 将消息发送到相应的窗口函数
        DispatchMessage(&msg);
    }

    // 当GetMessage返回0时程序结束
    return msg.wParam;
}

BOOL APIENTRY DllMain(HANDLE hModule, DWORD dwReason, LPVOID lpReserved)
{
    (void) hModule; (void) lpReserved;

    if(dwReason == DLL_PROCESS_ATTACH && is_ignored_process() == 0) {
        monitor_init(hModule);
        monitor_hook(NULL, NULL);
        pipe("LOADED:%d,%d", get_current_process_id(), g_monitor_track);
    }

    return TRUE;
}
