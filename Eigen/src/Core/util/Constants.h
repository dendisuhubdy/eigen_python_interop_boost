// This file is part of Eigen, a lightweight C++ template library
// for linear algebra. Eigen itself is part of the KDE project.
//
// Copyright (C) 2008 Gael Guennebaud <g.gael@free.fr>
// Copyright (C) 2006-2008 Benoit Jacob <jacob@math.jussieu.fr>
//
// Eigen is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public
// License as published by the Free Software Foundation; either
// version 3 of the License, or (at your option) any later version.
//
// Alternatively, you can redistribute it and/or
// modify it under the terms of the GNU General Public License as
// published by the Free Software Foundation; either version 2 of
// the License, or (at your option) any later version.
//
// Eigen is distributed in the hope that it will be useful, but WITHOUT ANY
// WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
// FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License or the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public
// License and a copy of the GNU General Public License along with
// Eigen. If not, see <http://www.gnu.org/licenses/>.

#ifndef EIGEN_CONSTANTS_H
#define EIGEN_CONSTANTS_H

const int Dynamic = 10000;

// matrix/expression flags
const unsigned int RowMajorBit = 0x1;
const unsigned int EvalBeforeNestingBit = 0x2;  ///< means the expression should be evaluated by the calling expression
const unsigned int EvalBeforeAssigningBit = 0x4;///< means the expression should be evaluated before any assignement
const unsigned int LargeBit = 0x8;
#ifdef EIGEN_VECTORIZE
const unsigned int VectorizableBit = 0x10;  ///< means the expression might be vectorized
#else
const unsigned int VectorizableBit = 0x0;
#endif
const unsigned int Like1DArrayBit = 0x20;   ///< means the expression can be seen as 1D vector (used for explicit vectorization)
const unsigned int NullDiagBit = 0x40;      ///< means all diagonal coefficients are equal to 0
const unsigned int UnitDiagBit = 0x80;      ///< means all diagonal coefficients are equal to 1
const unsigned int NullLowerBit = 0x200;    ///< means the strictly triangular lower part is 0
const unsigned int NullUpperBit = 0x400;    ///< means the strictly triangular upper part is 0

enum { Upper=NullLowerBit, Lower=NullUpperBit };

// list of flags that are lost by default
const unsigned int DefaultLostFlagMask = ~(VectorizableBit | Like1DArrayBit | NullDiagBit | UnitDiagBit | NullLowerBit | NullUpperBit);

enum { ConditionalJumpCost = 5 };
enum CornerType { TopLeft, TopRight, BottomLeft, BottomRight };
enum DirectionType { Vertical, Horizontal };
enum ProductEvaluationMode { NormalProduct, CacheOptimalProduct };

#endif // EIGEN_CONSTANTS_H