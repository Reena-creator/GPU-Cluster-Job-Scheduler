#include <iostream>
#include <vector>
#include <algorithm>
#include <climits>
using namespace std;

class GPUNode {
public:
    int nodeId;
    int totalGPUs;
    int availableGPUs;
    int memoryPerGPU;

    GPUNode(int id, int gpus, int memory)
        : nodeId(id), totalGPUs(gpus),
          availableGPUs(gpus), memoryPerGPU(memory) {}

    bool canRun(int gpusRequired, int memoryRequired) const {
        return availableGPUs >= gpusRequired &&
               memoryPerGPU >= memoryRequired;
    }

    bool allocateGPU(int gpusRequired, int memoryRequired) {
        if (!canRun(gpusRequired, memoryRequired))
            return false;

        availableGPUs -= gpusRequired;
        return true;
    }

    void releaseGPU(int gpusToRelease) {
        availableGPUs += gpusToRelease;

        if (availableGPUs > totalGPUs)
            availableGPUs = totalGPUs;
    }

    void display() const {
        cout << "Node " << nodeId
             << " | Available GPUs: " << availableGPUs
             << "/" << totalGPUs
             << " | Memory per GPU: "
             << memoryPerGPU << " GB\n";
    }
};

class Job {
public:
    int jobId;
    int gpuRequired;
    int memoryRequired;
    int arrivalTime;
    int executionTime;
    int priority;

    bool started;
    bool completed;
    int startTime;

    Job(int id, int gpus, int memory,
        int arrival, int runtime, int p)
        : jobId(id), gpuRequired(gpus),
          memoryRequired(memory), arrivalTime(arrival),
          executionTime(runtime), priority(p),
          started(false), completed(false), startTime(-1) {}

    void display() const {
        cout << "Job " << jobId
             << " | Arrival: " << arrivalTime
             << " | GPUs: " << gpuRequired
             << " | Memory/GPU: " << memoryRequired
             << " GB | Runtime: " << executionTime
             << " | Priority: " << priority << "\n";
    }
};

struct RunningJob {
    int jobIndex;
    int nodeIndex;
    int startTime;
    int finishTime;
};

int main() {
    vector<GPUNode> nodes;
    nodes.push_back(GPUNode(1, 4, 32));
    nodes.push_back(GPUNode(2, 4, 32));

    vector<Job> jobs;
    jobs.push_back(Job(101, 3, 16, 0, 10, 2));
    jobs.push_back(Job(102, 2, 24, 0, 20, 1));
    jobs.push_back(Job(103, 2, 8, 1, 5, 3));
    jobs.push_back(Job(104, 3, 16, 2, 4, 2));
    jobs.push_back(Job(105, 2, 8, 3, 3, 1));

    for (size_t i = 0; i < jobs.size(); i++) {
        bool feasible = false;

        for (size_t n = 0; n < nodes.size(); n++) {
            if (nodes[n].totalGPUs >= jobs[i].gpuRequired &&
                nodes[n].memoryPerGPU >= jobs[i].memoryRequired) {
                feasible = true;
                break;
            }
        }

        if (!feasible || jobs[i].gpuRequired <= 0 ||
            jobs[i].memoryRequired <= 0 ||
            jobs[i].executionTime <= 0 ||
            jobs[i].arrivalTime < 0) {
            cout << "Invalid or infeasible job: "
                 << jobs[i].jobId << "\n";
            return 1;
        }
    }

    stable_sort(jobs.begin(), jobs.end(),
        [](const Job& a, const Job& b) {
            if (a.arrivalTime != b.arrivalTime)
                return a.arrivalTime < b.arrivalTime;

            if (a.priority != b.priority)
                return a.priority < b.priority;

            return a.jobId < b.jobId;
        });

    vector<RunningJob> runningJobs;
    vector<int> waitingJobs;

    int nextJob = 0;
    int currentTime = 0;
    int completedCount = 0;

    cout << "===== GPU CLUSTER JOB SCHEDULER =====\n";
    for (size_t n = 0; n < nodes.size(); n++)
        nodes[n].display();

    cout << "\n===== JOB QUEUE =====\n";
    for (size_t i = 0; i < jobs.size(); i++)
        jobs[i].display();

    while (completedCount < (int)jobs.size()) {
        while (nextJob < (int)jobs.size() &&
               jobs[nextJob].arrivalTime <= currentTime) {

            waitingJobs.push_back(nextJob);

            cout << "\nTime " << currentTime
                 << ": Job " << jobs[nextJob].jobId
                 << " arrived\n";

            nextJob++;
        }

        for (int i = 0; i < (int)runningJobs.size();) {
            if (runningJobs[i].finishTime <= currentTime) {
                int j = runningJobs[i].jobIndex;
                int n = runningJobs[i].nodeIndex;

                nodes[n].releaseGPU(jobs[j].gpuRequired);
                jobs[j].completed = true;
                completedCount++;

                cout << "Time " << currentTime
                     << ": Job " << jobs[j].jobId
                     << " completed on Node "
                     << nodes[n].nodeId << "\n";

                runningJobs.erase(runningJobs.begin() + i);
            } else {
                i++;
            }
        }

        while (!waitingJobs.empty()) {
            int headIndex = waitingJobs[0];
            Job& head = jobs[headIndex];

            int headNode = -1;

            for (int n = 0; n < (int)nodes.size(); n++) {
                if (nodes[n].canRun(head.gpuRequired,
                                    head.memoryRequired)) {
                    headNode = n;
                    break;
                }
            }

            if (headNode != -1) {
                nodes[headNode].allocateGPU(
                    head.gpuRequired, head.memoryRequired);

                head.started = true;
                head.startTime = currentTime;

                runningJobs.push_back({
                    headIndex,
                    headNode,
                    currentTime,
                    currentTime + head.executionTime
                });

                cout << "Time " << currentTime
                     << ": Job " << head.jobId
                     << " started on Node "
                     << nodes[headNode].nodeId
                     << "; finishes at "
                     << currentTime + head.executionTime
                     << "\n";

                waitingJobs.erase(waitingJobs.begin());
                continue;
            }

            int reservationTime = INT_MAX;

            for (int n = 0; n < (int)nodes.size(); n++) {
                if (nodes[n].memoryPerGPU < head.memoryRequired ||
                    nodes[n].totalGPUs < head.gpuRequired) {
                    continue;
                }

                int freeGPUs = nodes[n].availableGPUs;

                vector<pair<int, int> > releases;

                for (size_t r = 0; r < runningJobs.size(); r++) {
                    if (runningJobs[r].nodeIndex == n) {
                        int j = runningJobs[r].jobIndex;

                        releases.push_back(make_pair(
                            runningJobs[r].finishTime,
                            jobs[j].gpuRequired
                        ));
                    }
                }

                sort(releases.begin(), releases.end());

                int possibleTime = currentTime;

                if (freeGPUs < head.gpuRequired) {
                    for (size_t k = 0; k < releases.size(); k++) {
                        freeGPUs += releases[k].second;
                        possibleTime = releases[k].first;

                        if (freeGPUs >= head.gpuRequired)
                            break;
                    }
                }

                if (freeGPUs >= head.gpuRequired &&
                    possibleTime < reservationTime) {
                    reservationTime = possibleTime;
                }
            }

            bool backfilled = false;

            if (reservationTime != INT_MAX) {
                for (size_t w = 1; w < waitingJobs.size(); w++) {
                    int candidateIndex = waitingJobs[w];
                    Job& candidate = jobs[candidateIndex];

                    int candidateNode = -1;

                    for (int n = 0; n < (int)nodes.size(); n++) {
                        if (nodes[n].canRun(
                                candidate.gpuRequired,
                                candidate.memoryRequired)) {
                            int finish = currentTime +
                                         candidate.executionTime;

                            if (finish <= reservationTime) {
                                candidateNode = n;
                                break;
                            }
                        }
                    }

                    if (candidateNode != -1) {
                        nodes[candidateNode].allocateGPU(
                            candidate.gpuRequired,
                            candidate.memoryRequired);

                        candidate.started = true;
                        candidate.startTime = currentTime;

                        runningJobs.push_back({
                            candidateIndex,
                            candidateNode,
                            currentTime,
                            currentTime + candidate.executionTime
                        });

                        cout << "Time " << currentTime
                             << ": Job " << candidate.jobId
                             << " BACKFILLED on Node "
                             << nodes[candidateNode].nodeId
                             << "; finishes at "
                             << currentTime + candidate.executionTime
                             << "; head reservation at "
                             << reservationTime << "\n";

                        waitingJobs.erase(waitingJobs.begin() + w);
                        backfilled = true;
                        break;
                    }
                }
            }

            (void)backfilled;
            break;
        }

        if (completedCount == (int)jobs.size())
            break;

        int nextEvent = INT_MAX;

        for (size_t r = 0; r < runningJobs.size(); r++) {
            if (runningJobs[r].finishTime > currentTime) {
                nextEvent = min(nextEvent,
                                runningJobs[r].finishTime);
            }
        }

        if (nextJob < (int)jobs.size()) {
            nextEvent = min(nextEvent, jobs[nextJob].arrivalTime);
        }

        if (nextEvent == INT_MAX) {
            cout << "\nScheduler stopped: no future event available.\n";
            break;
        }

        if (nextEvent <= currentTime) {
            cout << "Error: simulation time failed to advance.\n";
            break;
        }

        currentTime = nextEvent;
    }

    cout << "\n===== FINAL JOB SUMMARY =====\n";

    for (size_t i = 0; i < jobs.size(); i++) {
        cout << "Job " << jobs[i].jobId
             << " | Arrival: " << jobs[i].arrivalTime
             << " | Start: " << jobs[i].startTime;

        if (jobs[i].completed) {
            int finishTime = jobs[i].startTime +
                             jobs[i].executionTime;

            cout << " | Finish: " << finishTime
                 << " | Turnaround: "
                 << finishTime - jobs[i].arrivalTime
                 << " | Waiting: "
                 << jobs[i].startTime - jobs[i].arrivalTime;
        } else {
            cout << " | Not completed";
        }

        cout << "\n";
    }

    cout << "\n===== FINAL GPU STATUS =====\n";
    for (size_t n = 0; n < nodes.size(); n++)
        nodes[n].display();

    return 0;
}
