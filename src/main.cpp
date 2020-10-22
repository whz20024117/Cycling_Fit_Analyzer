#include<iostream>
#include<fstream>

#include "my_listener.hpp"
#include "../lib/fitsdk/fit_decode.hpp"
#include "../lib/fitsdk/fit_mesg_broadcaster.hpp"

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

}
