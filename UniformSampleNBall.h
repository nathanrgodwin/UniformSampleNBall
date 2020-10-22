#pragma once


#include <random>
#include <thread>

namespace nrg
{

template <unsigned int N, typename DataT>
struct n_root
{
    static DataT compute(const DataT& data)
    {
        return std::pow(data, 1.0 / (double) N);
    }
};

template<typename DataT>
struct n_root<2, DataT>
{
    static DataT compute(const DataT& data)
    {
        return std::sqrt(data);
    }
};

template<typename DataT>
struct n_root<3, DataT>
{
    static DataT compute(const DataT& data)
    {
        return std::cbrt(data);
    }
};

/**
* Samples a point uniformly in an N-ball center at origin
* @tparam   N           Template argument
* @tparam   DataT       Point data type
* @tparam   PointT      The center point type. Must be addressable by [] operator
* @tparam   PointT      The output point type. Must be addressable by [] operator
* @param    radius      The radius of the N-ball
* @param    center      The center of the N-ball
* @param    seed        The seed for the random number generator
* @param    point       The sampled point
* @param    thread_id   If multithreaded calls are used, this helps the generator 
*                       produce unique results per thread by using different seeds
**/
template <unsigned int N, typename DataT, typename CenterPointT, typename PointT> void
UniformSampleNBall(const DataT radius, const CenterPointT& center, unsigned int seed, PointT& point, unsigned int thread_id = 0)
{
    static thread_local std::mt19937 generator(seed + thread_id);
    std::uniform_real_distribution<DataT> mag_distribution(0.0, 1.0);

    //Because one generator is used, the resultant positions may not be IID
    //I don't know for sure, although the documentation provided when the
    //standard introduced <random> implies a single generator is acceptable.
    std::normal_distribution<DataT> pos_distribution(0.0, 1.0);

    DataT norm = 0;

    for (int i = 0; i < N; i++)
    {
        point[i] = pos_distribution(generator);
        norm += pow(point[i], 2);
    }
    norm = sqrt(norm);

    DataT mag = n_root<N, float>::compute(mag_distribution(generator)) * radius
                / norm;


    for (int i = 0; i < N; i++)
    {
        point[i] = point[i] * mag + center[i];
    }
}
}