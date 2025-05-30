// This file is part of 4C multiphysics licensed under the
// GNU Lesser General Public License v3.0 or later.
//
// See the LICENSE.md file in the top-level for license information.
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#ifndef FOUR_C_FLUID_TIMINT_HDG_WEAK_COMP_HPP
#define FOUR_C_FLUID_TIMINT_HDG_WEAK_COMP_HPP


#include "4C_config.hpp"

#include "4C_fluid_timint_genalpha.hpp"

FOUR_C_NAMESPACE_OPEN


namespace FLD
{
  /*!
  \brief time integration for HDG fluid (only gen-alpha implemented)

  */
  class TimIntHDGWeakComp : public TimIntGenAlpha
  {
   public:
    /// Standard Constructor
    TimIntHDGWeakComp(const std::shared_ptr<Core::FE::Discretization>& actdis,
        const std::shared_ptr<Core::LinAlg::Solver>& solver,
        const std::shared_ptr<Teuchos::ParameterList>& params,
        const std::shared_ptr<Core::IO::DiscretizationWriter>& output, bool alefluid);

    /*!
    \brief initialization

    */
    void init() override;

    /*!
    \brief Set theta_ to its value, dependent on integration method for GenAlpha and BDF2

    */
    void set_theta() override;

    /*!
    \brief do explicit predictor step to start nonlinear iteration from
           a better initial value
    */
    void explicit_predictor() override;

    /*!
    \brief Set custom parameters in the respective time integration class (Loma, RedModels...)

    */
    void set_custom_ele_params_assemble_mat_and_rhs(Teuchos::ParameterList& eleparams) override;

    /*!
    \brief Set states in the time integration schemes: additional vectors for HDG

    */
    void set_state_tim_int() override;

    /*!
    \brief Call discret_->ClearState() after assembly (HDG needs to read from state vectors...)

    */
    void clear_state_assemble_mat_and_rhs() override;

    /*!
    \brief Set the part of the right hand side belonging to the last
           time step for incompressible or low-Mach-number flow

       for low-Mach-number flow: distinguish momentum and continuity part
       (continuity part only meaningful for low-Mach-number flow)

       Stationary/af-generalized-alpha:

                     mom: hist_ = 0.0
                    (con: hist_ = 0.0)

       One-step-Theta:

                     mom: hist_ = veln_  + dt*(1-Theta)*accn_
                    (con: hist_ = densn_ + dt*(1-Theta)*densdtn_)

       BDF2: for constant time step:

                     mom: hist_ = 4/3 veln_  - 1/3 velnm_
                    (con: hist_ = 4/3 densn_ - 1/3 densnm_)


    */
    void set_old_part_of_righthandside() override;

    /*!
    \brief update within iteration

    */
    void iter_update(const std::shared_ptr<const Core::LinAlg::Vector<double>> increment) override;

    /*!
    \brief Update the solution after convergence of the nonlinear
           iteration. Current solution becomes old solution of next
           time step.
    */
    void time_update() override;

    /*!
    \brief Update the grid velocity
    */
    void update_gridv() override;

    /*!
    \brief set initial flow field for analytical test problems

    */
    void set_initial_flow_field(
        const Inpar::FLUID::InitialField initfield, const int startfuncno) override;

    /*!
    \brief calculate error between a analytical solution and the
           numerical solution of a test problems

    */
    std::shared_ptr<std::vector<double>> evaluate_error_compared_to_analytical_sol() override;

    /*!
    \brief Reset state vectors
     */
    void reset(bool completeReset = false, int numsteps = 1, int iter = -1) override;

    /*!
    \brief update configuration and output to file/screen

    */
    void output() override;

    /*!
    \brief accessor to interior velocity

    */
    virtual std::shared_ptr<Core::LinAlg::Vector<double>> return_int_velnp() { return intvelnp_; }
    virtual std::shared_ptr<Core::LinAlg::Vector<double>> return_int_veln() { return intveln_; }
    virtual std::shared_ptr<Core::LinAlg::Vector<double>> return_int_velnm() { return intvelnm_; }


   protected:
    /// copy constructor
    TimIntHDGWeakComp(const TimIntHDGWeakComp& old);

    /*!
    \brief update acceleration for generalized-alpha time integration

    */
    void gen_alpha_update_acceleration() override;

    /*!
    \brief compute values at intermediate time steps for gen.-alpha

    */
    void gen_alpha_intermediate_values() override;

    //! @name mixed variable, density and momentum at time n+1, n, n-1
    //!  and n+alpha_F for element interior in HDG
    std::shared_ptr<Core::LinAlg::Vector<double>> intvelnp_;
    std::shared_ptr<Core::LinAlg::Vector<double>> intveln_;
    std::shared_ptr<Core::LinAlg::Vector<double>> intvelnm_;
    std::shared_ptr<Core::LinAlg::Vector<double>> intvelaf_;
    //@}

    //! @name time derivatives at time n+1, n and n+alpha_M/(n+alpha_M/n)
    //!  and n-1 for element interior in HDG
    //@{
    std::shared_ptr<Core::LinAlg::Vector<double>>
        intaccnp_;                                           ///< acceleration at time \f$t^{n+1}\f$
    std::shared_ptr<Core::LinAlg::Vector<double>> intaccn_;  ///< acceleration at time \f$t^{n}\f$
    std::shared_ptr<Core::LinAlg::Vector<double>>
        intaccnm_;  ///< acceleration at time \f$t^{n-1}\f$
    std::shared_ptr<Core::LinAlg::Vector<double>>
        intaccam_;  ///< acceleration at time \f$t^{n+\alpha_M}\f$
    //@}

    //! @name other HDG-specific auxiliary vectors for output
    std::shared_ptr<Core::LinAlg::MultiVector<double>> interpolatedMixedVar_;
    std::shared_ptr<Core::LinAlg::Vector<double>> interpolatedDensity_;
    //@}


   private:
    ///< Print stabilization details to screen. Do nothing here because we do not use stabilization
    void print_stabilization_details() const override {}

    ///< time algorithm flag actually set (we internally reset it)
    Inpar::FLUID::TimeIntegrationScheme timealgoset_;

    ///< Keep track of whether we do the first assembly of a time step because we reconstruct the
    ///< local HDG solution as part of assembly
    bool first_assembly_;

  };  // class TimIntHDGWeakComp

}  // namespace FLD


FOUR_C_NAMESPACE_CLOSE

#endif
