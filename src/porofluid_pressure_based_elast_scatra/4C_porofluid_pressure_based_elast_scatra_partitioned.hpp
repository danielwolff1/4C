// This file is part of 4C multiphysics licensed under the
// GNU Lesser General Public License v3.0 or later.
//
// See the LICENSE.md file in the top-level for license information.
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#ifndef FOUR_C_POROFLUID_PRESSURE_BASED_ELAST_SCATRA_PARTITIONED_HPP
#define FOUR_C_POROFLUID_PRESSURE_BASED_ELAST_SCATRA_PARTITIONED_HPP

#include "4C_config.hpp"

#include "4C_porofluid_pressure_based_elast_scatra_base.hpp"

FOUR_C_NAMESPACE_OPEN

namespace PoroPressureBased
{
  //! Base class of all solid-scatra algorithms
  class PoroMultiPhaseScaTraPartitioned : public PoroMultiPhaseScaTraBase
  {
   public:
    PoroMultiPhaseScaTraPartitioned(MPI_Comm comm, const Teuchos::ParameterList& globaltimeparams)
        : PoroMultiPhaseScaTraBase(comm, globaltimeparams) {};  // Problem builder


  };  // PoroMultiPhasePartitioned


}  // namespace PoroPressureBased



FOUR_C_NAMESPACE_CLOSE

#endif
