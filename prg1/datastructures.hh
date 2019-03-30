//GUO JIAQI 268459
//TIE-20106 ASSIGNMENT 1

// Datastructures.hh

#ifndef DATASTRUCTURES_HH
#define DATASTRUCTURES_HH

#include <string>
#include <vector>
#include <memory>
using namespace std;

struct TownData
{
    std::string name;
    int x;
    int y;
    int dz;
};

class Datastructures
{
public:
    Datastructures();
    ~Datastructures();

    // Estimate of performance:O(1)
    // Short rationale for estimate:constant,just return
    //the private data_number_ .
    unsigned int size();

    // Estimate of performance:O(1)
    // Short rationale for estimate:
    void clear();

    // Estimate of performance: O(1)
    // Short rationale for estimate:constant, simply return the
    //private vector.
    std::vector<TownData*> all_towns();

    // Estimate of performance: O(1)
    // Short rationale for estimate:constant 8
    TownData* add_town(std::string const& name, int x, int y);

    // Estimate of performance:O(n*lg(n))
    // Short rationale for estimate: randomized quick sort
    std::vector<TownData*> towns_alphabetically();

    // Estimate of performance: O(n*lg(n))
    // Short rationale for estimate:randomized quick sort
    std::vector<TownData*> towns_distance_increasing();

    // Estimate of performance: O(n)
    // Short rationale for estimate:
    TownData* find_town(std::string const& name);

    // Estimate of performance: O(n*lg(n))
    // Short rationale for estimate:using randomized quick sort
    //and return the first element of the sorted vector
    TownData* min_distance();

    // Estimate of performance: O(n*lg(n))
    // Short rationale for estimate:using randomized quick sort
    //and return the last element of the sorted vector
    TownData* max_distance();

    // Estimate of performance:O(n)
    // Short rationale for estimate:quick select algorithm
    TownData* nth_distance(unsigned int n);

    // Non-compulsory operations

    // Estimate of performance: O(n)
    // Short rationale for estimate:contains a for loop going
    //through every element, linear
    void remove_town(std::string const& town_name);

    // Estimate of performance: O(n*lg(n))
    // Short rationale for estimate:a for loop costs linear time and use
    //randomized quick sort
    std::vector<TownData*> towns_distance_increasing_from(int x, int y);

private:


    vector<TownData*> data_list_;
    unsigned int data_number_;

};

#endif // DATASTRUCTURES_HH
