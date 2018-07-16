check_PROGRAMS = \
	test-bm-add-rows \
	test-bm-identity \
	test-bm-invert \
	test-bm-multiply \
	test-bm-rank \
	test-bm-row-column-rank \
	test-bm-solve \
	test-bm-swap-rows \
	test-complete-search-deceptive-jump \
	test-complete-search-four-peaks \
	test-complete-search-hiff \
	test-complete-search-jump \
	test-complete-search-leading-ones \
	test-complete-search-needle \
	test-complete-search-one-max \
	test-complete-search-plateau \
	test-complete-search-ridge \
	test-complete-search-trap \
	test-first-ascent-hill-climbing \
	test-hamming-sphere \
	test-hypercube-iterator \
	test-incremental-one-max-bernoulli-process \
	test-incremental-one-max-hamming-ball \
	test-incremental-one-max-hamming-sphere \
	test-incremental-one-max-single-bit-flip \
	test-max-sat \
	test-sbm-multiply \
	test-serialize-affine-map \
	test-serialize-equal-products \
	test-serialize-linear-function \
	test-serialize-linear-map \
	test-serialize-map-composition \
	test-serialize-max-sat \
	test-serialize-nk-landscape \
	test-serialize-permutation \
	test-serialize-translation \
	test-serialize-walsh-expansion \
	test-serialize-walsh-expansion-1 \
	test-serialize-walsh-expansion-2 \
	test-single-bit-flip-iterator \
	test-steepest-ascent-hill-climbing

AM_CXXFLAGS = $(OPENMP_CXXFLAGS)

AM_CPPFLAGS = -I $(top_srcdir)/lib

LDADD = ../lib/libhnco.la -ldl -lboost_serialization

test_bm_add_rows_SOURCES = test-bm-add-rows.cc
test_bm_identity_SOURCES = test-bm-identity.cc
test_bm_invert_SOURCES = test-bm-invert.cc
test_bm_multiply_SOURCES = test-bm-multiply.cc
test_bm_rank_SOURCES = test-bm-rank.cc
test_bm_row_column_rank_SOURCES = test-bm-row-column-rank.cc
test_bm_solve_SOURCES = test-bm-solve.cc
test_bm_swap_rows_SOURCES = test-bm-swap-rows.cc
test_complete_search_deceptive_jump_SOURCES = test-complete-search-deceptive-jump.cc
test_complete_search_four_peaks_SOURCES = test-complete-search-four-peaks.cc
test_complete_search_hiff_SOURCES = test-complete-search-hiff.cc
test_complete_search_jump_SOURCES = test-complete-search-jump.cc
test_complete_search_leading_ones_SOURCES = test-complete-search-leading-ones.cc
test_complete_search_needle_SOURCES = test-complete-search-needle.cc
test_complete_search_one_max_SOURCES = test-complete-search-one-max.cc
test_complete_search_plateau_SOURCES = test-complete-search-plateau.cc
test_complete_search_ridge_SOURCES = test-complete-search-ridge.cc
test_complete_search_trap_SOURCES = test-complete-search-trap.cc
test_first_ascent_hill_climbing_SOURCES = test-first-ascent-hill-climbing.cc
test_hamming_sphere_SOURCES = test-hamming-sphere.cc
test_hypercube_iterator_SOURCES = test-hypercube-iterator.cc
test_incremental_one_max_bernoulli_process_SOURCES = test-incremental-one-max-bernoulli-process.cc
test_incremental_one_max_hamming_ball_SOURCES = test-incremental-one-max-hamming-ball.cc
test_incremental_one_max_hamming_sphere_SOURCES = test-incremental-one-max-hamming-sphere.cc
test_incremental_one_max_single_bit_flip_SOURCES = test-incremental-one-max-single-bit-flip.cc
test_max_sat_SOURCES = test-max-sat.cc
test_sbm_multiply_SOURCES = test-sbm-multiply.cc
test_serialize_affine_map_SOURCES = test-serialize-affine-map.cc
test_serialize_equal_products_SOURCES = test-serialize-equal-products.cc
test_serialize_linear_function_SOURCES = test-serialize-linear-function.cc
test_serialize_linear_map_SOURCES = test-serialize-linear-map.cc
test_serialize_map_composition_SOURCES = test-serialize-map-composition.cc
test_serialize_max_sat_SOURCES = test-serialize-max-sat.cc
test_serialize_nk_landscape_SOURCES = test-serialize-nk-landscape.cc
test_serialize_permutation_SOURCES = test-serialize-permutation.cc
test_serialize_translation_SOURCES = test-serialize-translation.cc
test_serialize_walsh_expansion_1_SOURCES = test-serialize-walsh-expansion-1.cc
test_serialize_walsh_expansion_2_SOURCES = test-serialize-walsh-expansion-2.cc
test_serialize_walsh_expansion_SOURCES = test-serialize-walsh-expansion.cc
test_single_bit_flip_iterator_iterator_SOURCES = test-single-bit-flip-iterator.cc
test_steepest_ascent_hill_climbing_SOURCES = test-steepest-ascent-hill-climbing.cc

TESTS = $(check_PROGRAMS)

DISTCLEANFILES = \
	test-serialize-affine-map.txt \
	test-serialize-equal-products.txt \
	test-serialize-linear-function.txt \
	test-serialize-linear-map.txt \
	test-serialize-map-composition.txt \
	test-serialize-max-sat.txt \
	test-serialize-nk-landscape.txt \
	test-serialize-permutation.txt \
	test-serialize-translation.txt \
	test-serialize-walsh-expansion-1.txt \
	test-serialize-walsh-expansion-2.txt \
	test-serialize-walsh-expansion.txt
