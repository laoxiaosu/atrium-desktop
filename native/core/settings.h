// User-adjustable appearance and layout. One flat JSON object, shared with
// Atrium.Settings (settings/Atrium.Settings/BarSettings.cs) and the pipe
// protocol (docs/SETTINGS-PROTOCOL.md). Lengths are in DIP.
#pragma once

#include "shell/task_model.h"

#include <cstdint>
#include <optional>
#include <string>
#include <vector>

namespace atrium {

struct Settings {
    static constexpr int kVersion = 2; // 2: Windows notifications on by default (v1 files are migrated)

    // Material (presets change only these). Liquid follows macOS Liquid Glass:
    // nearly clear centre, strong lensing over a wide rim, visible dispersion.
    std::wstring preset = L"liquid"; // liquid | frosted | custom (informational)
    double transparency = 0.95;      // 0 = fully tinted, 1 = clear glass
    double blur = 0.50;              // 0 = sharp backdrop, 1 = fully frosted
    double refraction = 30;          // max sample displacement at the rim
    double edge = 24;                // width of the refracting rim
    double dispersion = 0.30;        // relative channel spread of the refraction
    double highlight = 0.80;
    double shadow = 0.30;
    uint32_t tint = 0xFFFFFF;        // 0xRRGGBB

    // Layout and content.
    double cornerRadius = 30;        // clamped to half the bar height when drawn
    double width = 1200;             // maximum width when autoWidth
    double height = 60;
    double margin = 10;              // gap above the bottom of the work area
    double iconSize = 30;
    double magnify = 1.7;            // dock magnification under the pointer: icon scale, 1 = off
    bool autoWidth = true;           // shrink to the content
    bool showClock = true;
    bool showWeekNumber = false;
    std::wstring weekNumberRule = L"iso"; // iso | locale
    bool showTray = true;
    bool showStatus = true;
    bool showNotifications = true;
    bool showRunning = true;
    std::wstring textColor = L"auto"; // auto | light | dark
    // Notifications (Atrium's own cards above the bell).
    double notifyTimeout = 6;        // seconds a popup stays (3..30)
    bool notifyHideBody = false;     // privacy: no message text; cards excluded from screen capture
    bool notifyListener = true;      // Windows notifications as Atrium cards (UserNotificationListener)
    bool notifyFeishu = true;        // Feishu messages as Atrium cards (its own popup, read through UI Automation)
    bool notifyWeChat = true;        // WeChat messages as Atrium cards (its chat list, read through UI Automation)
    // Three independent modules: the bar (with its popups and Atrium's notification cards), the
    // desktop widgets and Stage Manager. Any of them may run alone.
    bool bar = true;                 // the glass bar; off: Windows' own taskbar and banners stay in charge
    bool widgets = true;             // desktop widgets (their layout is widgets.json)
    bool widgetsOnRight = false;     // mirror the logical grid without rewriting widgets.json
    std::wstring widgetBackdrop = L"desktop"; // the widgets' glass: desktop (captured live, D38) | wallpaper (the files, least work)
    // Stage Manager (ui/stage_manager.h): the app in front on the stage, the others in a strip at the side.
    bool stageManager = false;
    bool stageOnRight = false;       // the strip's side; left by default (the widgets take the other side)
    bool stageAutoHide = false;      // the strip only comes out while the pointer rests on its edge of the screen
    // The widgets' material: glass (the bar's, its ink adapting) | dusk (the glass dimmed, white ink;
    // macOS's dimmed desktop widgets) | color (their own opaque faces, as macOS's full-colour widgets).
    std::wstring widgetMaterial = L"glass";
    // Windows' own shell (shell/system_chrome.h): put back when turned off or on exit.
    bool hideTaskbar = false;        // the system taskbar is hidden while Atrium runs
    bool hideBanners = false;        // Do Not Disturb: no system banners, the notifications still become cards
    bool startOnLogin = true;        // independent of starting/stopping the current session
    // Pinned apps; nullopt = never saved yet (first run imports the Windows pins).
    std::optional<std::vector<PinnedApp>> pins;

    // Clamps every value into its valid range (input from disk or the pipe).
    void Clamp();

    std::wstring ToJson() const;
    // Missing keys keep the values of `base`; malformed JSON returns false.
    static bool FromJson(const std::wstring& json, const Settings& base, Settings& out);
};

// %LOCALAPPDATA%\Atrium\settings.json
std::wstring SettingsPath();
// UTF-8 text files (a BOM is skipped); the write replaces the file at once.
std::wstring ReadTextFile(const std::wstring& path);
bool WriteTextFile(const std::wstring& path, const std::wstring& text);
Settings LoadSettings();
// True means the file was saved. A failed startup change is reported separately;
// callers must accept the saved snapshot even when warning is nonempty.
bool SaveSettings(const Settings& s, std::wstring* warning = nullptr);
bool IsDevelopmentBuild();
// The current user's Windows logon entry, pointing at this core executable.
bool SetStartOnLogin(bool enabled);
// The Run entry turned off in Task Manager's startup apps.
bool StartupTurnedOff();

} // namespace atrium
