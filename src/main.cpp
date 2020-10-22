#include<iostream>
#include<fstream>

#include "my_listener.hpp"
#include "../lib/fitsdk/fit_decode.hpp"
#include "../lib/fitsdk/fit_mesg_broadcaster.hpp"

int main(int argc, char* argv[])
{
    fit::Decode decode;
   
    fit::MesgBroadcaster mesgBroadcaster;
    Listener listener(argv[2]);
    std::fstream file;


    if (argc != 3)
    {
        printf("Usage: beta <fit_file> <outputfile>\n");
        return -1;
    }

    file.open(argv[1], std::ios::in | std::ios::binary);
    
    if (!file.is_open())
    {
        printf("Error opening file %s\n", argv[1]);
        return -1;
    }

    if (!decode.CheckIntegrity(file))
    {
        printf("FIT file integrity failed.\nAttempting to decode...\n");
    }

    mesgBroadcaster.AddListener((fit::MesgListener &) listener);
    mesgBroadcaster.AddListener((fit::RecordMesgListener&) listener);
    mesgBroadcaster.AddListener((fit::FileIdMesgListener &)listener);
    mesgBroadcaster.AddListener((fit::UserProfileMesgListener &)listener);
    mesgBroadcaster.AddListener((fit::MonitoringMesgListener &)listener);
    mesgBroadcaster.AddListener((fit::DeviceInfoMesgListener &)listener);
    

    try
    {
        decode.Read(file, mesgBroadcaster);
    }
    catch (const fit::RuntimeException& e)
    {
        printf("Exception decoding file: %s\n", e.what());
        return -1;
    }


}