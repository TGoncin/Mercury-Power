/**___________________________________________________________________________________________________
|
|    @date      May 22, 2026 ~ EDIT: August 17 2026
|    @author    Tom Goncin
|    @file      phase_Amperage.cpp
|    @brief     Transformer calculations, constants, & OS-specific utilities, paths, conversions.
|    @brief     Transformer::Physics    is for "Behavior & Calculations" 
|___________________________________________________________________________________________________*/

#include "alias.h"
using namespace Transformer::Types;
#include "enums.h"
#include "transformer_Config.h"

#include "phase_Amperage.h"




namespace Transformer::Physics
{

    // Inlined safe_div logic matching standard domain functions signature
    static inline double safe_div(double numerator, double denominator)
    {
        if (denominator == 0.0)            { return 0.0; } 
        return numerator / denominator;
    }

    std::tuple<std::vector<double>, std::vector<double>> PhaseAmperage::calculate_phase_amperageX()
    {
        std::vector <double> amp_phase_list;
        std::vector <double> amps_line_list;

        amp_phase_list.reserve(len_kv);
        amps_line_list.reserve(len_kv);

        bool isWyeOrAuto            = ( connection == static_cast <int>( Config::ConnectionType::Wye) || connection == static_cast <int>( Config::Type::AutoTransformer));
        double connection_factor    = isWyeOrAuto ? 1.0 : std::sqrt(3.0);

        if (len_kv > 1)
        {
            for (int i = 0; i < len_kv; ++i)
            {
                double kv           = kv_values[ i];
                double amp_phase_p  = safe_div( mva, static_cast <double>( phases));
                amp_phase_p         = safe_div( amp_phase_p, kv) * 1000.0;
                double amp_line_p   = amp_phase_p * connection_factor;

                amp_phase_list.push_back(amp_phase_p);
                amps_line_list.push_back(amp_line_p);
            }
        }
        else
        {
            double kv           = kv_values[ 0];
            double amp_phase_p  = safe_div( mva, static_cast <double>( phases));
            amp_phase_p         = safe_div( amp_phase_p, kv) * 1000.0;
            double amp_line_p   = amp_phase_p * connection_factor;

            amp_phase_list.push_back(amp_phase_p);
            amps_line_list.push_back(amp_line_p);
        }
        return std::make_tuple(amp_phase_list, amps_line_list);
    }

    int PhaseAmperage::calculate_phase_amperage_h()
    {
    // Clear out output targets passed via parameters
        phase_amp_without_ratio.clear();
        phase_amp_h_values.clear();
        line_amp_h_values.clear();

        std::vector <double> line_amp_without_ratio_local;

        auto [len_phase_amp, phase_sin, line_sin] = calculate_phase_amp_without_ratio_h();
        
        phase_amp_without_ratio = phase_sin;
        line_amp_without_ratio_local = line_sin;

        if (len_phase_amp > 1)
        {
            for (int i = 0; i < len_phase_amp; ++i) 
            {
                phase_amp_h_values.push_back    ( safe_div (phase_amp_without_ratio [ i],   series_ratio));
                line_amp_h_values.push_back     ( safe_div ( line_amp_without_ratio_local [ i],   series_ratio));
            }
        }
        else
        {
            phase_amp_h_values.push_back    ( safe_div( phase_amp_without_ratio[ 0],    series_ratio));
            line_amp_h_values.push_back     ( safe_div( line_amp_without_ratio_local [ 0],     series_ratio));
        }
        return len_phase_amp;
    }

    std::tuple <int, std::vector <double>, std::vector <double>> PhaseAmperage::calculate_phase_amp_without_ratio_h()
    {
        auto [phase_amp_without_ratio_local, line_amp_without_ratio_local] = calculate_phase_amperageX();

        int len_phase_amp = len_kv;

        return std::make_tuple (len_phase_amp, phase_amp_without_ratio_local, line_amp_without_ratio_local);
    }

    std::tuple <int, std::vector <double>, std::vector <double>, std::vector <double>> PhaseAmperage::calculate_phase_amperage()
    {
        int len_amp_phase = len_kv;
        
        auto [amp_phase_sin_rel, line_amp_list] = calculate_phase_amperageX();

        if (regulation == Config::REG)
        {
            int pos_piv = (lr_type == Config::REV) ? pos_nom : (len_kv - 1);

            for (int i = 0; i < len_kv; ++i)
            {
                if (i == pos_piv)            {  amp_phase_sin_rel[i] = 0.0; }
            }
        }
        size_t maxSize = std::max 
        (
            {
                static_cast <size_t> (len_amp_phaseX),
                static_cast <size_t> (len_amp_phaseH),
                static_cast <size_t> (len_kv)
            }
        );

        std::vector <double> amp_phase(maxSize, 0.0);
        std::vector <double> amp_phase_diff_pos(9, 0.0);

        for (int i = 0; i < len_amp_phase; ++i)         { amp_phase[i] = amp_phase_sin_rel[i] / series_ratio; }

        return std::make_tuple ( len_amp_phase,  amp_phase_sin_rel,  amp_phase,  amp_phase_diff_pos );
    }

}