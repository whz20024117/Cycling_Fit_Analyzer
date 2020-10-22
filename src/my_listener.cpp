#include <fstream>
#include <iostream>
#include <string>

#include "../lib/fitsdk/fit_decode.hpp"
#include "../lib/fitsdk/fit_mesg_broadcaster.hpp"
#include "../lib/fitsdk/fit_developer_field_description.hpp"
#include "my_listener.hpp"

#define NULL_HOLDER -100.0


void Listener::OnMesg(fit::RecordMesg& mesg)
{
    // get all the interested fields from Record Message
    fit::Field *field_timestamp = mesg.GetField(fit::RecordMesg::FieldDefNum::Timestamp);
    fit::Field *field_altitude = mesg.GetField(fit::RecordMesg::FieldDefNum::Altitude);
    fit::Field *field_power = mesg.GetField(fit::RecordMesg::FieldDefNum::Power);
    fit::Field *field_heartrate = mesg.GetField(fit::RecordMesg::FieldDefNum::HeartRate);
    fit::Field *field_cadence = mesg.GetField(fit::RecordMesg::FieldDefNum::Cadence);
    fit::Field *field_speed = mesg.GetField(fit::RecordMesg::FieldDefNum::Speed);

    // To CSV record
    if (FIT_NULL != field_timestamp){recordfile << field_timestamp->GetSTRINGValue() << ",";} else {recordfile << "null,";}
    if (FIT_NULL != field_altitude){recordfile << field_altitude->GetSTRINGValue() << ",";} else {recordfile << "null,";}
    if (FIT_NULL != field_power){recordfile << field_power->GetSTRINGValue() << ",";} else {recordfile << "null,";}
    if (FIT_NULL != field_heartrate){recordfile << field_heartrate->GetSTRINGValue() << ",";} else {recordfile << "null,";}
    if (FIT_NULL != field_cadence){recordfile << field_cadence->GetSTRINGValue() << ",";} else {recordfile << "null,";}
    if (FIT_NULL != field_speed){recordfile << field_speed->GetSTRINGValue() << "\n";} else {recordfile << "null\n";}

    // Save in vector for basic analysis
    timestamp_vec.emplace_back(field_timestamp->GetUINT32Value());

    if (FIT_NULL != field_altitude)
    {altitude_vec.emplace_back(field_altitude->GetFLOAT32Value());} else {altitude_vec.emplace_back(NULL_HOLDER);}

    if (FIT_NULL != field_power)
    {power_vec.emplace_back(field_power->GetFLOAT32Value());} else {power_vec.emplace_back(NULL_HOLDER);}

    if (FIT_NULL != field_heartrate)
    {heartrate_vec.emplace_back(field_heartrate->GetFLOAT32Value());} else {heartrate_vec.emplace_back(NULL_HOLDER);}

    if (FIT_NULL != field_cadence)
    {cadence_vec.emplace_back(field_cadence->GetFLOAT32Value());} else {cadence_vec.emplace_back(NULL_HOLDER);}

    if (FIT_NULL != field_speed)
    {speed_vec.emplace_back(field_speed->GetFLOAT32Value());} else {speed_vec.emplace_back(NULL_HOLDER);}

    // run ./Activity.fit ./beta.csv
};

void Listener::OnMesg(fit::Mesg& mesg)
{   
    // Debug Function for all mesg
    if (mesg.GetName() == "record"){return;}
    if (mesg.GetName() == "device_info"){return;}
    if (mesg.GetName() == "lap"){return;}
    if (mesg.GetName() == "event"){return;}
    if (mesg.GetName() == "activity"){return;}
    if (mesg.GetName() == "session"){return;}
    if (mesg.GetName() == "file_id"){return;}

    printf("On Mesg:\n");
    std::wcout << L"   New Mesg: " << mesg.GetName().c_str() << L".  It has " << mesg.GetNumFields() << L" field(s) and " << mesg.GetNumDevFields() << " developer field(s).\n";

    for (FIT_UINT16 i = 0; i < (FIT_UINT16)mesg.GetNumFields(); i++)
    {
        fit::Field* field = mesg.GetFieldByIndex(i);
        std::wcout << L"   Field" << i << " (" << field->GetName().c_str() << ") has " << field->GetNumValues() << L" value(s)\n";
        PrintValues(*field);
    }

    for (auto devField : mesg.GetDeveloperFields())
    {
        std::wcout << L"   Developer Field(" << devField.GetName().c_str() << ") has " << devField.GetNumValues() << L" value(s)\n";
        PrintValues(devField);
    }
}

void Listener::PrintValues(const fit::FieldBase& field)
{
    // This is a debug function from fitsdk library
        for (FIT_UINT8 j=0; j< (FIT_UINT8)field.GetNumValues(); j++)
        {
            std::wcout << L"       Val" << j << L": ";
            switch (field.GetType())
            {
            // Get float 64 values for numeric types to receive values that have
            // their scale and offset properly applied.
            case FIT_BASE_TYPE_ENUM:
            case FIT_BASE_TYPE_BYTE:
            case FIT_BASE_TYPE_SINT8:
            case FIT_BASE_TYPE_UINT8:
            case FIT_BASE_TYPE_SINT16:
            case FIT_BASE_TYPE_UINT16:
            case FIT_BASE_TYPE_SINT32:
            case FIT_BASE_TYPE_UINT32:
            case FIT_BASE_TYPE_SINT64:
            case FIT_BASE_TYPE_UINT64:
            case FIT_BASE_TYPE_UINT8Z:
            case FIT_BASE_TYPE_UINT16Z:
            case FIT_BASE_TYPE_UINT32Z:
            case FIT_BASE_TYPE_UINT64Z:
            case FIT_BASE_TYPE_FLOAT32:
            case FIT_BASE_TYPE_FLOAT64:
                std::wcout << field.GetFLOAT64Value(j);
                break;
            case FIT_BASE_TYPE_STRING:
                std::wcout << field.GetSTRINGValue(j);
                break;
            default:
                break;
            }
            std::wcout << L" " << field.GetUnits().c_str() << L"\n";;
        }
}

void Listener::OnMesg(fit::LapMesg& mesg)
{
    fit::Field *field_timestamp = mesg.GetField(fit::LapMesg::FieldDefNum::Timestamp);
    fit::Field *field_totalelapsedtime = mesg.GetField(fit::LapMesg::FieldDefNum::TotalElapsedTime);
    fit::Field *field_totaldistance = mesg.GetField(fit::LapMesg::FieldDefNum::TotalDistance);
    fit::Field *field_totalcalories = mesg.GetField(fit::LapMesg::FieldDefNum::TotalCalories);
    fit::Field *field_avgspeed = mesg.GetField(fit::LapMesg::FieldDefNum::AvgSpeed);
    fit::Field *field_maxspeed = mesg.GetField(fit::LapMesg::FieldDefNum::MaxSpeed);
    fit::Field *field_avgpower = mesg.GetField(fit::LapMesg::FieldDefNum::AvgPower);
    fit::Field *field_maxpower = mesg.GetField(fit::LapMesg::FieldDefNum::MaxPower);
    fit::Field *field_totalascent = mesg.GetField(fit::LapMesg::FieldDefNum::TotalAscent);
    fit::Field *field_totaldescent = mesg.GetField(fit::LapMesg::FieldDefNum::TotalDescent);
    fit::Field *field_avgheartrate = mesg.GetField(fit::LapMesg::FieldDefNum::AvgHeartRate);
    fit::Field *field_maxheartrate = mesg.GetField(fit::LapMesg::FieldDefNum::MaxHeartRate);
    fit::Field *field_avgcadence = mesg.GetField(fit::LapMesg::FieldDefNum::AvgCadence);
    fit::Field *field_maxcadence = mesg.GetField(fit::LapMesg::FieldDefNum::MaxCadence);

    if (FIT_NULL != field_timestamp){lapfile << field_timestamp->GetSTRINGValue() << ",";} else {lapfile << "null,";}
    if (FIT_NULL != field_totalelapsedtime){lapfile << field_totalelapsedtime->GetSTRINGValue() << ",";} else {lapfile << "null,";}
    if (FIT_NULL != field_totaldistance){lapfile << field_totaldistance->GetSTRINGValue() << ",";} else {lapfile << "null,";}
    if (FIT_NULL != field_totalcalories){lapfile << field_totalcalories->GetSTRINGValue() << ",";} else {lapfile << "null,";}
    if (FIT_NULL != field_avgspeed){lapfile << field_avgspeed->GetSTRINGValue() << ",";} else {lapfile << "null,";}
    if (FIT_NULL != field_maxspeed){lapfile << field_maxspeed->GetSTRINGValue() << ",";} else {lapfile << "null,";}
    if (FIT_NULL != field_avgpower){lapfile << field_avgpower->GetSTRINGValue() << ",";} else {lapfile << "null,";}
    if (FIT_NULL != field_maxpower){lapfile << field_maxpower->GetSTRINGValue() << ",";} else {lapfile << "null,";}
    if (FIT_NULL != field_totalascent){lapfile << field_totalascent->GetSTRINGValue() << ",";} else {lapfile << "null,";}
    if (FIT_NULL != field_totaldescent){lapfile << field_totaldescent->GetSTRINGValue() << ",";} else {lapfile << "null,";}
    if (FIT_NULL != field_avgheartrate){lapfile << field_avgheartrate->GetSTRINGValue() << ",";} else {lapfile << "null,";}
    if (FIT_NULL != field_maxheartrate){lapfile << field_maxheartrate->GetSTRINGValue() << ",";} else {lapfile << "null,";}
    if (FIT_NULL != field_avgcadence){lapfile << field_avgcadence->GetSTRINGValue() << ",";} else {lapfile << "null,";}
    if (FIT_NULL != field_maxcadence){lapfile << field_maxcadence->GetSTRINGValue() << "\n";} else {lapfile << "null\n";}
}

void Listener::OnMesg(fit::SessionMesg& mesg)
{
    fit::Field *field_timestamp = mesg.GetField(fit::SessionMesg::FieldDefNum::Timestamp);
    fit::Field *field_totalelapsedtime = mesg.GetField(fit::SessionMesg::FieldDefNum::TotalElapsedTime);
    fit::Field *field_totaldistance = mesg.GetField(fit::SessionMesg::FieldDefNum::TotalDistance);
    fit::Field *field_totalcalories = mesg.GetField(fit::SessionMesg::FieldDefNum::TotalCalories);
    fit::Field *field_avgspeed = mesg.GetField(fit::SessionMesg::FieldDefNum::AvgSpeed);
    fit::Field *field_maxspeed = mesg.GetField(fit::SessionMesg::FieldDefNum::MaxSpeed);
    fit::Field *field_avgpower = mesg.GetField(fit::SessionMesg::FieldDefNum::AvgPower);
    fit::Field *field_maxpower = mesg.GetField(fit::SessionMesg::FieldDefNum::MaxPower);
    fit::Field *field_totalascent = mesg.GetField(fit::SessionMesg::FieldDefNum::TotalAscent);
    fit::Field *field_totaldescent = mesg.GetField(fit::SessionMesg::FieldDefNum::TotalDescent);
    fit::Field *field_avgheartrate = mesg.GetField(fit::SessionMesg::FieldDefNum::AvgHeartRate);
    fit::Field *field_maxheartrate = mesg.GetField(fit::SessionMesg::FieldDefNum::MaxHeartRate);
    fit::Field *field_avgcadence = mesg.GetField(fit::SessionMesg::FieldDefNum::AvgCadence);
    fit::Field *field_maxcadence = mesg.GetField(fit::SessionMesg::FieldDefNum::MaxCadence);

    if (FIT_NULL != field_timestamp){sessionfile << field_timestamp->GetSTRINGValue() << ",";} else {sessionfile << "null,";}
    if (FIT_NULL != field_totalelapsedtime){sessionfile << field_totalelapsedtime->GetSTRINGValue() << ",";} else {sessionfile << "null,";}
    if (FIT_NULL != field_totaldistance){sessionfile << field_totaldistance->GetSTRINGValue() << ",";} else {sessionfile << "null,";}
    if (FIT_NULL != field_totalcalories){sessionfile << field_totalcalories->GetSTRINGValue() << ",";} else {sessionfile << "null,";}
    if (FIT_NULL != field_avgspeed){sessionfile << field_avgspeed->GetSTRINGValue() << ",";} else {sessionfile << "null,";}
    if (FIT_NULL != field_maxspeed){sessionfile << field_maxspeed->GetSTRINGValue() << ",";} else {sessionfile << "null,";}
    if (FIT_NULL != field_avgpower){sessionfile << field_avgpower->GetSTRINGValue() << ",";} else {sessionfile << "null,";}
    if (FIT_NULL != field_maxpower){sessionfile << field_maxpower->GetSTRINGValue() << ",";} else {sessionfile << "null,";}
    if (FIT_NULL != field_totalascent){sessionfile << field_totalascent->GetSTRINGValue() << ",";} else {sessionfile << "null,";}
    if (FIT_NULL != field_totaldescent){sessionfile << field_totaldescent->GetSTRINGValue() << ",";} else {sessionfile << "null,";}
    if (FIT_NULL != field_avgheartrate){sessionfile << field_avgheartrate->GetSTRINGValue() << ",";} else {sessionfile << "null,";}
    if (FIT_NULL != field_maxheartrate){sessionfile << field_maxheartrate->GetSTRINGValue() << ",";} else {sessionfile << "null,";}
    if (FIT_NULL != field_avgcadence){sessionfile << field_avgcadence->GetSTRINGValue() << ",";} else {sessionfile << "null,";}
    if (FIT_NULL != field_maxcadence){sessionfile << field_maxcadence->GetSTRINGValue() << "\n";} else {sessionfile << "null\n";}

    sessionSummary summary;
    
    if (FIT_NULL != field_totalelapsedtime)
    {summary.elapsedtime=field_totalelapsedtime->GetFLOAT32Value();} else {summary.elapsedtime=NULL_HOLDER;}
    if (FIT_NULL != field_totaldistance)
    {summary.distance=field_totaldistance->GetFLOAT32Value();} else {summary.distance=NULL_HOLDER;}
    if (FIT_NULL != field_totalcalories)
    {summary.calories=field_totalcalories->GetFLOAT32Value();} else {summary.calories=NULL_HOLDER;}
    if (FIT_NULL != field_avgspeed)
    {summary.avgspeed=field_avgspeed->GetFLOAT32Value();} else {summary.avgspeed=NULL_HOLDER;}
    if (FIT_NULL != field_maxspeed)
    {summary.maxspeed=field_maxspeed->GetFLOAT32Value();} else {summary.maxspeed=NULL_HOLDER;}
    if (FIT_NULL != field_avgpower)
    {summary.avgpower=field_avgpower->GetFLOAT32Value();} else {summary.avgpower=NULL_HOLDER;}
    if (FIT_NULL != field_maxpower)
    {summary.maxpower=field_maxpower->GetFLOAT32Value();} else {summary.maxpower=NULL_HOLDER;}
    if (FIT_NULL != field_totalascent)
    {summary.ascent=field_totalascent->GetFLOAT32Value();} else {summary.ascent=NULL_HOLDER;}
    if (FIT_NULL != field_totaldescent)
    {summary.descent=field_totaldescent->GetFLOAT32Value();} else {summary.descent=NULL_HOLDER;}
    if (FIT_NULL != field_avgheartrate)
    {summary.avgheartrate=field_avgheartrate->GetFLOAT32Value();} else {summary.avgheartrate=NULL_HOLDER;}
    if (FIT_NULL != field_maxheartrate)
    {summary.maxheartrate=field_maxheartrate->GetFLOAT32Value();} else {summary.maxheartrate=NULL_HOLDER;}
    if (FIT_NULL != field_avgcadence)
    {summary.avgcadence=field_avgcadence->GetFLOAT32Value();} else {summary.avgcadence=NULL_HOLDER;}
    if (FIT_NULL != field_maxcadence)
    {summary.maxcadence=field_maxcadence->GetFLOAT32Value();} else {summary.maxcadence=NULL_HOLDER;}

    this->summaries.emplace_back(summary);
    
}

const std::vector<sessionSummary> Listener::getSummaries()
{
    return summaries;
}
