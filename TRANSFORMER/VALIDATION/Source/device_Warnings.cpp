/**___________________________________________________________________________________________________
|    @date      May 28, 2026
|    @author    Tom Goncin
|    @file      device_Warnings.cpp
|    @brief     Compliance auditor for core ratings, naming syntax, & op efficiency.         
|    @brief     Transformer::Validation     is "Quality Control"
|
|       More info:
|           - PARSES:       data descriptions using strict regex
|           - VALIDATES:    overall op efficiency losses against guaranteed limits
|           - VERIFICATION: ensures manufacturing & performance tokens match specification
|___________________________________________________________________________________________________*/

#include <sstream>
#include <iomanip>

#include "alias.h"
#include "alias.h"
using namespace Transformer::Types;
#include "math_Utilities.h"

#include "device.h"
#include "device_Warnings.h"

namespace Transformers::Config      { enum class home_WarningSeverity;   }
namespace Transformer::Core         { class Device; }




namespace Transformer::Validation
{
    const std::regex DeviceWarnings::DATA_DESC_REGEX ( R"(^_[A-Za-z]{2,4}_[A-Za-z]{3}_[0-9]{1,2},_[0-9]{2}; .+/$ Alvarado)" );

    DeviceWarnings::DeviceWarnings(home_Device *parent) : device(parent) {}

    home_Device* DeviceWarnings::getParentDevice() const
    {
        if (!device)     throw std::runtime_error ("This warnings is not attached to any Device.");
        return device;
    }

    std::string DeviceWarnings::trim(const std::string &str) const
    {
        size_t first = str.find_first_not_of(" \t\r\n");

        if (std::string::npos == first)         {   return "";  }

        size_t last = str.find_last_not_of(" \t\r\n");
        return str.substr(first, (last - first + 1));
    }

    bool DeviceWarnings::validate_DataDescription(const std::string &value) const
    {
        if (value.empty())      return false;

        std::vector <std::string>   blocks;
        std::stringstream           ss(value);
        std::string                 item;

        while (std::getline(ss, item, '/'))             // Split block loops by delimiter & trim whitespace
        {
            std::string trimmed = trim(item);
            if (!trimmed.empty())                       blocks.push_back ( trimmed + "/" );
        }

        for (const auto &block : blocks)                // Match blocks against spec criteria
        {
            if (!std::regex_match(block, DATA_DESC_REGEX))          return false;     // VALIDATION:  IF fail, return false
        }
        return true;
    }

    home_WarningSeverity DeviceWarnings::warning_DataDescription(int index, std::vector<std::string> &values)
    {
        home_Device* device = getParentDevice();

        std::vector <std::string>   dataDescriptions = device->deviceFields [ "data_description" ].get<std::vector<std::string>>();
        std::string                 data             = ( index >= 0 && index < static_cast<int>(dataDescriptions.size())) ? dataDescriptions[index] : "";

        home_WarningSeverity warningSeverity = home_WarningSeverity::None;
        values[0] = "0.0";
        values[1] = "0.0";

        if ( !validate_DataDescription ( data ) && !data.empty() )
        {
            warningSeverity             = home_WarningSeverity::Error;
            std::string warningMsg      = "Invalid format. Expected:_initials_mmm_dd,_yy; description/";
            std::string name            = "tInvalidFormatDataDescription" + std::to_string(index);

            addErrorMessage ( device, name, warningSeverity, warningMsg );
            values [ 0 ] = data;
            values [ 1 ] = data;
        }
        return warningSeverity;
    }

    home_WarningSeverity DeviceWarnings::warning_Efficiency (std::vector <double> &values)
    {
        home_Device* device = getParentDevice();

        double perEfficiency                    = device->deviceFields ["dPerEfficiency"].get<double>();
        double perEffDoE2016                    = device->deviceFields ["dPerEffDoE2016"].get<double>();
        double perEff8021                       = device->deviceFields ["dPerEff8021"]   .get<double>();
        double perEff8023                       = device->deviceFields ["dPerEff8023"]   .get<double>();
        double perdEffGuar                      = std::max({perEffDoE2016, perEff8021, perEff8023}); // EXTRACT: max efficency loss guarantee
        
        home_WarningSeverity warningSeverity    = home_WarningSeverity::None;

        values[0]         = 0.0;
        values[1]         = 0.0;

        if (perdEffGuar == 0.0)        {    return warningSeverity; }

        if (perEfficiency < perdEffGuar)
        {
            warningSeverity = home_WarningSeverity::Error;
            
            std::string warningMsg =    "The efficiency loss ("                                                 + Config::format_Double(perEfficiency) 
                                     +  ") must be greater than or equal to the guaranteed efficiency loss ("   + Config::format_Double(perdEffGuar) 
                                     +  ").";
            std::string name       =    "tErrorEfficiency";

            addErrorMessage(device, name, warningSeverity, warningMsg);

            values[0] = perEfficiency;
            values[1] = perdEffGuar;
        }
        return warningSeverity;
    }
}
// end of namespace Transformer::Validation