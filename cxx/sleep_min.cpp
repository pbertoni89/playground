// this_thread::sleep_for example
#include <iostream>       // std::cout
#include <iomanip>        // std::put_time
#include <thread>         // std::this_thread::sleep_until
#include <chrono>         // std::chrono::system_clock
#include <ctime>          // std::time_t, std::tm, std::localtime, std::mktime

int main()
{
  using std::chrono::system_clock;
  std::time_t tt = system_clock::to_time_t (system_clock::now());

  struct std::tm * ptm = std::localtime(&tt);
  std::cout << "Current time: " << std::put_time(ptm,"%X") << '\n';

  std::cout << "Waiting for the next minute to begin..." << std::endl;

  ptm->tm_min ++;
  ptm->tm_sec = 0;
  ptm->tm_usec = 100;
  std::this_thread::sleep_until (system_clock::from_time_t (mktime(ptm)));

  std::cout << std::put_time(ptm,"%X") << " reached!\n";

  return 0;
}
