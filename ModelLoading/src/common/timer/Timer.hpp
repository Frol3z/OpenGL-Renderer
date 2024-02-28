#pragma once

class Timer {
public:
    static Timer& Get() 
    {
        static Timer instance;
        return instance;
    }

    void Update(float currentTime)
    {
        m_DeltaTime = currentTime - m_LastTime; 
        m_LastTime = currentTime;
    }

    inline double GetFPS() { return 1 / m_DeltaTime; }
    inline double GetMSPF() { return m_DeltaTime * 1000; }
    inline float GetDeltaTime() { return m_DeltaTime; }
    
private:
    Timer() {}
    Timer(const Timer&) = delete;
    Timer& operator=(const Timer&) = delete;

private:
    float m_LastTime = 0.0f;
    float m_DeltaTime = 0.0f;
};