// This file is part of 4C multiphysics licensed under the
// GNU Lesser General Public License v3.0 or later.
//
// See the LICENSE.md file in the top-level for license information.
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#ifndef FOUR_C_FLUID_TIMINT_STAT_HDG_HPP
#define FOUR_C_FLUID_TIMINT_STAT_HDG_HPP
// TODO also fix
// fluid_timint_stat_hdg
// because it is not working


#include "4C_config.hpp"

#include "4C_fluid_timint_stat.hpp"

FOUR_C_NAMESPACE_OPEN


namespace FLD
{
  class TimIntStationaryHDG : public virtual TimIntStationary
  {
   public:
    /// Standard Constructor
    TimIntStationaryHDG(const std::shared_ptr<Core::FE::Discretization>& actdis,
        const std::shared_ptr<Core::LinAlg::Solver>& solver,
        const std::shared_ptr<Teuchos::ParameterList>& params,
        const std::shared_ptr<Core::IO::DiscretizationWriter>& output, bool alefluid = false);

    /*!
    \brief initialization

    */
    void init() override;

    /*!
    \brief Set the part of the righthandside belonging to the last
           timestep

       Stationary:

                     mom: hist_ = 0.0
                    (con: hist_ = 0.0)


    */
    /*!
      \brief Reset state vectors
       */
    void reset(bool completeReset = false, int numsteps = 1, int iter = -1) override;

    void set_old_part_of_righthandside() override;

    /*!
      \brief Set custom parameters in the respective time integration class (Loma, RedModels...)

    */
    void set_custom_ele_params_assemble_mat_and_rhs(Teuchos::ParameterList& eleparams) override;

    /*!
    \brief Set states in the time integration schemes: differs between GenAlpha and the others

    */
    void set_state_tim_int() override;

    /*!
    \brief Call discret_->ClearState() after assembly (HDG needs to read from state vectors...)

    */
    void clear_state_assemble_mat_and_rhs() override;

    /*!
    \brief set initial flow field for analytical test problems

    */
    void set_initial_flow_field(
        const Inpar::FLUID::InitialField initfield, const int startfuncno) override;

    /*!

    \brief parameter (fix over a time step) are set in this method.
           Therefore, these parameter are accessible in the fluid element
           and in the fluid boundary element

    */
    void set_element_time_parameter() override;

    //@}

   protected:
    //! @name velocity gradient, velocity and pressure at time n+1, n, n-1
    //!  and n+alpha_F for element interior in HDG
    std::shared_ptr<Core::LinAlg::Vector<double>> intvelnp_;

   private:
    ///< Keep track of whether we do the first assembly because we reconstruct the local HDG
    ///< solution as part of assembly
    bool first_assembly_;


  };  // class TimIntStationaryHDG

}  // namespace FLD

FOUR_C_NAMESPACE_CLOSE

#endif
