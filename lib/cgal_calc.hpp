#pragma once
#include "problem.hpp"

#include <CGAL/Exact_predicates_exact_constructions_kernel_with_sqrt.h>
#include <CGAL/Arr_circle_segment_traits_2.h>
#include <CGAL/Surface_sweep_2_algorithms.h>

using Kernel = CGAL::Exact_predicates_exact_constructions_kernel_with_sqrt;
using Point_2 = Kernel::Point_2;
using Segment_2 = Kernel::Segment_2;
using Direction_2 = Kernel::Direction_2;

using Traits_2 = CGAL::Arr_circle_segment_traits_2<Kernel>;
using Curve_2 = Traits_2::Curve_2;

using Res_Point_2 = Traits_2::Point_2;
using SqrtExtT = CGAL::Sqrt_extension<Kernel::FT, Kernel::FT, CGAL::Tag_true, CGAL::Tag_true>;

uint64_t calc_score(Problem problem, std::vector<Point> placements);
