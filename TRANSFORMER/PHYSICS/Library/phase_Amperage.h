/**___________________________________________________________________________________________________
|
|    @date      May 22, 2026
|    @author    Tom Goncin
|    @file      phase_Amperage.h
|    @brief     Transformer calculations, constants, & OS-specific utilities, paths, conversions.
|    @brief     Transformer::Physics    is for "Behavior & Calculations" 
|
|    NAME MAPPING DICTIONARY
|  -  -  -  -  -  OLD NAME  -  -  -  -  -  -  -    =     -  -  -  NEW NAME  -  -  -  -  -  -  -  -    
|            tipo de regulacion                    =    regulation_type
|            si no lleva regulacion                =    if no regulation is applied
|            para saber si es bobina regulacion    =    to identify if it is a regulation winding
|            tipo de conductor                     =    conductor_type
|            SOLERA                                =    FLAT_WIRE
|            lineal o reversing                    =    linear or reversing
|            calculate_amp_fase_h                  =    calculate_phase_amperage_h
|            iRedCap                               =    reduced_capacity
|            iNomPos                               =    pos_nom
|            iConexion                             =    connection
|            iPhases                               =    phases
|            iLongkV                               =    len_kv
|            iLongkVX                              =    len_kvX
|            dMVA                                  =    mva
|            dSerieRel                             =    series_ratio
|            d_kV                                  =    kv_values
|            d_kVX                                 =    kv_x_values
|            d_PhaseAmpX                           =    phase_amp_x_values
|            d_PhaseAmpSinRel                      =    phase_amp_without_ratio
|            d_PhaseAmpH                           =    phase_amp_h_values
|            d_LineAmpH                            =    line_amp_h_values
|            calculate_phase_amp_sin_rel_h         =    calculate_phase_amp_without_ratio_h
|            d_LineAmpSinRel                       =    line_amp_without_ratio
|            iLongPhaseAmp                         =    len_phase_amp
|___________________________________________________________________________________________________*/
#pragma once

#include <vector>
#include <cmath>
#include <tuple>
#include <stdexcept>
#include <algorithm>

#include "definitions.h"

namespace Transformer::Physics
{
    class PhaseAmperage 
    {
    private:
        int     reduced_capacity;
        int     pos_nom;
        int     connection;
        int     phases;
        int     len_kv;
        int     len_kvX;
        int     len_kvH;
        int     len_amp_phaseX;
        int     len_amp_phaseH;
        
        double  mva;
        double  series_ratio;
        
        int     auto_connection;
        int     lr_type;
        int     winding_dev;
        int     regulation;
        int     reg_connection_x;
        bool    meets_vfvv_range;

        std::vector<double> kv_values;
        std::vector<double> kv_x_values;
        std::vector<double> kv_h_values;
        std::vector<double> phase_amp_x_values;
        std::vector<double> phase_amp_h_values;
        std::vector<double> phase_amp_without_ratio;
        std::vector<double> line_amp_h_values;

    public:
        PhaseAmperage()  = default;
        ~PhaseAmperage() = default;

        // _________________________________________________________________________________________
        // |                                      GETTERS                                          |
        // |_______________________________________________________________________________________|
        int                     get_ReducedCapacity         () const { return reduced_capacity;         }
        int                     get_PosNom                  () const { return pos_nom;                  }
        int                     get_Connection              () const { return connection;               }
        int                     get_Phases                  () const { return phases;                   }
        int                     get_LenKv                   () const { return len_kv;                   }
        int                     get_LenKvX                  () const { return len_kvX;                  }
        int                     get_LenKvH                  () const { return len_kvH;                  }
        int                     get_LenAmpPhaseX            () const { return len_amp_phaseX;           }
        int                     get_LenAmpPhaseH            () const { return len_amp_phaseH;           }
        
        double                  get_Mva                     () const { return mva;                      }
        double                  get_SeriesRatio             () const { return series_ratio;             }
        
        int                     get_AutoConnection          () const { return auto_connection;          }
        int                     get_LrType                  () const { return lr_type;                  }
        int                     get_WindingDev              () const { return winding_dev;              }
        int                     get_Regulation              () const { return regulation;               }
        int                     get_RegConnectionX          () const { return reg_connection_x;         }
        bool                    get_MeetsVfvvRange          () const { return meets_vfvv_range;         }

        std::vector<double>     get_KvValues                () const { return kv_values;                }
        std::vector<double>     get_KvXValues               () const { return kv_x_values;              }
        std::vector<double>     get_KvHValues               () const { return kv_h_values;              }
        std::vector<double>     get_PhaseAmpXValues         () const { return phase_amp_x_values;       }
        std::vector<double>     get_PhaseAmpHValues         () const { return phase_amp_h_values;       }
        std::vector<double>     get_PhaseAmpWithoutRatio    () const { return phase_amp_without_ratio;  }
        std::vector<double>     get_LineAmpHValues          () const { return line_amp_h_values;        }

        // _________________________________________________________________________________________
        // |                                      SETTERS                                          |
        // |_______________________________________________________________________________________|
        void    set_ReducedCapacity         (int                 val) { reduced_capacity        = val; }
        void    set_PosNom                  (int                 val) { pos_nom                 = val; }
        void    set_Connection              (int                 val) { connection              = val; }
        void    set_Phases                  (int                 val) { phases                  = val; }
        void    set_LenKv                   (int                 val) { len_kv                  = val; }
        void    set_LenKvX                  (int                 val) { len_kvX                 = val; }
        void    set_LenKvH                  (int                 val) { len_kvH                 = val; }
        void    set_LenAmpPhaseX            (int                 val) { len_amp_phaseX          = val; }
        void    set_LenAmpPhaseH            (int                 val) { len_amp_phaseH          = val; }
        
        void    set_Mva                     (double              val) { mva                     = val; }
        void    set_SeriesRatio             (double              val) { series_ratio            = val; }
        
        void    set_AutoConnection          (int                 val) { auto_connection         = val; }
        void    set_LrType                  (int                 val) { lr_type                 = val; }
        void    set_WindingDev              (int                 val) { winding_dev             = val; }
        void    set_Regulation              (int                 val) { regulation              = val; }
        void    set_RegConnectionX          (int                 val) { reg_connection_x        = val; }
        void    set_MeetsVfvvRange          (bool                val) { meets_vfvv_range        = val; }

        void    set_KvValues                (std::vector<double> val) { kv_values               = val; }
        void    set_KvXValues               (std::vector<double> val) { kv_x_values             = val; }
        void    set_KvHValues               (std::vector<double> val) { kv_h_values             = val; }
        void    set_PhaseAmpXValues         (std::vector<double> val) { phase_amp_x_values      = val; }
        void    set_PhaseAmpHValues         (std::vector<double> val) { phase_amp_h_values      = val; }
        void    set_PhaseAmpWithoutRatio    (std::vector<double> val) { phase_amp_without_ratio = val; }
        void    set_LineAmpHValues          (std::vector<double> val) { line_amp_h_values       = val; }

        // _________________________________________________________________________________________
        // |                                  CORE CALCULATIONS                                    |
        // |_______________________________________________________________________________________|
        std::tuple<std::vector<double>, std::vector<double>> calculate_phase_amperageX();

        int calculate_phase_amperage_h();

        std::tuple<int, std::vector<double>, std::vector<double>> calculate_phase_amp_without_ratio_h();

        std::tuple<int, std::vector<double>, std::vector<double>, std::vector<double>> calculate_phase_amperage();
    };
}