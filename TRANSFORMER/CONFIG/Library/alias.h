//    @date      June 19, 2026 ~ EDIT: August 17 2026
//    @author    Tom Goncin
//    @file      alias.h
//    @brief     Created aliases to reduce convolution in code

#pragma once



namespace Transformer::Components {     class Bushing;      class Changer;  class Coil;         class Conductor;    struct DiscsResult;
                                        class Core;         class Discs;    class Mechanical;   class Winding;      struct ConductorSolution;
                                    }

namespace Transformer::Core         {   class Device;       class FieldValue;   class Designer;
                                    }

namespace Transformer::Physics      {   class Cooling;     class Gradients;    class Losses;
                                    }

namespace Transformer::Validation   {   class CoilWarnings;         class ConductorWarnings;    class CoreWarnings;
                                        class DeviceWarnings;       class GradientWarnings;     class MechanicalWarnings;
                                        class GuaranteeWarnings;    class LossWarnings;         class WindingWarnings;
                                        class Tests;
                                    }

namespace Transformer::Commercial {
                                    class Guarantee;           class Costs;             class MaterialConstants;        class ConductorTable;      class CoilInsulation3Ph; class ResistanceTable;      struct ConductorStripRow; 
                                    class SapStandardCoreLeads;class CoilBil1Ph;        class CoilBil3Ph;               class CoilInsulation;      class ResistiveMaterial; class ResistivityTable;     struct ConductorMagnetRow;
                                  }

namespace Transformer::Config       {   enum class WindingDesc;         enum class LaminationType;      enum class ConductorType;       enum class Coating;         enum class BIL;  
                                        enum class Material;            enum class Segment;             enum class WarningSeverity;     enum class EfficiencySpec;  enum class DesignSpecCodes;
                                        enum class DesignSpecStandards; enum class ChangerCapacity;     enum class ArrayType;           enum class RegulationType;  enum class CoilType;
                                        enum class Sequence;            enum class SingleCoolingType;   enum class CoolingType;         enum class ConnectionType;  enum class Coating;
                                        enum class ConductorType;       enum class CoreTolerance;       enum class Standard;            enum class TapExtremes;     enum class TemperatureRiseStandard;
                                        enum class WindingSelection;
                                    }


namespace Transformer::Types
{ 
    using home_Bushing              = Transformer::Components::     Bushing;
    using home_Changer              = Transformer::Components::     Changer;
    using home_Coil                 = Transformer::Components::     Coil;
    using home_Conductor            = Transformer::Components::     Conductor;
    using home_Core                 = Transformer::Components::     Core;
    using home_Discs                = Transformer::Components::     Discs;
    using home_Mechanical           = Transformer::Components::     Mechanical;
    using home_Winding              = Transformer::Components::     Winding;
    using home_ConductorSolution    = Transformer::Components::     ConductorSolution;
    using home_DiscsResult          = Transformer::Components::     DiscsResult;

    using home_FieldValue           = Transformer::Core::       FieldValue;
    using home_Device               = Transformer::Core::       Device;
    using home_Designer	            = Transformer::Core::       Designer;

    using home_Cooling              = Transformer::Physics::    Cooling;
    using home_Gradients            = Transformer::Physics::    Gradients;
    using home_Losses               = Transformer::Physics::    Losses;

    using home_Guarantee            = Transformer::Commercial::     Guarantee;
    using home_Costs                = Transformer::Commercial::     Costs;
    using home_MaterialConstants    = Transformer::Commercial::     MaterialConstants;
    using home_SapStandardCoreLeads = Transformer::Commercial::     SapStandardCoreLeads;
    using home_ConductorTable       = Transformer::Commercial::     ConductorTable;
    using home_ConductorStripRow    = Transformer::Commercial::     ConductorStripRow;
    using home_ConductorMagnetRow   = Transformer::Commercial::     ConductorMagnetRow;
    using home_CoilBil1Ph           = Transformer::Commercial::     CoilBil1Ph;
    using home_CoilBil3Ph           = Transformer::Commercial::     CoilBil3Ph;
    using home_CoilInsulation3Ph    = Transformer::Commercial::     CoilInsulation3Ph;
    using home_CoilInsulation       = Transformer::Commercial::     CoilInsulation;
    using home_ResistiveMaterial    = Transformer::Commercial::     ResistiveMaterial;
    using home_ResistivityTable     = Transformer::Commercial::     ResistivityTable;
    using home_ResistanceTable      = Transformer::Commercial::     ResistanceTable;

    using home_CoilWarnings         = Transformer::Validation::     CoilWarnings; 
    using home_ConductorWarnings    = Transformer::Validation::     ConductorWarnings;
    using home_CoreWarnings         = Transformer::Validation::     CoreWarnings;
    using home_DeviceWarnings       = Transformer::Validation::     DeviceWarnings;
    using home_GradientWarnings     = Transformer::Validation::     GradientWarnings; 
    using home_GuaranteeWarnings    = Transformer::Validation::     GuaranteeWarnings;
    using home_LossWarnings         = Transformer::Validation::     LossWarnings; 
    using home_MechanicalWarnings   = Transformer::Validation::     MechanicalWarnings; 
    using home_Tests                = Transformer::Validation::     Tests;
    using home_WindingWarnings      = Transformer::Validation::     WindingWarnings; 

    using home_WindingDesc          = Transformer::Config::     WindingDesc;
    using home_LaminationType       = Transformer::Config::     LaminationType;
    using home_ConductorType        = Transformer::Config::     ConductorType;
    using home_Material             = Transformer::Config::     Material;
    using home_WarningSeverity      = Transformer::Config::     WarningSeverity;
    using home_EfficiencySpec       = Transformer::Config::     EfficiencySpec;
    using home_DesignSpecCodes      = Transformer::Config::     DesignSpecCodes;
    using home_DesignSpecStandards  = Transformer::Config::     DesignSpecStandards;
    using home_RegulationType       = Transformer::Config::     RegulationType;
    using home_CoilType             = Transformer::Config::     CoilType;
    using home_TemperatureRise      = Transformer::Config::     TemperatureRiseStandard;
    using home_SingleCoolingType    = Transformer::Config::     SingleCoolingType;
    using home_CoolingType          = Transformer::Config::     CoolingType;
    using home_ConnectionType       = Transformer::Config::     ConnectionType;
    using home_BIL                  = Transformer::Config::     BIL;
    using home_ArrayType            = Transformer::Config::     ArrayType;   
    using home_Segment              = Transformer::Config::     Segment;
    using home_ChangerCapacity      = Transformer::Config::     ChangerCapacity;
    using home_Coating              = Transformer::Config::     Coating;
    using home_CoreTolerance        = Transformer::Config::     CoreTolerance;
    using home_Sequence             = Transformer::Config::     Sequence;
    using home_Standard             = Transformer::Config::     Standard;
    using home_TapExtremes          = Transformer::Config::     TapExtremes; 
    using home_WindingSelection     = Transformer::Config::     WindingSelection;
}
