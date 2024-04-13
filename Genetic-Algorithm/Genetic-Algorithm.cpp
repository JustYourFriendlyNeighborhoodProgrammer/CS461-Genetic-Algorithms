#include <iostream>
#include <vector>
#include <map>
#include <random>
#include <algorithm>
#include <cmath>
#include <string>
#include <iterator>

using namespace std;

struct Facilitator 
{
     string name;
     vector<string> preferred_activities;
     vector<string> other_activities;
     int total_activities = 0;
};

struct Room
{
     string name;
     int capacity;
};

struct Activity
{
     string name;
     int expected_enrollment;
     vector<string> preferred_activities;
     vector<string> other_activities;
};

struct TimeSlot
{
     string time;
};

struct Assignment
{
     string activity_name;
     string room_name;
     string facilitator_name;
     string time;
};

double calculate_fitness(const vector<Assignment>& schedule);
int find_time_slot_index(const vector<TimeSlot>& time_slots, const string& time);

vector<Facilitator> facilitators = {
    {"Lock", {"SLA100A", "SLA100B", "SLA191A", "SLA191B", "SLA291"}, {"SLA201", "SLA303", "SLA304", "SLA394", "SLA449", "SLA451"}},
    {"Glen", {"SLA100A", "SLA100B", "SLA191A", "SLA191B", "SLA201", "SLA303"}, {"SLA291", "SLA304", "SLA394", "SLA449", "SLA451"}},
    {"Banks", {"SLA100A", "SLA100B", "SLA191A", "SLA191B", "SLA201", "SLA291", "SLA303"}, {"SLA304", "SLA394", "SLA449", "SLA451"}},
    {"Richards", {"SLA100A", "SLA100B", "SLA191A", "SLA191B"}, {"SLA201", "SLA291", "SLA303", "SLA304", "SLA394", "SLA449", "SLA451"}},
    {"Shaw", {"SLA201", "SLA291", "SLA303"}, {"SLA100A", "SLA100B", "SLA191A", "SLA191B", "SLA304", "SLA394", "SLA449", "SLA451"}},
    {"Singer", {"SLA201", "SLA291", "SLA303", "SLA304", "SLA394", "SLA449", "SLA451"}, {"SLA100A", "SLA100B", "SLA191A", "SLA191B"}},
    {"Uther", {"SLA304", "SLA394", "SLA449", "SLA451"}, {"SLA100A", "SLA100B", "SLA191A", "SLA191B", "SLA201", "SLA291", "SLA303"}},
    {"Tyler", {"SLA304", "SLA394", "SLA449", "SLA451"}, {"SLA100A", "SLA100B", "SLA191A", "SLA191B", "SLA201", "SLA291", "SLA303"}},
    {"Numen", {"SLA100A", "SLA100B", "SLA191A", "SLA191B", "SLA201", "SLA291", "SLA303", "SLA304", "SLA394", "SLA449", "SLA451"}, {}},
    {"Zeldin", {"SLA100A", "SLA100B", "SLA191A", "SLA191B", "SLA201", "SLA291", "SLA303", "SLA304", "SLA394", "SLA449", "SLA451"}, {}}
};

vector<Room> rooms = {
     {"Slater 003", 45},
     {"Roman 216", 30},
     {"Loft 206", 75},
     {"Roman 201", 50},
     {"Loft 310", 108},
     {"Beach 201", 60},
     {"Beach 301", 75},
     {"Logos 325", 450},
     {"Frank 119", 60}

};

vector<TimeSlot> time_slots = {
     {"10 AM"},
     {"11 AM"},
     {"12 PM"},
     {"1 PM"},
     {"2 PM"},
     {"3 PM"}
};
 
vector<Activity> activities = {
    // goes as room, enrollment, preferred, other
    {"SLA100A", 50, {"Glen", "Lock", "Banks", "Zeldin"}, {"Numen", "Richards"}},
    {"SLA100B", 50, {"Glen", "Lock", "Banks", "Zeldin"}, {"Numen", "Richards"}},
    {"SLA191A", 50, {"Glen", "Lock", "Banks", "Zeldin"}, {"Numen", "Richards"}},
    {"SLA191B", 50, {"Glen", "Lock", "Banks", "Zeldin"}, {"Numen", "Richards"}},
    {"SLA201", 50, {"Glen", "Banks", "Zeldin", "Shaw"}, {"Numen", "Richards", "Singer"}},
    {"SLA291", 50, {"Lock", "Banks", "Zeldin", "Singer"}, {"Numen", "Richards", "Shaw", "Tyler"}},
    {"SLA303", 60, {"Glen", "Zeldin", "Banks"}, {"Numen", "Singer", "Shaw"}},
    {"SLA304", 25, {"Glen", "Banks", "Tyler"}, {"Numen", "Singer", "Shaw", "Richards", "Uther", "Zeldin"}},
    {"SLA394", 20, {"Tyler", "Singer"}, {"Richards", "Zeldin"}},
    {"SLA449", 60, {"Tyler", "Singer", "Shaw"}, {"Zeldin", "Uther"}},
    {"SLA451", 100, {"Tyler", "Singer", "Shaw"}, {"Zeldin", "Uther", "Richards", "Banks"}}
};

// Function to initialize the population
vector<vector<Assignment>> initialize_population(int population_size, const vector<Activity>& activities, const vector<Room>& rooms, const vector<Facilitator>& facilitators, const vector<TimeSlot>& time_slots) {
     vector<vector<Assignment>> population(population_size);

     // Validate time slots
     for (const auto& activity : activities) {
          for (const auto& preferred_time : time_slots) {
               bool time_found = false;
               for (const auto& assigned_time : time_slots) {
                    if (preferred_time.time == assigned_time.time) {
                         time_found = true;
                         break;
                    }
               }
               if (!time_found) {
                    cerr << "Error: Time slot " << preferred_time.time << " is missing." << endl;
                    // Handle the error (e.g., choose a default time slot)
               }
          }
     }

     // Initialize population with valid time slots
     // Iterate over the population size
     for (int i = 0; i < population_size; ++i) {
          // Create a new schedule
          vector<Assignment> schedule;

          // Randomly assign activities to rooms, facilitators, and time slots
          for (const auto& activity : activities) {
               Assignment assignment;
               assignment.activity_name = activity.name;

               // Randomly select room
               int room_index = rand() % rooms.size();
               assignment.room_name = rooms[room_index].name;

               // Randomly select facilitator
               vector<string> possible_facilitators = activity.preferred_activities;
               possible_facilitators.insert(possible_facilitators.end(), activity.other_activities.begin(), activity.other_activities.end());
               int facilitator_index = rand() % possible_facilitators.size();
               assignment.facilitator_name = possible_facilitators[facilitator_index];

               // Randomly select time slot (ensure it exists in time_slots vector)
               int time_slot_index = rand() % time_slots.size();
               assignment.time = time_slots[time_slot_index].time;

               // Add assignment to schedule
               schedule.push_back(assignment);
          }

          // Add the generated schedule to the population
          population[i] = schedule;
     }

     return population;
}

// Function to perform tournament selection
vector<vector<Assignment>> tournament_selection(const vector<vector<Assignment>>& population, int tournament_size) {
     vector<vector<Assignment>> selected_parents;

     // Perform tournament selection until enough parents are selected
     while (selected_parents.size() < population.size()) {
          // Randomly select individuals for the tournament
          vector<int> tournament_indices;
          while (tournament_indices.size() < tournament_size) {
               int index = rand() % population.size();
               tournament_indices.push_back(index);
          }

          // Determine the fittest individual from the tournament
          double best_fitness = -1.0;
          vector<Assignment> best_individual;
          for (int index : tournament_indices) {
               double fitness = calculate_fitness(population[index]);
               if (fitness > best_fitness) {
                    best_fitness = fitness;
                    best_individual = population[index];
               }
          }

          // Add the fittest individual to the selected parents
          selected_parents.push_back(best_individual);
     }

     return selected_parents;
}

// Function to perform one-point crossover
pair<vector<Assignment>, vector<Assignment>> one_point_crossover(const vector<Assignment>& parent1, const vector<Assignment>& parent2) {
     int crossover_point = rand() % (parent1.size() - 1) + 1; // Random crossover point, ensuring it's not at the beginning

     vector<Assignment> offspring1(parent1.begin(), parent1.begin() + crossover_point);
     offspring1.insert(offspring1.end(), parent2.begin() + crossover_point, parent2.end());

     vector<Assignment> offspring2(parent2.begin(), parent2.begin() + crossover_point);
     offspring2.insert(offspring2.end(), parent1.begin() + crossover_point, parent1.end());

     return make_pair(offspring1, offspring2);
}

// Function to perform mutation
void mutate(vector<Assignment>& schedule, double mutation_rate, const vector<Room>& rooms, const vector<Facilitator>& facilitators, const vector<TimeSlot>& time_slots) {
     // Iterate over each assignment in the schedule
     for (Assignment& assignment : schedule) {
          // Mutate room with a certain probability
          if (rand() / static_cast<double>(RAND_MAX) < mutation_rate) {
               // Randomly select a new room
               int room_index = rand() % rooms.size();
               assignment.room_name = rooms[room_index].name;
          }

          // Mutate facilitator with a certain probability
          if (rand() / static_cast<double>(RAND_MAX) < mutation_rate) {
               // Randomly select a new facilitator
               int facilitator_index = rand() % facilitators.size();
               assignment.facilitator_name = facilitators[facilitator_index].name;
          }

          // Mutate time slot with a certain probability
          if (rand() / static_cast<double>(RAND_MAX) < mutation_rate) {
               // Randomly select a new time slot
               int time_slot_index = rand() % time_slots.size();
               assignment.time = time_slots[time_slot_index].time;
          }
     }
}

// Function to calculate fitness for a single activity
double calculate_activity_fitness(const Assignment& assignment, const vector<Assignment>& schedule, const vector<Room>& rooms, const vector<Facilitator>& facilitators) {
     double fitness = 0.0;

     // Check if the activity is scheduled at the same time in the same room as another activity
     for (const auto& other_assignment : schedule) {
          if (assignment.activity_name != other_assignment.activity_name && assignment.time == other_assignment.time && assignment.room_name == other_assignment.room_name) {
               fitness -= 0.5;
               break;
          }
     }

     // Check room size
     double room_capacity = 0.0;
     for (const auto& room : rooms) {
          if (room.name == assignment.room_name) {
               room_capacity = room.capacity;
               break;
          }
     }
     double expected_enrollment = 0.0;
     for (const auto& activity : activities) {
          if (activity.name == assignment.activity_name) {
               expected_enrollment = activity.expected_enrollment;
               break;
          }
     }
     if (room_capacity < expected_enrollment) {
          fitness -= 0.5;
     }
     else if (room_capacity > 6 * expected_enrollment) {
          fitness -= 0.4;
     }
     else if (room_capacity > 3 * expected_enrollment) {
          fitness -= 0.2;
     }
     else {
          fitness += 0.3;
     }

     // Check facilitator
     bool preferred_facilitator = false;
     bool other_facilitator = false;
     for (const auto& facilitator : facilitators) {
          if (facilitator.name == assignment.facilitator_name) {
               if (find(facilitator.preferred_activities.begin(), facilitator.preferred_activities.end(), assignment.activity_name) != facilitator.preferred_activities.end()) {
                    preferred_facilitator = true;
               }
               else if (find(facilitator.other_activities.begin(), facilitator.other_activities.end(), assignment.activity_name) != facilitator.other_activities.end()) {
                    other_facilitator = true;
               }
               else {
                    fitness -= 0.1;
               }
               // Check facilitator load
               if (facilitator.total_activities == 1) {
                    fitness += 0.2;
               }
               else if (facilitator.total_activities > 1) {
                    fitness -= 0.2;
               }
               if (facilitator.total_activities > 4) {
                    fitness -= 0.5;
               }
               if ((facilitator.total_activities == 1 || facilitator.total_activities == 2) && facilitator.name != "Tyler") {
                    fitness -= 0.4;
               }
               break;
          }
     }
     if (preferred_facilitator) {
          fitness += 0.5;
     }
     else if (other_facilitator) {
          fitness += 0.2;
     }


     // Adjust the activity-specific adjustments using the helper function
     if (assignment.activity_name == "SLA100A" || assignment.activity_name == "SLA100B") {
          // Check if both sections are more than 4 hours apart
          int assignment_index = find_time_slot_index(time_slots, assignment.time);
          for (const auto& other_assignment : schedule) {
               if ((other_assignment.activity_name == "SLA100A" || other_assignment.activity_name == "SLA100B") &&
                    other_assignment.time != assignment.time) {
                    int other_assignment_index = find_time_slot_index(time_slots, other_assignment.time);
                    if (abs(assignment_index - other_assignment_index) > 1) {
                         fitness += 0.5;
                    }
                    else {
                         fitness -= 0.5;
                    }
                    break;
               }
          }
     }
     // Assuming you've already implemented the find_time_slot_index function as previously described
     else if (assignment.activity_name == "SLA191A" || assignment.activity_name == "SLA191B") {
          // Check if both sections of SLA191 are more than 4 hours apart
          int assignment_index = find_time_slot_index(time_slots, assignment.time);
          for (const auto& other_assignment : schedule) {
               if ((other_assignment.activity_name == "SLA191A" || other_assignment.activity_name == "SLA191B") &&
                    other_assignment.time != assignment.time) {
                    int other_assignment_index = find_time_slot_index(time_slots, other_assignment.time);
                    if (abs(assignment_index - other_assignment_index) > 1) {
                         fitness += 0.5;
                    }
                    else {
                         fitness -= 0.5;
                    }
                    break;
               }
          }
     }
     else if ((assignment.activity_name == "SLA100A" || assignment.activity_name == "SLA100B") ||
          (assignment.activity_name == "SLA191A" || assignment.activity_name == "SLA191B")) {
          // Check if sections of SLA100 and SLA191 are taught in consecutive time slots
          int assignment_index = find_time_slot_index(time_slots, assignment.time);
          for (const auto& other_assignment : schedule) {
               if (((assignment.activity_name == "SLA100A" || assignment.activity_name == "SLA100B") &&
                    (other_assignment.activity_name == "SLA191A" || other_assignment.activity_name == "SLA191B")) ||
                    ((assignment.activity_name == "SLA191A" || assignment.activity_name == "SLA191B") &&
                         (other_assignment.activity_name == "SLA100A" || other_assignment.activity_name == "SLA100B"))) {
                    int other_assignment_index = find_time_slot_index(time_slots, other_assignment.time);
                    if (abs(assignment_index - other_assignment_index) == 1) {
                         // Check if one activity is in Roman or Beach and the other isn't
                         bool roman_or_beach = (assignment.room_name.find("Roman") != string::npos) || (assignment.room_name.find("Beach") != string::npos);
                         bool other_roman_or_beach = (other_assignment.room_name.find("Roman") != string::npos) || (other_assignment.room_name.find("Beach") != string::npos);
                         if (roman_or_beach != other_roman_or_beach) {
                              fitness -= 0.4;
                         }
                         fitness += 0.5;
                    }
                    break;
               }
          }
     }
     else {
          // Check if an SLA100 and SLA191 are taught separated by 1 hour
          int assignment_index = find_time_slot_index(time_slots, assignment.time);
          for (const auto& other_assignment : schedule) {
               if (((assignment.activity_name == "SLA101A" || assignment.activity_name == "SLA101B") &&
                    (other_assignment.activity_name == "SLA191A" || other_assignment.activity_name == "SLA191B")) ||
                    ((assignment.activity_name == "SLA191A" || assignment.activity_name == "SLA191B") &&
                         (other_assignment.activity_name == "SLA100A" || other_assignment.activity_name == "SLA100B"))) {
                    int other_assignment_index = find_time_slot_index(time_slots, other_assignment.time);
                    if (abs(assignment_index - other_assignment_index) == 2) {
                         fitness += 0.25;
                    }
                    else {
                         fitness -= 0.25;
                    }
                    break;
               }
          }
     }


     return fitness;
}

// Helper function to find the index of a specific time in the time_slots vector
int find_time_slot_index(const vector<TimeSlot>& time_slots, const string& time) {
     auto it = find_if(time_slots.begin(), time_slots.end(), [&](const TimeSlot& slot) { return slot.time == time; });
     if (it != time_slots.end()) {
          return distance(time_slots.begin(), it);
     }
     return -1; // Return -1 if the time is not found
}

// Function to calculate fitness for the entire schedule
double calculate_fitness(const vector<Assignment>& schedule) {
     double total_fitness = 0.0;

     // Iterate over each assignment in the schedule and calculate fitness
     for (const auto& assignment : schedule) {
          total_fitness += calculate_activity_fitness(assignment, schedule, rooms, facilitators);
     }

     return total_fitness;
}

int main() {
     // Initialize population
     int population_size = 500; // Adjust population size as needed
     vector<vector<Assignment>> population = initialize_population(population_size, activities, rooms, facilitators, time_slots);

     // Main loop
     int num_generations = 100; // Adjust number of generations as needed
     double mutation_rate = 0.03; // Adjust mutation rate as needed
     for (int generation = 0; generation < num_generations; ++generation) {
          // Selection
          vector<vector<Assignment>> selected_parents = tournament_selection(population, 2); // Tournament selection with tournament size 2

          // Crossover
          vector<vector<Assignment>> offspring_population;
          for (size_t i = 0; i < selected_parents.size(); i += 2) {
               auto offspring_pair = one_point_crossover(selected_parents[i], selected_parents[i + 1]);
               offspring_population.push_back(offspring_pair.first);
               offspring_population.push_back(offspring_pair.second);
          }

          // Mutation
          for (vector<Assignment>& offspring : offspring_population) {
               mutate(offspring, mutation_rate, rooms, facilitators, time_slots);
          }

          // Calculate fitness for each schedule in the offspring population
          vector<double> offspring_fitness;
          for (const auto& offspring : offspring_population) {
               offspring_fitness.push_back(calculate_fitness(offspring));
          }

          // Replace the old population with the offspring population
          population = offspring_population;

          // Find the best fitness of the current generation
          double best_fitness = *max_element(offspring_fitness.begin(), offspring_fitness.end());

          // Report best fitness of the current generation
          cout << "Generation " << generation + 1 << ": Best Fitness = " << best_fitness << endl;
     }

     // Report final best schedule and its fitness
     // Find the index of the best schedule in the population
     double best_fitness = -1.0;
     size_t best_schedule_index = 0;
     for (size_t i = 0; i < population.size(); ++i) {
          double fitness = calculate_fitness(population[i]);
          if (fitness > best_fitness) {
               best_fitness = fitness;
               best_schedule_index = i;
          }
     }

     vector<Assignment> best_schedule = population[best_schedule_index];

     // Initialize a 2-dimensional array with 4 rows and 11 columns
     string schedule_array_temp[4][11];

     // Iterate over the best schedule and copy values to the array
     for (size_t i = 0; i < best_schedule.size(); ++i) {
          schedule_array_temp[0][i] = best_schedule[i].activity_name;
          schedule_array_temp[1][i] = best_schedule[i].room_name;
          schedule_array_temp[2][i] = best_schedule[i].facilitator_name;
          schedule_array_temp[3][i] = best_schedule[i].time;
     }

     // Initialize a 3-dimensional array
     const int ROWS = 1;
     const int ARRAYS = 3;
     const int COLUMNS = 6; // Assuming there are 6 time slots
     string schedule_array[ARRAYS][ROWS][COLUMNS + 1]; // Add 1 extra column for activity name

     // Time slots
     string time_slots[] = { "10 AM", "11 AM", "12 PM", "1 PM", "2 PM", "3 PM" };

     // Iterate over the best schedule and extract the class names
     for (const auto& assignment : best_schedule) {
          string activity_name = assignment.activity_name;
          string room_name = assignment.room_name;
          string facilitator_name = assignment.facilitator_name;
          string time = assignment.time;

          // Calculate the position in the 3-dimensional array
          int time_index = -1;
          for (int j = 0; j < COLUMNS; ++j) {
               if (time_slots[j] == time) {
                    time_index = j;
                    break;
               }
          }

          // Find the first empty row for the current time slot
          for (int i = 0; i < ARRAYS; ++i) {
               if (schedule_array[i][0][time_index].empty()) {
                    // Copy the class name into the array
                    schedule_array[i][0][time_index] = activity_name;
                    break;
               }
          }
     }
     cout << endl;
     cout << "Best Schedule (Final Generation):" << endl;
     cout << endl;
     // Print the 3-dimensional array
     for (int i = 0; i < ARRAYS; ++i) {
          switch (i) {
          case 0:
               cout << "Monday" << ":" << endl;
               break;

          case 1:
               cout << "Wednesday" << ":" << endl;
               break;

          case 2:
               cout << "Friday" << ":" << endl;
               break;
          default:
               break;
          }

          // Iterate over the rows (ROW) and columns (COLUMNS) of the 3-dimensional array
          for (int row = 0; row < ROWS; ++row) {
               for (int col = 0; col < COLUMNS; ++col) {
                    // Check if the activity name in the 3-dimensional array matches the one in the 2-dimensional array
                    if (!schedule_array[i][row][col].empty()) {
                         for (size_t k = 0; k < best_schedule.size(); ++k) {
                              // If the activity names match, print out the information
                              if (schedule_array[i][row][col] == schedule_array_temp[0][k]) {
                                   cout << "Activity: " << schedule_array_temp[0][k]
                                        << ", Room: " << schedule_array_temp[1][k]
                                        << ", Facilitator: " << schedule_array_temp[2][k]
                                        << ", Time: " << schedule_array_temp[3][k] << endl;
                              }
                         }
                    }
               }
          }
          cout << endl;
     }


     //// Output the best schedule and its fitness
     //cout << "Best Schedule (Final Generation):" << endl;
     //for (const auto& assignment : population[best_schedule_index]) {
     //     cout << "Activity: " << assignment.activity_name << ", Room: " << assignment.room_name << ", Facilitator: " << assignment.facilitator_name << ", Time: " << assignment.time << endl;
     //}

     cout << "Fitness of the Best Schedule: " << best_fitness << endl;

     return 0;
}