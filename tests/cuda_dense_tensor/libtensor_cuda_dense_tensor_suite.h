#ifndef LIBTENSOR_LIBTENSOR_CUDA_DENSE_TENSOR_SUITE_H
#define LIBTENSOR_LIBTENSOR_CUDA_DENSE_TENSOR_SUITE_H

#include <libtest/test_suite.h>
#include "cuda_tod_add_test.h"
#include "cuda_tod_contract2_test.h"
#include "cuda_tod_copy_test.h"
#include "cuda_tod_copy_hd_test.h"
#include "cuda_tod_set_test.h"

using libtest::unit_test_factory;

namespace libtensor {

/** \defgroup libtensor_cuda_dense_tensor_tests Tests of CUDA tensor operations
    \ingroup libtensor_tests
 **/


/** \brief Test suite for CUDA tensor operations in libtensor

    This suite runs the following tests:
     - libtensor::cuda_tod_add_test
     - libtensor::cuda_tod_contract2_test
     - libtensor::cuda_tod_copy_test
     - libtensor::cuda_tod_copy_hd_test
     - libtensor::cuda_tod_set_test

    \ingroup libtensor_cuda_dense_tensor_tests
 **/
class libtensor_cuda_dense_tensor_suite : public libtest::test_suite {
private:
    unit_test_factory<cuda_tod_add_test> m_utf_cuda_tod_add;
    unit_test_factory<cuda_tod_contract2_test> m_utf_cuda_tod_contract2;
    unit_test_factory<cuda_tod_copy_test> m_utf_cuda_tod_copy;
    unit_test_factory<cuda_tod_copy_hd_test> m_utf_cuda_tod_copy_hd;
    unit_test_factory<cuda_tod_set_test> m_utf_cuda_tod_set;

public:
    //! Creates the suite
    libtensor_cuda_dense_tensor_suite();
};


} // namespace libtensor

#endif // LIBTENSOR_LIBTENSOR_CUDA_DENSE_TENSOR_SUITE_H

