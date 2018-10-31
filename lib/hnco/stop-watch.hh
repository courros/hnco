#include <ctime>                // clock, clock_t


namespace hnco
{

  /// Stop watch
  class StopWatch
  {
    /// Total time
    double _total = 0;

    /// Start time
    clock_t _start;

  public:

    /// Start
    void start() {
      _start = clock();
    }

    /// Stop
    void stop() {
      _total += double(clock() - _start) / CLOCKS_PER_SEC;
    }

    /// Get total
    double get_total() { return _total; }

  };

}
