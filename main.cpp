//PROJECT IDENTIFIER: 5949F553E20B650AB0FB2266D3C0822B13D248B0

#include <iostream>
#include <vector>
#include <stack>
#include <queue>
#include <fstream>
#include <string>
#include <sstream>
#include <getopt.h>
#include <iomanip>
#include <cmath>
#include <limits>

using namespace std;

struct node{
    uint32_t data;
    node* next;
};

struct pokemon{
    vector <bool> beenVisited;
    vector <pair <int, int> > coordinates;
    vector <double> distance;
    vector <uint32_t> parent;
    vector <char> terrain;
};

struct prims{
    bool k = false;
    double d = numeric_limits<double>::infinity();
    int p;
};

struct coord{
    int32_t x;
    int32_t y;
};

double weight = 0.0;
uint32_t amount;
uint32_t pivot = 0; 
pokemon map;

vector <uint32_t> path;
vector <uint32_t> bestPath;
double runningTotal = 0;
double bestDistance = 0;

vector<coord> locs;

bool partA = false;
bool partB = false;
bool partC = false;

bool isWater(pair<int, int> position){

    if(position.first < 0 && position.second < 0){
        return true;
    } else {
        return false;
    }
}

bool isCoast(pair<int, int> position){

    if(position.first == 0 && position.second == 0){
        return true;
    } else if(position.first == 0 && position.second < 0){
        return true;
    } else if(position.first < 0 && position.second == 0){
        return true;
    } else {
        return false;
    }

}

double calculateDistance(pair<int, int> a, pair <int, int> b, char aTerrain, char bTerrain){
    

    if((aTerrain == 'l' && bTerrain == 'w') || (aTerrain == 'w' && bTerrain == 'l')){
        return numeric_limits<double>::infinity();
    } else {
        int distanceA;
        int distanceB;
        double distanceSquared;
        distanceA = abs(a.first - b.first);
        distanceB = abs(a.second - b.second);
        distanceSquared = pow(distanceA, 2) + pow(distanceB, 2);

        return sqrt(distanceSquared);
    }

}

double calculateDist(pair<int, int> a, pair <int, int> b){
    return sqrt(double(a.first - b.first)*double(a.first - b.first) + double(a.second - b.second)*double(a.second - b.second));
}

double calcDist(uint32_t &c1, uint32_t &c2){
    return sqrt(double(locs[c1].x - locs[c2].x)*double(locs[c1].x - locs[c2].x) + double(locs[c1].y - locs[c2].y)*double(locs[c1].y - locs[c2].y));
}

void MST() {

    for(uint32_t i = 0; i < amount; i++){
        for(uint32_t j = 0; j < amount; j++){
            double distance = calculateDistance(map.coordinates[pivot], map.coordinates[j], map.terrain[pivot], map.terrain[j]);
            if(distance < map.distance[j]){
                map.distance[j] = distance;
                
                if(pivot != j){
                map.parent[j] = pivot;
                }
            }
        }
        double smallest = numeric_limits<double>::infinity();
        uint32_t smallestIndex;
        for(uint32_t k = 0; k < amount; k++){
            if(map.beenVisited[k] == false){
                if(map.distance[k] < smallest){
                    smallest = map.distance[k];
                    smallestIndex = k;
                }
            }
        }
        map.beenVisited[smallestIndex] = true;
        weight += map.distance[smallestIndex];
        pivot = smallestIndex;
    }

    cout << weight << "\n";


    for (uint32_t i = 1; i < amount; i++){
        if(i < map.parent[i]){
            cout << i << " " << map.parent[i] << "\n";
        } else {
            cout << map.parent[i] << " " << i << "\n";
        }
    }
    
}

void FASTTSP(){

    node* start = new node;
    node* end = new node;
    node* itsHere = start;
    node* insertInto = start;
    node* pointerNew;
    start -> data = 0;
    end -> data = 0;
    start -> next = end;
    end -> next = nullptr;
    
    
    for(uint32_t i = 1; i < amount; i++){
        double smallest = numeric_limits<double>::infinity();
        uint32_t wheretoInsert = 0;
        uint32_t dataInsert = 0;
        
        while(itsHere != end){
            
            double distance = calculateDist(map.coordinates[itsHere -> data], map.coordinates[i]);
            double distance2 = calculateDist(map.coordinates[i],map.coordinates[itsHere -> next -> data]);
            double distance3 = calculateDist(map.coordinates[itsHere -> data], map.coordinates[itsHere -> next -> data]);
            double total = distance + distance2 - distance3;
            if(total < smallest){
                smallest = total;
                wheretoInsert = itsHere -> data;
                dataInsert = i;
            }
            
            itsHere = itsHere -> next;
        }

        
        pointerNew = new node;
        pointerNew -> data = dataInsert;

        while(insertInto -> data != wheretoInsert){
            insertInto = insertInto -> next;
        }


        pointerNew -> next = insertInto -> next;

        double addWeight = smallest;
        
        weight += addWeight;

        insertInto -> next = pointerNew;
        insertInto = start;
        itsHere = start;
        
    }

    if(partB){
        cout << weight << "\n";
        while(itsHere != end){
            cout << itsHere -> data << " ";
            itsHere = itsHere -> next;
            
        }
    } else {
        while(itsHere != end){
            path.push_back(itsHere -> data);
            itsHere = itsHere -> next;
        }
    }


}

double lowerBound(uint32_t permLength){
    
    vector <prims> primTable(path.size() - permLength);
    primTable[0].d = 0;

    double localWeight = 0.0;
    uint32_t minIndex = 0;
    double nearest;
    double temp;
    

    for(uint32_t i = 0; i < primTable.size(); i++){
        nearest = numeric_limits<double>::infinity();
        for(uint32_t j = 0; j < primTable.size(); j++){
            if(primTable[j].d < nearest && (!primTable[j].k)){
                nearest = primTable[j].d;
                minIndex = j;
            }
        }
        localWeight += primTable[minIndex].d;
        primTable[minIndex].k = true;
        for(uint32_t z = 0; z < primTable.size(); z++){
            if(!primTable[z].k){
                temp = calcDist(path[z + permLength], path[minIndex + permLength]);
                if(temp < primTable[z].d){
                    primTable[z].d = temp;
                    primTable[z].p = int(minIndex);
                }
            }
        }

    }
    
    double smalllest1 = numeric_limits<double>::infinity();
    double smallest2 = numeric_limits<double>::infinity();

    for(uint32_t i = permLength; i < path.size(); i++){
        double temp = calcDist(path[0], path[i]);
        if (temp < smalllest1){
            smalllest1 = temp;
        }
    }
    
    for(uint32_t i = permLength; i < path.size(); i++){
        double temp = calcDist(path[permLength - 1], path[i]);
        if (temp < smallest2){
            smallest2 = temp;
        }
    }

    localWeight += smalllest1;
    localWeight += smallest2;

    return (runningTotal + localWeight);
}

bool promising(uint32_t &permLength){

    if(lowerBound(permLength) < bestDistance){
        
        return true;
    }
    return false;
    
}

void genPerms(uint32_t permLength) {
    
    if (permLength == path.size()) {
        if(runningTotal < bestDistance){
            bestDistance = runningTotal + calcDist(path[0], path[path.size() - 1]);
            bestPath = path;
        }
    return;
    }
    
    if (!promising(permLength)){
        return;
    }  
    
    for (size_t i = permLength; i < path.size(); ++i) {
        swap(path[permLength], path[i]);
        double cost = calcDist(path[permLength - 1], path[permLength]);
        runningTotal += cost;
        genPerms(permLength + 1);
        runningTotal -= cost;
        swap(path[permLength], path[i]);
    } 
} 

void OPTTSP(){

    FASTTSP();
    bestDistance = weight;
    
    genPerms(1);
    
    cout << bestDistance << "\n";
    for(uint32_t i = 0; i < bestPath.size(); i++){
        cout << bestPath[i] << " ";
    }
}

int main(int argc, char** argv){

ios_base::sync_with_stdio(false);


        int option_index = 0, option = 0;
        
        opterr = false;
        
        struct option longOpts[] = {{"help", no_argument, nullptr, 'h'},
                                    {"mode", required_argument, nullptr, 'm'},
                                    {nullptr, 0, nullptr, '\0'}};
    
        while ((option = getopt_long(argc, argv, "hm:", longOpts, &option_index)) != -1) {
            switch (option) {
                case 'h':
                    cout << "This is a call for help";
                    exit(0);
                    break;

                case 'm':
                    if(string(optarg) == "MST"){
                        partA = true;
                        break;
                    } else if(string(optarg) == "FASTTSP"){
                        partB = true;
                        break;
                    } else if (string(optarg) == "OPTTSP"){
                        partC = true;
                        break;
                    } else {
                        cerr << "Invalid mode entered \n";
                        exit(1);
                    }
                    
                    break;   

                default:
                    exit(1);

            }  
        }

        cout << setprecision(2);
        cout << fixed;


        
        cin >> amount;

        map.beenVisited.resize(amount);
        map.distance.resize(amount);
        map.parent.resize(amount);
        map.terrain.resize(amount);
        
        if(partC){
            locs.resize(amount);
            map.coordinates.resize(amount);
            coord temp;
            pair<int, int> temp1;
            for(uint32_t i = 0; i < locs.size(); i++){
                cin >> temp.x;
                temp1.first = temp.x;
                cin >> temp.y;
                temp1.second = temp.y;
                locs[i] = temp;
                map.coordinates[i] = temp1;
            }

            
        }else{
            for(uint32_t i = 0; i < amount; i++){

            pair <int,int> temp;
            cin >> temp.first;
            cin >> temp.second;
            
            map.coordinates.push_back(temp);
            map.beenVisited[i] = false;
            map.distance[i] = numeric_limits<double>::infinity();

            if(partA){
                uint32_t waterCount = 0;
                uint32_t landCount = 0;
                uint32_t coastCount = 0;

                if(isWater(temp)){
                    map.terrain[i] = 'w';
                    waterCount++;
                }
                else if(isCoast(temp)){
                    map.terrain[i] = 'c';
                    coastCount++;
                } else {
                    map.terrain[i] = 'l';
                    landCount++;
                }

                if(waterCount > 1 && landCount > 1 && coastCount == 0){
                    cerr << "Cannot construct MST";
                    exit(1);
                }
            }

            }
        }

        map.beenVisited[0] = true;
        map.distance[0] = 0;

        if(partA){
            MST();
        }
        else if(partB){
            FASTTSP();
        } 
        else if(partC){
            OPTTSP();
        }


}