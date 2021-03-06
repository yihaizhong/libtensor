#include "batch_provider_test.h"
#include "test_fixtures/permute_3d_sparse_120_test_f.h"
#include "test_fixtures/contract2_test_f.h"
#include <libtensor/expr/dag/expr_tree.h>
#include <libtensor/expr/dag/node_assign.h>
#include <libtensor/expr/dag/node_contract.h>
#include <libtensor/expr/iface/node_ident_any_tensor.h>
#include <libtensor/core/scalar_transf_double.h>
#include <libtensor/expr/dag/node_transform.h>
#include <libtensor/block_sparse/sparse_btensor_new.h>
#include <libtensor/block_sparse/batch_provider_new.h>

using namespace std;

namespace libtensor {

using namespace expr;

void batch_provider_test::perform() throw(libtest::test_exception) 
{
    test_permute_3d_sparse_120();
    /*test_contract2();*/
}

void batch_provider_test::test_permute_3d_sparse_120() throw(libtest::test_exception)
{
    static const char *test_name = "batch_provider_test::test_permute_3d_sparse_120()";

    permute_3d_sparse_120_test_f tf;

    sparse_btensor_new<3> A(tf.input_bispace,tf.input_arr,true);
    sparse_btensor_new<3> B(tf.output_bispace);

    node_assign root(3, false);
    expr_tree e(root);
    expr_tree::node_id_t root_id = e.get_root();
    e.add(root_id, node_ident_any_tensor<3,double>(B));

    idx_list perm_entries(1,1);
    perm_entries.push_back(2);
    perm_entries.push_back(0);
    node_transform<double> perm_node(perm_entries, scalar_transf<double>());
    expr_tree::node_id_t perm_node_id = e.add(root_id,perm_node);
    e.add(perm_node_id,node_ident_any_tensor<3,double>(A));

    //Finally, check that we can get the full tensor
    batch_provider_new<double> bp(e);
    bp.get_batch((double*)B.get_data_ptr());

    sparse_btensor_new<3> B_correct(tf.output_bispace,tf.output_arr,true);

    if(B != B_correct)
    {
        fail_test(test_name,__FILE__,__LINE__,
                "batch_provider::get_batch(...) did not return correct value for 3d sparse 120 permutation");
    }
}

void batch_provider_test::test_contract2() throw(libtest::test_exception)
{
    static const char *test_name = "batch_provider_test::test_contract2()";

    contract2_test_f tf;

    sparse_btensor_new<3> A(tf.spb_A,tf.A_arr,true);
    sparse_btensor_new<3> B(tf.spb_B,tf.B_arr,true);
    sparse_btensor_new<2> C(tf.spb_C);

    node_assign root(2, false);
    expr_tree e(root);
    expr_tree::node_id_t root_id = e.get_root();
    e.add(root_id, node_ident_any_tensor<2,double>(C));

    multimap<size_t,size_t> contr_map;
    contr_map.insert(idx_pair(1,3));
    contr_map.insert(idx_pair(2,4));
    node_contract contr_node(2,contr_map,true);
    expr_tree::node_id_t contr_node_id = e.add(root_id,contr_node);
    e.add(contr_node_id,node_ident_any_tensor<3,double>(A));
    e.add(contr_node_id,node_ident_any_tensor<3,double>(B));

    //Finally, check that we can get the full tensor
    batch_provider_new<double> bp(e);
    bp.get_batch((double*)C.get_data_ptr());

    sparse_btensor_new<2> C_correct(tf.spb_C,tf.C_arr,true);

    if(C != C_correct)
    {
        fail_test(test_name,__FILE__,__LINE__,
                "batch_provider::get_batch(...) did not return correct value contract2 test case");
    }
}

} // namespace libtensor
