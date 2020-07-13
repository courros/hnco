# Copyright (C) 2016, 2017, 2018, 2019, 2020 Arnaud Berny

# This file is part of HNCO.

# HNCO is free software: you can redistribute it and/or modify it
# under the terms of the GNU Lesser General Public License as
# published by the Free Software Foundation, either version 3 of the
# License, or (at your option) any later version.

# HNCO is distributed in the hope that it will be useful, but WITHOUT
# ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
# or FITNESS FOR A PARTICULAR PURPOSE. See the GNU Lesser General
# Public License for more details.

# You should have received a copy of the GNU Lesser General Public
# License along with HNCO. If not, see
# <http://www.gnu.org/licenses/>.

check_PROGRAMS = \
	test-bm-add-columns \
	test-bm-add-rows \
	test-bm-identity \
	test-bm-invert \
	test-bm-multiply \
	test-bm-rank \
	test-bm-row-column-rank \
	test-bm-solve \
	test-bm-swap-rows \
	test-bv-from-stream \
	test-bv-random \
	test-bv-to-size-type \
	test-complete-search-deceptive-jump \
	test-complete-search-four-peaks \
	test-complete-search-hiff \
	test-complete-search-jump \
	test-complete-search-leading-ones \
	test-complete-search-linear-function \
	test-complete-search-long-path \
	test-complete-search-max-sat \
	test-complete-search-needle \
	test-complete-search-one-max \
	test-complete-search-plateau \
	test-complete-search-ridge \
	test-complete-search-six-peaks \
	test-complete-search-trap \
	test-complete-search-walsh-expansion-1 \
	test-dyadic-integer-representation \
	test-dyadic-real-representation \
	test-first-ascent-hill-climbing \
	test-first-ascent-hill-climbing-local \
	test-hamming-sphere \
	test-hamming-sphere-iterator \
	test-hypercube-iterator \
	test-incremental-linear-function-bernoulli-process \
	test-incremental-negation \
	test-incremental-nn-ising-1 \
	test-incremental-nn-ising-2 \
	test-incremental-one-max-bernoulli-process \
	test-incremental-one-max-hamming-ball \
	test-incremental-one-max-hamming-sphere \
	test-incremental-one-max-single-bit-flip \
	test-incremental-walsh-expansion-1-bernoulli-process \
	test-injection-projection \
	test-one-plus-one-ea-one-max \
	test-population-evaluate-in-parallel \
	test-sbm-multiply \
	test-serialize-affine-map \
	test-serialize-equal-products \
	test-serialize-linear-function \
	test-serialize-linear-map \
	test-serialize-map-composition \
	test-serialize-max-sat \
	test-serialize-nk-landscape \
	test-serialize-nn-ising-1 \
	test-serialize-nn-ising-2 \
	test-serialize-partition \
	test-serialize-permutation \
	test-serialize-translation \
	test-serialize-ts-affine-map \
	test-serialize-walsh-expansion \
	test-serialize-walsh-expansion-1 \
	test-serialize-walsh-expansion-2 \
	test-single-bit-flip-iterator \
	test-steepest-ascent-hill-climbing \
	test-steepest-ascent-hill-climbing-local \
	test-translation \
	test-transvection-is-involution \
	test-ts-multiply \
	test-ts-random-commuting \
	test-ts-random-commuting-is-involution \
	test-ts-random-disjoint \
	test-ts-random-disjoint-is-involution \
	test-ts-random-non-commuting \
	test-ts-random-unique-destination-is-involution \
	test-ts-random-unique-source-is-involution \
	test-walsh-transform

if ENABLE_FACTORIZATION
check_PROGRAMS += test-complete-search-factorization
endif

AM_CXXFLAGS = $(OPENMP_CXXFLAGS)

AM_CPPFLAGS = -I $(top_srcdir)/lib

LDADD = ../lib/libhnco.la -ldl -lboost_serialization

test_bm_add_columns_SOURCES = test-bm-add-columns.cc
test_bm_add_rows_SOURCES = test-bm-add-rows.cc
test_bm_identity_SOURCES = test-bm-identity.cc
test_bm_invert_SOURCES = test-bm-invert.cc
test_bm_multiply_SOURCES = test-bm-multiply.cc
test_bm_rank_SOURCES = test-bm-rank.cc
test_bm_row_column_rank_SOURCES = test-bm-row-column-rank.cc
test_bm_solve_SOURCES = test-bm-solve.cc
test_bm_swap_rows_SOURCES = test-bm-swap-rows.cc
test_bv_from_stream_SOURCES = test-bv-from-stream.cc
test_bv_random_SOURCES = test-bv-random.cc
test_bv_to_size_type_SOURCES = 	test-bv-to-size-type.cc
test_complete_search_deceptive_jump_SOURCES = test-complete-search-deceptive-jump.cc
test_complete_search_factorization_SOURCES = test-complete-search-factorization.cc
test_complete_search_four_peaks_SOURCES = test-complete-search-four-peaks.cc
test_complete_search_hiff_SOURCES = test-complete-search-hiff.cc
test_complete_search_jump_SOURCES = test-complete-search-jump.cc
test_complete_search_leading_ones_SOURCES = test-complete-search-leading-ones.cc
test_complete_search_linear_function_SOURCES = test-complete-search-linear-function.cc
test_complete_search_long_path_SOURCES = test-complete-search-long-path.cc
test_complete_search_max_sat_SOURCES = test-complete-search-max-sat.cc
test_complete_search_needle_SOURCES = test-complete-search-needle.cc
test_complete_search_one_max_SOURCES = test-complete-search-one-max.cc
test_complete_search_plateau_SOURCES = test-complete-search-plateau.cc
test_complete_search_ridge_SOURCES = test-complete-search-ridge.cc
test_complete_search_six_peaks_SOURCES = test-complete-search-six-peaks.cc
test_complete_search_trap_SOURCES = test-complete-search-trap.cc
test_complete_search_walsh_expansion_1_SOURCES = test-complete-search-walsh-expansion-1.cc
test_dyadic_integer_representation_SOURCES = test-dyadic-integer-representation.cc
test_dyadic_real_representation_SOURCES = test-dyadic-real-representation.cc
test_first_ascent_hill_climbing_SOURCES = test-first-ascent-hill-climbing.cc
test_first_ascent_hill_climbing_local_SOURCES = test-first-ascent-hill-climbing-local.cc
test_hamming_sphere_SOURCES = test-hamming-sphere.cc
test_hamming_sphere_iterator_SOURCES = test-hamming-sphere-iterator.cc
test_hypercube_iterator_SOURCES = test-hypercube-iterator.cc
test_incremental_linear_function_bernoulli_process_SOURCES = test-incremental-linear-function-bernoulli-process.cc
test_incremental_negation_SOURCES = test-incremental-negation.cc
test_incremental_nn_ising_1_SOURCES = test-incremental-nn-ising-1.cc
test_incremental_nn_ising_2_SOURCES = test-incremental-nn-ising-2.cc
test_incremental_one_max_bernoulli_process_SOURCES = test-incremental-one-max-bernoulli-process.cc
test_incremental_one_max_hamming_ball_SOURCES = test-incremental-one-max-hamming-ball.cc
test_incremental_one_max_hamming_sphere_SOURCES = test-incremental-one-max-hamming-sphere.cc
test_incremental_one_max_single_bit_flip_SOURCES = test-incremental-one-max-single-bit-flip.cc
test_incremental_walsh_expansion_1_bernoulli_process_SOURCES = test-incremental-walsh-expansion-1-bernoulli-process.cc
test_injection_projection_SOURCES = test-injection-projection.cc
test_one_plus_one_ea_one_max_SOURCES = test-one-plus-one-ea-one-max.cc
test_population_evaluate_in_parallel_SOURCES = test-population-evaluate-in-parallel.cc
test_sbm_multiply_SOURCES = test-sbm-multiply.cc
test_serialize_affine_map_SOURCES = test-serialize-affine-map.cc
test_serialize_equal_products_SOURCES = test-serialize-equal-products.cc
test_serialize_linear_function_SOURCES = test-serialize-linear-function.cc
test_serialize_linear_map_SOURCES = test-serialize-linear-map.cc
test_serialize_map_composition_SOURCES = test-serialize-map-composition.cc
test_serialize_max_sat_SOURCES = test-serialize-max-sat.cc
test_serialize_nk_landscape_SOURCES = test-serialize-nk-landscape.cc
test_serialize_nn_ising_1_SOURCES = test-serialize-nn-ising-1.cc
test_serialize_nn_ising_2_SOURCES = test-serialize-nn-ising-2.cc
test_serialize_partition_SOURCES = test-serialize-partition.cc
test_serialize_permutation_SOURCES = test-serialize-permutation.cc
test_serialize_translation_SOURCES = test-serialize-translation.cc
test_serialize_ts_affine_map_SOURCES = test-serialize-ts-affine-map.cc
test_serialize_walsh_expansion_1_SOURCES = test-serialize-walsh-expansion-1.cc
test_serialize_walsh_expansion_2_SOURCES = test-serialize-walsh-expansion-2.cc
test_serialize_walsh_expansion_SOURCES = test-serialize-walsh-expansion.cc
test_single_bit_flip_iterator_SOURCES = test-single-bit-flip-iterator.cc
test_steepest_ascent_hill_climbing_SOURCES = test-steepest-ascent-hill-climbing.cc
test_steepest_ascent_hill_climbing_local_SOURCES = test-steepest-ascent-hill-climbing-local.cc
test_translation_SOURCES = test-translation.cc
test_transvection_is_involution_SOURCES = test-transvection-is-involution.cc
test_ts_multiply_SOURCES = test-ts-multiply.cc
test_ts_random_commuting_SOURCES = test-ts-random-commuting.cc
test_ts_random_commuting_is_involution_SOURCES = test-ts-random-commuting-is-involution.cc
test_ts_random_disjoint_SOURCES = test-ts-random-disjoint.cc
test_ts_random_disjoint_is_involution_SOURCES = test-ts-random-disjoint-is-involution.cc
test_ts_random_non_commuting_SOURCES = test-ts-random-non-commuting.cc
test_ts_random_unique_destination_is_involution_SOURCES = test-ts-random-unique-destination-is-involution.cc
test_ts_random_unique_source_is_involution_SOURCES = test-ts-random-unique-source-is-involution.cc
test_walsh_transform_SOURCES = test-walsh-transform.cc

TESTS = $(check_PROGRAMS)

DISTCLEANFILES = \
	test-bv-from-stream.txt \
	test-serialize-affine-map.txt \
	test-serialize-equal-products.txt \
	test-serialize-linear-function.txt \
	test-serialize-linear-map.txt \
	test-serialize-map-composition.txt \
	test-serialize-max-sat.txt \
	test-serialize-nk-landscape.txt \
	test-serialize-nn-ising-1.txt \
	test-serialize-nn-ising-2.txt \
	test-serialize-partition.txt \
	test-serialize-permutation.txt \
	test-serialize-translation.txt \
	test-serialize-ts-affine-map.txt \
	test-serialize-walsh-expansion-1.txt \
	test-serialize-walsh-expansion-2.txt \
	test-serialize-walsh-expansion.txt
