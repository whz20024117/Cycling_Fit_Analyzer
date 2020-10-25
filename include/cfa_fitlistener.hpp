#include <fstream>
#include <iostream>
#include <string>

#include "fitsdk/fit_decode.hpp"
#include "fitsdk/fit_mesg_broadcaster.hpp"

namespace cfa
{

    struct sessionSummary
    {
        float avgpower;
        float maxpower;
        float distance;
        float avgcadence;
        float maxcadence;
        float calories;
        float elapsedtime;
        float avgspeed;
        float maxspeed;
        float avgheartrate;
        float maxheartrate;
        float ascent;
        float descent;
    };

    class FitListener
        : 
        public fit::MesgListener
        , public fit::RecordMesgListener
        , public fit::LapMesgListener
        , public fit::SessionMesgListener
    {
        std::wofstream recordfile;
        std::wofstream sessionfile;
        std::wofstream lapfile;

        std::vector<sessionSummary> summaries;
        
        public:
            std::vector<FIT_UINT32> timestamp_vec;
            std::vector<FIT_FLOAT32> altitude_vec;
            std::vector<FIT_FLOAT32> power_vec;
            std::vector<FIT_FLOAT32> heartrate_vec;
            std::vector<FIT_FLOAT32> cadence_vec;
            std::vector<FIT_FLOAT32> speed_vec;

            explicit FitListener(std::string path)
            {
                // Prep Record File
                recordfile.open((path + "/records.csv").c_str());
                if(!recordfile.is_open()){
                    printf(" Please make sure the directory is valid and existed.\n");
                    exit(-1);
                }
                recordfile<< "timestamp(s)," 
                << "altitude(m)," 
                << "power(watts)," 
                << "heart_rate(bpm)," 
                << "cadence(rpm),"
                <<"speed(m/s)"
                <<"\n";

                // Prep Session File
                sessionfile.open((path + "/sessions.csv").c_str());
                sessionfile<< "timestamp(s),"
                << "total_elapsed_time(s),"
                << "total_distance(m),"
                << "total_calories(kcal),"
                << "avg_speed(m/s),"
                << "max_speed(m/s),"
                << "avg_power(watts),"
                << "max_power(watts),"
                << "total_ascent(m),"
                << "total_descent(m),"
                << "avg_heart_rate(bpm),"
                << "max_heart_rate(bpm),"
                << "avg_cadence(rpm),"
                << "max_cadence(rpm)"
                <<"\n";

                // Prep Lap File
                lapfile.open((path + "/lap.csv").c_str());
                lapfile<< "timestamp(s),"
                << "total_elapsed_time(s),"
                << "total_distance(m),"
                << "total_calories(kcal),"
                << "avg_speed(m/s),"
                << "max_speed(m/s),"
                << "avg_power(watts),"
                << "max_power(watts),"
                << "total_ascent(m),"
                << "total_descent(m),"
                << "avg_heart_rate(bpm),"
                << "max_heart_rate(bpm),"
                << "avg_cadence(rpm),"
                << "max_cadence(rpm)"
                <<"\n";
            }

            void OnMesg(fit::Mesg& mesg) override;
            void OnMesg(fit::RecordMesg& mesg) override;
            void OnMesg(fit::LapMesg& mesg) override;
            void OnMesg(fit::SessionMesg& mesg) override;
            static void PrintValues(const fit::FieldBase& field);
            const std::vector<sessionSummary> getSummaries();

        private:
            FitListener(const FitListener&);
            FitListener& operator=(const FitListener&);

    };

}