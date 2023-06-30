#include "../include/SocketClient.h"
#include "../include/AvaObj.h"
#include "../include/definitions.h"
#include <iostream>
#include <fstream>
#include <string>
#include <thread>
#include <sstream>
#include <vector>
#include <sstream>
#include <cstdint>
#include <iomanip>
#include <algorithm>

using namespace std;

/// @todo Konstruktor soll die membervariablen uA ipRemoteTarget initialisieren
SocketClient::SocketClient() : UDPSocket() 
{
	///////////////////////////get sockaddr_in m_remoteIP from std::string m_remoteIPtarget

	memset(buffer, 0, sizeof(buffer)); //clear the buffer

}

void SocketClient::sendBytes(vector<std::byte> bytes)
{
     ///@todo prüfen ob das funktioniert
    size_t len = bytes.size();
    char* charBytes = reinterpret_cast<char*>(bytes.data()); 
    //SendTo(m_remoteIPtarget, m_port, charBytes, len);///////////////////////////////////////////////////////////// use of old SendTo function, not compatible anymore
}

void SocketClient::SendWaveAssociationRequest()
{
    sendBytes(aarq_msg_wave_ext_poll2);
}

void SocketClient::ProcessPacket(char* buffer)
{
    //size_t session_head_size = sizeof(4bytes)
    size_t session_head_size = uInt16Size*2;
    size_t ROapdu_type_size = uInt16Size;
	auto ROapdu_type = htons(ReadByteValuesFromBuffer(buffer ,session_head_size, ROapdu_type_size));
    switch (ROapdu_type)
            {
                case ROIV_APDU:
                    // This is an MDS create event, answer with create response
                    ParseMDSCreateEventReport(buffer);
                    SendMDSCreateEventResult();
                    break;
                case RORS_APDU:
                    CheckPollPacketActionType(buffer);
                    break;
                case RORLS_APDU:
                    CheckLinkedPollPacketActionType(buffer);
                    break;
                case ROER_APDU:
                    break;
                default:
                    break;
            }
}

void SocketClient::CheckLinkedPollPacketActionType(char* buffer)
{
            char* header = ReadBytesFromBuffer(buffer, 0, 22);
            uint16_t action_type = htons((Read16ByteValuesFromBuffer(buffer, 22)));
            m_actiontype = action_type;

            switch (action_type)
            {
                case NOM_ACT_POLL_MDIB_DATA:
                    PollPacketDecoder(buffer, 46);
                    break;
                case NOM_ACT_POLL_MDIB_DATA_EXT:
                    PollPacketDecoder(buffer, 48);
                    break;
                default:
                    break;
            }
}

void SocketClient::ParseMDSCreateEventReport(char* buffer)
{
    //size_t header_size = sizeof(34bytes)
    size_t attriblist_count_size = 2;
    size_t attriblist_length_size = 2;
    size_t header_size = 34;
    auto attriblist_count = htons(ReadByteValuesFromBuffer(buffer,header_size, attriblist_count_size));
    auto attriblist_length = htons(ReadByteValuesFromBuffer(buffer,header_size+attriblist_count_size, attriblist_length_size));
    int avaobjectscount = attriblist_count;   
     if (avaobjectscount > 0)
            {
                char* attriblistobjects_buffer = ReadBytesFromBuffer(buffer,header_size+attriblist_count_size+attriblist_length_size, attriblist_length);

                for (size_t i = 0; i < avaobjectscount; i++)
                {
                    DecodeMDSAttribObjects(move(make_unique<AvaObj>()), attriblistobjects_buffer, i);
                }
            }

}

void SocketClient::DecodeMDSAttribObjects(unique_ptr<AvaObj> avaobject, char* attriblistobjects_buffer, size_t avaObjCount)
{

    ///@todo sizeof 2 bytes per avaObjCount
    avaobject->attribute_id = htons(ReadByteValuesFromBuffer(attriblistobjects_buffer, uInt16Size*2*avaObjCount, uInt16Size)); 
    avaobject->length = htons(ReadByteValuesFromBuffer(attriblistobjects_buffer, uInt16Size*2*avaObjCount+uInt16Size, uInt16Size)); 
     //avaobject.attribute_val = htons(binreader4.ReadUInt16());
    if (avaobject->length > 0)
        {
            char* avaattribobjects_buffer = ReadBytesFromBuffer(attriblistobjects_buffer, 4*(avaObjCount+1), avaobject->length);


            switch (avaobject->attribute_id)
             {
                //Get Date and Time
                case NOM_ATTR_TIME_ABS:
                    m_baseDateTime = GetAbsoluteTimeFromBCDFormat(avaattribobjects_buffer);
                        break;
                    //Get Relative Time attribute
                    case NOM_ATTR_TIME_REL:
                        GetBaselineRelativeTimestamp(avaattribobjects_buffer);
                        break;
                    //Get Patient demographics
                    case NOM_ATTR_PT_ID:
                        break;
                    case NOM_ATTR_PT_NAME_GIVEN:
                        break;
                    case NOM_ATTR_PT_NAME_FAMILY:
                        break;
                    case NOM_ATTR_PT_DOB:
                        break;
            }
    }
}

void SocketClient::GetBaselineRelativeTimestamp(char* bcdtimebuffer)
{
    m_baseRelativeTime = htons(int(ReadByteValuesFromBuffer(bcdtimebuffer, 0, sizeof(bcdtimebuffer))));
}

tm SocketClient::GetAbsoluteTimeFromBCDFormat(char* bcdtimebuffer)
        {
            ///@todo vielleicht anderen struct als tm benutzen
            tm dateTime = m_baseDateTime;

            int century = BinaryCodedDecimalToInteger(bcdtimebuffer[0]); //century
            int year = BinaryCodedDecimalToInteger(bcdtimebuffer[1]); //year
            int month = BinaryCodedDecimalToInteger(bcdtimebuffer[2]); //month
            int day = BinaryCodedDecimalToInteger(bcdtimebuffer[3]);//day
            int hour = BinaryCodedDecimalToInteger(bcdtimebuffer[4]);//hour
            int minute = BinaryCodedDecimalToInteger(bcdtimebuffer[5]);//minute
            int second = BinaryCodedDecimalToInteger(bcdtimebuffer[6]);//second
            int fraction = BinaryCodedDecimalToInteger(bcdtimebuffer[7]); //fraction

            int formattedyear = (century * 100) + year;

            dateTime.tm_sec = second;
            dateTime.tm_min = minute;
            dateTime.tm_hour = hour;
            dateTime.tm_mday = day;
            dateTime.tm_mon = month;
            dateTime.tm_year = year;

            return dateTime;

        }
    


void SocketClient::SendMDSCreateEventResult()
{
    sendBytes(mds_create_resp_msg);
}

void SocketClient::CheckPollPacketActionType(char* buffer)
{
    size_t header_size = 20;
    uint16_t action_type = htons(ReadByteValuesFromBuffer(buffer, header_size, uInt16Size));
    m_actiontype = action_type;
    
    switch (action_type)
            {
                case NOM_ACT_POLL_MDIB_DATA:
                    PollPacketDecoder(buffer, 44);
                    break;
                case NOM_ACT_POLL_MDIB_DATA_EXT:
                    PollPacketDecoder(buffer, 46);
                    break;
                default:
                    break;
            }

}

void SocketClient::PollPacketDecoder(char* packetbuffer, size_t headersize)
    {
        size_t packetsize = sizeof(packetbuffer);

        char* packetdata = ReadBytesFromBuffer(packetbuffer, headersize, packetsize-headersize);

        ///@todo 
        //m_strTimestamp = GetPacketTimestamp(header);

        //DateTime dtDateTime = DateTime.Now;
        // uint currentRelativeTime = UInt32.Parse(m_strTimestamp);
        // DateTime dtDateTime = GetAbsoluteTimeFromRelativeTimestamp(currentRelativeTime);

        // string strDateTime = dtDateTime.ToString("dd-MM-yyyy HH:mm:ss.fff", CultureInfo.InvariantCulture);
        //Console.WriteLine("Time:{0}", strDateTime);
        //Console.WriteLine("Time:{0}", m_strTimestamp);


        //ParsePacketType

        PollInfoList* pollobjects;

        int scpollobjectscount = DecodePollObjects(pollobjects, packetdata);

        if (scpollobjectscount > 0)
            {
                for (int i = 0; i < scpollobjectscount; i++)
                {

                    SingleContextPoll* scpoll;
                    int obpollobjectscount = DecodeSingleContextPollObjects(scpoll, pollobjects->scpollarray);

                    if (obpollobjectscount > 0)
                    {
                        for (int j = 0; j < obpollobjectscount; j++)
                        {

                            ObservationPoll* obpollobject;
                            int avaobjectscount = DecodeObservationPollObjects(obpollobject, scpoll->obpollobjectsarray);

                            if (avaobjectscount > 0)
                            {

                                for (int k = 0; k < avaobjectscount; k++)
                                {
                                    DecodeAvaObjects(move(make_unique<AvaObj>()), obpollobject->avaobjectsarray);
                                }

                            }
                        }
                    }
                }
                /*
                if (m_dataexportset == 2) ExportNumValListToJSON("Numeric");
                if (m_dataexportset == 3) ExportNumValListToMQTT("Numeric");
                */
                if (m_dataexportset != 3)
                {
                    ///@todo
                    // ExportDataToCSV();
                    // ExportWaveToCSV();
                }
                //clear memory

                ///@todo
               // m_WaveValResultList.RemoveRange(0, m_WaveValResultList.Count);
            }

    }


void SocketClient::ExportDataToCSV()
        {
            switch (m_csvexportset)
            {
                case 1:
                    SaveNumericValueList();
                    break;
                    ///@todo 
                // case 2:
                //     SaveNumericValueListRows();
                //     break;
                // case 3:
                //     SaveNumericValueListConsolidatedCSV();
                //     break;
                // default:
                //     break;
            }
        }

void SocketClient::SaveNumericValueList()
        {
            if (m_NumericValList.empty() != 0)
            {
                ///@todo
                //string pathcsv = Path.Combine(Directory.GetCurrentDirectory(),Program.Globals.pathFile + "_Philips_MPDataExport.csv"); //IB

                for(auto& it : m_NumericValList)
                {
                    string exportstring = "";
                    exportstring += it.Timestamp;
                    exportstring +=',';
                    exportstring +=it.Relativetimestamp;
                    exportstring +=',';
                    exportstring +=it.SystemLocalTime;
                    exportstring +=',';
                    exportstring +=it.PhysioID;
                    exportstring +=',';
                    exportstring +=it.Value;
                    ///@todo add line
                    //m_strbuildvalues.AppendLine();
                }

                ///@todo 
                // ExportNumValListToCSVFile(pathcsv, m_strbuildvalues);
                // m_NumericValList.RemoveRange(0, m_NumericValList.Count);
            }
        }

void SocketClient::DecodeAvaObjects(unique_ptr<AvaObj> avaobject, char* buffer)
{
    avaobject->attribute_id = htons(Read16ByteValuesFromBuffer(buffer, 0));
    avaobject->length = htons(Read16ByteValuesFromBuffer(buffer, uInt16Size));
    //avaobject.attribute_val = htons(binreader4.ReadUInt16());
    if (avaobject->length > 0)
        {
            char* avaattribobjects = ReadBytesFromBuffer(buffer, 2*uInt16Size, avaobject->length);


                switch (avaobject->attribute_id)
                {
                    case NOM_ATTR_ID_HANDLE:
                        //ReadIDHandle(avaattribobjects);
                        break;
                    case NOM_ATTR_ID_LABEL:
                        ReadIDLabel(avaattribobjects);
                        break;
                    case NOM_ATTR_NU_VAL_OBS:
                        ReadNumericObservationValue(avaattribobjects);
                        break;
                    case NOM_ATTR_NU_CMPD_VAL_OBS:
                        ReadCompoundNumericObsValue(avaattribobjects);
                        break;
                    case NOM_ATTR_METRIC_SPECN:
                        break;
                    case NOM_ATTR_ID_LABEL_STRING:
                        ReadIDLabelString(avaattribobjects);
                        break;
                    case NOM_ATTR_SA_VAL_OBS:
                        ReadWaveSaObservationValueObject(avaattribobjects);
                        break;
                    case NOM_ATTR_SA_CMPD_VAL_OBS:
                        ReadCompoundWaveSaObservationValue(avaattribobjects);
                        break;
                    case NOM_ATTR_SA_SPECN:
                        ReadSaSpecifications(avaattribobjects);
                        break;
                    case NOM_ATTR_SCALE_SPECN_I16:
                        ReadSaScaleSpecifications(avaattribobjects);
                        break;
                    case NOM_ATTR_SA_CALIB_I16:
                        ReadSaCalibrationSpecifications(avaattribobjects);
                        break;
                    default:
                        // unknown attribute -> do nothing
                        break;

                }
            }
}
void SocketClient::ReadIDLabel(char* buffer)
{
    m_idlabelhandle = htons(Read16ByteValuesFromBuffer(buffer, 0));
}
void SocketClient::ReadNumericObservationValue(char* buffer)
{
    NuObsValue NumObjectValue;
    NumObjectValue.physio_id = htons(Read16ByteValuesFromBuffer(buffer, 0));
    NumObjectValue.state = htons(Read16ByteValuesFromBuffer(buffer, 0));
    NumObjectValue.unit_code = htons(Read16ByteValuesFromBuffer(buffer, 0));
    NumObjectValue.value = htonl(ReadByteValuesFromBuffer(buffer, 3*uInt16Size, 2*uInt16Size));
    double value = NumObjectValue.value;
    string physiostring_id;
    ///@todo exception handling 
    AlertSource physio_id = AlertSource(NumObjectValue.physio_id);
    if(physio_id == AlertSource::NOM_METRIC_NOS)
        {
            IDLabel cIDLabel;
            //cIDLabel = m_IDLabelList.Find(x => x.obpoll_handle == m_obpollhandle);
            auto it = std::find_if(m_IDLabelList.begin(), m_IDLabelList.end(), [&](const auto& x) 
            {
                return x.obpoll_handle == m_obpollhandle;
            });

            if (it != m_IDLabelList.end()) 
            {
                cIDLabel = *it;
                physiostring_id = cIDLabel.idlabelstring;
            }
        }

        string state = to_string(NumObjectValue.state);
        string unit_code = to_string(NumObjectValue.unit_code);

        string valuestr;
        if (!std::isnan(value))
        {
            std::ostringstream oss;
            oss << std::fixed << std::setprecision(2) << value;
            std::string valuestr = oss.str();
        }
        else valuestr = "-";

        NumericValResult NumVal;
        NumVal.Relativetimestamp = m_strTimestamp;

        ///@todo time

        //DateTime dtDateTime = DateTime.Now;
        // uint32_t currentRelativeTime = stoi(m_strTimestamp);
        // tm dtDateTime = GetAbsoluteTimeFromRelativeTimestamp(currentRelativeTime);

        //     //NumVal.Timestamp = dtDateTime.ToString();

        //     string strDateTime = dtDateTime.ToString("dd-MM-yyyy HH:mm:ss.fff", CultureInfo.InvariantCulture);
        //     //string strDateTime = dtDateTime.ToString("G", DateTimeFormatInfo.InvariantInfo);
        //     NumVal.Timestamp = strDateTime;
        //     //NumVal.Timestamp = DateTime.Now.ToString();

        //     DateTime dtSystemDateTime = DateTime.Now;

        //     string strSystemLocalDateTime = dtSystemDateTime.ToString("dd-MM-yyyy HH:mm:ss.fff", CultureInfo.InvariantCulture);
        //     NumVal.SystemLocalTime = strSystemLocalDateTime;

            NumVal.PhysioID = physiostring_id;
            NumVal.Value = valuestr;
            NumVal.DeviceID = m_DeviceID;

            m_NumericValList.push_back(NumVal);
            m_NumValHeaders.push_back(NumVal.PhysioID);

            //Console.WriteLine("Physiological ID: {0}", physio_id);
            //Console.WriteLine("State: {0}", state);
            //Console.WriteLine("Unit code: {0}", unit_code);
            //Console.WriteLine("Value: {0}", valuestr);
            //Console.WriteLine();
}
void SocketClient::ReadCompoundNumericObsValue(char* buffer)
{
    m_idlabelhandle = htons(Read16ByteValuesFromBuffer(buffer, 0));
}

void SocketClient::ReadCompoundNumericObsValue(char* buffer)
        {

            NuObsValueCmp NumObjectValueCmp;
            NumObjectValueCmp.count = htons(Read16ByteValuesFromBuffer(buffer, 0));
            NumObjectValueCmp.length = htons(Read16ByteValuesFromBuffer(buffer, uInt16Size));

            int cmpnumericobjectscount = NumObjectValueCmp.count;

            if (cmpnumericobjectscount > 0)
            {
                for (int j = 0; j < cmpnumericobjectscount; j++)
                {
                    char* cmpnumericarrayobject = ReadBytesFromBuffer(buffer, 2*uInt16Size+j*10, 10); //Read 10 Bytes every iteration

                    ReadNumericObservationValue(cmpnumericarrayobject);
                }
            }

        }

void SocketClient::ReadIDLabelString(char* buffer)
        {
            StringMP strmp;

            strmp.length = htons(Read16ByteValuesFromBuffer(buffer, 0));
            //strmp.value1 = correctendianshortus(binreader5.ReadUInt16());
            char* stringval = ReadBytesFromBuffer(buffer, uInt16Size, strmp.length);

            string label = Utf8ToString(stringval);

            m_idlabelstring = TrimString(ReplaceNullCharacters(label));
            //Console.WriteLine("Label String: {0}", m_idlabelstring);

            AddIDLabelToList();
        }

void SocketClient::AddIDLabelToList()
        {
            IDLabel cIDLabel;
            
            cIDLabel.obpoll_handle = m_obpollhandle;
            cIDLabel.idlabelhandle = m_idlabelhandle;
            cIDLabel.idlabelstring = m_idlabelstring;

            //Add to a list of ID Labels if it's not already present

            //int idlistindex = m_IDLabelList.FindIndex(x => x.obpoll_handle == cIDLabel.obpoll_handle);
            auto it = std::find_if(m_IDLabelList.begin(), m_IDLabelList.end(), [&](const IDLabel& x) {
            return x.obpoll_handle == cIDLabel.obpoll_handle;
            });

            int idlistindex = (it != m_IDLabelList.end()) ? std::distance(m_IDLabelList.begin(), it) : -1;

            if (idlistindex == -1)
            {
                m_IDLabelList.push_back(cIDLabel);
            }
            else
            {
                m_IDLabelList.erase(m_IDLabelList.begin() + idlistindex);
                m_IDLabelList.push_back(cIDLabel);
            }
        }

void SocketClient::ReadWaveSaObservationValueObject(char* avaattribobjects)
    {
        ReadWaveSaObservationValue(avaattribobjects);
    }

void SocketClient::ReadWaveSaObservationValue(char* buffer)
        {
            SaObsValue WaveSaObjectValue;
            WaveSaObjectValue.physio_id = htons(Read16ByteValuesFromBuffer(buffer, 0));
            WaveSaObjectValue.state = htons(Read16ByteValuesFromBuffer(buffer, uInt16Size));
            WaveSaObjectValue.length = htons(Read16ByteValuesFromBuffer(buffer, 2*uInt16Size));

            int wavevalobjectslength = WaveSaObjectValue.length;
            char* WaveValObjects = ReadBytesFromBuffer(buffer, 3*uInt16Size, wavevalobjectslength);

            AlertSource physio_id = AlertSource(WaveSaObjectValue.physio_id);
            string physiostring_id = "";
            if(physio_id == AlertSource::NOM_METRIC_NOS)
            {
               IDLabel cIDLabel;
               //cIDLabel = m_IDLabelList.Find(x => x.obpoll_handle == m_obpollhandle);
               auto it = std::find_if(m_IDLabelList.begin(), m_IDLabelList.end(), [&](const auto& x) 
            {
                return x.obpoll_handle == m_obpollhandle;
            });

            if (it != m_IDLabelList.end()) 
            {
                cIDLabel = *it;
                physiostring_id = cIDLabel.idlabelstring;
            }
            }

            WaveValResult WaveVal;
            WaveVal.Relativetimestamp = m_strTimestamp;

            //DateTime dtDateTime = DateTime.Now;
            ///@todo 
            // uint32_t currentRelativeTime = stoi(m_strTimestamp);
            // DateTime dtDateTime = GetAbsoluteTimeFromRelativeTimestamp(currentRelativeTime);

            // string strDateTime = dtDateTime.ToString("dd-MM-yyyy HH:mm:ss.fff", CultureInfo.InvariantCulture);
            // //WaveVal.Timestamp = DateTime.Now.ToString();

            // DateTime dtSystemDateTime = DateTime.Now;
            // string strSystemLocalDateTime = dtSystemDateTime.ToString("dd-MM-yyyy HH:mm:ss.fff", CultureInfo.InvariantCulture);
            // WaveVal.SystemLocalTime = strSystemLocalDateTime;

            // WaveVal.Timestamp = strDateTime;
            WaveVal.PhysioID = physio_id;
            WaveVal.DeviceID = m_DeviceID;

            WaveVal.obpoll_handle = m_obpollhandle;
            uint16_t physio_id_handle = WaveSaObjectValue.physio_id;

            //WaveVal.saCalibData = m_SaCalibDataSpecList.Find(x => x.obpoll_handle == m_obpollhandle); //optional
            auto it = std::find_if(m_SaCalibDataSpecList.begin(), m_SaCalibDataSpecList.end(), [&](const auto& x) 
            {
                return x.obpoll_handle == m_obpollhandle;
            });
             if (it != m_SaCalibDataSpecList.end()) 
            {
                WaveVal.saCalibData = *it;
            }
            //WaveVal.ScaleRangeSpec16 = m_ScaleRangeSpecList.Find(x => x.obpoll_handle == m_obpollhandle); //mandatory
           auto it2 = std::find_if(m_ScaleRangeSpecList.begin(), m_ScaleRangeSpecList.end(), [&](const auto& x) 
            {
                return x.obpoll_handle == m_obpollhandle;
            });
             if (it2 != m_ScaleRangeSpecList.end()) 
            {
                WaveVal.ScaleRangeSpec16  = *it2;
            }


            if (WaveVal.ScaleRangeSpec16.lower_absolute_value == -0.13371337)
            {
                if (physio_id_handle == 0x100)
                {
                    //use default values for ecg
                    WaveVal.saCalibData.lower_absolute_value = -40.96;
                    WaveVal.saCalibData.upper_absolute_value = 40.955;
                    WaveVal.saCalibData.lower_scaled_value = 0x0000;
                    WaveVal.saCalibData.upper_scaled_value = 0x3fff;
                }
                else if (physio_id_handle == 0x4A10)
                {
                    //use default values for art ibp
                    WaveVal.ScaleRangeSpec16.lower_absolute_value = -40;
                    WaveVal.ScaleRangeSpec16.upper_absolute_value = 520;
                    WaveVal.ScaleRangeSpec16.lower_scaled_value = 0x00a0;
                    WaveVal.ScaleRangeSpec16.upper_scaled_value = 0x23a0;
                }
                else if (physio_id_handle == 0x5000)
                {
                    //use default values for resp
                    WaveVal.ScaleRangeSpec16.lower_absolute_value = -0.6;
                    WaveVal.ScaleRangeSpec16.upper_absolute_value = 1.9;
                    WaveVal.ScaleRangeSpec16.lower_scaled_value = 0x0000;
                    WaveVal.ScaleRangeSpec16.upper_scaled_value = 0x0fff;
                }
                /*else if (physio_id_handle == 0xF001)
                {
                    //use default values for custom
                    WaveVal.saCalibData.lower_absolute_value = 0;
                    WaveVal.saCalibData.upper_absolute_value = 15;
                    WaveVal.saCalibData.lower_scaled_value = 0x0000;
                    WaveVal.saCalibData.upper_scaled_value = 0x0fff;
                }*/

            }

            WaveVal.Value = wavevalobjectslength;
           
            ///@todo schauen ob die beiden Zeilen äquivalten sind
             //Array.Copy(WaveValObjects, WaveVal.Value, wavevalobjectslength);
            std::copy(WaveValObjects, WaveValObjects + wavevalobjectslength, WaveVal.Value);

            //Find the Sample array specification definition that matches the observation sample array size

            //WaveVal.ScaleRangeSpec16 = m_ScaleRangeSpecList.Find(x => x.obpoll_handle == m_obpollhandle); //mandatory
           auto it3 = std::find_if(m_SaSpecList.begin(), m_SaSpecList.end(), [&](const auto& x) 
            {
                return x.obpoll_handle == m_obpollhandle;
            });
             if (it3 != m_SaSpecList.end()) 
            {
                WaveVal.saSpecData  = *it3;
            }

            if (WaveVal.saSpecData.array_size == 0)
            {
                if (wavevalobjectslength % 128 == 0)
                {
                    //use default values for ecg
                    WaveVal.saSpecData.significant_bits = static_cast<std::byte>(0x0E);
                    WaveVal.saSpecData.SaFlags = 0x3000;
                    WaveVal.saSpecData.sample_size =  static_cast<std::byte>(0x10);
                    WaveVal.saSpecData.array_size = 0x80;
                }
                else if (wavevalobjectslength % 64 == 0)
                {
                    //use default values for art ibp
                    WaveVal.saSpecData.significant_bits =  static_cast<std::byte>(0x0E);
                    WaveVal.saSpecData.SaFlags = 0x3000;
                    WaveVal.saSpecData.sample_size =  static_cast<std::byte>(0x10);
                    WaveVal.saSpecData.array_size = 0x40;

                }
                else if (wavevalobjectslength % 32 == 0)
                {
                    //use default values for resp
                    WaveVal.saSpecData.significant_bits =  static_cast<std::byte>(0x0C);
                    WaveVal.saSpecData.SaFlags = 0x8000;
                    WaveVal.saSpecData.sample_size =  static_cast<std::byte>(0x10);
                    WaveVal.saSpecData.array_size = 0x20;
                }
                else if (wavevalobjectslength % 16 == 0)
                {
                    //use default values for pleth
                    WaveVal.saSpecData.significant_bits =  static_cast<std::byte>(0x0C);
                    WaveVal.saSpecData.SaFlags = 0x8000;
                    WaveVal.saSpecData.sample_size =  static_cast<std::byte>(0x10);
                    WaveVal.saSpecData.array_size = 0x10;
                }

            }

            m_WaveValResultList.push_back(WaveVal);

        }

void SocketClient::ReadCompoundWaveSaObservationValue(char* buffer)
{
            SaObsValueCmp WaveSaObjectValueCmp;
            WaveSaObjectValueCmp.count = htons(Read16ByteValuesFromBuffer(buffer, 0));
            WaveSaObjectValueCmp.length = htons(Read16ByteValuesFromBuffer(buffer, uInt16Size));

            int cmpwaveobjectscount = WaveSaObjectValueCmp.count;
            int cmpwaveobjectslength =  WaveSaObjectValueCmp.length;

            char* cmpwavearrayobject = ReadBytesFromBuffer(buffer, 2*uInt16Size, cmpwaveobjectslength);

            if (cmpwaveobjectscount > 0)
            {
                for (int k = 0; k < cmpwaveobjectscount; k++)
                {
                    ReadWaveSaObservationValue(cmpwavearrayobject);
                }
            }
}
void SocketClient::ReadSaSpecifications(char* buffer)
{
            SaSpec Saspecobj;
            Saspecobj.array_size = htons(Read16ByteValuesFromBuffer(buffer, 0));
            //
            Saspecobj.sample_size = static_cast<std::byte>(*ReadBytesFromBuffer(buffer, uInt16Size, 1));
            Saspecobj.significant_bits = static_cast<std::byte>(*ReadBytesFromBuffer(buffer, uInt16Size+1, 1));
            Saspecobj.SaFlags = htons(Read16ByteValuesFromBuffer(buffer, uInt16Size+2));

            Saspecobj.obpoll_handle = m_obpollhandle;

            //Add to a list of Sample array specification definitions if it's not already present

            auto it = std::find_if(m_SaSpecList.begin(), m_SaSpecList.end(), [&](const IDLabel& x) {
            return x.obpoll_handle == Saspecobj.obpoll_handle;
            });

            int salistindex = (it != m_SaSpecList.end()) ? std::distance(m_SaSpecList.begin(), it) : -1;

            if (salistindex == -1)
            {
                m_SaSpecList.push_back(Saspecobj);
            }
            else
            {
                m_SaSpecList.erase(m_SaSpecList.begin() + salistindex);
                m_SaSpecList.push_back(Saspecobj);
            }
}

void SocketClient::ReadSaScaleSpecifications(char* buffer)
{
            ScaleRangeSpec16 ScaleSpec;

            ScaleSpec.lower_absolute_value = htonl(ReadByteValuesFromBuffer(buffer, 0, 2*uInt16Size));
            ScaleSpec.upper_absolute_value = htonl(ReadByteValuesFromBuffer(buffer, 2*uInt16Size, 2*uInt16Size));
            ScaleSpec.lower_scaled_value = htons(Read16ByteValuesFromBuffer(buffer, 4*uInt16Size));
            ScaleSpec.upper_scaled_value = htons(Read16ByteValuesFromBuffer(buffer, 5*uInt16Size));

            ScaleSpec.obpoll_handle = m_obpollhandle;
            ScaleSpec.physio_id = Get16bitLSBfromUInt(m_idlabelhandle);

            //Add to a list of Sample array scale range specification definitions if it's not already present

             auto it = std::find_if(m_ScaleRangeSpecList.begin(), m_ScaleRangeSpecList.end(), [&](const IDLabel& x) {
            return x.obpoll_handle == ScaleSpec.obpoll_handle;
            });

            int salistindex = (it != m_ScaleRangeSpecList.end()) ? std::distance(m_ScaleRangeSpecList.begin(), it) : -1;

            if (salistindex == -1)
            {
                m_ScaleRangeSpecList.push_back(ScaleSpec);
            }
            else
            {
                m_ScaleRangeSpecList.erase(m_ScaleRangeSpecList.begin() + salistindex);
                m_ScaleRangeSpecList.push_back(ScaleSpec);
            }
            
}
void SocketClient::ReadSaCalibrationSpecifications(char* buffer)
{
            SaCalibData16 SaCalibData;

            SaCalibData.lower_absolute_value = htonl(ReadByteValuesFromBuffer(buffer, 0, 2*uInt16Size));
            SaCalibData.upper_absolute_value = htonl(ReadByteValuesFromBuffer(buffer, 2*uInt16Size, 2*uInt16Size));
            SaCalibData.lower_scaled_value = htons(Read16ByteValuesFromBuffer(buffer, 4*uInt16Size));
            SaCalibData.upper_scaled_value = htons(Read16ByteValuesFromBuffer(buffer, 5*uInt16Size));
            SaCalibData.increment = htons(Read16ByteValuesFromBuffer(buffer, 6*uInt16Size));
            SaCalibData.cal_type = htons(Read16ByteValuesFromBuffer(buffer, 7*uInt16Size));

            SaCalibData.obpoll_handle = m_obpollhandle;

            //Get 16 bit physiological id from 32 bit wave id label
            SaCalibData.physio_id = Get16bitLSBfromUInt(m_idlabelhandle);

            //Add to a list of Sample array calibration specification definitions if it's not already present

             auto it = std::find_if(m_SaCalibDataSpecList.begin(), m_SaCalibDataSpecList.end(), [&](const IDLabel& x) {
            return x.obpoll_handle == SaCalibData.obpoll_handle;
            });

            int salistindex = (it != m_SaCalibDataSpecList.end()) ? std::distance(m_SaCalibDataSpecList.begin(), it) : -1;

            if (salistindex == -1)
            {
                m_SaCalibDataSpecList.push_back(SaCalibData);
            }
            else
            {
                m_SaCalibDataSpecList.erase(m_SaCalibDataSpecList.begin() + salistindex);
                m_SaCalibDataSpecList.push_back(SaCalibData);
            }
}


int SocketClient::DecodeSingleContextPollObjects(SingleContextPoll* scpoll, char* buffer)
{
    scpoll->context_id = htons(Read16ByteValuesFromBuffer(buffer, 0));
    scpoll->count = htons(Read16ByteValuesFromBuffer(buffer, uInt16Size));
    //There can be empty singlecontextpollobjects
    //if(scpoll.count>0) scpoll.length = htons(binreader2.ReadUInt16());
    scpoll->length = htons(Read16ByteValuesFromBuffer(buffer,2*uInt16Size));

    int obpollobjectscount = int(scpoll->count);
    if (scpoll->length > 0)
    {
        scpoll->obpollobjectsarray = ReadBytesFromBuffer(buffer, 3*uInt16Size, scpoll->length);
    } 

            return obpollobjectscount;
}

int SocketClient::DecodeObservationPollObjects(ObservationPoll* obpollobject, char* buffer)
{
    obpollobject->obj_handle = htons(Read16ByteValuesFromBuffer(buffer, 0));

            m_obpollhandle = obpollobject->obj_handle;

            AttributeList* attributeliststruct;

            attributeliststruct->count = htons(Read16ByteValuesFromBuffer(buffer, uInt16Size));
            if (attributeliststruct->count > 0) 
            {
                attributeliststruct->length = htons(Read16ByteValuesFromBuffer(buffer, 2*uInt16Size));
            }

            int avaobjectscount = attributeliststruct->count;
            if (attributeliststruct->length > 0) 
            {
                obpollobject->avaobjectsarray = ReadBytesFromBuffer(buffer, 3*uInt16Size, attributeliststruct->length);
            }

            return avaobjectscount;
}



int SocketClient::DecodePollObjects(PollInfoList* pollobjects, char* packetbuffer)
        {
            pollobjects->count = htons(Read16ByteValuesFromBuffer(packetbuffer, 0));
            if (pollobjects->count > 0){
                pollobjects->length = htons(Read16ByteValuesFromBuffer(packetbuffer, uInt16Size));
            }

            int scpollobjectscount = pollobjects->count;
            if (pollobjects->length > 0) {
                pollobjects->scpollarray = ReadBytesFromBuffer(packetbuffer, 2*uInt16Size, pollobjects->length);
            }

            return scpollobjectscount;
        }


void SocketClient::SendCycledExtendedPollDataRequest()
{

}

void SocketClient::SendCycledExtendedPollWaveDataRequest()
{

}

void SocketClient::BeginReceive(int flags = 0) {

	UdpState temp_client_state;
	temp_client_state.state_ip = m_sa_remoteIPtarget;
	temp_client_state.state_client = this;	

	WSAOVERLAPPED overlapped{};	
	memset(&overlapped, 0, sizeof(WSAOVERLAPPED));
	overlapped.hEvent = reinterpret_cast<HANDLE>(&temp_client_state); //= reinterpret_cast<HANDLE>(new UdpState{ sock, m_sa_remoteIPtarget });


	int receiveResult = RecvFrom(temp_client_state.state_ip, temp_client_state.state_client.buffer, &overlapped, flags, ReceiveCallback);
	if (receiveResult == SOCKET_ERROR)
	{
		if (WSAGetLastError() != WSA_IO_PENDING)
		{
			std::cerr << "Failed to initiate receive. Error: " << WSAGetLastError() << std::endl;
			delete reinterpret_cast<UdpState*>(overlapped.hEvent);
			closesocket(sock);
			return;
		}
	}

	// Wait for the receive operation to complete
    if (WSAGetOverlappedResult(
            sock,                                               // SOCKET s
            reinterpret_cast<LPWSAOVERLAPPED>(&overlapped),     // LPWSAOVERLAPPED lpOverlapped
            reinterpret_cast<LPDWORD>(&numBytesReceived),    	// LPDWORD lpcbTransfer
            TRUE,                                               // BOOL fWait -- whether the function should wait until the overlapped operation is completed (true = wait, false = retrive results immediately)
            reinterpret_cast<LPDWORD>(&flags)))                 // LPDWORD lpdwFlags
    {
        // Data received successfully, process it
        if (numBytesReceived > 0)
        {
			// Receive operation completed successfully
			ReceiveCallback(0, numBytesReceived, &overlapped); //go and process data
        }
    }
    else
    {
        // Receive operation failed
        std::uint32_t errorCode = WSAGetLastError();
        if (errorCode != WSA_IO_PENDING) //WSA_IO_PENDING = overlapped operation is still in progress
        {
            // Handle error
            std::cout << "Receive error: " << errorCode << std::endl;
        }
    }

	// Clean up resources
	delete reinterpret_cast<UdpState*>(overlapped.hEvent);
}


/// @todo error handling
/// @todo add path_to_file
/// @todo version A (as is now, with string) or B (commented out, with bytes)?
void CALLBACK SocketClient::ReceiveCallback(DWORD errorCode, DWORD numBytesReceived /*cbTransferred*/, LPWSAOVERLAPPED overlapped, DWORD flags = 0)
{
    if (errorCode == 0) 
    {
		UdpState udpState = *static_cast<UdpState*>(overlapped->hEvent);
				//or pointer? //UdpState* udpState = reinterpret_cast<UdpState*>(overlapped->hEvent);

		//read the number of bytes received from the buffer into receivedData
        std::string receivedData(udpState.state_client.buffer, numBytesReceived); 

		// Convert buffer to std::vector<byte> if needed
	// B: std::vector<std::byte> data_bytes(udpState.state_client.buffer, udpState.state_client.buffer + numBytesReceived);

	//im cs code: string path = Path.Combine(Directory.GetCurrentDirectory(),Globals.pathFile+"_Philips_MPrawoutput.txt");
		std::string path_to_file = ""; //////////////////////////////////////////////////

		// Write data to file
		bool writing_to_file_successful = ByteArrayToFile(path_to_file, receivedData);
	// B: bool writing_to_file_successful2 = ByteArrayToFile(path_to_file, data_bytes, numBytesReceived);

    }
    else
    {
        std::cerr << "ReceiveCallback error: " << errorCode << std::endl;
        // Handle the error
        // ... //
    }
}


bool SocketClient::ByteArrayToFile(const std::string& path_to_file, const std::string& bytes_string)
{
    try
    {
        // Open file for writing
        std::ofstream outFile(path_to_file, std::ios::app);
        if (outFile.is_open())
        {
            // Write data to file
            outFile << bytes_string << std::endl;

            // Close file stream
            outFile.close();

            return true;
        }
    }
    catch (const std::exception& e)
    {
        std::cerr << "Exception caught in process: " << e.what() << std::endl;
    }

    // Error occurred, return false
    return false;
}


bool SocketClient::ByteArrayToFile(const std::string& path_to_file, const std::vector<std::byte>& data_bytes, uint32_t numBytesReceived)
{
    try
    {
        // Open file for writing
        std::ofstream outFile(path_to_file, std::ios::app);
        if (outFile.is_open())
        {
            // Convert byte array to hex string representation
            std::stringstream data_to_string;
            for (int i = 0; i < numBytesReceived; i++)
            {
                data_to_string << std::hex << static_cast<int>(data_bytes[i]);
                if (i != numBytesReceived - 1) data_to_string << "-";
            }
            std::string datastr = data_to_string.str();

            // Write data to file
            outFile << datastr << std::endl;

            // Close file stream
            outFile.close();

            return true;
        }
    }
    catch (const std::exception& e)
    {
        std::cerr << "Exception caught in process: " << e.what() << std::endl;
    }

    // Error occurred, return false
    return false;
}


void SocketClient::RecheckMDSAttributes(int nInterval = 0)
{
	int nMillisecond = 6 * 1000;
    if (nMillisecond != 0 && nInterval > 1000)
    {
        while (true)
        {
            SendMDSPollDataRequest();

            std::this_thread::sleep_for(std::chrono::milliseconds(nMillisecond));

        }
    }
}


void SocketClient::SendMDSPollDataRequest()
{



}


void SocketClient::KeepConnectionAlive(int nInterval = 0)
{
	int nMillisecond = 6 * 1000;
    if (nMillisecond != 0 && nInterval > 1000)
    {
        while (true)
        {
            SendMDSCreateEventResult();

            std::this_thread::sleep_for(std::chrono::milliseconds(nMillisecond));

        }
    }
}


void SocketClient::SendMDSCreateEventResult()
{


	
}


void SocketClient::establishLanConnection() 
{
    //  try
    //     {
            SendWaveAssociationRequest();
            //Receive AssociationResult message 
			///@todo schauen wie viel Buffer space wir brauchen (chapter4 p.29 sagt 1380 ->testen)
            char readassocbuffer[1380] = ""; 
            sockaddr_in addr;
					//try:
            addr = RecvFrom(readassocbuffer, 1380);
                    // _MPudpclient.ByteArrayToFile(path, readassocbuffer, readassocbuffer.GetLength(0));
                    //Console.WriteLine("Receiving SOC buffer");
					//if(addr = "")
					//{cout << "address empty">> endl;}		
					// catch: 
			//Receive MDSCreateEventReport message
            char readmdsconnectbuffer[1380] = "";
            sockaddr_in addr;
            addr = RecvFrom(readmdsconnectbuffer, 1380);
            		//readmdsconnectbuffer = _MPudpclient.Receive(ref _MPudpclient.m_remoteIPtarget);
                    // _MPudpclient.ByteArrayToFile(path, readmdsconnectbuffer, readmdsconnectbuffer.GetLength(0));
                    // //Console.WriteLine("Receiving ARRAY from buffer");

            //Send MDSCreateEventResult message
            ProcessPacket(readmdsconnectbuffer);

            //Send PollDataRequest message
                    //_MPudpclient.SendPollDataRequest();

                    //Send Extended PollData Requests cycled every second
                   // Task.Run(() => _MPudpclient.SendCycledExtendedPollDataRequest(nInterval));
                    //Console.WriteLine("POLL data REQUESTED");

                    //WaitForSeconds(1);

		//create string path to "_Philips_MPrawoutput.txt" in the current directory
				//string path = Path.Combine(Directory.GetCurrentDirectory(), Globals.pathFile+ "_Philips_MPrawoutput.txt"); //IB


		//Receive AssociationResult message (aka SOC buffer) and MDSCreateEventReport message
			//readassocbuffer = RecvFrom();
				// _MPudpclient.ByteArrayToFile(path, readassocbuffer, readassocbuffer.GetLength(0));


		//Receive ARRAY from buffer
			//readmdsconnectbuffer = _MPudpclient.Receive(ref _MPudpclient.m_remoteIPtarget);
				// _MPudpclient.ByteArrayToFile(path, readmdsconnectbuffer, readmdsconnectbuffer.GetLength(0));


		//Send MDSCreateEventResult message
				//_MPudpclient.ProcessPacket(readmdsconnectbuffer);


		//Send Extended PollData Requests cycled every second
				//Task.Run(() => _MPudpclient.SendCycledExtendedPollDataRequest(nInterval));


		/*if (nWaveformSet != 0)
		{
			_MPudpclient.GetRTSAPriorityListRequest();
			if (nWaveformSet != 11) 

						//what is nwaveformset? why 11? possible reasons with 11 in guide:

						//header information p.30: The protocol_id field contains ID and version information.
						//It can be used to define different service access points. Data Export uses the ID 0x11.

						//The StringFormat describes how strings are encoded. The IntelliVue monitor uses unicode encoding.
						//typedef u_16 StringFormat; #define STRFMT_UNICODE_NT 11

						//Num 11 Placeholder for Vuelink Flex Text
						//Label: NLS_VUELINK_FLX1_NPS_TEXT_NUM11 

						//alert code NOM_EVT_EXT_DEV_AL_CODE_11

						//AbortSessionData
						//0x11 0x01 0x03


			{
				_MPudpclient.SetRTSAPriorityList(nWaveformSet); //wave object priority list specifies processing order of wave objects/data
			}

			Task.Run(() => _MPudpclient.SendCycledExtendedPollWaveDataRequest(nInterval)); //method executed asynchronously in the background thread pool thread

						//The (cycled, repetitive) request aims to retrieve waveform data (a sequence of values that represent a waveform or signal) and may encompass 
						//additional info or extended data associated with the waveforms (metadata, timestamps, measurement parameters, etc)
						
						//By using Task.Run(), the given code snippet creates a new task that runs the lambda expression in a separate background thread. 
						//It enables the code to run asynchronously, allowing other tasks or operations to continue without waiting for the completion of the specified method.

						//(parameters) => {expression}
						//Lambda expressions are often used in situations where a short, inline function is required. 
						//Instead of declaring a separate method or delegate, a lambda expression allows you to define the functionality right at the point where it is needed.

						//in c++: 	std::thread([&]() { _MPudpclient.SendCycledExtendedPollWaveDataRequest(nInterval); }).detach();
		}*/


////////////////////////////////////////////////  :))  //////////////////////////////////////////////////////////////////////////////////////////////////////////////

		/// @todo add where nInterval is actually coming from
		int nInterval = 0;


		//Recheck MDS Attributes	//Task.Run(() => _MPudpclient.RecheckMDSAttributes(nInterval));				
		std::thread recheckMDSAttributesThread([&]() {	
			//RecheckMDSAttributes(nInterval);
			RecheckMDSAttributes(nInterval);

		});
		recheckMDSAttributesThread.detach(); //allows the thread to continue executing independently, without requiring synchronization or joining with the parent thread

				//lambda functions in C++:	[capture-list](parameters) -> return-type { function-body }
				//capture-list being the variables from the enclosing scope that you want to have accessible within the lambda function, can be captured by value, reference, or a mix of both

				//possibly [&] instead of [this] --> & indicates that you want to capture all variables from the enclosing scope by reference, 
				//this for capturing all the class members by ref so you can work w the class members within the lambda body as if you were inside a member function of the class


		//Keep Connection Alive		//Task.Run(() => _MPudpclient.KeepConnectionAlive(nInterval));
		std::thread keepConnectionAliveThread([&]() {
			//KeepConnectionAlive(nInterval);
			KeepConnectionAlive(nInterval);
		});
		keepConnectionAliveThread.detach();


		//Receive PollDataResponse message / Receive poll data		//_MPudpclient.BeginReceive(new AsyncCallback(ReceiveCallback), state);
		std::thread receiveThread([&]() {
			BeginReceive(); //receive data asynchronously
		});
		receiveThread.detach();

				//It's important to note that the ReceiveCallback function should be implemented in a way that it can handle multiple invocations and 
				//process each packet of data independently. The function should not rely on any assumptions about the order or timing of packet arrivals, 
				//as they can occur in an unpredictable manner due to the asynchronous nature of the UDP communication.


////////////////////////////////////////////////  :))  //////////////////////////////////////////////////////////////////////////////////////////////////////////////


	// 	} catch (Exception ex) {
	//			Console.WriteLine("Error opening/writing to UDP port :: " + ex.Message, "Error!");
	//          Console.Beep();
	//	} 
	
	// ... //






//     try
//     {
//         WSASession Session; //initialize the windows sockets api session
//         UDPSocket Socket; //create a udp socket
        
//         string data = "hello world"; //define an example string to send
//         char buffer[100]; //create a buffer to hold received data

//         Socket.SendTo("127.0.0.1", 100, data.c_str(), data.size()); // Send data to IP address and port 100
//         Socket.RecvFrom(buffer, 100); // Receive data of length 100 into the buffer
//         cout << buffer; // Output the received data
//     }
//     catch (exception &ex) //catch any occurring system errors
//     {
//         cout << ex.what();  //print error message
//     }
//     char c;
//     cin >> c; // Wait for user input before exiting
//}
}

