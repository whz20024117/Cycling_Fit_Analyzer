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
    if (mesg.GetName() == "record"){return;}
    if (mesg.GetName() == "device_info"){return;}
    if (mesg.GetName() == "lap"){return;}

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

void Listener::OnMesg(fit::LapMesg& mesg){}
void Listener::OnMesg(fit::SessionMesg& mesg)
{

}