/**___________________________________________________________________________________________________
|    @date      May 26, 2026 ~ EDIT: August 17 2026
|    @author    Tom Goncin
|    @file      Conductor.h
|    @brief     Optimization analyzing electrical conductor sizing.
|    @brief     Transformer::Components     is "Physical Hardware"
|
|               Executes:
|                   - Initialization of conductor properties mapping to specialized FieldValues.[cite: 30]
|                   - Resolves automated layer configuration matrices, strip parameters, and magnet widths.[cite: 30]
|                   - Computes volumetric mass weight, current densities, and geometric layout footprints.[cite: 30]
|                   - Optimizes solution arrays exploring physical boundaries to minimize manufacturing cost.[cite: 30]
|               Returns:
|                   - High-performance structures tracking electrical transformer conductor physics.[cite: 30]
|___________________________________________________________________________________________________*/
#pragma once

#include <string>
#include <vector>
#include <map>

#include "alias.h"
using namespace Transformer::Types;

#include "materials.h"
#include "enums.h"
#include "field_Value.h"
#include "sap_Tables.h"

#include "conductor_Warnings.h"



namespace Transformer::Components
{
    struct ConductorSolution
    {
        int numberOfConductors;
        int radialDimensions;
        int axialDimensions;
    };

    class Winding;
    class Coil;

    class Conductor
    {
        private:
            static int id_Counter;

        public:
            std::map <std::string, home_FieldValue> conductorFields;

            home_Device*    device;
            home_Winding*   winding;
            home_Coil*      coil;

            home_ConductorTable    table_ConductorTable;
            home_CoilBil3Ph        table_CoilBil3Ph;
            home_MaterialConstants table_MaterialConstants;

            std::map <std::string, double>  initialFieldValues;
            home_ConductorWarnings          conductorWarnings;

            explicit Conductor      (   home_Coil* coil = nullptr, 
                                        home_MaterialConstants  materialConstants      = {}, 
                                        home_CoilBil3Ph         table_CoilBil3Ph       = {}, 
                                        home_ConductorTable     table_ConductorTables  = {} 
                                    );

            home_Device*    getParentDevice() const;
            home_Winding*   getParentWinding() const;
            home_Coil*      getParentCoil() const;

            std::vector<int> get_NominalConductor (int windingType);
            std::vector<home_ConductorSolution> find_PossibleConductors( home_Winding *winding);
            std::pair<std::vector<int>, std::vector<std::string>> get_NominalConductorOpt(int windingType);


            void reset_Fields();
            void copy                   (const Conductor& conductorObject_CopiedFrom);
            void erase_Warnings();

            int look_ConductorDevice    (int numberPhases, home_Winding* winding, int windingType);
            int look_NoConductor        (int conductorType, int numberPhases, const std::string& conductor, int windingType);

            void assign_Winding         (home_Winding* winding);
            void assign_Defaults        (home_Device* device, home_Winding* winding, home_Coil* coil, int windingType);

            int calc_DefaultType        (int windingType);
            int calc_UncorrLyrs         (home_Coil* coil);
            int calc_DefaultCorrLyrs    (home_Coil* coil);
            int calc_DefaultMaterial();
            int calc_DefaultNoConductor (int windingType);

            int get_MaxDensityConductorOpt();

            double calc_UncorrSB             (home_Winding* winding, home_Coil* coil,      int numberPhases);
            double calc_UncorrCondSpc        (home_Coil* coil,       int windingType,       int numberPhases);
            double calc_MaxTurnsLyr          (home_Winding* winding, int windingType,       int numberPhases);
            double calc_CorrectTurnsLyrs     (home_Winding* winding, int windingType,       home_Coil* coil,   int numberPhases);
            double calc_DefaultConductorSpace(home_Coil* coil,       int windingType,       int numberPhases);
            double calc_DefaultSBMin         (home_Device* device,   home_Winding* winding, home_Coil* coil);
            double calc_DefaultStayback      (home_Winding* winding, home_Coil* coil,       int windingType,   int numberPhases);

            void calc_Conductor       (home_Device* device, home_Winding* winding, home_Coil* coil, int windingType);
            void calc_Tables          (home_Winding* winding, int windingType);
            void calc_MaterialAl      (int windingType);
            void calc_WidthStrand     (int windingType);
            void calc_ThickStrand     (int windingType);
            void calc_TotalWidthTurn  (home_Coil* coil);
            void calc_TotalThickTurn  (home_Coil* coil);
            void calc_MagnetArea      (int windingType);
            void calc_Description();
            void calc_SWConductor     (int windingType);
            void calc_MatConductor();
            void calc_PaperConductor();
            void calc_Weight          (home_Device* device, home_Coil* coils, int windingType);
            void calc_Density         (home_Winding* Winding, int windingType);
            void calc_Material();
            void calc_Warnings();
            void calc_densityOpt     (home_Winding* winding);
            void calc_Area           (home_Coil *coil, int windingType);

        // _________________________________________________________________________________________
        // |                                      GETTERS                                          |
        // |_______________________________________________________________________________________|

            int     getId              () const { return conductorFields.at("id")           .get<int>(); }
            int     getIType           () const { return conductorFields.at("iType")        .get<int>(); }
            int     getINoConductor    () const { return conductorFields.at("iNoConductor") .get<int>(); }
            int     getINoAxial        () const { return conductorFields.at("iNoAxial")     .get<int>(); }
            int     getINoRadial       () const { return conductorFields.at("iNoRadial")    .get<int>(); }
            int     getIRadialLevel    () const { return conductorFields.at("iRadialLevel") .get<int>(); }
            int     getTMatAluminum    () const { return conductorFields.at("tMatAluminum") .get<int>(); }
            
            double  getDWidth          () const { return conductorFields.at("dWidth")           .get<double>(); }
            double  getDWeight         () const { return conductorFields.at("dWeight")          .get<double>(); }
            double  getDWidthStrand    () const { return conductorFields.at("dWidthStrand")     .get<double>(); }
            double  getDThickStrand    () const { return conductorFields.at("dThickStrand")     .get<double>(); }
            double  getAreaStrand      () const { return conductorFields.at("areaStrand")       .get<double>(); }
            double  getDTotalWidthTurn () const { return conductorFields.at("dTotalWidthTurn")  .get<double>(); }
            double  getDTotalThickTurn () const { return conductorFields.at("dTotalThickTurn")  .get<double>(); }
            double  getDMagnetArea     () const { return conductorFields.at("dMagnetArea")      .get<double>(); }
            double  getDAreaCond       () const { return conductorFields.at("dAreaCond")        .get<double>(); }
            double  getDDensity        () const { return conductorFields.at("dDensity")         .get<double>(); }
            double  getDDensityONAF    () const { return conductorFields.at("dDensityONAF")     .get<double>(); }
            double  getDDensityFCBN    () const { return conductorFields.at("dDensityFCBN")     .get<double>(); }
            double  getDDensityFCBNONAF() const { return conductorFields.at("dDensityFCBNONAF") .get<double>(); }
            double  getDConductorSpace () const { return conductorFields.at("dConductorSpace")  .get<double>(); }
            double  getDStayback       () const { return conductorFields.at("dStayback")        .get<double>(); }
            double  getDT              () const { return conductorFields.at("dT")               .get<double>(); }
            double  getDT2             () const { return conductorFields.at("dT2")              .get<double>(); }
            double  getDSBMin          () const { return conductorFields.at("dSBMin")           .get<double>(); }
            double  getDUncorrSB       () const { return conductorFields.at("dUncorrSB")        .get<double>(); }
            double  getDUncorrCondSpc  () const { return conductorFields.at("dUncorrCondSpc")   .get<double>(); }
            double  getDMaxTurnsLyr    () const { return conductorFields.at("dMaxTurnsLyr")     .get<double>(); }
            double  getDUncorrLyrs     () const { return conductorFields.at("dUncorrLyrs")      .get<double>(); }
            double  getDCorrTurnsLyrs  () const { return conductorFields.at("dCorrTurnsLyrs")   .get<double>(); }
            double  getDCorrLyrs       () const { return conductorFields.at("dCorrLyrs")        .get<double>(); }
            
            std::string     getName            () const { return conductorFields.at("name")             .get<std::string>(); }
            std::string     getSWeight         () const { return conductorFields.at("sWeight")          .get<std::string>(); }
            std::string     getSDensity        () const { return conductorFields.at("sDensity")         .get<std::string>(); }
            std::string     getSDescription    () const { return conductorFields.at("sDescription")     .get<std::string>(); }
            std::string     getSSWConductor    () const { return conductorFields.at("sSWConductor")     .get<std::string>(); }
            std::string     getSMatConductor   () const { return conductorFields.at("sMatConductor")    .get<std::string>(); }
            std::string     getSPapperConductor() const { return conductorFields.at("sPapperConductor") .get<std::string>(); }

        // _________________________________________________________________________________________
        // |                                      SETTERS                                          |
        // |_______________________________________________________________________________________|

            void setId                      (int value) { conductorFields["id"]             = home_FieldValue(value); }
            void setIType                   (int value) { conductorFields["iType"]          = home_FieldValue(value); }
            void setINoConductor            (int value) { conductorFields["iNoConductor"]   = home_FieldValue(value); }
            void setINoAxial                (int value) { conductorFields["iNoAxial"]       = home_FieldValue(value); }
            void setINoRadial               (int value) { conductorFields["iNoRadial"]      = home_FieldValue(value); }
            void setIRadialLevel            (int value) { conductorFields["iRadialLevel"]   = home_FieldValue(value); }
            void setTMatAluminum            (int value) { conductorFields["tMatAluminum"]   = home_FieldValue(value); }

            void setDWidth                  (double value)  { conductorFields["dWidth"]             = home_FieldValue(value); }
            void setDWeight                 (double value)  { conductorFields["dWeight"]            = home_FieldValue(value); }
            void setDWidthStrand            (double value)  { conductorFields["dWidthStrand"]       = home_FieldValue(value); }
            void setDThickStrand            (double value)  { conductorFields["dThickStrand"]       = home_FieldValue(value); }
            void setAreaStrand              (double value)  { conductorFields["areaStrand"]         = home_FieldValue(value); }
            void setDTotalWidthTurn         (double value)  { conductorFields["dTotalWidthTurn"]    = home_FieldValue(value); }
            void setDTotalThickTurn         (double value)  { conductorFields["dTotalThickTurn"]    = home_FieldValue(value); }
            void setDMagnetArea             (double value)  { conductorFields["dMagnetArea"]        = home_FieldValue(value); }
            void setDAreaCond               (double value)  { conductorFields["dAreaCond"]          = home_FieldValue(value); }
            void setDDensity                (double value)  { conductorFields["dDensity"]           = home_FieldValue(value); }
            void setDDensityONAF            (double value)  { conductorFields["dDensityONAF"]       = home_FieldValue(value); }
            void setDDensityFCBN            (double value)  { conductorFields["dDensityFCBN"]       = home_FieldValue(value); }
            void setDDensityFCBNONAF        (double value)  { conductorFields["dDensityFCBNONAF"]   = home_FieldValue(value); }
            void setDConductorSpace         (double value)  { conductorFields["dConductorSpace"]    = home_FieldValue(value); }
            void setDStayback               (double value)  { conductorFields["dStayback"]          = home_FieldValue(value); }
            void setDT                      (double value)  { conductorFields["dT"]                 = home_FieldValue(value); }
            void setDT2                     (double value)  { conductorFields["dT2"]                = home_FieldValue(value); }
            void setDSBMin                  (double value)  { conductorFields["dSBMin"]             = home_FieldValue(value); }
            void setDUncorrSB               (double value)  { conductorFields["dUncorrSB"]          = home_FieldValue(value); }
            void setDUncorrCondSpc          (double value)  { conductorFields["dUncorrCondSpc"]     = home_FieldValue(value); }
            void setDMaxTurnsLyr            (double value)  { conductorFields["dMaxTurnsLyr"]       = home_FieldValue(value); }
            void setDUncorrLyrs             (double value)  { conductorFields["dUncorrLyrs"]        = home_FieldValue(value); }
            void setDCorrTurnsLyrs          (double value)  { conductorFields["dCorrTurnsLyrs"]     = home_FieldValue(value); }
            void setDCorrLyrs               (double value)  { conductorFields["dCorrLyrs"]          = home_FieldValue(value); }

            void setName                    (const std::string& value)  { conductorFields["name"]               = home_FieldValue(value); }
            void setSWeight                 (const std::string& value)  { conductorFields["sWeight"]            = home_FieldValue(value); }
            void setSDensity                (const std::string& value)  { conductorFields["sDensity"]           = home_FieldValue(value); }
            void setSDescription            (const std::string& value)  { conductorFields["sDescription"]       = home_FieldValue(value); }
            void setSSWConductor            (const std::string& value)  { conductorFields["sSWConductor"]       = home_FieldValue(value); }
            void setSMatConductor           (const std::string& value)  { conductorFields["sMatConductor"]      = home_FieldValue(value); }
            void setSPapperConductor        (const std::string& value)  { conductorFields["sPapperConductor"]   = home_FieldValue(value); }
    };
// end of conductor class
}
//end of Transformer::Components::Conductor