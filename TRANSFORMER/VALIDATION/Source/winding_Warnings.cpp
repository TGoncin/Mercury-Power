/**___________________________________________________________________________________________________
|    @date      May 28, 2026
|    @author    Tom Goncin
|    @file      windingWarnings.h
|    @brief     Structural layout and electrical insulation engine for transformer windings.
|    @brief     Transformer::Validation     is "Quality Control"

|           - VALIDATES:    low-voltage turn parity (ensuring even turn counts on single-phase units)
|           - EVALUATES:    internal and outer cooling duct distributions relative to coil layers
|           - VERIFICATION: Basic Insulation Level (BIL) limits under specified grounding rules
|___________________________________________________________________________________________________*/
#include <sstream>
#include <iomanip>

#include "alias.h"
using namespace Transformer::Types;
#include "winding_Warnings.h"
#include "math_Utilities.h"
#include "enums.h"
#include "field_Value.h"
#include "transformer_Config.h"

#include "device.h"
#include "winding.h"
#include "coil.h"




namespace Transformer::Validation
{
    WindingWarnings::WindingWarnings(home_Winding* parent) : winding(parent) {}
    home_Winding* WindingWarnings::getParentWinding() const
    {
        if (!winding)            throw std::runtime_error("This warnings is not attached to any Winding");
        return winding;
    }
    WarningSeverity WindingWarnings::warningTurnsLV (std::vector<double>& values)
    {
        home_Device* device     = winding->getParentDevice();

        home_Winding* targetWinding = nullptr;
        for (auto& w : device->windings)
        {
            if ( w.get() == winding )
            {
                targetWinding = w.get();
                break;
            }
        }
        int NumberPhases  = winding->windingFields["iNoPhases"].get<int>();
        int windingType   = winding->get_WindingType();
        
        WarningSeverity warningSeverity  =  WarningSeverity::None;
        values[0] = 0.0;
        values[1] = 0.0;

        if (windingType != static_cast<int>( WindingDesc::LV))       return warningSeverity;
        if (NumberPhases != 1)                                       return warningSeverity;

        double turns = winding->windingFields["dTurnsNom"].get<double>();
        
        if (std::fmod(turns, 2.0) != 0.0)         // modulo check on double parameter
        {
            warningSeverity        = WarningSeverity::Error;   
            std::string warningMsg = "Number of turns in the LV Winding must be an EVEN number.";
            std::string name       = "tErrorTurnsLV";
            add_ErrorMessage (winding, name, warningSeverity, warningMsg);

            values[0] = turns;
            values[1] = turns + 1.0;
        }
        return warningSeverity;
    }
    WarningSeverity WindingWarnings::warningDuctsInner(std::vector<double>& values)
    {
        home_Winding*   winding         = getParentWinding();
        auto            windingField    = winding->windingFields;
        int             windingType     = winding->get_WindingType();
        WarningSeverity warningSeverity = WarningSeverity::None;
        values[0] = 0.0;
        values[1] = 0.0;

        double numberInnerDuct  = windingField["iNoDuctInner"].get<double>();
        double numberLayersBody = winding->coils[0]->coilFields["dNoLayersBody"].get<double>();
        
        if (numberInnerDuct >= numberLayersBody)
        {
            warningSeverity               = WarningSeverity::Error;
            std::string WarningDescription= windingField["tDescription"].get<std::string>();
            std::string WarningMsg        = WarningDescription +   " inner ducts ("                                    + format_Double(numberInnerDuct) 
                                                               +   ") cannot be greater than the number of layers("    + format_Double(numberLayersBody) 
                                                               +   ")";
            std::string name = "tErrorDuctsInner" + std::to_string(windingType);
            
            add_ErrorMessage(winding, name, warningSeverity, WarningMsg);

            values[0] = numberInnerDuct;
            values[1] = numberLayersBody;
        }
        return warningSeverity;
    }
    WarningSeverity WindingWarnings::warningDuctsOuter(std::vector<double>& values)
    {
        home_Winding*    winding         = getParentWinding();
        auto        windingField    = winding->windingFields;
        int         windingType     = winding->get_WindingType();

        WarningSeverity warningSeverity = WarningSeverity::None;
        values[0] = 0.0;
        values[1] = 0.0;

        if (windingType != static_cast<int>(WindingDesc::LV))           return warningSeverity;

        double numberOuterDucts = windingField ["iNoDuctOuter"].get<double>();
        double numberLayersBody = winding->coils[0]->coilFields ["dNoLayersBody"].get<double>();
        
        if (numberOuterDucts >= numberLayersBody)
        {
            warningSeverity                = WarningSeverity::Error;
            std::string WarningDescription = windingField["tDescription"].get<std::string>();
            std::string WarningMsg         = WarningDescription +   " Outer ducts ("                                    + format_Double (numberOuterDucts) 
                                                                +   ") cannot be greater than the number of layers("    + format_Double (numberLayersBody) 
                                                                +   ")";
            std::string name               = "tErrorDuctsOuter" + std::to_string(windingType);
            add_ErrorMessage(winding, name, warningSeverity, WarningMsg);

            values[0] = numberOuterDucts;
            values[1] = numberLayersBody;
        }
        return warningSeverity;
    }
    WarningSeverity WindingWarnings::warningBILUnder (std::vector <double>& values)
    {
        home_Winding*        winding         = getParentWinding();
        auto            windingField    = winding->windingFields; 
        int             windingType     = winding->get_WindingType();
        WarningSeverity warningSeverity = WarningSeverity::None;

        values[0] = 0.0;        //Initialization to safely reset the data
        values[1] = 0.0;

        if (windingType == static_cast<int> (WindingDesc::LV))           { return warningSeverity; }
        
        int  bilUnder = windingField ["iBILUnder"] .get<int>();
        int  bil      = windingField ["iBIL"]      .get<int>();
        bool ground_Y = windingField ["ground_Y"]  .get<bool>();
        
        if (bilUnder < bil && ground_Y == false)
        {
            warningSeverity             = WarningSeverity::Error; // Added explicit assignment missing from original Python script trace
            std::string WarningMsg =    "Under BIL("                                 + std::to_string(bilUnder) 
                                    +   ") must be greater than or equal to HV BIL(" + std::to_string(bil) 
                                    +   ").";
            std::string name    =      "tErrorUnderBil" + std::to_string(windingType);
            
            add_ErrorMessage (winding, name, warningSeverity, WarningMsg);
            values[0] = static_cast<double>(bilUnder);
            values[1] = static_cast<double>(bil);
        }
        return warningSeverity;
    }
    WarningSeverity WindingWarnings::warningLeadsDensity (std::vector <double>& values)
    {
        home_Winding*        winding         = getParentWinding();
        auto            windingField   = winding->windingFields;
        int             windingType     = winding->get_WindingType();
        WarningSeverity warningSeverity = WarningSeverity::None;

        values[0] = 0.0;
        values[1] = 0.0;

        double leadDensity       = windingField ["dLeadsDensity"]          .get<double>(); 
        double maxDensityAllowed = windingField ["dMaxLeadsDensityAllowed"].get<double>(); 

        if (leadDensity > maxDensityAllowed)
        {
            warningSeverity                = WarningSeverity::Warning;
            std::string WarningDescription = windingField["tDescription"].get<std::string>();
            std::string WarningMsg         = WarningDescription 
                                            + " current density of leads ("                            + format_Double(leadDensity) 
                                            + " A/mm²) exceeds maximum permitted design threshold ("   + format_Double(maxDensityAllowed) 
                                            + " A/mm²).";
            std::string name               = "warningSeverityLeadsDensity" + std::to_string(windingType);
            
            add_ErrorMessage(winding, name, warningSeverity, WarningMsg);
        
            values[0] = leadDensity;
            values[1] = maxDensityAllowed;
        }
        return warningSeverity;
    }
}
// end of WindingWarnings class 