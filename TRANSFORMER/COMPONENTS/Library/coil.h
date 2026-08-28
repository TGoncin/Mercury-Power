/**___________________________________________________________________________________________________
|    @date      May 26, 2026 ~ EDIT: August 17 2026
|    @author    Tom Goncin
|    @file      Coil.h
|    @brief     Calculation architecture resolving transformer coil mechanical properties.
|    @brief     Transformer::Components     is "Physical Hardware"
|___________________________________________________________________________________________________*/
#pragma once

#include <string>
#include <vector>
#include <map>
#include <memory>
#include <cmath>
#include <algorithm>

#include "alias.h"
#include "field_Value.h"
#include "enums.h"
#include "materials.h"
#include "coil_Warnings.h"
#include "conductor.h"

using namespace Transformer::Types;
using namespace Transformer::Core;


namespace Transformer::Components
{
    class Winding;

    class Coil
    {
        private:
            static int id_Counter;

        public:
            std::map <std::string, FieldValue>  coilFields;
            std::map <std::string, double>      initialFieldValues;
            std::vector<home_Conductor>         conductor;

            home_CoilBil1Ph                     table_CoilBil1Ph;
            home_CoilBil3Ph                     table_CoilBil3Ph;
            home_CoilInsulation                 table_CoilInsulation;
            home_CoilInsulation3Ph              table_CoilInsulation3Ph;
            home_Device*                        device;
            home_Winding*                       winding;
            home_MaterialConstants              table_MaterialConstants;
            home_ConductorTable                 table_Conductor;

            Coil (  home_Device*           device             = nullptr, 
                    home_CoilType          coilType           = home_CoilType::Default, 
                    home_RegulationType    regulationType     = home_RegulationType::None, 
                    home_MaterialConstants materialConstants  = {}, 
                    home_CoilBil1Ph        table_CoilBil1Ph   = {}, 
                    home_CoilBil3Ph        table_CoilBil3Ph   = {}, 
                    home_ConductorTable    table_Conductor    = {}
                 );

            home_Device*  parentDevice() const;
            home_Winding* getParentWinding() const;

            size_t numberConductors() const;

            std::string calc_DefaultInsulationEnd   (home_Winding* winding, int windingType, int numberPhases);
            std::string calc_DefaultInsulationInt   (int windingType);

            int get_Position() const;
            int calc_DefaultTotalNumberEndLayers    (Winding* winding);
            int calc_SectionsLeg                    (home_Winding* winding);

            double calc_DefaultOvercoil         (int windingType, home_Winding* wind_Lo);
            double calc_DefaultTurnsSection     (int windingType);
            double calc_DefaultTurnsLayers      (const home_Conductor& conductor, int windingType);
            double calc_DefaultNumberLayersBody (home_Winding* winding, const home_Conductor& conductor, int windingType);
            double calc_DefaultTotalLayers();
            double calc_DefaultInterlayerPaper  (home_Device* device, home_Winding* winding, const home_Conductor& conductor, int windingType, home_DesignSpecStandards designSpecStandards, int numberPhases );
            double calc_TotalLayers();
            double calc_DefaultEndPaper         (int windingType, home_DesignSpecStandards designSpecStandards, int numberPhases);
            double calc_ImpkV();
            double calc_InsulationFirst         (home_DesignSpecStandards designSpecStandards);
            double calc_InsulationBody          (int numberPhases, home_DesignSpecStandards designSpecStandards);
            double calc_MinInsulationMechanical (int windingType, double totalThickTurn);
            double calc_InterPaper              (int windingType, home_DesignSpecStandards designSpecStandards, int numberPhases);
            
            void assign_Index                   (int index);
            void assign_WindingDesc             (home_WindingDesc  windingDesc);
            void assign_Winding                 (home_Winding*     winding);
            void assign_Parents();
            void assign_Defaults();
            void reset_Fields();
            void erase_Warnings();
            void remove_Winding();
            void copy                           (const Coil& otherCoil);
            void getInfo_CoilTable              (home_Device* device);
            void calc_Warnings();
            void calc_OverOuterPreliminar       (home_Winding* winding);
            void calc_Coil();
            void calc_Kvmm();
            void calc_RequiredNumberDucts       (home_Winding* winding, int windingType);
            void calc_Ducts                     (home_Winding* winding, int windingType, home_Coil* coil, const home_Conductor& conductor, int numberPhases);
            void calc_WidthDucts                (home_Device* device, home_Winding* winding);
            void calc_StringDucts               (home_Winding* winding, int windingType);
            void calc_RadialBuilt();
            void calc_RBSides                   (home_Conductor* conductor, home_Winding* winding, int windingType, int numberPhases);
            void calc_RadialBuiltInNoses        (home_Winding* winding, int windingType);
            void calc_RadialBuiltOutNoses       (home_Winding* winding, int windingType);
            void calc_Height();
            void calc_Width();
            void calc_ConductorThicknessPerTurn();
            void calc_ConductorWidthTurn();
            void calc_Turns                (Winding* winding);
            void calc_OverOuter            (home_Device* device, Winding* winding, int windingType);

        // _________________________________________________________________________________________
        // |                                      GETTERS                                          |
        // |_______________________________________________________________________________________|

            int                             getId                           () const { return coilFields.at("id")                           .get<int>();                                        }
            std::string                     getName                         () const { return coilFields.at("name")                         .get<std::string>();                                }
            int                             getIBil                         () const { return coilFields.at("iBil")                         .get<int>();                                        }
            int                             getTDescDevanado                () const { return coilFields.at("tDescDevanado")                .get<int>();                                        }
            std::vector<double>             getIBiles                       () const { return coilFields.at("i_Biles")                      .get<std::vector<double>>();                        }
            double                          getDTurns                       () const { return coilFields.at("dTurns")                       .get<double>();                                     }
            int                             getILayers                      () const { return coilFields.at("iLayers")                      .get<int>();                                        }
            int                             getISectionsLeg                 () const { return coilFields.at("iSectionsLeg")                 .get<int>();                                        }
            bool                            getBConstantStayback            () const { return coilFields.at("bConstantStayback")            .get<bool>();                                       }
            int                             getTRegulationType              () const { return coilFields.at("tRegulationType")              .get<int>();                                        }
            int                             getTCoilType                    () const { return coilFields.at("tCoilType")                    .get<int>();                                        }
            double                          getDStaybak                     () const { return coilFields.at("dStaybak")                     .get<double>();                                     }
            double                          getDCoolingDuct                 () const { return coilFields.at("dCoolingDuct")                 .get<double>();                                     }
            double                          getDCoreProtection              () const { return coilFields.at("dCoreProtection")              .get<double>();                                     }
            double                          getDFormDetails                 () const { return coilFields.at("dFormDetails")                 .get<double>();                                     }
            double                          getDFormDuct                    () const { return coilFields.at("dFormDuct")                    .get<double>();                                     }
            double                          getDForm001Paper                () const { return coilFields.at("dForm001Paper")                .get<double>();                                     }
            double                          getDUnderHV001Paper             () const { return coilFields.at("dUnderHV001Paper")             .get<double>();                                     }
            double                          getDUOHV001Paper                () const { return coilFields.at("dUOHV001Paper")                .get<double>();                                     }
            double                          getDUOHVDuct                    () const { return coilFields.at("dUOHVDuct")                    .get<double>();                                     }
            double                          getDUOHVCrepePaper              () const { return coilFields.at("dUOHVCrepePaper")              .get<double>();                                     }
            double                          getDUOHV001Paper2               () const { return coilFields.at("dUOHV001Paper2")               .get<double>();                                     }
            double                          getDCoilCoreDuct                () const { return coilFields.at("dCoilCoreDuct")                .get<double>();                                     }
            double                          getDCoilCorePacking             () const { return coilFields.at("dCoilCorePacking")             .get<double>();                                     }
            double                          getDCoilCoilDuct                () const { return coilFields.at("dCoilCoilDuct")                .get<double>();                                     }
            double                          getDCoilCoilPacking             () const { return coilFields.at("dCoilCoilPacking")             .get<double>();                                     }
            double                          getDCoilCoilDuct2               () const { return coilFields.at("dCoilCoilDuct2")               .get<double>();                                     }
            std::vector<double>             getPdPhaseAmp                   () const { return coilFields.at("pdPhaseAmp")                   .get<std::vector<double>>();                        }
            std::vector<double>             getPdPhaseAmpSinRel             () const { return coilFields.at("pdPhaseAmpSinRel")             .get<std::vector<double>>();                        }
            double                          getILongPhaseAmp                () const { return coilFields.at("iLongPhaseAmp")                .get<double>();                                     }
            std::vector<double>             getPdPhaseAmpDifPos             () const { return coilFields.at("pdPhaseAmpDifPos")             .get<std::vector<double>>();                        }
            double                          getDOvercoil                    () const { return coilFields.at("dOvercoil")                    .get<double>();                                     }
            double                          getDTurnsSection                () const { return coilFields.at("dTurnsSection")                .get<double>();                                     }
            int                             getITurnsLayers                 () const { return coilFields.at("iTurnsLayers")                 .get<int>();                                        }
            double                          getDNoLayersUnder               () const { return coilFields.at("dNoLayersUnder")               .get<double>();                                     }
            double                          getDNoLayersBody                () const { return coilFields.at("dNoLayersBody")                .get<double>();                                     }
            double                          getDNoLayersOver                () const { return coilFields.at("dNoLayersOver")                .get<double>();                                     }
            double                          getDTotLayers                   () const { return coilFields.at("dTotLayers")                   .get<double>();                                     }
            std::vector<int>                getIDucts                       () const { return coilFields.at("i_Ducts")                      .get<std::vector<int>>();                           }
            std::string                     getDuctsIn                      () const { return coilFields.at("ductsIn")                      .get<std::string>();                                }
            std::string                     getDuctsOut                     () const { return coilFields.at("ductsOut")                     .get<std::string>();                                }
            double                          getDWidthDucts                  () const { return coilFields.at("dWidthDucts")                  .get<double>();                                     }
            std::string                     getSWidthDucts                  () const { return coilFields.at("sWidthDucts")                  .get<std::string>();                                }
            double                          getDInterPaper                  () const { return coilFields.at("dInterPaper")                  .get<double>();                                     }
            double                          getDEndPaper                    () const { return coilFields.at("dEndPaper")                    .get<double>();                                     }
            int                             getITotNoEndLayers              () const { return coilFields.at("iTotNoEndLayers")              .get<int>();                                        }
            std::string                     getSInsulationEnd               () const { return coilFields.at("sInsulationEnd")               .get<std::string>();                                }
            std::string                     getSPreInsEnd                   () const { return coilFields.at("sPreInsEnd")                   .get<std::string>();                                }
            std::string                     getSInsulationInt               () const { return coilFields.at("sInsulationInt")               .get<std::string>();                                }
            std::string                     getSInsulationIntComplete       () const { return coilFields.at("sInsulationIntComplete")       .get<std::string>();                                }
            double                          getIRequiredNumDucts            () const { return coilFields.at("iRequiredNumDucts")            .get<double>();                                     }
            std::vector<std::string>        getSOverOuterPreliminar         () const { return coilFields.at("s_OverOuterPreliminar")        .get<std::vector<std::string>>();                   }
            std::string                     getSOver                        () const { return coilFields.at("sOver")                        .get<std::string>();                                }
            double                          getDkVmm                        () const { return coilFields.at("dkVmm")                        .get<double>();                                     }
            double                          getDRadialBuilt                 () const { return coilFields.at("dRadialBuilt")                 .get<double>();                                     }
            std::string                     getSDescriptionTb               () const { return coilFields.at("sDescriptionTb")               .get<std::string>();                                }
            double                          getDRadialBuiltSides            () const { return coilFields.at("dRadialBuiltSides")            .get<double>();                                     }
            double                          getDRadialBuiltIntNoses         () const { return coilFields.at("dRadialBuiltIntNoses")         .get<double>();                                     }
            double                          getDRadialBuiltOutNoses         () const { return coilFields.at("dRadialBuiltOutNoses")         .get<double>();                                     }
            std::string                     getSRadialBuilds                () const { return coilFields.at("sRadialBuilds")                .get<std::string>();                                }
            int                             getIHigh                        () const { return coilFields.at("iHigh")                        .get<int>();                                        }
            int                             getIWide                        () const { return coilFields.at("iWide")                        .get<int>();                                        }
            double                          getDCondThickTurn               () const { return coilFields.at("dCondThickTurn")               .get<double>();                                     }
            double                          getDCondWdthTurn                () const { return coilFields.at("dCondWdthTurn")                .get<double>();                                     }
            std::vector<std::string>        getSErrorName                   () const { return coilFields.at("sErrorName")                   .get<std::vector<std::string>>();                   }
            std::vector<int>                getTErrorEnum                   () const { return coilFields.at("tErrorEnum")                   .get<std::vector<int>>();                           }
            std::vector<std::string>        getSErrorMessages               () const { return coilFields.at("sErrorMessages")               .get<std::vector<std::string>>();                   }

        // _________________________________________________________________________________________
        // |                                      SETTERS                                          |
        // |_______________________________________________________________________________________|

            void setId                          (int v)                                     { coilFields["id"]                          = home_FieldValue(v);                               }
            void setName                        (const std::string& v)                      { coilFields["name"]                        = home_FieldValue(v);                               }
            void setIBil                        (int v)                                     { coilFields["iBil"]                        = home_FieldValue(v);                               }
            void setTDescDevanado               (int v)                                     { coilFields["tDescDevanado"]               = home_FieldValue(v);                               }
            void setIBiles                      (const std::vector<double>& v)              { coilFields["i_Biles"]                     = home_FieldValue(home_FieldValue::List(v.begin(), v.end())); }
            void setDTurns                      (double v)                                  { coilFields["dTurns"]                      = home_FieldValue(v);                               }
            void setILayers                     (int v)                                     { coilFields["iLayers"]                     = home_FieldValue(v);                               }
            void setISectionsLeg                (int v)                                     { coilFields["iSectionsLeg"]                = home_FieldValue(v);                               }
            void setBConstantStayback           (bool v)                                    { coilFields["bConstantStayback"]           = home_FieldValue(v);                               }
            void setTRegulationType             (int v)                                     { coilFields["tRegulationType"]             = home_FieldValue(v);                               }
            void setTCoilType                   (int v)                                     { coilFields["tCoilType"]                   = home_FieldValue(v);                               }
            void setDStaybak                    (double v)                                  { coilFields["dStaybak"]                    = home_FieldValue(v);                               }
            void setDCoolingDuct                (double v)                                  { coilFields["dCoolingDuct"]                = home_FieldValue(v);                               }
            void setDCoreProtection             (double v)                                  { coilFields["dCoreProtection"]             = home_FieldValue(v);                               }
            void setDFormDetails                (double v)                                  { coilFields["dFormDetails"]                = home_FieldValue(v);                               }
            void setDFormDuct                   (double v)                                  { coilFields["dFormDuct"]                   = home_FieldValue(v);                               }
            void setDForm001Paper               (double v)                                  { coilFields["dForm001Paper"]               = home_FieldValue(v);                               }
            void setDUnderHV001Paper            (double v)                                  { coilFields["dUnderHV001Paper"]            = home_FieldValue(v);                               }
            void setDUOHV001Paper               (double v)                                  { coilFields["dUOHV001Paper"]               = home_FieldValue(v);                               }
            void setDUOHVDuct                   (double v)                                  { coilFields["dUOHVDuct"]                   = home_FieldValue(v);                               }
            void setDUOHVCrepePaper             (double v)                                  { coilFields["dUOHVCrepePaper"]             = home_FieldValue(v);                               }
            void setDUOHV001Paper2              (double v)                                  { coilFields["dUOHV001Paper2"]              = home_FieldValue(v);                               }
            void setDCoilCoreDuct               (double v)                                  { coilFields["dCoilCoreDuct"]               = home_FieldValue(v);                               }
            void setDCoilCorePacking            (double v)                                  { coilFields["dCoilCorePacking"]            = home_FieldValue(v);                               }
            void setDCoilCoilDuct               (double v)                                  { coilFields["dCoilCoilDuct"]               = home_FieldValue(v);                               }
            void setDCoilCoilPacking            (double v)                                  { coilFields["dCoilCoilPacking"]            = home_FieldValue(v);                               }
            void setDCoilCoilDuct2              (double v)                                  { coilFields["dCoilCoilDuct2"]              = home_FieldValue(v);                               }
            void setPdPhaseAmp                  (const std::vector<double>& v)              { coilFields["pdPhaseAmp"]                  = home_FieldValue(home_FieldValue::List(v.begin(), v.end())); }
            void setPdPhaseAmpSinRel            (const std::vector<double>& v)              { coilFields["pdPhaseAmpSinRel"]            = home_FieldValue(home_FieldValue::List(v.begin(), v.end())); }
            void setILongPhaseAmp               (double v)                                  { coilFields["iLongPhaseAmp"]               = home_FieldValue(v);                               }
            void setPdPhaseAmpDifPos            (const std::vector<double>& v)              { coilFields["pdPhaseAmpDifPos"]            = home_FieldValue(home_FieldValue::List(v.begin(), v.end())); }
            void setDOvercoil                   (double v)                                  { coilFields["dOvercoil"]                   = home_FieldValue(v);                               }
            void setDTurnsSection               (double v)                                  { coilFields["dTurnsSection"]               = home_FieldValue(v);                               }
            void setITurnsLayers                (int v)                                     { coilFields["iTurnsLayers"]                = home_FieldValue(v);                               }
            void setDNoLayersUnder              (double v)                                  { coilFields["dNoLayersUnder"]              = home_FieldValue(v);                               }
            void setDNoLayersBody               (double v)                                  { coilFields["dNoLayersBody"]               = home_FieldValue(v);                               }
            void setDNoLayersOver               (double v)                                  { coilFields["dNoLayersOver"]               = home_FieldValue(v);                               }
            void setDTotLayers                  (double v)                                  { coilFields["dTotLayers"]                  = home_FieldValue(v);                               }
            void setIDucts                      (const std::vector<int>& v)                 { coilFields["i_Ducts"]                     = home_FieldValue(home_FieldValue::List(v.begin(), v.end())); }
            void setDuctsIn                     (const std::string& v)                      { coilFields["ductsIn"]                     = home_FieldValue(v);                               }
            void setDuctsOut                    (const std::string& v)                      { coilFields["ductsOut"]                    = home_FieldValue(v);                               }
            void setDWidthDucts                 (double v)                                  { coilFields["dWidthDucts"]                 = home_FieldValue(v);                               }
            void setSWidthDucts                 (const std::string& v)                      { coilFields["sWidthDucts"]                 = home_FieldValue(v);                               }
            void setDInterPaper                 (double v)                                  { coilFields["dInterPaper"]                 = home_FieldValue(v);                               }
            void setDEndPaper                   (double v)                                  { coilFields["dEndPaper"]                   = home_FieldValue(v);                               }
            void setITotNoEndLayers             (int v)                                     { coilFields["iTotNoEndLayers"]             = home_FieldValue(v);                               }
            void setSInsulationEnd              (const std::string& v)                      { coilFields["sInsulationEnd"]              = home_FieldValue(v);                               }
            void setSPreInsEnd                  (const std::string& v)                      { coilFields["sPreInsEnd"]                  = home_FieldValue(v);                               }
            void setSInsulationInt              (const std::string& v)                      { coilFields["sInsulationInt"]              = home_FieldValue(v);                               }
            void setSInsulationIntComplete      (const std::string& v)                      { coilFields["sInsulationIntComplete"]      = home_FieldValue(v);                               }
            void setIRequiredNumDucts           (double v)                                  { coilFields["iRequiredNumDucts"]           = home_FieldValue(v);                               }
            void setSOverOuterPreliminar        (const std::vector<std::string>& v)         { coilFields["s_OverOuterPreliminar"]       = home_FieldValue(home_FieldValue::List(v.begin(), v.end())); }
            void setSOver                       (const std::string& v)                      { coilFields["sOver"]                       = home_FieldValue(v);                               }
            void setDkVmm                       (double v)                                  { coilFields["dkVmm"]                       = home_FieldValue(v);                               }
            void setDRadialBuilt                (double v)                                  { coilFields["dRadialBuilt"]                = home_FieldValue(v);                               }
            void setSDescriptionTb              (const std::string& v)                      { coilFields["sDescriptionTb"]              = home_FieldValue(v);                               }
            void setDRadialBuiltSides           (double v)                                  { coilFields["dRadialBuiltSides"]           = home_FieldValue(v);                               }
            void setDRadialBuiltIntNoses        (double v)                                  { coilFields["dRadialBuiltIntNoses"]        = home_FieldValue(v);                               }
            void setDRadialBuiltOutNoses        (double v)                                  { coilFields["dRadialBuiltOutNoses"]        = home_FieldValue(v);                               }
            void setSRadialBuilds               (const std::string& v)                      { coilFields["sRadialBuilds"]               = home_FieldValue(v);                               }
            void setIHigh                       (int v)                                     { coilFields["iHigh"]                       = home_FieldValue(v);                               }
            void setIWide                       (int v)                                     { coilFields["iWide"]                       = home_FieldValue(v);                               }
            void setDCondThickTurn              (double v)                                  { coilFields["dCondThickTurn"]              = home_FieldValue(v);                               }
            void setDCondWdthTurn               (double v)                                  { coilFields["dCondWdthTurn"]               = home_FieldValue(v);                               }
            void setSErrorName                  (const std::vector<std::string>& v)         { coilFields["sErrorName"]                  = home_FieldValue(home_FieldValue::List(v.begin(), v.end())); }
            void setTErrorEnum                  (const std::vector<int>& v)                 { coilFields["tErrorEnum"]                  = home_FieldValue(home_FieldValue::List(v.begin(), v.end())); }
            void setSErrorMessages              (const std::vector<std::string>& v)         { coilFields["sErrorMessages"]              = home_FieldValue(home_FieldValue::List(v.begin(), v.end())); }
    };
// end of Coil class
}
// end of Transformer::Components::Coil