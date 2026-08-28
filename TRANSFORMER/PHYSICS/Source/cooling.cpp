/**___________________________________________________________________________________________________
|    @date      May 26, 2026
|    @author    Tom Goncin
|    @file      Cooling.cpp
|    @brief     Specialized mathematical tracking engine evaluating transformer cooling states.
|    @brief     Transformer::Physics    is for "Behavior & Calculations"
|___________________________________________________________________________________________________*/
#include <sstream>
#include <iomanip>

#include "alias.h"
using namespace Transformer::Types;

#include "cooling.h"
#include "winding.h"
#include "device.h"
#include "gradient.h"
#include "mechanical.h"

#include "coil.h"



namespace Transformer::Physics
{
    Cooling::Cooling(home_Device *parentDevice) : device(parentDevice)
    {
        coolingFields["iCoolingStepsReal"]          = FieldValue (1);
        coolingFields["iCoolingSteps"]              = FieldValue (1);
        coolingFields["t_CoolingType"]              = FieldValue (FieldValue::List (5, static_cast<int>    (SingleCoolingType::None)));
        coolingFields["tCoolingTypes"]              = FieldValue (static_cast<int> (home_CoolingType::None));
        coolingFields["tTemperatureRise"]           = FieldValue (FieldValue::List (5, static_cast<int>    (TemperatureRiseStandard::None)));
        coolingFields["tTemperatureRises"]          = FieldValue (static_cast<int> (TemperatureRiseStandard::None));
        coolingFields["sTempCoolRise"]              = FieldValue (FieldValue::List (5, ""));
        coolingFields["tProvONAF"]                  = FieldValue (static_cast<int> (false));
        coolingFields["tK4Factor"]                  = FieldValue (static_cast<int> (false));
        coolingFields["t_CoolingTypeK4"]            = FieldValue (FieldValue::List (5, static_cast<int>(SingleCoolingType::None)));
        coolingFields["tTemperatureRiseStandardK4"] = FieldValue (FieldValue::List (5, static_cast<int>(TemperatureRiseStandard::None)));
        coolingFields["dNewTempRise"]               = FieldValue (0.0);
        coolingFields["dCurrentempRise"]            = FieldValue (0.0);
        coolingFields["sUseFR3"]                    = FieldValue ("");
        coolingFields["dMaxONAF"]                   = FieldValue (FieldValue::List (3, 0.0));
        coolingFields["dTORONAF"]                   = FieldValue (FieldValue::List (3, 0.0));
        coolingFields["tTempONAF"]                  = FieldValue (FieldValue::List (3, static_cast<int>(TemperatureRiseStandard::None)));
        coolingFields["overExcitation"]             = FieldValue (static_cast<int> (false));
        coolingFields["tTempOExc"]                  = FieldValue (static_cast<int> (TemperatureRiseStandard::None));
        coolingFields["dMVAOExc"]                   = FieldValue (FieldValue::List (5, 0.0));
        coolingFields["sMVAOExc"]                   = FieldValue ("");
        coolingFields["sErrorName"]                 = FieldValue (FieldValue::List(ERRORMESSAGES, ""));
        coolingFields["tErrorEnum"]                 = FieldValue (FieldValue::List(ERRORMESSAGES, static_cast<int>(WarningSeverity::None)));
        coolingFields["sErrorMessages"]             = FieldValue (FieldValue::List(ERRORMESSAGES, ""));
        coolingFields["sCoolingStepsList"]          = FieldValue (FieldValue::List(0, ""));
        coolingFields["type_cooling"]               = FieldValue (FieldValue::List(5, ""));
    }

    home_Device *Cooling::getParentDevice() const
    {
        if (!device)        throw std::runtime_error("This conductor is not attached to any Device");
        return device;
    }

    void Cooling::reset_Fields()
    {
        for (auto& pair : coolingFields)
        {
            try
            {
                pair.second.reset();
            }
            catch (...) {}
        }
    }
    void Cooling::erase_Warnings()
    {
        for (int i = 0; i < ERRORMESSAGES; ++i)
        {
            coolingFields["sErrorName"].set("", i);
            coolingFields["tErrorEnum"].set(static_cast<int>(home_WarningSeverity::None), i);
            coolingFields["sErrorMessages"].set("", i);
        }
    }
    void Cooling::calc_Cooling()
    {
        home_Device*            device          = getParentDevice();
        home_DesignSpecCodes    designSpecCodes = device->deviceFields["DesignSpecCodes"].get<home_DesignSpecCodes>();
        home_Mechanical*        mechanical      = device->mechanical.get();
        home_Gradients*         gradients       = device->gradients.get();
        auto*                   windingHv       = device->get_WindingHV();

        double nominalKva = device->cooling->coolingFields["nominal_kva"].get<double>();

        calc_CoolingType();
        calc_TempRise();
        calc_ONAF();
        calc_MVAOExc(windingHv, designSpecCodes, nominalKva);
        calc_NewCurrentTemp();
        calc_UseFR3();
        calc_MaxONAF(mechanical);
        calc_TORONAF(gradients);
    }
    void Cooling::calc_CoolingType()
    {
        auto& objectsFields  = coolingFields;
        auto  coolingTypes   = objectsFields["tCoolingTypes"].get<CoolingType>();

        if (coolingTypes == CoolingType::None)                  return;

        auto TemperatureRiseStandardsVal   = objectsFields["tTemperatureRiseStandards"].get<TemperatureRiseStandard>();
        bool overExcitation        = (TemperatureRiseStandardsVal == TemperatureRiseStandard::Temp_55_65);

        int steps = 1;

        std::vector<SingleCoolingType> types;

        switch (coolingTypes)
        {
            case CoolingType::ONAN:
                steps = 1;
                types = overExcitation ?  std::vector<SingleCoolingType>   { SingleCoolingType::ONAN, SingleCoolingType::ONAN }
                                        : std::vector<SingleCoolingType>   { SingleCoolingType::ONAN };
                break;

            case CoolingType::ONAN_ONAF:
                steps = 2;
                types = overExcitation ?  std::vector<SingleCoolingType>   {   SingleCoolingType::ONAN, 
                                                                               SingleCoolingType::ONAF, 
                                                                               SingleCoolingType::ONAF
                                                                           }
                                        : std::vector<SingleCoolingType> { SingleCoolingType::ONAN, SingleCoolingType::ONAF };
                break;

            case CoolingType::ONAN_ONAF_ONAF:
                steps = 3;
                types = overExcitation ? std::vector<SingleCoolingType> { SingleCoolingType::ONAN, 
                                                                          SingleCoolingType::ONAF, 
                                                                          SingleCoolingType::ONAF, 
                                                                          SingleCoolingType::ONAF
                                                                        }
                                        : std::vector<SingleCoolingType> { SingleCoolingType::ONAN, 
                                                                           SingleCoolingType::ONAF, 
                                                                           SingleCoolingType::ONAF
                                                                        };
                break;

            case CoolingType::KNAN:
                steps = 1;
                types = overExcitation     ?  std::vector<SingleCoolingType> { SingleCoolingType::KNAN, SingleCoolingType::KNAN } 
                                            : std::vector<SingleCoolingType> { SingleCoolingType::KNAN };
                break;

            case CoolingType::KNAN_KNAF:
                steps = 2;
                types = overExcitation ?  std::vector<SingleCoolingType> { SingleCoolingType::KNAN, SingleCoolingType::KNAF, SingleCoolingType::KNAF }
                                        : std::vector<SingleCoolingType> { SingleCoolingType::KNAN, SingleCoolingType::KNAF };
                break;

            case CoolingType::KNAN_KNAF_KNAF:
                steps = 3;
                types = overExcitation ?  std::vector<SingleCoolingType> {  SingleCoolingType::KNAN, 
                                                                            SingleCoolingType::KNAF, 
                                                                            SingleCoolingType::KNAF, 
                                                                            SingleCoolingType::KNAF 
                                                                        }
                                        : std::vector<SingleCoolingType> {  SingleCoolingType::KNAN, 
                                                                            SingleCoolingType::KNAF, 
                                                                            SingleCoolingType::KNAF
                                                                        };
                break;

            default:
                break;
        }

        objectsFields["iCoolingStepsReal"].set(steps);

        if (overExcitation)
        {
            steps += 1;
        }

        objectsFields["iCoolingSteps"].set(steps);

        for (size_t i = 0; i < types.size(); ++i)
        {
            objectsFields["t_CoolingType"].set(static_cast<int>(types[i]), static_cast<int>(i)); 
        }

        if (objectsFields["tK4Factor"].get<bool>() == true)
        {
            for (size_t i = 0; i < types.size(); ++i)
            {
                objectsFields["t_CoolingTypeK4"].set(static_cast<int>(types[i]), static_cast<int>(i)); 
            }
        }
    }
    void Cooling::calc_TempRise()
    {
    auto &coolingField      = coolingFields;
    auto TemperatureRiseStandards   = coolingField["tTemperatureRiseStandards"].get<TemperatureRiseStandard>();
    int coolingSteps        = coolingField["iCoolingSteps"].get<int>();

    if (TemperatureRiseStandards == TemperatureRiseStandard::None)      {    return;    }

    std::vector<TemperatureRiseStandard> tempRise;

    if (TemperatureRiseStandards == TemperatureRiseStandard::Temp_55_65)
    {
        for (int i = 0; i < coolingSteps; ++i)
        {
            tempRise.push_back((i == 0) ? TemperatureRiseStandard::Temp_55 : TemperatureRiseStandard::Temp_65);
        }
    }
    else if (TemperatureRiseStandards == TemperatureRiseStandard::Temp_55)
    {
        for (int i = 0; i < coolingSteps; ++i)
        {
            tempRise.push_back(TemperatureRiseStandard::Temp_55);  
        }
    }
    else if (TemperatureRiseStandards == TemperatureRiseStandard::Temp_65)
    {
        for (int i = 0; i < coolingSteps; ++i)
        {
            tempRise.push_back(TemperatureRiseStandard::Temp_65);
        }
    }

    auto cooling_types = coolingFields["t_CoolingType"].get<std::vector<FieldValue::Value>>();

    for (size_t i = 0; i < tempRise.size(); ++i)
    {
        coolingFields["tTemperatureRiseStandard"].set(static_cast<int>(tempRise[i]), static_cast<int>(i));

        std::stringstream sStream;
        sStream << static_cast<int>(tempRise[i]) << " C " << to_string(static_cast<SingleCoolingType>(std::get<int>(cooling_types[i])));
        coolingFields["sTempCoolRise"].set(sStream.str(), static_cast<int>(i));
    }

    if (coolingFields["tK4Factor"].get<bool>() == true)
    {
        for (size_t i = 0; i < tempRise.size(); ++i)
        {
            coolingFields["tTemperatureRiseStandardK4"].set(static_cast<int>(tempRise[i]), static_cast<int>(i));
            coolingFields["sTempCoolRise"].set(std::to_string(static_cast<int>(tempRise[i])) + " C PER K4 FACTOR", static_cast<int>(i + 1));
        }
    }
    }
    void Cooling::calc_ONAF()
    {
        auto&   coolingField     = coolingFields;
        auto    TemperatureRiseStandards = coolingField["tTemperatureRiseStandards"].get<TemperatureRiseStandard>();

        int              coolingSteps     = coolingField["iCoolingSteps"].get<int>();
        bool             overExcitation   = false;
        FieldValue::List result (3, static_cast<int> (TemperatureRiseStandard::None));

        if (coolingSteps == 2)
        {
            if (TemperatureRiseStandards == TemperatureRiseStandard::Temp_55)
            {
                result = {  static_cast<int>(TemperatureRiseStandard::Temp_55), 
                            static_cast<int>(TemperatureRiseStandard::Temp_55), 
                            static_cast<int>(TemperatureRiseStandard::None)         };
            }
            else if (TemperatureRiseStandards == TemperatureRiseStandard::Temp_55_65)
            {
                result = {  static_cast<int>(TemperatureRiseStandard::Temp_55), 
                            static_cast<int>(TemperatureRiseStandard::Temp_65), 
                            static_cast<int>(TemperatureRiseStandard::None)         };
                overExcitation = true;
            }
            else if (TemperatureRiseStandards == TemperatureRiseStandard::Temp_65)
            {
                result = {  static_cast<int>(TemperatureRiseStandard::Temp_65), 
                            static_cast<int>(TemperatureRiseStandard::Temp_65),
                            static_cast<int>(TemperatureRiseStandard::None)};       }
        }
        else if (coolingSteps == 3)
        {
            if (TemperatureRiseStandards == TemperatureRiseStandard::Temp_55)
            {
                result = {  static_cast<int>(TemperatureRiseStandard::Temp_55), 
                            static_cast<int>(TemperatureRiseStandard::Temp_55),
                            static_cast<int>(TemperatureRiseStandard::Temp_55)};    }

            else if (TemperatureRiseStandards == TemperatureRiseStandard::Temp_55_65)
            {
                result = {  static_cast<int>(TemperatureRiseStandard::Temp_55), 
                            static_cast<int>(TemperatureRiseStandard::Temp_55), 
                            static_cast<int>(TemperatureRiseStandard::Temp_65)      };
                overExcitation = true;

            }
            else if (TemperatureRiseStandards == TemperatureRiseStandard::Temp_65)
            {
                result = {  static_cast<int>(TemperatureRiseStandard::Temp_65), 
                            static_cast<int>(TemperatureRiseStandard::Temp_65), 
                            static_cast<int>(TemperatureRiseStandard::Temp_65)      };
            }
        }
        else
        {
            if (TemperatureRiseStandards == TemperatureRiseStandard::Temp_55)
            {
                result = {  static_cast<int>(TemperatureRiseStandard::Temp_55), 
                            static_cast<int>(TemperatureRiseStandard::None),
                            static_cast<int>(TemperatureRiseStandard::None)
                         };
            }
            else if (TemperatureRiseStandards == TemperatureRiseStandard::Temp_55_65)
            {
                result = {
                            static_cast<int>(TemperatureRiseStandard::Temp_55), 
                            static_cast<int>(TemperatureRiseStandard::Temp_65),
                            static_cast<int>(TemperatureRiseStandard::None)
                            };
                overExcitation = true;
            }
            else if (TemperatureRiseStandards == TemperatureRiseStandard::Temp_65)
            {
                result = {
                            static_cast<int>(TemperatureRiseStandard::Temp_65),
                            static_cast<int>(TemperatureRiseStandard::None),
                            static_cast<int>(TemperatureRiseStandard::None)
                            };
            }
        }

        coolingField["tTempONAF"]       .set(result);
        coolingField["overExcitation"]  .set(static_cast<int>(overExcitation));
        coolingField["tTempOExc"]       .set(static_cast<int>(TemperatureRiseStandard::Temp_65));
    }
    void Cooling::calc_MVAOExc(home_Winding *windingHV, home_DesignSpecCodes designSpecCodes, double nominalKva)
    {
        int     coolingSteps     = coolingFields["iCoolingSteps"].get<int>();
        auto    coolingType      = coolingFields["tCoolingTypes"].get<CoolingType>();
        double  base             = nominalKva * 1.12 / 1000.0;

        auto &dMVAOExc           = coolingFields["dMVAOExc"];
        std::string resultText   = "";

        auto round_to_two        = [](double val)      {   return std::round(val * 100.0) / 100.0; };

        if (coolingFields["overExcitation"].get<bool>() == true)
        {
            if (coolingType == CoolingType::ONAN_ONAF || coolingType == CoolingType::KNAN_KNAF)
            {
                double first  = round_to_two(base);
                double second = 0.0;

                if (designSpecCodes == DesignSpecCodes::ANSI)
                {
                    second = (nominalKva <= 2000.0) ? round_to_two(base * 1.15) : ((nominalKva <= 10000.0) ? round_to_two(base * 1.25) : 0.0);
                }
                else        {   second = round_to_two(base * 4.0 / 3.0);    }

                std::stringstream sStream;
                sStream << std::fixed << std::setprecision(2) << first << " / " << second;
                resultText = sStream.str();

                dMVAOExc.set(first * 1000.0, 0);
                dMVAOExc.set(second * 1000.0, 1);
            }
            else if (coolingType == CoolingType::ONAN_ONAF_ONAF || coolingType == CoolingType::KNAN_KNAF_KNAF)
            {
                double first = round_to_two(base);
                
                double second = (designSpecCodes == DesignSpecCodes::ANSI)   ? ((nominalKva <= 2000.0) 
                                                                    ? round_to_two(base * 1.15) 
                                                                    : round_to_two(base * 1.25)) 
                                                                    : round_to_two(base * 4.0 / 3.0);

                double third = (designSpecCodes == DesignSpecCodes::ANSI)    ? ((nominalKva <= 2000.0) 
                                                                    ? round_to_two(base * 4.0 / 3.0) 
                                                                    : round_to_two(base * 1.5)) 
                                                                    : round_to_two(base * 5.0 / 3.0);

                std::stringstream sStream;
                sStream << std::fixed << std::setprecision(2) << first << " / " << second << " / " << third;
                resultText = sStream.str();

                dMVAOExc.set(first * 1000.0, 0);
                dMVAOExc.set(second * 1000.0, 1);
                dMVAOExc.set(third * 1000.0, 2);
            }
            else
            {
                double result = nominalKva * 1.12;

                dMVAOExc.set(result, 0);

                std::stringstream sStream;
                sStream << std::fixed << std::setprecision(1) << result;
                resultText = sStream.str();
            }
            coolingFields["sMVAOExc"].set(resultText);
        }
        else
        {
            if (coolingSteps > 1)
            {
                auto kva = coolingFields["d_kVA"].get<std::vector<double>>();

                for (size_t i = 0; i < kva.size(); ++i)
                {
                    dMVAOExc.set(kva[i], static_cast<int>(i));       
                }
            }
            else             {  dMVAOExc.reset();   }

            coolingFields["sMVAOExc"].reset();
        }
    }
    void Cooling::calc_UseFR3()        {    coolingFields["sUseFR3"].set(isKNAN() ? "Use of FR3" : "");   }
    void Cooling::calc_NewCurrentTemp()
    {
        double newTemp        = 20.0;
        double currentTemp    = calc_TempRiseNumber() + newTemp;

        coolingFields["dNewTempRise"].set(newTemp);
        coolingFields["dCurrentempRise"].set(currentTemp);
    }
    void Cooling::calc_MaxONAF(home_Mechanical* mechanical)
        {
        int coolingSteps    = coolingFields["iCoolingSteps"].get<int>();
        bool k4Factor       = (coolingFields["tK4Factor"].get<bool>() == true);
        int factor          = k4Factor ? 1 : 0;

        if (coolingSteps > 1)
        {
            auto kw1 = coolingFields["kw1"].get<std::vector<double>>();
            
            for (int step = 1; step < coolingSteps; ++step)
            {
                coolingFields["dMaxONAF"].set(kw1[step - factor] * 1000.0, step);
            }
        }
        else if (coolingSteps == 1 && k4Factor)
        {
            auto kw1 = mechanical->mechanicalFields["d_kW1"].get<std::vector<double>>();

            coolingFields["dMaxONAF"].set(kw1[0] * 1000.0, 1);
        }
    }
    void Cooling::calc_TORONAF(home_Gradients *gradients)
    {
        int coolingSteps = coolingFields["iCoolingSteps"].get<int>();

        if (coolingSteps > 1)
        {
            auto TORFinal = coolingFields["TORFinal"].get<std::vector<double>>();

            coolingFields["dTORONAF"].set(TORFinal[0], 0);
        }
    }
    bool Cooling::isKNAN() const
    {
        auto coolingTypes = coolingFields.at("tCoolingTypes").get<CoolingType>();

        return ( coolingTypes == CoolingType::KNAN  ||  coolingTypes == CoolingType::KNAN_KNAF  ||  coolingTypes == CoolingType::KNAN_KNAF_KNAF );
    }
    int Cooling::calc_TempRiseNumber() const
    {
        auto tempRises = coolingFields.at("tTemperatureRiseStandards").get<TemperatureRiseStandard>();

        if (tempRises == TemperatureRiseStandard::Temp_55)         return 55;
        if (tempRises == TemperatureRiseStandard::Temp_65)         return 65;
        if (tempRises == TemperatureRiseStandard::Temp_55_65)      return 55;

        return 0;
    }

    void Cooling::add_Step(const std::string& step)
    {
        auto steps = coolingFields["sCoolingStepsList"].get<std::vector<std::string>>();
        steps.push_back(step);

        coolingFields["sCoolingStepsList"].set(steps);
    }

    void Cooling::clear_Steps()
    {
        std::vector<std::string> emptyList;
        coolingFields["sCoolingStepsList"].set(emptyList);
    }

    void Cooling::set_CoolingType_At(int index, const std::string& value)
    {
        if (index >= 0 && index < 5)
        {
            auto types = coolingFields["type_cooling"].get<std::vector<std::string>>();
            types[index] = value;

            coolingFields["type_cooling"].set(types);
        }
        else        {   throw std::out_of_range("Index must be between 0 and 4");   }
    }
}
// end of home_cooling