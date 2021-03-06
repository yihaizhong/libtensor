#include "libtensor_symmetry_suite.h"

namespace libtensor {

libtensor_symmetry_suite::libtensor_symmetry_suite() :
                    libtest::test_suite("libtensor") {

    add_test("adjacency_list", m_utf_adjacency_list);
    add_test("block_labeling", m_utf_block_labeling);
    add_test("combine_label", m_utf_combine_label);
    add_test("combine_part", m_utf_combine_part);
    add_test("eval_sequence_list", m_utf_eval_sequence_list);
    add_test("er_merge", m_utf_er_merge);
    add_test("er_optimize", m_utf_er_optimize);
    add_test("er_reduce", m_utf_er_reduce);
    add_test("evaluation_rule", m_utf_evaluation_rule);
    add_test("permutation_group", m_utf_permutation_group);
    add_test("point_group_table", m_utf_point_group_table);
    add_test("product_rule", m_utf_product_rule);
    add_test("product_table_container", m_utf_product_table_container);
    add_test("se_label", m_utf_se_label);
    add_test("se_part", m_utf_se_part);
    add_test("se_perm", m_utf_se_perm);
    add_test("so_apply_se_label", m_utf_so_apply_se_label);
    add_test("so_apply_se_part", m_utf_so_apply_se_part);
    add_test("so_apply_se_perm", m_utf_so_apply_se_perm);
    add_test("so_apply", m_utf_so_apply);
    add_test("so_copy", m_utf_so_copy);
    add_test("so_dirprod_se_label", m_utf_so_dirprod_se_label);
    add_test("so_dirprod_se_part", m_utf_so_dirprod_se_part);
    add_test("so_dirprod_se_perm", m_utf_so_dirprod_se_perm);
    add_test("so_dirprod", m_utf_so_dirprod);
    add_test("so_dirsum_se_label", m_utf_so_dirsum_se_label);
    add_test("so_dirsum_se_part", m_utf_so_dirsum_se_part);
    add_test("so_dirsum_se_perm", m_utf_so_dirsum_se_perm);
    add_test("so_dirsum", m_utf_so_dirsum);
    add_test("so_merge_se_label", m_utf_so_merge_se_label);
    add_test("so_merge_se_part", m_utf_so_merge_se_part);
    add_test("so_merge_se_perm", m_utf_so_merge_se_perm);
    add_test("so_merge", m_utf_so_merge);
    add_test("so_permute_se_label", m_utf_so_permute_se_label);
    add_test("so_permute_se_part", m_utf_so_permute_se_part);
    add_test("so_permute_se_perm", m_utf_so_permute_se_perm);
    add_test("so_reduce_se_label", m_utf_so_reduce_se_label);
    add_test("so_reduce_se_part", m_utf_so_reduce_se_part);
    add_test("so_reduce_se_perm", m_utf_so_reduce_se_perm);
    add_test("so_reduce", m_utf_so_reduce);
    add_test("so_symmetrize_se_label", m_utf_so_symmetrize_se_label);
    add_test("so_symmetrize_se_part", m_utf_so_symmetrize_se_part);
    add_test("so_symmetrize", m_utf_so_symmetrize);
    add_test("symmetry_element_set_adapter",
            m_utf_symmetry_element_set_adapter);
}

}

