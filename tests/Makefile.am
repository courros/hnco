check_PROGRAMS = \
	test-bm-add-rows \
	test-bm-identity \
	test-bm-invert \
	test-bm-multiply \
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
	test-hamming-sphere \
	test-hypercube-iterator \
	test-max-sat \
	test-sbm-multiply \
	test-serialize-equal-products \
	test-serialize-linear-function \
	test-serialize-max-sat \
	test-serialize-nk-landscape \
	test-serialize-quadratic-function \
	test-serialize-translation \
	test-serialize-permutation \
	test-serialize-affine-map \
	test-serialize-linear-map \
	test-steepest-ascent-hill-climbing

AM_CPPFLAGS = -I $(top_srcdir)/lib

LDADD = ../lib/libhnco.la -ldl -lboost_serialization

test_bm_add_rows_SOURCES = test-bm-add-rows.cc
test_bm_identity_SOURCES = test-bm-identity.cc
test_bm_invert_SOURCES = test-bm-invert.cc
test_bm_multiply_SOURCES = test-bm-multiply.cc
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
test_hamming_sphere_SOURCES = test-hamming-sphere.cc
test_hypercube_iterator_SOURCES = test-hypercube-iterator.cc
test_max_sat_SOURCES = test-max-sat.cc
test_sbm_multiply_SOURCES = test-sbm-multiply.cc
test_serialize_equal_products_SOURCES = test-serialize-equal-products.cc
test_serialize_linear_function_SOURCES = test-serialize-linear-function.cc
test_serialize_max_sat_SOURCES = test-serialize-max-sat.cc
test_serialize_nk_landscape_SOURCES = test-serialize-nk-landscape.cc
test_serialize_quadratic_function_SOURCES = test-serialize-quadratic-function.cc
test_serialize_translation_SOURCES = test-serialize-translation.cc
test_serialize_permutation_SOURCES = test-serialize-permutation.cc
test_serialize_affine_map_SOURCES = test-serialize-affine-map.cc
test_serialize_linear_map_SOURCES = test-serialize-linear-map.cc
test_steepest_ascent_hill_climbing_SOURCES = test-steepest-ascent-hill-climbing.cc

TESTS = $(check_PROGRAMS)

DISTCLEANFILES = test-serialize.txt
