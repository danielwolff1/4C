// This file is part of 4C multiphysics licensed under the
// GNU Lesser General Public License v3.0 or later.
//
// See the LICENSE.md file in the top-level for license information.
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#ifndef FOUR_C_PARTICLE_ENGINE_UNIQUE_GLOBAL_ID_HPP
#define FOUR_C_PARTICLE_ENGINE_UNIQUE_GLOBAL_ID_HPP

/*---------------------------------------------------------------------------*
 | headers                                                                   |
 *---------------------------------------------------------------------------*/
#include "4C_config.hpp"

#include <mpi.h>

#include <map>
#include <memory>
#include <string>
#include <vector>

FOUR_C_NAMESPACE_OPEN

/*---------------------------------------------------------------------------*
 | forward declarations                                                      |
 *---------------------------------------------------------------------------*/
class RuntimeVtpWriter;

namespace Core::IO
{
  class DiscretizationWriter;
  class DiscretizationReader;
}  // namespace Core::IO

/*---------------------------------------------------------------------------*
 | class declarations                                                        |
 *---------------------------------------------------------------------------*/
namespace PARTICLEENGINE
{
  /*!
   * \brief unique global identifier handler
   *
   * A class that handles unique global identifiers (or also denoted global ids) for objects (e.g.,
   * particles) in particle simulations, meaning assignment of global ids to newly generated
   * objects, collecting of global ids of removed objects, and re-usage of free global ids.
   *
   * \note This requires communication among the processors in order to ensure that all global ids
   * are unique.
   *
   */
  class UniqueGlobalIdHandler final
  {
   public:
    /*!
     * \brief constructor
     *
     *
     * \param[in] comm communicator
     */
    explicit UniqueGlobalIdHandler(MPI_Comm comm, const std::string& objectname);

    /*!
     * \brief init unique global identifier handler
     *
     */
    void init();

    /*!
     * \brief setup unique global identifier handler
     *
     */
    void setup();

    /*!
     * \brief write restart of unique global identifier handler
     *
     *
     * \param[in] writer discretization writer
     */
    void write_restart(std::shared_ptr<Core::IO::DiscretizationWriter> writer) const;

    /*!
     * \brief read restart of unique global identifier handler
     *
     *
     * \param[in] reader discretization reader
     */
    void read_restart(const std::shared_ptr<Core::IO::DiscretizationReader> reader);

    /*!
     * \brief return maximum global id
     *
     *
     * \return maximum global id
     */
    inline int get_max_global_id() { return maxglobalid_; }

    /*!
     * \brief return number of reusable global ids on this processor
     *
     *
     * \return number of reusable global ids on this processor
     */
    inline int get_number_of_reusable_global_ids() { return reusableglobalids_.size(); }

    /*!
     * \brief insert freed global id
     *
     * The global id of an object that is removed from the simulation is freed and stored herein to
     * be reassigned in case of new object generation.
     *
     *
     * \param[in] freedglobalid freed global id
     */
    inline void insert_freed_global_id(int freedglobalid)
    {
      reusableglobalids_.push_back(freedglobalid);
    }

    /*!
     * \brief draw requested number of global ids
     *
     *
     * \param[in,out] requesteduniqueglobalids requested unique global ids
     */
    void draw_requested_number_of_global_ids(std::vector<int>& requesteduniqueglobalids);

   private:
    /*!
     * \brief gather reusable global ids from all processors on master processor
     *
     * Gather all reusable global ids from all processors on the master processor. The gathered
     * reusable global ids are sorted on the master processor. On all non-master processors the
     * storage of reusable global ids is cleared.
     *
     */
    void gather_reusable_global_ids_from_all_procs_on_master_proc();

    /*!
     * \brief prepare requested global ids for all processors
     *
     *
     * \param[in]     numberofrequestedgids number of requested global ids on this processor
     * \param[in,out] preparedglobalids     prepared global ids
     */
    void prepare_requested_global_ids_for_all_procs(
        int numberofrequestedgids, std::map<int, std::vector<int>>& preparedglobalids);

    /*!
     * \brief extract requested global ids on master processor
     *
     *
     * \param[in]     preparedglobalids        prepared global ids
     * \param[in,out] requesteduniqueglobalids requested unique global ids
     */
    void extract_requested_global_ids_on_master_proc(
        std::map<int, std::vector<int>>& preparedglobalids,
        std::vector<int>& requesteduniqueglobalids) const;

    /*!
     * \brief distribute requested global ids from master processor to all processors
     *
     *
     * \param[in]     tobesendglobalids        global ids to be send to each processor
     * \param[in,out] requesteduniqueglobalids requested unique global ids
     */
    void distribute_requested_global_ids_from_master_proc_to_all_procs(
        std::map<int, std::vector<int>>& tobesendglobalids,
        std::vector<int>& requesteduniqueglobalids) const;

    //! communicator
    MPI_Comm comm_;

    //! processor id
    const int myrank_;

    //! master processor id
    const int masterrank_;

    //! object name
    const std::string objectname_;

    /*!
     * \brief maximum global id
     *
     * This is the maximum global id either actively used in the simulation by an object or as
     * present in the reusable global ids.
     *
     * \note This maximum global id is solely incremented on the master processor but broadcasted to
     * all other processors.
     *
     * \note The global ids are assigned starting from zero.
     *
     */
    int maxglobalid_;

    //! reusable global ids
    std::vector<int> reusableglobalids_;
  };
}  // namespace PARTICLEENGINE

/*---------------------------------------------------------------------------*/
FOUR_C_NAMESPACE_CLOSE

#endif
