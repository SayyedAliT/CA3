#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <cstdio>
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
#define CLASS_LENGTH 90
#define CLASS_TWO 2
#define CLASS_ONE 1

vector<vector<int>> class1_board = {
    {0, 0, 0},
    {0, 0, 0},
    {0, 0, 0},
    {0, 0, 0},
    {0, 0, 0}};

vector<vector<int>> class2_board = {
    {0, 0, 0},
    {0, 0, 0},
    {0, 0, 0},
    {0, 0, 0},
    {0, 0, 0}};

enum Weekdays
{
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
struct Teacher_class_detail
{
    string day;
    bool class_one;
    bool class_two;
    bool class_three;
};

struct Teachers_information
{
    string teacher_name;
    int number_of_free_days;
    vector<string> free_days;
    vector<Teacher_class_detail> teacher_board;
    int number_of_courses;
    vector<string> Courses;
};

struct Class_information
{
    string teacher_name;
    Time start_time, finish_time;
};
struct Courses_information
{
    string course_name;
    string day1;
    string day2;
    Time start_time;
    Time finish_time;
    bool was_teached_for_class_one;
    bool was_teached_for_class_two;
    vector<Class_information> result;
    
};
// struct Weekly_schedule
// {
//     Teachers_information teacher;
// };


vector<string> days = {"Saturday", "Sunday", "Monday", "Tuesday", "Wednesday"};
typedef vector<Teachers_information> TeacherBook;
typedef vector<Courses_information> CourseBook;
typedef vector<vector<int>> Classes_board;
typedef vector<vector<Time>> TimeTable;
typedef vector<Class_information> Weekly_schedule;

//// Readind voids

Teachers_information read_each_teacher_information(Teachers_information &new_teacher)
{
    cin >> new_teacher.teacher_name >> new_teacher.number_of_free_days;
    for (int day = 0; day < new_teacher.number_of_free_days; day++)
    {
        string day_name;
        cin >> day_name;
        Teacher_class_detail new_inf;
        
        new_inf.day=day_name;
        new_inf.class_one=false;
        new_inf.class_two=false;
        new_inf.class_two=false;
        new_teacher.teacher_board.push_back(new_inf);
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
    new_course.was_teached_for_class_one =false;
    new_course.was_teached_for_class_two =false;
    Class_information new_result1,new_result2;

    new_result1.teacher_name="undefined";
    new_result2.teacher_name="undefined";
    new_course.result.push_back(new_result1);
    new_course.result.push_back(new_result2);

    return new_course;
}

TeacherBook read_teachers_information(TeacherBook &teachers_information)
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

void read_courses_information(CourseBook &courses_information)
{
    int numberOfCourse;
    cin >> numberOfCourse;
    for (int course = 0; course < numberOfCourse; course++)
    {
        Courses_information new_course;
        courses_information.push_back(read_each_course_information(new_course));
    }
}

//// proccesing voids







bool finish_is_after(Time finish_time, Time class_finish_time)
{
    return (finish_time.hours > class_finish_time.hours) || ((finish_time.hours == class_finish_time.hours) && (finish_time.minutes >= class_finish_time.minutes));
}

bool start_is_before(Time course_start_time, Time class_start_time)
{
    return ((course_start_time.hours < class_start_time.hours) || (course_start_time.hours == class_start_time.hours && course_start_time.minutes <= class_start_time.minutes));
}

Time add_minutes(Time t, int m)
{
    // precondition: the result will not be greater than 23:59
    Time result = t;
    result.minutes += m;
    result.hours += (result.minutes / 60);
    result.minutes %= 60;
    return result;
}









bool compare_teachers(Teachers_information a, Teachers_information b) /*debugged*/
{
    if ((a.number_of_free_days < b.number_of_free_days) || (a.number_of_free_days == b.number_of_free_days && a.teacher_name[0] < b.teacher_name[0]))
        return 1;
    else
        return 0;
}

bool compare_course(Courses_information a, Courses_information b) /*debugged*/
{
    /** to use another attribute for sorting, just replace 'age' with it eg. (a.roll_no < b.roll_no)
     the return value determines which student will go first in the sorted array **/
    if (a.course_name[0] < b.course_name[0])
        return 1;
    else
        return 0;
}





bool check_teacher_days(Teachers_information teacher, string day1, string day2)
{
    for (auto first_day : teacher.free_days)
    {
        if (first_day == day1)
        {
            for (auto second_day : teacher.free_days)
            {
                if (second_day == day2)
                {
                    return true;
                }
            }
        }
    }
    return false;
}

// bool check_teacher_time(Time finish_time_of_class , Time start_time_of_class, int day1,int day2,Teachers_information teacher)
// {

//     return finish_is_after(teacher,finish_time_of_class) && start_is_before(course.start_time,current_time)&& (check_teacher_days(teacher,day1,day2));
// }






bool already_teached(Teachers_information teacher, CourseBook courses_information,Courses_information lessons_can_be_presented, int current_day, int class_number_index)
{
    for ( int day = 0 ; teacher.number_of_free_days <5;day++)
        if(teacher.teacher_board[day].day==days[current_day])
        {
            switch (class_number_index)
           {
                case 0:
                    if(teacher.teacher_board[day].class_one==true)
                    {
                        return true;
                    }
                    break;
                case 1:
                    if(teacher.teacher_board[day].class_two==true)
                    {
                        return true;
                    }
                    break;
                case 2:
                    if(teacher.teacher_board[day].class_three==true)
                    {
                        return true;
                    }
                    break;
            }
        }
    return false;    
}

// bool already_teached_for_this_class(Teachers_information teacher, Weekly_schedule weekly_schedule_class1, Courses_information lesson_can_be_present)
// {
//     for ( Class_information programm: weekly_schedule_class1)
//     {
//         if(programm.course_name == lesson_can_be_present.course_name && programm.teacher_name == teacher.teacher_name )
//         {
//             return true;
//         }
        
//     }
//     return false;
// }
int find_class_index(Time current_time)
{
    int current_index ;
    switch(current_time.hours)
    {
        case 7:
            current_index=0;
            break;
        case 9:
            current_index=1;
            break;
        case 11:
            current_index=2;
            break;
    }
    return current_index;

}
void update_teacher(TeacherBook &teacher_information , string teacher_name,int current_day , Time current_time)
{
    int current_index = find_class_index(current_time);
    for(int teacher = 0 ;teacher<teacher_information.size();teacher++)
    {
        if(teacher_name==teacher_information[teacher].teacher_name)
        {
            for (int day =0 ; day<teacher_information[teacher].number_of_free_days;day++)
            {
                if (teacher_information[teacher].teacher_board[day].day==days[current_day])
                {
                    switch (current_index)
                    {
                        case 0:
                            teacher_information[teacher].teacher_board[day].class_one = true;
                            break;
                        case 1:
                            teacher_information[teacher].teacher_board[day].class_two = true;
                            break;
                        case 2:
                            teacher_information[teacher].teacher_board[day].class_three = true;
                            break;
                    }
                }
            }
        }
    }

}
void create_schedule(CourseBook &courses_information,Teachers_information teacher_can_present,Courses_information lesson_can_be_presented,int current_day,Time current_time,int class_number,TeacherBook &teachers_information)
{
    for(int course = 0 ;course <courses_information.size();course++)
    {
        if(courses_information[course].course_name==lesson_can_be_presented.course_name)
        {
            if(class_number == CLASS_ONE)
            {
                courses_information[course].was_teached_for_class_one = true;
                courses_information[course].result[0].teacher_name = teacher_can_present.teacher_name;
                courses_information[course].result[0].start_time = current_time;
                courses_information[course].result[0].finish_time = add_minutes(current_time,CLASS_LENGTH);
                update_teacher(teachers_information,teacher_can_present.teacher_name,current_day,current_time);
            }
            if(class_number==CLASS_TWO)
            {
                courses_information[course].was_teached_for_class_two = true;
                courses_information[course].result[1].teacher_name = teacher_can_present.teacher_name;
                courses_information[course].result[1].start_time = current_time;
                courses_information[course].result[1].finish_time = add_minutes(current_time,CLASS_LENGTH);
                update_teacher(teachers_information,teacher_can_present.teacher_name,current_day,current_time);
            }

            
        }
    }
}







void find_teacher_for_course(Courses_information lessons_can_be_presented, TeacherBook &teacher_information,CourseBook &courses_information, int day, Time start_time, Time finish_time ,int class_number_index, int class_number)
{
    TeacherBook teacher_can_present;
    for (int teacher=0;teacher<teacher_information.size() ; teacher++)
    {
        if (class_number == CLASS_TWO)
        {
            if (already_teached(teacher_information[teacher],courses_information ,lessons_can_be_presented, day,class_number_index))
            {
                continue;
            }
        }
        if (check_teacher_days(teacher_information[teacher], lessons_can_be_presented.day1, lessons_can_be_presented.day2))
        {
            teacher_can_present.push_back(teacher_information[teacher]);
        }
    }
    // from here to end must be in other function
    if (teacher_can_present.size() == 1)
    {
        create_schedule(courses_information,teacher_can_present[0],lessons_can_be_presented,day,start_time,class_number,teacher_information);
        cout << "For lesson: "<<lessons_can_be_presented.course_name << " in day: "<<days[day]<<"\n"<<teacher_can_present[0].teacher_name<<endl;
        // result = create_schedule(result, lessons_can_be_presented, teacher_can_present, start_time, finish_time);
    }
    else if (teacher_can_present.size() > 1)
    {
        sort(teacher_can_present.begin(), teacher_can_present.end(), compare_teachers);
        create_schedule(courses_information,teacher_can_present[0],lessons_can_be_presented,day,start_time,class_number,teacher_information);
        cout << "For lesson: "<<lessons_can_be_presented.course_name << " in day: "<<days[day]<<"\n"<<teacher_can_present[0].teacher_name<<endl;
    }
}













int convert_daytoi(string day)
{
    if (day == "Saturday")
        return 0;
    if (day == "Sunday")
        return 1;
    if (day == "Monday")
        return 2;
    if (day == "Tuesday")
        return 3;
    if (day == "Wednesday")
        return 4;
}










bool check_days(Courses_information course, int day)
{
    return (course.day1 == days[day] || course.day2 == days[day]);
}








CourseBook find_lesson_can_be_presented(Time current_time, CourseBook &courses_information, int day, int course_ring_number,int class_number)
{
    CourseBook lesson_can_present;
    Time finish_time_of_class = add_minutes(current_time, CLASS_LENGTH);

    for (int course=0; course<courses_information.size() ; course++)
    {
        if((courses_information[course].was_teached_for_class_one==true&&class_number==CLASS_ONE))
        {
            continue;
        }
        if((courses_information[course].was_teached_for_class_two==true&&class_number==CLASS_TWO))
        {
            continue;
        }
        int day1 = convert_daytoi(courses_information[course].day1), day2 = convert_daytoi(courses_information[course].day2);
        if(class_number==CLASS_ONE)

        {
            if ((class1_board[day1][course_ring_number] == 1 || class1_board[day2][course_ring_number] == 1))
            {
                continue;
            }

        }
        if(class_number==CLASS_TWO)
        {
            if(class2_board[day1][course_ring_number] == 1 || class2_board[day2][course_ring_number] == 1)
            {
                continue;
            }
        }
        if (finish_is_after(courses_information[course].finish_time, finish_time_of_class) && start_is_before(courses_information[course].start_time, current_time) && (check_days(courses_information[course], day)))
        {
            lesson_can_present.push_back(courses_information[course]);
        }
    }

    return lesson_can_present;
}


void create_weekly_schedule_class1(TeacherBook &teachers_information, CourseBook &courses_information)
{

    int class_number = CLASS_ONE;
    Time current_time = {INITIAL_TIME_HOUR, INITIAL_TIME_MINUTE};
    Time finish_time_of_class = add_minutes(current_time, CLASS_LENGTH);
    int class_board_index_row = 0;
    int current_day = Saturday;
    for (current_day; current_day <= Wednesday; current_day++)
    {
        while ((current_time.hours < FINAL_TIME_HOUR))
        {
            if (class1_board[current_day][class_board_index_row] == 1)
            {
                current_time = add_minutes(current_time, 120);
                class_board_index_row += 1;
                continue;
            }
            CourseBook lessons_can_be_presented = find_lesson_can_be_presented(current_time,courses_information,current_day,class_board_index_row,class_number);
            if (lessons_can_be_presented.size() == 1)
            {
                find_teacher_for_course(lessons_can_be_presented[0],teachers_information,courses_information,current_day,current_time,finish_time_of_class,class_board_index_row,class_number);
                int day1 = convert_daytoi(lessons_can_be_presented[0].day1), day2 = convert_daytoi(lessons_can_be_presented[0].day2);
                class1_board[day1][class_board_index_row] = 1;
                class1_board[day2][class_board_index_row] = 1;
            }
            else if (lessons_can_be_presented.size() > 1)
            {
                sort(lessons_can_be_presented.begin(), lessons_can_be_presented.end(), compare_course);

                find_teacher_for_course(lessons_can_be_presented[0],teachers_information,courses_information,current_day,current_time,finish_time_of_class,class_board_index_row,class_number);
                int day1 = convert_daytoi(lessons_can_be_presented[0].day1), day2 = convert_daytoi(lessons_can_be_presented[0].day2);
                class1_board[day1][class_board_index_row] = 1;
                class1_board[day2][class_board_index_row] = 1;
            }
            // else if (lessons_can_be_presented.size() == 0)
            // {
            //     class1_schedule.push_back(create_empty_schedule(current_time, class_board_index_row, current_day));
            //     class1_board[current_day][class_board_index_row] = -1;
            // }
            current_time = add_minutes(current_time, 120);
            class_board_index_row += 1;
        }
        current_time = {INITIAL_TIME_HOUR, INITIAL_TIME_MINUTE};
        class_board_index_row = 0;
    }
    
}










void create_weekly_schedule_class2(TeacherBook &teachers_information, CourseBook &courses_information)
{
    
    int class_number = CLASS_TWO;
    Time current_time = {INITIAL_TIME_HOUR, INITIAL_TIME_MINUTE};
    Time finish_time_of_class = add_minutes(current_time, CLASS_LENGTH);
    int class_board_index_row = 0;
    int current_day = Saturday;
    for (current_day; current_day <= Wednesday; current_day++)
    {
        while ((current_time.hours < FINAL_TIME_HOUR))
        {
            if (class2_board[current_day][class_board_index_row] == 1)
            {
                current_time = add_minutes(current_time, 120);
                class_board_index_row += 1;
                continue;
            }
            CourseBook lessons_can_be_presented = find_lesson_can_be_presented(current_time,courses_information,current_day,class_board_index_row,class_number);
            if (lessons_can_be_presented.size() == 1)
            {
                find_teacher_for_course(lessons_can_be_presented[0],teachers_information,courses_information,current_day,current_time,finish_time_of_class,class_board_index_row,class_number);
                int day1 = convert_daytoi(lessons_can_be_presented[0].day1), day2 = convert_daytoi(lessons_can_be_presented[0].day2);
                class2_board[day1][class_board_index_row] = 1;
                class2_board[day2][class_board_index_row] = 1;
            }
            else if (lessons_can_be_presented.size() > 1)
            {
                sort(lessons_can_be_presented.begin(), lessons_can_be_presented.end(), compare_course);

                find_teacher_for_course(lessons_can_be_presented[0],teachers_information,courses_information,current_day,current_time,finish_time_of_class,class_board_index_row,class_number);
                int day1 = convert_daytoi(lessons_can_be_presented[0].day1), day2 = convert_daytoi(lessons_can_be_presented[0].day2);
                class2_board[day1][class_board_index_row] = 1;
                class2_board[day2][class_board_index_row] = 1;
            }
            // else if (lessons_can_be_presented.size() == 0)
            // {
            //     class1_schedule.push_back(create_empty_schedule(current_time, class_board_index_row, current_day));
            //     class1_board[current_day][class_board_index_row] = -1;
            // }
            current_time = add_minutes(current_time, 120);
            class_board_index_row += 1;
        }
        current_time = {INITIAL_TIME_HOUR, INITIAL_TIME_MINUTE};
        class_board_index_row = 0;
    }
}













//// generating output and printing voids

void generate_output(CourseBook &courses_information)
{
    for(int i = 0 ;i<courses_information.size();i++)
    {
        if(courses_information[i].result[0].teacher_name=="undefined")
        {
            courses_information[i].result[0].teacher_name="Not Found";
        }
        if(courses_information[i].result[1].teacher_name=="undefined")
        {
            courses_information[i].result[1].teacher_name="Not Found";
        }
    }
}




void print_class_schedule(CourseBook final_program)
{
    sort(final_program.begin(),final_program.end(),compare_course);
    for(Courses_information programm: final_program)
    {
        cout << programm.course_name <<endl;
        cout << programm.result[0].teacher_name;
        printf(": %02d:%02d %02d:%02d\n",programm.result[0].start_time.hours,programm.result[0].start_time.minutes,
                                         programm.result[0].finish_time.hours,programm.result[0].finish_time.minutes);
        cout << programm.result[1].teacher_name;
        printf(": %02d:%02d %02d:%02d\n",programm.result[1].start_time.hours,programm.result[1].start_time.minutes,
                                         programm.result[1].finish_time.hours,programm.result[1].finish_time.minutes);
    }
}







int main()
{
    // read input
    TeacherBook teachers_information;
    CourseBook courses_information;
    Weekly_schedule weekly_schedule_class1, weekly_schedule_class2;
    
    
    read_teachers_information(teachers_information);
    read_courses_information(courses_information);
    
    
    sort(courses_information.begin(),courses_information.end(),compare_course);
    sort(teachers_information.begin(),teachers_information.end(),compare_teachers);

    create_weekly_schedule_class1(teachers_information, courses_information);
    create_weekly_schedule_class2(teachers_information, courses_information);
    
    
    
    generate_output(courses_information);
    print_class_schedule(courses_information);
    // weekly_schedule_class2 = create_weekly_schedule_class2(teachers_information, courses_information,weekly_schedule_class1,weekly_schedule_class2);
    // print_class_schedule(weekly_schedule_class1, weekly_schedule_class2);
    // proccess program
    //  generate output
}

// find the time that can be present
// vector <vector < int > > grid(5,vector < int >grid2(3,0))
// first error: 1- if amin teaches Sat and Mon and wants to teach science on Sun and Mon --> occurs problem
