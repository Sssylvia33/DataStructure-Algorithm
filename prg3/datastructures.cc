
// Datastructures.cc

#include "datastructures.hh"

#include <random>
#include<bits/stdc++.h>
#include <cmath>
#include <utility>
#include <stack>
#include <queue>

std::minstd_rand rand_engine; // Reasonably quick pseudo-random generator

template <typename Type>
Type random_in_range(Type start, Type end)
{
    auto range = end-start;
    ++range;

    auto num = std::uniform_int_distribution<unsigned long int>(0, range-1)(rand_engine);

    return static_cast<Type>(start+num);
}

//comparator object for distance comparing to different origin
class cmp {
    Coord xy_;
public:
    //x,y=new origin
    cmp(Coord XY) : xy_(XY) {}

    bool operator()(std::pair<TownID, TownData*> town1 ,std::pair<TownID, TownData*> town2) {
        int dist1 = abs(town1.second->xy.first-xy_.first)+abs(town1.second->xy.second-xy_.second);
        int dist2 = abs(town2.second->xy.first-xy_.first)+abs(town2.second->xy.second-xy_.second);
        return dist1 < dist2;
    }
};



Datastructures::Datastructures()
{
    disSortedFlag_ =0;
    alphSortedFlag_ =0;
}

Datastructures::~Datastructures()
{
 // Replace with actual implementation
}

unsigned int Datastructures::size()
{
    return allData_.size();
}

void Datastructures::clear()
{    allData_.clear();
     sortDist_.clear();
     sortAlpha_.clear();
     disSortedFlag_ =0;
     alphSortedFlag_ =0;
}

std::string Datastructures::get_name(TownID id)
{
    auto town_itr = allData_.find(id);
    if(town_itr == allData_.end()){return NO_NAME;}

    auto name = town_itr->second->name;
    return name;

}

Coord Datastructures::get_coordinates(TownID id)
{   auto town_itr = allData_.find(id);

    if(town_itr == allData_.end()){return {NO_VALUE, NO_VALUE}; }

    //std::pair<int,int> coordinate = {town_itr->second->x,town_itr->second->y};

    return town_itr->second->xy;
}

int Datastructures::get_tax(TownID id)
{
    auto town_itr = allData_.find(id);

    if(town_itr == allData_.end()){return NO_VALUE;}

    int tax = town_itr->second->tax;
    return tax;
}

std::vector<TownID> Datastructures::get_vassals(TownID id)
{
    auto town_itr = allData_.find(id);

    if(town_itr == allData_.end()){return {NO_ID};}
    auto vassals = town_itr->second->vassals;
    std::vector<TownID> vassals_list;
    for(auto i=0;i<vassals.size();++i){
        auto id = vassals.at(i)->id;
        vassals_list.push_back(id);
    }
    std::sort(vassals_list.begin(),vassals_list.end());
    return vassals_list;
}

std::vector<TownID> Datastructures::all_towns()
{
    std::vector<TownID> townID_list;
    for (auto const& pair : allData_){
        townID_list.push_back(pair.first);
    }
    return townID_list;
}

bool Datastructures::add_town(TownID id, const std::string& name, int x, int y, int tax)
{
    if (allData_.find(id) != allData_.end()){
        return false;
    }else{

        TownData* newTown = new TownData;
        newTown->id = id;
        newTown->name = name;
        newTown->xy = std::make_pair(x,y);
        newTown->tax = tax;
        newTown->vassals= {};
        newTown->master = nullptr;
        newTown->routes ={};
        newTown->visited = 0;
        newTown->rFrom = nullptr;
        newTown->d = INFINITY;


        allData_.insert({id,newTown});
        alphSortedFlag_ =0;
        disSortedFlag_ =0;

        return true;
}}

bool Datastructures::change_town_name(TownID id, const std::string& newname){
    auto town_itr = allData_.find(id);

    if(town_itr == allData_.end()){return false;}

    town_itr->second->name = newname;
    return true;
}

bool Datastructures::remove_town(TownID id){
    auto removedTown = allData_.find(id);
    if (removedTown == allData_.end()){
        return false;
    }
    auto town_itr = removedTown->second;
    if (town_itr->master != nullptr and town_itr->vassals.size() !=0 ){
        //add the vassals of removedTown to its master town vassal_list
        town_itr->master->vassals.insert(town_itr->master->vassals.end(),
                                         town_itr->vassals.begin(), town_itr->vassals.end());

        //delete removedTowm from its master vassal_list
        auto v_of_mas =town_itr->master->vassals;
        for(auto i=0; i< v_of_mas.size();++i){
            if(v_of_mas.at(i)==town_itr){
                v_of_mas.erase(v_of_mas.begin()+i);
            }
        }


        // delete removedTown from its vassal towns master_list and
        //change it to the new master
        for(auto i=0; i <town_itr->vassals.size();++i){
            town_itr->vassals.at(i)->master = town_itr->master;
        }

     }else if(town_itr->master == nullptr){
            for(auto i=0; i <town_itr->vassals.size();++i){
                town_itr->vassals.at(i)->master = nullptr;
        }

    }else if(town_itr->vassals.size() ==0){

        auto v_of_mas =town_itr->master->vassals;
        for(auto i=0; i< v_of_mas.size();++i){
            if(v_of_mas.at(i)==town_itr){
                v_of_mas.erase(v_of_mas.begin()+i);
            }
        }
     }
    allData_.erase(id);
    disSortedFlag_=0;
    alphSortedFlag_=0;
    return true;
}

//function for comparing the names
bool compName (std::pair<TownID, TownData*> town1 ,std::pair<TownID, TownData*> town2)
            {
                return town1.second->name < town2.second->name;
            }

std::vector<TownID> Datastructures::towns_alphabetically()
{
    sortAlpha_.clear();

    //ceate a list for the storing sorted towns
    std::vector<std::pair<TownID, TownData*>> elems(allData_.begin(), allData_.end());

    std::sort(elems.begin(), elems.end(), compName);

    //get the ids of sorted towns
    for (auto const& pair : elems){
        sortAlpha_.push_back(pair.first);
    }
    alphSortedFlag_ = 1;
    return sortAlpha_;
}

std::vector<TownID> Datastructures::towns_distance_increasing()
{
    //ceate a list for the storing sorted towns
    std::vector<std::pair<TownID, TownData*>> elems(allData_.begin(), allData_.end());

    std::sort(elems.begin(), elems.end(), cmp({0,0}));

    //get the ids of sorted towns
    sortDist_.clear();
    for (auto const& pair : elems){
        sortDist_.push_back(pair.first);
    }
    disSortedFlag_ = 1;
    return sortDist_;

}

std::vector<TownID> Datastructures::find_towns(std::string const& name)
{
    std::vector<TownID> id_list = {};
    for(auto i = allData_.begin(); i != allData_.end(); i++){
        if(i->second->name == name){
            id_list.push_back(i->first);
        }
    }
    std::sort(id_list.begin(),id_list.end());
    return id_list;
}

TownID Datastructures::min_distance()
{
    if(allData_.size()==0){return NO_ID;}

    if(disSortedFlag_==1){
        return sortDist_.at(0);
    }
    return towns_distance_increasing().at(0);
}

TownID Datastructures::max_distance()
{
    int last = allData_.size();
    if(last==0){return NO_ID;}

    if(disSortedFlag_==1){
        return sortDist_.at(last-1);
    }
    return towns_distance_increasing().at(last-1);
}

TownID Datastructures::nth_distance(unsigned int n)
{
    if(n==0|| n>allData_.size()){
        return NO_ID;
    }
    if(disSortedFlag_==1){
        return sortDist_.at(n-1);
    }

    std::vector<std::pair<TownID, TownData*>> elems(allData_.begin(), allData_.end());
    std::nth_element(elems.begin(), elems.begin() + n-1, elems.end(),cmp({0,0}));
    return elems.at(n-1).first;

}

std::vector<TownID> Datastructures::towns_distance_increasing_from(int x, int y)
{
    std::vector<std::pair<TownID, TownData*>> elems(allData_.begin(), allData_.end());

    std::sort(elems.begin(), elems.end(),cmp({x,y}));

    //get the ids of sorted towns
    std::vector<TownID> list_id;
    for (auto const& pair : elems){
        list_id.push_back(pair.first);
    }
    return list_id;
}

bool Datastructures::add_vassalship(TownID vassalid, TownID masterid)
{
    auto vassal_itr = allData_.find(vassalid);
    auto master_itr = allData_.find(masterid);

    if(vassal_itr == allData_.end()||master_itr == allData_.end()){return false;}
    else if(vassal_itr->second->master != nullptr){return false;}

    master_itr->second->vassals.push_back(vassal_itr->second);
    vassal_itr->second->master = master_itr->second;
    return true;
}

std::vector<TownID> Datastructures::taxer_path(TownID id)
{
    std::vector<TownID> taxer={};
    auto town_itr = allData_.find(id);

    if(town_itr == allData_.end()){return taxer;}

    auto town = town_itr->second;
    taxer.push_back(town->id);
    while(town->master!=nullptr){
        TownID master_id = town->master->id;
        taxer.push_back(master_id);
        town = town->master;
    }
    return taxer;
}

std::vector<std::pair<TownID, TownID>> Datastructures::all_roads()
{   std::vector<std::pair<TownID, TownID>> allRoads;
    std::pair<TownID, TownID> singleRoad;
    for (auto it = allData_.begin();it != allData_.end();++it){
        auto roads = it->second->routes;
        TownID t1ID = it->second->id;
        for (auto k=0; k< roads.size();++k){
            TownID t2ID = roads.at(k)->id;
            if(t1ID<t2ID){

                singleRoad = {t1ID,t2ID};
            }
            else{
                singleRoad = {t2ID,t1ID};
            }
            //check if it is already existed
            if(find(allRoads.begin(),allRoads.end(),singleRoad) == allRoads.end())
                allRoads.push_back(singleRoad);
        }
    }
    std::sort(allRoads.begin(),allRoads.end());

    return allRoads;

}

std::vector<TownID> Datastructures::get_roads_from(TownID id)
{   auto town_itr = allData_.find(id);
    std::vector<TownID> roadsfrom;

    //town dose not exist
    if(town_itr == allData_.end()){return {NO_ID};}
    for(auto i=0; i< town_itr->second->routes.size();++i){
        TownID id = town_itr->second->routes.at(i)->id;
        roadsfrom.push_back(id);
    }
    std::sort(roadsfrom.begin(),roadsfrom.end());
    return roadsfrom;

}

bool Datastructures::add_road(TownID town1, TownID town2)
{
    auto town1_itr = allData_.find(town1);
    auto town2_itr = allData_.find(town2);
    //town dose not exist
    if(town1_itr == allData_.end()|| town2_itr == allData_.end()){return false;}

    //check if there is already a route

    for(auto i=0; i< town1_itr->second->routes.size();++i){
        if(town1_itr->second->routes.at(i)==town2_itr->second){
            return false;
        }
    }
    //not find, add roads
    town1_itr->second->routes.push_back(town2_itr->second);
    town2_itr->second->routes.push_back(town1_itr->second);
    return true;

}

bool Datastructures::remove_road(TownID town1, TownID town2)
{
    auto town1_itr = allData_.find(town1);
    auto town2_itr = allData_.find(town2);
    //town dose not exist
    if(town1_itr == allData_.end()|| town2_itr == allData_.end()){return false;}

    //check if there is route ,then delete it and return
    //auto roads1=town1_itr->second->routes;
    for(auto i=0; i< town1_itr->second->routes.size();++i){
        if(town1_itr->second->routes.at(i)==town2_itr->second){
            town1_itr->second->routes.erase(town1_itr->second->routes.begin()+i);
        }
    }
    //auto roads2=town2_itr->second->routes;
    for(auto i=0; i< town2_itr->second->routes.size();++i){
        if(town2_itr->second->routes.at(i)==town1_itr->second){
            town2_itr->second->routes.erase(town2_itr->second->routes.begin()+i);
            return true;
        }
    }

    return false;

}

void Datastructures::clear_roads()
{
    for (auto it = allData_.begin();it != allData_.end();++it){
        it->second->routes.clear();
    }

}



void DFS_visit(std::stack <TownData*> V,TownData* toid){
    auto thisTown = V.top();

    for (auto i=0; i< thisTown->routes.size();++i){
        auto nextTown = thisTown->routes.at(i);
        if(nextTown->id == toid->id){
            nextTown->rFrom = thisTown;
            return;
        }
        if (nextTown->visited == 0){
            nextTown->visited = 1;
            V.push(nextTown);
            nextTown->rFrom = thisTown;
            DFS_visit(V,toid);
        }
        V.pop();
        return;

    }
}



std::vector<TownID> Datastructures::any_route(TownID fromid, TownID toid)
{   auto from_itr = allData_.find(fromid);
    auto to_itr = allData_.find(toid);
    //town dose not exist
    if(from_itr == allData_.end()|| to_itr == allData_.end()){return {NO_ID};}

    for (auto it = allData_.begin();it != allData_.end();++it){
        if (it->second->visited!=0){
        it->second->visited = 0;
        it->second->rFrom = nullptr;}
    }

    from_itr->second->visited = 1;
    std::stack <TownData*> visitors;
    visitors.push(from_itr->second);
    DFS_visit(visitors,to_itr->second);

    std::vector<TownID> route;

    auto helpNode = to_itr->second;
    while(helpNode->rFrom != nullptr){
        route.push_back(helpNode->id);
        helpNode = helpNode->rFrom;
    }
    if(route.size()!=0){
        route.push_back(from_itr->second->id);
    }
    std::reverse(std::begin(route), std::end(route));

    return route;


}



std::pair<TownID, int> find_longVpath(TownData* town){
    if(town->vassals.size()==0){
        std::pair<TownID, int> node = { town->id,0};
        return node ;
    }else{
        std::vector<std::pair<TownID, int>> all_options={};
        for(auto i=0; i<town->vassals.size();++i){
           auto option = find_longVpath(town->vassals.at(i));
           all_options.push_back(option);
        }
        auto node = std::max_element(all_options.begin(),all_options.end(),[](auto a, auto b){
            return a.second < b.second;
        });
        //auto node = all_options.at(pos);
        node->second = node->second+1;
        return *node;

    }

}


std::vector<TownID> Datastructures::longest_vassal_path(TownID id)
{
    std::vector<TownID> vassalsList={};

    auto town_itr = allData_.find(id);
    if(town_itr == allData_.end()){
        return vassalsList;}
    auto town =town_itr->second;
    auto leaf = find_longVpath(town);
    auto masterList = taxer_path(leaf.first);
    int num = leaf.second;

    for(auto i =num; i>=0;--i){
        vassalsList.push_back(masterList.at(i));
    }

    return vassalsList;

}

int payTax(TownData* town, int isLast=0){
    int tax2M =0;
    int vSize = town->vassals.size();
    if(vSize==0){
        tax2M = floor(town->tax/10);
        return tax2M;
    }
    for(auto i=0; i <vSize; ++i ){
        tax2M += payTax(town->vassals.at(i));
    }

    //if the town is the one we want to know the
    //net tax, the function will only return the tax
    //by its vassals
    if(isLast !=0){
        return tax2M;
    }

    tax2M = floor((town->tax+tax2M)/10);
    return tax2M;
}


int Datastructures::total_net_tax(TownID id)
{
    auto town_itr = allData_.find(id);
    if(town_itr == allData_.end()){
        return NO_VALUE;
    }

    int netTax=0;
    auto town = town_itr->second;
    int tax_from_v;

    //for town has no vassals
    if(town->vassals.size()==0){
        tax_from_v= 0;
    }else {
        tax_from_v=payTax(town,1);
    }

    if(town->master == nullptr){
        netTax = town->tax+tax_from_v;
        return netTax;
    }else{
        netTax = (town->tax+tax_from_v)-floor((town->tax+tax_from_v)/10);
        return netTax;
    }
}


void BFS_visit(std::queue <TownData*> V, TownData* toTown){
    if(V.size()==0){
        return;
    }
    auto thisTown = V.front();
    V.pop();
    for (auto i=0; i< thisTown->routes.size();++i){
        auto nextTown = thisTown->routes.at(i);
        if(nextTown->id == toTown->id){
            nextTown->rFrom = thisTown;
            return;
        }
        if (nextTown->visited == 0){
            nextTown->visited = 1;
            V.push(nextTown);
            nextTown->rFrom = thisTown;
        }
    }
    BFS_visit(V,toTown);
}

std::vector<TownID> Datastructures::least_towns_route(TownID fromid, TownID toid)
{   auto from_itr = allData_.find(fromid);
    auto to_itr = allData_.find(toid);
    //town dose not exist
    if(from_itr == allData_.end()|| to_itr == allData_.end()){return {NO_ID};}

    for (auto it = allData_.begin();it != allData_.end();++it){
        if (it->second->visited!=0){it->second->visited = 0;
        it->second->rFrom = nullptr;}

    }

    from_itr->second->visited = 1;
    std::queue <TownData*> visitors;
    visitors.push(from_itr->second);
    BFS_visit(visitors,to_itr->second);

    std::vector<TownID> route;

    auto helpNode = to_itr->second;
    while(helpNode->rFrom != from_itr->second->rFrom && helpNode->rFrom !=nullptr){
        route.push_back(helpNode->id);
        helpNode = helpNode->rFrom;
    }
    if(route.size()!=0){
        route.push_back(from_itr->second->id);
    }
    std::reverse(std::begin(route), std::end(route));

    return route;// Replace with actual implementation
}

void AstarRelax(TownData* A,TownData* B,int w){
    if(A->d + w < B->d){
        B->d = A->d + w;
        B->rFrom = A;
    }
    return;
}

void A_Star(std::vector <TownData*> V){
    if(V.size()==0){
        return;
    }

    auto cmp = [](TownData* left, TownData* right) { return (left->d) < (right->d);};

    auto thisTown = *std::min_element(V.begin(),V.end(),cmp);


    int w;
    for (auto i=0; i< thisTown->routes.size();++i){
        auto nextTown = thisTown->routes.at(i);

        if (nextTown->visited == 0){
            nextTown->visited = 1;
            V.push_back(nextTown);
        }
        //calculate distance and update the distance
        w = abs(thisTown->xy.first-nextTown->xy.first)+abs(thisTown->xy.second-nextTown->xy.second);
        AstarRelax(thisTown,nextTown,w);
    }
    V.erase(std::find(V.begin(),V.end(),thisTown));
    A_Star(V);
}

std::vector<TownID> Datastructures::shortest_route(TownID fromid, TownID toid)
{   auto from_itr = allData_.find(fromid);
    auto to_itr = allData_.find(toid);
    //town dose not exist
    if(from_itr == allData_.end()|| to_itr == allData_.end()){return {NO_ID};}

    for (auto it = allData_.begin();it != allData_.end();++it){
        if (it->second->visited!=0){
        it->second->visited = 0;
        it->second-> d =INFINITY;
        it->second->rFrom = nullptr;}

    }

    from_itr->second->visited = 1;
    from_itr->second->d=0;

    //auto cmp = [](TownData* left, TownData* right) { return (left->d) > (right->d);};
    //std::priority_queue<TownData*, std::vector<TownData*>, decltype(cmp)> visitors(cmp);

    std::vector <TownData*> visitors;

    visitors.push_back(from_itr->second);
    A_Star(visitors);

    std::vector<TownID> route;
    auto helpNode = to_itr->second;
    while(helpNode->rFrom != nullptr){
        route.push_back(helpNode->id);
        helpNode = helpNode->rFrom;
    }
    if(route.size()!=0){
        route.push_back(from_itr->second->id);
        std::reverse(route.begin(), route.end());
    }


    return route; // Replace with actual implementation
}



TownData* CycleFind(TownData* thisTown,std::vector <TownData*> V){


    V.push_back(thisTown);

    for (auto i=0; i< thisTown->routes.size();++i){
        auto nextTown = thisTown->routes.at(i);
        if (nextTown->visited == 0){
            nextTown->visited = 1;

            nextTown->rFrom = thisTown;
            TownData* cycle = CycleFind(nextTown,V);
            if(cycle !=nullptr){return cycle;}
        }
        else if(thisTown->rFrom != nextTown){
            nextTown->rFrom = thisTown;
            V.push_back(nextTown);
            return nextTown;
        }
    }
    //nothing found
    return nullptr;
}

std::vector<TownID> Datastructures::road_cycle_route(TownID startid)
{
        auto from_itr = allData_.find(startid);

        //town dose not exist
        if(from_itr == allData_.end()){return {NO_ID};}

        for (auto it = allData_.begin();it != allData_.end();++it){
            if (it->second->visited!=0){
            it->second->visited = 0;
            it->second->rFrom = nullptr;}}

        std::vector <TownData*> visitors;

        auto cyclePoint = CycleFind(from_itr->second,visitors);

        if (cyclePoint==nullptr){
            return {};
        }

        std::vector<TownID> route;

        auto helpNode = cyclePoint;

        while(helpNode->rFrom != cyclePoint){
            route.push_back(helpNode->id);
            helpNode = helpNode->rFrom;
        }
        route.push_back(helpNode->id);
        route.push_back(cyclePoint->id);
        std::reverse(route.begin(), route.end());
        return route;
}

Dist Datastructures::trim_road_network()
{
    return NO_DIST;
}
