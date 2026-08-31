/**___________________________________________________________________________________________________
|    @date      May 26, 2026 ~ EDIT: August 17 2026
|    @author    Tom Goncin
|    @file      Coil.cpp
|    @brief     Calculation architecture resolving transformer coil mechanical properties.
|    @brief     Transformer::Components     is "Physical Hardware"
|___________________________________________________________________________________________________*/
#include <iostream>
#include <sstream>
#include <iomanip>

#include "alias.h"
#include "definitions.h"
#include "system_Config.h"
#include "conversions.h"
#include "table_Search.h"
#include "math_Utilities.h"
#include "transformer_Config.h"

#include "winding.h"
#include "guarantee.h"
#include "device.h"
#include "designer.h"
#include "discs.h"
#include "cooling.h"
#include "coil.h"

using namespace Transformer::Types;




namespace Transformer::Components
{
    int Coil::id_Counter = 1;

    Coil::Coil (home_Device*             device, 
                home_CoilType            coilType, 
                home_RegulationType      regulationType, 
                home_MaterialConstants   materialConstants, 
                home_CoilBil1Ph          table_CoilBil1Ph, 
                home_CoilBil3Ph          table_CoilBil3Ph, 
                home_ConductorTable      table_Conductor
                )
            : 
                device                  (device), 
                winding                 (nullptr), 
                table_MaterialConstants (materialConstants), 
                table_CoilBil1Ph        (table_CoilBil1Ph), 
                table_CoilBil3Ph        (table_CoilBil3Ph), 
                table_Conductor         (table_Conductor)
    {
        coilFields["id"] =                  home_FieldValue (Coil::id_Counter);
        coilFields["name"] =                home_FieldValue ("Coil " + std::to_string         (Coil::id_Counter));
        coilFields["iBil"] =                home_FieldValue (30);
        coilFields["tDescDevanado"] =       home_FieldValue (static_cast<int>                 (home_WindingDesc::None));
        coilFields["i_Biles"] =             home_FieldValue (home_FieldValue::List({30.0, 30.0, 30.0}));
        coilFields["dTurns"] =              home_FieldValue (0.0);
        coilFields["iLayers"] =             home_FieldValue (1);
        coilFields["iSectionsLeg"] =        home_FieldValue (1);
        coilFields["bConstantStayback"]=    home_FieldValue (false);
        coilFields["tRegulationType"] =     home_FieldValue(static_cast<int>                  (regulationType));
        coilFields["tCoilType"] =           home_FieldValue(static_cast<int>                  (coilType));
        coilFields["dStaybak"] =            home_FieldValue (0.00);
        coilFields["dCoolingDuct"] =        home_FieldValue (0.00);
        coilFields["dCoreProtection"] =     home_FieldValue (0.00);
        coilFields["dFormDetails"] =        home_FieldValue (0.00);
        coilFields["dFormDuct"] =           home_FieldValue (0.00);
        coilFields["dForm001Paper"] =       home_FieldValue (0.00);
        coilFields["dUnderHV001Paper"] =    home_FieldValue (0.00);
        coilFields["dUOHV001Paper"] =       home_FieldValue (0.00);
        coilFields["dUOHVDuct"] =           home_FieldValue (0.00);
        coilFields["dUOHVCrepePaper"] =     home_FieldValue (0.00);
        coilFields["dUOHV001Paper2"] =      home_FieldValue (0.00);
        coilFields["dCoilCoreDuct"] =       home_FieldValue (0.00);
        coilFields["dCoilCorePacking"] =    home_FieldValue (0.00);
        coilFields["dCoilCoilDuct"] =       home_FieldValue (0.00);
        coilFields["dCoilCoilPacking"] =    home_FieldValue (0.00);
        coilFields["dCoilCoilDuct2"] =      home_FieldValue (0.00);
        coilFields["pdPhaseAmp"] =          home_FieldValue (home_FieldValue::List(MAX_TAPS, 0.0));
        coilFields["pdPhaseAmpSinRel"] =    home_FieldValue (home_FieldValue::List(MAX_TAPS, 0.0));
        coilFields["iLongPhaseAmp"] =       home_FieldValue (0.0);
        coilFields["pdPhaseAmpDifPos"] =    home_FieldValue (home_FieldValue::List(MAX_TAPS, 0.0));
        coilFields["dOvercoil"] =           home_FieldValue (0.0);
        coilFields["dTurnsSection"] =       home_FieldValue (0.0);
        coilFields["iTurnsLayers"] =        home_FieldValue (1);
        coilFields["dNoLayersUnder"] =      home_FieldValue (0.0);
        coilFields["dNoLayersBody"] =       home_FieldValue (0.0);
        coilFields["dNoLayersOver"] =       home_FieldValue (0.0);
        coilFields["dTotLayers"] =          home_FieldValue (0.0);
        coilFields["i_Ducts"] =             home_FieldValue (home_FieldValue::List(10, 0));
        coilFields["ductsIn"] =             home_FieldValue ("");
        coilFields["ductsOut"] =            home_FieldValue ("");
        coilFields["dWidthDucts"] =         home_FieldValue (0.0);
        coilFields["sWidthDucts"] =         home_FieldValue ("");
        coilFields["dInterPaper"] =            home_FieldValue (0.0);
        coilFields["dEndPaper"] =              home_FieldValue (0.0);
        coilFields["iTotNoEndLayers"] =        home_FieldValue (0);
        coilFields["sInsulationEnd"] =         home_FieldValue ("");
        coilFields["sPreInsEnd"] =             home_FieldValue ("");
        coilFields["sInsulationInt"] =         home_FieldValue ("");
        coilFields["sInsulationIntComplete"] = home_FieldValue ("");
        coilFields["iRequiredNumDucts"] =      home_FieldValue (0.0);
        coilFields["s_OverOuterPreliminar"] =  home_FieldValue (home_FieldValue::List({"", ""}));
        coilFields["sOver"] =                  home_FieldValue ("");
        coilFields["dkVmm"] =                 home_FieldValue (0.00);
        coilFields["dRadialBuilt"] =          home_FieldValue (0.00);
        coilFields["sDescriptionTb"] =        home_FieldValue ("");
        coilFields["dRadialBuiltSides"] =     home_FieldValue (0.00);
        coilFields["dRadialBuiltIntNoses"] =  home_FieldValue (0.00);
        coilFields["dRadialBuiltOutNoses"] =  home_FieldValue (0.00);
        coilFields["sRadialBuilds"] =         home_FieldValue ("");
        coilFields["iHigh"] =                 home_FieldValue (0);
        coilFields["iWide"] =                 home_FieldValue (0);
        coilFields["dCondThickTurn"] =        home_FieldValue (0.00);
        coilFields["dCondWdthTurn"] =         home_FieldValue (0.00);
        coilFields["sErrorName"] =     home_FieldValue(home_FieldValue::List(ERRORMESSAGES, ""));
        coilFields["tErrorEnum"] =     home_FieldValue(home_FieldValue::List(ERRORMESSAGES, static_cast<int> (home_WarningSeverity::None)));
        coilFields["sErrorMessages"] = home_FieldValue(home_FieldValue::List(ERRORMESSAGES, ""));

        conductor.push_back(home_Conductor(this, table_MaterialConstants, table_CoilBil3Ph, table_Conductor));
        
        Coil::id_Counter++;
    }

    void Coil::assign_Index(int index) 
    {
        id_Counter = index;
        coilFields["id"].set(index);
        coilFields["name"].set("Coil " + std::to_string(id_Counter));
    }

    int Coil::get_Position() const 
    {
        if (device == nullptr) return 0;
        for (size_t i = 0; i < device->coil.size(); ++i) 
        {
            if (device->coil[i].get() == this) 
            {
                return static_cast<int>(i);
            }
        }
        return 0;
    }

    void Coil::assign_WindingDesc(home_WindingDesc winding_desc) 
    {
        if (winding != nullptr && coilFields["tDescDevanado"].get<home_WindingDesc>() == winding_desc) return;

        remove_Winding();
        coilFields["tDescDevanado"].assignDefault(static_cast<int>(winding_desc));

        if (device != nullptr) 
        {
            if      (winding_desc == home_WindingDesc::HV || winding_desc == home_WindingDesc::RegHV)         winding = device->get_WindingHV();
            else if (winding_desc == home_WindingDesc::LV || winding_desc == home_WindingDesc::RegLV)         winding = device->get_WindingLV();
            else if (winding_desc == home_WindingDesc::TV)                                                    winding = device->get_WindingTV();

            if (winding != nullptr)                                                                           winding->add_Coils(this);
        }
    }

    void Coil::assign_Winding(home_Winding* winding) 
    {
        this->winding = winding;
        for (auto& assignConductor : conductor)         assignConductor.assign_Winding(winding);
    }

    void Coil::remove_Winding() 
    {
        if (winding != nullptr) 
        {
            auto& coilsList = winding->coils;
            coilsList.erase(std::remove(coilsList.begin(), coilsList.end(), this), coilsList.end());
            winding = nullptr;
        }
    }

    void Coil::copy(const Coil& other_coil) 
    {
        coilFields["iLayers"]           .set(other_coil.coilFields.at("iLayers"));
        coilFields["bConstantStayback"] .set(other_coil.coilFields.at("bConstantStayback"));
        coilFields["iSectionsLeg"]      .set(other_coil.coilFields.at("iSectionsLeg"));
        
        size_t activeConductorSize = numberConductors();

        if (activeConductorSize == 1)                                                conductor[0].copy(other_coil.conductor[0]);
        else 
        {
            for (size_t i = 0; i < activeConductorSize - 1; ++i)                     conductor[i].copy(other_coil.conductor[i]);
        }
    }

    home_Device* Coil::parentDevice() const 
    {
        if (device == nullptr)      throw std::runtime_error("This conductor is not attached to any Device");
        return device;
    }

    home_Winding* Coil::getParentWinding() const 
    {
        if (winding == nullptr)         throw std::runtime_error("This conductor is not attached to any Winding");
        return winding;
    }

    size_t Coil::numberConductors() const           {    return conductor.size();   }

    void Coil::assign_Parents() 
    {
        home_Device* device     = parentDevice();
        home_Winding* winding   = getParentWinding();

        for (auto& conductorField : conductor) 
        {
            conductorField.device   = device;
            conductorField.winding  = winding;
            conductorField.coil     = this;
        }
    }

    void Coil::reset_Fields() 
    {
        for (auto& pair : coilFields)
        {
            try
            {
                pair.second.reset();
            }
            catch (...) {}
        }
    }

    void Coil::erase_Warnings() 
    {
        for (auto& conductorFields : conductor)                conductorFields.erase_Warnings();

        for (int i = 0; i < Config::ERRORMESSAGES; ++i) 
        {
            coilFields["sErrorName"]    .set("", i);
            coilFields["tErrorEnum"]    .set(static_cast<int>(home_WarningSeverity::None), i);
            coilFields["sErrorMessages"].set("", i);
        }
    }

    void Coil::calc_Warnings() 
    {
        for (auto& conductorFields : conductor)                conductorFields.calc_Warnings();
    }

    void Coil::assign_Defaults() 
    {
        home_Winding*    winding      = getParentWinding();
        int              windingType  = winding->get_WindingType();
        int              bil          = static_cast<int>(coilFields["iBIL"].get<int>());
        home_Device*     device       = parentDevice();
        home_Winding*    windingLv    = device->get_WindingLV();
        auto&            conductor0   = conductor[0];
        int              numberPhases = coilFields["numberPhases"].get<int>();

        home_DesignSpecStandards designSpecStandards  = coilFields["DesignSpecStandards"].get<home_DesignSpecStandards>();
        home_WindingDesc         windingDescription   = coilFields["tDescDevanado"]      .get<home_WindingDesc>();   
        
        calc_Turns (winding);
        coilFields ["iBil"]              .assignDefault                                   (bil);
        coilFields ["i_Biles"]           .assignDefault (home_FieldValue::List                 ({static_cast<double>(bil), static_cast<double>(bil), static_cast<double>(bil)}));
        coilFields ["iSectionsLeg"]      .assignDefault (calc_SectionsLeg                 (winding));
        coilFields ["dOvercoil"]         .assignDefault (calc_DefaultOvercoil             (windingType, windingLv));
        coilFields ["dTurnsSection"]     .assignDefault (calc_DefaultTurnsSection         (windingType));
        coilFields ["iTurnsLayers"]      .assignDefault (calc_DefaultTurnsLayers          (conductor0, windingType));
        coilFields ["dNoLayersBody"]     .assignDefault (calc_DefaultNumberLayersBody     (winding, conductor0, windingType));
        coilFields ["dTotLayers"]        .assignDefault (calc_DefaultTotalLayers());
        coilFields ["dInterPaper"]       .assignDefault (calc_DefaultInterlayerPaper      (device, winding, conductor0, windingType, designSpecStandards, numberPhases));
        coilFields ["dEndPaper"]         .assignDefault (calc_DefaultEndPaper             (windingType,  designSpecStandards, numberPhases));
        coilFields ["iTotNoEndLayers"]   .assignDefault (calc_DefaultTotalNumberEndLayers (winding));
        coilFields ["sInsulationEnd"]    .assignDefault (calc_DefaultInsulationEnd        (winding, windingType, numberPhases));
        coilFields ["sInsulationInt"]    .assignDefault (calc_DefaultInsulationInt        (windingType));
        
        calc_OverOuterPreliminar(winding);

        for (auto& cond : conductor)            {        cond.assign_Defaults(device, winding, this, windingType);      }

        calc_OverOuter(device, winding, windingType);
    }

    int Coil::calc_SectionsLeg (home_Winding* winding)
    {
       return static_cast<int> (winding->coils.size());   
    }

    double Coil::calc_DefaultOvercoil(int windingType, home_Winding* windingLv) 
    {
        if (windingType == static_cast<int>(home_WindingDesc::LV)) 
        {
            return coilFields.at("dConductorSpace").get<double>() + 2.0 * coilFields.at("dStayback").get<double>();
        } 
        else 
        {
            return windingLv->coils[0]->coilFields["dOvercoil"].get<double>();
        }
    }

    double Coil::calc_DefaultTurnsSection(int windingType) 
    {
        double turns = coilFields["dTurns"].get<double>();

        if (windingType == static_cast<int> (home_WindingDesc::LV))     {   return safe_Division (turns, static_cast<double> (coilFields["iSectionsLeg"] .get<int>())); } 
        else                                                            {   return turns;   }
    }

    double Coil::calc_DefaultTurnsLayers (const home_Conductor& conductor, int windingType) 
    {
        auto type = coilFields.at("iType").get<home_ConductorType>();

        if (windingType == static_cast<int>(home_WindingDesc::LV)) 
        {
            if (type == home_ConductorType::Strip)     return 1.0;
            else                                       return safe_Division(coilFields["dTurnsSection"].get<double>(), static_cast<double>(coilFields["iLayers"].get<int>()));
        } 
        else                return coilFields.at("layersCorrectTurns").get<double>();
    }

    double Coil::calc_DefaultNumberLayersBody(home_Winding* winding, const home_Conductor& conductor, int windingType) 
    {
        auto type = coilFields.at("iType").get<home_ConductorType>();
        if (windingType == static_cast<int>(home_WindingDesc::LV)) 
        {
            if (type == home_ConductorType::Strip)   return safe_Division(coilFields["dNomTurnsUI"].get<double>(), static_cast<double>(coilFields["iSectionsLeg"].get<int>()));
            else                                     return static_cast<double>(coilFields["iLayers"].get<int>());
        }
        else        return coilFields.at("dCorrLyrs").get<double>();
    }

    double Coil::calc_DefaultTotalLayers()   
    {
        return coilFields["dNoLayersUnder"].get<double>() + coilFields["dNoLayersBody"] .get<double>() + coilFields["dNoLayersOver"] .get<double>();    
    }

    double Coil::calc_DefaultInterlayerPaper (home_Device* device, home_Winding* winding, const home_Conductor& conductor, int windingType, home_DesignSpecStandards designSpecStandards, int numberPhases)
    {
        return calc_InterPaper(windingType, designSpecStandards, numberPhases);
    }

    double Coil::calc_InterPaper (int windingType, home_DesignSpecStandards designSpecStandards, int numberPhases) 
    {
        double layerInsulation             = coilFields.at("dLayerInsulation")  .get<double>();
        double totalThickTurn              = coilFields.at("dTotalThickTurn")   .get<double>();
        double insulationBody              = calc_InsulationBody                (numberPhases, designSpecStandards);
        double minInsulationMechanical     = calc_MinInsulationMechanical       (windingType, totalThickTurn);
        
        return (layerInsulation > 0.0) ? layerInsulation : std::max(insulationBody, minInsulationMechanical);
    }

    double Coil::calc_ImpkV() 
    {
        double corrTurnsLyrs   = coilFields.at("dCorrLyrs").get<double>();
        double result           = 0.0;
        
        if (corrTurnsLyrs > 19)              result = table_CoilInsulation3Ph.tableData_Coil.back()[0];
        else                                 result = lookup_Value(corrTurnsLyrs, table_CoilInsulation3Ph.tableData_Coil);

        return (result / 100.0) * coilFields["iBil"].get<int>();
    }

    double Coil::calc_InsulationFirst(home_DesignSpecStandards designSpecStandards) 
    {
        if (designSpecStandards == home_DesignSpecStandards::C8816) 
        {
            double maxInterlayerStress  = MAX_INTERLAYER_STRESS_C88;
            double voltsLayers          = 2.0 * coilFields.at("layersCorrectTurns").get<double>() * coilFields["dVxT"].get<double>();

            return std::ceil((voltsLayers / maxInterlayerStress) / 5.0) * 5.0 / 1000.0;
        } 
        else 
        {
            double dImpkV = calc_ImpkV();

            for (const auto& row : table_CoilInsulation3Ph.tableData_RangeKv) 
            {
                if (!row.empty() && dImpkV <= row[0])                   return row[1];
            }
        }
        return 0.0;
    }

    double Coil::calc_InsulationBody(int numberPhases, home_DesignSpecStandards designSpecStandards) 
    {
        double maxInterlayerStress = (designSpecStandards == home_DesignSpecStandards::C8816)   ?    MAX_INTERLAYER_STRESS_C88 :   (   (numberPhases == 1) ? MAX_INTERLAYER_STRESS_1C2 : MAX_INTERLAYER_STRESS_C2 );

        double voltsLayers = 2.0 * coilFields.at("layersCorrectTurns").get<double>() * coilFields["dVxT"].get<double>();
        return std::ceil ( (voltsLayers / maxInterlayerStress) / 5.0 ) * 5.0 / 1000.0;
    }

    double Coil::calc_MinInsulationMechanical(int windingType, double totalThickTurn) 
    {
        if (windingType == static_cast<int> (home_WindingDesc::LV)) 
        {
            auto type = coilFields.at("iType") .get<home_ConductorType>();

            if (type == home_ConductorType::Strip) 
            {
                auto materialAluminum   = coilFields.at("tMatAluminum")       .get<home_Material>();
                int numberAxial         = coilFields.at("iNoAxial")           .get<int>();
                double totalWidthTurn   = coilFields.at("dTotalWidthTurn")    .get<double>();
                
                if (materialAluminum == home_Material::Aluminum && numberAxial == 1 && totalThickTurn <= 0.04)               return (totalWidthTurn <= 24.0) ? 0.005 : 0.01;
                else            return (totalWidthTurn <= 24.0) ? 0.01 : 0.015;
            } 
            else 
            {
                if (totalThickTurn <= 0.0475)     return 0.01;
                if (totalThickTurn < 0.103)       return 0.015;
                
                return 0.02;
            }
        } 
        else 
        {
            if ((totalThickTurn - 0.008) <= 0.0475)     return 0.01;
            if ((totalThickTurn - 0.008) < 0.103)       return 0.015;

            return 0.02;
        }
    }

    double Coil::calc_DefaultEndPaper(int windingType, home_DesignSpecStandards designSpecStandards, int numberPhases) 
    {
        double layerInsulation    = coilFields ["dLayerInsulation"]   .get<double>();
        double endInsulation      = coilFields ["dEndInsulation"]     .get<double>();
        double optional           = (windingType == static_cast <int> (home_WindingDesc::LV)) ? layerInsulation : endInsulation;

        double insulationFirst             = calc_InsulationFirst        (designSpecStandards);
        double insulationBody              = calc_InsulationBody         (numberPhases, designSpecStandards);
        double minInsulationMechanical     = calc_MinInsulationMechanical(windingType, coilFields.at("dTotalThickTurn").get<double>());
        
        return (optional > 0.0) ? optional : std::max({insulationFirst, insulationBody, minInsulationMechanical});
    }

    int Coil::calc_DefaultTotalNumberEndLayers(home_Winding* winding)          {     return (coilFields["type_connection"].get<home_ConnectionType>() == home_ConnectionType::Wye) ? 3 : 6;       }

    void Coil::calc_OverOuterPreliminar(home_Winding* winding) 
    {
        double insertLayer   = coilFields["dEndPaper"]               .get<double>();
        int    bil           = winding->coils[0]->coilFields["iBil"] .get<int>();
        std::string result   = "";

        if      (insertLayer == 0.0022 && bil == 30) result = "8 - 0.0022";
        else if (insertLayer == 0.01   && bil == 30) result = "3 - 0.010";
        else if (insertLayer == 0.0025 && bil == 60) result = "5 - 0.01";
        else if (insertLayer == 0.01   && bil == 60) result = "5 - 0.010";
        else if (insertLayer == 0.0022 && bil <= 95) result = "9 - 0.01";
        else if (insertLayer == 0.01   && bil == 75) result = "9 - 0.010";
        else if (insertLayer == 0.005  && bil == 30) result = "4 - 0.005";

        coilFields["s_OverOuterPreliminar"].set(result, 0);

        std::string fallbackResult    = result;

        if      (result.empty() && insertLayer == 0.005 && bil <= 95)           fallbackResult = "8 - 0.005";
        else if (result.empty() && insertLayer == 0.02  && bil > 29 )           fallbackResult = "4 - 0.010";

        coilFields["s_OverOuterPreliminar"].set(fallbackResult, 1);
    }

    std::string Coil::calc_DefaultInsulationEnd(home_Winding* winding, int windingType, int numberPhases) 
    {
        double endPaper     = coilFields["dEndPaper"]   .get<double>();

        if (windingType == static_cast<int> (home_WindingDesc::LV)) 
        {
            std::stringstream ss;
            ss << std::fixed << std::setprecision(3) << endPaper;
            return ss.str();
        } 
        else 
        {
            double interPaper = coilFields["dInterPaper"].get<double>();
            if (endPaper != interPaper) 
            {
                if (numberPhases == 1) 
                {
                    int bil_Under   = coilFields["iBILUnder"]  .get<int>();
                    int bil         = coilFields["iBil"]       .get<int>();

                    if (bil_Under == bil) 
                    {
                        coilFields["sPreInsEnd"].set("First & last 3 lyrs/section");
                        return "First & last 3 lyrs/section " + std::to_string(endPaper);
                    } 
                    else 
                    {
                        coilFields["sPreInsEnd"].set("Last 3 layers");
                        return "Last 3 layers " + std::to_string(endPaper);
                    }
                } 
                else 
                {
                    auto typeConn = coilFields["type_connection"].get<home_ConnectionType>();

                    if (typeConn == home_ConnectionType::Delta) 
                    {
                        coilFields["sPreInsEnd"].set("First & last 3 lyrs/section");
                        return "First & last 3 lyrs/section " + std::to_string(endPaper);
                    } 
                    else 
                    {
                        coilFields["sPreInsEnd"].set("Last 3 layers");
                        return "Last 3 layers " + std::to_string(endPaper);
                    }
                }
            } 
            else 
            {
                coilFields["sPreInsEnd"].set("");
                return "";
            }
        }
    }

    std::string Coil::calc_DefaultInsulationInt(int windingType)
    {
        if (windingType == static_cast<int> (home_WindingDesc::LV))         return "";
        else 
        {
            double endPaper     = coilFields["dEndPaper"]   .get<double>();
            double interPaper   = coilFields["dInterPaper"] .get<double>();
            
            std::stringstream ss;

            if (endPaper == interPaper) 
            {
                ss << "All " << std::fixed << std::setprecision(3) << interPaper;
                coilFields["sInsulationIntComplete"].set(ss.str());
                return "All";
            } 
            else 
            {
                ss << "Remaining " << std::fixed << std::setprecision(3) << interPaper;
                coilFields["sInsulationIntComplete"].set(ss.str());
                return "Remaining ";
            }
        }
    }

    void Coil::calc_Coil() 
    {
        home_Winding* winding        = getParentWinding();
        int           windingType    = winding->get_WindingType();
        home_Device*  device         = parentDevice();
        home_Coil*    coil           = device->coil[device->get_PositionBT()].get();
        auto&         conductor0     = conductor[0];
        int           numberPhases   = coilFields["numberPhases"].get<int>();

        getInfo_CoilTable(device);
        calc_Kvmm();
        calc_RadialBuilt();
        calc_RequiredNumberDucts(winding, windingType);
        calc_Ducts              (winding, windingType, coil, conductor0 , numberPhases);
        calc_WidthDucts         (device,  winding );
        calc_StringDucts        (winding, windingType);
        calc_RBSides            (&conductor0, winding, windingType, numberPhases);
        calc_RadialBuiltInNoses (winding, windingType);
        
        if ((winding->coils.size() > 1 && windingType == static_cast<int> (home_WindingDesc::LV)) || windingType == static_cast <int>(home_WindingDesc::HV))     
        {
            calc_RadialBuiltOutNoses(winding, windingType);
        }

        for (auto& cond : conductor)                cond.calc_Conductor(device, winding, this, windingType);
        
        calc_Height();
        calc_Width();
        calc_ConductorThicknessPerTurn();
        calc_ConductorWidthTurn();
    }

    void Coil::getInfo_CoilTable (home_Device* device) 
    {
        if (device->deviceFields["numberPhases"].get<int>() != 1)        return;

        int bil = coilFields["iBil"].get<int>();

        std::vector<double> row = lookup_RowValue(bil, table_CoilBil1Ph.tableData);

        if(!row.empty()) 
        {
            coilFields["dStaybak"]          .set(row[1]);
            coilFields["dCoolingDuct"]      .set(row[2]);
            coilFields["dCoreProtection"]   .set(row[3]);
            coilFields["dFormDetails"]      .set(row[4]);
            coilFields["dFormDuct"]         .set(row[5]);
            coilFields["dForm001Paper"]     .set(row[6]);
            coilFields["dUnderHV001Paper"]  .set(row[7]);
            coilFields["dUOHV001Paper"]     .set(row[8]);
            coilFields["dUOHVDuct"]         .set(row[9]);
            coilFields["dUOHVCrepePaper"]   .set(row[10]);
            coilFields["dUOHV001Paper2"]    .set(row[11]);
            coilFields["dCoilCoreDuct"]     .set(row[12]);
            coilFields["dCoilCorePacking"]  .set(row[13]);
            coilFields["dCoilCoilDuct"]     .set(row[14]);
            coilFields["dCoilCoilPacking"]  .set(row[15]);
            coilFields["dCoilCoilDuct2"]    .set(row[16]);
        }
    }

    void Coil::calc_Kvmm() 
    {
        int bil                 = coilFields["iBil"]            .get<int>();
        double stayback         = coilFields["dStaybak"]        .get<double>();
        double coolingDucts     = coilFields["dCoolingDuct"]    .get<double>();
        double coreProtection   = coilFields["dCoreProtection"] .get<double>();    

        coilFields["dkVmm"].set(safe_Division(static_cast<double>(bil), (stayback + coolingDucts + coreProtection) * 25.4));
    }

    void Coil::calc_RadialBuilt() 
    {
        coilFields["dRadialBuilt"].set(coilFields["dStaybak"].get<double>());
    }

    void Coil::calc_RequiredNumberDucts(home_Winding* winding, int windingType) 
    {
        double totalLayers   = coilFields["dTotLayers"].get<double>();
        int numberDuctsInner = winding->windingFields["iNoDuctInner"].get<int>();
        double duct          = 0.0;

        if (windingType == static_cast<int>(home_WindingDesc::LV)) 
        {
            if (id_Counter == 0) 
            {
                duct = (safe_Division(totalLayers, numberDuctsInner + 1) < 1.0) ? 1.0 : safe_Division(totalLayers, numberDuctsInner + 1);
            } 
            else 
            {
                int iNoDuctOuterE = winding->windingFields["iNoDuctOuter"].get<int>();
                duct = (safe_Division(totalLayers, iNoDuctOuterE + 1) < 1.0) ? 1.0 : safe_Division(totalLayers, iNoDuctOuterE + 1);
            }
        } 
        else 
        {
            duct = (safe_Division(totalLayers, numberDuctsInner + 1) < 1.0) ? 1.0 : safe_Division(totalLayers, numberDuctsInner + 1);
        }
        coilFields["iRequiredNumDucts"].assignDefault(duct);
    }

    void Coil::calc_Ducts(home_Winding* winding, int windingType, home_Coil* coil, const home_Conductor& conductor, int numberPhases) 
    {
        double requiredDucts = coilFields["iRequiredNumDucts"].get<double>();
        std::string sDuctPlacement = winding->windingFields["sDuctPlacement"].get<std::string>();
        std::vector<int> numbers;
        
        if (!sDuctPlacement.empty()) 
        {
            std::string cleaned = sDuctPlacement;
            cleaned.erase(std::remove(cleaned.begin(), cleaned.end(), '@'), cleaned.end());
            std::stringstream ss(cleaned);
            std::string token;

            while (std::getline(ss, token, ',')) 
            {
                numbers.push_back(std::stoi(token));
            }
        }

        for (int idx = 0; idx < 10; ++idx) 
        {
            coilFields["i_Ducts"].set(0, idx);
        }

        if (windingType == static_cast<int>(home_WindingDesc::LV)) 
        {
            int ducts               = (id_Counter == 0) ? winding->windingFields["iNoDuctInner"].get<int>() : winding->windingFields["iNoDuctOuter"].get<int>();
            double requiredDuctsInt = coil->coilFields["iRequiredNumDucts"].get<double>();
            bool out                = (id_Counter != 0);

            if (static_cast<int>(requiredDucts) == 1)   { coilFields["i_Ducts"].set(static_cast<int>(requiredDucts + 1e-6) + 1, 0); }
            else                                        { coilFields["i_Ducts"].set(static_cast<int>(requiredDucts + 1e-6), 0);     }

            double requiredDuctsAux = out ? requiredDuctsInt : requiredDucts;
            for (int d = 1; d < 10; ++d) 
            {
                if (ducts > d) 
                {
                    int calculatedVal = static_cast<int>((d + 1) * requiredDuctsAux + 1e-6);

                    if (static_cast<int>(requiredDucts) == 1)   { coilFields["i_Ducts"].set(calculatedVal + 1, d); } 
                    else                                        { coilFields["i_Ducts"].set(calculatedVal, d); }
                }
            }
        } 
        else 
        {
            if (!sDuctPlacement.empty()) 
            {
                for (size_t i = 0; i < numbers.size() && i < 10; ++i)           coilFields["i_Ducts"].set(numbers[i], static_cast<int>(i));
                return;
            }
            
            coilFields["i_Ducts"].set(static_cast<int>(requiredDucts), 0);

            int ducts                   = winding->windingFields["iNoDuctInner"].get<int>();
            int trunc                   = static_cast<int>(requiredDucts + 1e-6);
            double layersCorrectTurns   = conductor.conductorFields.at("layersCorrectTurns").get<double>();

            if (ducts > 1 && numberPhases == 1)                   coilFields["i_Ducts"].set(static_cast<int>(2.0 * requiredDucts + 1e-6), 1);
            else if (ducts >= 1 && numberPhases != 1)             coilFields["i_Ducts"].set((trunc == 1) ? trunc + 1 : trunc, 0);

            for (int d = 2; d <= 10; ++d) 
            {
                if (ducts > d && numberPhases == 1) 
                {
                    coilFields["i_Ducts"].set(static_cast<int>((d + 1) * requiredDucts + 1e-6), d - 1);
                } 
                else if (ducts >= d && numberPhases != 1) 
                {
                    if (d < layersCorrectTurns) 
                    {
                        int truncE = static_cast<int>(d * requiredDucts + 1e-6);
                        coilFields["i_Ducts"].set((trunc == 1) ? truncE + 1 : truncE, d - 1);
                    }
                }
            }
        }
    }

    void Coil::calc_WidthDucts(home_Device* device, home_Winding* winding) 
    {
        
        double ductSize     = winding->windingFields["ductSize"].get<double>();

        if (ductSize > 0.0)             coilFields["dWidthDucts"].assignDefault(ductSize); 
        else 
        {
            double kVA       = winding->windingFields["d_kVA"]        .get<std::vector<double>>()[0];
            auto coolingType = device->deviceFields["t_CoolingType"]  .get<std::vector<home_SingleCoolingType>>()[0];

            if (kVA >= 3000.0 || coolingType == home_SingleCoolingType::KNAN || coolingType == home_SingleCoolingType::LNAN) 
            {
                coilFields["dWidthDucts"].assignDefault(convert_Inch_CM(10.0 * 0.25, 1));
            } 
            else 
            {
                if (kVA >= 1000.0)    coilFields["dWidthDucts"].assignDefault(convert_Inch_CM(10.0 * 0.1875, 1));
                else                  coilFields["dWidthDucts"].assignDefault(convert_Inch_CM(10.0 * 0.125, 1));
            }
        }
    }

    void Coil::calc_StringDucts(home_Winding* winding, int windingType) 
    {
        std::string stringDucts = "";
        auto ductValues = coilFields["i_Ducts"].get<std::vector<int>>();

        for (int i = 0; i < 10; ++i) 
        {
            int ducts = ductValues[i];

            if (ducts > 0) 
            {
                stringDucts += (i == 0) ? "@" : ",";
                stringDucts += std::to_string(ducts);
            }
        }

        coilFields["sDuctsIn"].assignDefault("");

        if (windingType == static_cast<int>(home_WindingDesc::LV)) 
        {
            if (id_Counter == 0) 
            {
                std::string InterDucts = std::to_string( winding->windingFields["iNoDuctInner"].get<int>()  ) + " " + stringDucts;
                winding->windingFields["sNoseDuctsInner"].assignDefault(InterDucts);
                coilFields["ductsIn"]                    .assignDefault(stringDucts);
            } 
            else 
            {
                int numberOuterDucts    = winding->windingFields["iNoDuctOuter"].get<int>();
                std::string sDuctsOuter = (numberOuterDucts > 0) ? (std::to_string(numberOuterDucts) + " " + stringDucts) : "";
                
                winding->windingFields["sNoseDuctsOuter"].assignDefault(sDuctsOuter);
                coilFields["ductsOut"]                   .assignDefault(stringDucts);
            }
        } 
        else 
        {
            std::string InterDucts      = std::to_string( winding->windingFields["iNoDuctInner"].get<int>() ) + " " + stringDucts;

            winding->windingFields["sNoseDuctsInner"].assignDefault(InterDucts);
            coilFields["ductsIn"]                    .assignDefault(stringDucts);
        }

        coilFields["sWidthDucts"].assignDefault(format_Inches(convert_CM_Inch(coilFields["dWidthDucts"].get<double>() / 10.0, 1)) + "\"");
    }

    void Coil::calc_RBSides(home_Conductor* conductor, home_Winding* winding, int windingType, int numberPhases) 
    {
        double totalThickTurn  = conductor->conductorFields.at  ("dTotalThickTurn").get<double>();
        double totalLayers     = coilFields                     ["dTotLayers"].get<double>();
        double endPaper        = coilFields                     ["dEndPaper"].get<double>();
        double interPaper      = coilFields                     ["dInterPaper"].get<double>();
        double buildFactorUsed = winding->windingFields         ["dBuildFactorUsed"].get<double>();
        double correctLayers   = conductor->conductorFields.at  ("dCorrLyrs").get<double>();
        double widthDucts      = coilFields                     ["dWidthDucts"].get<double>();
        auto   connectionType  = winding->windingFields         ["type_connection"].get<home_ConnectionType>();
        int    numberDuctOuter = coilFields                     ["iNoDuctOuter"].get<int>();
        
        double result = 0.0;
        double factor = (widthDucts == 0.125) ? 0.146 : ((widthDucts == 0.1875) ? 0.209 : 0.272);

        if (windingType == static_cast<int> (home_WindingDesc::LV)) 
        {
            result = (((totalThickTurn * totalLayers) + ((totalLayers - 1.0) * endPaper)) + factor * numberDuctOuter) * (1.0 + buildFactorUsed / 100.0);
        } 
        else 
        {
            int hVaDucts = 0;
            int numEndPaper = (connectionType == home_ConnectionType::Wye) ? 3 : 6;
            double insulationTerm = totalThickTurn * correctLayers 
                                    + ( ( (correctLayers - 1.0 - numEndPaper) > 1.0 ) 
                                    ? (numEndPaper * endPaper + (correctLayers - 1.0 - numEndPaper) * interPaper)
                                    : (correctLayers - 1.0) * interPaper );

            result = (totalThickTurn * correctLayers) + insulationTerm + (factor * hVaDucts);
            result *= (1.0 + buildFactorUsed / 100.0);
        }
        coilFields["dRadialBuiltSides"].assignDefault(result);

        if (result != 0.0) 
        {
            std::stringstream ss;
            ss << std::fixed << std::setprecision(2) << result;
            coilFields["sRadialBuilds"].assignDefault(ss.str());
        } 
        else
        {
            coilFields["sRadialBuilds"].assignDefault("        ");
        }
    }

    void Coil::calc_RadialBuiltInNoses(home_Winding* winding, int windingType) 
    {
        double ductWidthInch = convert_CM_Inch(coilFields["dWidthDucts"].get<double>() / 10.0, 1);
        double result = 0.0;
        
        if (windingType == static_cast<int>(home_WindingDesc::LV)) 
        {
            double factor = (ductWidthInch == 0.125) ? 0.146 : ((ductWidthInch == 0.1875) ? 0.209 : 0.272);
            result = coilFields["dRadialBuiltSides"].get<double>() + (coilFields["iNoDuctInner"].get<int>() * factor) * (1.0 + coilFields["dBuildFactorUsed"].get<double>() / 100.0);
        }

        coilFields["dRadialBuiltIntNoses"].assignDefault(result);
        std::string sResult = coilFields["sRadialBuilds"].get<std::string>();

        if (result != 0.0) 
        {
            std::stringstream ss;
            ss << sResult << "   " << std::fixed << std::setprecision(2) << result;
            coilFields["sRadialBuilds"].assignDefault(ss.str());
        } 
        else     {        coilFields["sRadialBuilds"].assignDefault(sResult + "           ");   }
    }

    void Coil::calc_RadialBuiltOutNoses(home_Winding* winding, int windingType) 
    {
        double widthDuctsInch   = convert_CM_Inch(coilFields["dWidthDucts"].get<double>() / 10.0, 1);
        double radialBuiltSides = coilFields["dRadialBuiltSides"].get<double>();
        double buildFactorUsed  = coilFields["dBuildFactorUsed"].get<double>();
        double result           = 0.0;
        double factor           = (widthDuctsInch == 0.125) ? 0.146 : ((widthDuctsInch == 0.1875) ? 0.209 : 0.272);

        if (windingType == static_cast<int>(home_WindingDesc::LV)) 
        {
            result = radialBuiltSides + (coilFields["iNoDuctOuter"].get<int>() * factor) * (1.0 + buildFactorUsed / 100.0);
        } 
        else 
        {
            result = radialBuiltSides + (coilFields["iNoDuctInner"].get<int>() * factor - coilFields["dHLOverNoseReduction"].get<double>()) * (1.0 + buildFactorUsed / 100.0);
        }
        
        coilFields["dRadialBuiltOutNoses"].assignDefault(result);
        std::string sResult = coilFields["sRadialBuilds"].get<std::string>();

        if (result != 0.0) 
        {
            std::stringstream ss;
            ss << sResult << "   " << std::fixed << std::setprecision(2) << result;
            coilFields["sRadialBuilds"].assignDefault(ss.str());
        } 
        else     { coilFields["sRadialBuilds"].assignDefault(sResult + "           "); }
    }

    void Coil::calc_Height() 
    {
        int result = 0;

        if (numberConductors() > 1) 
        {
            const auto& conductor1 = conductor[1].conductorFields; 

            if (conductor1.at("iNoConductor").get<int>() > 1    &&   conductor1.at("iRadialLevel").get<int>() == 2) {    result = 1; }
        }

        result += conductor[0].conductorFields.at("iNoRadial").get<int>();

        coilFields["iHigh"].assignDefault(result);
    }

    void Coil::calc_Width() 
    {
        int result = 0;

        if (numberConductors() > 1) 
        {
            const auto& conductor1 = conductor[1].conductorFields;

            if (conductor1.at("iNoConductor").get<int>() > 1    &&   conductor1.at("iRadialLevel").get<int>() == 2) {    result = 1; }
        }
        result += conductor[0].conductorFields.at("iNoAxial").get<int>();

        coilFields["iWide"].assignDefault(result);
    }

    void Coil::calc_ConductorThicknessPerTurn()
    {
        coilFields["dCondThickTurn"].assignDefault(conductor[0].conductorFields.at("dTotalThickTurn").get<double>());
    }

    void Coil::calc_ConductorWidthTurn()
    {
        coilFields["dCondWdthTurn"].assignDefault(conductor[0].conductorFields.at("dTotalWidthTurn").get<double>());
    }

    void Coil::calc_Turns(home_Winding* winding)
    {
        coilFields["dTurns"].assignDefault(winding->windingFields["d_Turns"].get<std::vector<double>>()[0]);
    }

    void Coil::calc_OverOuter(home_Device* device, home_Winding* winding, int windingType) 
    {
        int windingTypeLastCoil = device->get_WindingLastCoil();

        if (windingType == static_cast<int>(home_WindingDesc::LV) && windingTypeLastCoil == static_cast<int>(home_WindingDesc::LV)) 
        {
            double endPaper          = coilFields["dEndPaper"].get<double>();
            int bil                  = coilFields["iBil"].get<int>();
            auto overOuterPreliminar = coilFields["s_OverOuterPreliminar"].get<std::vector<std::string>>();
            
            std::string result = ( endPaper == 0.005 && bil == 60 ) ? "10 - 0.005" : ( overOuterPreliminar[0].empty() ? overOuterPreliminar[1] : overOuterPreliminar[0] );

            coilFields["sOver"].set("Over Outer: " + result);
        } 
        else if     (windingType == static_cast<int>(home_WindingDesc::HV))  coilFields["sOver"].set("Over: " + winding->windingFields["sHLOverHV"].get<std::string>());
        else        coilFields["sOver"].set("");
    }
// end of Coil class
}
// end of Transformer::Components