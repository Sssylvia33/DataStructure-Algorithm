// Datastructures.cc

#include "datastructures.hh"

#include <random>
#include <algorithm>

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
    int x_;
    int y_;
public:
    //x,y=new origin
    cmp(int x,int y) : x_(x),y_(y) {}

    bool operator()(std::pair<TownID, TownData*> town1 ,std::pair<TownID, TownData*> town2) {
        int dist1 = abs(town1.second->x-x_)+abs(town1.second->y-y_);
        int dist2 = abs(town2.second->x-x_)+abs(town2.second->y-y_);
        return dist1 < dist2;
    }
};

Datastructures::Datastructures(){
    disSortedFlag_ =0;
    alphSortedFlag_ =0;

}

Datastructures::~Datastructures(){


}

unsigned int Datastructures::size(){
    return allData_.size();
}

void Datastructures::clear(){
    allData_.clear();
    sortDist_.clear();
    sortAlpha_.clear();
    disSortedFlag_ =0;
    alphSortedFlag_ =0;
}

std::string Datastructures::get_name(TownID id){

    auto town_itr = allData_.find(id);
    if(town_itr == allData_.end()){return NO_NAME;}

    auto name = town_itr->second->name;
    return name;

}

std::pair<int, int> Datastructures::get_coordinates(TownID id){

    auto town_itr = allData_.find(id);

    if(town_itr == allData_.end()){return {NO_VALUE, NO_VALUE}; }

    std::pair<int,int> coordinate = {town_itr->second->x,town_itr->second->y};
    return coordinate;


}

int Datastructures::get_tax(TownID id){
    auto town_itr = allData_.find(id);

    if(town_itr == allData_.end()){return NO_VALUE;}

    int tax = town_itr->second->tax;
    return tax;
}

std::vector<TownID> Datastructures::get_vassals(TownID id){
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

std::vector<TownID> Datastructures::all_towns(){
    std::vector<TownID> townID_list;
    for (auto const& pair : allData_){
        townID_list.push_back(pair.first);
    }
    return townID_list;

}

bool Datastructures::add_town(TownID id, const std::string& name, int x, int y, int tax){
    if (allData_.find(id) != allData_.end()){
        return false;
    }else{

        TownData* newTown = new TownData;
        newTown->id = id;
        newTown->name = name;
        newTown->x = x;
        newTown->y = y;
        newTown->tax = tax;
        newTown->vassals= {};
        newTown->master = nullptr;

        allData_.insert({id,newTown});
        alphSortedFlag_ =0;
        disSortedFlag_ =0;

        return true;
   }
}


bool Datastructures::change_town_name(TownID id, const std::string& newname)
{
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


std::vector<TownID> Datastructures::towns_alphabetically(){
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


std::vector<TownID> Datastructures::towns_distance_increasing(){
    //ceate a list for the storing sorted towns
    std::vector<std::pair<TownID, TownData*>> elems(allData_.begin(), allData_.end());

    std::sort(elems.begin(), elems.end(), cmp(0,0));

    //get the ids of sorted towns
    sortDist_.clear();
    for (auto const& pair : elems){
        sortDist_.push_back(pair.first);
    }
    disSortedFlag_ = 1;
    return sortDist_;

}

std::vector<TownID> Datastructures::find_towns(std::string const& name){

    std::vector<TownID> id_list = {};
    for(auto i = allData_.begin(); i != allData_.end(); i++){
        if(i->second->name == name){
            id_list.push_back(i->first);
        }
    }
    std::sort(id_list.begin(),id_list.end());
    return id_list;
}

TownID Datastructures::min_distance(){
    if(allData_.size()==0){return NO_ID;}

    if(disSortedFlag_==1){
        return sortDist_.at(0);
    }
    return towns_distance_increasing().at(0);
}

TownID Datastructures::max_distance(){
    int last = allData_.size();
    if(last==0){return NO_ID;}

    if(disSortedFlag_==1){
        return sortDist_.at(last-1);
    }
    return towns_distance_increasing().at(last-1);
}

TownID Datastructures::nth_distance(unsigned int n){
    if(n==0|| n>allData_.size()){
        return NO_ID;
    }
    if(disSortedFlag_==1){
        return sortDist_.at(n-1);
    }

    std::vector<std::pair<TownID, TownData*>> elems(allData_.begin(), allData_.end());
    std::nth_element(elems.begin(), elems.begin() + n-1, elems.end(),cmp(0,0));
    return elems.at(n-1).first;


}

std::vector<TownID> Datastructures::towns_distance_increasing_from(int x, int y)
{
    std::vector<std::pair<TownID, TownData*>> elems(allData_.begin(), allData_.end());

    std::sort(elems.begin(), elems.end(),cmp(x,y));

    //get the ids of sorted towns
    std::vector<TownID> list_id;
    for (auto const& pair : elems){
        list_id.push_back(pair.first);
    }
    return list_id;
}

bool Datastructures::add_vassalship(TownID vassalid, TownID masterid){
    auto vassal_itr = allData_.find(vassalid);
    auto master_itr = allData_.find(masterid);

    if(vassal_itr == allData_.end()||master_itr == allData_.end()){return false;}
    else if(vassal_itr->second->master != nullptr){return false;}

    master_itr->second->vassals.push_back(vassal_itr->second);
    vassal_itr->second->master = master_itr->second;
    return true;
}

std::vector<TownID> Datastructures::taxer_path(TownID id){
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

std::vector<TownID> Datastructures::longest_vassal_path(TownID id){
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

int Datastructures::total_net_tax(TownID id){
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
