#include "framework.h"
#include "MbWebApp.h"
#include "mb.h"
#include<string>
#include<iostream>
#include<fstream>

mbWebView MB_CALL_TYPE handleCreateView(mbWebView webView, void* param, mbNavigationType navigationType, const utf8* url, const mbWindowFeatures* windowFeatures)
{
    /*mbWebView mbView = mbCreateWebWindow(MB_WINDOW_TYPE_POPUP, NULL, 110, 60, 600, 700);
    ::mbOnCreateView(mbView, handleCreateView, nullptr);
    ::mbSetNavigationToNewWindowEnable(mbView, true);
    ::mbSetCspCheckEnable(mbView, true);
    ::mbShowWindow(mbView, TRUE);
    return mbView;*/
    return webView;
}

static BOOL MB_CALL_TYPE handleLoadUrlBegin(mbWebView webView, void* param, const char* url, void* job)
{
    //     std::string urlStr("handleLoadUrlBegin:");
    //     urlStr += url;
    //     urlStr += "\n";
    //     printf(urlStr.c_str());
    //
    //     if (hookUrl(job, url, "universal-report.min.js", L"G:\\test\\web_test\\51iwifi\\universal-report.min.js", "text/html"))
    //         return true;
    // 
    //     if (hookUrl(job, url, "_app-f54e3843f15fa10c7198.js", L"D:\\test\\web\\cc_163\\_app-f54e3843f15fa10c7198.js", "text/javascript"))
    //         return true;
    // 
    //     if (hookUrl(job, url, "act/webcc/link-pk-game/v1.9.1/index.js", L"D:\\test\\web\\cc_163\\webcc_191_index.js", "text/javascript"))
    //         return true;
    // 
    //     if (hookUrl(job, url, "act/webcc/performance-reporter/v1.2.0/index.js", L"D:\\test\\web\\cc_163\\performance-reporter.js", "text/javascript"))
    //         return true;
    // 
    //     if (0 != strstr(url, "mini_original.js?v")) {
    //         mbNetChangeRequestUrl(job, "http://192.168.83.1:8080/mini_original.js");
    //     }
    return TRUE;
}

void MB_CALL_TYPE onJsQueryCallback(mbWebView webView, void* param, mbJsExecState es, int64_t queryId, int customMsg, const utf8* request)
{
    mbResponseQuery(webView, queryId, customMsg, "I am response");
}

BOOL MB_CALL_TYPE onCloseCallback(mbWebView webView, void* param, void* unuse)
{
    mbExitMessageLoop();
    return TRUE; // 当真实窗口模式下收到WM_CLOSE消息时触发此回调。可通过在回调中返回false来拒绝关闭窗口
}

void MB_CALL_TYPE onDocumentReadyCallback(mbWebView webView, void* param, mbWebFrameHandle frameId)
{
    OutputDebugStringA("onDocumentReadyCallback\n");
    const  utf8* url = mbGetUrl(webView);
    OutputDebugStringA(url);
    OutputDebugStringA("\n");
    mbStringPtr srcHtml = mbGetSourceSync(webView);
    size_t strLen = mbGetStringLen(srcHtml);
    const utf8* html = mbGetString(srcHtml);
    std::string shtml(html, strLen);
    OutputDebugStringA(shtml.c_str());

    std::ofstream file("aqy.html", std::ios::out | std::ios::binary|std::ios::trunc);
    if (file.is_open()) {
        file.write(shtml.data(), shtml.size());
        file.flush();
        file.close();
    }

}

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);
    mbSetMbMainDllPath(L"mb132_x64.dll");
    mbInit(nullptr);

    mbWebView mbView = mbCreateWebWindow(MB_WINDOW_TYPE_POPUP, NULL, 100, 50, 1024, 768); // 创建一个普通窗口
    //mbWebView mbView = ::mbCreateWebCustomWindow(NULL, WS_POPUP, 0, 100, 50, 600, 700); // 创建了一个无标题栏无边框的窗口

    ::mbSetWindowTitle(mbView, "mb132_x64");
    ::mbOnLoadUrlBegin(mbView, handleLoadUrlBegin, nullptr);
    ::mbOnCreateView(mbView, handleCreateView, nullptr);
    ::mbOnJsQuery(mbView, onJsQueryCallback, nullptr);
    ::mbSetDebugConfig(mbView, "ncHittestPaddingWidth", "2"); // 设置边框边缘多长为可拉伸
    ::mbOnClose(mbView, onCloseCallback, NULL);
    ::mbOnDocumentReady(mbView, onDocumentReadyCallback, NULL);

    //const char* url ="https://www.bilibili.com/";
    const char* url = "https://www.iqiyi.com/";
    ::mbLoadURL(mbView, url); // 通过url加载网页
    //::mbLoadHtmlWithBaseUrl(mbView, kHtmlTestStr, "file:///test.html"); // 通过字符串加载html
    ::mbShowWindow(mbView, TRUE);

    //void* gtkWin = mbGetPlatformWindowHandle(mbView); // 获取gtk的GtkWidget*，但不一定能成功，需要等待窗口创建完毕才行
    ::mbRunMessageLoop(); // 需要退出可调用mbExitMessageLoop
    return 0;
}



