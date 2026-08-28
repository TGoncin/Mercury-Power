/**___________________________________________________________________________________________________
|    @date      May 19, 2026 ~ EDIT: August 26, 2026
|    @author    Tom Goncin
|    @file      designer.cpp
|    @brief     Coordinator transformer execution, talks to serializer framework
|    @brief    Transformer::Core       is "The Engine Framework"
|               copyright   © PTI Transformers
|
|               Executes:
|                  - Initialization of transformer object
|                  - Maps entire devices macro-structure
|                  - Evaluates multivariable/global engineering penalty metrics
|
|               Heavily references & instantiates micro frameworks (Dependancies):
|                  - Complex custom data structures [e.g. dev.Device() ]
|                  - Warning managers
|                  - Serializer framework:      Translates complex, in-memory data structures/objects into standardized format
|___________________________________________________________________________________________________*/
#pragma once

#include <vector>
#include <string>
#include <memory>

#include "device.h"


namespace Transformer::Core
{
    class Designer
    {
        public:
            double  cost;
            double  penalty;
            double  warning;
            bool    validDesign;
            int     penaltyLevel;

            std::vector<double> listSolutions_LV;
            std::vector<double> listSolutions_HV;

            home_Device* device; // Using the actual project type

            explicit Designer(home_Device* deviceE = nullptr);

            double designSinglePhase();
            void mechanicalDevice();
            void designDevice();
            void calculateWarning();
            void calculatePenalty();
            void setPenalty(int iLevel);


//                  GETTERS

        double getCost() const      { return cost; }
        double getPenalty() const   { return penalty; }
        double getWarning() const   { return warning; }
        bool getValidDesign() const { return validDesign; }
        int getPenaltyLevel() const { return penaltyLevel; }

        std::vector<double> getListSolutions_LV() const { return listSolutions_LV; }
        std::vector<double> getListSolutions_HV() const { return listSolutions_HV; }
        home_Device* getDevice() const { return device; }


//                  SETTERS

        void setCost(double v)      { cost = v; }
        void setWarning(double v)   { warning = v; }
        void setValidDesign(bool v) { validDesign = v; }
        void setPenaltyLevel(int v) { penaltyLevel = v; }

        void setListSolutions_LV(const std::vector<double>& v) { listSolutions_LV = v; }
        void setListSolutions_HV(const std::vector<double>& v) { listSolutions_HV = v; }
        void setDevice(home_Device* v) { device = v; }
    };
}