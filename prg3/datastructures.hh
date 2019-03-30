// Datastructures.hh

#ifndef DATASTRUCTURES_HH
#define DATASTRUCTURES_HH

#include <string>
#include <vector>
#include <utility>
#include <limits>
#include <unordered_map>
#include <algorithm>

// Type for town IDs
using TownID = std::string;

// Return value for cases where required town was not found
TownID const NO_ID = "----------";

// Return value for cases where integer values were not found
int const NO_VALUE = std::numeric_limits<int>::min();

// Return value for cases where name values were not found
std::string const NO_NAME = "-- unknown --";

// Type for a coordinate (x, y)
using Coord = std::pair<int, int>;

// Return value for cases where coordinates were not found
Coord const NO_COORD{NO_VALUE, NO_VALUE};

// Type for distance
using Dist = int;

// Return value for cases where distance is unknown
Dist const NO_DIST = NO_VALUE;


struct TownData{
    TownID id;
    std::string name;
    Coord xy;
    Dist d;
    int tax;
    std::vector<TownData*> vassals;
    TownData* master;
    std::vector<TownData*> routes;
    int visited;
    TownData* rFrom;

};

class Datastructures
{
public:
    Datastructures();
    ~Datastructures();

    //! !!!!!!!performance of operation from project 2 are skipped
    //only the performance of new member function is written

    // Estimate of performance:
    // Short rationale for estimate:
    unsigned int size();

    // Estimate of performance:
    // Short rationale for estimate:
    void clear();

    // Estimate of performance:
    // Short rationale for estimate:
    std::string get_name(TownID id);

    // Estimate of performance:
    // Short rationale for estimate:
    Coord get_coordinates(TownID id);

    // Estimate of performance:
    // Short rationale for estimate:
    int get_tax(TownID id);

    // Estimate of performance:
    // Short rationale for estimate:
    std::vector<TownID> get_vassals(TownID id);

    // Estimate of performance:
    // Short rationale for estimate:
    std::vector<TownID> all_towns();

    // Estimate of performance:
    // Short rationale for estimate:
    bool add_town(TownID id, std::string const& name, int x, int y, int tax);

    // Estimate of performance:
    // Short rationale for estimate:
    bool change_town_name(TownID id, std::string const& newname);

    // Estimate of performance:
    // Short rationale for estimate:
    std::vector<TownID> towns_alphabetically();

    // Estimate of performance:
    // Short rationale for estimate:
    std::vector<TownID> towns_distance_increasing();

    // Estimate of performance:
    // Short rationale for estimate:
    std::vector<TownID> find_towns(std::string const& name);

    // Estimate of performance:
    // Short rationale for estimate:
    TownID min_distance();

    // Estimate of performance:
    // Short rationale for estimate:
    TownID max_distance();

    // Estimate of performance:
    // Short rationale for estimate:
    TownID nth_distance(unsigned int n);

    // Estimate of performance:
    // Short rationale for estimate:
    bool add_vassalship(TownID vassalid, TownID masterid);

    // Estimate of performance:
    // Short rationale for estimate:
    std::vector<TownID> taxer_path(TownID id);

    // Estimate of performance:linear
    // Short rationale for estimate:go through all the towns in the map
    std::vector<std::pair<TownID, TownID>> all_roads();

    // Estimate of performance:linear
    // Short rationale for estimate: go through its children roads, time complexity depands on the number of children roads
    std::vector<TownID> get_roads_from(TownID id);

    // Estimate of performance:O(m)
    // Short rationale for estimate: the time complexity is constant,using find function of unodered_map
    // and then check the itr position and go through its roads which is linear
    bool add_road(TownID town1, TownID town2);

    // Estimate of performance:linear
    // Short rationale for estimate:check if towns are exist, and go through the children roads
    bool remove_road(TownID town1, TownID town2);

    // Estimate of performance:
    // Short rationale for estimate:
    void clear_roads();

    // Estimate of performance:O(N+M)
    // Short rationale for estimate: a DFS searching algorithm
    std::vector<TownID> any_route(TownID fromid, TownID toid);

    // Non-compulsory operations

    // Estimate of performance:
    // Short rationale for estimate:
    bool remove_town(TownID id);

    // Estimate of performance:
    // Short rationale for estimate:
    std::vector<TownID> towns_distance_increasing_from(int x, int y);

    // Estimate of performance:
    // Short rationale for estimate:
    std::vector<TownID> longest_vassal_path(TownID id);

    // Estimate of performance:
    // Short rationale for estimate:
    int total_net_tax(TownID id);

    // Estimate of performance:O(N+M)
    // Short rationale for estimate:using BFS searching algorithm
    std::vector<TownID> least_towns_route(TownID fromid, TownID toid);

    // Estimate of performance:O(m+n*log(n)) Where m is the node
    // Short rationale for estimate: using Dijkstra's algorithm to find the
    // path with minimum cost
    std::vector<TownID> shortest_route(TownID fromid, TownID toid);

    // Estimate of performance:O(N+M)
    // Short rationale for estimate: searching algorithm based on DFS searching
    std::vector<TownID> road_cycle_route(TownID startid);

    // Estimate of performance:
    // Short rationale for estimate:
    Dist trim_road_network();




private:
    // Add stuff needed for your class implementation here
    std::unordered_map<TownID,TownData*> allData_ ;
    int disSortedFlag_;
    int alphSortedFlag_;
    std::vector<TownID> sortDist_;
    std::vector<TownID> sortAlpha_;

};

#endif // DATASTRUCTURES_HH
