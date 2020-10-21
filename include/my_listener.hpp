#include <fstream>
#include <iostream>
#include <string>

#include "fit_decode.hpp"
#include "fit_mesg_broadcaster.hpp"
#include "fit_developer_field_description.hpp"

class Listener
    : 
    public fit::MesgListener
    , public fit::RecordMesgListener
    , public fit::LapMesgListener
    , public fit::SessionMesgListener
{
    std::wofstream outfile;
    
    public:
        Listener(std::string path)
        {
            outfile.open(path.c_str());
            outfile<< "timestamp(s)," 
            << "altitude(m)," 
            << "power(watts)," 
            << "heart_rate(bpm)," 
            << "cadence(rpm),"
            <<"speed(m/s)"
            <<"\n";
        }
        ~Listener(){outfile.close();}
        void OnMesg(fit::Mesg& mesg) override;
        void OnMesg(fit::RecordMesg& mesg) override;
        void OnMesg(fit::LapMesg& mesg) override;
        void OnMesg(fit::SessionMesg& mesg) override;
        static void PrintValues(const fit::FieldBase& field);

};