#include <fstream>
#include <iostream>
#include <string>

#include "fit_decode.hpp"
#include "fit_mesg_broadcaster.hpp"
#include "fit_developer_field_description.hpp"

class Listener
    : 
    public fit::FileIdMesgListener
    , public fit::UserProfileMesgListener
    , public fit::MonitoringMesgListener
    , public fit::DeviceInfoMesgListener
    , public fit::MesgListener
    , public fit::DeveloperFieldDescriptionListener
    , public fit::RecordMesgListener
{
    std::ofstream outfile;
    
    public:
        Listener(std::string path)
        {
            outfile.open(path.c_str());
            outfile<< "timestamp(s)," << "power(watts)," << "heart_rate(bpm)," << "cadence(rmp),"<<"speed(m/s)";
        }
        ~Listener(){outfile.close();}
        void OnMesg(fit::Mesg& mesg) override;
        void OnMesg(fit::RecordMesg& mesg) override;
        void OnMesg(fit::FileIdMesg& mesg) override;
        void OnMesg(fit::UserProfileMesg& mesg) override;
        void OnMesg(fit::DeviceInfoMesg& mesg) override;
        void OnMesg(fit::MonitoringMesg& mesg) override;
        void OnDeveloperFieldDescription( const fit::DeveloperFieldDescription& desc ) override;
        static void PrintValues(const fit::FieldBase& field);

};