#pragma once
#include <set>
#include "types.hxx"

//raport dla konkretnych tur
class SpecificTurnsReportNotifier {
public:
    explicit SpecificTurnsReportNotifier(const std::set<Time>& turns)
        : turns_(turns) {}

    bool should_generate_report(Time t) const {
        return turns_.count(t) > 0;
    }

private:
    std::set<Time> turns_;
};

//raport w określonych odstępach
class IntervalReportNotifier {
public:
    explicit IntervalReportNotifier(TimeOffset interval)
        : interval_(interval) {}

    bool should_generate_report(Time t) const {
        return ((t - 1) % interval_) == 0;
    }

private:
    TimeOffset interval_;
};
