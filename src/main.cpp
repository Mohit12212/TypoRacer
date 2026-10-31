/*
 * TypoRacer Pro - Advanced Computer Aptitude & Keystroke Dynamics Suite
 * Pure Native C++ / Win32 GDI Desktop Application
 */

#define UNICODE
#define _UNICODE
#define _WIN32_WINNT 0x0600

#include <windows.h>
#include <commctrl.h>
#include <shellapi.h>
#include <string>
#include <sstream>
#include <iomanip>
#include <vector>
#include <algorithm>
#include "../resources/resource.h"
#include "TypingTest.h"
#include "SoundEngine.h"

#pragma comment(lib, "gdi32.lib")
#pragma comment(lib, "comctl32.lib")
#pragma comment(lib, "user32.lib")
#pragma comment(lib, "shell32.lib")
#pragma comment(lib, "winmm.lib")

// ── Visual Palette ────────────────────────────────────────────────────────────
static const COLORREF CLR_BG          = RGB(13,  17,  28);  // deep obsidian navy
static const COLORREF CLR_PANEL       = RGB(22,  29,  48);  // card background
static const COLORREF CLR_CARD_ACTIVE = RGB(30,  42,  72);  // selected card background
static const COLORREF CLR_EDIT_BG     = RGB(25,  34,  58);  // input edit background
static const COLORREF CLR_ACCENT      = RGB(0,  210, 255);  // neon cyan
static const COLORREF CLR_CORRECT     = RGB(0,  230, 118);  // vibrant mint green
static const COLORREF CLR_WRONG       = RGB(255, 75,  92);  // bright coral red
static const COLORREF CLR_AMBER       = RGB(255, 183, 77);  // warning amber
static const COLORREF CLR_PURPLE      = RGB(178, 102, 255); // neon purple
static const COLORREF CLR_TEXT        = RGB(240, 246, 255); // bright text
static const COLORREF CLR_MUTED       = RGB(138, 155, 185); // secondary labels
static const COLORREF CLR_BORDER      = RGB(42,  54,  84);  // subtle card border
static const COLORREF CLR_BTN_HOVER   = RGB(20, 230, 255);  // button hover

// ── Application Globals ────────────────────────────────────────────────────────
static HINSTANCE  g_hInstance = NULL;
static HWND       g_hWnd      = NULL;
static TypingTest g_test;

// State Flags
static int  g_hoveredBtnId = 0;
static bool g_capsLockOn   = false;
static bool g_showKeyboard = true;

// GDI Fonts
static HFONT g_hFontHero   = NULL;
static HFONT g_hFontTitle  = NULL;
static HFONT g_hFontCard   = NULL;
static HFONT g_hFontMono   = NULL;
static HFONT g_hFontNormal = NULL;
static HFONT g_hFontBold   = NULL;
static HFONT g_hFontSmall  = NULL;
static HFONT g_hFontGrade  = NULL;

// GDI Brushes
static HBRUSH g_hBrushBG      = NULL;
static HBRUSH g_hBrushPanel   = NULL;
static HBRUSH g_hBrushCardSel = NULL;
static HBRUSH g_hBrushEdit    = NULL;
static HBRUSH g_hBrushAccent  = NULL;

// ── Control Handles: Navigation Toolbar ────────────────────────────────────────
static HWND hNav_Title, hNav_Subtitle;
static HWND hNav_BtnSound, hNav_BtnHistory, hNav_BtnKeyboard;

// ── Control Handles: Home Screen ───────────────────────────────────────────────
static HWND hHome_TabProse, hHome_TabCode, hHome_TabNumbers, hHome_TabSurvival;
static HWND hHome_DiffLabel;
static HWND hHome_CardEasy, hHome_CardMed, hHome_CardHard;
static HWND hHome_BotLabel, hHome_BtnBotOff, hHome_BtnBot35, hHome_BtnBot60, hHome_BtnBot85;
static HWND hHome_BtnStart, hHome_BtnCustom;
static HWND hHome_ScoreBox, hHome_Footer;

// ── Control Handles: Test Screen ───────────────────────────────────────────────
static HWND hTest_RaceTrack;
static HWND hTest_Canvas;
static HWND hTest_Edit;
static HWND hTest_Tip, hTest_CapsWarn;
static HWND hTest_WpmVal,  hTest_WpmLbl;
static HWND hTest_CpmVal,  hTest_CpmLbl;
static HWND hTest_AccVal,  hTest_AccLbl;
static HWND hTest_ConsVal, hTest_ConsLbl;
static HWND hTest_TimeVal, hTest_TimeLbl;
static HWND hTest_BackVal, hTest_BackLbl;
static HWND hTest_Progress;
static HWND hTest_Keyboard;
static HWND hTest_BtnPause, hTest_BtnStop;

// ── Control Handles: Results Screen ────────────────────────────────────────────
static HWND hRes_Panel, hRes_Title, hRes_GradeBadge;
static HWND hRes_WpmVal,  hRes_WpmLbl;
static HWND hRes_CpmVal,  hRes_CpmLbl;
static HWND hRes_AccVal,  hRes_AccLbl;
static HWND hRes_ConsVal, hRes_ConsLbl;
static HWND hRes_WordVal, hRes_WordLbl;
static HWND hRes_ErrVal,  hRes_ErrLbl;
static HWND hRes_TimeVal, hRes_TimeLbl;
static HWND hRes_BestMsg;
static HWND hRes_GraphPanel;
static HWND hRes_Diagnostics;
static HWND hRes_BtnTryAgain, hRes_BtnCert, hRes_BtnHistory, hRes_BtnMenu;

// Subclass for Edit Input Control
static WNDPROC g_OldEditProc = NULL;
static WNDPROC g_OldButtonProc = NULL;

// ── Forward Declarations ───────────────────────────────────────────────────────
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
static void InitThemeGDI();
static void CleanThemeGDI();
static void BuildHomeUI(HWND);
static void BuildTestUI(HWND);
static void BuildResultsUI(HWND);
static void ShowScreen(AppScreen);
static void RefreshTestUI();
static void RefreshResultsUI();
static void DrawRoundedCard(HDC, const RECT&, int, COLORREF, COLORREF, int = 1);
static void DrawOwnerButton(DRAWITEMSTRUCT*);
static void DrawDifficultyCard(HWND, HDC, Difficulty, const wchar_t*, const wchar_t*, const wchar_t*, COLORREF);
static void DrawRaceTrack(HWND, HDC);
static void DrawHighlightedCanvas(HWND, HDC);
static void DrawProgressBar(HWND, HDC);
static void DrawVirtualKeyboard(HWND, HDC);
static void DrawResultsGraph(HWND, HDC);
static void DrawGradeCircle(HWND, HDC);
static void OpenCustomTextDialog(HWND);
static void OpenHistoryDialog(HWND);
static void PromptAndExportCertificate(HWND);

// ── Helper Drawing Routines ────────────────────────────────────────────────────
static void DrawRoundedCard(HDC hdc, const RECT& rc, int radius, COLORREF fillClr, COLORREF borderClr, int borderWidth) {
    HPEN pen = CreatePen(PS_SOLID, borderWidth, borderClr);
    HBRUSH brush = CreateSolidBrush(fillClr);
    HPEN oldPen = (HPEN)SelectObject(hdc, pen);
    HBRUSH oldBrush = (HBRUSH)SelectObject(hdc, brush);

    RoundRect(hdc, rc.left, rc.top, rc.right, rc.bottom, radius, radius);

    SelectObject(hdc, oldPen);
    SelectObject(hdc, oldBrush);
    DeleteObject(pen);
    DeleteObject(brush);
}

// ── Edit Subclass: Capture Every Keystroke & Tactile Sound ─────────────────────
LRESULT CALLBACK EditSubclassProc(HWND hEdit, UINT msg, WPARAM wp, LPARAM lp) {
    // Prevent clipboard and drag/drop shortcuts from bypassing the typing test.
    if (msg == WM_PASTE || msg == WM_DROPFILES || msg == WM_CUT) return 0;

    if (msg == WM_KEYDOWN) {
        if (wp == VK_BACK) {
            g_test.RecordBackspace();
            SoundEngine::Instance().Play(SoundFX::Backspace);
        } else if (wp == VK_SPACE) {
            SoundEngine::Instance().Play(SoundFX::Spacebar);
        } else if (wp >= 0x30 && wp <= 0x5A) {
            SoundEngine::Instance().Play(SoundFX::KeyClick);
        }
    }

    if (msg == WM_KEYDOWN || msg == WM_KEYUP || msg == WM_CHAR) {
        // Detect Caps Lock state
        bool caps = (GetKeyState(VK_CAPITAL) & 0x0001) != 0;
        if (caps != g_capsLockOn) {
            g_capsLockOn = caps;
            ShowWindow(hTest_CapsWarn, g_capsLockOn ? SW_SHOW : SW_HIDE);
        }

        LRESULT res = CallWindowProc(g_OldEditProc, hEdit, msg, wp, lp);

        const bool textChanged = (msg == WM_CHAR) ||
                                 (msg == WM_KEYDOWN && wp == VK_DELETE);
        if (textChanged && g_test.IsRunning() && !g_test.IsPaused()) {
            int len = GetWindowTextLength(hEdit);
            std::vector<wchar_t> buf(len + 2, 0);
            GetWindowText(hEdit, buf.data(), len + 1);
            g_test.UpdateInput(buf.data());

            // Repaint text canvas, keyboard, track, and live HUD
            InvalidateRect(hTest_Canvas, NULL, FALSE);
            InvalidateRect(hTest_Progress, NULL, FALSE);
            InvalidateRect(hTest_RaceTrack, NULL, FALSE);
            if (g_showKeyboard) InvalidateRect(hTest_Keyboard, NULL, FALSE);

            RefreshTestUI();

            if (g_test.IsFinished()) {
                SoundEngine::Instance().Play(SoundFX::Bell);
                PostMessage(GetParent(hEdit), WM_COMMAND, MAKEWPARAM(IDC_BTN_STOP, BN_CLICKED), 0);
            }
        }
        return res;
    }
    return CallWindowProc(g_OldEditProc, hEdit, msg, wp, lp);
}

// ── Button Hover & Selection Window Procedure ──────────────────────────────────
LRESULT CALLBACK CustomButtonProc(HWND hBtn, UINT msg, WPARAM wp, LPARAM lp) {
    int id = GetDlgCtrlID(hBtn);

    switch (msg) {
        case WM_SETFOCUS:
        case WM_KILLFOCUS:
            return 0; // Strip standard Windows dotted focus outline
        case WM_MOUSEMOVE: {
            if (g_hoveredBtnId != id) {
                g_hoveredBtnId = id;
                InvalidateRect(hBtn, NULL, TRUE);

                TRACKMOUSEEVENT tme;
                tme.cbSize = sizeof(tme);
                tme.dwFlags = TME_LEAVE;
                tme.hwndTrack = hBtn;
                TrackMouseEvent(&tme);
            }
            break;
        }
        case WM_MOUSELEAVE: {
            if (g_hoveredBtnId == id) {
                g_hoveredBtnId = 0;
                InvalidateRect(hBtn, NULL, TRUE);
            }
            break;
        }
    }
    return CallWindowProc(g_OldButtonProc, hBtn, msg, wp, lp);
}

// ── WinMain Entry Point ────────────────────────────────────────────────────────
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR, int nCmdShow) {
    g_hInstance = hInstance;

    INITCOMMONCONTROLSEX icc = { sizeof(icc), ICC_STANDARD_CLASSES | ICC_PROGRESS_CLASS };
    InitCommonControlsEx(&icc);

    WNDCLASSEX wc    = {};
    wc.cbSize        = sizeof(wc);
    wc.style         = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc   = WndProc;
    wc.hInstance     = hInstance;
    wc.hCursor       = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = NULL;
    wc.lpszClassName = L"TypoRacerMasterWnd";
    wc.hIcon         = LoadIcon(NULL, IDI_APPLICATION);
    wc.hIconSm       = LoadIcon(NULL, IDI_APPLICATION);
    RegisterClassEx(&wc);

    // Spacious modern 1040 x 740 window
    HWND hWnd = CreateWindowEx(
        0, L"TypoRacerMasterWnd",
        L"TypoRacer Pro - Advanced Computer Aptitude & Keystroke Dynamics Suite",
        WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX,
        CW_USEDEFAULT, CW_USEDEFAULT, 1040, 740,
        NULL, NULL, hInstance, NULL);

    if (!hWnd) return 0;

    // Center window
    RECT rc;
    GetWindowRect(hWnd, &rc);
    int sw = GetSystemMetrics(SM_CXSCREEN);
    int sh = GetSystemMetrics(SM_CYSCREEN);
    SetWindowPos(hWnd, NULL,
        (sw - (rc.right - rc.left)) / 2,
        (sh - (rc.bottom - rc.top)) / 2,
        0, 0, SWP_NOSIZE | SWP_NOZORDER);

    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);

    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return (int)msg.wParam;
}

// ── Initialize GDI Fonts & Brushes ─────────────────────────────────────────────
static void InitThemeGDI() {
    g_hFontHero   = CreateFont(36, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE,
                               DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
                               CLEARTYPE_QUALITY, DEFAULT_PITCH, L"Segoe UI");

    g_hFontTitle  = CreateFont(24, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE,
                               DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
                               CLEARTYPE_QUALITY, DEFAULT_PITCH, L"Segoe UI");

    g_hFontCard   = CreateFont(18, 0, 0, 0, FW_SEMIBOLD, FALSE, FALSE, FALSE,
                               DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
                               CLEARTYPE_QUALITY, DEFAULT_PITCH, L"Segoe UI");

    g_hFontBold   = CreateFont(15, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE,
                               DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
                               CLEARTYPE_QUALITY, DEFAULT_PITCH, L"Segoe UI");

    g_hFontNormal = CreateFont(14, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE,
                               DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
                               CLEARTYPE_QUALITY, DEFAULT_PITCH, L"Segoe UI");

    g_hFontSmall  = CreateFont(12, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE,
                               DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
                               CLEARTYPE_QUALITY, DEFAULT_PITCH, L"Segoe UI");

    g_hFontMono   = CreateFont(18, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE,
                               DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
                               CLEARTYPE_QUALITY, FIXED_PITCH, L"Consolas");

    g_hFontGrade  = CreateFont(52, 0, 0, 0, FW_EXTRABOLD, FALSE, FALSE, FALSE,
                               DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
                               CLEARTYPE_QUALITY, DEFAULT_PITCH, L"Segoe UI");

    g_hBrushBG      = CreateSolidBrush(CLR_BG);
    g_hBrushPanel   = CreateSolidBrush(CLR_PANEL);
    g_hBrushCardSel = CreateSolidBrush(CLR_CARD_ACTIVE);
    g_hBrushEdit    = CreateSolidBrush(CLR_EDIT_BG);
    g_hBrushAccent  = CreateSolidBrush(CLR_ACCENT);
}

static void CleanThemeGDI() {
    DeleteObject(g_hFontHero);
    DeleteObject(g_hFontTitle);
    DeleteObject(g_hFontCard);
    DeleteObject(g_hFontBold);
    DeleteObject(g_hFontNormal);
    DeleteObject(g_hFontSmall);
    DeleteObject(g_hFontMono);
    DeleteObject(g_hFontGrade);

    DeleteObject(g_hBrushBG);
    DeleteObject(g_hBrushPanel);
    DeleteObject(g_hBrushCardSel);
    DeleteObject(g_hBrushEdit);
    DeleteObject(g_hBrushAccent);
}

// ── Control Builder Helpers ────────────────────────────────────────────────────
static HWND CreateStaticLabel(HWND parent, const wchar_t* text, int x, int y, int w, int h, HFONT font, int id = 0, DWORD style = SS_LEFT) {
    HWND hCtl = CreateWindowEx(0, L"STATIC", text,
                               WS_CHILD | WS_VISIBLE | style,
                               x, y, w, h, parent, (HMENU)(INT_PTR)id, g_hInstance, NULL);
    SendMessage(hCtl, WM_SETFONT, (WPARAM)font, TRUE);
    return hCtl;
}

static HWND CreateOwnerBtn(HWND parent, const wchar_t* text, int x, int y, int w, int h, int id) {
    HWND hBtn = CreateWindowEx(0, L"BUTTON", text,
                               WS_CHILD | WS_VISIBLE | BS_OWNERDRAW,
                               x, y, w, h, parent, (HMENU)(INT_PTR)id, g_hInstance, NULL);
    SendMessage(hBtn, WM_SETFONT, (WPARAM)g_hFontBold, TRUE);

    if (!g_OldButtonProc) {
        g_OldButtonProc = (WNDPROC)GetWindowLongPtr(hBtn, GWLP_WNDPROC);
    }
    SetWindowLongPtr(hBtn, GWLP_WNDPROC, (LONG_PTR)CustomButtonProc);
    return hBtn;
}

// ── Build Home Screen ──────────────────────────────────────────────────────────
static void BuildHomeUI(HWND hWnd) {
    // Top Navigation & Sound / History / Keyboard Toolbar
    hNav_Title    = CreateStaticLabel(hWnd, L"⚡ TypoRacer Pro", 40, 18, 420, 38, g_hFontHero, IDC_STATIC_TITLE, SS_LEFT);
    hNav_Subtitle = CreateStaticLabel(hWnd, L"Computer Aptitude, Keystroke Dynamics & Touch Typing Suite", 40, 56, 520, 20, g_hFontNormal, IDC_STATIC_SUBTITLE, SS_LEFT);

    hNav_BtnSound    = CreateOwnerBtn(hWnd, L"🔊 SOUND: ON", 630, 22, 115, 34, IDC_BTN_SOUND_TOGGLE);
    hNav_BtnHistory  = CreateOwnerBtn(hWnd, L"📊 HISTORY", 755, 22, 115, 34, IDC_BTN_VIEW_HISTORY);
    hNav_BtnKeyboard = CreateOwnerBtn(hWnd, L"⌨ KEYBOARD", 880, 22, 115, 34, IDC_BTN_KB_TOGGLE);

    // Category Tabs (4 Mode Switchers)
    int tabW = 225;
    int tabH = 38;
    int tabY = 95;
    int tabStartX = 40;
    int tabGap = 16;

    hHome_TabProse    = CreateOwnerBtn(hWnd, L"📄 GENERAL PROSE", tabStartX, tabY, tabW, tabH, IDC_TAB_PROSE);
    hHome_TabCode     = CreateOwnerBtn(hWnd, L"💻 CODE APTITUDE", tabStartX + (tabW + tabGap), tabY, tabW, tabH, IDC_TAB_CODE);
    hHome_TabNumbers  = CreateOwnerBtn(hWnd, L"🔢 NUMBERS & SYMBOLS", tabStartX + (tabW + tabGap) * 2, tabY, tabW, tabH, IDC_TAB_NUMBERS);
    hHome_TabSurvival = CreateOwnerBtn(hWnd, L"⚡ SURVIVAL SPRINT", tabStartX + (tabW + tabGap) * 3, tabY, tabW, tabH, IDC_TAB_SURVIVAL);

    // Difficulty Subtitle
    hHome_DiffLabel = CreateStaticLabel(hWnd, L"SELECT DIFFICULTY TIER", 40, 150, 950, 20, g_hFontSmall, IDC_STATIC_DIFF_LABEL, SS_LEFT);

    // Interactive Owner-Drawn Difficulty Cards
    int cardW = 300;
    int cardH = 130;
    int startX = 40;
    int gap = 25;
    int cardY = 175;

    hHome_CardEasy = CreateWindowEx(0, L"STATIC", NULL,
                                    WS_CHILD | WS_VISIBLE | SS_OWNERDRAW | SS_NOTIFY,
                                    startX, cardY, cardW, cardH,
                                    hWnd, (HMENU)IDC_CARD_EASY, g_hInstance, NULL);

    hHome_CardMed  = CreateWindowEx(0, L"STATIC", NULL,
                                    WS_CHILD | WS_VISIBLE | SS_OWNERDRAW | SS_NOTIFY,
                                    startX + cardW + gap, cardY, cardW, cardH,
                                    hWnd, (HMENU)IDC_CARD_MEDIUM, g_hInstance, NULL);

    hHome_CardHard = CreateWindowEx(0, L"STATIC", NULL,
                                    WS_CHILD | WS_VISIBLE | SS_OWNERDRAW | SS_NOTIFY,
                                    startX + (cardW + gap) * 2, cardY, cardW, cardH,
                                    hWnd, (HMENU)IDC_CARD_HARD, g_hInstance, NULL);

    // AI Ghost Bot Selector Row
    hHome_BotLabel   = CreateStaticLabel(hWnd, L"AI GHOST RACER OPPONENT:", 40, 325, 230, 28, g_hFontBold, IDC_STATIC_BOT_LABEL, SS_LEFT);
    hHome_BtnBotOff  = CreateOwnerBtn(hWnd, L"OFF", 280, 320, 90, 34, IDC_BTN_BOT_OFF);
    hHome_BtnBot35   = CreateOwnerBtn(hWnd, L"35 WPM", 380, 320, 100, 34, IDC_BTN_BOT_EASY);
    hHome_BtnBot60   = CreateOwnerBtn(hWnd, L"60 WPM", 490, 320, 100, 34, IDC_BTN_BOT_MED);
    hHome_BtnBot85   = CreateOwnerBtn(hWnd, L"85 WPM", 600, 320, 100, 34, IDC_BTN_BOT_HARD);

    // High Score Badge Box
    hHome_ScoreBox = CreateStaticLabel(hWnd, L"★ Record: 0.0 WPM", 40, 380, 950, 28, g_hFontCard, IDC_STATIC_SCORE_VAL, SS_CENTER);

    // Primary Action Buttons
    hHome_BtnStart  = CreateOwnerBtn(hWnd, L"▶  START TEST", 350, 430, 330, 56, IDC_BTN_START);
    hHome_BtnCustom = CreateOwnerBtn(hWnd, L"✎  CUSTOM PRACTICE TEXT", 390, 500, 250, 38, IDC_BTN_CUSTOM);

    hHome_Footer = CreateStaticLabel(hWnd, L"Computer Aptitude Metrics: Net WPM, CPM, Accuracy, Keystroke Consistency & Problem Keys Diagnostics", 0, 650, 1040, 20, g_hFontSmall, IDC_STATIC_FOOTER, SS_CENTER);
}

// ── Build Test Screen ──────────────────────────────────────────────────────────
static void BuildTestUI(HWND hWnd) {
    // Live Dual-Lane Race Track (Owner-Drawn)
    hTest_RaceTrack = CreateWindowEx(
        0, L"STATIC", NULL,
        WS_CHILD | SS_OWNERDRAW,
        40, 75, 950, 60, hWnd, (HMENU)IDC_STATIC_RACETRACK, g_hInstance, NULL);

    // Canvas displaying text to type with live syntax highlighting (Flat Modern Surface)
    hTest_Canvas = CreateWindowEx(
        0, L"STATIC", NULL,
        WS_CHILD | SS_OWNERDRAW,
        40, 145, 950, 130, hWnd, (HMENU)IDC_STATIC_CANVAS, g_hInstance, NULL);

    // Caps Lock Warning Indicator
    hTest_CapsWarn = CreateStaticLabel(hWnd, L"⚠ WARNING: CAPS LOCK IS ON!", 40, 280, 350, 18, g_hFontBold, IDC_STATIC_CAPS_WARN, SS_LEFT);
    ShowWindow(hTest_CapsWarn, SW_HIDE);

    // Tip
    hTest_Tip = CreateStaticLabel(hWnd, L"Type the sample text accurately. The timer runs until completed.", 400, 280, 590, 18, g_hFontNormal, IDC_STATIC_TIP, SS_RIGHT);

    // Real-time Text Input Field (Flat Modern Surface)
    hTest_Edit = CreateWindowEx(
        0, L"EDIT", NULL,
        WS_CHILD | WS_VISIBLE | ES_MULTILINE | ES_AUTOVSCROLL | WS_VSCROLL,
        40, 302, 950, 75, hWnd, (HMENU)IDC_EDIT_INPUT, g_hInstance, NULL);
    SendMessage(hTest_Edit, WM_SETFONT, (WPARAM)g_hFontMono, TRUE);

    // Subclass Edit Control
    g_OldEditProc = (WNDPROC)SetWindowLongPtr(hTest_Edit, GWLP_WNDPROC, (LONG_PTR)EditSubclassProc);

    // Dynamic Progress Bar (Owner-drawn)
    hTest_Progress = CreateWindowEx(
        0, L"STATIC", NULL,
        WS_CHILD | SS_OWNERDRAW,
        40, 384, 950, 14, hWnd, (HMENU)IDC_STATIC_PROGRESS_BG, g_hInstance, NULL);

    // Live Metrics HUD Cards (6 statistics side-by-side)
    int hudW = 145;
    int hudH = 68;
    int hudStartX = 40;
    int hudGap = 16;
    int hudY = 406;

    // Card 1: WPM
    hTest_WpmLbl  = CreateStaticLabel(hWnd, L"SPEED (WPM)", hudStartX, hudY + 8, hudW, 14, g_hFontSmall, IDC_STATIC_WPM_LABEL, SS_CENTER);
    hTest_WpmVal  = CreateStaticLabel(hWnd, L"0.0", hudStartX, hudY + 24, hudW, 34, g_hFontTitle, IDC_STATIC_WPM_VAL, SS_CENTER);

    // Card 2: CPM
    hTest_CpmLbl  = CreateStaticLabel(hWnd, L"SPEED (CPM)", hudStartX + (hudW + hudGap), hudY + 8, hudW, 14, g_hFontSmall, IDC_STATIC_CPM_LABEL, SS_CENTER);
    hTest_CpmVal  = CreateStaticLabel(hWnd, L"0", hudStartX + (hudW + hudGap), hudY + 24, hudW, 34, g_hFontTitle, IDC_STATIC_CPM_VAL, SS_CENTER);

    // Card 3: ACCURACY
    hTest_AccLbl  = CreateStaticLabel(hWnd, L"ACCURACY", hudStartX + (hudW + hudGap) * 2, hudY + 8, hudW, 14, g_hFontSmall, IDC_STATIC_ACC_LABEL, SS_CENTER);
    hTest_AccVal  = CreateStaticLabel(hWnd, L"100%", hudStartX + (hudW + hudGap) * 2, hudY + 24, hudW, 34, g_hFontTitle, IDC_STATIC_ACC_VAL, SS_CENTER);

    // Card 4: CONSISTENCY
    hTest_ConsLbl = CreateStaticLabel(hWnd, L"CONSISTENCY", hudStartX + (hudW + hudGap) * 3, hudY + 8, hudW, 14, g_hFontSmall, IDC_STATIC_CONSISTENCY_LBL, SS_CENTER);
    hTest_ConsVal = CreateStaticLabel(hWnd, L"100%", hudStartX + (hudW + hudGap) * 3, hudY + 24, hudW, 34, g_hFontTitle, IDC_STATIC_CONSISTENCY_VAL, SS_CENTER);

    // Card 5: TIME LEFT
    hTest_TimeLbl = CreateStaticLabel(hWnd, L"TIME REMAINING", hudStartX + (hudW + hudGap) * 4, hudY + 8, hudW, 14, g_hFontSmall, IDC_STATIC_TIME_LABEL, SS_CENTER);
    hTest_TimeVal = CreateStaticLabel(hWnd, L"45s", hudStartX + (hudW + hudGap) * 4, hudY + 24, hudW, 34, g_hFontTitle, IDC_STATIC_TIME_VAL, SS_CENTER);

    // Card 6: CORRECTIONS
    hTest_BackLbl = CreateStaticLabel(hWnd, L"CORRECTIONS", hudStartX + (hudW + hudGap) * 5, hudY + 8, hudW, 14, g_hFontSmall, IDC_STATIC_BACKSPACE_LABEL, SS_CENTER);
    hTest_BackVal = CreateStaticLabel(hWnd, L"0", hudStartX + (hudW + hudGap) * 5, hudY + 24, hudW, 34, g_hFontTitle, IDC_STATIC_BACKSPACE_VAL, SS_CENTER);

    // Interactive Virtual Touch-Typing Keyboard Guide
    hTest_Keyboard = CreateWindowEx(
        0, L"STATIC", NULL,
        WS_CHILD | SS_OWNERDRAW,
        40, 485, 950, 160, hWnd, (HMENU)IDC_STATIC_KEYBOARD, g_hInstance, NULL);

    // Controls Row
    hTest_BtnPause = CreateOwnerBtn(hWnd, L"⏸  PAUSE", 330, 655, 170, 42, IDC_BTN_PAUSE);
    hTest_BtnStop  = CreateOwnerBtn(hWnd, L"⏹  STOP & FINISH", 520, 655, 190, 42, IDC_BTN_STOP);
}

// ── Build Results Screen ───────────────────────────────────────────────────────
static void BuildResultsUI(HWND hWnd) {
    hRes_Panel = CreateWindowEx(
        0, L"STATIC", NULL,
        WS_CHILD | SS_OWNERDRAW,
        40, 75, 950, 565, hWnd, (HMENU)IDC_STATIC_RES_PANEL, g_hInstance, NULL);

    hRes_Title = CreateStaticLabel(hWnd, L"COMPUTER APTITUDE EVALUATION REPORT", 40, 88, 950, 32, g_hFontTitle, IDC_STATIC_RES_TITLE, SS_CENTER);

    // Grade Circular Badge (Owner-drawn)
    hRes_GradeBadge = CreateWindowEx(
        0, L"STATIC", NULL,
        WS_CHILD | SS_OWNERDRAW,
        65, 130, 120, 120, hWnd, (HMENU)IDC_STATIC_RES_GRADE, g_hInstance, NULL);

    // Performance Metrics Grid (Left Column)
    int startY = 130;
    int gapY = 52;
    int col1X = 205;
    int col1W = 220;

    hRes_WpmLbl  = CreateStaticLabel(hWnd, L"NET SPEED (WPM)", col1X, startY, col1W, 15, g_hFontSmall);
    hRes_WpmVal  = CreateStaticLabel(hWnd, L"0.0 WPM", col1X, startY + 16, col1W, 30, g_hFontTitle, IDC_STATIC_RES_WPM);

    hRes_CpmLbl  = CreateStaticLabel(hWnd, L"RAW CHARACTERS (CPM)", col1X, startY + gapY, col1W, 15, g_hFontSmall);
    hRes_CpmVal  = CreateStaticLabel(hWnd, L"0 CPM", col1X, startY + gapY + 16, col1W, 30, g_hFontTitle, IDC_STATIC_RES_CPM);

    hRes_AccLbl  = CreateStaticLabel(hWnd, L"ACCURACY PERCENTAGE", col1X, startY + gapY * 2, col1W, 15, g_hFontSmall);
    hRes_AccVal  = CreateStaticLabel(hWnd, L"100.0%", col1X, startY + gapY * 2 + 16, col1W, 30, g_hFontTitle, IDC_STATIC_RES_ACC);

    hRes_ConsLbl = CreateStaticLabel(hWnd, L"KEYSTROKE CONSISTENCY", col1X, startY + gapY * 3, col1W, 15, g_hFontSmall);
    hRes_ConsVal = CreateStaticLabel(hWnd, L"100.0%", col1X, startY + gapY * 3 + 16, col1W, 30, g_hFontTitle, IDC_STATIC_RES_CONSISTENCY);

    // Interactive Real-Time WPM Timeline Graph (Owner-drawn Canvas)
    hRes_GraphPanel = CreateWindowEx(
        0, L"STATIC", NULL,
        WS_CHILD | SS_OWNERDRAW,
        450, 130, 520, 220, hWnd, (HMENU)IDC_STATIC_RES_GRAPH, g_hInstance, NULL);

    // Additional Stats Row (Words, Errors, Duration)
    int statRowY = 360;
    hRes_WordLbl = CreateStaticLabel(hWnd, L"WORDS (CORRECT / WRONG)", 65, statRowY, 260, 16, g_hFontSmall);
    hRes_WordVal = CreateStaticLabel(hWnd, L"0 / 0", 65, statRowY + 18, 260, 26, g_hFontCard, IDC_STATIC_RES_CORRECT);

    hRes_ErrLbl  = CreateStaticLabel(hWnd, L"CORRECTIONS (BACKSPACES)", 350, statRowY, 260, 16, g_hFontSmall);
    hRes_ErrVal  = CreateStaticLabel(hWnd, L"0", 350, statRowY + 18, 260, 26, g_hFontCard, IDC_STATIC_RES_BACKSPACE);

    hRes_TimeLbl = CreateStaticLabel(hWnd, L"TOTAL TEST DURATION", 650, statRowY, 260, 16, g_hFontSmall);
    hRes_TimeVal = CreateStaticLabel(hWnd, L"0 seconds", 650, statRowY + 18, 260, 26, g_hFontCard, IDC_STATIC_RES_TIME);

    // Diagnostics Box (Weakest keys & Aptitude classification)
    hRes_Diagnostics = CreateStaticLabel(hWnd, L"", 65, 415, 900, 48, g_hFontBold, IDC_STATIC_RES_KEYS, SS_LEFT);

    // Best Record Banner
    hRes_BestMsg = CreateStaticLabel(hWnd, L"", 65, 470, 900, 26, g_hFontBold, IDC_STATIC_RES_BEST, SS_CENTER);

    // Action Buttons
    int btnW = 210;
    int btnH = 46;
    int btnGap = 18;
    int btnStartX = 65;
    int btnY = 515;

    hRes_BtnTryAgain = CreateOwnerBtn(hWnd, L"↺  TRY AGAIN", btnStartX, btnY, btnW, btnH, IDC_BTN_TRYAGAIN);
    hRes_BtnCert     = CreateOwnerBtn(hWnd, L"📜  EXPORT CERTIFICATE", btnStartX + (btnW + btnGap), btnY, btnW, btnH, IDC_BTN_EXPORT_CERT);
    hRes_BtnHistory  = CreateOwnerBtn(hWnd, L"📊  SESSION LOGS", btnStartX + (btnW + btnGap) * 2, btnY, btnW, btnH, IDC_BTN_VIEW_HISTORY);
    hRes_BtnMenu     = CreateOwnerBtn(hWnd, L"⚙  MAIN MENU", btnStartX + (btnW + btnGap) * 3, btnY, btnW, btnH, IDC_BTN_CHANGEDIFF);
}

// ── Screen Visibility Controller ───────────────────────────────────────────────
static void ShowScreen(AppScreen screen) {
    auto SetVis = [](HWND hw, bool show) {
        if (hw) ShowWindow(hw, show ? SW_SHOW : SW_HIDE);
    };

    bool isHome    = (screen == AppScreen::Home);
    bool isTest    = (screen == AppScreen::Test);
    bool isResults = (screen == AppScreen::Results);

    // Navigation & Toolbar
    SetVis(hNav_Title, true);
    SetVis(hNav_Subtitle, true);
    SetVis(hNav_BtnSound, true);
    SetVis(hNav_BtnHistory, true);
    SetVis(hNav_BtnKeyboard, true);

    // Home Screen Controls
    SetVis(hHome_TabProse, isHome);
    SetVis(hHome_TabCode, isHome);
    SetVis(hHome_TabNumbers, isHome);
    SetVis(hHome_TabSurvival, isHome);
    SetVis(hHome_DiffLabel, isHome);
    SetVis(hHome_CardEasy, isHome);
    SetVis(hHome_CardMed, isHome);
    SetVis(hHome_CardHard, isHome);
    SetVis(hHome_BotLabel, isHome);
    SetVis(hHome_BtnBotOff, isHome);
    SetVis(hHome_BtnBot35, isHome);
    SetVis(hHome_BtnBot60, isHome);
    SetVis(hHome_BtnBot85, isHome);
    SetVis(hHome_BtnStart, isHome);
    SetVis(hHome_BtnCustom, isHome);
    SetVis(hHome_ScoreBox, isHome);
    SetVis(hHome_Footer, isHome);

    // Test Screen Controls
    SetVis(hTest_RaceTrack, isTest);
    SetVis(hTest_Canvas, isTest);
    SetVis(hTest_Tip, isTest);
    SetVis(hTest_CapsWarn, isTest && g_capsLockOn);
    SetVis(hTest_Edit, isTest);
    SetVis(hTest_Progress, isTest);
    SetVis(hTest_WpmLbl, isTest);   SetVis(hTest_WpmVal, isTest);
    SetVis(hTest_CpmLbl, isTest);   SetVis(hTest_CpmVal, isTest);
    SetVis(hTest_AccLbl, isTest);   SetVis(hTest_AccVal, isTest);
    SetVis(hTest_ConsLbl, isTest);  SetVis(hTest_ConsVal, isTest);
    SetVis(hTest_TimeLbl, isTest);  SetVis(hTest_TimeVal, isTest);
    SetVis(hTest_BackLbl, isTest);  SetVis(hTest_BackVal, isTest);
    SetVis(hTest_Keyboard, isTest && g_showKeyboard);
    SetVis(hTest_BtnPause, isTest);
    SetVis(hTest_BtnStop, isTest);

    // Results Screen Controls
    SetVis(hRes_Panel, isResults);
    SetVis(hRes_Title, isResults);
    SetVis(hRes_GradeBadge, isResults);
    SetVis(hRes_WpmLbl, isResults);   SetVis(hRes_WpmVal, isResults);
    SetVis(hRes_CpmLbl, isResults);   SetVis(hRes_CpmVal, isResults);
    SetVis(hRes_AccLbl, isResults);   SetVis(hRes_AccVal, isResults);
    SetVis(hRes_ConsLbl, isResults);  SetVis(hRes_ConsVal, isResults);
    SetVis(hRes_WordLbl, isResults);  SetVis(hRes_WordVal, isResults);
    SetVis(hRes_ErrLbl, isResults);   SetVis(hRes_ErrVal, isResults);
    SetVis(hRes_TimeLbl, isResults);  SetVis(hRes_TimeVal, isResults);
    SetVis(hRes_BestMsg, isResults);
    SetVis(hRes_GraphPanel, isResults);
    SetVis(hRes_Diagnostics, isResults);
    SetVis(hRes_BtnTryAgain, isResults);
    SetVis(hRes_BtnCert, isResults);
    SetVis(hRes_BtnHistory, isResults);
    SetVis(hRes_BtnMenu, isResults);

    if (isHome) {
        wchar_t buf[128];
        const wchar_t* diffName = L"Medium";
        switch (g_test.GetDifficulty()) {
            case Difficulty::Easy:   diffName = L"Easy"; break;
            case Difficulty::Medium: diffName = L"Medium"; break;
            case Difficulty::Hard:   diffName = L"Hard"; break;
        }
        swprintf(buf, 128, L"★ All-Time Personal Best: %.1f WPM", g_test.GetCurrentHighScore());
        SetWindowText(hHome_ScoreBox, buf);
        InvalidateRect(hHome_CardEasy, NULL, TRUE);
        InvalidateRect(hHome_CardMed, NULL, TRUE);
        InvalidateRect(hHome_CardHard, NULL, TRUE);
        InvalidateRect(hHome_TabProse, NULL, TRUE);
        InvalidateRect(hHome_TabCode, NULL, TRUE);
        InvalidateRect(hHome_TabNumbers, NULL, TRUE);
        InvalidateRect(hHome_TabSurvival, NULL, TRUE);
        InvalidateRect(hHome_BtnBotOff, NULL, TRUE);
        InvalidateRect(hHome_BtnBot35, NULL, TRUE);
        InvalidateRect(hHome_BtnBot60, NULL, TRUE);
        InvalidateRect(hHome_BtnBot85, NULL, TRUE);
    } else if (isTest) {
        SetWindowText(hTest_Edit, L"");
        SetWindowText(hTest_BtnPause, L"⏸  PAUSE");
        SetFocus(hTest_Edit);
        RefreshTestUI();
    } else if (isResults) {
        RefreshResultsUI();
    }

    InvalidateRect(g_hWnd, NULL, TRUE);
}

// ── Refresh Live HUD on Test Screen ───────────────────────────────────────────
static void RefreshTestUI() {
    wchar_t buf[64];

    swprintf(buf, 64, L"%.1f", g_test.GetLiveWPM());
    SetWindowText(hTest_WpmVal, buf);

    swprintf(buf, 64, L"%.0f", g_test.GetLiveCPM());
    SetWindowText(hTest_CpmVal, buf);

    swprintf(buf, 64, L"%.1f%%", g_test.GetLiveAccuracy());
    SetWindowText(hTest_AccVal, buf);

    swprintf(buf, 64, L"%.1f%%", g_test.GetLiveConsistency());
    SetWindowText(hTest_ConsVal, buf);

    swprintf(buf, 64, L"%ds", g_test.GetTimeRemaining());
    SetWindowText(hTest_TimeVal, buf);

    swprintf(buf, 64, L"%d", g_test.GetBackspaces());
    SetWindowText(hTest_BackVal, buf);

    InvalidateRect(hTest_Progress, NULL, FALSE);
    InvalidateRect(hTest_Canvas, NULL, FALSE);
    InvalidateRect(hTest_RaceTrack, NULL, FALSE);
    if (g_showKeyboard) InvalidateRect(hTest_Keyboard, NULL, FALSE);
}

// ── Refresh Results Dashboard ──────────────────────────────────────────────────
static void RefreshResultsUI() {
    const TestResult& res = g_test.GetResult();
    wchar_t buf[256];

    swprintf(buf, 256, L"%.1f WPM", res.wpm);
    SetWindowText(hRes_WpmVal, buf);

    swprintf(buf, 256, L"%.0f CPM", res.cpm);
    SetWindowText(hRes_CpmVal, buf);

    swprintf(buf, 256, L"%.1f%%", res.accuracy);
    SetWindowText(hRes_AccVal, buf);

    swprintf(buf, 256, L"%.1f%%", res.consistency);
    SetWindowText(hRes_ConsVal, buf);

    swprintf(buf, 256, L"%d Correct / %d Wrong", res.correctWords, res.wrongWords);
    SetWindowText(hRes_WordVal, buf);

    swprintf(buf, 256, L"%d corrections", res.backspaces);
    SetWindowText(hRes_ErrVal, buf);

    swprintf(buf, 256, L"%d seconds", res.timeTaken);
    SetWindowText(hRes_TimeVal, buf);

    // Diagnostics String
    std::wstring diag = L"APTITUDE RANK: " + res.aptitudeRank + L"  |  DIAGNOSTICS: ";
    if (res.problemKeys.empty()) {
        diag += L"Zero uncorrected errors! Flawless keystroke execution.";
    } else {
        diag += L"Weakest Keys: ";
        for (size_t i = 0; i < res.problemKeys.size(); ++i) {
            wchar_t k = res.problemKeys[i].first;
            if (k == L' ') diag += L"'[Space]'";
            else if (k == L'\n') diag += L"'[Enter]'";
            else { diag += L"'"; diag += k; diag += L"'"; }
            diag += L" (" + std::to_wstring(res.problemKeys[i].second) + L")";
            if (i + 1 < res.problemKeys.size()) diag += L", ";
        }
    }
    SetWindowText(hRes_Diagnostics, diag.c_str());

    EnableWindow(hRes_BtnCert, res.isSavedAttempt ? TRUE : FALSE);
    if (g_test.IsSurvivalFailed()) {
        SetWindowText(hRes_BestMsg, L"💥 SURVIVAL FAILED: Accuracy dropped below threshold or excessive consecutive errors!");
    } else if (!res.isSavedAttempt) {
        SetWindowText(hRes_BestMsg, L"ATTEMPT NOT SAVED: Finish a valid run (at least 3 seconds) to record a score or export a certificate.");
    } else if (res.isNewBest) {
        SetWindowText(hRes_BestMsg, L"🎉 NEW RECORD! Personal best speed achieved for this category!");
    } else {
        swprintf(buf, 256, L"Current record for this category: %.1f WPM", g_test.GetCurrentHighScore());
        SetWindowText(hRes_BestMsg, buf);
    }

    InvalidateRect(hRes_GradeBadge, NULL, TRUE);
    InvalidateRect(hRes_Panel, NULL, TRUE);
    InvalidateRect(hRes_GraphPanel, NULL, TRUE);
}

// ── Custom Drawing: Dual-Lane Ghost Race Track ────────────────────────────────
static void DrawRaceTrack(HWND hTrack, HDC hdc) {
    RECT cr;
    GetClientRect(hTrack, &cr);

    DrawRoundedCard(hdc, cr, 10, CLR_PANEL, CLR_BORDER, 1);

    int laneH = (cr.bottom - cr.top - 10) / 2;
    int trackLeft = cr.left + 90;
    int trackRight = cr.right - 60;
    int trackW = trackRight - trackLeft;

    // Lane 1: Player Racer
    int y1 = cr.top + 6;
    SetBkMode(hdc, TRANSPARENT);
    SetTextColor(hdc, CLR_ACCENT);
    SelectObject(hdc, g_hFontSmall);
    RECT l1Lbl = { cr.left + 10, y1 + 3, trackLeft - 10, y1 + laneH };
    DrawText(hdc, L"🏎 YOU", -1, &l1Lbl, DT_LEFT | DT_SINGLELINE);

    // Lane 1 Track Line
    HPEN lanePen = CreatePen(PS_DOT, 1, RGB(45, 60, 95));
    HPEN oldPen = (HPEN)SelectObject(hdc, lanePen);
    MoveToEx(hdc, trackLeft, y1 + laneH / 2, NULL);
    LineTo(hdc, trackRight, y1 + laneH / 2);

    // Player Avatar Position
    double playerProg = g_test.GetPlayerProgress();
    int playerX = trackLeft + (int)(trackW * playerProg);
    if (playerX > trackRight) playerX = trackRight;

    HBRUSH playerBr = CreateSolidBrush(CLR_ACCENT);
    HBRUSH oldBr = (HBRUSH)SelectObject(hdc, playerBr);
    SelectObject(hdc, GetStockObject(NULL_PEN));
    RoundRect(hdc, playerX - 12, y1 + 3, playerX + 12, y1 + laneH - 3, 6, 6);
    SelectObject(hdc, oldBr);
    DeleteObject(playerBr);

    // Lane 2: AI Bot Racer
    int y2 = y1 + laneH + 2;
    int botSpeed = g_test.GetBotSpeed();
    if (botSpeed > 0) {
        SetTextColor(hdc, CLR_AMBER);
        RECT l2Lbl = { cr.left + 10, y2 + 3, trackLeft - 10, y2 + laneH };
        wchar_t botTag[32];
        swprintf(botTag, 32, L"🤖 BOT (%d)", botSpeed);
        DrawText(hdc, botTag, -1, &l2Lbl, DT_LEFT | DT_SINGLELINE);

        MoveToEx(hdc, trackLeft, y2 + laneH / 2, NULL);
        LineTo(hdc, trackRight, y2 + laneH / 2);

        double botProg = g_test.GetBotProgress();
        int botX = trackLeft + (int)(trackW * botProg);
        if (botX > trackRight) botX = trackRight;

        HBRUSH botBr = CreateSolidBrush(CLR_WRONG);
        SelectObject(hdc, botBr);
        RoundRect(hdc, botX - 12, y2 + 3, botX + 12, y2 + laneH - 3, 6, 6);
        DeleteObject(botBr);

        // Lead indicator text
        int lead = g_test.GetRaceLeadChars();
        wchar_t leadBuf[64];
        if (lead >= 0) {
            swprintf(leadBuf, 64, L"+%d chars ahead", lead);
            SetTextColor(hdc, CLR_CORRECT);
        } else {
            swprintf(leadBuf, 64, L"%d chars behind", lead);
            SetTextColor(hdc, CLR_WRONG);
        }
        RECT leadRc = { trackRight - 110, y1 + 2, trackRight, y1 + laneH };
        DrawText(hdc, leadBuf, -1, &leadRc, DT_RIGHT | DT_SINGLELINE);
    } else {
        SetTextColor(hdc, CLR_MUTED);
        RECT l2Lbl = { cr.left + 10, y2 + 3, trackLeft - 10, y2 + laneH };
        DrawText(hdc, L"🤖 BOT (OFF)", -1, &l2Lbl, DT_LEFT | DT_SINGLELINE);
    }

    // Finish Flag Marker
    SetTextColor(hdc, RGB(255, 215, 0));
    SelectObject(hdc, g_hFontBold);
    RECT flagRc = { trackRight + 5, y1 + 6, cr.right - 5, cr.bottom - 6 };
    DrawText(hdc, L"🏁", -1, &flagRc, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

    SelectObject(hdc, oldPen);
    DeleteObject(lanePen);
}

// ── Custom Drawing: Highlighted Canvas ─────────────────────────────────────────
static void DrawHighlightedCanvas(HWND hCanvas, HDC hdc) {
    RECT cr;
    GetClientRect(hCanvas, &cr);

    DrawRoundedCard(hdc, cr, 8, CLR_PANEL, CLR_BORDER, 1);

    if (g_test.IsPaused()) {
        SetBkMode(hdc, TRANSPARENT);
        SetTextColor(hdc, CLR_AMBER);
        SelectObject(hdc, g_hFontHero);
        DrawText(hdc, L"⏸ TEST PAUSED", -1, &cr, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
        return;
    }

    SelectObject(hdc, g_hFontMono);
    SetBkMode(hdc, TRANSPARENT);

    const std::wstring&            text     = g_test.GetSampleText();
    const std::vector<CharStatus>& statuses = g_test.GetCharStatuses();

    int x = 16, y = 14;
    int lineHeight = 26;
    int maxX = cr.right - 16;

    SIZE spaceSz;
    GetTextExtentPoint32(hdc, L" ", 1, &spaceSz);

    size_t i = 0;
    while (i < text.size()) {
        if (text[i] == L'\n') {
            x = 16;
            y += lineHeight;
            ++i;
            if (y + lineHeight > cr.bottom - 6) break;
            continue;
        }

        size_t wStart = i;
        while (i < text.size() && text[i] != L' ' && text[i] != L'\n') ++i;
        size_t wEnd = i;
        bool hasSpace = (i < text.size() && text[i] == L' ');
        if (hasSpace) ++i;

        std::wstring word = text.substr(wStart, wEnd - wStart);
        SIZE wordSz;
        GetTextExtentPoint32(hdc, word.c_str(), (int)word.size(), &wordSz);

        if (x + wordSz.cx > maxX && x > 16) {
            x = 16;
            y += lineHeight;
            if (y + lineHeight > cr.bottom - 6) break;
        }

        for (size_t ci = wStart; ci < wEnd; ++ci) {
            CharStatus st = (ci < statuses.size()) ? statuses[ci] : CharStatus::Untouched;
            wchar_t ch[2] = { text[ci], 0 };
            SIZE chSz;
            GetTextExtentPoint32(hdc, ch, 1, &chSz);

            if (st == CharStatus::Wrong) {
                RECT errBg = { x - 1, y, x + chSz.cx + 1, y + lineHeight };
                HBRUSH errBr = CreateSolidBrush(RGB(90, 25, 35));
                FillRect(hdc, &errBg, errBr);
                DeleteObject(errBr);
            }

            COLORREF clr = CLR_TEXT;
            if (st == CharStatus::Correct)     clr = CLR_CORRECT;
            else if (st == CharStatus::Wrong)   clr = CLR_WRONG;
            else                                clr = RGB(170, 185, 220);

            SetTextColor(hdc, clr);
            RECT tr = { x, y, x + chSz.cx + 2, y + lineHeight };
            DrawText(hdc, ch, 1, &tr, DT_LEFT | DT_TOP | DT_NOCLIP);

            // Cursor underline for next character
            if (ci == g_test.GetTypedText().size()) {
                RECT curRc = { x, y + lineHeight - 3, x + chSz.cx, y + lineHeight - 1 };
                HBRUSH curBr = CreateSolidBrush(CLR_ACCENT);
                FillRect(hdc, &curRc, curBr);
                DeleteObject(curBr);
            }

            x += chSz.cx;
        }

        if (hasSpace) {
            if (wEnd == g_test.GetTypedText().size()) {
                RECT curRc = { x, y + lineHeight - 3, x + spaceSz.cx, y + lineHeight - 1 };
                HBRUSH curBr = CreateSolidBrush(CLR_ACCENT);
                FillRect(hdc, &curRc, curBr);
                DeleteObject(curBr);
            }
            x += spaceSz.cx;
        }
    }
}

// ── Custom Drawing: Animated Gradient Progress Bar ────────────────────────────
static void DrawProgressBar(HWND hBar, HDC hdc) {
    RECT cr;
    GetClientRect(hBar, &cr);

    HBRUSH bg = CreateSolidBrush(RGB(32, 42, 68));
    FillRect(hdc, &cr, bg);
    DeleteObject(bg);

    int total = g_test.GetTotalTime();
    int rem   = g_test.GetTimeRemaining();
    if (total <= 0) return;

    double ratio = (double)rem / total;
    if (ratio < 0.0) ratio = 0.0;
    if (ratio > 1.0) ratio = 1.0;

    int fillW = (int)((cr.right - cr.left) * ratio);
    if (fillW > 0) {
        COLORREF barClr = CLR_ACCENT;
        if (ratio <= 0.25)      barClr = CLR_WRONG;
        else if (ratio <= 0.50) barClr = CLR_AMBER;

        RECT fillRc = { cr.left, cr.top, cr.left + fillW, cr.bottom };
        HBRUSH fillBr = CreateSolidBrush(barClr);
        FillRect(hdc, &fillRc, fillBr);
        DeleteObject(fillBr);
    }
}

// ── Custom Drawing: Interactive Virtual Keyboard Guide ────────────────────────
static void DrawVirtualKeyboard(HWND hKb, HDC hdc) {
    RECT cr;
    GetClientRect(hKb, &cr);

    DrawRoundedCard(hdc, cr, 10, CLR_PANEL, CLR_BORDER, 1);

    // Keyboard Key Definitions
    struct KeyDef {
        const wchar_t* label;
        wchar_t matchChar;
        int fingerColorIdx; // 0=Pinky, 1=Ring, 2=Middle, 3=Index, 4=Thumb
        float widthUnits;
    };

    static const COLORREF FINGER_COLORS[5] = {
        RGB(255, 110, 140), // Pinky (Coral)
        RGB(255, 175, 75),  // Ring (Orange/Amber)
        RGB(245, 220, 80),  // Middle (Yellow)
        RGB(0,   215, 160), // Index (Green/Mint)
        RGB(0,   200, 255)  // Thumb (Cyan)
    };

    static const std::vector<std::vector<KeyDef>> ROWS = {
        {
            { L"`", '`', 0, 1.0f }, { L"1", '1', 0, 1.0f }, { L"2", '2', 1, 1.0f }, { L"3", '3', 2, 1.0f },
            { L"4", '4', 3, 1.0f }, { L"5", '5', 3, 1.0f }, { L"6", '6', 3, 1.0f }, { L"7", '7', 3, 1.0f },
            { L"8", '8', 2, 1.0f }, { L"9", '9', 1, 1.0f }, { L"0", '0', 0, 1.0f }, { L"-", '-', 0, 1.0f },
            { L"=", '=', 0, 1.0f }, { L"Bksp", 8, 0, 1.6f }
        },
        {
            { L"Tab", 9, 0, 1.5f }, { L"Q", 'Q', 0, 1.0f }, { L"W", 'W', 1, 1.0f }, { L"E", 'E', 2, 1.0f },
            { L"R", 'R', 3, 1.0f }, { L"T", 'T', 3, 1.0f }, { L"Y", 'Y', 3, 1.0f }, { L"U", 'U', 3, 1.0f },
            { L"I", 'I', 2, 1.0f }, { L"O", 'O', 1, 1.0f }, { L"P", 'P', 0, 1.0f }, { L"[", '[', 0, 1.0f },
            { L"]", ']', 0, 1.0f }, { L"\\", '\\', 0, 1.1f }
        },
        {
            { L"Caps", 0, 0, 1.7f }, { L"A", 'A', 0, 1.0f }, { L"S", 'S', 1, 1.0f }, { L"D", 'D', 2, 1.0f },
            { L"F", 'F', 3, 1.0f }, { L"G", 'G', 3, 1.0f }, { L"H", 'H', 3, 1.0f }, { L"J", 'J', 3, 1.0f },
            { L"K", 'K', 2, 1.0f }, { L"L", 'L', 1, 1.0f }, { L";", ';', 0, 1.0f }, { L"'", '\'', 0, 1.0f },
            { L"Enter", 13, 0, 1.9f }
        },
        {
            { L"Shift", 0, 0, 2.2f }, { L"Z", 'Z', 0, 1.0f }, { L"X", 'X', 1, 1.0f }, { L"C", 'C', 2, 1.0f },
            { L"V", 'V', 3, 1.0f }, { L"B", 'B', 3, 1.0f }, { L"N", 'N', 3, 1.0f }, { L"M", 'M', 3, 1.0f },
            { L",", ',', 2, 1.0f }, { L".", '.', 1, 1.0f }, { L"/", '/', 0, 1.0f }, { L"Shift", 0, 0, 2.4f }
        },
        {
            { L"Ctrl", 0, 0, 1.5f }, { L"Alt", 0, 0, 1.5f }, { L"Space", ' ', 4, 7.5f },
            { L"Alt", 0, 0, 1.5f }, { L"Ctrl", 0, 0, 1.5f }
        }
    };

    wchar_t nextChar = g_test.GetNextChar();
    wchar_t nextUpper = (nextChar >= 'a' && nextChar <= 'z') ? (wchar_t)(nextChar - 32) : nextChar;

    int totalW = cr.right - cr.left - 24;
    float baseUnitW = (float)totalW / 15.6f;
    int keyH = 26;
    int startY = cr.top + 8;
    int rowGap = 4;

    for (size_t r = 0; r < ROWS.size(); ++r) {
        int curX = cr.left + 12;
        int curY = startY + (int)r * (keyH + rowGap);

        for (const auto& key : ROWS[r]) {
            int curW = (int)(key.widthUnits * baseUnitW);
            RECT krc = { curX, curY, curX + curW - 3, curY + keyH };

            bool isNext = false;
            if (nextChar != 0) {
                if (key.matchChar == ' ' && nextChar == ' ') isNext = true;
                else if (key.matchChar != 0 && key.matchChar == nextUpper) isNext = true;
            }

            COLORREF bg = isNext ? RGB(0, 180, 255) : RGB(32, 42, 66);
            COLORREF border = isNext ? RGB(255, 255, 255) : RGB(50, 65, 100);
            DrawRoundedCard(hdc, krc, 5, bg, border, isNext ? 2 : 1);

            // Finger guide dot
            COLORREF fClr = FINGER_COLORS[key.fingerColorIdx];
            HBRUSH fBr = CreateSolidBrush(fClr);
            HPEN noP = (HPEN)GetStockObject(NULL_PEN);
            HPEN oP = (HPEN)SelectObject(hdc, noP);
            HBRUSH oB = (HBRUSH)SelectObject(hdc, fBr);
            Ellipse(hdc, krc.left + 4, krc.bottom - 7, krc.left + 8, krc.bottom - 3);
            SelectObject(hdc, oP);
            SelectObject(hdc, oB);
            DeleteObject(fBr);

            // Key label
            SetBkMode(hdc, TRANSPARENT);
            SetTextColor(hdc, isNext ? RGB(10, 15, 28) : CLR_TEXT);
            SelectObject(hdc, g_hFontSmall);
            DrawText(hdc, key.label, -1, &krc, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

            curX += curW;
        }
    }
}

// ── Custom Drawing: Real-time WPM Timeline Line Graph ─────────────────────────
static void DrawResultsGraph(HWND hGraph, HDC hdc) {
    RECT cr;
    GetClientRect(hGraph, &cr);

    DrawRoundedCard(hdc, cr, 10, CLR_PANEL, CLR_BORDER, 1);

    const std::vector<double>& timeline = g_test.GetResult().wpmTimeline;
    if (timeline.size() < 2) {
        SetBkMode(hdc, TRANSPARENT);
        SetTextColor(hdc, CLR_MUTED);
        SelectObject(hdc, g_hFontNormal);
        DrawText(hdc, L"Performance Timeline: Collecting Keystroke Samples...", -1, &cr, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
        return;
    }

    int leftM = 45;
    int rightM = 20;
    int topM = 25;
    int botM = 30;

    int graphW = cr.right - cr.left - leftM - rightM;
    int graphH = cr.bottom - cr.top - topM - botM;

    // Find peak WPM
    double maxWPM = 60.0;
    for (double w : timeline) {
        if (w > maxWPM) maxWPM = w;
    }
    maxWPM = std::ceil(maxWPM / 20.0) * 20.0;

    // Graph Title
    SetBkMode(hdc, TRANSPARENT);
    SetTextColor(hdc, CLR_ACCENT);
    SelectObject(hdc, g_hFontSmall);
    RECT titleRc = { cr.left + leftM, cr.top + 6, cr.right - rightM, cr.top + 22 };
    DrawText(hdc, L"📈 REAL-TIME SPEED TIMELINE (WPM vs TIME)", -1, &titleRc, DT_LEFT | DT_SINGLELINE);

    // Draw Grid Lines & Y-Axis Labels
    HPEN gridPen = CreatePen(PS_DOT, 1, RGB(40, 52, 80));
    HPEN oldPen = (HPEN)SelectObject(hdc, gridPen);

    for (int steps = 0; steps <= 4; ++steps) {
        double val = (maxWPM / 4.0) * steps;
        int y = cr.bottom - botM - (int)((val / maxWPM) * graphH);

        MoveToEx(hdc, cr.left + leftM, y, NULL);
        LineTo(hdc, cr.right - rightM, y);

        wchar_t yLbl[16];
        swprintf(yLbl, 16, L"%.0f", val);
        RECT yRc = { cr.left + 5, y - 8, cr.left + leftM - 6, y + 8 };
        SetTextColor(hdc, CLR_MUTED);
        SelectObject(hdc, g_hFontSmall);
        DrawText(hdc, yLbl, -1, &yRc, DT_RIGHT | DT_SINGLELINE);
    }
    SelectObject(hdc, oldPen);
    DeleteObject(gridPen);

    // Plot WPM Line with glowing accent
    HPEN linePen = CreatePen(PS_SOLID, 3, CLR_ACCENT);
    oldPen = (HPEN)SelectObject(hdc, linePen);

    size_t n = timeline.size();
    std::vector<POINT> pts(n);
    for (size_t i = 0; i < n; ++i) {
        int x = cr.left + leftM + (int)((double)i / (n - 1) * graphW);
        int y = cr.bottom - botM - (int)((timeline[i] / maxWPM) * graphH);
        pts[i] = { x, y };
        if (i == 0) MoveToEx(hdc, x, y, NULL);
        else LineTo(hdc, x, y);
    }
    SelectObject(hdc, oldPen);
    DeleteObject(linePen);

    // Draw vertex dots
    HBRUSH dotBr = CreateSolidBrush(CLR_CORRECT);
    HBRUSH oBr = (HBRUSH)SelectObject(hdc, dotBr);
    HPEN nPen = (HPEN)GetStockObject(NULL_PEN);
    SelectObject(hdc, nPen);
    for (size_t i = 0; i < n; i += (n > 20 ? 2 : 1)) {
        Ellipse(hdc, pts[i].x - 3, pts[i].y - 3, pts[i].x + 4, pts[i].y + 4);
    }
    SelectObject(hdc, oBr);
    DeleteObject(dotBr);
}

// ── Custom Drawing: Owner-Drawn Buttons ────────────────────────────────────────
static void DrawOwnerButton(DRAWITEMSTRUCT* dis) {
    HDC hdc = dis->hDC;
    RECT rc = dis->rcItem;
    int id = dis->CtlID;

    bool isHovered = (g_hoveredBtnId == id);
    bool isPressed = (dis->itemState & ODS_SELECTED);

    COLORREF fillClr = CLR_ACCENT;
    COLORREF textClr = RGB(10, 15, 28);
    COLORREF borderClr = isHovered ? RGB(255, 255, 255) : fillClr;

    // Category Tabs Active State
    if (id == IDC_TAB_PROSE || id == IDC_TAB_CODE || id == IDC_TAB_NUMBERS || id == IDC_TAB_SURVIVAL) {
        bool isCurrent = false;
        Category cur = g_test.GetCategory();
        if (id == IDC_TAB_PROSE    && cur == Category::Prose)    isCurrent = true;
        if (id == IDC_TAB_CODE     && cur == Category::Code)     isCurrent = true;
        if (id == IDC_TAB_NUMBERS  && cur == Category::Numbers)  isCurrent = true;
        if (id == IDC_TAB_SURVIVAL && cur == Category::Survival) isCurrent = true;

        fillClr = isCurrent ? CLR_ACCENT : (isHovered ? RGB(38, 50, 80) : CLR_PANEL);
        textClr = isCurrent ? RGB(10, 15, 28) : CLR_TEXT;
        borderClr = isCurrent ? CLR_ACCENT : (isHovered ? CLR_ACCENT : CLR_BORDER);
    }
    // Bot Speed Buttons Active State
    else if (id == IDC_BTN_BOT_OFF || id == IDC_BTN_BOT_EASY || id == IDC_BTN_BOT_MED || id == IDC_BTN_BOT_HARD) {
        int curBot = g_test.GetBotSpeed();
        bool isCurrent = false;
        if (id == IDC_BTN_BOT_OFF  && curBot == 0)  isCurrent = true;
        if (id == IDC_BTN_BOT_EASY && curBot == 35) isCurrent = true;
        if (id == IDC_BTN_BOT_MED  && curBot == 60) isCurrent = true;
        if (id == IDC_BTN_BOT_HARD && curBot == 85) isCurrent = true;

        fillClr = isCurrent ? CLR_AMBER : (isHovered ? RGB(38, 50, 80) : CLR_PANEL);
        textClr = isCurrent ? RGB(10, 15, 28) : CLR_TEXT;
        borderClr = isCurrent ? CLR_AMBER : (isHovered ? CLR_AMBER : CLR_BORDER);
    }
    // Toolbar buttons
    else if (id == IDC_BTN_SOUND_TOGGLE || id == IDC_BTN_VIEW_HISTORY || id == IDC_BTN_KB_TOGGLE) {
        fillClr = isHovered ? RGB(45, 60, 95) : RGB(24, 32, 52);
        textClr = CLR_TEXT;
        borderClr = isHovered ? CLR_ACCENT : CLR_BORDER;
    }
    // Stop / Pause / Custom
    else if (id == IDC_BTN_STOP) {
        fillClr = isHovered ? RGB(255, 95, 110) : CLR_WRONG;
        textClr = RGB(255, 255, 255);
        borderClr = fillClr;
    } else if (id == IDC_BTN_PAUSE) {
        fillClr = isHovered ? RGB(255, 205, 110) : CLR_AMBER;
        textClr = RGB(20, 20, 20);
        borderClr = fillClr;
    } else if (id == IDC_BTN_CUSTOM || id == IDC_BTN_CHANGEDIFF) {
        fillClr = isHovered ? RGB(45, 60, 95) : RGB(30, 42, 68);
        textClr = CLR_TEXT;
        borderClr = isHovered ? CLR_ACCENT : CLR_BORDER;
    } else if (id == IDC_BTN_EXPORT_CERT) {
        fillClr = isHovered ? RGB(0, 255, 140) : CLR_CORRECT;
        textClr = RGB(10, 15, 28);
        borderClr = fillClr;
    } else {
        if (isHovered) fillClr = CLR_BTN_HOVER;
        if (isPressed) fillClr = RGB(0, 175, 220);
        borderClr = fillClr;
    }

    DrawRoundedCard(hdc, rc, 8, fillClr, borderClr, 1);

    wchar_t txt[128] = { 0 };
    GetWindowText(dis->hwndItem, txt, 128);

    SetBkMode(hdc, TRANSPARENT);
    SetTextColor(hdc, textClr);
    SelectObject(hdc, g_hFontBold);
    DrawText(hdc, txt, -1, &rc, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
}

// ── Custom Drawing: Difficulty Selection Card ─────────────────────────────────
static void DrawDifficultyCard(HWND hCard, HDC hdc, Difficulty diff, const wchar_t* title, const wchar_t* timeStr, const wchar_t* desc, COLORREF colorDot) {
    RECT cr;
    GetClientRect(hCard, &cr);

    bool isSelected = (g_test.GetDifficulty() == diff);
    COLORREF bgClr     = isSelected ? CLR_CARD_ACTIVE : CLR_PANEL;
    COLORREF borderClr = isSelected ? CLR_ACCENT : CLR_BORDER;

    DrawRoundedCard(hdc, cr, 12, bgClr, borderClr, isSelected ? 2 : 1);

    // Accent Dot
    HBRUSH dotBr = CreateSolidBrush(colorDot);
    HPEN noPen = (HPEN)GetStockObject(NULL_PEN);
    HPEN oldPen = (HPEN)SelectObject(hdc, noPen);
    HBRUSH oldBr = (HBRUSH)SelectObject(hdc, dotBr);
    Ellipse(hdc, cr.left + 20, cr.top + 20, cr.left + 34, cr.top + 34);
    SelectObject(hdc, oldPen);
    SelectObject(hdc, oldBr);
    DeleteObject(dotBr);

    // Card Title
    SetBkMode(hdc, TRANSPARENT);
    SetTextColor(hdc, CLR_TEXT);
    SelectObject(hdc, g_hFontCard);
    RECT tr = { cr.left + 44, cr.top + 16, cr.right - 20, cr.top + 40 };
    DrawText(hdc, title, -1, &tr, DT_LEFT | DT_SINGLELINE);

    // Time Limit Label
    SetTextColor(hdc, CLR_ACCENT);
    SelectObject(hdc, g_hFontBold);
    RECT tmr = { cr.left + 20, cr.top + 48, cr.right - 20, cr.top + 68 };
    DrawText(hdc, timeStr, -1, &tmr, DT_LEFT | DT_SINGLELINE);

    // Subtitle Description
    SetTextColor(hdc, CLR_MUTED);
    SelectObject(hdc, g_hFontNormal);
    RECT dsr = { cr.left + 20, cr.top + 74, cr.right - 20, cr.bottom - 10 };
    DrawText(hdc, desc, -1, &dsr, DT_LEFT | DT_WORDBREAK);
}

// ── Custom Drawing: Grade Circle Badge ────────────────────────────────────────
static void DrawGradeCircle(HWND hCircle, HDC hdc) {
    RECT cr;
    GetClientRect(hCircle, &cr);

    std::wstring grade = g_test.GetResult().grade;
    COLORREF badgeClr = RGB(244, 63, 94); // Coral Red
    if (grade == L"S+") badgeClr = RGB(250, 204, 21);       // Warm Gold
    else if (grade == L"A") badgeClr = RGB(34, 197, 94);     // Emerald
    else if (grade == L"B") badgeClr = RGB(56, 189, 248);    // Sky Blue
    else if (grade == L"C") badgeClr = RGB(251, 146, 60);    // Amber

    HBRUSH bg = CreateSolidBrush(CLR_PANEL);
    FillRect(hdc, &cr, bg);
    DeleteObject(bg);

    HBRUSH br = CreateSolidBrush(RGB(28, 36, 54));
    HPEN pen = CreatePen(PS_SOLID, 3, badgeClr);
    HPEN oPen = (HPEN)SelectObject(hdc, pen);
    HBRUSH oBr = (HBRUSH)SelectObject(hdc, br);
    Ellipse(hdc, 8, 8, cr.right - 8, cr.bottom - 8);
    SelectObject(hdc, oPen);
    SelectObject(hdc, oBr);
    DeleteObject(br);
    DeleteObject(pen);

    // Grade Text in matching neon color
    SetBkMode(hdc, TRANSPARENT);
    SetTextColor(hdc, badgeClr);
    SelectObject(hdc, g_hFontGrade);
    RECT gradeRc = { cr.left, cr.top + 12, cr.right, cr.bottom - 24 };
    DrawText(hdc, grade.c_str(), -1, &gradeRc, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

    SetTextColor(hdc, CLR_MUTED);
    SelectObject(hdc, g_hFontSmall);
    RECT rankRc = { cr.left, cr.bottom - 32, cr.right, cr.bottom - 8 };
    DrawText(hdc, L"RANK", -1, &rankRc, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
}

// ── Custom Practice Text Dialog Creator ────────────────────────────────────────
static void OpenCustomTextDialog(HWND hParent) {
    HWND hDlg = CreateWindowEx(
        WS_EX_DLGMODALFRAME | WS_EX_TOPMOST,
        L"#32770", L"Enter Custom Practice Text",
        WS_VISIBLE | WS_POPUP | WS_CAPTION | WS_SYSMENU,
        CW_USEDEFAULT, CW_USEDEFAULT, 560, 380,
        hParent, NULL, g_hInstance, NULL);

    if (hDlg) {
        RECT prc; GetWindowRect(hParent, &prc);
        SetWindowPos(hDlg, NULL, prc.left + 240, prc.top + 160, 560, 380, SWP_NOZORDER);

        CreateWindowEx(0, L"STATIC", L"Paste or type custom code, study paragraphs, or aptitude drills:",
                       WS_CHILD | WS_VISIBLE,
                       25, 20, 500, 24, hDlg, NULL, g_hInstance, NULL);

        HWND hInput = CreateWindowEx(
            WS_EX_CLIENTEDGE, L"EDIT",
            L"The advancement of computer systems requires robust logic, clean syntax, and rapid precision.",
            WS_CHILD | WS_VISIBLE | ES_MULTILINE | ES_AUTOVSCROLL | WS_VSCROLL,
            25, 50, 495, 200, hDlg, (HMENU)IDC_EDIT_CUSTOM_INPUT, g_hInstance, NULL);

        CreateWindowEx(0, L"BUTTON", L"Save & Apply Text",
                       WS_CHILD | WS_VISIBLE | BS_DEFPUSHBUTTON,
                       140, 275, 160, 38, hDlg, (HMENU)IDOK, g_hInstance, NULL);

        CreateWindowEx(0, L"BUTTON", L"Cancel",
                       WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
                       320, 275, 110, 38, hDlg, (HMENU)IDCANCEL, g_hInstance, NULL);

        SendMessage(hInput, WM_SETFONT, (WPARAM)g_hFontNormal, TRUE);
        EnableWindow(hParent, FALSE);

        MSG msg;
        while (IsWindow(hDlg) && GetMessage(&msg, NULL, 0, 0)) {
            if (msg.message == WM_COMMAND) {
                int cid = LOWORD(msg.wParam);
                if (cid == IDOK) {
                    int len = GetWindowTextLength(hInput);
                    if (len > 5) {
                        std::vector<wchar_t> b(len + 2, 0);
                        GetWindowText(hInput, b.data(), len + 1);
                        g_test.SetCustomText(b.data());
                        DestroyWindow(hDlg);
                        break;
                    } else {
                        MessageBox(hDlg, L"Please enter at least a few words to practice.", L"Text Too Short", MB_ICONWARNING);
                    }
                } else if (cid == IDCANCEL) {
                    DestroyWindow(hDlg);
                    break;
                }
            }
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
        EnableWindow(hParent, TRUE);
        SetForegroundWindow(hParent);
        ShowScreen(AppScreen::Home);
    }
}

// ── History Dialog ─────────────────────────────────────────────────────────────
static void OpenHistoryDialog(HWND hParent) {
    HWND hDlg = CreateWindowEx(
        WS_EX_DLGMODALFRAME | WS_EX_TOPMOST,
        L"#32770", L"Historical Test Records & Aptitude Analytics",
        WS_VISIBLE | WS_POPUP | WS_CAPTION | WS_SYSMENU,
        CW_USEDEFAULT, CW_USEDEFAULT, 680, 480,
        hParent, NULL, g_hInstance, NULL);

    if (hDlg) {
        RECT prc; GetWindowRect(hParent, &prc);
        SetWindowPos(hDlg, NULL, prc.left + 180, prc.top + 120, 680, 480, SWP_NOZORDER);

        const auto& hist = g_test.GetHistory();
        wchar_t sumBuf[256];
        double avgWpm = 0.0, bestWpm = 0.0;
        for (const auto& item : hist) {
            avgWpm += item.wpm;
            if (item.wpm > bestWpm) bestWpm = item.wpm;
        }
        if (!hist.empty()) avgWpm /= hist.size();

        swprintf(sumBuf, 256, L"Total Sessions: %d  |  Avg Speed: %.1f WPM  |  All-Time Best: %.1f WPM",
                 (int)hist.size(), avgWpm, bestWpm);

        CreateWindowEx(0, L"STATIC", sumBuf,
                       WS_CHILD | WS_VISIBLE,
                       25, 18, 620, 24, hDlg, NULL, g_hInstance, NULL);

        HWND hList = CreateWindowEx(
            WS_EX_CLIENTEDGE, L"LISTBOX", NULL,
            WS_CHILD | WS_VISIBLE | LBS_NOTIFY | WS_VSCROLL,
            25, 50, 615, 320, hDlg, (HMENU)IDC_LIST_HISTORY, g_hInstance, NULL);
        SendMessage(hList, WM_SETFONT, (WPARAM)g_hFontMono, TRUE);

        for (int i = (int)hist.size() - 1; i >= 0; --i) {
            const auto& it = hist[i];
            wchar_t row[256];
            swprintf(row, 256, L"[%s] %-14s | %5.1f WPM | %4.1f%% Acc | %4.1f%% Cons | Rank: %s",
                     it.timestamp.c_str(), it.category.c_str(), it.wpm, it.accuracy, it.consistency, it.grade.c_str());
            SendMessage(hList, LB_ADDSTRING, 0, (LPARAM)row);
        }

        CreateWindowEx(0, L"BUTTON", L"Clear History",
                       WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
                       180, 390, 140, 36, hDlg, (HMENU)IDC_BTN_HISTORY_CLEAR, g_hInstance, NULL);

        CreateWindowEx(0, L"BUTTON", L"Close",
                       WS_CHILD | WS_VISIBLE | BS_DEFPUSHBUTTON,
                       350, 390, 140, 36, hDlg, (HMENU)IDC_BTN_HISTORY_CLOSE, g_hInstance, NULL);

        EnableWindow(hParent, FALSE);

        MSG msg;
        while (IsWindow(hDlg) && GetMessage(&msg, NULL, 0, 0)) {
            if (msg.message == WM_COMMAND) {
                int cid = LOWORD(msg.wParam);
                if (cid == IDC_BTN_HISTORY_CLEAR) {
                    if (MessageBox(hDlg, L"Are you sure you want to clear all typing records?", L"Confirm Clear", MB_YESNO | MB_ICONQUESTION) == IDYES) {
                        g_test.ClearHistory();
                        SendMessage(hList, LB_RESETCONTENT, 0, 0);
                    }
                } else if (cid == IDC_BTN_HISTORY_CLOSE || cid == IDCANCEL) {
                    DestroyWindow(hDlg);
                    break;
                }
            }
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
        EnableWindow(hParent, TRUE);
        SetForegroundWindow(hParent);
    }
}

// ── Export Official Aptitude Certificate ───────────────────────────────────────
static void PromptAndExportCertificate(HWND hWnd) {
    // Prompt student name via simple dialog
    HWND hDlg = CreateWindowEx(
        WS_EX_DLGMODALFRAME | WS_EX_TOPMOST,
        L"#32770", L"Official Certificate Candidate Name",
        WS_VISIBLE | WS_POPUP | WS_CAPTION | WS_SYSMENU,
        CW_USEDEFAULT, CW_USEDEFAULT, 420, 190,
        hWnd, NULL, g_hInstance, NULL);

    if (hDlg) {
        RECT prc; GetWindowRect(hWnd, &prc);
        SetWindowPos(hDlg, NULL, prc.left + 300, prc.top + 240, 420, 190, SWP_NOZORDER);

        CreateWindowEx(0, L"STATIC", L"Enter candidate name for the official certificate:",
                       WS_CHILD | WS_VISIBLE,
                       25, 20, 360, 20, hDlg, NULL, g_hInstance, NULL);

        HWND hNameInput = CreateWindowEx(
            WS_EX_CLIENTEDGE, L"EDIT", L"Mohit",
            WS_CHILD | WS_VISIBLE | ES_AUTOHSCROLL,
            25, 48, 355, 28, hDlg, NULL, g_hInstance, NULL);
        SendMessage(hNameInput, WM_SETFONT, (WPARAM)g_hFontBold, TRUE);

        CreateWindowEx(0, L"BUTTON", L"Generate Certificate",
                       WS_CHILD | WS_VISIBLE | BS_DEFPUSHBUTTON,
                       80, 95, 160, 36, hDlg, (HMENU)IDOK, g_hInstance, NULL);

        CreateWindowEx(0, L"BUTTON", L"Cancel",
                       WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
                       255, 95, 90, 36, hDlg, (HMENU)IDCANCEL, g_hInstance, NULL);

        EnableWindow(hWnd, FALSE);

        std::wstring candidateName = L"Candidate Student";
        bool confirmed = false;

        MSG msg;
        while (IsWindow(hDlg) && GetMessage(&msg, NULL, 0, 0)) {
            if (msg.message == WM_COMMAND) {
                int cid = LOWORD(msg.wParam);
                if (cid == IDOK) {
                    wchar_t nameBuf[128];
                    GetWindowText(hNameInput, nameBuf, 128);
                    if (wcslen(nameBuf) > 0) candidateName = nameBuf;
                    confirmed = true;
                    DestroyWindow(hDlg);
                    break;
                } else if (cid == IDCANCEL) {
                    DestroyWindow(hDlg);
                    break;
                }
            }
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
        EnableWindow(hWnd, TRUE);
        SetForegroundWindow(hWnd);

        if (confirmed) {
            std::wstring certPath;
            if (g_test.ExportCertificateHTML(candidateName, certPath)) {
                ShellExecuteW(NULL, L"open", certPath.c_str(), NULL, NULL, SW_SHOWNORMAL);
                MessageBox(hWnd, L"Aptitude Certificate generated and opened in browser! You can save or print to PDF.", L"Certificate Ready", MB_ICONINFORMATION);
            } else {
                MessageBox(hWnd, L"Failed to generate certificate file.", L"Export Error", MB_ICONERROR);
            }
        }
    }
}

// ── Main Window Procedure ──────────────────────────────────────────────────────
LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    switch (msg) {
        case WM_CREATE: {
            g_hWnd = hWnd;
            InitThemeGDI();
            BuildHomeUI(hWnd);
            BuildTestUI(hWnd);
            BuildResultsUI(hWnd);
            ShowScreen(AppScreen::Home);
            return 0;
        }

        case WM_ERASEBKGND: {
            // The complete client area is painted in WM_PAINT. Suppress the
            // separate erase pass to avoid the dark/blank flash between frames.
            return 1;
        }

        case WM_PAINT: {
            PAINTSTRUCT ps;
            HDC screenDC = BeginPaint(hWnd, &ps);
            RECT rc;
            GetClientRect(hWnd, &rc);

            // Double-buffer the parent window: render the full frame off-screen
            // and copy it to the display in one operation to prevent flicker.
            HDC frameDC = CreateCompatibleDC(screenDC);
            HBITMAP frameBitmap = CreateCompatibleBitmap(screenDC,
                (rc.right > 0 ? rc.right : 1), (rc.bottom > 0 ? rc.bottom : 1));
            HGDIOBJ oldBitmap = NULL;
            if (frameDC && frameBitmap) oldBitmap = SelectObject(frameDC, frameBitmap);
            HDC drawDC = (frameDC && frameBitmap) ? frameDC : screenDC;

            FillRect(drawDC, &rc, g_hBrushBG);

            AppScreen scr = g_test.GetScreen();

            // Decorative Top Neon Bar
            RECT topBar = { 0, 0, rc.right, 4 };
            FillRect(drawDC, &topBar, g_hBrushAccent);

            if (scr == AppScreen::Test) {
                // Background cards for HUD Statistics Row
                const int hudW = 145;
                const int hudH = 68;
                const int hudStartX = 40;
                const int hudGap = 16;
                const int hudY = 406;

                for (int i = 0; i < 6; ++i) {
                    RECT hrc = { hudStartX + (hudW + hudGap) * i, hudY,
                        hudStartX + (hudW + hudGap) * i + hudW, hudY + hudH };
                    DrawRoundedCard(drawDC, hrc, 8, CLR_PANEL, CLR_BORDER, 1);
                }
            }

            if (frameDC && frameBitmap) {
                BitBlt(screenDC, 0, 0, rc.right, rc.bottom, frameDC, 0, 0, SRCCOPY);
                if (oldBitmap) SelectObject(frameDC, oldBitmap);
                DeleteObject(frameBitmap);
                DeleteDC(frameDC);
            }

            EndPaint(hWnd, &ps);
            return 0;
        }

        case WM_CTLCOLORSTATIC: {
            HDC  hdc  = (HDC)wParam;
            HWND hCtl = (HWND)lParam;
            SetBkMode(hdc, TRANSPARENT);

            if (hCtl == hNav_Title || hCtl == hRes_Title) {
                SetTextColor(hdc, CLR_ACCENT);
                return (LRESULT)g_hBrushBG;
            }
            if (hCtl == hHome_ScoreBox || hCtl == hRes_BestMsg) {
                SetTextColor(hdc, CLR_CORRECT);
                return (LRESULT)g_hBrushBG;
            }
            if (hCtl == hTest_CapsWarn) {
                SetTextColor(hdc, CLR_WRONG);
                return (LRESULT)g_hBrushBG;
            }
            if (hCtl == hTest_WpmVal || hCtl == hTest_CpmVal || hCtl == hTest_AccVal || hCtl == hTest_ConsVal || hCtl == hTest_TimeVal || hCtl == hTest_BackVal) {
                SetTextColor(hdc, CLR_ACCENT);
                return (LRESULT)g_hBrushPanel;
            }
            if (hCtl == hRes_WpmVal || hCtl == hRes_CpmVal || hCtl == hRes_AccVal || hCtl == hRes_ConsVal || hCtl == hRes_WordVal || hCtl == hRes_ErrVal || hCtl == hRes_TimeVal) {
                SetTextColor(hdc, CLR_TEXT);
                return (LRESULT)g_hBrushPanel;
            }
            if (hCtl == hRes_Diagnostics) {
                SetTextColor(hdc, RGB(220, 235, 255));
                return (LRESULT)g_hBrushPanel;
            }
            if (hCtl == hTest_WpmLbl || hCtl == hTest_CpmLbl || hCtl == hTest_AccLbl || hCtl == hTest_ConsLbl || hCtl == hTest_TimeLbl || hCtl == hTest_BackLbl ||
                hCtl == hRes_WpmLbl || hCtl == hRes_CpmLbl || hCtl == hRes_AccLbl || hCtl == hRes_ConsLbl || hCtl == hRes_WordLbl || hCtl == hRes_ErrLbl || hCtl == hRes_TimeLbl ||
                hCtl == hHome_DiffLabel || hCtl == hHome_BotLabel || hCtl == hHome_Footer || hCtl == hNav_Subtitle) {
                SetTextColor(hdc, CLR_MUTED);
                return (LRESULT)g_hBrushBG;
            }

            SetTextColor(hdc, CLR_TEXT);
            return (LRESULT)g_hBrushBG;
        }

        case WM_CTLCOLOREDIT: {
            HDC hdc = (HDC)wParam;
            SetTextColor(hdc, CLR_TEXT);
            SetBkColor(hdc, CLR_EDIT_BG);
            return (LRESULT)g_hBrushEdit;
        }

        case WM_DRAWITEM: {
            DRAWITEMSTRUCT* dis = (DRAWITEMSTRUCT*)lParam;
            if (!dis) break;

            if (dis->hwndItem == hHome_CardEasy) {
                Category c = g_test.GetCategory();
                if (c == Category::Code) {
                    DrawDifficultyCard(dis->hwndItem, dis->hDC, Difficulty::Easy,
                                       L"C++ Algorithms", L"⏱ 60 Seconds", L"Vectors, templates, pointer logic & binary search.", CLR_CORRECT);
                } else {
                    DrawDifficultyCard(dis->hwndItem, dis->hDC, Difficulty::Easy,
                                       L"Easy Mode", L"⏱ 60 Seconds", L"Simple vocabulary and relaxed sentences for beginners.", CLR_CORRECT);
                }
            } else if (dis->hwndItem == hHome_CardMed) {
                Category c = g_test.GetCategory();
                if (c == Category::Code) {
                    DrawDifficultyCard(dis->hwndItem, dis->hDC, Difficulty::Medium,
                                       L"Python / Data", L"⏱ 60 Seconds", L"List comprehensions, JSON pipelines & quick sort.", CLR_ACCENT);
                } else {
                    DrawDifficultyCard(dis->hwndItem, dis->hDC, Difficulty::Medium,
                                       L"Medium Mode", L"⏱ 45 Seconds", L"Technical concepts, distributed systems & cloud computing.", CLR_ACCENT);
                }
            } else if (dis->hwndItem == hHome_CardHard) {
                Category c = g_test.GetCategory();
                if (c == Category::Code) {
                    DrawDifficultyCard(dis->hwndItem, dis->hDC, Difficulty::Hard,
                                       L"SQL & Web APIs", L"⏱ 60 Seconds", L"Database queries, joins, async/await fetch calls.", CLR_WRONG);
                } else {
                    DrawDifficultyCard(dis->hwndItem, dis->hDC, Difficulty::Hard,
                                       L"Hard Mode", L"⏱ 30 Seconds", L"Post-quantum crypto, memory alignment & rapid pace.", CLR_WRONG);
                }
            } else if (dis->hwndItem == hTest_RaceTrack) {
                DrawRaceTrack(dis->hwndItem, dis->hDC);
            } else if (dis->hwndItem == hTest_Canvas) {
                DrawHighlightedCanvas(dis->hwndItem, dis->hDC);
            } else if (dis->hwndItem == hTest_Progress) {
                DrawProgressBar(dis->hwndItem, dis->hDC);
            } else if (dis->hwndItem == hTest_Keyboard) {
                DrawVirtualKeyboard(dis->hwndItem, dis->hDC);
            } else if (dis->hwndItem == hRes_Panel) {
                RECT pr; GetClientRect(dis->hwndItem, &pr);
                DrawRoundedCard(dis->hDC, pr, 14, CLR_PANEL, CLR_BORDER, 1);
            } else if (dis->hwndItem == hRes_GradeBadge) {
                DrawGradeCircle(dis->hwndItem, dis->hDC);
            } else if (dis->hwndItem == hRes_GraphPanel) {
                DrawResultsGraph(dis->hwndItem, dis->hDC);
            } else {
                DrawOwnerButton(dis);
            }
            return TRUE;
        }

        case WM_TIMER: {
            if (wParam == TIMER_COUNTDOWN) {
                g_test.Tick();
                RefreshTestUI();
                if (g_test.IsFinished()) {
                    KillTimer(hWnd, TIMER_COUNTDOWN);
                    SoundEngine::Instance().Play(SoundFX::Bell);
                    ShowScreen(AppScreen::Results);
                }
            }
            return 0;
        }

        case WM_COMMAND: {
            int id = LOWORD(wParam);

            // Category Tab Switches
            if (id == IDC_TAB_PROSE) {
                g_test.SetCategory(Category::Prose);
                ShowScreen(AppScreen::Home);
            } else if (id == IDC_TAB_CODE) {
                g_test.SetCategory(Category::Code);
                ShowScreen(AppScreen::Home);
            } else if (id == IDC_TAB_NUMBERS) {
                g_test.SetCategory(Category::Numbers);
                ShowScreen(AppScreen::Home);
            } else if (id == IDC_TAB_SURVIVAL) {
                g_test.SetCategory(Category::Survival);
                ShowScreen(AppScreen::Home);
            }

            // Difficulty Cards Selection
            else if (id == IDC_CARD_EASY) {
                g_test.SetDifficulty(Difficulty::Easy);
                ShowScreen(AppScreen::Home);
            } else if (id == IDC_CARD_MEDIUM) {
                g_test.SetDifficulty(Difficulty::Medium);
                ShowScreen(AppScreen::Home);
            } else if (id == IDC_CARD_HARD) {
                g_test.SetDifficulty(Difficulty::Hard);
                ShowScreen(AppScreen::Home);
            }

            // Bot Speed Switches
            else if (id == IDC_BTN_BOT_OFF) {
                g_test.SetBotSpeed(0);
                InvalidateRect(hWnd, NULL, TRUE);
            } else if (id == IDC_BTN_BOT_EASY) {
                g_test.SetBotSpeed(35);
                InvalidateRect(hWnd, NULL, TRUE);
            } else if (id == IDC_BTN_BOT_MED) {
                g_test.SetBotSpeed(60);
                InvalidateRect(hWnd, NULL, TRUE);
            } else if (id == IDC_BTN_BOT_HARD) {
                g_test.SetBotSpeed(85);
                InvalidateRect(hWnd, NULL, TRUE);
            }

            // Sound Toggle
            else if (id == IDC_BTN_SOUND_TOGGLE) {
                SoundEngine::Instance().Toggle();
                SetWindowText(hNav_BtnSound, SoundEngine::Instance().IsEnabled() ? L"🔊 SOUND: ON" : L"🔈 SOUND: OFF");
                InvalidateRect(hNav_BtnSound, NULL, TRUE);
            }

            // History Modal
            else if (id == IDC_BTN_VIEW_HISTORY) {
                OpenHistoryDialog(hWnd);
            }

            // Keyboard Toggle
            else if (id == IDC_BTN_KB_TOGGLE) {
                g_showKeyboard = !g_showKeyboard;
                if (g_test.GetScreen() == AppScreen::Test) {
                    ShowWindow(hTest_Keyboard, g_showKeyboard ? SW_SHOW : SW_HIDE);
                }
            }

            // Start Test
            else if (id == IDC_BTN_START) {
                g_test.StartTest();
                SendMessage(hTest_Edit, EM_SETLIMITTEXT, (WPARAM)g_test.GetSampleText().size(), 0);
                ShowScreen(AppScreen::Test);
                SetTimer(hWnd, TIMER_COUNTDOWN, 1000, NULL);
            }

            // Custom Text Mode
            else if (id == IDC_BTN_CUSTOM) {
                OpenCustomTextDialog(hWnd);
            }

            // Pause / Resume Toggle
            else if (id == IDC_BTN_PAUSE) {
                g_test.TogglePause();
                if (g_test.IsPaused()) {
                    SetWindowText(hTest_BtnPause, L"▶  RESUME");
                    EnableWindow(hTest_Edit, FALSE);
                } else {
                    SetWindowText(hTest_BtnPause, L"⏸  PAUSE");
                    EnableWindow(hTest_Edit, TRUE);
                    SetFocus(hTest_Edit);
                }
                InvalidateRect(hTest_Canvas, NULL, FALSE);
            }

            // Stop Test
            else if (id == IDC_BTN_STOP) {
                KillTimer(hWnd, TIMER_COUNTDOWN);
                if (g_test.IsRunning()) {
                    g_test.StopTest(false); // User-initiated stop is an aborted run, not a score.
                }
                ShowScreen(AppScreen::Results);
            }

            // Try Again
            else if (id == IDC_BTN_TRYAGAIN) {
                g_test.StartTest();
                SendMessage(hTest_Edit, EM_SETLIMITTEXT, (WPARAM)g_test.GetSampleText().size(), 0);
                ShowScreen(AppScreen::Test);
                SetTimer(hWnd, TIMER_COUNTDOWN, 1000, NULL);
            }

            // Certificate Export
            else if (id == IDC_BTN_EXPORT_CERT) {
                PromptAndExportCertificate(hWnd);
            }

            // Change Difficulty / Return to Menu
            else if (id == IDC_BTN_CHANGEDIFF) {
                g_test.RestartTest();
                ShowScreen(AppScreen::Home);
            }

            return 0;
        }

        case WM_DESTROY: {
            KillTimer(hWnd, TIMER_COUNTDOWN);
            CleanThemeGDI();
            PostQuitMessage(0);
            return 0;
        }
    }
    return DefWindowProc(hWnd, msg, wParam, lParam);
}
