#include <iostream>
#include <string>
#include <vector>
#include <memory>
using namespace std;
class Trip;

class Mission {
protected:
    int mission_id;
    int start_timestamp;
    int end_timestamp;
    int reward_amount;
public:
    Mission(int id, long start, long end, long reward);
    ~Mission();
    virtual bool is_completed() = 0;
    virtual void create_new_trip(shared_ptr<Trip> new_trip) = 0;
    virtual shared_ptr<Mission> clone()=0;
    bool is_trip_in_misson(shared_ptr<Trip> new_trip);

    int get_mission_id() { return mission_id; }

    int get_start_timestamp() { return start_timestamp; }

    int get_end_timestamp() { return end_timestamp; }

    int get_reward_amount() { return reward_amount; }

};


class Trip {
private:
    int start_timestamp;
    int end_timestamp;
    int driver_id;
    int distance_traveled;
public:
    Trip(int start, int end, int driver, int distance) : start_timestamp(start), end_timestamp(end), driver_id(driver), distance_traveled(distance) {}
    int get_start_timestamp() {
        return start_timestamp;
    }

    int get_end_timestamp() {
        return end_timestamp;
    }

    int get_driver_id() {
        return driver_id;
    }

    int get_distance_traveled() {
        return distance_traveled;
    }
};


class TimeMission : public Mission {
private:
    int target_time_in_minutes;
public:
    TimeMission(int id, int start, int end, int reward, int target_time) ;
    bool is_completed();
    void create_new_trip(shared_ptr<Trip> new_trip);
    shared_ptr<Mission> clone();
};

class CountMission : public Mission {
private:
    int target_number;
public:
    CountMission(int id, int start, int end, int reward, int target_count) : Mission(id, start, end, reward), target_number(target_count) {}

    bool is_completed();
    void create_new_trip(shared_ptr<Trip> new_trip);
    shared_ptr<Mission> clone();

};

class DistanceMission : public Mission {
private:
    int target_distance_in_meters;
public:
    DistanceMission(int id, int start, int end, int reward, int target_distance) : Mission(id, start, end, reward), target_distance_in_meters(target_distance) {}

    bool is_completed();

    shared_ptr<Mission> clone();
    void create_new_trip(shared_ptr<Trip> new_trip);

    int get_remaining_time() {
        return -1; // not applicable
    }

    int get_remaining_trips() {
        return -1; // not applicable
    }
};

class Driver {
private:
    int driver_id;
    vector<shared_ptr<Mission>> missions;
    bool is_duplicate_mission(int id);
public:
    Driver(int id) : driver_id(id) {}

    void assign_mission(shared_ptr<Mission> mission);

    vector<shared_ptr<Mission>> get_completed_missions();

    int get_driver_id(){return driver_id;}

    void update_missions(shared_ptr<Trip> new_trip);

    void show_driver_missions();

    vector<shared_ptr<Mission>> get_new_completed_missions(vector<shared_ptr<Mission>> base_mission);

    vector<shared_ptr<Mission>> get_mission(){return missions;}
};


class SystemHandler {
public:
    SystemHandler();

    void add_time_mission(int id, int start, int end, int target_time, int reward);

    void add_distance_mission(int id, int start, int end, int target_distance, int reward);

    void add_count_mission(int id, int start, int end, int target_count, int reward);

    void add_driver(int id);

    void assign_mission_to_driver(int mission_id, int driver_id);

    shared_ptr<Driver> find_driver_by_id(int driver_id);

    shared_ptr<Mission> find_mission_by_id(int mission_id);

    void record_ride(int start_time, int end_time, int driver_id, int distance);

    vector<shared_ptr<Mission>> get_completed_mission();

    void show_mission_status(int driver_id);

    void print_new_completed_mission(vector<shared_ptr<Mission>> completed_missions,int driver_id);
private:
    vector<shared_ptr<Mission>> all_missions;
    vector<shared_ptr<Driver>> drivers;
    vector<shared_ptr<Trip>> completed_trips;
    vector<shared_ptr<Mission>> completed_missions;


};




















Mission::Mission(int id_, long start_time_, long end_time_, long reward_) : mission_id(id_),
                                                                            start_timestamp(start_time_),
                                                                            end_timestamp(end_time_),
                                                                            reward_amount(reward_) {}



Mission::~Mission() {
}


TimeMission::TimeMission(int id, int start, int end, int reward, int target_time) : Mission(id, start, end, reward),
                                                                                    target_time_in_minutes(
                                                                                            target_time) {}

bool TimeMission::is_completed() {
    return target_time_in_minutes <= 0;
}

void TimeMission::create_new_trip(shared_ptr<Trip> new_trip) {
    if (this->is_trip_in_misson(new_trip)) {
        int duration = new_trip->get_end_timestamp() - new_trip->get_start_timestamp();
        target_time_in_minutes -= duration;
    }
}

shared_ptr<Mission> TimeMission::clone() {
    return make_shared<TimeMission>(*this);
}


bool DistanceMission::is_completed()
{
    return target_distance_in_meters <= 0;
}
void DistanceMission::create_new_trip(shared_ptr<Trip> new_trip)
{
    if (this->is_trip_in_misson(new_trip))
    {
        target_distance_in_meters-=new_trip->get_distance_traveled();
    }
}
shared_ptr<Mission> DistanceMission::clone() {
    return make_shared<DistanceMission>(*this);
}




bool CountMission::is_completed()
{
    return target_number <= 0;
}
void CountMission::create_new_trip(shared_ptr<Trip> new_trip)
{
    if (this->is_trip_in_misson(new_trip))
    {
        target_number-=1;
    }
}

shared_ptr<Mission> CountMission::clone() {
    return make_shared<CountMission>(*this);
}





bool Driver::is_duplicate_mission(int id) {
    for (shared_ptr<Mission> mission: missions) {
        if (mission->get_mission_id() == id) {
            return true;
        }
    }
    return false;

}

vector<shared_ptr<Mission>> Driver::get_completed_missions() {
    vector<shared_ptr<Mission>> completed_missions;
    for (shared_ptr<Mission> m: missions) {
        if (m->is_completed()) {
            completed_missions.push_back(m);
        }
    }
    return completed_missions;
}

void Driver::assign_mission(shared_ptr<Mission> mission) {
    if (is_duplicate_mission(mission->get_mission_id())) {
        throw "DUPLICATE_DRIVER_MISSION";
    }
    missions.push_back(mission->clone());
}

void Driver::update_missions(shared_ptr<Trip> trip) {
    for (shared_ptr<Mission> driver_mission: missions) {
        if ((driver_mission->is_trip_in_misson(trip)) && !driver_mission->is_completed()) {
            driver_mission->create_new_trip(trip);
        }
    }
}

void print_each_mission_info(shared_ptr<Mission> my_mission)
{
    cout << "mission "<< my_mission->get_mission_id()<<":"<<endl;
    cout <<"start timestamp: " <<my_mission->get_start_timestamp()<<endl;
    cout <<"end timestamp: " <<my_mission->get_end_timestamp()<<endl;
    cout <<"reward: " <<my_mission->get_reward_amount()<<endl;
    if(my_mission->is_completed())
        cout <<"status: completed" <<endl;
    else
        cout <<"ongoing"<<endl;
    cout <<endl;


}
void Driver::show_driver_missions() {
    for (shared_ptr<Mission> mission : missions)
    {
        if(mission->is_completed())
        {
            print_each_mission_info(mission);
        }
    }
}

vector<shared_ptr<Mission>> Driver::get_new_completed_missions(vector<shared_ptr<Mission>> base_mission)
{
    vector<shared_ptr<Mission>> new_completed_mission;
    for (int index = 0 ; index <missions.size() ;index)
    {
        if(missions[index]->is_completed() && !base_mission[index]->is_completed())
            new_completed_mission.push_back(missions[index]);

    }
    return new_completed_mission;
}



SystemHandler::SystemHandler() {}

void SystemHandler::add_time_mission(int id, int start, int end, int target_time, int reward) {
    for (shared_ptr<Mission> m: all_missions) {
        if (m->get_mission_id() == id) {
            throw "DUPLICATE_MISSION_ID";
        }
    }
    shared_ptr<Mission> mission = make_shared<TimeMission>(id, start, end, reward, target_time);
    all_missions.push_back(mission);
}

void SystemHandler::add_distance_mission(int id, int start, int end, int target_distance, int reward) {
    for (shared_ptr<Mission> m: all_missions) {
        if (m->get_mission_id() == id) {
            throw "DUPLICATE_MISSION_ID";
        }
    }
    shared_ptr<Mission> mission = make_shared<DistanceMission>(id, start, end, reward, target_distance);
    all_missions.push_back(mission);
}

void SystemHandler::add_count_mission(int id, int start, int end, int target_count, int reward) {
    for (shared_ptr<Mission> m: all_missions) {
        if (m->get_mission_id() == id) {
            throw "DUPLICATE_MISSION_ID";
        }
    }
    shared_ptr<Mission> mission = make_shared<CountMission>(id, start, end, reward, target_count);
    all_missions.push_back(mission);
}

void SystemHandler::add_driver(int id) {
    for (shared_ptr<Driver> d: drivers) { // this part is for finding
        if (d->get_driver_id() == id) {
            throw "DUPLICATE_DRIVER";
        }
    }
    shared_ptr<Driver> driver = make_shared<Driver>(id);
    drivers.push_back(driver);
}

void SystemHandler::assign_mission_to_driver(int mission_id, int driver_id) { /// need a void that find the misson by id
    shared_ptr<Mission> mission_to_assign = nullptr;
    mission_to_assign = find_mission_by_id(mission_id);
    if (mission_to_assign == nullptr) {
        throw "MISSION_NOT_FOUND";
    }

    shared_ptr<Driver> driver = nullptr;   // find driver by id
    for (shared_ptr<Driver> d: drivers) {
        if (d->get_driver_id() == driver_id) {
            driver = d;
            break;
        }
    }
    if (driver == nullptr) {
        throw "DRIVER_NOT_FOUND";
    }

    driver->assign_mission(mission_to_assign);
}

shared_ptr<Driver> SystemHandler::find_driver_by_id(int driver_id) {
    for (shared_ptr<Driver> driver: drivers) {
        if (driver->get_driver_id() == driver_id) {
            return driver;
        }
    }

    return nullptr;
}

void SystemHandler::print_new_completed_mission(vector<shared_ptr<Mission>> completed_missions,int driver_id)
{
    cout <<"completed missions for driver "<<driver_id<<":"<<endl;
    for(shared_ptr<Mission> new_mission : completed_missions)
    {
        cout <<"mission: " <<new_mission->get_mission_id()<<endl;
        cout <<"start timestamp: " <<new_mission->get_start_timestamp()<<endl;
        cout <<"end timestamp: " <<new_mission->get_end_timestamp()<<endl;
        cout <<"reward: "<<new_mission->get_reward_amount()<<endl;
        cout <<endl;

    }
}
void SystemHandler::record_ride(int start_time, int end_time, int driver_id, int distance) {
    shared_ptr<Trip> trip = make_shared<Trip>(start_time, end_time, driver_id, distance);
    completed_trips.push_back(trip);
    shared_ptr<Driver> target_driver = find_driver_by_id(driver_id);
    if (target_driver == nullptr) {
        add_driver(driver_id);
        target_driver = drivers[drivers.size() - 1];
    }
    vector<shared_ptr<Mission>> base_mission = target_driver->get_mission();
    target_driver->update_missions(trip);
    vector<shared_ptr<Mission>> new_completed = target_driver->get_new_completed_missions(base_mission);

    print_new_completed_mission(new_completed ,driver_id);

}
vector<shared_ptr<Mission>> SystemHandler::get_completed_mission()
{
    for(shared_ptr<Mission> mission : all_missions)
    {
        if(mission->is_completed())
        {
            completed_missions.push_back(mission);
        }
    }
    return completed_missions;
}

void SystemHandler::show_mission_status(int driver_id)
{
    shared_ptr<Driver>target_driver = find_driver_by_id(driver_id);
    target_driver->show_driver_missions();
}








const char SEPARATOR = ' ';
using namespace std;

int read_input(string command,SystemHandler &system_manager) {
    if (command == "add_time_mission")
    {
        int mission_id,start_timestamp,end_timestamp,target_time_in_minute,reward_amount;
        cin>>mission_id>>start_timestamp>>end_timestamp>>target_time_in_minute>>reward_amount;
        system_manager.add_time_mission(mission_id,start_timestamp,end_timestamp,target_time_in_minute,reward_amount);
    }
    else if (command == "add_count_mission")
    {
        int mission_id,start_timestamp,end_timestamp,target_number,reward_amount;
        cin>>mission_id>>start_timestamp>>end_timestamp>>target_number>>reward_amount;
        system_manager.add_count_mission(mission_id,start_timestamp,end_timestamp,target_number,reward_amount);
    }
    else if (command == "add_distance_mission")
    {
        int mission_id,start_timestamp,end_timestamp,target_distance_in_meters,reward_amount;
        cin>>mission_id>>start_timestamp>>end_timestamp>>target_distance_in_meters>>reward_amount;
        system_manager.add_distance_mission(mission_id,start_timestamp,end_timestamp,target_distance_in_meters,reward_amount);

    }
    else if (command == "assign_mission")
    {
        int mission_id,driver_id;
        cin>>mission_id>>driver_id;
        system_manager.assign_mission_to_driver(mission_id,driver_id);
    }
    else if (command == "record_ride")
    {
        int start_timestamp,end_timestamp,driver_id,distance;
        cin>>start_timestamp>>end_timestamp>>driver_id>>distance;
        system_manager.record_ride(start_timestamp,end_timestamp,driver_id,distance);
    }
    else if (command == "show_mission_status")
    {
        int driver_id;cin>>driver_id;
        system_manager.show_mission_status(driver_id);
    }
    else
        throw "INVALID_ARGUMENTS";
}


int main() {
    string command;
    SystemHandler system_manager;
    while (getline(cin, command, SEPARATOR)) {
        read_input(command,system_manager);
    }
}

