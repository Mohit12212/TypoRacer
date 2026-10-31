#pragma once

#include <windows.h>
#include <string>
#include <vector>
#include <map>

// ── Test Categories ───────────────────────────────────────────────────────────
enum class Category { Prose = 0, Code = 1, Numbers = 2, Survival = 3, Custom = 4 };

// ── Difficulty ─────────────────────────────────────────────────────────────────
enum class Difficulty { Easy = 0, Medium = 1, Hard = 2 };

// ── Screen state ───────────────────────────────────────────────────────────────
enum class AppScreen { Home, Test, Results };

// ── Per-character typing result ────────────────────────────────────────────────
enum class CharStatus { Untouched, Correct, Wrong };

// ── History Entry ──────────────────────────────────────────────────────────────
struct HistoryItem {
    std::wstring timestamp;
    std::wstring category;
    std::wstring difficulty;
    double       wpm         = 0.0;
    double       cpm         = 0.0;
    double       accuracy    = 0.0;
    double       consistency = 0.0;
    int          timeSec     = 0;
    std::wstring grade       = L"D";
};

// ── Test Result metrics ────────────────────────────────────────────────────────
struct TestResult {
    double                     wpm          = 0.0;
    double                     cpm          = 0.0;
    double                     accuracy     = 0.0;
    double                     consistency  = 0.0;
    int                        correctWords = 0;
    int                        wrongWords   = 0;
    int                        backspaces   = 0;
    int                        timeTaken    = 0;
    std::wstring               grade        = L"D";
    std::wstring               aptitudeRank = L"Novice";
    std::wstring               categoryName = L"General Prose";
    bool                       isNewBest    = false;
    bool                       isSavedAttempt = false;
    std::vector<double>        wpmTimeline;
    std::vector<std::pair<wchar_t, int>> problemKeys;
};

// ── TypingTest Core Logic ──────────────────────────────────────────────────────
class TypingTest {
public:
    TypingTest();
    ~TypingTest();

    // ── Setup & Controls ──────────────────────────────────────────────────────
    void SetCategory(Category c);
    void SetDifficulty(Difficulty d);
    void SetCustomText(const std::wstring& customText);
    void SetBotSpeed(int wpm); // 0 = Disabled, 35 = Easy, 60 = Med, 85 = Hard
    void StartTest();
    void StopTest(bool saveAttempt = true);
    void RestartTest();
    void TogglePause();
    void RecordBackspace();

    // ── Real-time Feed ────────────────────────────────────────────────────────
    void UpdateInput(const std::wstring& typed);
    void Tick(); // 1-second timer tick

    // ── Queries ───────────────────────────────────────────────────────────────
    Category                    GetCategory()        const { return m_category; }
    Difficulty                  GetDifficulty()      const { return m_difficulty; }
    AppScreen                   GetScreen()          const { return m_screen; }
    const std::wstring&         GetSampleText()      const { return m_sampleText; }
    const std::wstring&         GetTypedText()       const { return m_typedText; }
    int                         GetTimeRemaining()   const { return m_timeRemaining; }
    int                         GetTotalTime()       const { return m_totalTime; }
    int                         GetElapsedSeconds()  const { return m_totalTime - m_timeRemaining; }
    double                      GetLiveWPM()         const { return m_liveWPM; }
    double                      GetLiveCPM()         const { return m_liveCPM; }
    double                      GetLiveAccuracy()    const { return m_liveAccuracy; }
    double                      GetLiveConsistency() const { return m_liveConsistency; }
    int                         GetBackspaces()      const { return m_backspaces; }
    bool                        IsRunning()          const { return m_running; }
    bool                        IsPaused()           const { return m_paused; }
    bool                        IsFinished()         const { return m_finished; }
    bool                        IsSurvivalFailed()   const { return m_survivalFailed; }

    // ── Ghost Racer AI ─────────────────────────────────────────────────────────
    int                         GetBotSpeed()        const { return m_botWpm; }
    double                      GetPlayerProgress()  const;
    double                      GetBotProgress()     const;
    int                         GetRaceLeadChars()   const;

    // ── Next Key Query for Keyboard Highlighting ──────────────────────────────
    wchar_t                     GetNextChar()        const;

    double                      GetHighScore(Category c, Difficulty d) const;
    double                      GetCurrentHighScore() const { return GetHighScore(m_category, m_difficulty); }

    const std::vector<CharStatus>& GetCharStatuses() const { return m_charStatuses; }
    const TestResult&           GetResult()          const { return m_result; }
    const std::vector<HistoryItem>& GetHistory()     const { return m_history; }

    // ── Persistence & Reports ──────────────────────────────────────────────────
    void LoadScores();
    void SaveScores();
    void LoadHistory();
    void SaveHistory();
    void AppendHistory(const TestResult& res);
    void ClearHistory();
    bool ExportCertificateHTML(const std::wstring& studentName, std::wstring& outPath);

private:
    void LoadSampleText();
    void ComputeStats();
    void ComputeConsistency();
    std::wstring ComputeGrade(double wpm, double accuracy, double consistency) const;
    std::wstring ComputeAptitudeRank(double wpm, double accuracy) const;
    std::wstring GetScoreFilePath() const;
    std::wstring GetHistoryFilePath() const;

    Category                m_category;
    Difficulty              m_difficulty;
    AppScreen               m_screen;
    std::wstring            m_sampleText;
    std::wstring            m_typedText;
    std::wstring            m_customText;
    std::vector<CharStatus> m_charStatuses;

    int    m_totalTime;
    int    m_timeRemaining;
    int    m_backspaces;
    bool   m_running;
    bool   m_paused;
    bool   m_finished;
    bool   m_survivalFailed;

    double m_liveWPM;
    double m_liveCPM;
    double m_liveAccuracy;
    double m_liveConsistency;

    // Ghost Bot Racer
    int    m_botWpm; // 0 = off, 35, 60, 85

    // Keystroke Latency Dynamics
    DWORD                   m_lastKeystrokeTime;
    DWORD                   m_lastClockTick;
    double                  m_elapsedMs; // Active time only; excludes pauses
    std::vector<double>     m_keystrokeLatencies;
    std::map<wchar_t, int>  m_errorKeyMap;
    std::vector<double>     m_wpmTimeline;

    // High scores indexed by Category * 3 + Difficulty
    double m_highScores[15];

    TestResult              m_result;
    std::vector<HistoryItem> m_history;

    // Multi-category Pools
    static const std::vector<std::wstring> PROSE_EASY_TEXTS;
    static const std::vector<std::wstring> PROSE_MEDIUM_TEXTS;
    static const std::vector<std::wstring> PROSE_HARD_TEXTS;

    static const std::vector<std::wstring> CODE_CPP_TEXTS;
    static const std::vector<std::wstring> CODE_PYTHON_TEXTS;
    static const std::vector<std::wstring> CODE_SQL_TEXTS;
    static const std::vector<std::wstring> CODE_JS_TEXTS;

    static const std::vector<std::wstring> NUMBER_SYMBOL_TEXTS;
    static const std::vector<std::wstring> SURVIVAL_TEXTS;
};
