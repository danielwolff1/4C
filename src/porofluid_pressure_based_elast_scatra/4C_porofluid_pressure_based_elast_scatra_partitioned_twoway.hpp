// This file is part of 4C multiphysics licensed under the
// GNU Lesser General Public License v3.0 or later.
//
// See the LICENSE.md file in the top-level for license information.
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#ifndef FOUR_C_POROFLUID_PRESSURE_BASED_ELAST_SCATRA_PARTITIONED_TWOWAY_HPP
#define FOUR_C_POROFLUID_PRESSURE_BASED_ELAST_SCATRA_PARTITIONED_TWOWAY_HPP


#include "4C_config.hpp"

#include "4C_porofluid_pressure_based_elast_scatra_partitioned.hpp"

FOUR_C_NAMESPACE_OPEN

namespace PoroPressureBased
{
  //! Base class of all partitioned solid-scatra algorithms --> virtual
  class PoroMultiPhaseScaTraPartitionedTwoWay : public PoroMultiPhaseScaTraPartitioned
  {
   public:
    PoroMultiPhaseScaTraPartitionedTwoWay(
        MPI_Comm comm, const Teuchos::ParameterList& globaltimeparams);

    /// initialization
    void init(const Teuchos::ParameterList& globaltimeparams,
        const Teuchos::ParameterList& algoparams, const Teuchos::ParameterList& poroparams,
        const Teuchos::ParameterList& structparams, const Teuchos::ParameterList& fluidparams,
        const Teuchos::ParameterList& scatraparams, const std::string& struct_disname,
        const std::string& fluid_disname, const std::string& scatra_disname, bool isale,
        int nds_disp, int nds_vel, int nds_solidpressure, int ndsporofluid_scatra,
        const std::map<int, std::set<int>>* nearbyelepairs) override;

    /// setup
    void setup_system() override;

    /// setup solver (only needed for poromultiphase monolithic coupling)
    void setup_solver() override;

    /// time step of coupled problem
    void time_step() override { return solve(); };

    /// print header
    void print_header_partitioned();

    /// print header
    void iter_update_states();

    //! perform iteration loop between fields
    virtual void solve() = 0;


   protected:
    //! perform iteration step of structure-fluid field
    void do_poro_step();

    //! perform iteration step of scatra field
    void do_scatra_step();

    //! convergence check of outer loop
    bool convergence_check(int itnum);

    //! scalar increment of the outer loop
    std::shared_ptr<Core::LinAlg::Vector<double>> scaincnp_;
    //! structure increment of the outer loop
    std::shared_ptr<Core::LinAlg::Vector<double>> structincnp_;
    //! fluid increment of the outer loop
    std::shared_ptr<Core::LinAlg::Vector<double>> fluidincnp_;
    //! artery scatra increment of the outer loop
    std::shared_ptr<Core::LinAlg::Vector<double>> artscaincnp_;
    //! artery pressure increment of the outer loop
    std::shared_ptr<Core::LinAlg::Vector<double>> arterypressincnp_;

    //! maximum iteration steps
    int itmax_;
    //! convergence tolerance
    double ittol_;
    //! is artery coupling active
    bool artery_coupling_active_;


  };  // PoroMultiPhaseScatraPartitionedTwoWay

  //! Nested partitioned solution algorithm
  // +--------------------------+           +----------+
  // |         ---->            |  ------>  |          |
  // |  fluid        structure  |           | ScaTra   |
  // |         <----            |  <------  |          |
  // +--------------------------+           +----------+

  class PoroMultiPhaseScaTraPartitionedTwoWayNested : public PoroMultiPhaseScaTraPartitionedTwoWay
  {
   public:
    PoroMultiPhaseScaTraPartitionedTwoWayNested(
        MPI_Comm comm, const Teuchos::ParameterList& globaltimeparams);

    /// initialization
    void init(const Teuchos::ParameterList& globaltimeparams,
        const Teuchos::ParameterList& algoparams, const Teuchos::ParameterList& poroparams,
        const Teuchos::ParameterList& structparams, const Teuchos::ParameterList& fluidparams,
        const Teuchos::ParameterList& scatraparams, const std::string& struct_disname,
        const std::string& fluid_disname, const std::string& scatra_disname, bool isale,
        int nds_disp, int nds_vel, int nds_solidpressure, int ndsporofluid_scatra,
        const std::map<int, std::set<int>>* nearbyelepairs) override;

    //! perform iteration loop between fields
    void solve() override;

  };  // PoroMultiPhaseScatraPartitionedTwoWayNested

  //! Sequential partitioned solution algorithm
  // +-----------+          +-----------+           +-----------+
  // |           |  ----->  |           | --------> |           |
  // |   fluid   |          | structure |           |  ScaTra   |
  // |           |          |           |           |           |
  // +-----------+          +-----------+           +-----------+
  //      ^                                              |
  //      |----------------------------------------------+

  class PoroMultiPhaseScaTraPartitionedTwoWaySequential
      : public PoroMultiPhaseScaTraPartitionedTwoWay
  {
   public:
    PoroMultiPhaseScaTraPartitionedTwoWaySequential(
        MPI_Comm comm, const Teuchos::ParameterList& globaltimeparams);

    /// initialization
    void init(const Teuchos::ParameterList& globaltimeparams,
        const Teuchos::ParameterList& algoparams, const Teuchos::ParameterList& poroparams,
        const Teuchos::ParameterList& structparams, const Teuchos::ParameterList& fluidparams,
        const Teuchos::ParameterList& scatraparams, const std::string& struct_disname,
        const std::string& fluid_disname, const std::string& scatra_disname, bool isale,
        int nds_disp, int nds_vel, int nds_solidpressure, int ndsporofluid_scatra,
        const std::map<int, std::set<int>>* nearbyelepairs) override;

    //! perform iteration loop between fields
    void solve() override;

  };  // PoroMultiPhaseScatraPartitionedTwoWayNested


}  // namespace PoroPressureBased



FOUR_C_NAMESPACE_CLOSE

#endif
