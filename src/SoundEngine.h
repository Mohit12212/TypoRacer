#pragma once

#include <windows.h>
#include <mmsystem.h>
#include <vector>
#include <cmath>

#pragma comment(lib, "winmm.lib")

enum class SoundFX {
    KeyClick,
    Spacebar,
    Backspace,
    Error,
    Bell
};

class SoundEngine {
public:
    static SoundEngine& Instance() {
        static SoundEngine inst;
        return inst;
    }

    void SetEnabled(bool enabled) { m_enabled = enabled; }
    bool IsEnabled() const { return m_enabled; }
    void Toggle() { m_enabled = !m_enabled; }

    void Play(SoundFX fx) {
        if (!m_enabled) return;
        const std::vector<BYTE>* pBuf = nullptr;
        switch (fx) {
            case SoundFX::KeyClick:  pBuf = &m_wavClick;     break;
            case SoundFX::Spacebar:  pBuf = &m_wavSpace;     break;
            case SoundFX::Backspace: pBuf = &m_wavBackspace; break;
            case SoundFX::Error:     pBuf = &m_wavError;     break;
            case SoundFX::Bell:      pBuf = &m_wavBell;      break;
        }
        if (pBuf && !pBuf->empty()) {
            PlaySoundW((LPCWSTR)pBuf->data(), NULL, SND_MEMORY | SND_ASYNC | SND_NODEFAULT);
        }
    }

private:
    bool m_enabled;
    std::vector<BYTE> m_wavClick;
    std::vector<BYTE> m_wavSpace;
    std::vector<BYTE> m_wavBackspace;
    std::vector<BYTE> m_wavError;
    std::vector<BYTE> m_wavBell;

    SoundEngine() : m_enabled(true) {
        InitWaveBuffers();
    }

    std::vector<BYTE> BuildWav(const std::vector<short>& samples, int sampleRate = 44100) {
        int dataSize = (int)samples.size() * sizeof(short);
        int chunkSize = 36 + dataSize;
        std::vector<BYTE> wav(44 + dataSize, 0);

        // RIFF Header
        memcpy(&wav[0], "RIFF", 4);
        memcpy(&wav[4], &chunkSize, 4);
        memcpy(&wav[8], "WAVE", 4);

        // fmt chunk
        memcpy(&wav[12], "fmt ", 4);
        int subchunk1Size = 16;
        short audioFormat = 1; // PCM
        short numChannels = 1; // Mono
        int byteRate = sampleRate * 2;
        short blockAlign = 2;
        short bitsPerSample = 16;
        memcpy(&wav[16], &subchunk1Size, 4);
        memcpy(&wav[20], &audioFormat, 2);
        memcpy(&wav[22], &numChannels, 2);
        memcpy(&wav[24], &sampleRate, 4);
        memcpy(&wav[28], &byteRate, 4);
        memcpy(&wav[32], &blockAlign, 2);
        memcpy(&wav[34], &bitsPerSample, 2);

        // data chunk
        memcpy(&wav[36], "data", 4);
        memcpy(&wav[40], &dataSize, 4);
        memcpy(&wav[44], samples.data(), dataSize);

        return wav;
    }

    void InitWaveBuffers() {
        const int sr = 44100;
        const double PI = 3.14159265358979323846;

        // 1. KeyClick (Mechanical Switch Click - 14 ms)
        {
            int n = (int)(sr * 0.014);
            std::vector<short> s(n);
            for (int i = 0; i < n; ++i) {
                double t = (double)i / sr;
                double env = std::exp(-t * 400.0);
                double val = std::sin(2.0 * PI * 1800.0 * t) * 0.7 + std::sin(2.0 * PI * 3200.0 * t) * 0.3;
                s[i] = (short)(val * env * 22000.0);
            }
            m_wavClick = BuildWav(s, sr);
        }

        // 2. Spacebar (Mechanical Thump - 22 ms)
        {
            int n = (int)(sr * 0.022);
            std::vector<short> s(n);
            for (int i = 0; i < n; ++i) {
                double t = (double)i / sr;
                double env = std::exp(-t * 220.0);
                double val = std::sin(2.0 * PI * 650.0 * t) * 0.8 + std::sin(2.0 * PI * 1200.0 * t) * 0.2;
                s[i] = (short)(val * env * 25000.0);
            }
            m_wavSpace = BuildWav(s, sr);
        }

        // 3. Backspace (Double Mechanical Click - 20 ms)
        {
            int n = (int)(sr * 0.020);
            std::vector<short> s(n);
            for (int i = 0; i < n; ++i) {
                double t = (double)i / sr;
                double env1 = std::exp(-t * 500.0);
                double env2 = (t > 0.008) ? std::exp(-(t - 0.008) * 450.0) : 0.0;
                double val = std::sin(2.0 * PI * 1400.0 * t) * env1 + std::sin(2.0 * PI * 2200.0 * t) * env2;
                s[i] = (short)(val * 18000.0);
            }
            m_wavBackspace = BuildWav(s, sr);
        }

        // 4. Error (Low Warning Pip - 40 ms)
        {
            int n = (int)(sr * 0.040);
            std::vector<short> s(n);
            for (int i = 0; i < n; ++i) {
                double t = (double)i / sr;
                double env = std::exp(-t * 90.0);
                double val = std::sin(2.0 * PI * 260.0 * t);
                s[i] = (short)(val * env * 19000.0);
            }
            m_wavError = BuildWav(s, sr);
        }

        // 5. Typewriter Bell / Level Completion Chime (450 ms)
        {
            int n = (int)(sr * 0.45);
            std::vector<short> s(n);
            for (int i = 0; i < n; ++i) {
                double t = (double)i / sr;
                double env = std::exp(-t * 9.0);
                double val = std::sin(2.0 * PI * 2093.0 * t) * 0.6 +
                             std::sin(2.0 * PI * 3136.0 * t) * 0.3 +
                             std::sin(2.0 * PI * 4186.0 * t) * 0.1;
                s[i] = (short)(val * env * 24000.0);
            }
            m_wavBell = BuildWav(s, sr);
        }
    }
};
