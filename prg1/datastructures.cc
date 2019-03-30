//GUO JIAQI 268459
//TIE-20106 ASSIGNMENT 1

// Datastructures.cc

#include "datastructures.hh"
#include <cstdlib>
#include <random>
#include <iostream>
#include <memory>
using namespace std;



std::minstd_rand rand_engine; // Reasonably quick pseudo-random generator

template <typename Type>
Type random_in_range(Type start, Type end)
{
    auto range = end-start;
    ++range;

    auto num = std::uniform_int_distribution<unsigned long int>(0, range-1)(rand_engine);

    return static_cast<Type>(start+num);
}



Datastructures::Datastructures(){
    data_number_ = 0;

}

Datastructures::~Datastructures()
{

}

unsigned int Datastructures::size(){
    return data_number_;
}

void Datastructures::clear(){
    data_list_.clear();
    data_number_ = 0 ;

}

std::vector<TownData*> Datastructures::all_towns(){
    return data_list_; // Replace with actual implementation
}

TownData* Datastructures::add_town(const std::string& name, int x, int y){

    TownData* newData = new TownData;
    newData->name = name;
    newData->x = x;
    newData->y = y;
    newData->dz = abs(x)+abs(y);

    data_list_.push_back(newData);
    data_number_++;
    return newData;

}

int alphabet_random_partition(vector<TownData*>& vec, int left, int right){

    int pIdx = random_in_range(left, right);
    auto pivot = vec[pIdx]->name;
    swap(vec[pIdx], vec[right]); // move pivot element to the end
    pIdx = right;
    int i = left -1;

    for(int j=left; j<=right-1; j++)
    {
        if(vec[j]->name <= pivot)
        {
            i = i+1;
            swap(vec[i], vec[j]);
        }
    }

    swap(vec[i+1], vec[pIdx]);
    return i+1;
}

void alphabet_random_quick_sort(vector<TownData*>& vec, int left, int right){
    if(left < right) {
        int mid = alphabet_random_partition(vec, left, right);
        alphabet_random_quick_sort(vec,left, mid-1);
        alphabet_random_quick_sort(vec, mid+1, right);
    }
}

std::vector<TownData*> Datastructures::towns_alphabetically(){
    alphabet_random_quick_sort(data_list_, 0, data_number_-1);
    return data_list_;
}


int distance_random_partition(vector<TownData*>& vec,  int left,  int right){

    int pIdx = random_in_range(left, right);
    auto pivot = vec[pIdx]->dz;
    swap(vec[pIdx], vec[right]); // move pivot element to the end
    pIdx = right;
    int i = left -1;

    for(unsigned int j=left; j<=right-1; j++)
    {
        if(vec[j]->dz <= pivot)
        {
            i = i+1;
            swap(vec[i], vec[j]);
        }
    }

    swap(vec[i+1], vec[pIdx]);
    return i+1;
}

void dist_random_quick_sort(vector<TownData*>& vec,int left, int right){
     if(left < right) {
        unsigned int mid = distance_random_partition(vec, left, right);
        dist_random_quick_sort(vec, left, mid-1);
        dist_random_quick_sort(vec, mid+1, right);

    }
}

std::vector<TownData*> Datastructures::towns_distance_increasing(){
    dist_random_quick_sort(data_list_,0 ,data_number_-1);
    return data_list_;
}

TownData* Datastructures::find_town(std::string const& name){

    for(TownData* data:data_list_){
        if(data->name == name){
            return data;
        }
    }
    return nullptr; // Replace with actual implementation
}

TownData* Datastructures::min_distance(){
    if(data_number_ == 0){
        return nullptr;
    }else{
        dist_random_quick_sort(data_list_,0,data_number_-1);
        return data_list_.front();
    }
}



TownData* Datastructures::max_distance(){
    if(data_number_ == 0){
        return nullptr;
    } else if(data_number_ == 1){
        return data_list_[0];
    }else{
        dist_random_quick_sort(data_list_,0,data_number_-1);
        return data_list_.at(data_number_ - 1);
    }

}

TownData* select_nth(vector<TownData*>& A, unsigned int n){
    int right = A.size();
    auto r = random_in_range(0,right-1);
    auto pivot = A[r]->dz;
    vector<TownData*> A1,A2;

    //A1 of small elements and A2 of big elements
    for(auto i = 0; i< A.size();i++){
        if(A[i]->dz < pivot){
            A1.push_back(A[i]);
        }
        else if(A[i]->dz > pivot){
            A2.push_back(A[i]);
        }
    }

    if (n <= A1.size()){
        return select_nth(A1, n);
    }
    else if(n > (A.size() - A2.size())){
        return select_nth(A2, (n - (A.size() - A2.size())));
    }else{
        return A[r];
    }

}

TownData* Datastructures::nth_distance(unsigned int n){
    if(n==0 or n>data_number_){
        return nullptr;
    }
    return select_nth(data_list_,n);



}

void Datastructures::remove_town(const std::string& town_name){

    for(auto itr=0; itr!=data_number_; itr++){
        if(data_list_.at(itr)-> name == town_name){
            data_list_.erase(data_list_.begin()+itr);
            data_number_--;
            break;
        }
    }

}

std::vector<TownData*> Datastructures::towns_distance_increasing_from(int x, int y){
    vector<TownData*> difference;
    for(auto i =0;i<data_number_;i++){
        difference.push_back( data_list_[i]);
        difference[i]->dz =abs(data_list_[i]->x -x)+abs(data_list_[i]->y -y);
    }
    dist_random_quick_sort(difference,0,data_number_-1);
    return difference;


}
