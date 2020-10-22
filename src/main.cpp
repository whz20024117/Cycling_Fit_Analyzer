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
        decode.Read(file, mesgBroadcaster);
    }
    catch (const fit::RuntimeException& e)
    {
        printf("Exception decoding file: %s\n", e.what());
        return -1;
    }


}