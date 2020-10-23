#include <iostream>
#include <numeric>
#include <stdexcept>
#include <vector>

#include "fitsdk/fit.hpp"
#include "cfa_analysis.hpp"

namespace cfa
{

    std::vector<std::vector<float>> getPowerCurveVector(std::vector<FIT_UINT32>& timestamp_vec, std::vector<float>& power_vec)
    {
        if(!(timestamp_vec.size() == power_vec.size()))
        {
            throw std::runtime_error("    Skipping CP generation on error: Powervec and timestamp_vec not consistent.");
        }
        
        for (int i=0; i<power_vec.size(); i++)
        {
            if (power_vec[i] < 0){
                throw std::runtime_error("    Skipping CP generation on error: Invalid or missing power records.");
            }
        }

        int duration = timestamp_vec[timestamp_vec.size() - 1] - timestamp_vec[0];
        
        std::vector<std::vector<float>> ret;
        std::vector<float> timeframe;
        std::vector<float> criticalpower;
        std::vector<float> stamp;

        // Candidate timeframes
        std::vector<float> tframe;

        tframe = {3.0, 5.0, 10.0, 20.0, 30.0, 45.0, 60.0,   // second-based
                    3*60.0, 5*60.0, 10*60.0, 15*60.0, 20*60.0, 30*60.0, 45*60.0, 60*60.0};
        
        // Main Loop
        for (int i=0; i<tframe.size(); i++)
        {
            // Timeframe should not be larger than total duration.
            int tf = static_cast<int>(tframe[i]);
            if (tf > duration){continue;}
            
            // Find critical power
            float cp = -1.0;    // Criticall Power
            float st_cp = 0.0;  // Timestamp where critical power occurs
            int st = 0;         // Timestamp variable to control do loop
            float tmp_cp = -1.0;    // Temp variable to swap timestamp
            
            do
            {
                float tmp_cp = std::accumulate(power_vec.begin()+st, power_vec.begin()+st+tf, 0.0);
                tmp_cp /= tf;
                
                if(tmp_cp > cp)
                {
                    cp = tmp_cp;
                    st_cp = static_cast<float>(st);
                }
                st++;

            } while (st < duration - tf);
            
            timeframe.emplace_back(tframe[i]);
            criticalpower.emplace_back(cp);
            stamp.emplace_back(st_cp);
        }
        
        ret.emplace_back(timeframe);
        ret.emplace_back(criticalpower);
        ret.emplace_back(stamp);

        return ret;
    }

}
