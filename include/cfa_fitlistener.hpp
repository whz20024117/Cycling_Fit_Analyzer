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
        public:
            std::vector<FIT_UINT32> timestamp_vec;
            std::vector<FIT_FLOAT32> altitude_vec;
            std::vector<FIT_FLOAT32> power_vec;
            std::vector<FIT_FLOAT32> heartrate_vec;
            std::vector<FIT_FLOAT32> cadence_vec;
            std::vector<FIT_FLOAT32> speed_vec;

            explicit FitListener(std::wofstream& recordfile, std::wofstream& sessionfile, std::wofstream& lapfile);

            void OnMesg(fit::Mesg& mesg) override;
            void OnMesg(fit::RecordMesg& mesg) override;
            void OnMesg(fit::LapMesg& mesg) override;
            void OnMesg(fit::SessionMesg& mesg) override;
            static void PrintValues(const fit::FieldBase& field);
            const std::vector<sessionSummary> getSummaries();

        private:
            // Restrict copying
            FitListener(const FitListener&);
            FitListener& operator=(const FitListener&);
            
            std::wofstream& recordfile;
            std::wofstream& sessionfile;
            std::wofstream& lapfile;

            std::vector<sessionSummary> summaries;

    };

}