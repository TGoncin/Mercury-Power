/**___________________________________________________________________________________________________
|    @date      May 26, 2026 ~ EDIT: August 17 2026
|    @author    Tom Goncin
|    @file      Conductor.cpp
|    @brief     Optimization analyzing electrical conductor sizing.
|    @brief     Transformer::Components     is "Physical Hardware"
|___________________________________________________________________________________________________*/
#include <cmath> 
#include <sstream> 
#include <iomanip>
#include <regex> 
#include <algorithm> 
#include <cctype>

#include "alias.h"
using namespace Transformer::Types;
#include "definitions.h"
#include "system_Config.h"
#include "table_Search.h"
#include "math_Utilities.h"

#include "coil.h"
#include "winding.h"
#include "device.h"

#include "conductor.h"



namespace Transformer::Components
{
    int Conductor::id_Counter = 1;

    Conductor::Conductor 
        (
        home_Coil*              coil, 
        home_MaterialConstants  materialConstants, 
        home_CoilBil3Ph         table_CoilBil3Ph, 
        home_ConductorTable     table_Conductor
        )
        :   device                      (coil ? coil->device  : nullptr), 
            winding                     (coil ? coil->winding : nullptr), 
            coil                        (coil),
            table_ConductorTable        (table_Conductor), 
            table_CoilBil3Ph            (table_CoilBil3Ph), 
            table_MaterialConstants     (materialConstants),
            conductorWarnings           (this)
    {
        conductorFields["id"]                = FieldValue(Conductor::id_Counter);
        conductorFields["name"]              = FieldValue("Conductor " + std::to_string(Conductor::id_Counter));
        conductorFields["iType"]             = FieldValue(home_ConductorType::None);
        conductorFields["iNoConductor"]      = FieldValue(0);
        conductorFields["dWidth"]            = FieldValue(1.00);
        conductorFields["iNoAxial"]          = FieldValue(1);
        conductorFields["iNoRadial"]         = FieldValue(1);
        conductorFields["iRadialLevel"]      = FieldValue(1);
        conductorFields["dWeight"]           = FieldValue(0.00);
        conductorFields["sWeight"]           = FieldValue("");
        conductorFields["tMatAluminum"]      = FieldValue(home_Material::None);
        conductorFields["dWidthStrand"]      = FieldValue(0.00);
        conductorFields["dThickStrand"]      = FieldValue(0.00);
        conductorFields["areaStrand"]        = FieldValue(0.00);
        conductorFields["dTotalWidthTurn"]   = FieldValue(0.00);
        conductorFields["dTotalThickTurn"]   = FieldValue(0.00);
        conductorFields["dMagnetArea"]       = FieldValue(0.00);
        conductorFields["dAreaCond"]         = FieldValue(0.00);
        conductorFields["dDensity"]          = FieldValue(0.00);
        conductorFields["dDensityONAF"]      = FieldValue(0.00);
        conductorFields["sDensity"]          = FieldValue("");
        conductorFields["dDensityFCBN"]      = FieldValue(0.00);
        conductorFields["dDensityFCBNONAF"]  = FieldValue(0.00);
        conductorFields["sDescription"]      = FieldValue("");
        conductorFields["dConductorSpace"]   = FieldValue(0.00);
        conductorFields["dStayback"]         = FieldValue(0.00);
        conductorFields["dT"]                = FieldValue(0.00);
        conductorFields["dT2"]               = FieldValue(0.00);
        conductorFields["sSWConductor"]      = FieldValue("");
        conductorFields["sMatConductor"]     = FieldValue("");
        conductorFields["sPapperConductor"]  = FieldValue("");
        conductorFields["dSBMin"]            = FieldValue(0.0);
        conductorFields["dUncorrSB"]         = FieldValue(0.00);
        conductorFields["dUncorrCondSpc"]    = FieldValue(0.00);
        conductorFields["dMaxTurnsLyr"]      = FieldValue(0.00);
        conductorFields["dUncorrLyrs"]       = FieldValue(0.00);
        conductorFields["dCorrTurnsLyrs"]    = FieldValue(0.00);
        conductorFields["dCorrLyrs"]         = FieldValue(0.00);
        conductorFields["sErrorName"]        = FieldValue(FieldValue::List (Config::ERRORMESSAGES, ""));
        conductorFields["tErrorEnum"]        = FieldValue(FieldValue::List (Config::ERRORMESSAGES, home_WarningSeverity::None));
        conductorFields["sErrorMessages"]    = FieldValue(FieldValue::List (Config::ERRORMESSAGES, ""));

        Conductor::id_Counter++;
    }

    home_Device* Conductor::getParentDevice() const
    {
        if (!device)            throw std::runtime_error("This conductor is not attached to any Device");
        return device;
    }
    home_Winding *Conductor::getParentWinding() const
    {
        if (!winding)            throw std::runtime_error("This conductor is not attached to any Winding");
        return winding;
    }
    home_Coil *Conductor::getParentCoil() const
    {
        if (!coil)            throw std::runtime_error("This conductor is not attached to any Coil");
        return coil;
    }

    std::vector<int> Conductor::get_NominalConductor(int windingType)
    {
        auto type       = conductorFields["iType"]       .get<home_ConductorType>();
        auto aluminum   = conductorFields["tMatAluminum"].get<home_Material>();

        bool isAluminum = conductorFields["tMatAluminum"].is_Default();

        std::vector<int>  result;

        if (isAluminum)
        {
            if      (windingType == static_cast<int>(home_WindingDesc::LV))
            {
                if  (type == home_ConductorType::Strip)
                {
                    for (const auto& row : table_ConductorTable.tableData_AluminumStrip)    result.push_back(row.no_Conductor);
                    for (const auto& row : table_ConductorTable.tableData_CopperStrip)      result.push_back(row.no_Conductor);
                }
                else
                {
                    for (const auto& row : table_ConductorTable.tableData_CopperMagnet)     result.push_back(row.no_conductor);
                    for (const auto& row : table_ConductorTable.tableData_AluminumMagnet)   result.push_back(row.no_conductor);
                }
            }
            else if (windingType == static_cast<int>(home_WindingDesc::HV))
            {
                for (const auto& row : table_ConductorTable.tableData_CopperMagnet)     result.push_back(row.no_conductor);
                for (const auto& row : table_ConductorTable.tableData_AluminumMagnet)   result.push_back(row.no_conductor);
            }
        }
        else
        {
            if (windingType == static_cast<int>(home_WindingDesc::LV))
            {
                if      (type == home_ConductorType::Strip)               
                {
                    if  (aluminum == home_Material::Copper)
                        for (const auto& row : table_ConductorTable.tableData_CopperStrip)      { result.push_back(row.no_Conductor); }
                    else
                        for (const auto& row : table_ConductorTable.tableData_AluminumStrip)    { result.push_back(row.no_Conductor); }
                }
                else                                              
                {
                    if  (aluminum == home_Material::Copper)
                        for (const auto& row : table_ConductorTable.tableData_CopperMagnet)     { result.push_back(row.no_conductor); }
                    else
                        for (const auto& row : table_ConductorTable.tableData_AluminumMagnet)   { result.push_back(row.no_conductor); }
                }
            }
            else if (windingType == static_cast<int>(home_WindingDesc::HV))           
            {   
                if(aluminum == home_Material::Copper)
                    for (const auto& row : table_ConductorTable.tableData_CopperMagnet)         { result.push_back(row.no_conductor); }
                else
                    for (const auto& row : table_ConductorTable.tableData_AluminumMagnet)       { result.push_back(row.no_conductor); }
            }
        }
        return result;
    }
    std::vector<home_ConductorSolution> Conductor::find_PossibleConductors (home_Winding *winding)
    {
        std::vector<home_ConductorSolution> solutions;

        int windingType = winding->get_WindingType();
        const std::vector<int> list_ConductorNumbers = get_NominalConductor(windingType);

        for (size_t conductorNumber = 0; conductorNumber < list_ConductorNumbers.size(); ++conductorNumber)
        {
            conductorFields["iNoConductor"].set ( list_ConductorNumbers[conductorNumber] );  
        }
        return solutions;
    }

    std::pair<std::vector<int>, std::vector<std::string>> Conductor::get_NominalConductorOpt(int windingType)
    {
        auto type       = conductorFields["iType"]          .get<home_ConductorType>();
        auto aluminum   = conductorFields["tMatAluminum"]   .get<home_Material>();
        int  bil        = coil->coilFields["iBil"]          .get<int>();

        std::vector<int>         list_col0;
        std::vector<std::string> list_col5;

        if (type == home_ConductorType::Strip && windingType == static_cast<int>(home_WindingDesc::LV))
        {
            auto& table = (aluminum == home_Material::Aluminum)
                            ? table_ConductorTable.tableData_AluminumStrip 
                            : table_ConductorTable.tableData_CopperStrip;

            for(const auto& row : table)
            {
                if (row.price_Per_lb < 1000)
                {
                    list_col0.push_back(row.no_Conductor);
                    list_col5.push_back(row.size_Str);
                }
            }
        } 
        else 
        {
            auto& table = (aluminum == home_Material::Copper)
                            ? table_ConductorTable.tableData_CopperMagnet 
                            : table_ConductorTable.tableData_AluminumMagnet;
            
            size_t startIndex = 0;

            if (aluminum == home_Material::Copper && bil >= 250)        {   startIndex = 6;  }
            if (aluminum == home_Material::Aluminum && bil >= 250)      {   startIndex = 4;  }
            
            for(size_t i = startIndex; i < table.size(); ++i)
            {
                if (table[i].price_per_lb < 1000)
                {
                    list_col0.push_back(table[i].no_conductor);
                    list_col5.push_back(table[i].sap_name);
                }
            }
        }
        return {list_col0, list_col5};
    }

    int Conductor::calc_DefaultType(int windingType)
    {
        if (windingType == static_cast<int>(home_WindingDesc::LV))        return static_cast<int>(home_ConductorType::Strip);
        if (windingType == static_cast<int>(home_WindingDesc::HV))        return static_cast<int>(home_ConductorType::Magnet);

        return static_cast<int>(home_ConductorType::None);
    }
    int Conductor::calc_UncorrLyrs(home_Coil *coil)
    {
        return static_cast<int>(Config::safe_Division(coil->coilFields["dTurns"].get<double>(), conductorFields["dMaxTurnsLyr"].get<double>())) + 1;
    }
    int Conductor::calc_DefaultCorrLyrs(home_Coil *coil)
    {
        return static_cast<int>(std::ceil(Config::safe_Division(coil->coilFields["dTurns"].get<double>(), conductorFields["dCorrTurnsLyrs"].get<double>())));
    }
    int Conductor::calc_DefaultMaterial()
    {
        return static_cast<int>(home_Material::Aluminum);
    }
    int Conductor::calc_DefaultNoConductor (int windingType)
    {
        auto type               = conductorFields["iType"]       .get<home_ConductorType>();
        auto materialAluminum   = conductorFields["tMatAluminum"].get<home_Material>();
        int result              = 0;

        auto getFirstConductorCode = [&](const std::string& targetMaterial, bool isStrip) -> int            
        {
            for (const auto& row : table_ConductorTable.tableData_CopperMagnet)
            {
                bool matchShape = isStrip ? false : true;
                if (matchShape) return row.no_conductor;
            }
            return 0;
        };

        if (windingType == static_cast<int>(home_WindingDesc::LV))                                            
        {
            if (type == home_ConductorType::Strip)
            {
                result = (materialAluminum == home_Material::Copper) 
                            ? table_ConductorTable.tableData_CopperStrip[0].no_Conductor 
                            : table_ConductorTable.tableData_AluminumStrip[0].no_Conductor;  
            }
            else
            {
                result = (materialAluminum == home_Material::Copper) 
                            ? table_ConductorTable.tableData_CopperMagnet[0].no_conductor 
                            : table_ConductorTable.tableData_AluminumMagnet[0].no_conductor;  
            }
        }
        else if (windingType == static_cast<int>(home_WindingDesc::HV))
        {
            result = (materialAluminum == home_Material::Copper) 
                        ? table_ConductorTable.tableData_CopperMagnet[0].no_conductor 
                        : table_ConductorTable.tableData_AluminumMagnet[0].no_conductor;  
        }

        return result;
    }
    int Conductor::look_ConductorDevice    (int numberPhases, home_Winding* winding, int windingType)
    {
        std::string description = conductorFields["sDescription"].get<std::string>();
        auto conductorType      = conductorFields["tMatAluminum"].get<home_Material>();

        calc_Tables(winding, windingType);

        return look_NoConductor(static_cast<int>(conductorType), numberPhases, description, windingType);
    }
    int Conductor::look_NoConductor(int conductorType, int numberPhases, const std::string& sConductor, int windingType)
    {
        auto type           = conductorFields["iType"].get<home_ConductorType>();
        int  defaultNumber  = 0;

        auto trim = [] (std::string textString) -> std::string      // Helper lambda -> strip leading/trailing whitespace
        {
            textString.erase    ( textString.begin(), std::find_if (  textString.begin(), textString.end(), [](unsigned char character)   { return !std::isspace(character); }  ) );
            textString.erase    ( std::find_if (  textString.rbegin(), textString.rend(), [] (unsigned char character)                   { return !std::isspace(character);})    .base(), textString.end() );

            return textString;
        };

        std::string sConductorTrimmed = trim(sConductor);

        auto normalizeConductor = [trim] (const std::string &text) -> std::string
        {
            if (text.empty())       {   return "";  }
            
            std::string textString = trim(text);
            std::transform (textString.begin(), textString.end(), textString.begin(), ::tolower);

            textString = std::regex_replace (textString, std::regex("\\s+[a-z]+$"), "");    // remove trailing material tags (e.g., Al, Cu)
            textString = std::regex_replace (textString, std::regex("''"), "\"");           // normalize double quotes
            textString = std::regex_replace (textString, std::regex("'"), "\"");            // normalize single quotes

            std::regex re("\\s*x\\s*");                                                     // Split by 'x' or 'X'
            std::sregex_token_iterator it(textString.begin(), textString.end(), re, -1);
            std::sregex_token_iterator end;

            std::vector<std::string> parts(it, end);

            if (parts.size() != 2)          {   return "";  }

            try
            {
                std::string leftStr     = std::regex_replace(parts[0], std::regex("\""), "");   // strippin quotes out of separated parts
                std::string rightStr    = std::regex_replace(parts[1], std::regex("\""), "");

                double left     = std::stod(trim(leftStr));                                     // Converting to float/double
                double right    = std::stod(trim(rightStr));

                std::stringstream sStream;                                                       // Return formatted string keeping quote only on the second value
                sStream << left << " X " << right << "\"";
                return sStream.str();
            }
            catch (...) 
            {
                return "";
            }
        };

        std::string normalCond = normalizeConductor(sConductor);

        if (type == home_ConductorType::Strip && windingType == static_cast<int>(home_WindingDesc::LV))        // Split logic based on the struct type
        {
            auto& table = (conductorType == static_cast<int>(home_Material::Aluminum))    
                            ? table_ConductorTable.tableData_AluminumStrip 
                            : table_ConductorTable.tableData_CopperStrip;


            for (const auto &row : table)                                                       // Pass 1: Exact Match (checking size_Str)
            {
                if (trim(row.size_Str) == sConductorTrimmed)            { return row.no_Conductor; }
            }

            for (const auto &row : table)                                                       // Pass 2: Normalized Match
            {
                if (!normalCond.empty() && normalizeConductor(row.size_Str) == normalCond)          { return row.no_Conductor; }
            }
        }
        else
        {
            auto& table = (conductorType == static_cast<int>(home_Material::Copper))
                            ? table_ConductorTable.tableData_CopperMagnet 
                            : table_ConductorTable.tableData_AluminumMagnet;

            for (const auto &row : table)                       // Pass 1: Exact Match (checking sap_name / wire_size) -> If you need to search wire_size instead of sap_name, swap it below.
            {
                if (trim(row.sap_name) == sConductorTrimmed)            { return row.no_conductor; }
            }

            for (const auto &row : table)                                                   // Pass 2: Normalized Match
            {
                if (!normalCond.empty() && normalizeConductor(row.sap_name) == normalCond)          { return row.no_conductor; }
            }
        }
        return defaultNumber;
    }
    int Conductor::get_MaxDensityConductorOpt()
    {
        int     windingType     = winding->get_WindingType();
        int     nom_pos         = winding->get_NominalPosition();
        auto    phaseAmpList    = winding->windingFields["pdPhaseAmp"].get<FieldValue::List>();
        double  lineAmp         = std::get<double>(phaseAmpList[nom_pos]);
        double  areaConductor   = 0.005;                                                                    // MOCK threshold safely pulled via vector validation min elements

        return static_cast<int>(std::ceil(lineAmp / areaConductor));
    }

    double Conductor::calc_UncorrSB(home_Winding* winding, home_Coil* coil, int numberPhases)
    {
        if (numberPhases == 1)
        {
            double optStayback = winding->windingFields["dOptStayback"].get<double>();

            return (optStayback > 0.0) 
                    ? optStayback 
                    : coil->coilFields["dRadialBuilt"].get<double>();
        }
        return conductorFields["dSBMin"].get<double>();
    }
    double Conductor::calc_UncorrCondSpc(home_Coil *coil, int windingType, int numberPhases)
    {
        if (numberPhases == 1)       
        {
            return ( coil->coilFields["dOvercoil"].get<double>() - 2.0 * conductorFields["dUncorrSB"].get<double>() );
        }
        else
        {
            if (windingType == static_cast<int>(home_WindingDesc::LV))
            {
                auto   type             = conductorFields["iType"]          .get<home_ConductorType>();
                int    numberConductor  = conductorFields["iNoConductor"]   .get<int>();
                double totalWidthTurn   = conductorFields["dTotalWidthTurn"].get<double>();

                if (type == home_ConductorType::Strip)                    {  return totalWidthTurn;  }

                double  ratedTurns = coil->coilFields["dTurns"] .get<double>();
                int     layers     = coil->coilFields["iLayers"].get<int>();

                double value       = (numberConductor > 30) 
                                        ? ((ratedTurns / layers) + 1.0) * totalWidthTurn * 1.01
                                        : (ratedTurns / layers)         * totalWidthTurn * 1.02;

                return (std::floor(value / 0.125) + 1.0) * 0.125;
            }
            else
            {
                return ( coil->coilFields["dOvercoil"].get<double>() - 2.0 * conductorFields["dUncorrSB"].get<double>() );
            }
        }
    }
    double Conductor::calc_MaxTurnsLyr(home_Winding *winding, int windingType, int numberPhases)
    {
        int    numberConductor  = conductorFields["iNoConductor"]   .get<int>();
        double uncorrCondSpc    = conductorFields["dUncorrCondSpc"] .get<double>();
        double totalWidthTurn   = conductorFields["dTotalWidthTurn"].get<double>();

        if (numberPhases == 1)
        {
            if (numberConductor > 30 && numberConductor < 80)
            {
                return ( std::floor(Config::safe_Division(uncorrCondSpc, totalWidthTurn) / 1.01) - 1.0 );
            }
            return ( std::floor(Config::safe_Division(uncorrCondSpc, totalWidthTurn) / 1.02) );
        }
        else
        {
            auto type = conductorFields["iType"].get<home_ConductorType>();

            if (windingType == static_cast<int>(home_WindingDesc::LV))
            {
                if (type == home_ConductorType::Strip)                    return 1.0;
                if (numberConductor > 30)                                 return std::floor(Config::safe_Division(uncorrCondSpc - 1.0, totalWidthTurn) / 1.01);

                return std::floor(Config::safe_Division(uncorrCondSpc, totalWidthTurn) / 1.02);
            }
            else
            {
                if (type == home_ConductorType::Strip)                    return 1.0;

                double widthStrand         = conductorFields["dWidthStrand"]        .get<double>();
                double thickStrand         = conductorFields["dThickStrand"]        .get<double>();
                double areaCond            = conductorFields["dAreaCond"]           .get<double>();
                double uncorrCondSpcVal    = conductorFields["dUncorrCondSpc"]      .get<double>();
                auto   halfTurns           = winding->windingFields["bHalfTurns"]   .get<bool>();

                bool sameStrand    = (widthStrand == thickStrand);
                bool smallArea     = (areaCond < 0.013);

                if (halfTurns == true)
                {
                    double temp = (sameStrand && smallArea) 
                                    ? Config::safe_Division(uncorrCondSpcVal, totalWidthTurn) / 1.01 - widthStrand
                                    : Config::safe_Division(uncorrCondSpcVal, totalWidthTurn) / 1.02;

                    return std::floor(temp / 0.5) * 0.5;
                }
                else
                {
                    if (sameStrand && smallArea)
                    {
                        return ( std::floor(Config::safe_Division(uncorrCondSpcVal, totalWidthTurn) / 1.01) - 1.0 );
                    }
                    return ( std::floor(Config::safe_Division(uncorrCondSpcVal, totalWidthTurn) / 1.02) );
                }
            }
        }
    }
    double Conductor::calc_CorrectTurnsLyrs(home_Winding *winding, int windingType, home_Coil *coil, int numberPhases)
    {
        double dUncorrLyrs = conductorFields["dUncorrLyrs"].get<double>();
        double dTurns      = coil->coilFields["dTurns"].get<double>();

        if (numberPhases == 1 || windingType == static_cast<int>(home_WindingDesc::LV))            return std::ceil(Config::safe_Division(dTurns, dUncorrLyrs));
        else
        {
            auto bHalfTurns = winding->windingFields["bHalfTurns"].get<bool>();

            return (bHalfTurns == true) 
                    ? std::ceil(Config::safe_Division(dTurns, dUncorrLyrs) / 0.5) * 0.5 
                    : std::ceil(Config::safe_Division(dTurns, dUncorrLyrs));
        }
    }
    double Conductor::calc_DefaultConductorSpace(home_Coil *coil, int windingType, int numberPhases)
    {
        auto type = conductorFields["iType"].get<home_ConductorType>();

        double totalWidthTurn   = conductorFields["dTotalWidthTurn"].get<double>();
        int numberLayers        = coil->coilFields["iLayers"]       .get<int>();
        int numberConductor     = conductorFields["iNoConductor"]   .get<int>();

        if (numberPhases == 1)
        {
            if (windingType == static_cast<int>(home_WindingDesc::LV))
            {
                return (type == home_ConductorType::Strip) 
                        ? totalWidthTurn 
                        : (totalWidthTurn * numberLayers) / 0.25;
            }
            else
            {
                double correctTurnsPerLayer  = conductorFields["dCorrTurnsLyrs"] .get<double>();
                double value                 = (numberConductor > 95) 
                                                ? (correctTurnsPerLayer + 1.0) * totalWidthTurn * 1.01 
                                                : correctTurnsPerLayer * totalWidthTurn * 1.02;
                
                return (std::floor(value / 0.125) + 1.0) * 0.125;
            }
        }
        else
        {
            if (windingType == static_cast<int>(home_WindingDesc::LV))
            {
                if (type == home_ConductorType::Strip)                    { return totalWidthTurn;  }

                double correctTurnsPerLayerValue    = conductorFields["dCorrTurnsLyrs"].get<double>();
                double value                        = (numberConductor > 30) 
                                                        ? (correctTurnsPerLayerValue + 1.0) * totalWidthTurn * 1.01 
                                                        : correctTurnsPerLayerValue * totalWidthTurn * 1.02;

                return (std::floor(value / 0.125) + 1.0) * 0.125;
            }
            else
            {
                if (type == home_ConductorType::Strip)                    return conductorFields["dWidthStrand"].get<double>();

                double correctTurnsPerLayerValue = conductorFields["dCorrTurnsLyrs"].get<double>();
                bool is_even                     = ((static_cast<int>(correctTurnsPerLayerValue * 2.0)) == (static_cast<int>(std::floor(correctTurnsPerLayerValue) * 2.0)));

                if (is_even)
                {
                    double base = (numberConductor > 30) 
                                    ? ((correctTurnsPerLayerValue + 1.0) * totalWidthTurn) * 1.01 
                                    : (correctTurnsPerLayerValue * totalWidthTurn * 1.02);

                    return ( (std::floor(base / 0.125) + 1.0) * 0.125 );
                }
                else
                {
                    double base = (numberConductor > 30) 
                                    ? ((correctTurnsPerLayerValue + 1.0) * totalWidthTurn) 
                                    : ((correctTurnsPerLayerValue - 1.0) * totalWidthTurn * 1.02);

                    return ( (std::floor(base / 0.125) + 1.0) * 0.125 );
                }
            }
        }
    }
    double Conductor::calc_DefaultSBMin(home_Device *dev, home_Winding *windingE, home_Coil *coil)
    {
        int bil             = coil->coilFields["iBil"]                  .get<int>();
        double dOptStaybackE = windingE->windingFields["dOptStayback"]  .get<double>();

        double dT   = lookup_Value(bil, table_CoilBil3Ph.tableData_CoilsC88);
        double dT2  = lookup_Value(bil, table_CoilBil3Ph.tableData_CoilsC88);

        conductorFields["dT"]   .set(dT);
        conductorFields["dT2"]  .set(dT2);

        return (dOptStaybackE > 0.0)    
                    ? dOptStaybackE     
                    : lookup_Value(bil, table_CoilBil3Ph.tableData_CoilsC88);
    }
    double Conductor::calc_DefaultStayback(home_Winding *windingE, home_Coil *coil, int windingType, int numberPhases)
    {
        if (numberPhases == 1)
        {
            if (windingType == static_cast<int>(home_WindingDesc::LV))
            {
                double dOptStaybackE = windingE->windingFields["dOptStayback"].get<double>();
                return (dOptStaybackE > 0.0) 
                            ? dOptStaybackE 
                            : coil->coilFields["dStaybak"].get<double>();
            }
            else
            {
                return std::floor((coil->coilFields["dOvercoil"].get<double>() - conductorFields["dConductorSpace"].get<double>()) / 2.0 / 0.03125) * 0.03125;
            }
        }
        else
        {
            if (windingType == static_cast<int>(home_WindingDesc::LV))
            {
                double calc = (2.0 * conductorFields["dSBMin"].get<double>() + conductorFields["dUncorrCondSpc"].get<double>() - conductorFields["dConductorSpace"].get<double>()) / 2.0;
                return std::floor(calc / 0.0625) * 0.0625;
            }
            else
            {
                return std::floor((coil->coilFields["dOvercoil"].get<double>() - conductorFields["dConductorSpace"].get<double>()) / 2.0 / 0.0625) * 0.0625;
            }
        }
    }
    void Conductor::copy(const Conductor& conductorObject_CopiedFrom)
    {
        conductorFields["iType"]        .set_Value   (conductorObject_CopiedFrom.conductorFields.at("iType"));
        conductorFields["iNoConductor"] .set_Value   (conductorObject_CopiedFrom.conductorFields.at("iNoConductor"));
        conductorFields["dWidth"]       .set_Value   (conductorObject_CopiedFrom.conductorFields.at("dWidth"));
        conductorFields["iNoAxial"]     .set_Value   (conductorObject_CopiedFrom.conductorFields.at("iNoAxial"));
        conductorFields["iNoRadial"]    .set_Value   (conductorObject_CopiedFrom.conductorFields.at("iNoRadial"));
    }
    void Conductor::reset_Fields()
    {
        for (auto &pair : conductorFields)
        {
            try                                     {   pair.second.reset();   }
            catch (const std::exception &e)         {   std::cout << "No se pudo restaurar " << pair.first << ": " << e.what() << std::endl;    }
        }
    }
    void Conductor::assign_Winding(home_Winding *winding)
    {
        this->winding = winding;    
    }
    void Conductor::assign_Defaults    (home_Device* device, home_Winding* winding, home_Coil* coil, int windingType)
    {
        int numberPhases = device->deviceFields["iNoPhases"].get<int>();

        conductorFields["iType"]            .assign_Default (calc_DefaultType (windingType));
        conductorFields["tMatAluminum"]     .assign_Default (calc_DefaultMaterial());
        conductorFields["iNoConductor"]     .assign_Default (calc_DefaultNoConductor (windingType));
        conductorFields["dSBMin"]           .assign_Default (calc_DefaultSBMin (device, winding, coil));
        conductorFields["dUncorrSB"]        .assign_Default (calc_UncorrSB (winding, coil, numberPhases));
        conductorFields["dUncorrCondSpc"]   .assign_Default (calc_UncorrCondSpc (coil, windingType, numberPhases));
        conductorFields["dMaxTurnsLyr"]     .assign_Default (calc_MaxTurnsLyr (winding, windingType, numberPhases));
        conductorFields["dUncorrLyrs"]      .assign_Default (static_cast<double>(calc_UncorrLyrs(coil)));
        conductorFields["dCorrTurnsLyrs"]   .assign_Default (calc_CorrectTurnsLyrs (winding, windingType, coil, numberPhases));
        conductorFields["dConductorSpace"]  .assign_Default (calc_DefaultConductorSpace (coil, windingType, numberPhases));
        conductorFields["dCorrLyrs"]        .assign_Default (static_cast<double> (calc_DefaultCorrLyrs(coil)));
        conductorFields["dStayback"]        .assign_Default (calc_DefaultStayback (winding, coil, windingType, numberPhases));
    }
    void Conductor::erase_Warnings()
    {
        for (int i = 0; i < Config::ERRORMESSAGES; ++i)
        {
            conductorFields["sErrorName"]       .set("", i);
            conductorFields["tErrorEnum"]       .set(home_WarningSeverity::None, i);
            conductorFields["sErrorMessages"]   .set("", i);
        }
    }
    void Conductor::calc_Conductor(home_Device *device, home_Winding *winding, home_Coil *coil, int windingType)
    {
        calc_Tables             (winding, windingType);
        calc_MaterialAl         (windingType);
        calc_WidthStrand        (windingType);
        calc_ThickStrand        (windingType);
        calc_TotalWidthTurn     (coil);
        calc_TotalThickTurn     (coil);
        calc_MagnetArea         (windingType);
        calc_Area               (coil, windingType);
        calc_Description        ();
        calc_PaperConductor     ();
        calc_SWConductor        (windingType);
        calc_MatConductor       ();
        calc_Weight             (device, coil, windingType);
        calc_Density            (winding, windingType);
    }
    void Conductor::calc_Tables(home_Winding *winding, int windingType)
    {
        if (windingType == static_cast<int>(home_WindingDesc::LV))
        {
            auto coating        = winding->windingFields["tCoating"].get<home_Coating>();
            double width        = conductorFields["dWidth"]         .get<double>();
            // Unused variables 'final' and 'factor' removed to prevent warnings
        }
    }
    void Conductor::calc_MaterialAl(int windingType)
    {
        auto type = conductorFields["iType"].get<home_ConductorType>();

        int numberConductor   = conductorFields["iNoConductor"].get<int>();
        home_Material result  = home_Material::None;

        if (windingType == static_cast<int>(home_WindingDesc::LV))
        {
            if (type == home_ConductorType::Strip)                result = (numberConductor > 24) ? home_Material::Copper : home_Material::Aluminum;
            else                                                  result = (numberConductor < 47) ? home_Material::Copper : home_Material::Aluminum;
        }
        else if (windingType == static_cast<int>(home_WindingDesc::HV))
        {
            result = (numberConductor >= 80 && numberConductor <= 106) ? home_Material::Aluminum : home_Material::Copper;
        }

        conductorFields["tMatAluminum"].set(result);
    }
    void Conductor::calc_ThickStrand(int windingType)
    {
        auto type                = conductorFields["iType"].get<home_ConductorType>();
        auto materialAluminum    = conductorFields["tMatAluminum"].get<home_Material>();
        int numberConductor      = conductorFields["iNoConductor"].get<int>();
        double result            = 0.0;

        if (windingType == static_cast<int>(home_WindingDesc::LV))
        {
            if (type == home_ConductorType::Strip)
            {
                // Pass 1: Check Aluminum Strip
                for (const auto& row : table_ConductorTable.tableData_AluminumStrip)
                {
                    if (row.no_Conductor == numberConductor) { result = row.thick_Wire; break; }
                }
                // Pass 2: Fallback to Copper Strip if result is still 0.0
                if (result == 0.0)
                {
                    for (const auto& row : table_ConductorTable.tableData_CopperStrip)
                    {
                        if (row.no_Conductor == numberConductor) { result = row.thick_Wire; break; }
                    }
                }
            }
            else
            {
                // Safe ternary: Both tables are std::vector<ConductorMagnetRow>
                const auto& table = (materialAluminum == home_Material::Copper)
                                    ? table_ConductorTable.tableData_CopperMagnet 
                                    : table_ConductorTable.tableData_AluminumMagnet;
                for (const auto& row : table)
                {
                    if (row.no_conductor == numberConductor) { result = row.thick_wire; break; }
                }
            }
        }
        else if (windingType == static_cast<int>(home_WindingDesc::HV))
        {
            const auto& table = (materialAluminum == home_Material::Copper)
                                ? table_ConductorTable.tableData_CopperMagnet 
                                : table_ConductorTable.tableData_AluminumMagnet;
            for (const auto& row : table)
            {
                if (row.no_conductor == numberConductor) { result = row.thick_wire; break; }
            }
        }

        conductorFields["dThickStrand"].set(result);
    }
    void Conductor::calc_WidthStrand(int windingType)
    {
        auto type             = conductorFields["iType"].get<home_ConductorType>();
        auto materialAluminum = conductorFields["tMatAluminum"].get<home_Material>();
        int numberConductor   = conductorFields["iNoConductor"].get<int>();
        double result         = 0.0;

        if (windingType == static_cast<int>(home_WindingDesc::LV))
        {
            if (type == home_ConductorType::Strip)
            {
                result = conductorFields["dWidth"].get<double>();
            }
            else
            {
                const auto& table = (materialAluminum == home_Material::Copper)
                                    ? table_ConductorTable.tableData_CopperMagnet 
                                    : table_ConductorTable.tableData_AluminumMagnet;
                for (const auto& row : table)
                {
                    if (row.no_conductor == numberConductor) { result = row.width_wire; break; }
                }
            }
        }
        else if (windingType == static_cast<int>(home_WindingDesc::HV))
        {
            const auto& table = (materialAluminum == home_Material::Copper)
                                ? table_ConductorTable.tableData_CopperMagnet 
                                : table_ConductorTable.tableData_AluminumMagnet;
            for (const auto& row : table)
            {
                if (row.no_conductor == numberConductor) { result = row.width_wire; break; }
            }
        }
        
        conductorFields["dWidthStrand"].set(result);
    }
    void Conductor::calc_TotalWidthTurn(home_Coil *coil)
    {
        double widthStrand        = conductorFields["dWidthStrand"].get<double>();
        int numberAxial           = conductorFields["iNoAxial"].get<int>();
        double dWidthStrand2E     = 0.0;

        if (coil->coilFields["iLayers"].get<int>() == 2 && coil->numberConductors() > 1)
        {
            dWidthStrand2E = coil->conductor[1].conductorFields.at("dWidthStrand").get<double>();
        }

        conductorFields["dTotalWidthTurn"].set(widthStrand * numberAxial + dWidthStrand2E);
    }
    void Conductor::calc_TotalThickTurn(home_Coil *coil)
    {
        double thickStrand        = conductorFields["dThickStrand"].get<double>();
        int numberRadial          = conductorFields["iNoRadial"].get<int>();
        double dThickStrand2E     = 0.0;

        if (coil->coilFields["iLayers"].get<int>() == 2 && coil->numberConductors() > 1)
        {
            dThickStrand2E = coil->conductor[1].conductorFields.at("dThickStrand").get<double>();
        }

        conductorFields["dTotalThickTurn"].set(thickStrand * numberRadial + dThickStrand2E);
    }
    void Conductor::calc_MagnetArea(int windingType)
    {
        auto type               = conductorFields["iType"]          .get<home_ConductorType>();
        auto materialAluminum   = conductorFields["tMatAluminum"]   .get<home_Material>();
        int numberAxial         = conductorFields["iNoAxial"]       .get<int>();
        int numberRadial        = conductorFields["iNoRadial"]      .get<int>();
        int numberConductor     = conductorFields["iNoConductor"]   .get<int>();
        double widthStrand      = conductorFields["dWidthStrand"]   .get<double>();
        double thickStrand      = conductorFields["dThickStrand"]   .get<double>();
        double areaStrand       = 0.0;

        // Clean helper to extract area_in2
        auto getMagnetArea = [&](home_Material mat, int noCond) -> double
        {
            const auto& table = (mat == home_Material::Copper) 
                                ? table_ConductorTable.tableData_CopperMagnet 
                                : table_ConductorTable.tableData_AluminumMagnet;

            for (const auto& row : table)                                   {   if (row.no_conductor == noCond) return row.area_in2;    }

            return 0.0;
        };

        if (windingType == static_cast<int>(home_WindingDesc::LV))
        {
            if (type == home_ConductorType::Magnet)                         {   areaStrand = getMagnetArea(materialAluminum, numberConductor);  }
            else                                                            {   areaStrand = widthStrand * thickStrand;     }
        }
        else if (windingType == static_cast<int>(home_WindingDesc::HV))     {    areaStrand = getMagnetArea(materialAluminum, numberConductor);  }
        else                                                                {    areaStrand = widthStrand * thickStrand; }

        conductorFields["areaStrand"]   .set(areaStrand);
        conductorFields["dMagnetArea"]  .set(numberAxial * numberRadial * areaStrand);
    }
    void Conductor::calc_Area(home_Coil *coil, int windingType)
    {
        auto type       = conductorFields["iType"].get<home_ConductorType>();
        double result   = 0.0;

        if (windingType == static_cast<int>(home_WindingDesc::LV))
        {
            if (type == home_ConductorType::Strip)
            {
                result = conductorFields["dTotalWidthTurn"].get<double>() * conductorFields["dTotalThickTurn"].get<double>();
            }
            else
            {
                double dMagnetArea2E = (coil->coilFields["iLayers"].get<int>() == 2 && coil->numberConductors() > 1)
                                        ? coil->conductor[1].conductorFields.at("dMagnetArea").get<double>()
                                        : 0.0;

                result = conductorFields["dMagnetArea"].get<double>() + dMagnetArea2E;
            }
        }
        else if (windingType == static_cast<int>(home_WindingDesc::HV))
        {
            double dMagnetArea2E = (coil->coilFields["iLayers"].get<int>() == 2 && coil->numberConductors() > 1)
                                    ? coil->conductor[1].conductorFields.at("dMagnetArea").get<double>()
                                    : 0.0;
            result = conductorFields["dMagnetArea"].get<double>() + dMagnetArea2E;
        }

        conductorFields["dAreaCond"].set(result);
    }
    void Conductor::calc_Description()
    {
        auto type                = conductorFields["iType"]         .get<home_ConductorType>();
        auto materialAluminum    = conductorFields["tMatAluminum"]  .get<home_Material>();
        int numberConductor      = conductorFields["iNoConductor"]  .get<int>();
        std::string result       = "";

        if (type == home_ConductorType::Strip)
        {
            for(const auto& row : table_ConductorTable.tableData_AluminumStrip)
            {
                if (row.no_Conductor == numberConductor)            {   result = row.size_Str;  }
            }
            if (result.empty())
            {
                for(const auto& row : table_ConductorTable.tableData_CopperStrip)
                {
                    if (row.no_Conductor == numberConductor)        {   result = row.size_Str;  }
                }
            }
        } 
        else 
        {
            if (materialAluminum == home_Material::Copper)
            {
                for(const auto& row : table_ConductorTable.tableData_CopperMagnet)
                {
                    if (row.no_conductor == numberConductor)        {   result = row.sap_name;  }
                }
            } 
            else 
            {
                for(const auto& row : table_ConductorTable.tableData_AluminumMagnet)
                {
                    if (row.no_conductor == numberConductor)        {   result = row.sap_name;      }
                }
            }
        }

        conductorFields["sDescription"].set(result);
    }
    void Conductor::calc_SWConductor(int windingType)
    {
        if (windingType == static_cast<int>(home_WindingDesc::LV))
        {
            conductorFields["sSWConductor"].assign_Default((conductorFields["iType"].get<home_ConductorType>() == home_ConductorType::Strip)
                                                                                                                    ? "LV Strip"
                                                                                                                    : "LV Wire");
        }
        else                    conductorFields["sSWConductor"].assign_Default("HV Wire");
    }
    void Conductor::calc_MatConductor()
    {
        conductorFields["sMatConductor"].assign_Default((conductorFields["tMatAluminum"].get<home_Material>() == home_Material::Aluminum)
                                                                                                                    ? "Aluminum"
                                                                                                                    : "Copper");
    }
    void Conductor::calc_PaperConductor()
    {
        std::string desc = conductorFields["sDescription"].get<std::string>();
        conductorFields["sPapperConductor"].assign_Default((desc.find("HV") != std::string::npos) 
                                                                                ? "Paper wrapped" 
                                                                                : "");
    }
    void Conductor::calc_Weight(home_Device *device, home_Coil *coils, int windingType)
    {
        double  ratedTurns      = coils->coilFields["dTurns"]      .get<double>();
        int     numberphases    = device->deviceFields["iNoPhases"].get<int>();
        double  conductorArea   = conductorFields["dAreaCond"]     .get<double>();

        double dK_Dens = (windingType == static_cast<int>(home_WindingDesc::LV))
                            ? table_MaterialConstants.tableData[3][0]               // 3.854
                            : table_MaterialConstants.tableData[0][0];              // 3.854

        auto arrayType = device->deviceFields["arrayType"].get<home_ArrayType>();
        double factor1 = 0.0;

        if (windingType == static_cast<int>(home_WindingDesc::LV))
        {
            auto type = conductorFields["iType"].get<home_ConductorType>();
            auto tMatAluminumE = conductorFields["tMatAluminum"].get<home_Material>();

            double mt                   = (arrayType == home_ArrayType::LHL)
                                            ? device->deviceFields["dLV_MT"].get<double>()
                                            : device->deviceFields["dILV_MT"].get<double>();

            double wght_factor          = (type == home_ConductorType::Strip && tMatAluminumE == home_Material::Copper)
                                            ? 1.0
                                            : (1.0 + device->deviceFields["dLVCondWghtFactor"].get<double>() / 100.0);

            double cu_Strip_modifier    = (tMatAluminumE == home_Material::Copper && type == home_ConductorType::Strip) ? 1.05 : 1.0;

            factor1 = (mt / 12.0) * ratedTurns * dK_Dens * conductorArea * numberphases * wght_factor * cu_Strip_modifier;
        }
        else
        {
            factor1 = (device->deviceFields["dHV_MT"].get<double>() / 12.0) 
                        * ratedTurns 
                        * dK_Dens 
                        * numberphases 
                        * conductorArea 
                        * (1.0 + device->deviceFields["dHVCondWghtFactor"].get<double>()    /    100.0);
        }

        double factor2 = (conductorFields["iType"].get<home_ConductorType>() == home_ConductorType::Strip)
                            ? conductorFields["dWidthStrand"]       .get<double>() 
                                * conductorFields["dThickStrand"]   .get<double>() 
                                * conductorFields["iNoAxial"]       .get<int>() 
                                * conductorFields["iNoRadial"]      .get<int>()
                            : conductorFields["dMagnetArea"]        .get<double>();

        double result = Config::safe_Division((factor1 * factor2), conductorArea);
        conductorFields["dWeight"].set(result);

        std::stringstream ss;
        ss << std::fixed << std::setprecision(0) << result << " lbs";
        conductorFields["sWeight"].set(ss.str());
    }
    void Conductor::calc_Density(home_Winding *winding, int windingType)
    {
        int    nominalPosition  = winding->get_NominalPosition();

        auto phaseAmpList  = winding->windingFields["pdPhaseAmp"].get<FieldValue::List>();
        auto phaseAmp1List = winding->windingFields["pdPhaseAmp1"].get<FieldValue::List>();

        double lineAmp          = std::get<double>(phaseAmpList[nominalPosition]);
        double lineAmp1         = std::get<double>(phaseAmp1List[nominalPosition]);
        double areaConductor    = conductorFields["dAreaCond"] .get<double>();

        double result   = lineAmp / areaConductor;
        conductorFields["dDensity"].set(result);

        std::string density     = std::to_string (static_cast<int> (std::ceil(result)));
        double resultONAF       = lineAmp1 / areaConductor;
        conductorFields["dDensityONAF"].set(resultONAF);

        if (!Config::real_Equals(resultONAF, 0.0))
        {
            density += " / " + std::to_string(static_cast<int>(std::ceil(resultONAF)));
        }
        conductorFields["sDensity"].set(density);

        if ( windingType == static_cast<int>(home_WindingDesc::HV)
                &&  winding->have_Taps()
                &&  winding->changer.changerFields["tCapacity"].get<home_ChangerCapacity>() == home_ChangerCapacity::FCBN
           )
        {
            int windingSteps = winding->get_NumberSteps() - 1;

            if (windingSteps >= 0 && areaConductor > 0.0)       // Incase out of bounds or division of zero
            {
                if (windingSteps < static_cast<int>(phaseAmpList.size()))
                {
                    double lineAmpEOExc = std::get<double>(phaseAmpList[windingSteps]);
                    conductorFields["dDensityFCBN"].set(lineAmpEOExc / areaConductor);
                }

                if (windingSteps < static_cast<int>(phaseAmp1List.size()))
                {
                    double lineAmpEOExc1 = std::get<double>(phaseAmp1List[windingSteps]);
                    conductorFields["dDensityFCBNONAF"].set(lineAmpEOExc1 / areaConductor);
                }
            }
        }
    }
    void Conductor::calc_Material()
    {
        auto type               = conductorFields["iType"]       .get<home_ConductorType>();
        int numberConductors    = conductorFields["iNoConductor"].get<int>();
        int windingType         = winding->get_WindingType();

        conductorFields["tMatAluminum"].assign_Default(static_cast<int>(home_Material::Aluminum));

        if (windingType == static_cast<int>(home_WindingDesc::LV))
        {
            if (type == home_ConductorType::Strip)
            {
                if (numberConductors >= 25 && numberConductors <= 47)
                {
                    conductorFields["tMatAluminum"].set(home_Material::Copper);
                }
            }
            else
            {
                if (numberConductors >= 9 && numberConductors <= 46)
                {
                    conductorFields["tMatAluminum"].set(home_Material::Copper);
                }
            }
        }
        else
        {
            if (numberConductors >= 9 && numberConductors <= 46)
            {
               conductorFields["tMatAluminum"].set(home_Material::Copper); 
            }
        }
    }
    void Conductor::calc_Warnings()
    {
        std::vector<double> values = {0.0, 0.0};

        conductorWarnings.warning_CurrentDensityMax (values);
        conductorWarnings.warning_WidthTable        (values);
        conductorWarnings.warning_Width             (values);
    }
    void Conductor::calc_densityOpt(home_Winding *winding)
    {
        int nominalPosition = winding->get_NominalPosition();
        auto phaseAmpList   = winding->windingFields["pdPhaseAmp"].get<FieldValue::List>();
        double lineAmp      = std::get<double>(phaseAmpList[nominalPosition]);
        double density      = conductorFields["dDensity"] .get<double>();

        conductorFields["dAreaCond"].set(lineAmp / density);
    }
}
//end of Conductor class