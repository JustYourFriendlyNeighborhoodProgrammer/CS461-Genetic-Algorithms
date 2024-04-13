# CS 461 AI - Program 2: Genetic Algorithm

This project implements a genetic algorithm to generate a space utilization schedule for a client agency's activities. The goal is to optimize the assignment of rooms, times, and facilitators for each activity.

## Problem Description

The client agency schedules activities for Mondays, Wednesdays, and Fridays, each lasting 50 minutes. Several factors influence scheduling, including room capacity, facilitator availability, and activity enrollment. The goal is to create a schedule that maximizes space utilization while meeting the requirements of each activity.

### Constraints and Considerations

- Activities are scheduled on MWF for 50 minutes.
- Each activity requires a room, time slot, and facilitator.
- Facilitators have preferences for certain activities but can oversee others if necessary.
- Room capacity must accommodate expected enrollment.
- The genetic algorithm will evolve random schedules to improve fitness over generations.

## Fitness Function

The fitness function evaluates each schedule based on several criteria, including:

- Avoiding conflicts in room and time assignments.
- Matching activities with preferred facilitators.
- Balancing facilitator loads.
- Considering activity-specific adjustments.

## Computing Fitness

The fitness score of a schedule is the sum of the scores for individual activities. Softmax normalization converts fitness scores into a probability distribution. Mutation rate and generation count are parameters to be adjusted for optimal performance.

## Programming Notes

- Utilize a standard random number generator.
- Implementations can be done in Python, C++, C#, or Java.
- Output should include source code, sample output, and a brief report discussing challenges, schedule evaluation, potential improvements, and any additional remarks.

## Appendices

### Activities

- SLA100A, SLA100B
- SLA191A, SLA191B
- SLA201
- SLA291
- SLA303
- SLA304
- SLA394
- SLA449
- SLA451

### Rooms

- Slater 003 (Capacity: 45)
- Roman 216 (Capacity: 30)
- Loft 206 (Capacity: 75)
- Roman 201 (Capacity: 50)
- Loft 310 (Capacity: 108)
- Beach 201 (Capacity: 60)
- Beach 301 (Capacity: 75)
- Logos 325 (Capacity: 450)
- Frank 119 (Capacity: 60)

### Times

- 10 AM
- 11 AM
- 12 PM
- 1 PM
- 2 PM
- 3 PM

## Conclusion

This project aims to develop an efficient scheduling algorithm using a genetic approach. By considering various constraints and utilizing evolutionary principles, the goal is to produce high-quality schedules for the client agency's activities.

Also Note: This was made with ChatGPT 3.5
