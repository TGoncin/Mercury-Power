/**___________________________________________________________________________________________________
|    @date      May 28, 2026
|    @author    Tom Goncin
|    @file      CoreWarnings.h
|    @brief     Comprehensive mathematical compliance engine analyzing magnetic core optimization metrics.
|    @brief     Transformer::Validation     is "Quality Control" 

|           - EVALUATES:    magnetic flux density violations (for traditional or amorphous laminations)
|           - VALIDATES:    physical stacking limits & dimensional steel-to-stack layout ratios
|           - VALIDATES:    Restricts overall core geometric footprint bounds against standards
|           - VERIFICATION: flags auditing electrical transformer core physics
|___________________________________________________________________________________________________*/

#include <iomanip>
#include <sstream>
#include <algorithm>

#include "alias.h"
using namespace Transformer::Types;
#include "math_Utilities.h"

#include "core.h"
#include "device.h"
#include "winding.h"
#include "coil.h"
#include "guarantee.h"

#include "core_Warnings.h"



namespace Transformer::Validation
{
    inline void CoreWarnings::add_ErrorMessage(home_Core* core, const std::string& name, home_WarningSeverity type, const std::string& warningMessage) {}

    CoreWarnings::CoreWarnings(home_Core* parent) : coreParent(parent){}

    home_Core*  CoreWarnings::getParentCore() const 
    {
        if (this->coreParent == nullptr)                {   throw std::runtime_error("This warnings is not attached to any Core");  }
        return this->coreParent;
    }

//                                     EVALUATES:   flux density > max limit for core
    home_WarningSeverity CoreWarnings::warning_FluxDensity(std::vector<double> &values)
    {
        home_Core*  core              = getParentCore();
        auto        device            = core->getParentDevice();
        auto&       deviceFields      = device->deviceFields;
        auto&       coreFields        = core->coreFields;
        auto        laminationType    = coreFields["tLaminationType"].get<home_LaminationType>();
        bool        amorphous         = (laminationType == home_LaminationType::Am  ||  laminationType == home_LaminationType::AmHB1);
        double      fluxDensity       = coreFields["dFluxDensityVoltT"].get<double>();
        double      fluxDensityLimit  = coreFields["dFluxDensityLimitMax"].get<double>();

        home_WarningSeverity warningLabel       = home_WarningSeverity::None;

        if (values.size() < 2)        { values.resize(2); }

        values[0] = 0.0;
        values[1] = 0.0;

        double hardLimit = amorphous ? 1.39 : 1.65;

        if (fluxDensity > hardLimit)     // ERROR:   material grade physical limits
        {
            warningLabel                = home_WarningSeverity::Error;
            std::string typeCore        = amorphous ? "amorphous core" : "core";
            double      announcedLimit  = amorphous ? 1.4 : 1.65;               // MAPPING:  precision differences

            std::ostringstream sWarningStream;
            sWarningStream  << "The flux density(" << std::fixed << std::setprecision(2) << fluxDensity 
                            << "T) in the "                                              << typeCore 
                            << " has exceeded the allowable limit("                      << announcedLimit 
                            << "T).";

            std::string warningMsg = sWarningStream.str();
            std::string name    = "tErrorFluxDensityCore";

            values[0] = fluxDensity;
            values[1] = hardLimit;

            add_ErrorMessage(core, name, warningLabel, warningMsg);
        }
        else if (fluxDensity > fluxDensityLimit) // RECOMMENDATION: approaching flux density ^ limits
        {
            warningLabel = home_WarningSeverity::Warning;

            std::string typeCore = amorphous ? "amorphous core" : "core";

            std::ostringstream sWarningStream;
            sWarningStream << "It is recommended that the flux density " << std::fixed << std::setprecision(2) << fluxDensity 
                        << "T does not exceed " << fluxDensityLimit 
                        << "T.";

            std::string warningMsg    = sWarningStream.str();
            std::string name       = "tWarningFluxDensityCore";

            values[0] = fluxDensity;
            values[1] = fluxDensityLimit;

            add_ErrorMessage(core, name, warningLabel, warningMsg);
        }
        return warningLabel;
    }
//                                     EVALUATES:   flux density < min limit for core
    home_WarningSeverity CoreWarnings::warning_FluxDensityMin(std::vector<double> &values)
    {
    home_Core* core = getParentCore();
    auto coreFields = core->coreFields;

    auto laminationType   = coreFields["tLaminationType"].get<home_LaminationType>();
    bool amorphous        = (laminationType == home_LaminationType::Am || laminationType == home_LaminationType::AmHB1);

    double fluxDensity    = coreFields["dFluxDensityVoltT"].get<double>();
    double fluxDensityLimit = 0.95;

    home_WarningSeverity warningLabel = home_WarningSeverity::None;

    if (values.size() < 2)        { values.resize(2); }
    values[0] = 0.0;
    values[1] = 0.0;

    if (fluxDensity < fluxDensityLimit)
    {
        warningLabel = home_WarningSeverity::Error;

        std::string typeCore = amorphous ? "amorphous core" : "core";

        std::ostringstream sWarningStream;
        sWarningStream << "The flux density(" << std::fixed << std::setprecision(2) << fluxDensity
                       << "T) in the "                                              << typeCore 
                       << " is below the allowable limit("                          << fluxDensityLimit 
                       << "T).";

        std::string warningMsg    = sWarningStream.str();
        std::string name       = "tErrorFluxDensityMinCore";

        values[0] = fluxDensity;
        values[1] = fluxDensityLimit;

        add_ErrorMessage(core, name, warningLabel, warningMsg);
    }

    return warningLabel;
}
//                                     EVALUATES:   total stack > max limit for amorphous cores
home_WarningSeverity CoreWarnings::warning_TotalStackMax(std::vector<double> &values)
{
    home_Core*  core           = getParentCore();
    auto        coreFields     = core->coreFields;
    auto        laminationType = coreFields["tLaminationType"].get<home_LaminationType>();
    bool        amorphous      = (laminationType == home_LaminationType::Am     ||  laminationType == home_LaminationType::AmHB1);
    double      totalStack     = coreFields["dTotalStack"].get<double>();
    double      limit          = 9.0;

    home_WarningSeverity warningLabel = home_WarningSeverity::None;

    if (values.size() < 2)        { values.resize(2); }
    values[0] = 0.0;
    values[1] = 0.0;


    if (!amorphous)                 {   return warningLabel;    }   // EVALUATE: Return clear if uses standard silicon steels

    if (totalStack > limit)
    {
        warningLabel = home_WarningSeverity::Error;

        std::ostringstream warningStream;
        warningStream << "The Total Stack for amorphous cores must be less than " << std::fixed << std::setprecision(2) << limit << ".";

        std::string warningMsg  = warningStream.str();
        std::string name        = "tErrorTotalStackMax";

        values[0] = totalStack;
        values[1] = limit;

        add_ErrorMessage(core, name, warningLabel, warningMsg);
    }
    return warningLabel;
}
//                                     EVALUATES:   cross sectional ratios --> cooling channels
home_WarningSeverity CoreWarnings::warning_RatioSteelWidthTotalStack(std::vector<double> &values)
{
    home_Core*  core    = getParentCore();
    double totalStack   = core->coreFields["dTotalStack"].get<double>();
    double steelWidth   = core->coreFields["dSteelWidth"].get<double>();
    double result       = safe_Division(steelWidth, totalStack);
    double limitMin     = 1.3;
    double limitMax     = 1.8;

    home_WarningSeverity warningLabel = home_WarningSeverity::None;

    if (values.size() < 2)        { values.resize(2); }
    values[0] = 0.0;
    values[1] = 0.0;

    if (result < limitMin || result > limitMax)
    {
        warningLabel = home_WarningSeverity::Warning;

        std::ostringstream sWarningStream;
        sWarningStream << "The Steel Width-to-Total Stack ratio is outside the allowable range (" << limitMin << "-" << limitMax << ").";

        std::string warningMsg  = sWarningStream.str();
        std::string name        = "tWarningRatioSteelTotal";

        values[0] = result;
        values[1] = (result < limitMin) ? limitMin : limitMax;

        add_ErrorMessage(core, name, warningLabel, warningMsg);
    }
    return warningLabel;
}
//                                     EVALUATES:   Yoke length bounds meeting standards
home_WarningSeverity CoreWarnings::warning_MaxLength(std::vector<double> &values)
{
    home_Core*               core                   = getParentCore();
    home_Device*             device                 = core->getParentDevice();
    home_DesignSpecStandards designSpecStandards    = device->guarantee->guaranteeFields["tDesignSpec"].get<home_DesignSpecStandards>();
    home_WarningSeverity     warningLabel           = home_WarningSeverity::None;

    if (values.size() < 2)        { values.resize(2); }
    values[0] = 0.0;
    values[1] = 0.0;

//          FILTER RULES:   Explicitly tracking standard distribution enclosures

    if (designSpecStandards != home_DesignSpecStandards::C227306     &&        designSpecStandards != home_DesignSpecStandards::C227406   &&
        designSpecStandards != home_DesignSpecStandards::C227421     &&        designSpecStandards != home_DesignSpecStandards::C227508   &&
        designSpecStandards != home_DesignSpecStandards::C301106     &&        designSpecStandards != home_DesignSpecStandards::C301206)
    {
        return warningLabel; 
    }

    double nominalKVA = device->deviceFields["nominal_kva"].get<double>();

    home_Winding* windingLv = device->get_WindingLV();
    home_Winding* windingHv = device->get_WindingHV();

//  1 - DOES windingLv/Hv exist                                         ~ Prevents dereferencing bad ptr
//  2 - AND is coils vector in it NOT empty                             ~ Reading out of bounds from empty vector
//  3 - AND is the pointer in the coil vector not pointing to a nullptr ~ Prevents dereferencing bad ptr in coils* vector position1

    int bilLV  = ( windingLv    &&  !windingLv->coils.empty()   &&  windingLv->coils[0] ) 
                        ?   windingLv->coils[0]->coilFields["iBil"].get<int>() 
                        :   0;
    int bilHV  = ( windingHv    &&  !windingHv->coils.empty()   &&  windingHv->coils[0] ) 
                        ?   windingHv->coils[0]->coilFields["iBil"].get<int>() 
                        :   0;
    int bilMax = std::max(bilLV, bilHV);

    double coreLengthWithPB = core->coreFields["dCoreLengthWithPB"].get<double>();
    double limit            = coreLengthWithPB;

//************************************************************************
//  ASK ABOUT FIXING BOUNDARIES e.g.1200 MISSED W/ NO ERROR/WARNING
//************************************************************************

    if      (nominalKVA <= 75.0    && nominalKVA <= 300.0     && bilMax <= 95)            {        limit = 53.0;     }
    else if (nominalKVA >= 75.0    && nominalKVA <= 500.0     && bilMax <= 125)           {        limit = 53.0;     }
    else if (nominalKVA >= 500.0   && nominalKVA <= 1000.0    && bilMax <= 125)           {        limit = 62.75;    }
    else if (nominalKVA >= 1500.0  && nominalKVA <= 3000.0    && bilMax <= 125)           {        limit = 62.55;    }

    if (coreLengthWithPB > limit)
    {
        warningLabel = home_WarningSeverity::Error;

        std::ostringstream sWarningStream;
        sWarningStream << "The core length "                << std::fixed << std::setprecision(2) << coreLengthWithPB
                       << " exceeds the allowable limit "   << limit 
                       << " based on the standard "         << to_string(designSpecStandards)
                       << " for nominal kVA "               << nominalKVA
                       << " and BIL "                       << bilMax << ".";

        std::string warningMsg = sWarningStream.str();
        std::string name = "tErrorCoreMaxLength";

        values[0] = coreLengthWithPB;
        values[1] = limit;

        add_ErrorMessage(core, name, warningLabel, warningMsg);
    }
    return warningLabel;
}
//                                     EVALUATES:    Total height limits for frame interference in main tank
home_WarningSeverity CoreWarnings::warning_MaxHeight(std::vector<double> &values) 
{
    auto*                    core               = getParentCore();
    home_Device*             device             = core->getParentDevice();
    home_DesignSpecStandards designSpecStandards= device->guarantee->guaranteeFields["tDesignSpec"].get<home_DesignSpecStandards>();
    home_WarningSeverity     warningLabel       = home_WarningSeverity::None;

    if (values.size() < 2)            {   values.resize(2);    }
    values[0]     = 0.0;
    values[1]     = 0.0;

    if (designSpecStandards != home_DesignSpecStandards::C227306    &&
        designSpecStandards != home_DesignSpecStandards::C227406    &&
        designSpecStandards != home_DesignSpecStandards::C227421    &&
        designSpecStandards != home_DesignSpecStandards::C227508    &&
        designSpecStandards != home_DesignSpecStandards::C301106    &&
        designSpecStandards != home_DesignSpecStandards::C301206 )          {   return warningLabel;    }

    double  nominalKVA     = device -> deviceFields["nominal_kva"].get<double>();
    auto    windingLV      = device -> get_WindingLV();
    auto    windingHV      = device -> get_WindingHV();

//  1 - DOES windingLv/Hv exist                                         ~ Prevents dereferencing bad ptr
//  2 - AND is coils vector in it NOT empty                             ~ Reading out of bounds from empty vector
//  3 - AND is the pointer in the coil vector not pointing to a nullptr ~ Prevents dereferencing bad ptr in coils* vector position1

    int bilLV = ( windingLV   &&    !windingLV->coils.empty()    &&  windingLV->coils[0] )
                            ?   windingLV->coils[0]->coilFields.at("iBil").get<int>()
                            :   0;

    int bilHV  = windingHV->windingFields["iBil"].get<int>();
    int bilMax = std::max(bilLV, bilHV);

    double overallCoreHeight = core->coreFields["dOverallCoreHeight"].get<double>();
    double limit             = overallCoreHeight;

    if      (nominalKVA <= 75.0    && nominalKVA <= 300.0     && bilMax <= 95)        {   limit = 25.0;    }
    else if (nominalKVA >= 75.0    && nominalKVA <= 500.0     && bilMax <= 125)       {   limit = 25.0;    }
    else if (nominalKVA >= 500.0   && nominalKVA <= 1000.0    && bilMax <= 125)       {   limit = 30.0;    }
    else if (nominalKVA >= 1500.0  && nominalKVA <= 3000.0    && bilMax <= 125)       {   limit = 37.0;    }

    if (overallCoreHeight > limit)
    {
        warningLabel = home_WarningSeverity::Error;

        std::ostringstream sWarningStream;
        sWarningStream << "The core height "                << std::fixed << std::setprecision(2) << overallCoreHeight
                       << " exceeds the allowable limit "   << limit 
                       << " based on the standard "         << to_string(designSpecStandards) 
                       << " for nominal kVA "               << nominalKVA
                       << " and BIL "                       << bilMax << ".";

        std::string warningMsg = sWarningStream.str();
        std::string name       = "tErrorCoreMaxHeight";

        values[0] = overallCoreHeight;
        values[1] = limit;

        add_ErrorMessage ( core, name, warningLabel, warningMsg );
    }
    return warningLabel;
}
}