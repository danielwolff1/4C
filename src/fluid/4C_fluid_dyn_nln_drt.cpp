// This file is part of 4C multiphysics licensed under the
// GNU Lesser General Public License v3.0 or later.
//
// See the LICENSE.md file in the top-level for license information.
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#include "4C_fluid_dyn_nln_drt.hpp"

#include "4C_adapter_fld_base_algorithm.hpp"
#include "4C_fem_condition_utils.hpp"
#include "4C_fem_discretization.hpp"
#include "4C_fluid_turbulence_turbulent_flow_algorithm.hpp"
#include "4C_global_data.hpp"
#include "4C_linalg_utils_sparse_algebra_math.hpp"

FOUR_C_NAMESPACE_OPEN

/*----------------------------------------------------------------------*
 * Main control routine for fluid including various solvers:
 *
 *        o instationary one-step-theta
 *        o instationary BDF2
 *        o instationary generalized-alpha (two versions)
 *        o stationary
 *
 *----------------------------------------------------------------------*/
void dyn_fluid_drt(const int restart)
{
  // create a communicator
  MPI_Comm comm = Global::Problem::instance()->get_dis("fluid")->get_comm();

  // access to some parameter lists
  // const Teuchos::ParameterList& probtype = Global::Problem::instance()->ProblemTypeParams();
  const Teuchos::ParameterList& fdyn = Global::Problem::instance()->fluid_dynamic_params();

  // prepares a turbulent flow simulation with generation of turbulent inflow during the
  // actual simulation
  // this is done in two steps
  // 1. computation of inflow until it reaches a fully turbulent state
  // 2. computation of the main problem after restart
  // Remark: we restart the simulation to save procs!
  if ((fdyn.sublist("TURBULENT INFLOW").get<bool>("TURBULENTINFLOW")) and
      (restart < fdyn.sublist("TURBULENT INFLOW").get<int>("NUMINFLOWSTEP")))
  {
    if (Core::Communication::my_mpi_rank(comm) == 0)
    {
      std::cout << "#-----------------------------------------------#" << std::endl;
      std::cout << "#      ENTER TURBULENT INFLOW COMPUTATION       #" << std::endl;
      std::cout << "#-----------------------------------------------#" << std::endl;
    }

    // create instance of fluid turbulent flow algorithm
    FLD::TurbulentFlowAlgorithm turbfluidalgo(comm, fdyn);

    // read the restart information, set vectors and variables
    if (restart) turbfluidalgo.read_restart(restart);

    // run simulation for a separate part of the complete domain to get turbulent flow in it
    // after restart a turbulent inflow profile is computed in the separate inflow section and
    // transferred as dirichlet boundary condition to the problem domain of interest
    // this finally allows to get high quality turbulent inflow conditions during simulation of the
    // actual flow
    turbfluidalgo.time_loop();

    // perform result tests if required
    Global::Problem::instance()->add_field_test(turbfluidalgo.do_result_check());
    Global::Problem::instance()->test_all(comm);
  }
  // solve a simple fluid problem
  else
  {
    // create instance of fluid basis algorithm
    Adapter::FluidBaseAlgorithm fluidalgo(fdyn, fdyn, "fluid", false);

    // read the restart information, set vectors and variables
    if (restart) fluidalgo.fluid_field()->read_restart(restart);

    // run the simulation
    //    fluidalgo->fluid_field()->TimeLoop();
    fluidalgo.fluid_field()->integrate();

    // perform result tests if required
    Global::Problem::instance()->add_field_test(fluidalgo.fluid_field()->create_field_test());
    Global::Problem::instance()->test_all(comm);
  }

  // have fun with your results!
  return;

}  // end of dyn_fluid_drt()

FOUR_C_NAMESPACE_CLOSE
