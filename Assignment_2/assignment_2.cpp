#include <vector>
#include <iostream>
#include <string>
#include <cctype>    //For std::isspace
#include <fstream>   //Needed by read_sailings to work with files
#include <stdexcept> //Needed by read_sailings to handle exceptions
#include <iomanip>   //Needed by print_sailing to format output

/* A structure type to represent a year/month/day combination */
struct Date
{
    int day{ 0 };
    int month{ 0 };
    int year{ 0 };
};

/* A structure type to represent an hour:minute pair */
struct TimeOfDay
{
    int hour{ 0 };
    int minute{ 0 };
};

/* A structure type to store the parameters of a particular sailing */
struct Sailing
{
    int route_number{ 0 };
    std::string source_terminal{ "" };
    std::string dest_terminal{ "" };
    std::string vessel_name{ "" };

    Date departure_date{};
    TimeOfDay scheduled_departure_time{};

    int expected_duration{ 0 };
    int actual_duration{ 0 };
};

/* A structure type to store aggregated performance data for a single
   route. */
struct RouteStatistics
{
    int route_number{ 0 };
    int total_sailings{ 0 };
    int late_sailings{ 0 };
};

/* A structure type to store aggregated performance data for a single
   day. */
struct DayStatistics
{
    Date date{};
    int total_sailings{ 0 };
    int late_sailings{ 0 };
};


/* Helper Struct for including ratio in Day Statistc*/
struct ratio_DayStat
{
    Date date{};
    int total_sailings{ 0 };
    int late_sailings{ 0 };
    double ratio{ 0 };
};


/* Structure types to represent various issues that may occur during parsing */

/* Error case: Line contains too many or too few fields.
               The num_fields member will store the number of fields found. */
struct IncompleteLineException
{
    unsigned int num_fields{}; //
};

/* Error case: One (or more) fields in the line is empty (or contains only
               whitespace). The which_field member will store the index
               of the first empty field in the line. */
struct EmptyFieldException
{
    unsigned int which_field{}; //The first field is field 0, the last field is field 10
};

/* Error case: One or more numeric fields in the line contains non-numeric data.
               The bad_field member will store the text representation of the first
               field in the line with invalid non-numeric data. */
struct NonNumericDataException
{
    std::string bad_field{};
};

/* Error case: The departure hour/minute are valid numbers, but not a valid
               time (hour in the range 0 - 23, minute in the range 0 - 59).
               The bad_time member will store the invalid Time object. */
struct InvalidTimeException
{
    TimeOfDay bad_time{};
};

/* Function prototypes */
/* Put your implementations of these functions in a2_functions.cpp */


/*helpers for parse_sailing*/
bool check_space_errors(const std::string& el);

int convert_to_int_and_check(const std::string& str);

void time_check(const int hour, const int minute);

/*helpers for best_days & worst_days*/
bool cmp_date(const Date& date1, const Date& date2);

void update_ratio_struct(ratio_DayStat& stat, const Sailing& sailing);

/* Functions to implement */

/* parse_sailing(input_line)
   Given a string containing a line from the input file, convert the
   data in the line to a Sailing instance and return the complete
   Sailing object.

   If the line cannot be parsed successfully, the function will throw
   an exception instead of returning a complete Sailing object.
   The function may not throw ANY exceptions (directly or indirectly) besides
   the ones listed below.

   Your code must check for errors in the order they are listed below.
   For example, if the line has errors in both case 1 and case 2, the
   function will throw the exception for error case 1 (and never check
   case 2).

   When an exception is thrown for a particular error, your code must
   populate the exception object with the appropriate data (see the description
   of each object above).

   Error cases:
    1. If the line contains the wrong number of fields, throw an exception
       of type IncompleteLineException. There should be 11 fields, separated by
       commas (so there should be 10 commas in a line with the correct number of
       fields).
    2. If any of the fields in the line is empty (i.e. length zero) or contains
       only whitespace, throw an exception of type EmptyFieldException. You
       can check whether a single character is whitespace using std::isspace,
       and can use std::isspace in a loop to test whether an entire string is
       whitespace.
    3. If any of the numeric fields (route number, year, month, day, hour,
       minute, expected duration, actual duration) do not begin with
       numeric data (e.g. if one of those fields contains the text "abcd"
       "a123b" or "ab123"), throw an exception of type NonNumericDataException.
       Note that a field which begins with a number and contains non-numeric
       data afterward (e.g. "123abc") is not an error (and should be parsed
       as the number 123); this is for programming convenience (since that
       case is harder to detect).
    4. If the departure time fields (hour and minute) do not comprise a valid
       24-hour time, throw an exception of type InvalidTimeException.

   If none of the above errors occurred, store the parsed data into a Sailing
   object and return that.

   Advice: Create some extra functions to help with the various subtasks
           involved in this function, rather than putting all of the code
           in one place.
*/
Sailing parse_sailing(std::string const& input_line)
{
    /* Your Code Here */
    std::string temp{};
    std::vector<std::string> elements{};
    Sailing S{};

    for (size_t i = 0; i < input_line.size(); i++) {
        if (input_line.at(i) != ',')
            temp += input_line.at(i);
        else {
            elements.push_back(temp);
            /*std::cout << temp;*/
            temp.clear();

        }

    }
    elements.push_back(temp);
 
    if (elements.size() != 11) {
        const IncompleteLineException e{ elements.size()};

        throw e;
    }

    for (size_t i{ 0 }; i < elements.size(); i++) {
        if (check_space_errors(elements.at(i))) {
            const EmptyFieldException e{ i };
            throw e;
        }

    }
    for (size_t i{ 0 }; i < elements.size(); i++) {
        switch (i) {
        case 0:
            S.route_number = convert_to_int_and_check(elements.at(i)); 
            break;
        case 1:
            S.source_terminal = elements.at(i);
            break;
        case 2:
            S.dest_terminal = elements.at(i);
            break;
        case 3:
            S.departure_date.year = convert_to_int_and_check(elements.at(i));
            break;
        case 4:
            S.departure_date.month = convert_to_int_and_check(elements.at(i));
            break;
        case 5:
            S.departure_date.day = convert_to_int_and_check(elements.at(i));
            break;
        case 6:
            S.scheduled_departure_time.hour = convert_to_int_and_check(elements.at(i));
            break;
        case 7:
            S.scheduled_departure_time.minute = convert_to_int_and_check(elements.at(i));
            break;
        case 8:
            S.vessel_name = elements.at(i);
            break;
        case 9:
            S.expected_duration = convert_to_int_and_check(elements.at(i));
            break;
        case 10:
            S.actual_duration = convert_to_int_and_check(elements.at(i));
            break;
            
        }

    }

    time_check(S.scheduled_departure_time.hour, S.scheduled_departure_time.minute);

    return S;

}
bool check_space_errors(const std::string& el) {
    bool space{ false };
    for (size_t i{ 0 }; i < el.size(); i++) {
        if (!std::isspace(el.at(i))) {
            space = false;
            break;
        }
        else {
            space = true;
        }

    }
    return space;
}

int convert_to_int_and_check(const std::string& str) {
    int i{};
    try {
        i = stoi(str);
    }
    catch (std::invalid_argument &) {
        const NonNumericDataException e{ str };

        throw e;
    }
    return i;
}

void time_check(const int hour, const int minute) {
    if (!(hour <= 23 && hour >= 0 && minute <= 59 && minute >= 0)) {
        const InvalidTimeException e{ hour, minute };

        throw e;
    }

}

/* performance_by_route(sailings)
   Given a vector of Sailing instances (in no particular order), return
   a vector of RouteStatistics instances, with exactly one entry in the result
   vector for each unique route number in the input vector.

   To be clear, there may be many sailings in the input vector with a particular
   route number (e.g. 3), but each route number must appear in only one entry
   of the result vector.

   Each RouteStatistics object will also contain the total number of sailings
   and total number of late sailings on each route.

   Recall from the specification that a "late" sailing is any sailing
   that took at least five minutes longer than expected. That is, a sailing
   where
     actual_duration >= expected_duration + 5

   Return value:
     A vector of RouteStatistics objects, in any order, where each route number
     appearing in the input vector is represented by exactly one RouteStatistics
     instance in the result.

*/
std::vector<RouteStatistics> performance_by_route(std::vector<Sailing> const& sailings)
{
    /* Your Code Here */
    std::vector <RouteStatistics> RS;
    


    for (size_t i{ 0 }; i < sailings.size(); i++) {
        bool in{ false };
        for (size_t j{ 0 }; j < RS.size(); j++) {
            if (sailings.at(i).route_number == RS.at(j).route_number) {
                in = true;
                RS.at(j).total_sailings++;
                if (sailings.at(i).actual_duration - sailings.at(i).expected_duration >= 5) {
                    RS.at(j).late_sailings++;
                }
                break;
            }

        }

        if (!in) {
            RouteStatistics temp{};
            temp.route_number = sailings.at(i).route_number;
            temp.total_sailings++;
            if (sailings.at(i).actual_duration - sailings.at(i).expected_duration >= 5) {
                temp.late_sailings++;
            }
            RS.push_back(temp);
        }
    }
    return RS;

}

/* best_days(sailings)
   Recall that a sailing is considered "late" if
    actual_duration >= expected_duration + 5

   Generally, a day with a large number of late sailings is a "bad day" for
   travellers.

   The "best days" are any days in the dataset for which the ratio
     late_sailings/total_sailings
   is the lowest (in the best possible case, there will be zero late sailings,
   and the ratio will equal zero).

   Given a vector of Sailing objects, this function returns a vector of
   DayStatistics objects for all of the "best days" in the dataset, which
   are all days in the dataset that have the lowest ratio of late sailings
   to total sailings.
   Note that there may be multiple such days.

   Return value:
     A vector of DayStatistics objects, in any order, where each element of the vector
     corresponds to a different date (meeting the criteria of "best days" above).
     If the input vector of sailings is empty, the return value will be an empty vector.
*/
std::vector<DayStatistics> best_days(std::vector<Sailing> const& sailings)
{
    /* Your Code Here */

    std::vector <DayStatistics> best_days{};
    std::vector <ratio_DayStat> rds{};
    for (size_t i = 0; i < sailings.size(); i++)
    {
        bool in{ false };
        for (size_t j = 0; j < rds.size(); j++)
        {
            if (cmp_date(sailings.at(i).departure_date, rds.at(j).date)) {
                in = true;
                update_ratio_struct(rds.at(j), sailings.at(i));
                break;
            }

        }
        
        if (!in) {
            ratio_DayStat temp{ {sailings.at(i).departure_date.day,sailings.at(i).departure_date.month,sailings.at(i).departure_date.year},0,0,0 };
            rds.push_back(temp);
            update_ratio_struct(rds.back(), sailings.at(i));
        }
    }
    double sailing_ratio{ rds.at(0).ratio };
    for (ratio_DayStat stat : rds)
    {
        if (stat.ratio < sailing_ratio)
            sailing_ratio = stat.ratio;
    }

    for (ratio_DayStat stat : rds)
    {
        if (stat.ratio == sailing_ratio) {
            DayStatistics temp{};
            temp.date.day = stat.date.day;
            temp.date.month = stat.date.month;
            temp.date.year = stat.date.year;
            temp.total_sailings = stat.total_sailings;
            temp.late_sailings = stat.late_sailings;
            best_days.push_back(temp);
        }
    }

    return best_days;
}
bool cmp_date(const Date& date1, const Date& date2) {
    if (date1.day == date2.day && date1.month == date2.month && date1.year == date2.year)
        return true;
    else
        return false;
}
void update_ratio_struct(ratio_DayStat& stat, const Sailing& sailing) {
   
    stat.total_sailings++;
    if (sailing.actual_duration - sailing.expected_duration >= 5)
        stat.late_sailings++;
    stat.ratio = (double)stat.late_sailings /(double) stat.total_sailings;
}


/* worst_days(sailings)
   See the description of best_days().

   The "worst days" are any days in the dataset for which the ratio
     late_sailings/total_sailings
   is the highest (in the worst possible case, every sailing is late,
   and the ratio will equal one).

   Given a vector of Sailing objects, this function returns a vector of
   DayStatistics objects for all of the "worst days" in the dataset,which
   are all days in the dataset that have the highest ratio of late sailings
   to total sailings.
   Note that there may be multiple such days.

   Return value:
     A vector of DayStatistics objects, in any order, where each element of the vector
     corresponds to a different date (meeting the criteria of "worst days" above).
     If the input vector of sailings is empty, the return value will be an empty vector.
*/
std::vector<DayStatistics> worst_days(std::vector<Sailing> const& sailings)
{
    /* Your Code Here */
    std::vector <DayStatistics> worst_days;
    std::vector <ratio_DayStat> rds{};
    for (size_t i = 0; i < sailings.size(); i++)
    {
        bool in{ false };
        for (size_t j = 0; j < rds.size(); j++)
        {
            if (cmp_date(sailings.at(i).departure_date, rds.at(j).date)) {
                in = true;
                update_ratio_struct(rds.at(j), sailings.at(i));
                break;
            }

        }
        if (!in) {
            ratio_DayStat temp{ {sailings.at(i).departure_date.day,sailings.at(i).departure_date.month,sailings.at(i).departure_date.year},0,0,0 };
            rds.push_back(temp);
            update_ratio_struct(rds.back(), sailings.at(i));
        }
    }
    double  sailing_ratio{ rds.at(0).ratio };
    for (ratio_DayStat stat : rds)
    {
        if (stat.ratio > sailing_ratio)
            sailing_ratio = stat.ratio;
    }
    
    for (ratio_DayStat stat : rds)
    {
        if (stat.ratio == sailing_ratio) {
            DayStatistics temp{};
            temp.date.day = stat.date.day;
            temp.date.month = stat.date.month;
            temp.date.year = stat.date.year;
            temp.total_sailings = stat.total_sailings;
            temp.late_sailings = stat.late_sailings;
            worst_days.push_back(temp);
        }
    }

    return worst_days;
}

/* Provided functions (already implemented in a2_functions.cpp) */
/* You do not have to understand or modify these functions (although they
   are of the same level of difficulty as the other parts of the assignment) */
std::vector<Sailing> read_sailings(std::string const& input_filename)
{
    std::vector<Sailing> all_sailings;
    std::ifstream input_file;
    input_file.open(input_filename);

    int valid_sailings{ 0 };
    int total_lines{ 0 };

    if (input_file.is_open())
    {
        std::string line;
        while (std::getline(input_file, line))
        {
            total_lines++;
            try
            {
                Sailing s{ parse_sailing(line) };
                valid_sailings++;
                all_sailings.push_back(s);
            }
            catch (IncompleteLineException& e)
            {
                std::cout << "Line " << total_lines << " is invalid: ";
                std::cout << e.num_fields << " fields found." << std::endl;
            }
            catch (EmptyFieldException& e)
            {
                std::cout << "Line " << total_lines << " is invalid: ";
                std::cout << "Field " << e.which_field << " is empty." << std::endl;
            }
            catch (NonNumericDataException& e)
            {
                std::cout << "Line " << total_lines << " is invalid: ";
                std::cout << "\"" << e.bad_field << "\" is non-numeric." << std::endl;
            }
            catch (InvalidTimeException& e)
            {
                std::cout << "Line " << total_lines << " is invalid: ";
                std::cout << e.bad_time.hour << ":" << e.bad_time.minute << " is not a valid time." << std::endl;
            }
        }
        input_file.close();
    }
    else
    {
        throw std::runtime_error("Unable to open input file");
    }
    int invalid_sailings{ total_lines - valid_sailings };
    std::cout << "Read " << valid_sailings << " records." << std::endl;
    std::cout << "Skipped " << invalid_sailings << " invalid records." << std::endl;
    return all_sailings;
}

void print_sailing(Sailing const& sailing)
{
    std::cout << "Route " << sailing.route_number;
    std::cout << " (" << sailing.source_terminal << " -> " << sailing.dest_terminal << "): ";
    std::cout << sailing.departure_date.year << "-";
    std::cout << std::setfill('0') << std::setw(2) << sailing.departure_date.month << "-";
    std::cout << std::setfill('0') << std::setw(2) << sailing.departure_date.day << " ";
    std::cout << std::setfill('0') << std::setw(2) << sailing.scheduled_departure_time.hour << ":";
    std::cout << std::setfill('0') << std::setw(2) << sailing.scheduled_departure_time.minute << " ";
    std::cout << "[Vessel: " << sailing.vessel_name << "] ";
    std::cout << sailing.actual_duration << " minutes (" << sailing.expected_duration << " expected)" << std::endl;
}

int main(int argc, char** argv)
{
    /* if (argc < 3)
     {
         std::cout << "Usage: ./assignment_2 action input_filename" << std::endl;
         std::cout << "       where action is either 'route_summary' or 'days'" << std::endl;
         return 1;
     }*/

    std::string action{ "days" };
    std::string input_filename{ "./data/10_Sept2017_Aug2022_AllRoutes.txt" };

    auto all_sailings{ read_sailings(input_filename) };

    if (action == "route_summary")
    {
        std::cout << "Performance by route:" << std::endl;
        auto statistics{ performance_by_route(all_sailings) };
        for (auto stats : statistics)
        {
            std::cout << "Route " << stats.route_number << ": " << stats.total_sailings << " sailings (" << stats.late_sailings << " late)" << std::endl;
        }
    }
    else if (action == "days")
    {
        auto best{ best_days(all_sailings) };
        auto worst{ worst_days(all_sailings) };
        std::cout << "Best days:" << std::endl;
        for (auto stats : best)
        {
            std::cout << stats.date.year << "-" << stats.date.month << "-" << stats.date.day << ": ";
            std::cout << stats.total_sailings << " sailings (" << stats.late_sailings << " late)" << std::endl;
        }
        std::cout << "Worst days:" << std::endl;
        for (auto stats : worst)
        {
            std::cout << stats.date.year << "-" << stats.date.month << "-" << stats.date.day << ": ";
            std::cout << stats.total_sailings << " sailings (" << stats.late_sailings << " late)" << std::endl;
        }
    }
    else
    {
        std::cout << "Invalid action " << action << std::endl;
    }

    return 0;
}