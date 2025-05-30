// This file is part of 4C multiphysics licensed under the
// GNU Lesser General Public License v3.0 or later.
//
// See the LICENSE.md file in the top-level for license information.
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#include "4C_w1_poro_p1.hpp"

#include "4C_comm_pack_helpers.hpp"
#include "4C_comm_utils_factory.hpp"
#include "4C_w1_poro_p1_eletypes.hpp"

FOUR_C_NAMESPACE_OPEN

template <Core::FE::CellType distype>
Discret::Elements::Wall1PoroP1<distype>::Wall1PoroP1(int id, int owner)
    : Discret::Elements::Wall1Poro<distype>(id, owner)
{
}

template <Core::FE::CellType distype>
Discret::Elements::Wall1PoroP1<distype>::Wall1PoroP1(
    const Discret::Elements::Wall1PoroP1<distype>& old)
    : Discret::Elements::Wall1Poro<distype>(old)
{
}

template <Core::FE::CellType distype>
Core::Elements::Element* Discret::Elements::Wall1PoroP1<distype>::clone() const
{
  auto* newelement = new Discret::Elements::Wall1PoroP1<distype>(*this);
  return newelement;
}

template <Core::FE::CellType distype>
void Discret::Elements::Wall1PoroP1<distype>::pack(Core::Communication::PackBuffer& data) const
{
  // pack type of this instance of ParObject
  int type = unique_par_object_id();
  add_to_pack(data, type);

  // add base class Element
  Base::pack(data);
}

template <Core::FE::CellType distype>
void Discret::Elements::Wall1PoroP1<distype>::unpack(Core::Communication::UnpackBuffer& buffer)
{
  Core::Communication::extract_and_assert_id(buffer, unique_par_object_id());

  // extract base class Element
  Base::unpack(buffer);
}

template <Core::FE::CellType distype>
std::vector<std::shared_ptr<Core::Elements::Element>>
Discret::Elements::Wall1PoroP1<distype>::lines()
{
  return Core::Communication::element_boundary_factory<Wall1Line, Wall1PoroP1>(
      Core::Communication::buildLines, *this);
}

template <Core::FE::CellType distype>
std::vector<std::shared_ptr<Core::Elements::Element>>
Discret::Elements::Wall1PoroP1<distype>::surfaces()
{
  return {Core::Utils::shared_ptr_from_ref(*this)};
}

template <Core::FE::CellType distype>
void Discret::Elements::Wall1PoroP1<distype>::print(std::ostream& os) const
{
  os << "Wall1_PoroP1 ";
  Core::Elements::Element::print(os);
  std::cout << std::endl;
}

template <Core::FE::CellType distype>
int Discret::Elements::Wall1PoroP1<distype>::unique_par_object_id() const
{
  switch (distype)
  {
    case Core::FE::CellType::tri3:
      return Discret::Elements::WallTri3PoroP1Type::instance().unique_par_object_id();
    case Core::FE::CellType::quad4:
      return Discret::Elements::WallQuad4PoroP1Type::instance().unique_par_object_id();
    case Core::FE::CellType::quad9:
      return Discret::Elements::WallQuad9PoroP1Type::instance().unique_par_object_id();
    default:
      FOUR_C_THROW("unknown element type");
  }
  return -1;
}

template <Core::FE::CellType distype>
Core::Elements::ElementType& Discret::Elements::Wall1PoroP1<distype>::element_type() const
{
  {
    switch (distype)
    {
      case Core::FE::CellType::tri3:
        return Discret::Elements::WallTri3PoroP1Type::instance();
      case Core::FE::CellType::quad4:
        return Discret::Elements::WallQuad4PoroP1Type::instance();
      case Core::FE::CellType::quad9:
        return Discret::Elements::WallQuad9PoroP1Type::instance();
      default:
        FOUR_C_THROW("unknown element type");
    }
    return Discret::Elements::WallQuad4PoroP1Type::instance();
  }
}

template class Discret::Elements::Wall1PoroP1<Core::FE::CellType::tri3>;
template class Discret::Elements::Wall1PoroP1<Core::FE::CellType::quad4>;
template class Discret::Elements::Wall1PoroP1<Core::FE::CellType::quad9>;

FOUR_C_NAMESPACE_CLOSE
