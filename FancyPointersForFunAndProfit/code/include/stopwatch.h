//==================================================================================================
//  File:
//      stopwatch.h
//
//  Summary:
//      Defines a simple segmented addressing model as a class template.
//
//  Copyright (c) 2018 Bob Steagall, KEWB Computing
//==================================================================================================
//
#ifndef STOPWATCH_H_DEFINED
#define STOPWATCH_H_DEFINED

#include <chrono>

class stopwatch
{
  public:
    stopwatch();
    stopwatch(stopwatch&&) noexcept = default;
    stopwatch(stopwatch const&) noexcept = default;

    stopwatch&  operator =(stopwatch&&) noexcept = default;
    stopwatch&  operator =(stopwatch const&) noexcept = default;

    int64_t elapsed_msec() const;
    int64_t elapsed_nsec() const;
    int64_t elapsed_usec() const;

    void    start();
    void    stop();

  private:
    using time_point_t = std::chrono::time_point<std::chrono::high_resolution_clock>;
    using msec_t       = std::chrono::milliseconds;
    using usec_t       = std::chrono::microseconds;
    using nsec_t       = std::chrono::nanoseconds;

    time_point_t    m_start;
    time_point_t    m_stop;
};

inline
stopwatch::stopwatch()
:   m_start{}
,   m_stop{}
{
    start();
}

inline int64_t
stopwatch::elapsed_msec() const
{
    return (std::chrono::duration_cast<msec_t>(m_stop - m_start)).count();
}

inline int64_t
stopwatch::elapsed_nsec() const
{
    return (std::chrono::duration_cast<nsec_t>(m_stop - m_start)).count();
}

inline int64_t
stopwatch::elapsed_usec() const
{
    return (std::chrono::duration_cast<usec_t>(m_stop - m_start)).count();
}

inline void
stopwatch::start()
{
    m_start = std::chrono::high_resolution_clock::now();
}

inline void
stopwatch::stop()
{
    m_stop = std::chrono::high_resolution_clock::now();
}

#endif  //- STOPWATCH_H_DEFINED
