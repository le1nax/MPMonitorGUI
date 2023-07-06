#pragma once

#include <cmath>
#include <memory>
#include <list>
#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <map>
#include <memory>
#include <cstring>
#include <codecvt>
#include <locale>


constexpr int FLOATTYPE_NAN = 0x007FFFFF;
constexpr int FLOATTYPE_NRes = 0x800000;
constexpr int FLOATTYPE_POSITIVE_INFINITY = 0x7FFFFE;
constexpr int FLOATTYPE_NEGATIVE_INFINITY = 0x800002;

constexpr unsigned int maxbuffersize = 1380;


///@todo statt const variablen constexpr

template<typename... Ts>
std::vector<std::byte> make_bytes(Ts&&... args) noexcept {
return{std::byte(std::forward<Ts>(args))...};
}


static std::string ReplaceNullCharacters(const std::string& inputString) {
    std::string outputString = inputString;
    size_t pos = outputString.find('\0');
    while (pos != std::string::npos) {
        outputString.replace(pos, 1, "");
        pos = outputString.find('\0', pos);
    }
    return outputString;
}

static uint16_t Get16bitLSBfromUInt(uint32_t sourcevalue)
        {
            uint32_t lsb = (sourcevalue & 0xFFFF);
            return (uint16_t)lsb;
        }

static std::string TrimString(const std::string& inputString) {
    std::string trimmedString = inputString;
    size_t firstNonWhitespace = trimmedString.find_first_not_of(" \t\r\n");
    size_t lastNonWhitespace = trimmedString.find_last_not_of(" \t\r\n");
    if (firstNonWhitespace != std::string::npos && lastNonWhitespace != std::string::npos) {
        trimmedString = trimmedString.substr(firstNonWhitespace, lastNonWhitespace - firstNonWhitespace + 1);
    }
    else {
        trimmedString.clear();
    }
    return trimmedString;
}

static std::string Utf8ToString(const std::string& utf8String)
{
    std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
    std::wstring wideString = converter.from_bytes(utf8String);
    std::string outputString(wideString.begin(), wideString.end());
    return outputString;
}

static inline constexpr size_t uInt16Size = sizeof(unsigned short);

/// @todo testen
static size_t Read16ByteValuesFromBuffer(const char* buffer, size_t startIndex)
{
    try {
    size_t bufferSize = sizeof(buffer);
    if (startIndex >= bufferSize)
    {
        throw std::runtime_error("Error: Invalid starting index.");
    }
    if (startIndex + uInt16Size > bufferSize)
    {
        throw std::runtime_error("Error: Insufficient buffer size.");
    }
    size_t value;
    std::memcpy(&value, buffer + startIndex, uInt16Size);
    return value;
    } catch(...) {
        std::throw_with_nested(std::runtime_error("Failed to read: " + *buffer));
    }
}

/// @todo testen
static size_t ReadByteValuesFromBuffer(const char* buffer, size_t startIndex, size_t numBytesToRead)
{
    try {
    size_t bufferSize = sizeof(buffer);
    if (startIndex >= bufferSize)
    {
        throw std::runtime_error("Error: Invalid starting index.");
    }
    if (startIndex + uInt16Size > bufferSize)
    {
        throw std::runtime_error("Error: Insufficient buffer size.");
    }
    size_t value;
    std::memcpy(&value, buffer + startIndex, numBytesToRead);
    return value;
    } catch(...) {
        std::throw_with_nested(std::runtime_error("Failed to read: " + *buffer));
    }
}

static int BinaryCodedDecimalToInteger(int value)
        {
            if (value != 0xFF)
            {
                int lowerNibble = value & 0x0F;
                int upperNibble = value >> 4;

                int multipleOfOne = lowerNibble;
                int multipleOfTen = upperNibble * 10;

                return (multipleOfOne + multipleOfTen);
            }
            else return 0;
        }

static double FloattypeToValue(uint32_t fvalue)
{
    double value = 0;
    if (fvalue != FLOATTYPE_NAN)
    {
        int exponentbits = static_cast<int>(fvalue >> 24);
        int mantissabits = static_cast<int>(fvalue << 8);
        mantissabits = (mantissabits >> 8);

        signed char signedexponentbits = static_cast<signed char>(exponentbits); // Get Two's complement signed byte
        double exponent = static_cast<double>(signedexponentbits);

        double mantissa = mantissabits;
        value = mantissa * std::pow(10.0, exponent);

        return value;
    }
    else
    {
        return static_cast<double>(fvalue);
    }
}

/// @todo testen
static char* ReadBytesFromBuffer(const char* buffer, size_t startIndex, size_t numBytesToRead)
{
    try {
    size_t bufferSize = sizeof(buffer);
    if (startIndex >= bufferSize)
    {
        throw std::runtime_error("Error: Invalid starting index.");
    }
    if (startIndex + uInt16Size > bufferSize)
    {
        throw std::runtime_error("Error: Insufficient buffer size.");
    }
    char* read_buffer;
    std::memcpy(read_buffer, buffer + startIndex, numBytesToRead);
    return read_buffer;
    } catch(...) {
        std::throw_with_nested(std::runtime_error("Failed to read: " + *buffer));
    }
}

        //Date and Time
        constexpr int NOM_ATTR_TIME_ABS = 0x987;
        //Sample Period
        constexpr int NOM_ATTR_TIME_PD_SAMP = 0x98d;
        //Relative Time
        constexpr int NOM_ATTR_TIME_REL = 0x98f;
        //Absolute Time Stamp
        constexpr int NOM_ATTR_TIME_STAMP_ABS = 0x990;
        //Relative Time Stamp
        constexpr int NOM_ATTR_TIME_STAMP_REL = 0x991;
        //Patient Date of Birth
        constexpr int NOM_ATTR_PT_DOB = 0x958;
        //Patient ID
        constexpr int NOM_ATTR_PT_ID = 0x95a;
        //Family Name
        constexpr int NOM_ATTR_PT_NAME_FAMILY = 0x95c;
        //Given Name
        constexpr int NOM_ATTR_PT_NAME_GIVEN = 0x95d;




//ROapdus
    constexpr unsigned char ROIV_APDU = 1;
    constexpr unsigned char RORS_APDU = 2;
    constexpr unsigned char ROER_APDU = 3;
    constexpr unsigned char RORLS_APDU = 5;

//ActionType
    constexpr uint16_t NOM_ACT_POLL_MDIB_DATA = 3094; //Single poll
    constexpr uint16_t NOM_ACT_POLL_MDIB_DATA_EXT = 61755; //Extended poll

        constexpr uint16_t NOM_MOC_VMS_MDS = 33;
        //MDS
        constexpr int NOM_ATTR_METRIC_SPECN = 2367;
        constexpr int NOM_ATTR_METRIC_INFO_LABEL = 0x93c;
        constexpr int NOM_ATTR_METRIC_INFO_LABEL_STR = 0x93d;
        constexpr int NOM_ATTR_ID_HANDLE = 2337;
        constexpr int NOM_ATTR_ID_LABEL = 2340;
        constexpr int NOM_ATTR_ID_LABEL_STRING = 2343;
        constexpr int NOM_ATTR_NU_CMPD_VAL_OBS = 2379;
        constexpr int NOM_ATTR_NU_VAL_OBS = 2384;
        constexpr int NOM_ATTR_SYS_TYPE = 0x986;
        constexpr int NOM_ATTR_SA_CALIB_I16 = 0x964;
        //Compound Sample Array Observed Value
        constexpr int NOM_ATTR_SA_CMPD_VAL_OBS = 0x967;
        //Sample Array Physiological Range
        constexpr int NOM_ATTR_SA_RANGE_PHYS_I16 = 0x96a;
        //Sample Array Specification
        constexpr int NOM_ATTR_SA_SPECN = 0x96d;
        //Sample Array Observed Value
        constexpr int NOM_ATTR_SA_VAL_OBS = 0x96e;
        //Scale and Range Specification
        constexpr int NOM_ATTR_SCALE_SPECN_I16 = 0x96f;

     enum class Commands : uint16_t
        {
            CMD_EVENT_REPORT = 0,
            CMD_CONFIRMED_EVENT_REPORT = 1,
            CMD_GET = 3,
            CMD_SET = 4,
            CMD_CONFIRMED_SET = 5,
            CMD_CONFIRMED_ACTION = 7
        };

    enum class RemoteOperationHeader : uint16_t
        {
            ROIV_APDU = 1,
            //request/command from client (PC)
            RORS_APDU = 2,
            //single frame result from monitor except error
            ROER_APDU = 3,
            //remote operation error
            ROLRS_APDU = 5
            //e.g. Single Poll Linked Result
        };
    
    enum class AttributeIDs : uint16_t
        {
            //Device P-Alarm List
            NOM_ATTR_AL_MON_P_AL_LIST = 0x902,
            //Device T-Alarm List
            NOM_ATTR_AL_MON_T_AL_LIST = 0x904,
            //Altitude
            NOM_ATTR_ALTITUDE = 0x90c,
            //Application Area
            NOM_ATTR_AREA_APPL = 0x90d,
            //Color
            NOM_ATTR_COLOR = 0x911,
            //Device Alert Condition
            NOM_ATTR_DEV_AL_COND = 0x916,
            //Display Resolution
            NOM_ATTR_DISP_RES = 0x917,
            //Visual Grid
            NOM_ATTR_GRID_VIS_I16 = 0x91a,
            //Association Invoke Id
            NOM_ATTR_ID_ASSOC_NO = 0x91d,
            //Bed Label
            NOM_ATTR_ID_BED_LABEL = 0x91e,
            //Object Handle
            NOM_ATTR_ID_HANDLE = 0x921,
            //Label
            NOM_ATTR_ID_LABEL = 0x924,
            //Label String
            NOM_ATTR_ID_LABEL_STRING = 0x927,
            //System Model
            NOM_ATTR_ID_MODEL = 0x928,
            //Product Specification
            NOM_ATTR_ID_PROD_SPECN = 0x92d,
            //Object Type
            NOM_ATTR_ID_TYPE = 0x92f,
            //Line Frequency
            NOM_ATTR_LINE_FREQ = 0x935,
            //System Localization
            NOM_ATTR_LOCALIZN = 0x937,
            //Metric Info Label
            NOM_ATTR_METRIC_INFO_LABEL = 0x93c,
            //Metric Info Label String
            NOM_ATTR_METRIC_INFO_LABEL_STR = 0x93d,
            //Metric Specification
            NOM_ATTR_METRIC_SPECN = 0x93f,
            //Metric State
            NOM_ATTR_METRIC_STAT = 0x940,
            //Measure Mode
            NOM_ATTR_MODE_MSMT = 0x945,
            //Operating Mode
            NOM_ATTR_MODE_OP = 0x946,
            //Nomenclature Version
            NOM_ATTR_NOM_VERS = 0x948,
            //Compound Numeric Observed Value
            NOM_ATTR_NU_CMPD_VAL_OBS = 0x94b,
            //Numeric Observed Value
            NOM_ATTR_NU_VAL_OBS = 0x950,
            //Patient BSA
            NOM_ATTR_PT_BSA = 0x956,
            //Pat Demo State
            NOM_ATTR_PT_DEMOG_ST = 0x957,
            //Patient Date of Birth
            NOM_ATTR_PT_DOB = 0x958,
            //Patient ID
            NOM_ATTR_PT_ID = 0x95a,
            //Family Name
            NOM_ATTR_PT_NAME_FAMILY = 0x95c,
            //Given Name
            NOM_ATTR_PT_NAME_GIVEN = 0x95d,
            //Patient Sex
            NOM_ATTR_PT_SEX = 0x961,
            //Patient Type
            NOM_ATTR_PT_TYPE = 0x962,
            //Sample Array Calibration Specification
            NOM_ATTR_SA_CALIB_I16 = 0x964,
            //Compound Sample Array Observed Value
            NOM_ATTR_SA_CMPD_VAL_OBS = 0x967,
            //Sample Array Physiological Range
            NOM_ATTR_SA_RANGE_PHYS_I16 = 0x96a,
            //Sample Array Specification
            NOM_ATTR_SA_SPECN = 0x96d,
            //Sample Array Observed Value
            NOM_ATTR_SA_VAL_OBS = 0x96e,
            //Scale and Range Specification
            NOM_ATTR_SCALE_SPECN_I16 = 0x96f,
            //Safety Standard
            NOM_ATTR_STD_SAFETY = 0x982,
            //System ID
            NOM_ATTR_SYS_ID = 0x984,
            //System Specification
            NOM_ATTR_SYS_SPECN = 0x985,
            //System Type
            NOM_ATTR_SYS_TYPE = 0x986,
            //Date and Time
            NOM_ATTR_TIME_ABS = 0x987,
            //Sample Period
            NOM_ATTR_TIME_PD_SAMP = 0x98d,
            //Relative Time
            NOM_ATTR_TIME_REL = 0x98f,
            //Absolute Time Stamp
            NOM_ATTR_TIME_STAMP_ABS = 0x990,
            //Relative Time Stamp
            NOM_ATTR_TIME_STAMP_REL = 0x991,
            //Unit Code
            NOM_ATTR_UNIT_CODE = 0x996,
            //Enumeration Observed Value
            NOM_ATTR_VAL_ENUM_OBS = 0x99e,
            //MDS Status
            NOM_ATTR_VMS_MDS_STAT = 0x9a7,
            //Patient Age
            NOM_ATTR_PT_AGE = 0x9d8,
            //Patient Height
            NOM_ATTR_PT_HEIGHT = 0x9dc,
            //Patient Weight
            NOM_ATTR_PT_WEIGHT = 0x9df,
            //Sample Array Fixed Values Specification
            NOM_ATTR_SA_FIXED_VAL_SPECN = 0xa16,
            //Patient Paced Mode
            NOM_ATTR_PT_PACED_MODE = 0xa1e,
            //Internal Patient ID
            NOM_ATTR_PT_ID_INT = 0xf001,
            //Private Attribute
            NOM_SAT_O2_TONE_FREQ = 0xf008,
            //Private Attribute
            NOM_ATTR_CMPD_REF_LIST = 0xf009,
            //IP Address Information
            NOM_ATTR_NET_ADDR_INFO = 0xf100,
            //Protocol Support
            NOM_ATTR_PCOL_SUPPORT = 0xf101,
            //Notes1
            NOM_ATTR_PT_NOTES1 = 0xf129,
            //Notes2
            NOM_ATTR_PT_NOTES2 = 0xf12a,
            //Time for Periodic Polling
            NOM_ATTR_TIME_PD_POLL = 0xf13e,
            //Patient BSA Formula
            NOM_ATTR_PT_BSA_FORMULA = 0xf1ec,
            //Mds General System Info
            NOM_ATTR_MDS_GEN_INFO = 0xf1fa,
            //no of prioritized objects for poll request
            NOM_ATTR_POLL_OBJ_PRIO_NUM = 0xf228,
            //Numeric Object Priority List
            NOM_ATTR_POLL_NU_PRIO_LIST = 0xf239,
            //Wave Object Priority List
            NOM_ATTR_POLL_RTSA_PRIO_LIST = 0xf23a,
            //Metric Modality
            NOM_ATTR_METRIC_MODALITY = 0xf294,
            //The attributes are arranged in the following attribute groups:
            //Alert Monitor Group
            NOM_ATTR_GRP_AL_MON = 0x801,
            //Metric Observed Value Group
            NOM_ATTR_GRP_METRIC_VAL_OBS = 0x803,
            //Patient Demographics Attribute Group
            NOM_ATTR_GRP_PT_DEMOG = 0x807,
            //System Application Attribute Group
            NOM_ATTR_GRP_SYS_APPL = 0x80a,
            //System Identification Attribute Group
            NOM_ATTR_GRP_SYS_ID = 0x80b,
            //System Production Attribute Group
            NOM_ATTR_GRP_SYS_PROD = 0x80c,
            //VMO Dynamic Attribute Group
            NOM_ATTR_GRP_VMO_DYN = 0x810,
            //VMO Static Attribute Group
            NOM_ATTR_GRP_VMO_STATIC = 0x811
        };

    enum class WavesIDLabels : uint32_t
        {
            NLS_NOM_ECG_ELEC_POTL = 0x00020100,
            NLS_NOM_ECG_ELEC_POTL_I = 0x00020101,
            NLS_NOM_ECG_ELEC_POTL_II = 0x00020102,
            NLS_NOM_ECG_ELEC_POTL_III = 0x0002013D,
            NLS_NOM_ECG_ELEC_POTL_AVR = 0x0002013E,
            NLS_NOM_ECG_ELEC_POTL_AVL = 0x0002013F,
            NLS_NOM_ECG_ELEC_POTL_AVF = 0x00020140,
            NLS_NOM_ECG_ELEC_POTL_V1 = 0x00020103,
            NLS_NOM_ECG_ELEC_POTL_V2 = 0x00020104,
            NLS_NOM_ECG_ELEC_POTL_V3 = 0x00020105,
            NLS_NOM_ECG_ELEC_POTL_V4 = 0x00020106,
            NLS_NOM_ECG_ELEC_POTL_V5 = 0x00020107,
            NLS_NOM_ECG_ELEC_POTL_V6 = 0x00020108,
            NLS_NOM_PULS_OXIM_PLETH = 0x00024BB4,
            NLS_NOM_PULS_OXIM_SAT_O2 = 0x00024BB8,
            NLS_NOM_PULS_OXIM_PULS_RATE = 0x00024822,
            NLS_NOM_PRESS_BLD_ART = 0x00024A10,
            NLS_NOM_PRESS_BLD_ART_ABP = 0x00024A14,
            NLS_NOM_PRESS_BLD_VEN_CENT = 0x00024A44,
            NLS_NOM_RESP = 0x00025000,
            NLS_NOM_AWAY_CO2 = 0x000250AC,
            NLS_NOM_PRESS_AWAY = 0x000250F0,
            NLS_NOM_FLOW_AWAY = 0x000250D4,
            NLS_EEG_NAMES_EEG_CHAN1_LBL = 0x800F5401,
            NLS_EEG_NAMES_EEG_CHAN2_LBL = 0x800F5402,
            NLS_EEG_NAMES_EEG_CHAN3_LBL = 0x800F5432,
            NLS_EEG_NAMES_EEG_CHAN4_LBL = 0x800F5434,
            NLS_NOM_PRESS_INTRA_CRAN = 0x00025808,
            NLS_NOM_PRESS_INTRA_CRAN_2 = 0x0002F0B8,
            NLS_NOM_TEMP_BLD = 0x0002E014,
            NLS_VUELINK_FLX1_NPS_TEXT_WAVE1 = 0x80AAF001,
            NLS_VUELINK_FLX1_NPS_TEXT_WAVE2 = 0x80AAF003,
            NLS_VUELINK_FLX1_NPS_TEXT_WAVE3 = 0x80AAF005,
            NLS_VUELINK_FLX1_NPS_TEXT_WAVE4 = 0x80AAF007,
            NLS_VUELINK_FLX1_NPS_TEXT_WAVE5 = 0x80AAF009,
            NLS_VUELINK_FLX1_NPS_TEXT_WAVE6 = 0x80AAF00B,
            NLS_VUELINK_FLX1_NPS_TEXT_WAVE7 = 0x80AAF00D,
            NLS_VUELINK_FLX1_NPS_TEXT_WAVE8 = 0x80AAF00F

        };

enum class AlertSource : uint16_t
        {
            NOM_ECG_LEAD_IG_LEAD_I = 1,
            NOM_ECG_LEAD_II = 2,
            NOM_ECG_LEAD_LA = 21,
            NOM_ECG_LEAD_RA = 22,
            NOM_ECG_LEAD_LL = 23,
            NOM_ECG_LEAD_fI = 24,
            NOM_ECG_LEAD_fE = 25,
            NOM_ECG_LEAD_fA = 27,
            NOM_ECG_LEAD_C = 66,
            NOM_ECG_LEAD_C1FR = 82,
            NOM_ECG_LEAD_C2FR = 83,
            NOM_ECG_LEAD_C3FR = 84,
            NOM_ECG_LEAD_C4FR = 85,
            NOM_ECG_LEAD_C5FR = 87,
            NOM_ECG_LEAD_C6FR = 88,
            NOM_ECG_LEAD_C7FR = 89,
            NOM_ECG_LEAD_C8FR = 90,
            NOM_ECG_LEAD_ES = 100,
            NOM_ECG_LEAD_AS = 101,
            NOM_ECG_LEAD_AI = 102,
            NOM_ECG_LEAD_RL = 115,
            NOM_ECG_LEAD_EASI_S = 116,
            NOM_ECG_ELEC_POTL = 256,
            NOM_ECG_ELEC_POTL_I = 257,
            NOM_ECG_ELEC_POTL_II = 258,
            NOM_ECG_ELEC_POTL_V1 = 259,
            NOM_ECG_ELEC_POTL_V2 = 260,
            NOM_ECG_ELEC_POTL_V3 = 261,
            NOM_ECG_ELEC_POTL_V4 = 262,
            NOM_ECG_ELEC_POTL_V5 = 263,
            NOM_ECG_ELEC_POTL_V6 = 264,
            NOM_ECG_ELEC_POTL_III = 317,
            NOM_ECG_ELEC_POTL_AVR = 318,
            NOM_ECG_ELEC_POTL_AVL = 319,
            NOM_ECG_ELEC_POTL_AVF = 320,
            NOM_ECG_ELEC_POTL_V = 323,
            NOM_ECG_ELEC_POTL_MCL = 331,
            NOM_ECG_ELEC_POTL_MCL1 = 332,
            NOM_ECG_AMPL_ST = 768,
            NOM_ECG_AMPL_ST_I = 769,
            NOM_ECG_AMPL_ST_II = 770,
            NOM_ECG_AMPL_ST_V1 = 771,
            NOM_ECG_AMPL_ST_V2 = 772,
            NOM_ECG_AMPL_ST_V3 = 773,
            NOM_ECG_AMPL_ST_V4 = 774,
            NOM_ECG_AMPL_ST_V5 = 775,
            NOM_ECG_AMPL_ST_V6 = 776,
            NOM_ECG_AMPL_ST_III = 829,
            NOM_ECG_AMPL_ST_AVR = 830,
            NOM_ECG_AMPL_ST_AVL = 831,
            NOM_ECG_AMPL_ST_AVF = 832,
            NOM_ECG_AMPL_ST_V = 835,
            NOM_ECG_AMPL_ST_MCL = 843,
            NOM_ECG_AMPL_ST_ES = 868,
            NOM_ECG_AMPL_ST_AS = 869,
            NOM_ECG_AMPL_ST_AI = 870,
            NOM_ECG_TIME_PD_QT_GL = 16160,
            NOM_ECG_TIME_PD_QTc = 16164,
            NOM_ECG_CARD_BEAT_RATE = 16770,
            NOM_ECG_CARD_BEAT_RATE_BTB = 16778,
            NOM_ECG_V_P_C_CNT = 16993,
            NOM_ECG_V_P_C_RATE = 16994,
            NOM_ECG_V_P_C_FREQ = 17000,
            NOM_PULS_RATE = 18442,
            NOM_PLETH_PULS_RATE = 18466,
            NOM_RES_VASC_SYS_INDEX = 18688,
            NOM_WK_LV_STROKE_INDEX = 18692,
            NOM_WK_RV_STROKE_INDEX = 18696,
            NOM_OUTPUT_CARD_INDEX = 18700,
            NOM_PRESS_BLD = 18944,
            NOM_PRESS_BLD_SYS = 18945,
            NOM_PRESS_BLD_DIA = 18946,
            NOM_PRESS_BLD_MEAN = 18947,
            NOM_PRESS_BLD_NONINV = 18948,
            NOM_PRESS_BLD_NONINV_SYS = 18949,
            NOM_PRESS_BLD_NONINV_DIA = 18950,
            NOM_PRESS_BLD_NONINV_MEAN = 18951,
            NOM_PRESS_BLD_AORT = 18956,
            NOM_PRESS_BLD_AORT_SYS = 18957,
            NOM_PRESS_BLD_AORT_DIA = 18958,
            NOM_PRESS_BLD_AORT_MEAN = 18959,
            NOM_PRESS_BLD_ART = 18960,
            NOM_PRESS_BLD_ART_SYS = 18961,
            NOM_PRESS_BLD_ART_DIA = 18962,
            NOM_PRESS_BLD_ART_MEAN = 18963,
            NOM_PRESS_BLD_ART_ABP = 18964,
            NOM_PRESS_BLD_ART_ABP_SYS = 18965,
            NOM_PRESS_BLD_ART_ABP_DIA = 18966,
            NOM_PRESS_BLD_ART_ABP_MEAN = 18967,
            NOM_PRESS_BLD_ART_PULM = 18972,
            NOM_PRESS_BLD_ART_PULM_SYS = 18973,
            NOM_PRESS_BLD_ART_PULM_DIA = 18974,
            NOM_PRESS_BLD_ART_PULM_MEAN = 18975,
            NOM_PRESS_BLD_ART_PULM_WEDGE = 18980,
            NOM_PRESS_BLD_ART_UMB = 18984,
            NOM_PRESS_BLD_ART_UMB_SYS = 18985,
            NOM_PRESS_BLD_ART_UMB_DIA = 18986,
            NOM_PRESS_BLD_ART_UMB_MEAN = 18987,
            NOM_PRESS_BLD_ATR_LEFT = 18992,
            NOM_PRESS_BLD_ATR_LEFT_SYS = 18993,
            NOM_PRESS_BLD_ATR_LEFT_DIA = 18994,
            NOM_PRESS_BLD_ATR_LEFT_MEAN = 18995,
            NOM_PRESS_BLD_ATR_RIGHT = 18996,
            NOM_PRESS_BLD_ATR_RIGHT_SYS = 18997,
            NOM_PRESS_BLD_ATR_RIGHT_DIA = 18998,
            NOM_PRESS_BLD_ATR_RIGHT_MEAN = 18999,
            NOM_PRESS_BLD_VEN_CENT = 19012,
            NOM_PRESS_BLD_VEN_CENT_SYS = 19013,
            NOM_PRESS_BLD_VEN_CENT_DIA = 19014,
            NOM_PRESS_BLD_VEN_CENT_MEAN = 19015,
            NOM_PRESS_BLD_VEN_UMB = 19016,
            NOM_PRESS_BLD_VEN_UMB_SYS = 19017,
            NOM_PRESS_BLD_VEN_UMB_DIA = 19018,
            NOM_PRESS_BLD_VEN_UMB_MEAN = 19019,
            NOM_SAT_O2_CONSUMP = 19200,
            NOM_OUTPUT_CARD = 19204,
            NOM_RES_VASC_PULM = 19236,
            NOM_RES_VASC_SYS = 19240,
            NOM_SAT_O2 = 19244,
            NOM_SAT_O2_ART = 19252,
            NOM_SAT_O2_VEN = 19260,
            NOM_SAT_DIFF_O2_ART_ALV = 19264,
            NOM_TEMP = 19272,
            NOM_TEMP_ART = 19280,
            NOM_TEMP_AWAY = 19284,
            NOM_TEMP_CORE = 19296,
            NOM_TEMP_ESOPH = 19300,
            NOM_TEMP_INJ = 19304,
            NOM_TEMP_NASOPH = 19308,
            NOM_TEMP_SKIN = 19316,
            NOM_TEMP_TYMP = 19320,
            NOM_TEMP_VEN = 19324,
            NOM_VOL_BLD_STROKE = 19332,
            NOM_WK_CARD_LEFT = 19344,
            NOM_WK_CARD_RIGHT = 19348,
            NOM_WK_LV_STROKE = 19356,
            NOM_WK_RV_STROKE = 19364,
            NOM_PULS_OXIM_PERF_REL = 19376,
            NOM_PLETH = 19380,
            NOM_PULS_OXIM_SAT_O2 = 19384,
            NOM_PULS_OXIM_SAT_O2_DIFF = 19396,
            NOM_PULS_OXIM_SAT_O2_ART_LEFT = 19400,
            NOM_PULS_OXIM_SAT_O2_ART_RIGHT = 19404,
            NOM_OUTPUT_CARD_CTS = 19420,
            NOM_VOL_VENT_L_END_SYS = 19460,
            NOM_GRAD_PRESS_BLD_AORT_POS_MAX = 19493,
            NOM_RESP = 20480,
            NOM_RESP_RATE = 20490,
            NOM_AWAY_RESP_RATE = 20498,
            NOM_CAPAC_VITAL = 20608,
            NOM_COMPL_LUNG = 20616,
            NOM_COMPL_LUNG_DYN = 20620,
            NOM_COMPL_LUNG_STATIC = 20624,
            NOM_CONC_AWAY_CO2 = 20628,
            NOM_CONC_AWAY_CO2_ET = 20636,
            NOM_CONC_AWAY_CO2_INSP_MIN = 20646,
            NOM_AWAY_CO2 = 20652,
            NOM_AWAY_CO2_ET = 20656,
            NOM_AWAY_CO2_INSP_MIN = 20666,
            NOM_CO2_TCUT = 20684,
            NOM_O2_TCUT = 20688,
            NOM_FLOW_AWAY = 20692,
            NOM_FLOW_AWAY_EXP_MAX = 20697,
            NOM_FLOW_AWAY_INSP_MAX = 20701,
            NOM_FLOW_CO2_PROD_RESP = 20704,
            NOM_IMPED_TTHOR = 20708,
            NOM_PRESS_RESP_PLAT = 20712,
            NOM_PRESS_AWAY = 20720,
            NOM_PRESS_AWAY_MIN = 20722,
            NOM_PRESS_AWAY_CTS_POS = 20724,
            NOM_PRESS_AWAY_NEG_MAX = 20729,
            NOM_PRESS_AWAY_END_EXP_POS_INTRINSIC = 20736,
            NOM_PRESS_AWAY_INSP = 20744,
            NOM_PRESS_AWAY_INSP_MAX = 20745,
            NOM_PRESS_AWAY_INSP_MEAN = 20747,
            NOM_RATIO_IE = 20760,
            NOM_RATIO_AWAY_DEADSP_TIDAL = 20764,
            NOM_RES_AWAY = 20768,
            NOM_RES_AWAY_EXP = 20772,
            NOM_RES_AWAY_INSP = 20776,
            NOM_TIME_PD_APNEA = 20784,
            NOM_VOL_AWAY_TIDAL = 20796,
            NOM_VOL_MINUTE_AWAY = 20808,
            NOM_VOL_MINUTE_AWAY_EXP = 20812,
            NOM_VOL_MINUTE_AWAY_INSP = 20816,
            NOM_CONC_AWAY_O2 = 20836,
            NOM_VENT_CONC_AWAY_O2_DELTA = 20840,
            NOM_VENT_CONC_AWAY_O2_EXP = 20844,
            NOM_VENT_AWAY_CO2_EXP = 20860,
            NOM_VENT_PRESS_AWAY_END_EXP_POS = 20904,
            NOM_VENT_VOL_AWAY_DEADSP = 20912,
            NOM_VENT_VOL_LUNG_TRAPD = 20920,
            NOM_VENT_CONC_AWAY_O2_INSP = 29848,
            NOM_VENT_FLOW_RATIO_PERF_ALV_INDEX = 20880,
            NOM_VENT_FLOW_INSP = 20876,
            NOM_VENT_CONC_AWAY_CO2_INSP = 20832,
            NOM_VENT_PRESS_OCCL = 20892,
            NOM_VENT_VOL_AWAY_DEADSP_REL = 20916,
            NOM_VENT_VOL_MINUTE_AWAY_MAND = 20940,
            NOM_COEF_GAS_TRAN = 20948,
            NOM_CONC_AWAY_DESFL = 20952,
            NOM_CONC_AWAY_ENFL = 20956,
            NOM_CONC_AWAY_HALOTH = 20960,
            NOM_CONC_AWAY_SEVOFL = 20964,
            NOM_CONC_AWAY_ISOFL = 20968,
            NOM_CONC_AWAY_N2O = 20976,
            NOM_CONC_AWAY_DESFL_ET = 21012,
            NOM_CONC_AWAY_ENFL_ET = 21016,
            NOM_CONC_AWAY_HALOTH_ET = 21020,
            NOM_CONC_AWAY_SEVOFL_ET = 21024,
            NOM_CONC_AWAY_ISOFL_ET = 21028,
            NOM_CONC_AWAY_N2O_ET = 21036,
            NOM_CONC_AWAY_DESFL_INSP = 21096,
            NOM_CONC_AWAY_ENFL_INSP = 21100,
            NOM_CONC_AWAY_HALOTH_INSP = 21104,
            NOM_CONC_AWAY_SEVOFL_INSP = 21108,
            NOM_CONC_AWAY_ISOFL_INSP = 21112,
            NOM_CONC_AWAY_N2O_INSP = 21120,
            NOM_CONC_AWAY_O2_INSP = 21124,
            NOM_VENT_TIME_PD_PPV = 21344,
            NOM_VENT_PRESS_RESP_PLAT = 21352,
            NOM_VENT_VOL_LEAK = 21360,
            NOM_VENT_VOL_LUNG_ALV = 21364,
            NOM_CONC_AWAY_O2_ET = 21368,
            NOM_CONC_AWAY_N2 = 21372,
            NOM_CONC_AWAY_N2_ET = 21376,
            NOM_CONC_AWAY_N2_INSP = 21380,
            NOM_CONC_AWAY_AGENT = 21384,
            NOM_CONC_AWAY_AGENT_ET = 21388,
            NOM_CONC_AWAY_AGENT_INSP = 21392,
            NOM_PRESS_CEREB_PERF = 22532,
            NOM_PRESS_INTRA_CRAN = 22536,
            NOM_PRESS_INTRA_CRAN_SYS = 22537,
            NOM_PRESS_INTRA_CRAN_DIA = 22538,
            NOM_PRESS_INTRA_CRAN_MEAN = 22539,
            NOM_SCORE_GLAS_COMA = 22656,
            NOM_SCORE_EYE_SUBSC_GLAS_COMA = 22658,
            NOM_SCORE_MOTOR_SUBSC_GLAS_COMA = 22659,
            NOM_SCORE_SUBSC_VERBAL_GLAS_COMA = 22660,
            NOM_CIRCUM_HEAD = 22784,
            NOM_TIME_PD_PUPIL_REACT_LEFT = 22820,
            NOM_TIME_PD_PUPIL_REACT_RIGHT = 22824,
            NOM_EEG_ELEC_POTL_CRTX = 22828,
            NOM_EMG_ELEC_POTL_MUSCL = 22844,
            NOM_EEG_FREQ_PWR_SPEC_CRTX_DOM_MEAN = 22908,
            NOM_EEG_FREQ_PWR_SPEC_CRTX_PEAK = 22916,
            NOM_EEG_FREQ_PWR_SPEC_CRTX_SPECTRAL_EDGE = 22920,
            NOM_EEG_PWR_SPEC_TOT = 22968,
            NOM_EEG_PWR_SPEC_ALPHA_REL = 22996,
            NOM_EEG_PWR_SPEC_BETA_REL = 23000,
            NOM_EEG_PWR_SPEC_DELTA_REL = 23004,
            NOM_EEG_PWR_SPEC_THETA_REL = 23008,
            NOM_FLOW_URINE_INSTANT = 26636,
            NOM_VOL_URINE_BAL_PD = 26660,
            NOM_VOL_URINE_COL = 26672,
            NOM_VOL_INFUS_ACTUAL_TOTAL = 26876,
            NOM_CONC_PH_ART = 28676,
            NOM_CONC_PCO2_ART = 28680,
            NOM_CONC_PO2_ART = 28684,
            NOM_CONC_HB_ART = 28692,
            NOM_CONC_HB_O2_ART = 28696,
            NOM_CONC_PO2_VEN = 28732,
            NOM_CONC_PH_VEN = 28724,
            NOM_CONC_PCO2_VEN = 28728,
            NOM_CONC_HB_O2_VEN = 28744,
            NOM_CONC_PH_URINE = 28772,
            NOM_CONC_NA_URINE = 28780,
            NOM_CONC_NA_SERUM = 28888,
            NOM_CONC_PH_GEN = 28932,
            NOM_CONC_HCO3_GEN = 28936,
            NOM_CONC_NA_GEN = 28940,
            NOM_CONC_K_GEN = 28944,
            NOM_CONC_GLU_GEN = 28948,
            NOM_CONC_CA_GEN = 28952,
            NOM_CONC_PCO2_GEN = 28992,
            NOM_CONC_CHLORIDE_GEN = 29032,
            NOM_BASE_EXCESS_BLD_ART = 29036,
            NOM_CONC_PO2_GEN = 29044,
            NOM_CONC_HCT_GEN = 29060,
            NOM_VENT_MODE_MAND_INTERMIT = 53290,
            NOM_TEMP_RECT = 57348,
            NOM_TEMP_BLD = 57364,
            NOM_TEMP_DIFF = 57368,
            NOM_METRIC_NOS = 61439,
            NOM_ECG_AMPL_ST_INDEX = 61501,
            NOM_TIME_TCUT_SENSOR = 61502,
            NOM_TEMP_TCUT_SENSOR = 61503,
            NOM_VOL_BLD_INTRA_THOR = 61504,
            NOM_VOL_BLD_INTRA_THOR_INDEX = 61505,
            NOM_VOL_LUNG_WATER_EXTRA_VASC = 61506,
            NOM_VOL_LUNG_WATER_EXTRA_VASC_INDEX = 61507,
            NOM_VOL_GLOBAL_END_DIA = 61508,
            NOM_VOL_GLOBAL_END_DIA_INDEX = 61509,
            NOM_CARD_FUNC_INDEX = 61510,
            NOM_OUTPUT_CARD_INDEX_CTS = 61511,
            NOM_VOL_BLD_STROKE_INDEX = 61512,
            NOM_VOL_BLD_STROKE_VAR = 61513,
            NOM_EEG_RATIO_SUPPRN = 61514,
            NOM_ELECTRODE_IMPED = 61515,
            NOM_EEG_BIS_SIG_QUAL_INDEX = 61517,
            NOM_EEG_BISPECTRAL_INDEX = 61518,
            NOM_GAS_TCUT = 61521,
            NOM_CONC_AWAY_SUM_MAC = 61533,
            NOM_CONC_AWAY_SUM_MAC_ET = 61534,
            NOM_CONC_AWAY_SUM_MAC_INSP = 61535,
            NOM_RES_VASC_PULM_INDEX = 61543,
            NOM_WK_CARD_LEFT_INDEX = 61544,
            NOM_WK_CARD_RIGHT_INDEX = 61545,
            NOM_SAT_O2_CONSUMP_INDEX = 61546,
            NOM_PRESS_AIR_AMBIENT = 61547,
            NOM_SAT_DIFF_O2_ART_VEN = 61548,
            NOM_SAT_O2_DELIVER = 61549,
            NOM_SAT_O2_DELIVER_INDEX = 61550,
            NOM_RATIO_SAT_O2_CONSUMP_DELIVER = 61551,
            NOM_RATIO_ART_VEN_SHUNT = 61552,
            NOM_AREA_BODY_SURFACE = 61553,
            NOM_INTENS_LIGHT = 61554,
            NOM_HEATING_PWR_TCUT_SENSOR = 61558,
            NOM_RATE_DIFF_CARD_BEAT_PULSE = 61560,
            NOM_VOL_INJ = 61561,
            NOM_VOL_THERMO_EXTRA_VASC_INDEX = 61562,
            NOM_NUM_CATHETER_CONST = 61564,
            NOM_PULS_OXIM_PERF_REL_LEFT = 61578,
            NOM_PULS_OXIM_PERF_REL_RIGHT = 61579,
            NOM_PULS_OXIM_PLETH_RIGHT = 61580,
            NOM_PULS_OXIM_PLETH_LEFT = 61581,
            NOM_CONC_BLD_UREA_NITROGEN = 61583,
            NOM_CONC_BASE_EXCESS_ECF = 61584,
            NOM_VENT_VOL_MINUTE_AWAY_SPONT = 61585,
            NOM_CONC_DIFF_HB_O2_ATR_VEN = 61586,
            NOM_PAT_WEIGHT = 61587,
            NOM_PAT_HEIGHT = 61588,
            NOM_CONC_AWAY_MAC = 61593,
            NOM_PULS_OXIM_PLETH_TELE = 61595,
            NOM_PULS_OXIM_SAT_O2_TELE = 61596,
            NOM_PULS_OXIM_PULS_RATE_TELE = 61597,
            NOM_PRESS_BLD_NONINV_TELE = 61600,
            NOM_PRESS_BLD_NONINV_TELE_SYS = 61601,
            NOM_PRESS_BLD_NONINV_TELE_DIA = 61602,
            NOM_PRESS_BLD_NONINV_TELE_MEAN = 61603,
            NOM_PRESS_GEN_1 = 61604,
            NOM_PRESS_GEN_1_SYS = 61605,
            NOM_PRESS_GEN_1_DIA = 61606,
            NOM_PRESS_GEN_1_MEAN = 61607,
            NOM_PRESS_GEN_2 = 61608,
            NOM_PRESS_GEN_2_SYS = 61609,
            NOM_PRESS_GEN_2_DIA = 61610,
            NOM_PRESS_GEN_2_MEAN = 61611,
            NOM_PRESS_GEN_3 = 61612,
            NOM_PRESS_GEN_3_SYS = 61613,
            NOM_PRESS_GEN_3_DIA = 61614,
            NOM_PRESS_GEN_3_MEAN = 61615,
            NOM_PRESS_GEN_4 = 61616,
            NOM_PRESS_GEN_4_SYS = 61617,
            NOM_PRESS_GEN_4_DIA = 61618,
            NOM_PRESS_GEN_4_MEAN = 61619,
            NOM_PRESS_INTRA_CRAN_1 = 61620,
            NOM_PRESS_INTRA_CRAN_1_SYS = 61621,
            NOM_PRESS_INTRA_CRAN_1_DIA = 61622,
            NOM_PRESS_INTRA_CRAN_1_MEAN = 61623,
            NOM_PRESS_INTRA_CRAN_2 = 61624,
            NOM_PRESS_INTRA_CRAN_2_SYS = 61625,
            NOM_PRESS_INTRA_CRAN_2_DIA = 61626,
            NOM_PRESS_INTRA_CRAN_2_MEAN = 61627,
            NOM_PRESS_BLD_ART_FEMORAL = 61628,
            NOM_PRESS_BLD_ART_FEMORAL_SYS = 61629,
            NOM_PRESS_BLD_ART_FEMORAL_DIA = 61630,
            NOM_PRESS_BLD_ART_FEMORAL_MEAN = 61631,
            NOM_PRESS_BLD_ART_BRACHIAL = 61632,
            NOM_PRESS_BLD_ART_BRACHIAL_SYS = 61633,
            NOM_PRESS_BLD_ART_BRACHIAL_DIA = 61634,
            NOM_PRESS_BLD_ART_BRACHIAL_MEAN = 61635,
            NOM_TEMP_VESICAL = 61636,
            NOM_TEMP_CEREBRAL = 61637,
            NOM_TEMP_AMBIENT = 61638,
            NOM_TEMP_GEN_1 = 61639,
            NOM_TEMP_GEN_2 = 61640,
            NOM_TEMP_GEN_3 = 61641,
            NOM_TEMP_GEN_4 = 61642,
            NOM_USOUND_CARD_BEAT_RATE_FETAL = 61643,
            NOM_USOUND_CARD_BEAT_RATE_FETAL_BTB = 61644,
            NOM_USOUND_CARD_BEAT_FETAL_SIG_QUAL_INDEX = 61645,
            NOM_ECG_CARD_BEAT_FETAL = 61646,
            NOM_ECG_CARD_BEAT_RATE_FETAL = 61647,
            NOM_ECG_CARD_BEAT_RATE_FETAL_BTB = 61648,
            NOM_ECG_CARD_BEAT_FETAL_SIG_QUAL_INDEX = 61649,
            NOM_TRIG_BEAT_FETAL = 61650,
            NOM_ECG_ELEC_POTL_FETAL = 61651,
            NOM_TOCO = 61652,
            NOM_STAT_COINCIDENCE = 61653,
            NOM_PRESS_INTRA_UTERAL = 61656,
            NOM_VOL_AWAY = 61663,
            NOM_VOL_AWAY_INSP_TIDAL = 61664,
            NOM_VOL_AWAY_EXP_TIDAL = 61665,
            NOM_AWAY_RESP_RATE_SPIRO = 61666,
            NOM_PULS_PRESS_VAR = 61667,
            NOM_PRESS_BLD_NONINV_PULS_RATE = 61669,
            NOM_RATIO_FETAL_MVMT_TOTAL = 61680,
            NOM_VENT_RESP_RATE_MAND = 61681,
            NOM_VENT_VOL_TIDAL_MAND = 61682,
            NOM_VENT_VOL_TIDAL_SPONT = 61683,
            NOM_CARDIAC_TROPONIN_I = 61684,
            NOM_CARDIO_PULMONARY_BYPASS_MODE = 61685,
            NOM_BNP = 61686,
            NOM_TIME_PD_RESP_PLAT = 61695,
            NOM_SAT_O2_VEN_CENT = 61696,
            NOM_SNR = 61697,
            NOM_HUMID = 61699,
            NOM_FRACT_EJECT = 61701,
            NOM_PERM_VASC_PULM_INDEX = 61702,
            NOM_TEMP_ORAL = 61704,
            NOM_TEMP_AXIL = 61708,
            NOM_TEMP_ORAL_PRED = 61712,
            NOM_TEMP_RECT_PRED = 61716,
            NOM_TEMP_AXIL_PRED = 61720,
            NOM_TEMP_AIR_INCUB = 61738,
            NOM_PULS_OXIM_PERF_REL_TELE = 61740,
            NOM_TEMP_PRED = 61760,
            NOM_SHUNT_RIGHT_LEFT = 61770,
            NOM_ECG_TIME_PD_QT_HEART_RATE = 61780,
            NOM_ECG_TIME_PD_QT_BASELINE = 61781,
            NOM_ECG_TIME_PD_QTc_DELTA = 61782,
            NOM_ECG_TIME_PD_QT_BASELINE_HEART_RATE = 61783,
            NOM_CONC_PH_CAP = 61784,
            NOM_CONC_PCO2_CAP = 61785,
            NOM_CONC_PO2_CAP = 61786,
            NOM_SAT_O2_CAP = 61793,
            NOM_CONC_MG_ION = 61787,
            NOM_CONC_MG_SER = 61788,
            NOM_CONC_tCA_SER = 61789,
            NOM_CONC_P_SER = 61790,
            NOM_CONC_CHLOR_SER = 61791,
            NOM_CONC_FE_GEN = 61792,
            NOM_CONC_AN_GAP = 61794,
            NOM_CONC_AN_GAP_CALC = 61857,
            NOM_CONC_ALB_SER = 61795,
            NOM_SAT_O2_ART_CALC = 61796,
            NOM_SAT_O2_VEN_CALC = 61798,
            NOM_SAT_O2_CAP_CALC = 61856,
            NOM_CONC_HB_CO_GEN = 29056,
            NOM_CONC_HB_FETAL = 61797,
            NOM_CONC_HB_MET_GEN = 29052,
            NOM_PLTS_CNT = 61799,
            NOM_WB_CNT = 61800,
            NOM_RB_CNT = 61801,
            NOM_RET_CNT = 61802,
            NOM_PLASMA_OSM = 61803,
            NOM_CONC_CREA_CLR = 61804,
            NOM_NSLOSS = 61805,
            NOM_CONC_CHOLESTEROL = 61806,
            NOM_CONC_TGL = 61807,
            NOM_CONC_HDL = 61808,
            NOM_CONC_LDL = 61809,
            NOM_CONC_UREA_GEN = 61810,
            NOM_CONC_CREA = 61811,
            NOM_CONC_LACT = 61812,
            NOM_CONC_BILI_TOT = 61815,
            NOM_CONC_PROT_SER = 61816,
            NOM_CONC_PROT_TOT = 61817,
            NOM_CONC_BILI_DIRECT = 61818,
            NOM_CONC_LDH = 61819,
            NOM_ES_RATE = 61820,
            NOM_CONC_PCT = 61821,
            NOM_CONC_CREA_KIN_MM = 61823,
            NOM_CONC_CREA_KIN_SER = 61824,
            NOM_CONC_CREA_KIN_MB = 61825,
            NOM_CONC_CHE = 61826,
            NOM_CONC_CRP = 61827,
            NOM_CONC_AST = 61828,
            NOM_CONC_AP = 61829,
            NOM_CONC_ALPHA_AMYLASE = 61830,
            NOM_CONC_GPT = 61831,
            NOM_CONC_GOT = 61832,
            NOM_CONC_GGT = 61833,
            NOM_TIME_PD_ACT = 61834,
            NOM_TIME_PD_PT = 61835,
            NOM_PT_INTL_NORM_RATIO = 61836,
            NOM_TIME_PD_aPTT_WB = 61837,
            NOM_TIME_PD_aPTT_PE = 61838,
            NOM_TIME_PD_PT_WB = 61839,
            NOM_TIME_PD_PT_PE = 61840,
            NOM_TIME_PD_THROMBIN = 61841,
            NOM_TIME_PD_COAGULATION = 61842,
            NOM_TIME_PD_THROMBOPLAS = 61843,
            NOM_FRACT_EXCR_NA = 61844,
            NOM_CONC_UREA_URINE = 61845,
            NOM_CONC_CREA_URINE = 61846,
            NOM_CONC_K_URINE = 61847,
            NOM_CONC_K_URINE_EXCR = 61848,
            NOM_CONC_OSM_URINE = 61849,
            NOM_CONC_GLU_URINE = 61855,
            NOM_CONC_CHLOR_URINE = 61850,
            NOM_CONC_PRO_URINE = 61851,
            NOM_CONC_CA_URINE = 61852,
            NOM_FLUID_DENS_URINE = 61853,
            NOM_CONC_HB_URINE = 61854,
            NOM_ENERGY_BAL = 61861,
            NOM_PULS_OXIM_SAT_O2_PRE_DUCTAL = 61888,
            NOM_PULS_OXIM_PERF_REL_PRE_DUCTAL = 61996,
            NOM_PULS_OXIM_SAT_O2_POST_DUCTAL = 61908,
            NOM_PULS_OXIM_PERF_REL_POST_DUCTAL = 61916,
            NOM_PRESS_GEN_5 = 62452,
            NOM_PRESS_GEN_5_SYS = 62453,
            NOM_PRESS_GEN_5_DIA = 62454,
            NOM_PRESS_GEN_5_MEAN = 62455,
            NOM_PRESS_GEN_6 = 62456,
            NOM_PRESS_GEN_6_SYS = 62457,
            NOM_PRESS_GEN_6_DIA = 62458,
            NOM_PRESS_GEN_6_MEAN = 62459,
            NOM_PRESS_GEN_7 = 62460,
            NOM_PRESS_GEN_7_SYS = 62461,
            NOM_PRESS_GEN_7_DIA = 62462,
            NOM_PRESS_GEN_7_MEAN = 62463,
            NOM_PRESS_GEN_8 = 62464,
            NOM_PRESS_GEN_8_SYS = 62465,
            NOM_PRESS_GEN_8_DIA = 62466,
            NOM_PRESS_GEN_8_MEAN = 62467,
            NOM_ECG_AMPL_ST_BASELINE_I = 62481,
            NOM_ECG_AMPL_ST_BASELINE_II = 62482,
            NOM_ECG_AMPL_ST_BASELINE_V1 = 62483,
            NOM_ECG_AMPL_ST_BASELINE_V2 = 62484,
            NOM_ECG_AMPL_ST_BASELINE_V3 = 62485,
            NOM_ECG_AMPL_ST_BASELINE_V4 = 62486,
            NOM_ECG_AMPL_ST_BASELINE_V5 = 62487,
            NOM_ECG_AMPL_ST_BASELINE_V6 = 62488,
            NOM_ECG_AMPL_ST_BASELINE_III = 62541,
            NOM_ECG_AMPL_ST_BASELINE_AVR = 62542,
            NOM_ECG_AMPL_ST_BASELINE_AVL = 62543,
            NOM_ECG_AMPL_ST_BASELINE_AVF = 62544,
            NOM_AGE = 63504,
            NOM_AGE_GEST = 63505,
            NOM_AWAY_CORR_COEF = 63508,
            NOM_AWAY_RESP_RATE_SPONT = 63509,
            NOM_AWAY_TC = 63510,
            NOM_BIRTH_LENGTH = 63512,
            NOM_BREATH_RAPID_SHALLOW_INDEX = 63513,
            NOM_C20_PER_C_INDEX = 63514,
            NOM_CARD_CONTRACT_HEATHER_INDEX = 63516,
            NOM_CONC_ALP = 63517,
            NOM_CONC_CA_GEN_NORM = 63522,
            NOM_CONC_CA_SER = 63524,
            NOM_CONC_CO2_TOT = 63525,
            NOM_CONC_CO2_TOT_CALC = 63526,
            NOM_CONC_CREA_SER = 63527,
            NOM_RESP_RATE_SPONT = 63528,
            NOM_CONC_GLO_SER = 63529,
            NOM_CONC_GLU_SER = 63530,
            NOM_CONC_HB_CORP_MEAN = 63532,
            NOM_CONC_K_SER = 63535,
            NOM_CONC_NA_EXCR = 63536,
            NOM_CONC_PCO2_ART_ADJ = 63538,
            NOM_CONC_PCO2_CAP_ADJ = 63539,
            NOM_CONC_PH_CAP_ADJ = 63543,
            NOM_CONC_PH_GEN_ADJ = 63544,
            NOM_CONC_PO2_ART_ADJ = 63547,
            NOM_CONC_PO2_CAP_ADJ = 63548,
            NOM_CREA_OSM = 63551,
            NOM_EEG_BURST_SUPPRN_INDEX = 63552,
            NOM_EEG_ELEC_POTL_CRTX_GAIN_LEFT = 63553,
            NOM_EEG_ELEC_POTL_CRTX_GAIN_RIGHT = 63554,
            NOM_EEG_FREQ_PWR_SPEC_CRTX_MEDIAN_LEFT = 63563,
            NOM_EEG_FREQ_PWR_SPEC_CRTX_MEDIAN_RIGHT = 63564,
            NOM_EEG_PWR_SPEC_ALPHA_ABS_LEFT = 63573,
            NOM_EEG_PWR_SPEC_ALPHA_ABS_RIGHT = 63574,
            NOM_EEG_PWR_SPEC_BETA_ABS_LEFT = 63579,
            NOM_EEG_PWR_SPEC_BETA_ABS_RIGHT = 63580,
            NOM_EEG_PWR_SPEC_DELTA_ABS_LEFT = 63587,
            NOM_EEG_PWR_SPEC_DELTA_ABS_RIGHT = 63588,
            NOM_EEG_PWR_SPEC_THETA_ABS_LEFT = 63593,
            NOM_EEG_PWR_SPEC_THETA_ABS_RIGHT = 63594,
            NOM_ELEC_EVOK_POTL_CRTX_ACOUSTIC_AAI = 63603,
            NOM_EXTRACT_O2_INDEX = 63605,
            NOM_FLOW_AWAY_AIR = 63607,
            NOM_FLOW_AWAY_EXP_ET = 63610,
            NOM_FLOW_AWAY_MAX_SPONT = 63613,
            NOM_FLOW_AWAY_TOT = 63617,
            NOM_FLOW_CO2_PROD_RESP_TIDAL = 63618,
            NOM_FLOW_URINE_PREV_24HR = 63619,
            NOM_FREE_WATER_CLR = 63620,
            NOM_HB_CORP_MEAN = 63621,
            NOM_HEATING_PWR_INCUBATOR = 63622,
            NOM_OUTPUT_CARD_INDEX_ACCEL = 63625,
            NOM_PTC_CNT = 63627,
            NOM_PULS_OXIM_PLETH_GAIN = 63629,
            NOM_RATIO_AWAY_RATE_VOL_AWAY = 63630,
            NOM_RATIO_BUN_CREA = 63631,
            NOM_RATIO_CONC_BLD_UREA_NITROGEN_CREA_CALC = 63632,
            NOM_RATIO_CONC_URINE_CREA_CALC = 63633,
            NOM_RATIO_CONC_URINE_CREA_SER = 63634,
            NOM_RATIO_CONC_URINE_NA_K = 63635,
            NOM_RATIO_PaO2_FIO2 = 63636,
            NOM_RATIO_TIME_PD_PT = 63637,
            NOM_RATIO_TIME_PD_PTT = 63638,
            NOM_RATIO_TRAIN_OF_FOUR = 63639,
            NOM_RATIO_URINE_SER_OSM = 63640,
            NOM_RES_AWAY_DYN = 63641,
            NOM_RESP_BREATH_ASSIST_CNT = 63642,
            NOM_RIGHT_HEART_FRACT_EJECT = 63643,
            NOM_TIME_PD_EVOK_REMAIN = 63648,
            NOM_TIME_PD_EXP = 63649,
            NOM_TIME_PD_FROM_LAST_MSMT = 63650,
            NOM_TIME_PD_INSP = 63651,
            NOM_TIME_PD_KAOLIN_CEPHALINE = 63652,
            NOM_TIME_PD_PTT = 63653,
            NOM_TRAIN_OF_FOUR_1 = 63655,
            NOM_TRAIN_OF_FOUR_2 = 63656,
            NOM_TRAIN_OF_FOUR_3 = 63657,
            NOM_TRAIN_OF_FOUR_4 = 63658,
            NOM_TRAIN_OF_FOUR_CNT = 63659,
            NOM_TWITCH_AMPL = 63660,
            NOM_UREA_SER = 63661,
            NOM_VENT_ACTIVE = 63664,
            NOM_VENT_AMPL_HFV = 63665,
            NOM_VENT_CONC_AWAY_AGENT_DELTA = 63666,
            NOM_VENT_CONC_AWAY_DESFL_DELTA = 63667,
            NOM_VENT_CONC_AWAY_ENFL_DELTA = 63668,
            NOM_VENT_CONC_AWAY_HALOTH_DELTA = 63669,
            NOM_VENT_CONC_AWAY_ISOFL_DELTA = 63670,
            NOM_VENT_CONC_AWAY_N2O_DELTA = 63671,
            NOM_VENT_CONC_AWAY_O2_CIRCUIT = 63672,
            NOM_VENT_CONC_AWAY_SEVOFL_DELTA = 63673,
            NOM_VENT_PRESS_AWAY_END_EXP_POS_LIMIT_LO = 63674,
            NOM_VENT_PRESS_AWAY_PV = 63676,
            NOM_VENT_TIME_PD_RAMP = 63677,
            NOM_VENT_VOL_AWAY_INSP_TIDAL_HFV = 63678,
            NOM_VENT_VOL_TIDAL_HFV = 63679,
            NOM_VOL_AWAY_EXP_TIDAL_SPONT = 63682,
            NOM_VOL_AWAY_TIDAL_PSV = 63683,
            NOM_VOL_CORP_MEAN = 63684,
            NOM_VOL_FLUID_THORAC = 63685,
            NOM_VOL_FLUID_THORAC_INDEX = 63686,
            NOM_VOL_LVL_LIQUID_BOTTLE_AGENT = 63687,
            NOM_VOL_LVL_LIQUID_BOTTLE_DESFL = 63688,
            NOM_VOL_LVL_LIQUID_BOTTLE_ENFL = 63689,
            NOM_VOL_LVL_LIQUID_BOTTLE_HALOTH = 63690,
            NOM_VOL_LVL_LIQUID_BOTTLE_ISOFL = 63691,
            NOM_VOL_LVL_LIQUID_BOTTLE_SEVOFL = 63692,
            NOM_VOL_MINUTE_AWAY_INSP_HFV = 63693,
            NOM_VOL_URINE_BAL_PD_INSTANT = 63694,
            NOM_VOL_URINE_SHIFT = 63695,
            NOM_VOL_VENT_L_END_SYS_INDEX = 63697,
            NOM_WEIGHT_URINE_COL = 63699,
            NOM_SAT_O2_TISSUE = 63840,
            NOM_CEREB_STATE_INDEX = 63841,
            NOM_SAT_O2_GEN_1 = 63842,
            NOM_SAT_O2_GEN_2 = 63843,
            NOM_SAT_O2_GEN_3 = 63844,
            NOM_SAT_O2_GEN_4 = 63845,
            NOM_TEMP_CORE_GEN_1 = 63846,
            NOM_TEMP_CORE_GEN_2 = 63847,
            NOM_PRESS_BLD_DIFF = 63848,
            NOM_PRESS_BLD_DIFF_GEN_1 = 63852,
            NOM_PRESS_BLD_DIFF_GEN_2 = 63856,
            NOM_FLOW_PUMP_HEART_LUNG_MAIN = 63860,
            NOM_FLOW_PUMP_HEART_LUNG_SLAVE = 63861,
            NOM_FLOW_PUMP_HEART_LUNG_SUCTION = 63862,
            NOM_FLOW_PUMP_HEART_LUNG_AUX = 63863,
            NOM_FLOW_PUMP_HEART_LUNG_CARDIOPLEGIA_MAIN = 63864,
            NOM_FLOW_PUMP_HEART_LUNG_CARDIOPLEGIA_SLAVE = 63865,
            NOM_TIME_PD_PUMP_HEART_LUNG_AUX_SINCE_START = 63866,
            NOM_TIME_PD_PUMP_HEART_LUNG_AUX_SINCE_STOP = 63867,
            NOM_VOL_DELIV_PUMP_HEART_LUNG_AUX = 63868,
            NOM_VOL_DELIV_TOTAL_PUMP_HEART_LUNG_AUX = 63869,
            NOM_TIME_PD_PLEGIA_PUMP_HEART_LUNG_AUX = 63870,
            NOM_TIME_PD_PUMP_HEART_LUNG_CARDIOPLEGIA_MAIN_SINCE_START = 63871,
            NOM_TIME_PD_PUMP_HEART_LUNG_CARDIOPLEGIA_MAIN_SINCE_STOP = 63872,
            NOM_VOL_DELIV_PUMP_HEART_LUNG_CARDIOPLEGIA_MAIN = 63873,
            NOM_VOL_DELIV_TOTAL_PUMP_HEART_LUNG_CARDIOPLEGIA_MAIN = 63874,
            NOM_TIME_PD_PLEGIA_PUMP_HEART_LUNG_CARDIOPLEGIA_MAIN = 63875,
            NOM_TIME_PD_PUMP_HEART_LUNG_CARDIOPLEGIA_SLAVE_SINCE_START = 63876,
            NOM_TIME_PD_PUMP_HEART_LUNG_CARDIOPLEGIA_SLAVE_SINCE_STOP = 63877,
            NOM_VOL_DELIV_PUMP_HEART_LUNG_CARDIOPLEGIA_SLAVE = 63878,
            NOM_VOL_DELIV_TOTAL_PUMP_HEART_LUNG_CARDIOPLEGIA_SLAVE = 63879,
            NOM_TIME_PD_PLEGIA_PUMP_HEART_LUNG_CARDIOPLEGIA_SLAVE = 63880,
            NOM_RATIO_INSP_TOTAL_BREATH_SPONT = 63888,
            NOM_VENT_PRESS_AWAY_END_EXP_POS_TOTAL = 63889,
            NOM_COMPL_LUNG_PAV = 63890,
            NOM_RES_AWAY_PAV = 63891,
            NOM_RES_AWAY_EXP_TOTAL = 63892,
            NOM_ELAS_LUNG_PAV = 63893,
            NOM_BREATH_RAPID_SHALLOW_INDEX_NORM = 63894
        };
        enum class AlarmCodes : uint16_t
        {
            NOM_EVT_ABSENT = 4,
            NOM_EVT_CONTAM = 14,
            NOM_EVT_DISCONN = 22,
            NOM_EVT_DISTURB = 24,
            NOM_EVT_EMPTY = 26,
            NOM_EVT_ERRATIC = 32,
            NOM_EVT_EXH = 36,
            NOM_EVT_FAIL = 38,
            NOM_EVT_HI = 40,
            NOM_EVT_IRREG = 58,
            NOM_EVT_LO = 62,
            NOM_EVT_MALF = 70,
            NOM_EVT_NOISY = 74,
            NOM_EVT_OBSTRUC = 80,
            NOM_EVT_REVERSED = 96,
            NOM_EVT_SUST = 106,
            NOM_EVT_UNAVAIL = 110,
            NOM_EVT_UNDEF = 112,
            NOM_EVT_WARMING = 124,
            NOM_EVT_WEAK = 128,
            NOM_EVT_BREATH_ABSENT = 136,
            NOM_EVT_CALIB_FAIL = 138,
            NOM_EVT_CONFIG_ERR = 142,
            NOM_EVT_RANGE_ERR = 164,
            NOM_EVT_RANGE_OVER = 166,
            NOM_EVT_SRC_ABSENT = 174,
            NOM_EVT_SYNCH_ERR = 182,
            NOM_EVT_BATT_LO = 194,
            NOM_EVT_BATT_PROB = 198,
            NOM_EVT_CUFF_NOT_DEFLATED = 230,
            NOM_EVT_CUFF_INFLAT_OVER = 232,
            NOM_EVT_EQUIP_MALF = 242,
            NOM_EVT_TUBE_OCCL = 250,
            NOM_EVT_GAS_AGENT_IDENT_MALF = 258,
            NOM_EVT_LEAD_DISCONN = 268,
            NOM_EVT_LEADS_OFF = 274,
            NOM_EVT_O2_SUPPLY_LO = 296,
            NOM_EVT_OPTIC_MODULE_ABSENT = 298,
            NOM_EVT_OPTIC_MODULE_DEFECT = 300,
            NOM_EVT_SENSOR_DISCONN = 308,
            NOM_EVT_SENSOR_MALF = 310,
            NOM_EVT_SENSOR_PROB = 312,
            NOM_EVT_SW_VER_UNK = 322,
            NOM_EVT_TUBE_DISCONN = 326,
            NOM_EVT_TUBE_OBSTRUC = 330,
            NOM_EVT_XDUCR_DISCONN = 336,
            NOM_EVT_XDUCR_MALF = 338,
            NOM_EVT_INTENS_LIGHT_ERR = 350,
            NOM_EVT_MSMT_DISCONN = 352,
            NOM_EVT_MSMT_ERR = 354,
            NOM_EVT_MSMT_FAIL = 356,
            NOM_EVT_MSMT_INOP = 358,
            NOM_EVT_MSMT_INTERRUP = 362,
            NOM_EVT_MSMT_RANGE_OVER = 364,
            NOM_EVT_MSMT_RANGE_UNDER = 366,
            NOM_EVT_SIG_LO = 380,
            NOM_EVT_SIG_UNANALYZEABLE = 384,
            NOM_EVT_TEMP_HI_GT_LIM = 394,
            NOM_EVT_UNSUPPORTED = 400,
            NOM_EVT_WAVE_ARTIF_ERR = 432,
            NOM_EVT_WAVE_SIG_QUAL_ERR = 434,
            NOM_EVT_MSMT_INTERF_ERR = 436,
            NOM_EVT_WAVE_OSCIL_ABSENT = 442,
            NOM_EVT_VOLTAGE_OUT_OF_RANGE = 460,
            NOM_EVT_INCOMPAT = 600,
            NOM_EVT_ADVIS_CHK = 6658,
            NOM_EVT_ADVIS_CALIB_AND_ZERO_CHK = 6664,
            NOM_EVT_ADVIS_CONFIG_CHK = 6666,
            NOM_EVT_ADVIS_SETTINGS_CHK = 6668,
            NOM_EVT_ADVIS_SETUP_CHK = 6670,
            NOM_EVT_ADVIS_SRC_CHK = 6672,
            NOM_EVT_BATT_COND = 6676,
            NOM_EVT_BATT_REPLACE = 6678,
            NOM_EVT_ADVIS_CABLE_CHK = 6680,
            NOM_EVT_ADVIS_GAS_AGENT_CHK = 6688,
            NOM_EVT_ADVIS_LEAD_CHK = 6690,
            NOM_EVT_ADVIS_SENSOR_CHK = 6696,
            NOM_EVT_ADVIS_GAIN_DECR = 6704,
            NOM_EVT_ADVIS_GAIN_INCR = 6706,
            NOM_EVT_ADVIS_UNIT_CHK = 6710,
            NOM_EVT_APNEA = 3072,
            NOM_EVT_ECG_ASYSTOLE = 3076,
            NOM_EVT_ECG_BEAT_MISSED = 3078,
            NOM_EVT_ECG_BIGEM = 3082,
            NOM_EVT_ECG_BRADY_EXTREME = 3086,
            NOM_EVT_ECG_PACING_NON_CAPT = 3102,
            NOM_EVT_ECG_PAUSE = 3108,
            NOM_EVT_ECG_TACHY_EXTREME = 3122,
            NOM_EVT_ECG_CARD_BEAT_RATE_IRREG = 3158,
            NOM_EVT_ECG_PACER_NOT_PACING = 3182,
            NOM_EVT_ECG_SV_TACHY = 3192,
            NOM_EVT_ECG_V_P_C_RonT = 3206,
            NOM_EVT_ECG_V_P_C_MULTIFORM = 3208,
            NOM_EVT_ECG_V_P_C_PAIR = 3210,
            NOM_EVT_ECG_V_P_C_RUN = 3212,
            NOM_EVT_ECG_V_RHY = 3220,
            NOM_EVT_ECG_V_TACHY = 3224,
            NOM_EVT_ECG_V_TACHY_NON_SUST = 3226,
            NOM_EVT_ECG_V_TRIGEM = 3236,
            NOM_EVT_DESAT = 3246,
            NOM_EVT_ECG_V_P_C_RATE = 3252,
            NOM_EVT_STAT_AL_OFF = 6144,
            NOM_EVT_STAT_BATT_CHARGING = 6150,
            NOM_EVT_STAT_CALIB_MODE = 6152,
            NOM_EVT_STAT_CALIB_RUNNING = 6154,
            NOM_EVT_STAT_CALIB_INVIVO_RUNNING = 6156,
            NOM_EVT_STAT_CALIB_LIGHT_RUNNING = 6158,
            NOM_EVT_STAT_CALIB_PREINS_RUNNING = 6160,
            NOM_EVT_STAT_SELFTEST_RUNNING = 6164,
            NOM_EVT_STAT_ZERO_RUNNING = 6170,
            NOM_EVT_STAT_OPT_MOD_SENSOR_CONN = 6172,
            NOM_EVT_STAT_OPT_MOD_SENSOR_WARMING = 6174,
            NOM_EVT_STAT_SENSOR_WARMING = 6176,
            NOM_EVT_STAT_WARMING = 6178,
            NOM_EVT_STAT_ECG_AL_ALL_OFF = 6182,
            NOM_EVT_STAT_ECG_AL_SOME_OFF = 6184,
            NOM_EVT_STAT_LEARN = 6224,
            NOM_EVT_STAT_OFF = 6226,
            NOM_EVT_STAT_STANDBY = 6228,
            NOM_EVT_STAT_DISCONN = 6256,
            NOM_EVT_ADVIS_CALIB_REQD = 6662,
            NOM_EVT_ECG_V_FIB_TACHY = 61444,
            NOM_EVT_WAIT_CAL = 61678,
            NOM_EVT_ADVIS_CHANGE_SITE = 61682,
            NOM_EVT_ADVIS_CHECK_SITE_TIME = 61684,
            NOM_EVT_STAT_FW_UPDATE_IN_PROGRESS = 61688,
            NOM_EVT_EXT_DEV_AL_CODE_1 = 61690,
            NOM_EVT_EXT_DEV_AL_CODE_2 = 61692,
            NOM_EVT_EXT_DEV_AL_CODE_3 = 61694,
            NOM_EVT_EXT_DEV_AL_CODE_4 = 61696,
            NOM_EVT_EXT_DEV_AL_CODE_5 = 61698,
            NOM_EVT_EXT_DEV_AL_CODE_6 = 61700,
            NOM_EVT_EXT_DEV_AL_CODE_7 = 61702,
            NOM_EVT_EXT_DEV_AL_CODE_8 = 61704,
            NOM_EVT_EXT_DEV_AL_CODE_9 = 61706,
            NOM_EVT_EXT_DEV_AL_CODE_10 = 61708,
            NOM_EVT_EXT_DEV_AL_CODE_11 = 61710,
            NOM_EVT_EXT_DEV_AL_CODE_12 = 61712,
            NOM_EVT_EXT_DEV_AL_CODE_13 = 61714,
            NOM_EVT_EXT_DEV_AL_CODE_14 = 61716,
            NOM_EVT_EXT_DEV_AL_CODE_15 = 61718,
            NOM_EVT_EXT_DEV_AL_CODE_16 = 61720,
            NOM_EVT_EXT_DEV_AL_CODE_17 = 61722,
            NOM_EVT_EXT_DEV_AL_CODE_18 = 61724,
            NOM_EVT_EXT_DEV_AL_CODE_19 = 61726,
            NOM_EVT_EXT_DEV_AL_CODE_20 = 61728,
            NOM_EVT_EXT_DEV_AL_CODE_21 = 61730,
            NOM_EVT_EXT_DEV_AL_CODE_22 = 61732,
            NOM_EVT_EXT_DEV_AL_CODE_23 = 61734,
            NOM_EVT_EXT_DEV_AL_CODE_24 = 61736,
            NOM_EVT_EXT_DEV_AL_CODE_25 = 61738,
            NOM_EVT_EXT_DEV_AL_CODE_26 = 61740,
            NOM_EVT_EXT_DEV_AL_CODE_27 = 61742,
            NOM_EVT_EXT_DEV_AL_CODE_28 = 61744,
            NOM_EVT_EXT_DEV_AL_CODE_29 = 61746,
            NOM_EVT_EXT_DEV_AL_CODE_30 = 61748,
            NOM_EVT_EXT_DEV_AL_CODE_31 = 61750,
            NOM_EVT_EXT_DEV_AL_CODE_32 = 61752,
            NOM_EVT_EXT_DEV_AL_CODE_33 = 61754,
            NOM_EVT_ST_MULTI = 61756,
            NOM_EVT_ADVIS_BSA_REQD = 61760,
            NOM_EVT_ADVIS_PRESUMED_CVP = 61762,
            NOM_EVT_MSMT_UNSUPPORTED = 61764,
            NOM_EVT_BRADY = 61766,
            NOM_EVT_TACHY = 61768,
            NOM_EVT_ADVIS_CHANGE_SCALE = 61770,
            NOM_EVT_MSMT_RESTART = 61772,
            NOM_EVT_TOO_MANY_AGENTS = 61774,
            NOM_EVT_STAT_PULSE_SRC_RANGE_OVER = 61778,
            NOM_EVT_STAT_PRESS_SRC_RANGE_OVER = 61780,
            NOM_EVT_MUSCLE_NOISE = 61782,
            NOM_EVT_LINE_NOISE = 61784,
            NOM_EVT_IMPED_HI = 61786,
            NOM_EVT_AGENT_MIX = 61788,
            NOM_EVT_IMPEDS_HI = 61790,
            NOM_EVT_ADVIS_PWR_HI = 61792,
            NOM_EVT_ADVIS_PWR_OFF = 61794,
            NOM_EVT_ADVIS_PWR_OVER = 61796,
            NOM_EVT_ADVIS_DEACT = 61798,
            NOM_EVT_CO_WARNING = 61800,
            NOM_EVT_ADVIS_NURSE_CALL = 61802,
            NOM_EVT_COMP_MALF = 61804,
            NOM_EVT_AGENT_MEAS_MALF = 61806,
            NOM_EVT_ADVIS_WATER_TRAP_CHK = 61808,
            NOM_EVT_STAT_AGENT_CALC_RUNNING = 61810,
            NOM_EVT_ADVIS_ADAPTER_CHK = 61814,
            NOM_EVT_ADVIS_PUMP_OFF = 61816,
            NOM_EVT_ZERO_FAIL = 61818,
            NOM_EVT_ADVIS_ZERO_REQD = 61820,
            NOM_EVT_EXTR_HI = 61830,
            NOM_EVT_EXTR_LO = 61832,
            NOM_EVT_LEAD_DISCONN_YELLOW = 61833,
            NOM_EVT_LEAD_DISCONN_RED = 61834,
            NOM_EVT_CUFF_INFLAT_OVER_YELLOW = 61835,
            NOM_EVT_CUFF_INFLAT_OVER_RED = 61836,
            NOM_EVT_CUFF_NOT_DEFLATED_YELLOW = 61837,
            NOM_EVT_CUFF_NOT_DEFLATED_RED = 61838,
            NOM_EVT_ADVIS_ACTION_REQD = 61840,
            NOM_EVT_OUT_OF_AREA = 61842,
            NOM_EVT_LEADS_DISCONN = 61844,
            NOM_EVT_DEV_ASSOC_CHK = 61846,
            NOM_EVT_SYNCH_UNSUPPORTED = 61848,
            NOM_EVT_ECG_ADVIS_SRC_CHK = 61850,
            NOM_EVT_ALARM_TECH = 61852,
            NOM_EVT_ALARM_TECH_YELLOW = 61854,
            NOM_EVT_ALARM_TECH_RED = 61856,
            NOM_EVT_ALARM_MED_YELLOW_SHORT = 61858,
            NOM_EVT_ALARM_MED_YELLOW = 61860,
            NOM_EVT_ALARM_MED_RED = 61862,
            NOM_EVT_TELE_EQUIP_MALF = 61874,
            NOM_EVT_SYNCH_ERR_ECG = 61876,
            NOM_EVT_SYNCH_ERR_SPO2T = 61878,
            NOM_EVT_ADVIS_ACTION_REQD_YELLOW = 61880,
            NOM_EVT_ADVIS_NBP_SEQ_COMPLETED = 61882,
            NOM_EVT_PACER_OUTPUT_LO = 61884,
            NOM_EVT_ALARM_MORE_TECH = 61886,
            NOM_EVT_ALARM_MORE_TECH_YELLOW = 61888,
            NOM_EVT_ALARM_MORE_TECH_RED = 61890,
            NOM_EVT_ADVIS_PATIENT_CONFLICT = 61892,
            NOM_EVT_SENSOR_REPLACE = 61894,
            NOM_EVT_ECG_ATR_FIB = 61896,
            NOM_EVT_LIMITED_CONNECTIVITY = 61900,
            NOM_EVT_DISABLED = 61924,
            NOM_EVT_ECG_ABSENT = 61926,
            NOM_EVT_SRR_INTERF = 61928,
            NOM_EVT_SRR_INVALID_CHAN = 61930,
            NOM_EVT_EXT_DEV_DEMO = 62032,
            NOM_EVT_EXT_DEV_MONITORING = 62034
        };

struct GlbHandle
    {
		
         unsigned short context_id;
         unsigned short handle;
    };

	 struct ManagedObjectId
    {
		
         unsigned short m_obj_class;
         GlbHandle m_obj_inst;
    };

	struct NumericValResult
	{
	std::string Timestamp;
	std::string Relativetimestamp;
	std::string SystemLocalTime;
	std::string PhysioID;
	std::string Value;
	std::string DeviceID;
	};

   struct ActionResult
    {
		
         ManagedObjectId objectid;
         unsigned short action_type;
         unsigned short length;

    };

	 struct ScaleRangeSpec16
    {
		
         double lower_absolute_value=-0.13371337;
         double upper_absolute_value;
         unsigned short lower_scaled_value;
         unsigned short upper_scaled_value;
         unsigned short obpoll_handle;
         unsigned short physio_id;
    };

 	struct Ava
    {
		
         unsigned short attribute_id;
         unsigned short length;
         unsigned short attribute_val; //placeholder
    };

	 struct AttributeList
    {
		
         unsigned short count;
         unsigned short length;
        // Ava [] value = new Ava[1];
         Ava value1; //null placeholder
         char* avaobjectsarray;
    };

	 struct SessionHeader
    {
		
         std::byte type;
         std::byte length;
         unsigned short length2;
    };

	struct ObservationPoll
    {
		
         unsigned short obj_handle;
         AttributeList attributes;
         char* avaobjectsarray;
    };

    struct SingleContextPoll
    {
		unsigned short context_id;
        unsigned short count;
        unsigned short length;
        // ObservationPoll [] value = new ObservationPoll[1];
        ObservationPoll value1; //null placeholder
        char* obpollobjectsarray;
    };
    struct PollInfoList
    {
		unsigned short count;
        unsigned short length;
        // SingleContextPoll [] value= new SingleContextPoll[1];
        SingleContextPoll value1; //null placeholder
        char* scpollarray;
    };

	struct SaSpec
    {
		
         unsigned short array_size = 0;
         std::byte sample_size;
         std::byte significant_bits;
         unsigned short SaFlags;
         unsigned short obpoll_handle;
    };

	struct SaCalibData16
    {
		
         double lower_absolute_value;
         double upper_absolute_value;
         unsigned short lower_scaled_value;
         unsigned short upper_scaled_value;
         unsigned short increment;
         unsigned short cal_type;
         unsigned short obpoll_handle;
         unsigned short physio_id;
    };

    struct  WaveValResult
	{
		
		std::string Timestamp;
		std::string Relativetimestamp;
		std::string SystemLocalTime;
		std::string PhysioID;
		uint8_t Value;
		std::string DeviceID;
		uint16_t obpoll_handle;
		SaSpec saSpecData;
		SaCalibData16 saCalibData;
		ScaleRangeSpec16 ScaleRangeSpec16;
	};


    struct RawFrameHdr
    {
		
         std::byte protocol_id;
         std::byte msg_type;
         unsigned short length;
    };

     struct ROapdus
    {
		
         unsigned short ro_type;
         unsigned short length;
    };

     struct RORSapdu
    {
		
         unsigned short invoke_id;
         unsigned short command_type;
         unsigned short length;
    };

     struct ROIVapdu
    {
		
         unsigned short inovke_id;
         unsigned short command_type;
         unsigned short length;
    };

     struct RORLSapdu
    {
		
         std::byte state;
         std::byte count;
         RORSapdu apdu;
    };




     struct AbsoluteTime
    {
		
         std::byte century;
         std::byte year;
         std::byte month;
         std::byte day;
         std::byte hour;
         std::byte minute;
         std::byte second;
         std::byte fraction;
    };

     struct ObjectType
    {
		
         unsigned short partition;
         unsigned short code;
    };

     struct PollMdibDataReplyExt
    {
		
         unsigned short poll_number;
         unsigned short sequence_no;
         unsigned int rel_time_stamp;
         AbsoluteTime abs_time_stamp;
         ObjectType type;
         unsigned short polled_attr_grp;
    };

     struct ReceiveState
    {
		
         unsigned int state;
         unsigned int position;
    };

     struct NuObsValue
    {
		
         unsigned short physio_id;
         unsigned short state;
         unsigned short unit_code;
         unsigned int value;
    };

     struct NuObsValueCmp
    {
		
         unsigned short count;
         unsigned short length;
        // NuObsValue [] value = new NuObsValue[1];
         NuObsValue value1;
    };

     struct SaObsValue
    {
		
         unsigned short physio_id;
         unsigned short state;
         unsigned short length;
        // byte [] value = new byte[1];  
         std::byte value1;
    };

     struct SaObsValueCmp
    {
		
         unsigned short count;
         unsigned short length;
        // SaObsValue [] value = new SaObsValue[1];
         SaObsValue value1;
    };

    

    struct MetricStructure
    {
		
         std::byte ms_struct;
         std::byte ms_comp_no;
    };

    struct MetricSpec
    {
		
         unsigned int update_period;
         unsigned short category;
         unsigned short access;
        MetricStructure structure;
         unsigned short relevance;
    };

    struct StringMP
    {
		
         unsigned short length;
        // unsigned short [] value = new unsigned short[1];
         unsigned short value1;
    };

    struct PollProfileExt
    {
		
         unsigned int options;
        AttributeList ext_attr;
    };

    struct PollProfileSupport
    {
		
         unsigned int poll_profile_revision;
         unsigned int min_poll_period;
         unsigned int max_mtu_rx;
         unsigned int max_mtu_tx;
         unsigned int max_bw_tx;
         unsigned int options;
        AttributeList optional_packages;
    };

    
    struct MdseUserInfoStd
    {
		
         unsigned int protocol_version;
         unsigned int nomenclature_version;
         unsigned int functional_units;
         unsigned int system_type;
         unsigned int startup_mode;
         unsigned int option_list;
        AttributeList supported_aprofiles;
    };

     struct EventReportArgument
    {
		
         ManagedObjectId managed_object;
         unsigned int relative_time;
         unsigned short event_type;
         unsigned short length;

    };

    struct MDSCreateEvenReport
    {
		
         SessionHeader session_hdr;
         ROapdus remoteop_hdr;
         ROIVapdu remoteinvoke_hdr;
         EventReportArgument eventreport_hdr;
         ManagedObjectId objectid;
         AttributeList attributes;

    };

    //-----------------------------------------------------------------------------


     struct NumericObject
    {
		
         unsigned int validity;
         unsigned short pollNumber;
         unsigned int label;
        NuObsValue value;
        MetricSpec spec;
        StringMP labelString;

    };


    struct SinglePollLinkedPacketResult
    {
		
         SessionHeader session_hdr;
         ROapdus remoteop_hdr;
         RORLSapdu remoteop_cmd;
         ActionResult action_result;
         //PollMdibDataReply mdib_data;
         PollInfoList pollinfo_list; //null placeholder

    };

    struct ExtPollLinkedPacketResult
    {
		
         SessionHeader session_hdr;
         ROapdus remoteop_hdr;
         RORLSapdu remoteop_cmd;
         ActionResult action_result;
         PollMdibDataReplyExt mdib_data;
         PollInfoList pollinfo_list; //null placeholder

    };
	struct SinglePollPacketResult
    {
		
         SessionHeader session_hdr;
         ROapdus remoteop_hdr;
         RORSapdu remoteop_cmd;
         ActionResult action_result;
         //PollMdibDataReply mdib_data;
         PollInfoList pollinfo_list; //null placeholder

    };




	struct IDLabel
	{
		uint32_t idlabelhandle;
		std::string idlabelstring;
		uint16_t obpoll_handle;
	};
