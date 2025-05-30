// This file is part of 4C multiphysics licensed under the
// GNU Lesser General Public License v3.0 or later.
//
// See the LICENSE.md file in the top-level for license information.
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#ifndef FOUR_C_PRE_EXODUS_VALIDATE_HPP
#define FOUR_C_PRE_EXODUS_VALIDATE_HPP

#include "4C_config.hpp"

#include "4C_fem_general_element.hpp"
#include "4C_io_exodus.hpp"
#include "4C_linalg_serialdensematrix.hpp"

#include <string>

FOUR_C_NAMESPACE_OPEN

namespace EXODUS
{
  //! validate a given outfile
  void validate_input_file(const MPI_Comm comm, const std::string outfile);

  //! Check Elements for positive Jacobian and otherwise 'rewind' them
  void validate_mesh_element_jacobians(Core::IO::Exodus::Mesh& mymesh);

  //! Check for positive Jacobian for Element of distype and otherwise 'rewind' them
  void validate_element_jacobian(Core::IO::Exodus::Mesh& mymesh, const Core::FE::CellType distype,
      const Core::IO::Exodus::ElementBlock&);

  //! Check Elements of distype with full gauss integration rule for positive det at all gps and
  //! return number of negative dets
  int validate_element_jacobian_fullgp(Core::IO::Exodus::Mesh& mymesh,
      const Core::FE::CellType distype, const Core::IO::Exodus::ElementBlock& eb);

  //! Check one element for positive Jacobi determinant
  bool positive_ele(const int& eleid, const std::vector<int>& nodes,
      const Core::IO::Exodus::Mesh& mymesh, const Core::LinAlg::SerialDenseMatrix& deriv);
  int ele_sane_sign(
      const std::vector<int>& nodes, const std::map<int, std::vector<double>>& nodecoords);

  //! Rewind one Element
  std::vector<int> rewind_ele(std::vector<int> old_nodeids, const Core::FE::CellType distype);

}  // namespace EXODUS

FOUR_C_NAMESPACE_CLOSE

#endif
