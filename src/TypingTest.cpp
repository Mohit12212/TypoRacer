#include "TypingTest.h"
#include <sstream>
#include <fstream>
#include <algorithm>
#include <cstdlib>
#include <ctime>
#include <cmath>
#include <iomanip>

// ── Multi-Paragraph & Category Pools ──────────────────────────────────────────
const std::vector<std::wstring> TypingTest::PROSE_EASY_TEXTS = {
    L"The quick brown fox jumps over the lazy dog in the sunny park. "
    L"Birds were singing sweet melodies high up in the green branches of the oak tree. "
    L"A gentle warm breeze whispered through the quiet garden filled with colorful flowers.",

    L"Learning to type quickly and accurately is an essential computer aptitude skill. "
    L"Proper posture and steady finger rhythm allow you to type smoothly without looking. "
    L"Daily practice builds natural muscle memory across all rows of the keyboard.",

    L"The morning sunlight streamed through the window as coffee brewed in the kitchen. "
    L"Children gathered near the playground to play games and share joyful laughter. "
    L"Every great journey begins with a single step taken with confidence and purpose."
};

const std::vector<std::wstring> TypingTest::PROSE_MEDIUM_TEXTS = {
    L"Computer programming is the craft of transforming logical concepts into "
    L"precise executable instructions. Skilled software engineers write modular "
    L"code to solve complicated problems and design modern platforms that empower "
    L"millions of global users across diverse industries every single day.",

    L"The widespread adoption of cloud computing and ubiquitous mobile devices "
    L"has revolutionized global collaboration. Teams located across continents "
    L"can synchronize repositories, manage serverless workflows, and deliver "
    L"scalable applications with exceptional velocity and dependable uptime.",

    L"Artificial intelligence and deep machine learning algorithms continue to "
    L"transform technology by discovering non-linear patterns across large data "
    L"sets. Automation optimizes repetitive processes while opening up exciting "
    L"avenues for scientific research and creative problem solving."
};

const std::vector<std::wstring> TypingTest::PROSE_HARD_TEXTS = {
    L"The Byzantine Generals problem addresses fundamental consensus challenges "
    L"in decentralized distributed computing architectures. Asynchronous nodes "
    L"must reliably establish deterministic state transitions despite arbitrary, "
    L"malicious network partitions, out-of-order packets, and transient node faults.",

    L"Quantum superposition enables quantum bits to represent probabilistic linear "
    L"combinations of orthogonal basis states simultaneously. Cryptographic paradigms "
    L"relying upon computational intractability face unprecedented scrutiny, fostering "
    L"the rapid exploration and standardization of post-quantum lattice cryptography.",

    L"Polymorphism in object-oriented programming decouples client interfaces from "
    L"underlying concrete implementations via virtual dispatch tables. Simultaneously, "
    L"fine-grained memory management and lock-free concurrency require meticulous "
    L"cache line alignment to mitigate false sharing across multi-core processors."
};

const std::vector<std::wstring> TypingTest::CODE_CPP_TEXTS = {
    L"#include <iostream>\n#include <vector>\n#include <algorithm>\n\n"
    L"template <typename T>\n"
    L"int binarySearch(const std::vector<T>& arr, T target) {\n"
    L"    int low = 0, high = (int)arr.size() - 1;\n"
    L"    while (low <= high) {\n"
    L"        int mid = low + (high - low) / 2;\n"
    L"        if (arr[mid] == target) return mid;\n"
    L"        if (arr[mid] < target) low = mid + 1;\n"
    L"        else high = mid - 1;\n"
    L"    }\n"
    L"    return -1;\n"
    L"}",

    L"#include <memory>\n#include <string>\n\n"
    L"class DatabaseConnection {\n"
    L"private:\n"
    L"    std::string connectionString;\n"
    L"    bool isConnected;\n"
    L"public:\n"
    L"    DatabaseConnection(const std::string& conn) : connectionString(conn), isConnected(true) {}\n"
    L"    bool executeQuery(const std::string& query) {\n"
    L"        return isConnected && !query.empty();\n"
    L"    }\n"
    L"};"
};

const std::vector<std::wstring> TypingTest::CODE_PYTHON_TEXTS = {
    L"def quick_sort(arr):\n"
    L"    if len(arr) <= 1:\n"
    L"        return arr\n"
    L"    pivot = arr[len(arr) // 2]\n"
    L"    left = [x for x in arr if x < pivot]\n"
    L"    middle = [x for x in arr if x == pivot]\n"
    L"    right = [x for x in arr if x > pivot]\n"
    L"    return quick_sort(left) + middle + quick_sort(right)",

    L"import json\nimport math\n\n"
    L"class DataPipeline:\n"
    L"    def __init__(self, data_source):\n"
    L"        self.source = data_source\n"
    L"        self.records = []\n\n"
    L"    def process_records(self, threshold=0.85):\n"
    L"        return [r for r in self.records if r.get('score', 0) >= threshold]"
};

const std::vector<std::wstring> TypingTest::CODE_SQL_TEXTS = {
    L"SELECT u.user_id, u.username, COUNT(o.order_id) AS total_orders, "
    L"SUM(o.total_amount) AS lifetime_value\n"
    L"FROM users u\n"
    L"JOIN orders o ON u.user_id = o.customer_id\n"
    L"WHERE u.is_active = 1 AND o.created_at >= '2026-01-01'\n"
    L"GROUP BY u.user_id, u.username\n"
    L"HAVING SUM(o.total_amount) > 1500.00\n"
    L"ORDER BY lifetime_value DESC LIMIT 50;",

    L"CREATE TABLE system_logs (\n"
    L"    log_id INT AUTO_INCREMENT PRIMARY KEY,\n"
    L"    service_name VARCHAR(64) NOT NULL,\n"
    L"    severity ENUM('DEBUG', 'INFO', 'WARN', 'ERROR') DEFAULT 'INFO',\n"
    L"    payload JSON,\n"
    L"    recorded_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP\n"
    L");"
};

const std::vector<std::wstring> TypingTest::CODE_JS_TEXTS = {
    L"async function fetchApiResponse(endpoint, options = {}) {\n"
    L"    try {\n"
    L"        const res = await fetch(endpoint, {\n"
    L"            method: options.method || 'GET',\n"
    L"            headers: { 'Content-Type': 'application/json' }\n"
    L"        });\n"
    L"        if (!res.ok) throw new Error(`HTTP Error: ${res.status}`);\n"
    L"        return await res.json();\n"
    L"    } catch (err) {\n"
    L"        console.error('Network request failed:', err);\n"
    L"        return null;\n"
    L"    }\n"
    L"}"
};

const std::vector<std::wstring> TypingTest::NUMBER_SYMBOL_TEXTS = {
    L"IPv4: 192.168.1.254 | Port: 8080 | Subnet: 255.255.255.0 | Gateway: 10.0.0.1\n"
    L"Math: (alpha * 3.14159) / (beta + 1.414e-3) >= [x & 0xFF00] ^ 0x0F;\n"
    L"Regex: ^[a-zA-Z0-9._%+-]+@[a-zA-Z0-9.-]+\\.[a-zA-Z]{2,6}$",

    L"Account #8492-4910-3819 | Balance: $14,920.50 (USD) | Rate: +5.75%\n"
    L"Tokens: { @user_id: 98124, key: \"aB3#x9$K!\", expires_in: 3600s }\n"
    L"Matrix: [[1, 0, 0], [0, 1, 0], [0, 0, 1]] * [x, y, z]^T = [0, 0, 0];"
};

const std::vector<std::wstring> TypingTest::SURVIVAL_TEXTS = {
    L"Survival Mode demands relentless concentration and flawless keystroke precision. "
    L"Do not allow panic to degrade your rhythm. Every misplaced character brings immediate "
    L"failure. Breathe calmly, focus your eyes on the upcoming line, and strike each key cleanly.",

    L"In mission-critical software engineering, an unhandled exception or misplaced semicolon "
    L"can compromise an entire distributed infrastructure. Precision under pressure is what "
    L"separates amateur coders from elite systems architects. Maintain flawless speed now."
};

// ── Constructor ────────────────────────────────────────────────────────────────
TypingTest::TypingTest()
    : m_category(Category::Prose)
    , m_difficulty(Difficulty::Medium)
    , m_screen(AppScreen::Home)
    , m_totalTime(45)
    , m_timeRemaining(45)
    , m_backspaces(0)
    , m_running(false)
    , m_paused(false)
    , m_finished(false)
    , m_survivalFailed(false)
    , m_liveWPM(0.0)
    , m_liveCPM(0.0)
    , m_liveAccuracy(100.0)
    , m_liveConsistency(100.0)
    , m_botWpm(60)
    , m_lastKeystrokeTime(0)
    , m_lastClockTick(0)
    , m_elapsedMs(0.0)
{
    for (int i = 0; i < 15; ++i) {
        m_highScores[i] = 0.0;
    }

    std::srand((unsigned int)std::time(nullptr));
    LoadScores();
    LoadHistory();
    LoadSampleText();
}

TypingTest::~TypingTest() {
    SaveScores();
    SaveHistory();
}

// ── Score File Path ────────────────────────────────────────────────────────────
std::wstring TypingTest::GetScoreFilePath() const {
    wchar_t exePath[MAX_PATH];
    GetModuleFileNameW(NULL, exePath, MAX_PATH);
    std::wstring path(exePath);
    size_t pos = path.find_last_of(L"\\/");
    if (pos != std::wstring::npos) {
        return path.substr(0, pos + 1) + L"scores.dat";
    }
    return L"scores.dat";
}

std::wstring TypingTest::GetHistoryFilePath() const {
    wchar_t exePath[MAX_PATH];
    GetModuleFileNameW(NULL, exePath, MAX_PATH);
    std::wstring path(exePath);
    size_t pos = path.find_last_of(L"\\/");
    if (pos != std::wstring::npos) {
        return path.substr(0, pos + 1) + L"history.csv";
    }
    return L"history.csv";
}

void TypingTest::LoadScores() {
    std::wifstream fin(GetScoreFilePath().c_str());
    if (!fin.is_open()) return;
    for (int i = 0; i < 15; ++i) {
        if (!(fin >> m_highScores[i])) break;
    }
}

void TypingTest::SaveScores() {
    std::wofstream fout(GetScoreFilePath().c_str());
    if (!fout.is_open()) return;
    for (int i = 0; i < 15; ++i) {
        fout << m_highScores[i] << L"\n";
    }
}

void TypingTest::LoadHistory() {
    m_history.clear();
    std::wifstream fin(GetHistoryFilePath().c_str());
    if (!fin.is_open()) return;

    std::wstring line;
    // Skip CSV header if present
    if (std::getline(fin, line)) {
        if (line.find(L"Timestamp") != std::wstring::npos) {
            // Header line, proceed to data
        } else {
            // Data line without header
            std::wstringstream ss(line);
            HistoryItem item;
            std::wstring token;
            if (std::getline(ss, item.timestamp, L',')) {
                std::getline(ss, item.category, L',');
                std::getline(ss, item.difficulty, L',');
                if (std::getline(ss, token, L',')) item.wpm = std::wcstod(token.c_str(), nullptr);
                if (std::getline(ss, token, L',')) item.cpm = std::wcstod(token.c_str(), nullptr);
                if (std::getline(ss, token, L',')) item.accuracy = std::wcstod(token.c_str(), nullptr);
                if (std::getline(ss, token, L',')) item.consistency = std::wcstod(token.c_str(), nullptr);
                if (std::getline(ss, token, L',')) item.timeSec = (int)std::wcstol(token.c_str(), nullptr, 10);
                std::getline(ss, item.grade, L',');
                m_history.push_back(item);
            }
        }
    }

    while (std::getline(fin, line)) {
        if (line.empty()) continue;
        std::wstringstream ss(line);
        HistoryItem item;
        std::wstring token;
        if (std::getline(ss, item.timestamp, L',')) {
            std::getline(ss, item.category, L',');
            std::getline(ss, item.difficulty, L',');
            if (std::getline(ss, token, L',')) item.wpm = std::wcstod(token.c_str(), nullptr);
            if (std::getline(ss, token, L',')) item.cpm = std::wcstod(token.c_str(), nullptr);
            if (std::getline(ss, token, L',')) item.accuracy = std::wcstod(token.c_str(), nullptr);
            if (std::getline(ss, token, L',')) item.consistency = std::wcstod(token.c_str(), nullptr);
            if (std::getline(ss, token, L',')) item.timeSec = (int)std::wcstol(token.c_str(), nullptr, 10);
            std::getline(ss, item.grade, L',');
            m_history.push_back(item);
        }
    }
}

void TypingTest::SaveHistory() {
    std::wofstream fout(GetHistoryFilePath().c_str());
    if (!fout.is_open()) return;

    fout << L"Timestamp,Category,Difficulty,WPM,CPM,Accuracy,Consistency,Duration,Grade\n";
    for (const auto& item : m_history) {
        fout << item.timestamp << L","
             << item.category << L","
             << item.difficulty << L","
             << std::fixed << std::setprecision(1) << item.wpm << L","
             << std::fixed << std::setprecision(0) << item.cpm << L","
             << std::fixed << std::setprecision(1) << item.accuracy << L","
             << std::fixed << std::setprecision(1) << item.consistency << L","
             << item.timeSec << L","
             << item.grade << L"\n";
    }
}

void TypingTest::AppendHistory(const TestResult& res) {
    time_t rawtime;
    struct tm timeinfo;
    time(&rawtime);
    localtime_s(&timeinfo, &rawtime);

    wchar_t timeBuf[64];
    wcsftime(timeBuf, 64, L"%Y-%m-%d %H:%M", &timeinfo);

    HistoryItem item;
    item.timestamp   = timeBuf;
    item.category    = res.categoryName;
    item.wpm         = res.wpm;
    item.cpm         = res.cpm;
    item.accuracy    = res.accuracy;
    item.consistency = res.consistency;
    item.timeSec     = res.timeTaken;
    item.grade       = res.grade;

    switch (m_difficulty) {
        case Difficulty::Easy:   item.difficulty = L"Easy"; break;
        case Difficulty::Medium: item.difficulty = L"Medium"; break;
        case Difficulty::Hard:   item.difficulty = L"Hard"; break;
    }

    m_history.push_back(item);
    SaveHistory();
}

void TypingTest::ClearHistory() {
    m_history.clear();
    SaveHistory();
}

double TypingTest::GetHighScore(Category c, Difficulty d) const {
    int idx = static_cast<int>(c) * 3 + static_cast<int>(d);
    if (idx >= 0 && idx < 15) return m_highScores[idx];
    return 0.0;
}

// ── Setup Controls ─────────────────────────────────────────────────────────────
void TypingTest::SetCategory(Category c) {
    if (m_running) return;
    m_category = c;
    switch (m_category) {
        case Category::Prose:
            SetDifficulty(m_difficulty);
            break;
        case Category::Code:
            m_totalTime = 60;
            break;
        case Category::Numbers:
            m_totalTime = 45;
            break;
        case Category::Survival:
            m_totalTime = 45;
            break;
        case Category::Custom:
            m_totalTime = 60;
            break;
    }
    m_timeRemaining = m_totalTime;
    LoadSampleText();
}

void TypingTest::SetDifficulty(Difficulty d) {
    if (m_running) return;
    m_difficulty = d;
    if (m_category == Category::Prose) {
        switch (m_difficulty) {
            case Difficulty::Easy:   m_totalTime = 60; break;
            case Difficulty::Medium: m_totalTime = 45; break;
            case Difficulty::Hard:   m_totalTime = 30; break;
        }
    }
    m_timeRemaining = m_totalTime;
    LoadSampleText();
}

void TypingTest::SetCustomText(const std::wstring& customText) {
    m_customText = customText;
    m_category   = Category::Custom;
    m_totalTime  = 60;
    m_timeRemaining = 60;
    LoadSampleText();
}

void TypingTest::SetBotSpeed(int wpm) {
    m_botWpm = wpm;
}

// ── Paragraph Selection ────────────────────────────────────────────────────────
void TypingTest::LoadSampleText() {
    if (m_category == Category::Custom && !m_customText.empty()) {
        m_sampleText = m_customText;
    } else if (m_category == Category::Code) {
        const std::vector<std::wstring>* pools[] = { &CODE_CPP_TEXTS, &CODE_PYTHON_TEXTS, &CODE_SQL_TEXTS, &CODE_JS_TEXTS };
        int poolIdx = std::rand() % 4;
        int pick = std::rand() % pools[poolIdx]->size();
        m_sampleText = (*pools[poolIdx])[pick];
    } else if (m_category == Category::Numbers) {
        int pick = std::rand() % NUMBER_SYMBOL_TEXTS.size();
        m_sampleText = NUMBER_SYMBOL_TEXTS[pick];
    } else if (m_category == Category::Survival) {
        int pick = std::rand() % SURVIVAL_TEXTS.size();
        m_sampleText = SURVIVAL_TEXTS[pick];
    } else {
        const std::vector<std::wstring>* pool = nullptr;
        switch (m_difficulty) {
            case Difficulty::Easy:   pool = &PROSE_EASY_TEXTS;   break;
            case Difficulty::Medium: pool = &PROSE_MEDIUM_TEXTS; break;
            case Difficulty::Hard:   pool = &PROSE_HARD_TEXTS;   break;
            default:                 pool = &PROSE_MEDIUM_TEXTS; break;
        }
        int pick = std::rand() % pool->size();
        m_sampleText = (*pool)[pick];
    }
    m_charStatuses.assign(m_sampleText.size(), CharStatus::Untouched);
}

// ── Test Lifecycle ─────────────────────────────────────────────────────────────
void TypingTest::StartTest() {
    LoadSampleText();
    m_result = TestResult{};
    m_typedText.clear();
    m_charStatuses.assign(m_sampleText.size(), CharStatus::Untouched);
    m_timeRemaining      = m_totalTime;
    m_backspaces         = 0;
    m_liveWPM            = 0.0;
    m_liveCPM            = 0.0;
    m_liveAccuracy       = 100.0;
    m_liveConsistency    = 100.0;
    m_running            = true;
    m_paused             = false;
    m_finished           = false;
    m_survivalFailed     = false;
    m_lastKeystrokeTime  = 0;
    m_elapsedMs          = 0.0;
    m_lastClockTick      = GetTickCount();
    m_keystrokeLatencies.clear();
    m_errorKeyMap.clear();
    m_wpmTimeline.clear();
    m_wpmTimeline.push_back(0.0);
    m_screen             = AppScreen::Test;
}

void TypingTest::StopTest(bool saveAttempt) {
    if (!m_running) return;
    if (!m_paused) {
        DWORD now = GetTickCount();
        m_elapsedMs += static_cast<DWORD>(now - m_lastClockTick);
        m_lastClockTick = now;
    }
    m_running  = false;
    m_paused   = false;
    m_finished = true;
    ComputeStats();

    // Only persist credible attempts. This prevents accidental clicks, aborted
    // runs and sub-second input from corrupting records or the session ledger.
    const bool credible = saveAttempt && m_typedText.size() >= 5 && m_elapsedMs >= 3000.0 &&
                          m_result.wpm <= 300.0 && m_result.accuracy >= 0.0 &&
                          m_result.accuracy <= 100.0 && !m_survivalFailed;
    m_result.isNewBest = false;
    m_result.isSavedAttempt = credible;
    if (credible) {
        int idx = static_cast<int>(m_category) * 3 + static_cast<int>(m_difficulty);
        if (idx >= 0 && idx < 15 && m_result.wpm > m_highScores[idx]) {
            m_highScores[idx] = m_result.wpm;
            m_result.isNewBest = true;
            SaveScores();
        }
        AppendHistory(m_result);
    }
    m_screen = AppScreen::Results;
}

void TypingTest::RestartTest() {
    m_screen          = AppScreen::Home;
    m_running         = false;
    m_paused          = false;
    m_finished        = false;
    m_survivalFailed  = false;
    m_typedText.clear();
    m_charStatuses.assign(m_sampleText.size(), CharStatus::Untouched);
    m_timeRemaining   = m_totalTime;
    m_backspaces      = 0;
    m_liveWPM         = 0.0;
    m_liveCPM         = 0.0;
    m_liveAccuracy    = 100.0;
    m_liveConsistency = 100.0;
    m_wpmTimeline.clear();
}

void TypingTest::TogglePause() {
    if (!m_running) return;
    DWORD now = GetTickCount();
    if (!m_paused) {
        m_elapsedMs += static_cast<DWORD>(now - m_lastClockTick);
        m_paused = true;
    } else {
        m_lastClockTick = now;
        m_paused = false;
    }
}

void TypingTest::RecordBackspace() {
    if (m_running && !m_paused) {
        ++m_backspaces;
    }
}

// ── Next Key Query ─────────────────────────────────────────────────────────────
wchar_t TypingTest::GetNextChar() const {
    if (m_typedText.size() < m_sampleText.size()) {
        return m_sampleText[m_typedText.size()];
    }
    return 0;
}

// ── Ghost Racer AI Progress ────────────────────────────────────────────────────
double TypingTest::GetPlayerProgress() const {
    if (m_sampleText.empty()) return 0.0;
    double progress = (double)m_typedText.size() / (double)m_sampleText.size();
    if (progress > 1.0) progress = 1.0;
    return progress;
}

double TypingTest::GetBotProgress() const {
    if (m_botWpm <= 0 || m_sampleText.empty()) return 0.0;
    int elapsed = GetElapsedSeconds();
    // bot speed in characters per second = (botWPM * 5) / 60
    double botChars = (elapsed * m_botWpm * 5.0) / 60.0;
    double progress = botChars / (double)m_sampleText.size();
    if (progress > 1.0) progress = 1.0;
    return progress;
}

int TypingTest::GetRaceLeadChars() const {
    if (m_botWpm <= 0) return 0;
    int elapsed = GetElapsedSeconds();
    double botChars = (elapsed * m_botWpm * 5.0) / 60.0;
    return (int)m_typedText.size() - (int)botChars;
}

// ── Keystroke Update ───────────────────────────────────────────────────────────
void TypingTest::UpdateInput(const std::wstring& typed) {
    if (!m_running || m_paused) return;

    DWORD now = GetTickCount();
    if (m_lastKeystrokeTime > 0) {
        double latency = (double)(now - m_lastKeystrokeTime);
        if (latency >= 10.0 && latency <= 2500.0) {
            m_keystrokeLatencies.push_back(latency);
        }
    }
    m_lastKeystrokeTime = now;

    m_typedText = typed;
    size_t sampleLen = m_sampleText.size();
    size_t typedLen  = typed.size();

    m_charStatuses.assign(sampleLen, CharStatus::Untouched);
    m_errorKeyMap.clear(); // Analyze the current text, not every historical redraw.
    int currentErrors = 0;

    for (size_t i = 0; i < typedLen && i < sampleLen; ++i) {
        if (typed[i] == m_sampleText[i]) {
            m_charStatuses[i] = CharStatus::Correct;
        } else {
            m_charStatuses[i] = CharStatus::Wrong;
            ++currentErrors;
            // Record error character key
            m_errorKeyMap[m_sampleText[i]]++;
        }
    }

    ComputeConsistency();
    ComputeStats();

    // Sudden Death Rule (only trigger after significant characters and low accuracy)
    if (m_category == Category::Survival) {
        if (typedLen >= 20 && (m_liveAccuracy < 90.0 || currentErrors >= 8)) {
            m_survivalFailed = true;
            StopTest();
            return;
        }
    }

    if (typedLen >= sampleLen) {
        StopTest();
    }
}

// ── Timer Tick ─────────────────────────────────────────────────────────────────
void TypingTest::Tick() {
    if (!m_running || m_paused) return;
    DWORD now = GetTickCount();
    m_elapsedMs += static_cast<DWORD>(now - m_lastClockTick);
    m_lastClockTick = now;
    int elapsedWhole = static_cast<int>(m_elapsedMs / 1000.0);
    m_timeRemaining = (std::max)(0, m_totalTime - elapsedWhole);
    ComputeConsistency();
    ComputeStats();
    m_wpmTimeline.push_back(m_liveWPM);
    if (m_timeRemaining == 0) StopTest();
}

// ── Consistency Computation ───────────────────────────────────────────────────
void TypingTest::ComputeConsistency() {
    if (m_keystrokeLatencies.size() < 4) {
        m_liveConsistency = 100.0;
        return;
    }

    double sum = 0.0;
    for (double lat : m_keystrokeLatencies) sum += lat;
    double mean = sum / m_keystrokeLatencies.size();

    double varSum = 0.0;
    for (double lat : m_keystrokeLatencies) {
        double diff = lat - mean;
        varSum += diff * diff;
    }
    double stddev = std::sqrt(varSum / m_keystrokeLatencies.size());
    double cv = (mean > 0.0) ? (stddev / mean) : 0.0;

    // Consistency rating: standard touch typists have low coefficient of variation
    double rating = 100.0 - (cv * 65.0);
    if (rating < 15.0)  rating = 15.0;
    if (rating > 100.0) rating = 100.0;
    m_liveConsistency = rating;
}

// ── Statistics Computation ─────────────────────────────────────────────────────
void TypingTest::ComputeStats() {
    double elapsedMs = m_elapsedMs;
    if (m_running && !m_paused && m_lastClockTick != 0) {
        elapsedMs += static_cast<DWORD>(GetTickCount() - m_lastClockTick);
    }
    // Keep live WPM responsive but never divide by zero during the first keystroke.
    if (elapsedMs < 1000.0) elapsedMs = 1000.0;
    int elapsed = (std::max)(1, static_cast<int>(std::ceil(elapsedMs / 1000.0)));

    int correctChars = 0;
    int totalTyped   = static_cast<int>(m_typedText.size());
    int sampleLen    = static_cast<int>(m_sampleText.size());

    for (int i = 0; i < totalTyped && i < sampleLen; ++i) {
        if (m_typedText[i] == m_sampleText[i]) {
            ++correctChars;
        }
    }

    double minutes = elapsedMs / 60000.0;
    if (minutes > 0.0) {
        m_liveWPM = (correctChars / 5.0) / minutes;
        m_liveCPM = correctChars / minutes;
    } else {
        m_liveWPM = 0.0;
        m_liveCPM = 0.0;
    }

    m_liveAccuracy = (totalTyped > 0)
                     ? (static_cast<double>(correctChars) * 100.0 / totalTyped)
                     : 100.0;

    auto splitWords = [](const std::wstring& text) -> std::vector<std::wstring> {
        std::vector<std::wstring> words;
        std::wistringstream ss(text);
        std::wstring w;
        while (ss >> w) words.push_back(w);
        return words;
    };

    auto sampleWords = splitWords(m_sampleText);
    auto typedWords  = splitWords(m_typedText);

    int correctWords = 0, wrongWords = 0;
    int compareLen = static_cast<int>(std::min(sampleWords.size(), typedWords.size()));
    for (int i = 0; i < compareLen; ++i) {
        if (typedWords[i] == sampleWords[i]) {
            ++correctWords;
        } else {
            ++wrongWords;
        }
    }
    if (static_cast<int>(typedWords.size()) > compareLen) {
        wrongWords += static_cast<int>(typedWords.size()) - compareLen;
    }

    std::wstring catName = L"Prose Typing";
    switch (m_category) {
        case Category::Prose:    catName = L"General Prose"; break;
        case Category::Code:     catName = L"Code Aptitude"; break;
        case Category::Numbers:  catName = L"Numeric & Symbols"; break;
        case Category::Survival: catName = L"Survival Sprint"; break;
        case Category::Custom:   catName = L"Custom Practice"; break;
    }

    m_result.wpm          = m_liveWPM;
    m_result.cpm          = m_liveCPM;
    m_result.accuracy     = m_liveAccuracy;
    m_result.consistency  = m_liveConsistency;
    m_result.correctWords = correctWords;
    m_result.wrongWords   = wrongWords;
    m_result.backspaces   = m_backspaces;
    m_result.timeTaken    = elapsed;
    m_result.categoryName = catName;
    m_result.grade        = ComputeGrade(m_liveWPM, m_liveAccuracy, m_liveConsistency);
    m_result.aptitudeRank = ComputeAptitudeRank(m_liveWPM, m_liveAccuracy);
    m_result.wpmTimeline  = m_wpmTimeline;

    // Collect Top Problem Keys
    std::vector<std::pair<wchar_t, int>> keyVec(m_errorKeyMap.begin(), m_errorKeyMap.end());
    std::sort(keyVec.begin(), keyVec.end(), [](const std::pair<wchar_t, int>& a, const std::pair<wchar_t, int>& b) {
        return a.second > b.second;
    });
    if (keyVec.size() > 5) keyVec.resize(5);
    m_result.problemKeys = keyVec;
}

// ── Grade Assessment ───────────────────────────────────────────────────────────
std::wstring TypingTest::ComputeGrade(double wpm, double accuracy, double consistency) const {
    if (wpm >= 80.0 && accuracy >= 97.0 && consistency >= 75.0) return L"S+";
    if (wpm >= 65.0 && accuracy >= 95.0) return L"A";
    if (wpm >= 50.0 && accuracy >= 90.0) return L"B";
    if (wpm >= 35.0 && accuracy >= 80.0) return L"C";
    return L"D";
}

static std::wstring EscapeHtml(const std::wstring& input) {
    std::wstring out;
    for (wchar_t ch : input) {
        switch (ch) {
            case L'&': out += L"&amp;"; break;
            case L'<': out += L"&lt;"; break;
            case L'>': out += L"&gt;"; break;
            case L'\"': out += L"&quot;"; break;
            case L'\'': out += L"&#39;"; break;
            default: out += ch; break;
        }
    }
    return out;
}

static std::string WideToUtf8(const std::wstring& wstr) {
    if (wstr.empty()) return std::string();
    int size = WideCharToMultiByte(CP_UTF8, 0, wstr.data(), (int)wstr.size(), NULL, 0, NULL, NULL);
    std::string str(size, 0);
    WideCharToMultiByte(CP_UTF8, 0, wstr.data(), (int)wstr.size(), &str[0], size, NULL, NULL);
    return str;
}

std::wstring TypingTest::ComputeAptitudeRank(double wpm, double accuracy) const {
    // These are performance tiers, not population percentiles: no external
    // norming dataset is bundled with this local application.
    if (wpm >= 80.0 && accuracy >= 97.0) return L"Godspeed Master";
    if (wpm >= 65.0 && accuracy >= 95.0) return L"Professional Typist";
    if (wpm >= 50.0 && accuracy >= 90.0) return L"Intermediate Typist";
    if (wpm >= 35.0 && accuracy >= 80.0) return L"Developing Typist";
    return L"Foundational - Regular Practice Advised";
}

// ── Export HTML Certificate ────────────────────────────────────────────────────
bool TypingTest::ExportCertificateHTML(const std::wstring& studentName, std::wstring& outPath) {
    wchar_t exePath[MAX_PATH];
    GetModuleFileNameW(NULL, exePath, MAX_PATH);
    std::wstring path(exePath);
    size_t pos = path.find_last_of(L"\\/");
    std::wstring dir = (pos != std::wstring::npos) ? path.substr(0, pos + 1) : L"";
    outPath = dir + L"Typing_Aptitude_Certificate.html";

    std::ofstream fout(WideToUtf8(outPath).c_str(), std::ios::out | std::ios::binary);
    if (!fout.is_open()) return false;

    time_t rawtime;
    struct tm timeinfo;
    time(&rawtime);
    localtime_s(&timeinfo, &rawtime);
    wchar_t dateStr[64];
    wcsftime(dateStr, 64, L"%B %d, %Y", &timeinfo);

    std::wstring problemKeysStr = L"None (Flawless Keystroke Execution)";
    if (!m_result.problemKeys.empty()) {
        problemKeysStr.clear();
        for (size_t i = 0; i < m_result.problemKeys.size(); ++i) {
            wchar_t k = m_result.problemKeys[i].first;
            if (k == L' ') problemKeysStr += L"'[Space]'";
            else if (k == L'\n') problemKeysStr += L"'[Enter]'";
            else {
                problemKeysStr += L"'";
                problemKeysStr += k;
                problemKeysStr += L"'";
            }
            problemKeysStr += L" (" + std::to_wstring(m_result.problemKeys[i].second) + L" errors)";
            if (i + 1 < m_result.problemKeys.size()) problemKeysStr += L", ";
        }
    }

    std::wstringstream ss;
    ss << L"<!DOCTYPE html>\n"
       << L"<html lang=\"en\">\n<head>\n<meta charset=\"UTF-8\">\n"
       << L"<title>Typing Aptitude Evaluation Certificate</title>\n"
       << L"<style>\n"
       << L"  body { background: #0b0f19; color: #e2e8f0; font-family: 'Segoe UI', Arial, sans-serif; margin: 0; padding: 40px; display: flex; justify-content: center; align-items: center; min-height: 100vh; }\n"
       << L"  .cert-card { background: linear-gradient(145deg, #131b2e, #0e1526); border: 3px solid #00d2ff; border-radius: 20px; box-shadow: 0 20px 60px rgba(0, 210, 255, 0.25); max-width: 900px; width: 100%; padding: 50px 60px; position: relative; box-sizing: border-box; }\n"
       << L"  .header-decor { text-align: center; border-bottom: 2px solid rgba(0, 210, 255, 0.3); padding-bottom: 25px; margin-bottom: 30px; }\n"
       << L"  .inst-title { font-family: Georgia, 'Times New Roman', serif; font-size: 20px; letter-spacing: 4px; color: #94a3b8; text-transform: uppercase; margin: 0 0 8px 0; }\n"
       << L"  .cert-title { font-size: 32px; font-weight: 800; color: #00d2ff; margin: 0; letter-spacing: 1px; text-transform: uppercase; }\n"
       << L"  .cert-sub { color: #64748b; font-size: 14px; margin-top: 6px; letter-spacing: 2px; }\n"
       << L"  .presented-to { text-align: center; margin: 25px 0 10px 0; font-size: 14px; text-transform: uppercase; letter-spacing: 3px; color: #94a3b8; }\n"
       << L"  .student-name { text-align: center; font-family: Georgia, 'Times New Roman', serif; font-size: 38px; color: #38bdf8; font-weight: 700; border-bottom: 2px dashed #334155; padding-bottom: 15px; margin: 0 80px 25px 80px; }\n"
       << L"  .narrative { text-align: center; color: #cbd5e1; font-size: 15px; line-height: 1.6; max-width: 720px; margin: 0 auto 30px auto; }\n"
       << L"  .metrics-grid { display: grid; grid-template-columns: repeat(4, 1fr); gap: 15px; margin-bottom: 30px; }\n"
       << L"  .metric-box { background: rgba(15, 23, 42, 0.7); border: 1px solid #1e293b; border-radius: 12px; padding: 18px 12px; text-align: center; }\n"
       << L"  .metric-val { font-size: 26px; font-weight: 800; color: #00e676; font-family: Consolas, monospace; }\n"
       << L"  .metric-lbl { font-size: 11px; font-weight: 700; text-transform: uppercase; color: #94a3b8; margin-top: 6px; letter-spacing: 1px; }\n"
       << L"  .diag-section { background: rgba(30, 41, 59, 0.4); border-left: 4px solid #00d2ff; border-radius: 8px; padding: 16px 20px; margin-bottom: 35px; }\n"
       << L"  .diag-title { font-weight: 700; font-size: 13px; color: #38bdf8; text-transform: uppercase; letter-spacing: 1px; margin-bottom: 6px; }\n"
       << L"  .diag-body { font-size: 13px; color: #cbd5e1; line-height: 1.5; }\n"
       << L"  .cert-footer { display: flex; justify-content: space-between; align-items: flex-end; border-top: 1px solid #1e293b; padding-top: 25px; }\n"
       << L"  .sign-box { text-align: center; width: 220px; }\n"
       << L"  .sign-line { border-top: 1px solid #475569; margin-top: 40px; padding-top: 6px; font-size: 12px; color: #94a3b8; text-transform: uppercase; letter-spacing: 1px; }\n"
       << L"  .seal-badge { border: 3px double #eab308; border-radius: 50%; width: 90px; height: 90px; display: flex; flex-direction: column; align-items: center; justify-content: center; color: #eab308; text-align: center; font-size: 10px; font-weight: 700; }\n"
       << L"  @media print { body { background: white; color: black; padding: 0; } .cert-card { border: 2px solid #0284c7; box-shadow: none; background: white; color: black; } .student-name { color: #0284c7; } .metric-val { color: #059669; } }\n"
       << L"</style>\n</head>\n<body>\n"
       << L"<div class=\"cert-card\">\n"
       << L"  <div class=\"header-decor\">\n"
       << L"    <p class=\"inst-title\">Department of Computer Science & Aptitude</p>\n"
       << L"    <h1 class=\"cert-title\">Certificate of Typing Aptitude</h1>\n"
       << L"    <p class=\"cert-sub\">STANDARDIZED COMPUTER APTITUDE ASSESSMENT & KEYSTROKE DYNAMICS EVALUATION</p>\n"
       << L"  </div>\n"
       << L"  <div class=\"presented-to\">This is officially awarded to</div>\n"
       << L"  <div class=\"student-name\">" << EscapeHtml(studentName.empty() ? L"Candidate / Student" : studentName) << L"</div>\n"
       << L"  <p class=\"narrative\">\n"
       << L"    For demonstrating exceptional dexterity, accuracy, and keystroke consistency in the standardized\n"
       << L"    <b>" << EscapeHtml(m_result.categoryName) << L"</b> assessment. The candidate attained a performance tier of\n"
       << L"    <b>" << EscapeHtml(m_result.aptitudeRank) << L"</b> with an overall Grade rating of <b>" << EscapeHtml(m_result.grade) << L"</b>.\n"
       << L"  </p>\n"
       << L"  <div class=\"metrics-grid\">\n"
       << L"    <div class=\"metric-box\"><div class=\"metric-val\">" << std::fixed << std::setprecision(1) << m_result.wpm << L"</div><div class=\"metric-lbl\">Net WPM</div></div>\n"
       << L"    <div class=\"metric-box\"><div class=\"metric-val\">" << std::fixed << std::setprecision(0) << m_result.cpm << L"</div><div class=\"metric-lbl\">Char / Min (CPM)</div></div>\n"
       << L"    <div class=\"metric-box\"><div class=\"metric-val\">" << std::fixed << std::setprecision(1) << m_result.accuracy << L"%</div><div class=\"metric-lbl\">Accuracy</div></div>\n"
       << L"    <div class=\"metric-box\"><div class=\"metric-val\">" << std::fixed << std::setprecision(1) << m_result.consistency << L"%</div><div class=\"metric-lbl\">Consistency</div></div>\n"
       << L"  </div>\n"
       << L"  <div class=\"diag-section\">\n"
       << L"    <div class=\"diag-title\">Aptitude Diagnostics & Keystroke Biometrics:</div>\n"
       << L"    <div class=\"diag-body\">\n"
       << L"      <b>Duration:</b> " << m_result.timeTaken << L" seconds | <b>Words Correct:</b> " << m_result.correctWords << L" | <b>Corrections (Backspaces):</b> " << m_result.backspaces << L"<br>\n"
       << L"      <b>Target Improvement Keys:</b> " << EscapeHtml(problemKeysStr) << L"\n"
       << L"    </div>\n"
       << L"  </div>\n"
       << L"  <div class=\"cert-footer\">\n"
       << L"    <div class=\"sign-box\">\n"
       << L"      <div class=\"sign-line\">Date: " << dateStr << L"</div>\n"
       << L"    </div>\n"
       << L"    <div class=\"seal-badge\">\n"
       << L"      <span>★ ★ ★</span>\n"
       << L"      <span style=\"font-size:12px; margin: 3px 0;\">" << m_result.grade << L"</span>\n"
       << L"      <span>VERIFIED</span>\n"
       << L"    </div>\n"
       << L"    <div class=\"sign-box\">\n"
       << L"      <div class=\"sign-line\">Automated Evaluator<br>TypoRacer Pro Engine</div>\n"
       << L"    </div>\n"
       << L"  </div>\n"
       << L"</div>\n"
       << L"</body>\n</html>\n";

    std::string utf8Content = WideToUtf8(ss.str());
    fout.write(utf8Content.data(), utf8Content.size());
    fout.close();

    return true;
}
