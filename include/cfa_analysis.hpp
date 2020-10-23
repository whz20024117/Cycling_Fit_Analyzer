#include<iostream>
#include<fstream>
#include<numeric>
#include<stdexcept>
#include<vector>

#include "fitsdk/fit.hpp"


namespace cfa
{

    std::vector<std::vector<float>> getPowerCurveVector(std::vector<FIT_UINT32>& timestamp_vec, std::vector<float>& power_vec);


}
