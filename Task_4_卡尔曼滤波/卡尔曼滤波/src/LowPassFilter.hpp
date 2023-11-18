//LowPassFilter.hpp

#ifndef LOWPASSFILTER_HPP
#define LOWPASSFILTER_HPP

#include "Eigen/Dense"

template<class T, int x>
class LowPassFilter
{
    Eigen::Matrix<T, x, 1> prev_output;
    double alpha;
public:
    LowPassFilter(double sample_rate, double cutoff_frequency)
    {
        double dt = 1.0 / sample_rate;
        double RC = 1.0 / (cutoff_frequency * 2.0 * M_PI);
        alpha = dt / (dt + RC);
        prev_output = Eigen::Matrix<T, x, 1>::Zero();
    }

    Eigen::Matrix<T, x, 1> update(const Eigen::Matrix<T, x, 1>& input)
    {
        Eigen::Matrix<T, x, 1> output = alpha * input + (1.0 - alpha) * prev_output;
        prev_output = output;
        return output;
    }
    ~LowPassFilter() = default;
};

#endif //LOWPASSFILTER_HPP