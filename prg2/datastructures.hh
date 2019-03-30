// Datastructures.hh

#ifndef DATASTRUCTURES_HH
#define DATASTRUCTURES_HH

#include <string>
#include <vector>
#include <utility>
#include <limits>
#include <iostream>
#include <unordered_map>


// Type for town IDs
using TownID = std::string;

// Return value for cases where required town was not found
TownID const NO_ID = "----------";



// Return value for cases where integer values were not found
int const NO_VALUE = std::numeric_limits<int>::min();

// Return value for cases where name values were not found
std::string const NO_NAME = "-- unknown --";

struct TownData{
    TownID id;
    std::string name;
    int x;
    int y;
    int tax;
    std::vector<TownData*> vassals;
    TownData* master;

};

class Datastructures
{
public:
    Datastructures();
    ~Datastructures();

    // Estimate of performance:O(1)
    // Short rationale for estimate:the performace for calling unorder_map.size is constant
    unsigned int size();

    // Estimate of performance:O(n)
    // Short rationale for estimate:calling clear function is linear in time
    void clear();

    // Estimate of performance:O(1)
    // Short rationale for estimate:it is constant for calling find function in time
    //and constant for checking if condition and return the value
    std::string get_name(TownID id);

    // Estimate of performance:O(1)
    // Short rationale for estimate:it is constant for calling find function in time
    //and constant for checking if condition and return the a new value
    std::pair<int, int> get_coordinates(TownID id);

    // Estimate of performance:O(1)
    // Short rationale for estimate:it is constant for calling find function in time
    //and constant for checking if condition and return the value
    int get_tax(TownID id);

    // Estimate of performance: it depands on how many vassels the town has, O(vlog(v))
    // Short rationale for estimate:it is constant for calling find function and all the
    //other operations in time, but sorting the vassals vector requirs v*log(v),
    //where  v is number of its vassal towns
    std::vector<TownID> get_vassals(TownID id);

    // Estimate of performance:O(n)
    // Short rationale for estimate:it is constant to add 1 element to the end of the vector,
    //and it need to be done n times in total
    std::vector<TownID> all_towns();

    // Estimate of performance:O(1)
    // Short rationale for estimate:the complexity of find() in if condition is constant
    //creating new values is constant, and inserting an element to the unorder_map is constant
    bool add_town(TownID id, std::string const& name, int x, int y, int tax);

    // Estimate of performance:O(1)
    // Short rationale for estimate:it is constant for calling find function in time
    bool change_town_name(TownID id, std::string const& newname);

    // Estimate of performance:O(n*log(n))
    // Short rationale for estimate:using O(n) in time to copy the vector and using O(nlog(n))
    //in time to sort the vector by name,in total O(n+n*log(n))
    std::vector<TownID> towns_alphabetically();

    // Estimate of performance:O(n*log(n))
    // Short rationale for estimate:using O(n) in time to copy the vector and using O(nlog(n))
    //in time to sort the vector by distance,in total O(n+n*log(n))
    std::vector<TownID> towns_distance_increasing();

    // Estimate of performance:O(n+ m*log(m))
    // Short rationale for estimate:go through every element in the map to check name and add
    //to newthe vector which took O(n),and sort the vector takes O(m*log(m))
    std::vector<TownID> find_towns(std::string const& name);

    // Estimate of performance:O(1)/O(n*log(n))
    // Short rationale for estimate:if it is already sorted then it is constant in time to return the min_value
    //otherwise it call the towns_distance_increasing() function and return 1st value which takes O(n*log(n))
    TownID min_distance();

    // Estimate of performance:O(1)/O(n*log(n))
    // Short rationale for estimate:if it is already sorted then it is constant in time to return the max_value
    //otherwise it call the towns_distance_increasing() function and return last value which takes O(n*log(n))
    TownID max_distance();

    // Estimate of performance:O(n)
    // Short rationale for estimate:using O(n) in time to copy the vector and using O(n)
    //in time to find the nth the elementin total O(2n)
    TownID nth_distance(unsigned int n);

    // Estimate of performance:O(1)
    // Short rationale for estimate: it is constant for calling find function in time
    //and constant for checking if condition and all the other condition and return true
    bool add_vassalship(TownID vassalid, TownID masterid);

    // Estimate of performance:O(1+m)
    // Short rationale for estimate:constant for finding the target town,and linear in how many
    //masters it has in accessing the master then adding the ID to vector
    std::vector<TownID> taxer_path(TownID id);

    // Non-compulsory operations

    // Estimate of performance:
    // Short rationale for estimate:
    bool remove_town(TownID id);

    // Estimate of performance:O(n*log(n))
    // Short rationale for estimate:O(n) in time for copying the vector, and O(n*log(n))
    //for sorting the vec based on new origin, then O(n) for getting the IDs vector.
    std::vector<TownID> towns_distance_increasing_from(int x, int y);

    // Estimate of performance:
    // Short rationale for estimate:
    std::vector<TownID> longest_vassal_path(TownID id);

    // Estimate of performance:
    // Short rationale for estimate:
    int total_net_tax(TownID id);

private:
    // Add stuff needed for your class implementation here
    std::unordered_map<TownID,TownData*> allData_ ;
    int disSortedFlag_;
    int alphSortedFlag_;
    std::vector<TownID> sortDist_;
    std::vector<TownID> sortAlpha_;



};

#endif // DATASTRUCTURES_HH
