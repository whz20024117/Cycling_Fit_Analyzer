#include<iostream>
#include<fstream>
#include<numeric>
#include<stdexcept>

#include "my_listener.hpp"
#include "../lib/fitsdk/fit_decode.hpp"
#include "../lib/fitsdk/fit_mesg_broadcaster.hpp"

std::vector<std::vector<float>> power_curve(std::vector<FIT_UINT32>& timestamp_vec, std::vector<float>& power_vec)
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

int main(int argc, char* argv[])
{
    
    if (argc != 3)
    {
        printf("Usage: beta <fit_file> <out_dir>\n");
        return -1;
    }

    std::fstream file;

    file.open(argv[1], std::ios::in | std::ios::binary);
    
    if (!file.is_open())
    {
        printf("Error opening fit file %s\n", argv[1]);
        return -1;
    }

    fit::Decode decode;

    if (!decode.CheckIntegrity(file))
    {
        printf("FIT file integrity failed.\nAttempting to decode...\n");
    }

    fit::MesgBroadcaster mesgBroadcaster;
    Listener listener(argv[2]);
    
    mesgBroadcaster.AddListener((fit::MesgListener&) listener);
    mesgBroadcaster.AddListener((fit::RecordMesgListener&) listener);
    mesgBroadcaster.AddListener((fit::LapMesgListener&) listener);
    mesgBroadcaster.AddListener((fit::SessionMesgListener&) listener);
    

    try
    {
        printf("Start Reading FIT file...\n");
        decode.Read(file, mesgBroadcaster);
    }
    catch (const fit::RuntimeException& e)
    {
        printf("Exception decoding file: %s\n", e.what());
        return -1;
    }

    // Stdout Session summary(s)
    std::wcout.setf(std::ios::fixed);
    std::wcout.precision(2);
    std::wcout<<L"\nFinish reading and exporting.\n\n";
    std::wcout<<L"\nSession summary(s): \n";
    const std::vector<sessionSummary> summaries = listener.getSummaries();
    for(int i=0; i<summaries.size(); i++)
    {
        sessionSummary summary = summaries[i];
        std::wcout<<L"  Session "<< i+1 <<L": \n";
        std::wcout
        <<L"      Total Elapsed Time: "<<summary.elapsedtime/3600<<L" hours\n"
        << L"      Total Distance: "<<summary.distance/1000<<L" km\n"
        << L"      Total Calories: "<<summary.calories<<L" kcal\n"
        << L"      Average Speed: "<<summary.avgspeed*3.6<<L" km/h\n"
        << L"      Max Speed: "<<summary.maxspeed*3.6<<L" km/h\n"
        << L"      Average Power: "<<summary.avgpower<<L" watts\n"
        << L"      Max Power: "<<summary.maxpower<<L" watts\n"
        << L"      Total Ascent: "<<summary.ascent<<L" m\n"
        << L"      Total Descent: "<<summary.descent<<L" m\n"
        << L"      Average Heartrate: "<<summary.avgheartrate<<L" bpm\n"
        << L"      Max Heartrate: "<<summary.maxheartrate<<L" bpm\n"
        << L"      Average Cadence: "<<summary.avgcadence<<L" rpm\n"
        << L"      Max Cadence: "<<summary.maxcadence<<L" rpm\n"
        << L"\n"
        ;
    }
    // Generate Power curve
    std::wcout<<L"\nGenerating Critical Power...\n";
    try
    {
        std::vector<std::vector<float>> power_curve_data = power_curve(listener.timestamp_vec, listener.power_vec);
        std::vector<float> cpc_tf = power_curve_data[0];
        std::vector<float> cpc_cp = power_curve_data[1];
        std::vector<float> cpc_st = power_curve_data[2];

        // stdout
        for(int i=0; i<cpc_tf.size(); i++)
        {
        std::wcout
        << L"   Timeframe(s): "<< cpc_tf[i] <<"\n"
        << L"   Critical_Power(watts): " << cpc_cp[i] <<"\n"
        << L"   When(s): " << cpc_st[i] <<"\n"
        <<"\n"
        ;
        }

        // Write to File
        std::wofstream powerCurve;
        std::string save_dir = argv[2];
        
        powerCurve.open((save_dir + "/cpcurve.csv").c_str());

        powerCurve
        << "Timeframe(s)," 
        << "Critical_Power(watts)," 
        << "When(s)\n";

        for(int i=0; i<cpc_tf.size(); i++)
        {
            powerCurve
            <<cpc_tf[i]<<","
            <<cpc_cp[i]<<","
            <<cpc_st[i]<<"\n";
        }

        powerCurve.close();
        }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }
    
    
    

}
