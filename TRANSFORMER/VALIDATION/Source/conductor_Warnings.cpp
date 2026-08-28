/**___________________________________________________________________________________________________
|    @date      May 28, 2026
|    @author    Tom Goncin
|    @file      ConductorWarnings.h
|    @brief     Validator of current density limits & structural dimension tables for raw conductor layouts.
|    @brief     Transformer::Validation     is "Quality Control"

|               - EVALUATES:    max & min current density constraints (based on material state)
|               - AUDITS:       strip width config against manufacturer tables & physical limits
|               - REJECTS:      warning structures to managing parent conductor framework
|               - RETURNS:      flags electrical conductor design infractions
|___________________________________________________________________________________________________*/

#include <iomanip>
#include <sstream>

#include "alias.h"
using namespace Transformer::Types;
#include "table_Search.h"

#include "coil.h"
#include "winding.h"
#include "conductor.h"

#include "conductor_Warnings.h"


namespace Transformer::Validation
{
    ConductorWarnings::ConductorWarnings(home_Conductor* parent) : conductorParent(parent)    {}

    inline void ConductorWarnings::errorMessage(home_Conductor* conductor, const std::string& name, home_WarningSeverity type, const std::string& warningMsg) {}
    home_Conductor* ConductorWarnings::getConductorParent() const
    {
        if (this->conductorParent == nullptr) { throw std::runtime_error("This warnings is not attached to any Conductor"); }
        return this->conductorParent;
    }
    home_WarningSeverity ConductorWarnings::warning_CurrentDensityMax(std::vector <double>& values)
    {
        home_Conductor* conductor = this->getConductorParent();

        auto& conductorFields = conductor->conductorFields;
        auto       typeMaterialAl = conductorFields["tMatAluminum"].get<home_Material>();

        bool       materialAl = (typeMaterialAl == home_Material::Aluminum);
        double     currentDensity = conductorFields["dDensity"].get<double>();
        double     currentDensityLimit = materialAl ? 1200.0 : 1800.0;

        int        windingType = conductor->getParentWinding()->get_WindingType();

        home_WarningSeverity warningSeverity = home_WarningSeverity::None;

        if (values.size() < 2) { values.resize(2); }

        values[0] = 0.0;
        values[1] = 0.0;

        if (currentDensity > currentDensityLimit)
        {
            warningSeverity = home_WarningSeverity::Error;
            std::ostringstream           sWarningStream;
            sWarningStream << "The current density(" << std::fixed << std::setprecision(0) << currentDensity
                << " A/in2) for " << to_string(static_cast <home_WindingDesc> (windingType))
                << " has exceeded the allowed limit(" << currentDensityLimit
                << " A/in2).";

            std::string warningMsg = sWarningStream.str();
            std::string name = "tErrorCurrentDensityCondMax";

            errorMessage(conductor, name, warningSeverity, warningMsg);

            values[0] = currentDensity;
            values[1] = currentDensityLimit;
        }
        return warningSeverity;
    }
    //                                                      WARNING:            Current density min
    home_WarningSeverity ConductorWarnings::warning_CurrentDensityMin(std::vector<double>& values)
    {
        home_Conductor* conductor = this->getConductorParent();
        auto& conductorFields = conductor->conductorFields;

        auto    typeMatAl = conductorFields["tMatAluminum"].get<home_Material>();
        bool    materialAl = (typeMatAl == home_Material::Aluminum);

        double  currentDensity = conductorFields["dDensity"].get<double>();
        auto    coil = conductor->getParentCoil();

        // Condition based on insulation/impulse test metrics
        double  currentDensityLimit = (coil->coilFields["iBil"].get<int>() >= 250) ? 0.0 : 500.0;
        int     windingType = conductor->getParentWinding()->get_WindingType();

        home_WarningSeverity warningSeverity = home_WarningSeverity::None;

        if (values.size() < 2) { values.resize(2); }
        values[0] = 0.0;
        values[1] = 0.0;

        if (currentDensity < currentDensityLimit)
        {
            warningSeverity = home_WarningSeverity::Error;

            std::ostringstream sWarningStream;

            sWarningStream << "The current density(" << std::fixed << std::setprecision(0) << currentDensity
                << " A/in2) for " << to_string(static_cast<home_WindingDesc>(windingType))
                << " is below minimum (" << currentDensityLimit
                << " A/in2).";

            std::string warningMsg = sWarningStream.str();
            std::string name = "tErrorCurrentDensityCondMin";

            errorMessage(conductor, name, warningSeverity, warningMsg);

            values[0] = currentDensity;
            values[1] = currentDensityLimit;
        }
        return warningSeverity;
    }
    //                                                      WARNING:            strip dimensions adhere to dynamic manufacturing cross-section profiles
    home_WarningSeverity ConductorWarnings::warning_WidthTable(std::vector<double>& values)
    {
        this->conductorParent = getConductorParent();
        auto& conductorFields = this->conductorParent->conductorFields;

        home_WarningSeverity warningSeverity = home_WarningSeverity::None;

        if (values.size() < 2) { values.resize(2); }
        values[0] = 0.0;
        values[1] = 0.0;

        // VALIDATE:    escape if conductor form factor doesn't match sheet config
        if (conductorFields["iType"].get<home_ConductorType>() != home_ConductorType::Strip) { return warningSeverity; }

        int windingType = conductorParent->getParentWinding()->get_WindingType();

        // VERIFICATION:    low-voltage distribution constraints
        if (windingType == static_cast<int>(home_WindingDesc::LV))
        {
            int     numberConductor = conductorFields["iNoConductor"].get<int>();
            double  width = conductorFields["dWidth"].get<double>();

            // Mimic Python conditional evaluation selecting the specialized material lookup source
            double result = (numberConductor > 24)
                ? lookup_Value(numberConductor, conductorParent->table_ConductorTable.tableData_CopperStrip)
                : lookup_Value(numberConductor, conductorParent->table_ConductorTable.tableData_AluminumStrip);

            if (width > result)
            {
                warningSeverity = home_WarningSeverity::Error;

                std::ostringstream sWarningStream;
                sWarningStream << "The conductor width " << std::fixed << std::setprecision(0) << width
                    << "in is greater than specified by the manufacturer " << result
                    << "in.";

                std::string warningMsg = sWarningStream.str();
                std::string name = "tErrorWidthTable";

                errorMessage(conductorParent, name, warningSeverity, warningMsg);

                values[0] = width;
                values[1] = result;
            }
        }
        return warningSeverity;
    }
    //                                  WARNING:    Audits upper geometric constraints for standard assembly limitations
    home_WarningSeverity ConductorWarnings::warning_Width(std::vector<double>& values)
    {
        this->conductorParent = getConductorParent();
        auto& conductorFields = conductorParent->conductorFields;

        home_WarningSeverity warningSeverity = home_WarningSeverity::None;

        if (values.size() < 2) { values.resize(2); }
        values[0] = 0.0;
        values[1] = 0.0;

        if (conductorFields["iType"].get<home_ConductorType>() != home_ConductorType::Strip) { return warningSeverity; }

        double width = conductorFields["dWidth"].get<double>();

        int windingType = conductorParent->getParentWinding()->get_WindingType();

        if (windingType == static_cast<int>(home_WindingDesc::LV))
        {
            if (width > 36.0)
            {
                warningSeverity = home_WarningSeverity::Error;

                std::ostringstream sWarningStream;
                sWarningStream << "The conductor width " << std::fixed << std::setprecision(0) << width << "in exceeds 36in.";

                std::string warningMsg = sWarningStream.str();
                std::string name = "tErrorWidth36";

                errorMessage(conductorParent, name, warningSeverity, warningMsg);

                values[0] = width;
                values[1] = 36.0;
            }
        }
        return warningSeverity;
    }

}