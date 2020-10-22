#include <fstream>
#include <iostream>
#include <string>

#include "../lib/fitsdk/fit_decode.hpp"
#include "../lib/fitsdk/fit_mesg_broadcaster.hpp"
#include "../lib/fitsdk/fit_developer_field_description.hpp"

class Listener
    : 
    public fit::MesgListener
    , public fit::RecordMesgListener
    , public fit::LapMesgListener
    , public fit::SessionMesgListener
{
    std::wofstream recordfile;
    std::wofstream sessionfile;
    std::vector<FIT_UINT32> timestamp_vec;
    std::vector<FIT_FLOAT32> altitude_vec;
    std::vector<FIT_FLOAT32> power_vec;
    std::vector<FIT_FLOAT32> heartrate_vec;
    std::vector<FIT_FLOAT32> cadence_vec;
    std::vector<FIT_FLOAT32> speed_vec;
    
    public:
        Listener(std::string path)
        {
            recordfile.open(path.c_str());
            recordfile<< "timestamp(s)," 
            << "altitude(m)," 
            << "power(watts)," 
            << "heart_rate(bpm)," 
            << "cadence(rpm),"
            <<"speed(m/s)"
            <<"\n";


        }
        ~Listener()
        {
            recordfile.close();
        }
        void OnMesg(fit::Mesg& mesg) override;
        void OnMesg(fit::RecordMesg& mesg) override;
        void OnMesg(fit::LapMesg& mesg) override;
        void OnMesg(fit::SessionMesg& mesg) override;
        static void PrintValues(const fit::FieldBase& field);

};