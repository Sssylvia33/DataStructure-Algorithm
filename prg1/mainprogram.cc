// The main program (provided by the course), TIE-20100/TIE-20106
//
// DO ****NOT**** EDIT THIS FILE!
// (Preferably do not edit this even temporarily. And if you still decide to do so
//  (for debugging, for example), DO NOT commit any changes to git, but revert all
//  changes later. Otherwise you won't be able to get any updates/fixes to this
//  file from git!)

#include <string>
using std::string;
using std::getline;

#include <iostream>
using std::cout;
using std::cin;
using std::cerr;
using std::endl;
using std::flush;
using std::noskipws;

#include <istream>
using std::istream;

#include <ostream>
using std::ostream;

#include <fstream>
using std::ifstream;

#include <sstream>
using std::istringstream;
using std::ostringstream;
using std::stringstream;

#include <iomanip>
using std::setw;

#include <tuple>
using std::tuple;
using std::make_tuple;
using std::get;
using std::tie;

#include <regex>
using std::regex_match;
using std::regex_search;
using std::smatch;
using std::regex;
using std::sregex_token_iterator;

#include <algorithm>
using std::find_if;
using std::find;
using std::max_element;

#include <random>
using std::minstd_rand;
using std::uniform_int_distribution;

#include <chrono>

#include <functional>
using std::function;
using std::equal_to;

#include <vector>
using std::vector;

#include <array>
using std::array;

#include <ctime>
using std::time;

#include <memory>
using std::move;

#include <cstdlib>
#include <cstddef>
#include <cassert>


#include "mainprogram.hh"

#include "datastructures.hh"

#ifdef GRAPHICAL_GUI
#include "mainwindow.hh"
#endif

string const MainProgram::PROMPT = "> ";


//std::tuple<unsigned long int,string> mempeak(){
//    ifstream file_stream("/proc/self/status");
//    if( file_stream ){
//        string line;
//        string ID;
//        unsigned long int amnt;
//        string unit;
//        while( getline( file_stream, line ))
//        {
//            istringstream line_stream( line );
//            getline(line_stream, ID, ':');
//            if (ID == "VmPeak"){
//                line_stream >> amnt;
//                line_stream >> unit;
//                return make_tuple(amnt, unit);
//            }
//        }
//    }
//    file_stream.close();
//    return make_tuple(0, "?");
//}

//CmdResult test_get_functions(Datastructure& ds, PersonID id)
//{
//    ds.get_name(id);
//    ds.get_salary(id);
//    ds.get_title(id);
//}

MainProgram::CmdResult MainProgram::cmd_add_town(ostream& /*output*/, MatchIter begin, MatchIter end)
{
    string name = *begin++;
    string xstr = *begin++;
    string ystr = *begin++;
    assert( begin == end && "Impossible number of parameters!");
//    string heightstr = *begin++;
//    assert( begin == end && "Impossible number of parameters!");
//    int height;
//    if (!heightstr.empty())
//    {
//        height = convert_string_to<int>(heightstr);
//    }
//    else
//    {
//        height = -1; // REPLACE WITH HEIGTH FROM MAP!
//    }

    int x = convert_string_to<int>(xstr);
    int y = convert_string_to<int>(ystr);

    return {ds_.add_town(name, x, y)};
}

MainProgram::CmdResult MainProgram::cmd_nth_distance(ostream& /*output*/, MatchIter begin, MatchIter end)
{
    string nstr = *begin++;
    assert( begin == end && "Impossible number of parameters!");

    unsigned int n = convert_string_to<unsigned int>(nstr);

    auto result = ds_.nth_distance(n);

    return {result};
}

void MainProgram::test_nth_distance()
{
    auto n = ds_.size();

    if (n > 0) // Don't remove ceo if the only person
    {
        ds_.nth_distance(n);
    }
}

MainProgram::CmdResult MainProgram::cmd_towns_from(ostream& /*output*/, MatchIter begin, MatchIter end)
{
    string xstr = *begin++;
    string ystr = *begin++;
    assert( begin == end && "Impossible number of parameters!");

    int x = convert_string_to<int>(xstr);
    int y = convert_string_to<int>(ystr);

    auto result = ds_.towns_distance_increasing_from(x, y);

    return result;
}

void MainProgram::test_towns_from()
{
    int x = random<int>(1, 10000);
    int y = random<int>(1, 10000);
    ds_.towns_distance_increasing_from(x, y);
}

MainProgram::CmdResult MainProgram::cmd_remove(ostream& output, MatchIter begin, MatchIter end)
{
    string id = *begin++;
    assert( begin == end && "Impossible number of parameters!");

    ds_.remove_town(id);
    output << id << " removed." << endl;

    view_dirty = true;
    return {};
}

void MainProgram::test_remove()
{
    // Choose random number to remove
    if (random_towns_added_ > 0) // Don't remove if there's nothing to remove
    {
        auto name = n_to_name(random<decltype(random_towns_added_)>(0, random_towns_added_));
        ds_.remove_town(name);
    }
}

void MainProgram::add_random_towns(unsigned int size)
{
    for (unsigned int i = 0; i < size; ++i)
    {
        string name = n_to_name(random_towns_added_);

        int x = random<int>(1, 10000);
        int y = random<int>(1, 10000);

        ds_.add_town(name, x, y);

        ++random_towns_added_;
    }
}

MainProgram::CmdResult MainProgram::cmd_random_add(ostream& output, MatchIter begin, MatchIter end)
{
    string sizestr = *begin++;
    assert( begin == end && "Impossible number of parameters!");

    unsigned int size = convert_string_to<unsigned int>(sizestr);

    add_random_towns(size);

    output << "Added: " << size << " towns." << endl;

    view_dirty = true;

    return {};
}

void MainProgram::test_random_add()
{
    add_random_towns(1);
}

MainProgram::CmdResult MainProgram::cmd_randseed(ostream& output, MatchIter begin, MatchIter end)
{
    string seedstr = *begin++;
    assert(begin == end && "Invalid number of parameters");

    unsigned long int seed = convert_string_to<unsigned long int>(seedstr);

    rand_engine_.seed(seed);
    init_primes();

    output << "Random seed set to " << seed << endl;

    return {};
}

MainProgram::CmdResult MainProgram::cmd_read(ostream& output, MatchIter begin, MatchIter end)
{
    string filename = *begin++;
    assert( begin == end && "Impossible number of parameters!");

    ifstream input(filename);
    if (input)
    {
        output << "** Commands from '" << filename << "'" << endl;
        command_parser(input, output, PromptStyle::NORMAL);
        output << "** End of commands from '" << filename << "'" << endl;
    }
    else
    {
        output << "Cannot open file '" << filename << "'!" << endl;
    }

    return {};
}


MainProgram::CmdResult MainProgram::cmd_testread(ostream& output, MatchIter begin, MatchIter end)
{
    string infilename = *begin++;
    string outfilename = *begin++;
    assert( begin == end && "Impossible number of parameters!");

    ifstream input(infilename);
    if (input)
    {
        ifstream expected_output(outfilename);
        if (output)
        {
            stringstream actual_output;
            command_parser(input, actual_output, PromptStyle::NO_NESTING);

            vector<string> actual_lines;
            while (actual_output)
            {
                string line;
                getline(actual_output, line);
                if (!actual_output) { break; }
                actual_lines.push_back(line);
            }

            vector<string> expected_lines;
            while (expected_output)
            {
                string line;
                getline(expected_output, line);
                if (!expected_output) { break; }
                expected_lines.push_back(line);
            }

            string heading_actual = "Actual output";
            unsigned int actual_max_length = heading_actual.length();
            auto actual_max_iter = std::max_element(actual_lines.cbegin(), actual_lines.cend(),
                                                    [](string s1, string s2){ return s1.length() < s2.length(); });
            if (actual_max_iter != actual_lines.cend())
            {
                actual_max_length = actual_max_iter->length();
            }

            string heading_expected = "Expected output";
            unsigned int expected_max_length = heading_expected.length();
            auto expected_max_iter = std::max_element(expected_lines.cbegin(), expected_lines.cend(),
                                                    [](string s1, string s2){ return s1.length() < s2.length(); });
            if (expected_max_iter != expected_lines.cend())
            {
                expected_max_length = expected_max_iter->length();
            }

            auto pos_actual = actual_lines.cbegin();
            auto pos_expected = expected_lines.cbegin();
            output << "  " << heading_actual << string(actual_max_length - heading_actual.length(), ' ') << " | " << heading_expected << endl;
            output << "--" << string(actual_max_length, '-') << "-|-" << string(expected_max_length, '-') << endl;

            bool lines_ok = true;
            while (pos_expected != expected_lines.cend() || pos_actual != actual_lines.cend())
            {
                if (pos_expected != expected_lines.cend())
                {
                    if (pos_actual != actual_lines.cend())
                    {
                        bool ok = (*pos_expected == *pos_actual);
                        output << (ok ? ' ' : '?') << ' ' << *pos_actual << string(actual_max_length - pos_actual->length(), ' ')
                               << " | " << *pos_expected << endl;
                        lines_ok = lines_ok && ok;
                        ++pos_actual;
                    }
                    else
                    { // Actual output was too short
                        output << "? " << string(actual_max_length, ' ')
                               << " | " << *pos_expected << endl;
                        lines_ok = false;
                    }
                    ++pos_expected;
                }
                else
                { // Actual output was too long
                    output << "? " << *pos_actual << string(actual_max_length - pos_actual->length(), ' ')
                           << " | " << endl;
                    lines_ok = false;
                    ++pos_actual;
                }
            }
            if (lines_ok)
            {
                output << "**No differences in output.**" << endl;
                if (test_status_ == TestStatus::NOT_RUN)
                {
                    test_status_ = TestStatus::NO_DIFFS;
                }
            }
            else
            {
                output << "**Differences found! (Lines beginning with '?')**" << endl;
                test_status_ = TestStatus::DIFFS_FOUND;
            }

        }
        else
        {
            output << "Cannot open file '" << outfilename << "'!" << endl;
        }
    }
    else
    {
        output << "Cannot open file '" << infilename << "'!" << endl;
    }

    return {};
}

MainProgram::CmdResult MainProgram::cmd_size(ostream& output, MatchIter begin, MatchIter end)
{
    assert( begin == end && "Impossible number of parameters!");

    output << "Number of towns: " << ds_.size() << endl;

    return {};
}

MainProgram::CmdResult MainProgram::cmd_all_towns(ostream& output, MatchIter begin, MatchIter end)
{
    assert( begin == end && "Impossible number of parameters!");

    auto towns = ds_.all_towns();
    if (towns.empty())
    {
        output << "No towns!" << endl;
    }

    return towns;
}

void MainProgram::test_all_towns()
{
    ds_.all_towns();
}

MainProgram::CmdResult MainProgram::cmd_stopwatch(ostream& output, MatchIter begin, MatchIter end)
{
    string on = *begin++;
    string off = *begin++;
    string next = *begin++;
    assert(begin == end && "Invalid number of parameters");

    if (!on.empty())
    {
        stopwatch_mode = StopwatchMode::ON;
        output << "Stopwatch: on" << endl;
    }
    else if (!off.empty())
    {
        stopwatch_mode = StopwatchMode::OFF;
        output << "Stopwatch: off" << endl;
    }
    else if (!next.empty())
    {
        stopwatch_mode = StopwatchMode::NEXT;
        output << "Stopwatch: on for the next command" << endl;
    }
    else
    {
        assert(!"Impossible stopwatch mode!");
    }

    return {};
}

MainProgram::CmdResult MainProgram::cmd_clear(ostream& output, MatchIter begin, MatchIter end)
{
    assert(begin == end && "Invalid number of parameters");

    ds_.clear();
    init_primes();

    output << "Cleared all towns" << endl;

    view_dirty = true;

    return {};
}

MainProgram::CmdResult MainProgram::print_town(TownData* id, ostream& output)
{
//    if (id != NO_ID)
    if (id)
    {
//        output << "id " << id << " : " << ds.get_title(id) << " " << ds.get_name(id) << ", salary " << ds.get_salary(id);
//        output << p->name << "\": tax=" << p->tax << ", pos=(" << p->x << "," << p->y << "), height=" << p->height << endl;
        output << id->name << ": pos=(" << id->x << "," << id->y << ")" << endl;

    }
    else
    {
        output << "-- unknown --" << endl;
    }

    return {};
}

//CmdResult print_person_list(Datastructures& ds, PersonList const& person_list, ostream& output)
//{
//    for (auto person : person_list)
//    {
//        print_person(ds, person, output);
//        output << endl;
//    }
//}

MainProgram::CmdResult MainProgram::cmd_find(ostream& /*output*/, MatchIter begin, MatchIter end)
{
    string name = *begin++;
    assert( begin == end && "Impossible number of parameters!");

    auto result = ds_.find_town(name);

    return {result};
}

void MainProgram::test_find()
{
    // Choose random number to remove
    if (random_towns_added_ > 0) // Don't find if there's nothing to find
    {
        auto name = n_to_name(random<decltype(random_towns_added_)>(0, random_towns_added_));
        ds_.find_town(name);
    }
}

vector<MainProgram::CmdInfo> MainProgram::cmds_ =
{
    {"add_town", "Name (x,y)",
     "([a-zA-Z]+)[[:space:]]+\\(([0-9]+)[[:space:]]*,[[:space:]]*([0-9]+)\\)", &MainProgram::cmd_add_town, nullptr },
    {"random_add", "number_of_towns_to_add", "([0-9]+)", &MainProgram::cmd_random_add, &MainProgram::test_random_add },
    {"all_towns", "", "", &MainProgram::cmd_all_towns, &MainProgram::test_all_towns },
    {"size", "", "", &MainProgram::cmd_size, nullptr },
    {"clear", "", "", &MainProgram::cmd_clear, nullptr },
    {"alphalist", "", "", &MainProgram::NoParListCmd<&Datastructures::towns_alphabetically>, &MainProgram::NoParListTestCmd<&Datastructures::towns_alphabetically> },
    {"distlist", "", "", &MainProgram::NoParListCmd<&Datastructures::towns_distance_increasing>, &MainProgram::NoParListTestCmd<&Datastructures::towns_distance_increasing> },
    {"mindist", "", "", &MainProgram::NoParTownCmd<&Datastructures::min_distance>, &MainProgram::NoParTownTestCmd<&Datastructures::min_distance> },
    {"maxdist", "", "", &MainProgram::NoParTownCmd<&Datastructures::max_distance>, &MainProgram::NoParTownTestCmd<&Datastructures::max_distance> },
    {"nth_distance", "n", "([0-9]+)", &MainProgram::cmd_nth_distance, &MainProgram::test_nth_distance },
    {"towns_from", "(x,y)", "\\(([0-9]+)[[:space:]]*,[[:space:]]*([0-9]+)\\)", &MainProgram::cmd_towns_from, &MainProgram::test_towns_from },
    {"remove", "Name", "([A-Za-z]+)", &MainProgram::cmd_remove, &MainProgram::test_remove },
    {"find", "name", "([a-zA-Z]+)", &MainProgram::cmd_find, &MainProgram::test_find },
    {"quit", "", "", nullptr, nullptr },
    {"help", "", "", &MainProgram::help_command, nullptr },
    {"read", "\"in-filename\"",
     "\"([-a-zA-Z0-9 ./:_]+)\"", &MainProgram::cmd_read, nullptr },
    {"testread", "\"in-filename\" \"out-filename\"",
     "\"([-a-zA-Z0-9 ./:_]+)\"[[:space:]]+\"([-a-zA-Z0-9 ./:_]+)\"", &MainProgram::cmd_testread, nullptr },
//    {"perftest", "",
//     "([0-9a-zA-Z_]+(?:;[0-9a-zA-Z_]+)*)[[:space:]]+([0-9]+)[[:space:]]+([0-9]+)[[:space:]]+([0-9]+)[[:space:]]+([0-9]+(?:;[0-9]+)*)", &cmd_perftest, nullptr },
    {"perftest", "cmd1/all/compulsory[;cmd2;cmd3...] timeout repeat_count n1[;n2;n3...] (parts in [] are optional)",
     "([0-9a-zA-Z_]+(?:;[0-9a-zA-Z_]+)*)[[:space:]]+([0-9]+)[[:space:]]+([0-9]+)[[:space:]]+([0-9]+(?:;[0-9]+)*)", &MainProgram::cmd_perftest, nullptr },
    {"stopwatch", "on/off/next (one of these)", "(?:(on)|(off)|(next))", &MainProgram::cmd_stopwatch, nullptr },
    {"random_seed", "new-random-seed-integer", "([0-9]+)", &MainProgram::cmd_randseed, nullptr },
    {"#", "comment text", ".*", &MainProgram::cmd_comment, nullptr },
};

MainProgram::CmdResult MainProgram::help_command(ostream& output, MatchIter /*begin*/, MatchIter /*end*/)
{
    output << "Commands:" << endl;
    for (auto& i : cmds_)
    {
        output << "  " << i.cmd << " " << i.info << endl;
    }

    return {};
}

MainProgram::CmdResult MainProgram::cmd_perftest(ostream& output, MatchIter begin, MatchIter end)
{
#ifdef _GLIBCXX_DEBUG
    output << "WARNING: Debug STL enabled, performance will be worse than expected (maybe also asymptotically)!" << endl;
#endif // _GLIBCXX_DEBUG

    vector<string> optional_cmds({"remove", "towns_from"});
    vector<string> nondefault_cmds({"all_towns", "remove"});

    string commandstr = *begin++;
    unsigned int timeout = convert_string_to<unsigned int>(*begin++);
    unsigned int repeat_count = convert_string_to<unsigned int>(*begin++);
//    unsigned int friend_count = convert_string_to<unsigned int>(*begin++);
    string sizes = *begin++;
    assert(begin == end && "Invalid number of parameters");

    vector<string> testcmds;
    if (commandstr != "all" && commandstr != "compulsory")
    {
        regex commands_regex("([0-9a-zA-Z_]+);?");
        smatch scmd;
        auto cbeg = commandstr.cbegin();
        auto cend = commandstr.cend();
        for ( ; regex_search(cbeg, cend, scmd, commands_regex); cbeg = scmd.suffix().first)
        {
            testcmds.push_back(scmd[1]);
        }
    }

    vector<unsigned int> init_ns;
    regex sizes_regex("([0-9]+);?");
    smatch size;
    auto sbeg = sizes.cbegin();
    auto send = sizes.cend();
    for ( ; regex_search(sbeg, send, size, sizes_regex); sbeg = size.suffix().first)
    {
        init_ns.push_back(convert_string_to<unsigned int>(size[1]));
    }

    output << "Timeout for each N is " << timeout << " sec. " << endl;
//    output << "Add 0.." << friend_count << " friends for every employee." << endl;
    output << "For each N perform " << repeat_count << " random command(s) from:" << endl;

    // Initialize test functions
    vector<void(MainProgram::*)()> testfuncs;
    if (testcmds.empty())
    { // Add all commands
        for (auto& i : cmds_)
        {
            if (i.testfunc)
            {
                if (find(nondefault_cmds.begin(), nondefault_cmds.end(), i.cmd) == nondefault_cmds.end() &&
                    (commandstr == "all" || find(optional_cmds.begin(), optional_cmds.end(), i.cmd) == optional_cmds.end()))
                {
                    output << i.cmd << " ";
                    testfuncs.push_back(i.testfunc);
                }
            }
        }
    }
    else
    {
        for (auto& i : testcmds)
        {
            auto pos = find_if(cmds_.begin(), cmds_.end(), [&i](auto& cmd){ return cmd.cmd == i; });
            if (pos != cmds_.end() && pos->testfunc)
            {
                output << i << " ";
                testfuncs.push_back(pos->testfunc);
            }
            else
            {
                output << "(cannot test " << i << ") ";
            }
        }
    }
    output << endl << endl;

    if (testfuncs.empty())
    {
        output << "No commands to test!" << endl;
        return {};
    }

    output << setw(7) << "N" << " , " << setw(12) << "add (sec)" << " , " << setw(12) << "cmds (sec)"  << " , " << setw(12) << "total (sec)" << endl;
    flush_output(output);

    auto stop = false;
    for (unsigned int n : init_ns)
    {
        if (stop) { break; }

        output << setw(7) << n << " , " << flush;

        ds_.clear();
        init_primes();

        Stopwatch stopwatch;
        stopwatch.start();

        for (unsigned int i = 0; i < n / 1000; ++i)
        {
            add_random_towns(1000);

            stopwatch.stop();
            if (stopwatch.elapsed() >= timeout)
            {
                output << "Timeout!" << endl;
                stop = true;
                break;
            }
            if (check_stop())
            {
                output << "Stopped!" << endl;
                stop = true;
                break;
            }
            stopwatch.start();
        }
        if (stop) { break; }

        add_random_towns(n % 1000);

//        for (unsigned int i = 0; i < people_added; ++i)
//        {
//            auto friends = random(0u, friend_count+1);
//            while (friends != 0)
//            {
//                auto id1 = n_to_id(i);
//                auto id2 = n_to_id(random<decltype(people_added)>(0, people_added));
//                if (id1 != id2)
//                {
//                    auto cost = random<Cost>(0, 10);
//                    ds.add_friendship(id1, id2, cost);
//                }
//                --friends;
//            }
//        }

        auto addsec = stopwatch.elapsed();
        output << setw(12) << addsec << " , " << flush;
        if (addsec >= timeout)
        {
            output << "Timeout!" << endl;
            stop = true;
            break;
        }

        for (unsigned int repeat = 0; repeat < repeat_count; ++repeat)
        {
            auto cmdpos = random(testfuncs.begin(), testfuncs.end());

            (this->**cmdpos)();

            if (repeat % 10 == 0)
            {
                stopwatch.stop();
                if (stopwatch.elapsed() >= timeout)
                {
                    output << "Timeout!" << endl;
                    stop = true;
                    break;
                }
                if (check_stop())
                {
                    output << "Stopped!" << endl;
                    stop = true;
                    break;
                }
                stopwatch.start();
            }
        }
        if (stop) { break; }

        stopwatch.stop();
        auto totalsec = stopwatch.elapsed();
        output << setw(12) << totalsec-addsec << " , " << setw(12) << totalsec;

//        unsigned long int maxmem;
//        string unit;
//        tie(maxmem, unit) = mempeak();
//        maxmem -=  startmem;
//        if (maxmem != 0)
//        {
//            output << ", memory " << maxmem << " " << unit;
//        }
        output << endl;
        flush_output(output);
    }

    ds_.clear();
    init_primes();

#ifdef _GLIBCXX_DEBUG
    output << "WARNING: Debug STL enabled, performance will be worse than expected (maybe also asymptotically)!" << endl;
#endif // _GLIBCXX_DEBUG

    return {};
}

MainProgram::CmdResult MainProgram::cmd_comment(ostream& /*output*/, MatchIter /*begin*/, MatchIter /*end*/)
{
    return {};
}

bool MainProgram::command_parse_line(string inputline, ostream& output)
{
//    static unsigned int nesting_level = 0; // UGLY! Remember nesting level to print correct amount of >:s.
//    if (promptstyle != PromptStyle::NO_NESTING) { ++nesting_level; }

    // Create regex <whitespace>(cmd1|cmd2|...)<whitespace>(.*)
    string cmds_regex_str = "[[:space:]]*(";
    bool first = true;
    for (auto cmd : cmds_)
    {
        cmds_regex_str += (first ? "" : "|") + cmd.cmd;
        first = false;
    }
    cmds_regex_str += ")(?:[[:space:]]*$|[[:space:]]+(.*))";
    regex cmds_regex(cmds_regex_str);

    smatch match;
    bool matched = regex_match(inputline, match, cmds_regex);
    if (matched)
    {
        assert(match.size() == 3);
        string cmd = match[1];
        string params = match[2];

        auto pos = find_if(cmds_.begin(), cmds_.end(), [cmd](CmdInfo const& ci) { return ci.cmd == cmd; });
        assert(pos != cmds_.end());

        string params_regex_str = pos->param_regex_str+"[[:space:]]*";
        smatch match;
        bool matched = regex_match(params, match, regex(params_regex_str));
        if (matched)
        {
            if (pos->func)
            {
                assert(!match.empty());

                Stopwatch stopwatch;
                bool use_stopwatch = (stopwatch_mode != StopwatchMode::OFF);
                // Reset stopwatch mode if only for the next command
                if (stopwatch_mode == StopwatchMode::NEXT) { stopwatch_mode = StopwatchMode::OFF; }

               TestStatus initial_status = test_status_;
               test_status_ = TestStatus::NOT_RUN;

                if (use_stopwatch)
                {
                    stopwatch.start();
                }

                auto result = (this->*(pos->func))(output, ++(match.begin()), match.end());

                if (use_stopwatch)
                {
                    stopwatch.stop();
                }

                if (!result.empty())
                {
                    unsigned int num = 0;
                    for (TownData* p : result)
                    {
                        ++num;
                        if (result.size() > 1) { output << num << ". "; }
                        print_town(p, output);
                    }
                }
                if (result != prev_result)
                {
                    prev_result = move(result);
                    view_dirty = true;
                }

                if (use_stopwatch)
                {
                    output << "Command '" << cmd << "': " << stopwatch.elapsed() << " sec" << endl;
                }

                if (test_status_ != TestStatus::NOT_RUN)
                {
                    output << "Testread-tests have been run, " << ((test_status_ == TestStatus::DIFFS_FOUND) ? "differences found!" : "no differences found.") << endl;
                }
                if (test_status_ == TestStatus::NOT_RUN || (test_status_ == TestStatus::NO_DIFFS && initial_status == TestStatus::DIFFS_FOUND))
                {
                    test_status_ = initial_status;
                }
            }
            else
            { // No function to run = quit command
                return false;
            }
        }
        else
        {
            output << "Invalid parameters for command '" << cmd << "'!" << endl;
        }
    }
    else
    {
        output << "Unknown command!" << endl;
    }

    return true; // Signal continuing
}

void MainProgram::command_parser(istream& input, ostream& output, PromptStyle promptstyle)
{
    string line;
    do
    {
//        output << string(nesting_level, '>') << " ";
        output << PROMPT;
        getline(input, line, '\n');

        if (promptstyle != PromptStyle::NO_ECHO)
        {
            output << line << endl;
        }

        if (!input) { break; }

        bool cont = command_parse_line(line, output);
        view_dirty = false; // No need to keep track of individual result changes
        if (!cont) { break; }
    }
    while (input);
    //    if (promptstyle != PromptStyle::NO_NESTING) { --nesting_level; }

    view_dirty = true; // To be safe, assume that results have been changed
}

void MainProgram::setui(MainWindow* ui)
{
    ui_ = ui;
}

#ifdef GRAPHICAL_GUI
void MainProgram::flush_output(ostream& output)
{
    if (ui_)
    {
        if (auto soutput = dynamic_cast<ostringstream*>(&output))
        {
            ui_->output_text(*soutput);
        }
    }
}
#else
void MainProgram::flush_output(ostream& /*output*/)
{
}
#endif

bool MainProgram::check_stop() const
{
#ifdef GRAPHICAL_GUI
    if (ui_)
    {
        return ui_->check_stop_pressed();
    }
#endif
    return false;
}

std::array<unsigned long int, 20> const MainProgram::primes1{4943,   4951,   4957,   4967,   4969,   4973,   4987,   4993,   4999,   5003,
                                                             5009,   5011,   5021,   5023,   5039,   5051,   5059,   5077,   5081,   5087};
std::array<unsigned long int, 20> const MainProgram::primes2{81031,  81041,  81043,  81047,  81049,  81071,  81077,  81083,  81097,  81101,
                                                             81119,  81131,  81157,  81163,  81173,  81181,  81197,  81199,  81203,  81223};

MainProgram::MainProgram()
{
    rand_engine_.seed(time(nullptr));

    //    startmem = get<0>(mempeak());

    init_primes();
}

int MainProgram::mainprogram(int argc, char* argv[])
{
    vector<string> args(argv, argv+argc);

    if (args.size() > 2)
    {
        cerr << "Usage: " + ((args.size() > 0) ? args[0] : "<program name>") + " [<command file>]" << endl;
        return EXIT_FAILURE;
    }

    MainProgram mainprg;

    if (args.size() == 2)
    {
        string filename = args[1];
        ifstream input(filename);
        if (input)
        {
            mainprg.command_parser(input, cout, MainProgram::PromptStyle::NORMAL);
        }
        else
        {
            cout << "Cannot open file '" << filename << "'!" << endl;
        }
    }
    else
    {
        mainprg.command_parser(cin, cout, MainProgram::PromptStyle::NO_ECHO);
    }

    cerr << "Program ended normally." << endl;
    return EXIT_SUCCESS;
}

void MainProgram::init_primes()
{
    // Initialize id generator
    prime1_ = primes1[random<int>(0, primes1.size())];
    prime2_ = primes2[random<int>(0, primes2.size())];
    random_towns_added_ = 0;
}

string MainProgram::n_to_name(unsigned long n)
{
    unsigned long int hash = prime1_*n + prime2_;
    string name;

    while (hash > 0)
    {
        auto hexnum = hash % 26;
        hash /= 26;
        name.push_back('a'+hexnum);
    }

    return name;
}
