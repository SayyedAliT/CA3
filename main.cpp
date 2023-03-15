#include <iostream>
#include <vector>
using namespace std;
#define FINISH_TIME 1
#define START_TIME 0
#define NO_CLASS 0
#define FIRST_CLASS 0
#define SECOND_CLASS 1
#define THIRD_CLASS 2
#define INITIAL_TIME_HOUR 7
#define INITIAL_TIME_MINUTE 30
#define FINAL_TIME_HOUR 13
#define FINAL_TIME_MINUTE 0
enum Weekdays{
    Saturday,
    Sunday,
    Monday,
    Tuesday,
    Wednesday
};
struct Time
{
    int hours;
    int minutes;
};
struct Teachers_information
{
    string teacher_name;
    int number_of_free_days;
    vector<string> free_days;
    int number_of_courses;
    vector<string> Courses;
};

struct Courses_information
{
    string day1;
    Time start_time;
    string course_name;
    string day2;
    Time finish_time;
};
// struct Weekly_schedule
// {
//     Teachers_information teacher;
// };
struct Class_information
{
    string teacher_name;
    string day1, day2;
    Time start_time, finish_time;
};
vector <string > days={"Saturday","Sunday","Monday","Tuesday","Wednesday"};
typedef vector<Teachers_information> TeacherBook;
typedef vector<Courses_information> CourseBook;
typedef vector<vector<int> > Classes_board;
typedef vector<vector<Time> > TimeTable;
typedef vector<Class_information> Weekly_schedule;
TimeTable time_table;
void initialize_time()
{
    vector<Time> classtime1,classtime2,classtime3;
    classtime1.push_back({7,30});
    classtime1.push_back({9,0});
    classtime2.push_back({9,30});
    classtime2.push_back({11,0});
    classtime3.push_back({11,30});
    classtime3.push_back({13,0});
    
    time_table.push_back(classtime1);
    time_table.push_back(classtime2);
    time_table.push_back(classtime3);


}
Teachers_information read_each_teacher_information(Teachers_information &new_teacher)
{
    cin >> new_teacher.teacher_name;
    cin >> new_teacher.number_of_free_days;
    for (int day = 0; day < new_teacher.number_of_free_days; day++)
    {
        string day_name;
        cin >> day_name;
        new_teacher.free_days.push_back(day_name);
    }
    cin >> new_teacher.number_of_courses;
    for (int course = 0; course < new_teacher.number_of_courses; course++)
    {
        string course_name;
        cin >> course_name;
        new_teacher.Courses.push_back(course_name);
    }
    return new_teacher;
}

Courses_information read_each_course_information(Courses_information &new_course)
{
    cin >> new_course.course_name;
    cin >> new_course.day1;
    cin >> new_course.day2;
    char delimiter;
    cin >> new_course.start_time.hours;
    cin >> delimiter;
    cin >> new_course.start_time.minutes;
    cin >> new_course.finish_time.hours;
    cin >> delimiter;
    cin >> new_course.finish_time.minutes;
    return new_course;
}

TeacherBook read_teachers_information(TeacherBook teachers_information)
{
    int number_of_teachers;
    cin >> number_of_teachers;
    for (int teacher = 0; teacher < number_of_teachers; teacher++)
    {
        // get input
        Teachers_information new_teacher;
        teachers_information.push_back(read_each_teacher_information(new_teacher));
    }
    return teachers_information;
}

void  read_courses_information(CourseBook &courses_information)
{
    int numberOfCourse;
    cin >> numberOfCourse;
    for (int course = 0; course < numberOfCourse; course++)
    {
        Courses_information new_course;
        courses_information.push_back(read_each_course_information(new_course));
    }
}


bool finish_is_after(Time finish_time, Time class_finish_time)
{
    return (finish_time.hours > class_finish_time.hours) || ((finish_time.hours == class_finish_time.hours) && (finish_time.minutes >= class_finish_time.minutes));
}

// bool after(Time t1, Time t2)
// {
//     return (t1.hours > t2.hours) || ((t1.hours == t2.hours) && (t1.minutes > t2.minutes));
// }

// bool same(Time t1, Time t2)
// {
//     return (t1.hours == t2.hours) && (t1.minutes == t2.minutes);
// }
bool start_is_before(Time course_start_time , Time class_start_time)
{
    return ((course_start_time.hours<class_start_time.hours)||(course_start_time.hours == class_start_time.hours  && course_start_time.minutes<=class_start_time.minutes));
}
bool classtime_is_valid(Courses_information course , int day , int class_time)
{
    if ((finish_is_after(course.finish_time,time_table[class_time][FINISH_TIME])&&(start_is_before(course.start_time , time_table[class_time][START_TIME])))&&((course.day1 == days[day])||(course.day2 == days[day])))
    {
        return true;
    }
    return false;
        
}
Time add_minutes(Time t, int m) {
	// precondition: the result will not be greater than 23:59
	Time result = t;
	result.minutes += m;
	result.hours += (result.minutes / 60);
	result.minutes %= 60;
	return result;
}
bool compare( Courses_information a, Courses_information b){
	/** to use another attribute for sorting, just replace 'age' with it eg. (a.roll_no < b.roll_no)
	 the return value determines which student will go first in the sorted array **/
	if(a.course_name[0] < b.course_name[0])
		return 1;
	else 
		return 0;
}
Weekly_schedule weekly_schedule(TeacherBook teachers_information,CourseBook Courses_information)
{
    Weekly_schedule schedule;
    Time current_time = {INITIAL_TIME_HOUR,INITIAL_TIME_MINUTE};
    int current_day= Saturday;
    for (current_day ; current_day <= Wednesday ; current_day++)
    {
        while((current_time.hours<FINAL_TIME_HOUR))
        {

            

        }

    }
    
}

bool check_days(Courses_information course , int day)
{
    return (course.day1 == days[day] || course.day2 == days[day]);
}

CourseBook lesson_can_be_presented(Time current_time , CourseBook courses_information, int day)
{
    CourseBook lesson_can_present;
    Time finish_time_of_class;
    finish_time_of_class.hours = current_time.hours+2;
    finish_time_of_class.minutes = (current_time.minutes + 30)%60;

    for (Courses_information course : courses_information)
    {

        if(finish_is_after(course.finish_time,finish_time_of_class) && start_is_before(course.start_time,current_time)&& (check_days(course,day)))
        {
            lesson_can_present.push_back(course);
        }
    }
    
    return lesson_can_present;
}
int main()
{
    // read input
    TeacherBook teachers_information;
    CourseBook courses_information ;
    read_teachers_information(teachers_information);
    read_courses_information(courses_information);
    // proccess program
    //  generate output
}
