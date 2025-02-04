/* -*- c++ -*- */
/*
 * Copyright 2022 JM Friedt
 *
 * This is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3, or (at your option)
 * any later version.
 *
 * This software is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this software; see the file COPYING.  If not, write to
 * the Free Software Foundation, Inc., 51 Franklin Street,
 * Boston, MA 02110-1301, USA.
 */

#ifndef INCLUDED_ACARS_ACARS_IMPL_H
#define INCLUDED_ACARS_ACARS_IMPL_H

#include <acars/acars.h>      // Base class (acars)
#include <cstdio>            // for FILE*, std::printf, etc.
#include <vector>            // for std::vector
#include <string>

#include <gnuradio/fft/fft.h>         // if you need fft classes
#include <gnuradio/fft/fft_shift.h>   // remove if not used

namespace gr {
namespace acars {

class acars_impl : public acars
{
private:
    int   _Ntot;      ///< total number of samples accumulated
    int   _N;         ///< number of items in the current work call
    float _threshold; ///< running threshold
    int   _savenum;   ///< flag to save raw data
    int   _decompte;  ///< accumulate extra chunks if needed
    float _seuil;     ///< user threshold multiplier
    FILE* _FILE;      ///< output file pointer

    // Using std::vector rather than raw pointers
    std::vector<float> _d;       ///< buffer for raw data
    std::vector<char>  _toutd;   ///< buffer for demod bits
    std::vector<char>  _tout;    ///< buffer for final bits
    std::vector<char>  _message; ///< buffer for message bytes
    std::vector<char>  _somme;   ///< buffer for parity or other checks

    void  acars_parse(char* message, int ends);
    float remove_avgf(const float* d, float* out, int tot_len);
    void  acars_dec(float* d, int N);

public:
    acars_impl(float seuil, std::string filename, bool saveall);
    ~acars_impl();

    void set_seuil(float seuil1);

    // Core processing method
    int work(int noutput_items,
             gr_vector_const_void_star& input_items,
             gr_vector_void_star& output_items) override;
};

} // namespace acars
} // namespace gr

#endif /* INCLUDED_ACARS_ACARS_IMPL_H */
