#include <iostream>
#include <thread>
#include <condition_variable>
#include <mutex>
#include <chrono>
#include <queue>

std::condition_variable cond_var;
std::mutex m;


int main()
{
    int value = 100;
    bool notified = false;

    std::thread reporter([&]()
    {
        /*
        unique_lock<mutex> lock(m);
        while (!notified) {
            cond_var.wait(lock);
        }
        */
        std::cout << "The value is " << value << std::endl;
    });

    std::thread assigner([&]()
    {
        value = 20;
        /*
        notified = true;
        cond_var.notify_one();
        */
    });

    reporter.join();
    assigner.join();
    return 0;
}
