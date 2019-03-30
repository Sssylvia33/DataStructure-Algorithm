#ifndef MAINPROGRAM_HH
#define MAINPROGRAM_HH


#ifdef QT_CORE_LIB
#define GRAPHICAL_GUI
#endif


#include <string>
#include <random>
#include <regex>
#include <chrono>
#include <sstream>
#include <stdexcept>
#include <iostream>
#include <vector>
#include <array>
#include <functional>

#include "datastructures.hh"

class MainWindow; // In case there's UI

class MainProgram
{
public:
    MainProgram();


    class Stopwatch;

    enum class PromptStyle { NORMAL, NO_ECHO, NO_NESTING };
    enum class TestStatus { NOT_RUN, NO_DIFFS, DIFFS_FOUND };

    bool command_parse_line(std::string input, std::ostream& output);
    void command_parser(std::istream& input, std::ostream& output, PromptStyle promptstyle);

    void setui(MainWindow* ui);

    void flush_output(std::ostream& output);
    bool check_stop() const;

    static int mainprogram(int argc, char* argv[]);

private:
    Datastructures ds_;
    MainWindow* ui_ = nullptr;

    static std::string const PROMPT;

    std::minstd_rand rand_engine_;

    static std::array<unsigned long int, 20> const primes1;
    static std::array<unsigned long int, 20> const primes2;
    unsigned long int prime1_ = 0; // Will be initialized to random value from above
    unsigned long int prime2_ = 0; // Will be initialized to random value from above
    unsigned long int random_towns_added_ = 0; // Counter for random towns added
    void init_primes();
    std::string n_to_name(unsigned long int n);


    enum class StopwatchMode { OFF, ON, NEXT };
    StopwatchMode stopwatch_mode = StopwatchMode::OFF;

    using CmdResult = std::vector<TownData*>;
    CmdResult prev_result;
    bool view_dirty = true;

    TestStatus test_status_ = TestStatus::NOT_RUN;

    using MatchIter = std::smatch::const_iterator;
    struct CmdInfo
    {
        std::string cmd;
        std::string info;
        std::string param_regex_str;
        CmdResult(MainProgram::*func)(std::ostream& output, MatchIter begin, MatchIter end);
        void(MainProgram::*testfunc)();
    };
    static std::vector<CmdInfo> cmds_;

    CmdResult help_command(std::ostream& output, MatchIter begin, MatchIter end);
    CmdResult cmd_add_town(std::ostream& output, MatchIter begin, MatchIter end);
    CmdResult cmd_nth_distance(std::ostream& output, MatchIter begin, MatchIter end);
    CmdResult cmd_towns_from(std::ostream& output, MatchIter begin, MatchIter end);
    CmdResult cmd_remove(std::ostream& output, MatchIter begin, MatchIter end);
    CmdResult cmd_random_add(std::ostream& output, MatchIter begin, MatchIter end);
    CmdResult cmd_randseed(std::ostream& output, MatchIter begin, MatchIter end);
    CmdResult cmd_read(std::ostream& output, MatchIter begin, MatchIter end);
    CmdResult cmd_testread(std::ostream& output, MatchIter begin, MatchIter end);
    CmdResult cmd_size(std::ostream& output, MatchIter begin, MatchIter end);
    CmdResult cmd_all_towns(std::ostream& output, MatchIter begin, MatchIter end);
    CmdResult cmd_stopwatch(std::ostream& output, MatchIter begin, MatchIter end);
    CmdResult cmd_clear(std::ostream& output, MatchIter begin, MatchIter end);
    CmdResult cmd_find(std::ostream& output, MatchIter begin, MatchIter end);
    CmdResult cmd_perftest(std::ostream& output, MatchIter begin, MatchIter end);
    CmdResult cmd_comment(std::ostream& output, MatchIter begin, MatchIter end);

    void test_nth_distance();
    void test_towns_from();
    void test_remove();
    void test_find();
    void test_random_add();
    void test_all_towns();

    void add_random_towns(unsigned int size);
    static CmdResult print_town(TownData* id, std::ostream& output);

    template <typename Type>
    Type random(Type start, Type end);
    template <typename To>
    static To convert_string_to(std::string from);
    template <typename From>
    static std::string convert_to_string(From from);

    template<TownData*(Datastructures::*MFUNC)()>
    CmdResult NoParTownCmd(std::ostream& output, MatchIter begin, MatchIter end);

    template<CmdResult(Datastructures::*MFUNC)()>
    CmdResult NoParListCmd(std::ostream& output, MatchIter begin, MatchIter end);

    template<TownData*(Datastructures::*MFUNC)()>
    void NoParTownTestCmd();

    template<CmdResult(Datastructures::*MFUNC)()>
    void NoParListTestCmd();

    friend class MainWindow;
};

template <typename Type>
Type MainProgram::random(Type start, Type end)
{
    auto range = end-start;
    assert(range != 0 && "random() with zero range!");

    auto num = std::uniform_int_distribution<unsigned long int>(0, range-1)(rand_engine_);

    return static_cast<Type>(start+num);
}

template <typename To>
To MainProgram::convert_string_to(std::string from)
{
    std::istringstream istr(from);
    To result;
    istr >> std::noskipws >> result;
    if (istr.fail() || !istr.eof())
    {
        throw std::invalid_argument("Cannot convert string to required type");
    }
    return result;
}

template <typename From>
std::string MainProgram::convert_to_string(From from)
{
    std::ostringstream ostr;
    ostr << from;
    if (ostr.fail())
    {
        throw std::invalid_argument("Cannot convert type to string");
    }
    return ostr.str();
}

template<TownData*(Datastructures::*MFUNC)()>
MainProgram::CmdResult MainProgram::NoParTownCmd(std::ostream& /*output*/, MatchIter /*begin*/, MatchIter /*end*/)
{
    auto result = (ds_.*MFUNC)();
    return {result};
}

template<MainProgram::CmdResult(Datastructures::*MFUNC)()>
MainProgram::CmdResult MainProgram::NoParListCmd(std::ostream& /*output*/, MatchIter /*begin*/, MatchIter /*end*/)
{
    auto result = (ds_.*MFUNC)();
    return result;
}

template<TownData*(Datastructures::*MFUNC)()>
void MainProgram::NoParTownTestCmd()
{
    (ds_.*MFUNC)();
}

template<MainProgram::CmdResult(Datastructures::*MFUNC)()>
void MainProgram::NoParListTestCmd()
{
    (ds_.*MFUNC)();
}


class MainProgram::Stopwatch
{
public:
    using Clock = std::chrono::high_resolution_clock;

    void start() { running_ = true; starttime_ = Clock::now(); }
    void stop() { running_ = false; elapsed_ += (Clock::now() - starttime_); }
    void reset() { running_ = false; elapsed_ = elapsed_.zero(); }
    double elapsed()
    {
        if (!running_)
        {
            return static_cast<std::chrono::duration<double>>(elapsed_).count();
        }
        else
        {
            auto total = elapsed_ + (Clock::now() - starttime_);
            return static_cast<std::chrono::duration<double>>(total).count();
        }
    }
private:
    std::chrono::time_point<Clock> starttime_;
    Clock::duration elapsed_ = Clock::duration::zero();
    bool running_ = false;
};


#endif // MAINPROGRAM_HH
