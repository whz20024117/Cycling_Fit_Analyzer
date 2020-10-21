#include <fstream>
#include <iostream>

#include "fit_decode.hpp"
#include "fit_mesg_broadcaster.hpp"
#include "fit_developer_field_description.hpp"
#include "my_listener.hpp"


void Listener::OnMesg(fit::RecordMesg& mesg)
{
    // get all the interested fields
    fit::Field *field_timestamp = mesg.GetField(fit::RecordMesg::FieldDefNum::Timestamp);
    fit::Field *field_altitude = mesg.GetField(fit::RecordMesg::FieldDefNum::Altitude);
    fit::Field *field_power = mesg.GetField(fit::RecordMesg::FieldDefNum::Power);
    fit::Field *field_heartrate = mesg.GetField(fit::RecordMesg::FieldDefNum::HeartRate);
    fit::Field *field_cadence = mesg.GetField(fit::RecordMesg::FieldDefNum::Cadence);
    fit::Field *field_speed = mesg.GetField(fit::RecordMesg::FieldDefNum::Speed);

    // outfile << field_timestamp->GetName().c_str() << "(" << field_timestamp->GetUnits().c_str() << "),";
    // outfile << field_power->GetName().c_str() << "(" << field_power->GetUnits().c_str() << "),";
    // outfile << field_accumulatedpower->GetName().c_str() << "(" << field_accumulatedpower->GetUnits().c_str() << "),";
    // outfile << field_heartrate->GetName().c_str() << "(" << field_heartrate->GetUnits().c_str() << "),";
    // outfile << field_cadence->GetName().c_str() << "(" << field_cadence->GetUnits().c_str() << "),";
    // outfile << field_speed->GetName().c_str() << "(" << field_speed->GetUnits().c_str() << "),";
    // outfile << field_distance->GetName().c_str() << "(" << field_distance->GetUnits().c_str() << "),";

    // run ./Activity.fit ./beta.csv
};

void Listener::OnMesg(fit::Mesg& mesg)
{   
    //if (mesg.GetName() == "record"){return;}
    if (mesg.GetName() == "device_info"){return;}

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

void Listener::OnMesg(fit::FileIdMesg& mesg){}
void Listener::OnMesg(fit::UserProfileMesg& mesg){}
void Listener::OnMesg(fit::DeviceInfoMesg& mesg){}
void Listener::OnMesg(fit::MonitoringMesg& mesg){}
void Listener::OnDeveloperFieldDescription( const fit::DeveloperFieldDescription& desc )
{
    printf( "New Developer Field Description\n" );
    printf( "   App Version: %d\n", desc.GetApplicationVersion() );
    printf( "   Field Number: %d\n", desc.GetFieldDefinitionNumber() );
}