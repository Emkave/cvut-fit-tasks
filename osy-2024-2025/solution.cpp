#ifndef __PROGTEST__
#include <cstdio>
#include <cstdlib>                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                        //01040106031501201012
#include <cstring>
#include <cstdint>
#include <climits>
#include <cfloat>
#include <cassert>
#include <cmath>
#include <iostream>
#include <iomanip>
#include <algorithm>
#include <numeric>
#include <string>
#include <utility>
#include <vector>
#include <array>
#include <iterator>
#include <set>
#include <list>
#include <map>
#include <unordered_set>
#include <unordered_map>
#include <compare>
#include <queue>
#include <stack>
#include <deque>
#include <memory>
#include <functional>
#include <thread>
#include <mutex>
#include <atomic>
#include <chrono>
#include <stdexcept>
#include <condition_variable>
#include <pthread.h>
#include <semaphore.h>
#include "progtest_solver.h"
#include "sample_tester.h"
using namespace std;
#endif /* __PROGTEST__ */

#define checkpoint std::cout << "Checkpoint.\n";

class ThreadPool {      // Standard thread pool implementation
public:
    explicit ThreadPool(size_t);
    ~ThreadPool();
    void enqueue(const std::function<void()>& task);

private:
    std::vector<std::thread> workers;
    std::queue<std::function<void()>> tasks;

    std::mutex mtx2, mtx1, mtx3, mtx4, mtx5, mtx6;
    std::condition_variable condition;
    bool stop;

};


ThreadPool::ThreadPool(size_t threads) : stop(false) {
    for (size_t i=0; i<threads; i++) {
        this->workers.emplace_back([this]{
            while (true) {
                std::function<void()> task;

                {
                    std::unique_lock<std::mutex> lock (this->mtx2);
                    this->condition.wait(lock, [this]{return this->stop || !this->tasks.empty();});

                    if (this->stop && this->tasks.empty()) {
                        return;
                    }
                }

                {
                    std::unique_lock<std::mutex> lock (this->mtx3);

                    if (!this->tasks.empty()) {
                        task = std::move(this->tasks.front());
                        this->tasks.pop();
                    }
                }

                task();
            }
        });
    }
}


void ThreadPool::enqueue(const std::function<void()> & task) {
    {
        std::unique_lock<std::mutex> lock (this->mtx4);

        if (this->stop) {
            return;
        }

        tasks.emplace(task);
    }

    this->condition.notify_one();
}


ThreadPool::~ThreadPool() {
    {
        std::unique_lock<std::mutex> lock (this->mtx1);
        this->stop = true;
    }

    this->condition.notify_all();

    for (std::thread & worker : this->workers) {
        worker.join();
    }
}


//-------------------------------------------------------------------------------------------------------------------------------------------------------------
class COptimizer {
public:
    static bool usingProgtestSolver(void) {
        return true;
    }

    static void checkAlgorithmMin(APolygon p) {}

    static void checkAlgorithmCnt(APolygon p) {}

    ///Wrappers///=========================================================================================///
    ///ProblemPack wrapper defined///

     typedef struct ProblemPack {    //Encapsulation of the problem source
        AProblemPack source;          //Parent source
        __SIZE_TYPE__ problems;

        explicit ProblemPack(AProblemPack problem_pack) {
            this->source = std::move(problem_pack);

            if (this->source) {
                this->problems = this->source.get()->m_ProblemsCnt.size() + this->source.get()->m_ProblemsMin.size();
            }
        }
    } ProblemPack;

    ///Todo list for solvers///

    typedef struct Todo {
        std::shared_ptr<ProblemPack> source_pack = nullptr;
        size_t expected_to_solve_amount = 0;

        explicit Todo(std::shared_ptr<ProblemPack> pack) : source_pack(std::move(pack)) {}
    } Todo;


    ///Company wrapper defined///
     typedef struct Company {
        ACompany company;
        std::queue<std::shared_ptr<ProblemPack>> problem_pack_queue;

        explicit Company(ACompany company) {
            this->company = std::move(company);
        }

        void add_pack(const std::shared_ptr<ProblemPack> & pack) {
            this->problem_pack_queue.emplace(pack);
        }

        bool pop_pack(void) {
            if (!this->problem_pack_queue.empty()) {
                this->problem_pack_queue.pop();
                return true;
            }

            return false;
        }

        std::shared_ptr<ProblemPack> getProblemPack(void) {
            return std::make_shared<ProblemPack>(this->company->waitForPack());
        }
    } Company;

     ///Solvers wrapper defined///

     typedef struct Solver {
         AProgtestSolver solver;
         std::list<std::shared_ptr<Todo>> todo_list;

         Solver(AProgtestSolver solver_, std::list<std::shared_ptr<Todo>> todo_list) {
             this->solver = std::move(solver_);
             this->todo_list = std::move(todo_list);
         }
     } Solver;

    ///================================================================================================================///

    void start(int threadCount );
    void stop();
    inline void min_work();
    inline void cnt_work();
    void addCompany(ACompany company);

    std::queue<Solver> CntSolvers, MinSolvers;    // Queue of solvers to be executed
    std::vector<Company *> companies;

    std::shared_ptr<ThreadPool> thread_pool;

    std::condition_variable solved;
    std::mutex mtx1, mtx2, mtx3, mtx4, mtx5, mtx6;
    std::atomic<size_t> active_receivers = 0;



private:
    std::vector<std::thread> receivers;
    std::vector<std::thread> senders;

    void sender(Company * company);
    void receiver(Company * company);

};


inline void COptimizer::cnt_work() {
    {
        std::unique_lock<std::mutex> lock (this->mtx3);

        if (!this->CntSolvers.empty()) {
            Solver solver = this->CntSolvers.front();
            this->CntSolvers.pop();

            lock.unlock();


            solver.solver->solve();
            this->mtx5.lock();
            for (auto & task : solver.todo_list) {
                task->source_pack->problems -= task->expected_to_solve_amount;
            }

            this->solved.notify_all();
            this->mtx5.unlock();
        }
    }
}


inline void COptimizer::min_work() {
    {
        std::unique_lock<std::mutex> lock (this->mtx4);

        if (!this->MinSolvers.empty()) {
            Solver solver = this->MinSolvers.front();
            this->MinSolvers.pop();
            lock.unlock();

            solver.solver->solve();

            this->mtx6.lock();
            for (auto & task : solver.todo_list) {
                task->source_pack->problems -= task->expected_to_solve_amount;
            }

            this->solved.notify_all();
            this->mtx6.unlock();
        }
    }
}


void COptimizer::sender(Company * company) {
    while (true) { // It will work untill all solvers are solved
        std::unique_lock<std::mutex> lock (this->mtx5);

        this->solved.wait(lock, [company,this](){
            return (!company->problem_pack_queue.empty() and !company->problem_pack_queue.front()->problems) || (company->problem_pack_queue.empty() and this->MinSolvers.empty() and this->CntSolvers.empty());
        });

        if (company->problem_pack_queue.empty() and this->MinSolvers.empty() and this->CntSolvers.empty()) {
            break;
        }

        company->company->solvedPack(company->problem_pack_queue.front()->source);      // Send the solved stuff
        company->pop_pack();        // Pop the pack from the queue as solved.
    }
}


void COptimizer::receiver(Company * company) {
    while (true) {
        std::shared_ptr<ProblemPack> new_pack = company->getProblemPack();      // Get a package

        if (new_pack->source == nullptr) {
            this->active_receivers--;           // In case if there are unfilled solvers as last, we will exit the loop and launch add final tasks for the threads
            break;
        }

        company->add_pack(new_pack);            // Add package to the queue of packages of the company

        {
            std::shared_ptr<Todo> task = std::make_shared<Todo>(new_pack);
            size_t problem_counter = 0;

            for (size_t i = 0; i < new_pack->source->m_ProblemsCnt.size(); i++) {
                std::unique_lock<std::mutex> lock(this->mtx2);     // Same applies to here
                this->CntSolvers.back().solver->addPolygon(new_pack->source->m_ProblemsCnt[i]);
                problem_counter++;

                if (i + 1 == new_pack->source->m_ProblemsCnt.size()) {
                    task->expected_to_solve_amount = problem_counter;
                    problem_counter = 0;
                    this->CntSolvers.back().todo_list.emplace_back(task);
                }

                if (!this->CntSolvers.back().solver->hasFreeCapacity()) {
                    if (problem_counter) {
                        task->expected_to_solve_amount = problem_counter;
                        problem_counter = 0;
                        this->CntSolvers.back().todo_list.emplace_back(task);
                        task = std::make_shared<Todo>(new_pack);
                    }

                    this->CntSolvers.emplace(createProgtestCntSolver(), std::list<std::shared_ptr<Todo>>{});
                    this->thread_pool->enqueue([this]() { this->cnt_work(); });
                }
            }
        }

        {
            std::shared_ptr<Todo> task = std::make_shared<Todo>(new_pack);          // Needed to store info about tasks the solver will be doing to subtract later and call notify
            size_t problem_counter = 0;             // This counter is used to assign it later on to TODO instance and with the solver

            for (size_t i=0; i<new_pack->source->m_ProblemsMin.size(); i++) {   // Iterate over the problems
                std::unique_lock<std::mutex> lock(this->mtx1);     // Same applies to here

                this->MinSolvers.back().solver->addPolygon(new_pack->source->m_ProblemsMin[i]);     // Add problems
                problem_counter++;

                if (i + 1 == new_pack->source->m_ProblemsMin.size()) {      // In case if we have encountered a last problem from the package and the solver is not full yet, we add new todo task for the thread
                    task->expected_to_solve_amount = problem_counter;
                    problem_counter = 0;
                    this->MinSolvers.back().todo_list.emplace_back(task);
                }

                if (!this->MinSolvers.back().solver->hasFreeCapacity()) {
                    if (problem_counter) {                                  // If we encountered limit and haven't placed any problem yet, there is no need to pass tasks info for the thread
                        task->expected_to_solve_amount = problem_counter;
                        problem_counter = 0;
                        this->MinSolvers.back().todo_list.emplace_back(task);
                        task = std::make_shared<Todo>(new_pack);
                    }

                    this->MinSolvers.emplace(createProgtestMinSolver(), std::list<std::shared_ptr<Todo>>{});    // Create new solver
                    this->thread_pool->enqueue([this](){this->min_work();});            // Tell the thread that there is a ready solver to be worked
                }
            }
        }
    }

    if (!this->active_receivers) {
        this->thread_pool->enqueue([this](){this->min_work();});
        this->thread_pool->enqueue([this](){this->cnt_work();});
    }
}


void COptimizer::start(int threadCount) {
    this->thread_pool = std::make_shared<ThreadPool>(threadCount);
    this->MinSolvers.emplace(createProgtestMinSolver(), std::list<std::shared_ptr<Todo>>{});
    this->CntSolvers.emplace(createProgtestCntSolver(), std::list<std::shared_ptr<Todo>>{});
    this->active_receivers = this->companies.size();

    for (size_t i=0; i<this->companies.size(); i++) {
        this->receivers.emplace_back(std::thread(&COptimizer::receiver, this, this->companies[i]));
        this->senders.emplace_back(std::thread(&COptimizer::sender, this, this->companies[i]));
    }
}


void COptimizer::stop() {

    for (std::thread & thread : this->receivers) {
        thread.join();
    }

    for (std::thread & thread : this->senders) {
        thread.join();
    }

    this->thread_pool.reset();

    for (Company * company : this->companies) {
        delete company;
    }
}


void COptimizer::addCompany(ACompany company) {
    Company * new_company = new Company(std::move(company));
    this->companies.emplace_back(new_company);
}


// TODO: COptimizer implementation goes here
//-------------------------------------------------------------------------------------------------------------------------------------------------------------
#ifndef __PROGTEST__

int main(void) {
    for (size_t i=0; i<1; i++) {
        COptimizer optimizer;
        ACompanyTest c1 = std::make_shared<CCompanyTest>();
        /*ACompanyTest c2 = std::make_shared<CCompanyTest>();
        ACompanyTest c3 = std::make_shared<CCompanyTest>();
        ACompanyTest c4 = std::make_shared<CCompanyTest>();
        ACompanyTest c5 = std::make_shared<CCompanyTest>();
        ACompanyTest c6 = std::make_shared<CCompanyTest>();
        ACompanyTest c7 = std::make_shared<CCompanyTest>();*/

        optimizer.addCompany(c1);
        /*optimizer.addCompany(c2);
        optimizer.addCompany(c3);
        optimizer.addCompany(c4);
        optimizer.addCompany(c5);
        optimizer.addCompany(c6);
        optimizer.addCompany(c7);*/
        optimizer.start(1);
        optimizer.stop();

        if (!c1->allProcessed()) {
            std::cout << "C1: (some) problems were not correctly processsed.\n";
        }

        /*if (!c2->allProcessed()) {
            std::cout << "C2: (some) problems were not correctly processsed.\n";
        }

        if (!c3->allProcessed()) {
            std::cout << "C3: (some) problems were not correctly processsed.\n";
        }

        if (!c4->allProcessed()) {
            std::cout << "C4: (some) problems were not correctly processsed.\n";
        }

        if (!c5->allProcessed()) {
            std::cout << "C5: (some) problems were not correctly processsed.\n";
        }

        if (!c6->allProcessed()) {
            std::cout << "C6: (some) problems were not correctly processsed.\n";
        }

        if (!c7->allProcessed()) {
            std::cout << "C7: (some) problems were not correctly processsed.\n";
        }*/

    }

    return 0;
}
#endif /* __PROGTEST__ */