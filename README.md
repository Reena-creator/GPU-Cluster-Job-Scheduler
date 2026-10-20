# 🚀 GPU Cluster Job Scheduler

### A C++ Simulation | GPU Resource Allocation · Priority Scheduling · Backfilling

A **GPU Cluster Job Scheduler** that simulates how computational jobs are allocated to GPU nodes based on GPU availability, memory requirements, arrival times, execution durations, and priorities.

Built in C++, this project demonstrates resource management and scheduling strategies used in shared computing environments. It includes job arrivals, execution tracking, resource release, reservation-time estimation, and backfilling to improve GPU utilization while respecting the waiting queue's head-of-line reservation.

---

## ✨ Features

- 🖥️ **GPU Cluster Simulation** — Models multiple GPU nodes with configurable GPU capacity and memory per GPU.
- 📥 **Job Queue Management** — Handles jobs with different arrival times, GPU requirements, memory requirements, execution times, and priorities.
- ⚖️ **Priority-Based Scheduling** — Orders jobs by arrival time, priority, and job ID.
- 🔧 **GPU Resource Allocation** — Allocates GPUs when a node has sufficient available resources and memory capacity.
- ⏱️ **Event-Driven Simulation** — Advances simulation time to the next job arrival or job completion event.
- 🔒 **Head-of-Line Reservation** — Estimates when the first waiting job can obtain sufficient GPU resources.
- 🚄 **Backfilling** — Allows a later job to run if it can finish by the estimated reservation time of the first waiting job.
- ♻️ **Resource Release** — Returns allocated GPUs to a node when a job completes.
- 📊 **Job Performance Summary** — Reports arrival, start, finish, turnaround, and waiting times.
- 🛡️ **Input Validation** — Rejects jobs with invalid resource requirements or infeasible configurations.

---

## 🛠️ Tech Stack

| Technology | Purpose |
|---|---|
| C++ | Core implementation |
| STL `vector` | Stores GPU nodes, jobs, and running tasks |
| STL `algorithm` | Sorting jobs, ordering resource-release events, and finding minimum event times |
| Priority Ordering | Determines the order in which jobs enter the scheduling queue |
| Event-Driven Simulation | Models job arrivals and completions |
| Resource Allocation | Tracks available GPUs and releases them after execution |

---

## 🧠 Core Concepts

### 1. GPU Resource Allocation

Each GPU node maintains:
- Node ID
- Total GPU capacity
- Available GPUs
- Memory capacity per GPU

A job can run on a node only if the node has enough available GPUs and sufficient memory per GPU.

### 2. Priority-Based Job Scheduling

Jobs are sorted using the following order:

1. Earlier arrival time
2. Lower numerical priority value
3. Lower job ID

For example, a job with priority `1` is considered before a job with priority `3` when their arrival times are equal.

### 3. Head-of-Line Reservation

When the first waiting job cannot start immediately, the scheduler estimates the earliest time when a suitable node will have enough GPUs available.

This reservation time helps prevent smaller jobs from indefinitely delaying a larger waiting job.

### 4. Backfilling

Backfilling allows a later job in the queue to start before the first waiting job, provided that the later job can finish by the estimated reservation time.

This can improve resource utilization without delaying the reserved start time under the scheduler's estimated resource model.

### 5. Event-Driven Simulation

Instead of advancing time one unit at a time, the scheduler jumps to the next relevant event:
- A new job arrives.
- A running job completes.

This makes the simulation more efficient for jobs with different execution durations.

---

## 🏗️ System Architecture

The project uses three main data structures:

- **`GPUNode`** — Checks GPU availability, allocates resources, releases GPUs, and displays node status.
- **`Job`** — Stores job requirements, scheduling information, execution duration, and completion status.
- **`RunningJob`** — Tracks the job index, allocated node, start time, and finish time.

The main scheduler coordinates job arrivals, resource allocation, waiting-queue management, backfilling, event progression, and final performance reporting.

---

## 📋 Example Configuration

The simulation initializes two GPU nodes, each with four GPUs and 32 GB of memory per GPU.

| Node | Total GPUs | Memory per GPU |
|---|---:|---:|
| Node 1 | 4 | 32 GB |
| Node 2 | 4 | 32 GB |

It also initializes five jobs with different resource requirements and priorities.

| Job ID | GPUs Required | Memory per GPU | Arrival Time | Execution Time | Priority |
|---|---:|---:|---:|---:|---:|
| 101 | 3 | 16 GB | 0 | 10 | 2 |
| 102 | 2 | 24 GB | 0 | 20 | 1 |
| 103 | 2 | 8 GB | 1 | 5 | 3 |
| 104 | 3 | 16 GB | 2 | 4 | 2 |
| 105 | 2 | 8 GB | 3 | 3 | 1 |

*Note: The table shows simulation inputs, not guaranteed scheduling results. Actual execution order depends on resource availability and reservation estimates.*

---

## 🚀 Getting Started

### Prerequisites

- A C++ compiler supporting C++14 or later
- Windows PowerShell, Command Prompt, Linux terminal, or macOS terminal

### 1. Clone the Repository

```bash
git clone <YOUR_REPOSITORY_URL>
cd GPU-Cluster-Job-Scheduler
```

Replace `<YOUR_REPOSITORY_URL>` with your actual GitHub repository URL.

### 2. Compile the Program

Using GCC or MinGW:

```bash
g++ -std=c++14 main.cpp -o gpu_scheduler
```

### 3. Run the Application

**Windows PowerShell:**

```powershell
.\gpu_scheduler.exe
```

**Linux / macOS:**

```bash
./gpu_scheduler
```

---

## 📊 Output

The program displays:

- Initial GPU node status
- Job queue and resource requirements
- Job arrival events
- Job start and completion events
- Backfilled jobs and estimated reservation times
- Final job execution summary
- Turnaround and waiting times
- Final GPU availability for each node

### Performance Metrics

**Turnaround Time**

`Turnaround Time = Finish Time − Arrival Time`

**Waiting Time**

`Waiting Time = Start Time − Arrival Time`

These metrics help evaluate how long jobs wait before execution and how long they take from arrival to completion.

---

## 📚 Concepts Demonstrated

- GPU cluster resource management
- Priority-based scheduling
- Queue management and head-of-line blocking
- Backfilling and reservation-time estimation
- Event-driven discrete simulation
- Resource allocation and release
- C++ classes, structures, and STL containers
- Sorting, searching, and scheduling logic
- Job waiting time and turnaround time analysis

---

## 🔮 Future Improvements

- Support heterogeneous GPU nodes with different memory capacities.
- Add GPU memory accounting for total memory usage across concurrent jobs.
- Implement configurable scheduling policies such as FCFS, priority scheduling, and shortest-job-first.
- Compare scheduling strategies using average waiting time, turnaround time, and GPU utilization.
- Add job cancellation and failure recovery.
- Generate execution timelines or Gantt charts.
- Build a graphical dashboard for monitoring jobs and GPU resources.
- Add automated tests for scheduling edge cases.

---

## ⚠️ Project Scope

This project is an educational simulation of GPU cluster scheduling. It models resource allocation and job execution in software; it does not connect to, monitor, or allocate resources on real GPU hardware or a production cluster.

---

## 👩‍💻 Author

**Reena Meena**  
B.Tech Mechanical Engineering | IIT Indore

- GitHub: [@Reena-creator](https://github.com/Reena-creator)

---

<p align="center"><i>Exploring resource management and scheduling algorithms through practical C++ simulations.</i></p>