/* Copyright (C) 2016, 2017, 2018, 2019, 2020, 2021, 2022 Arnaud Berny

   This file is part of HNCO.

   HNCO is free software: you can redistribute it and/or modify it
   under the terms of the GNU Lesser General Public License as
   published by the Free Software Foundation, either version 3 of the
   License, or (at your option) any later version.

   HNCO is distributed in the hope that it will be useful, but WITHOUT
   ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
   or FITNESS FOR A PARTICULAR PURPOSE. See the GNU Lesser General
   Public License for more details.

   You should have received a copy of the GNU Lesser General Public
   License along with HNCO. If not, see
   <http://www.gnu.org/licenses/>.

*/

/** \file

    Check Nsga2ParetoFrontComputation.

*/

#include "hnco/multiobjective/algorithms/pareto-front-computation.hh" // hnco::multiobjective::algorithm::Nsga2ParetoFrontComputation

#include "test-pareto-front-computation.hh"


int main(int argc, char *argv[])
{
  Generator::set_seed();

  if (check<Nsga2ParetoFrontComputation>())
    return 0;
  else
    return 1;
}
