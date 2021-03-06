#ifndef LIBTENSOR_BLOCK_TENSOR_TRAITS_H
#define LIBTENSOR_BLOCK_TENSOR_TRAITS_H

#include <libtensor/dense_tensor/dense_tensor.h>
#include "block_factory.h"
#include "block_tensor_i_traits.h"

namespace libtensor {


template<size_t N, typename T> class dense_tensor_i;


/** \brief Block tensor traits
    \tparam T Tensor element type.
    \tparam Alloc Memory allocator.

    This structure specifies the types that define the identity of simple
    block tensors.

    \ingroup libtensor_block_tensor
 **/
template<typename T, typename Alloc>
struct block_tensor_traits {

    //! Type of tensor elements
    typedef T element_type;

    //! Type of allocator
    typedef Alloc allocator_type;

    //! Traits of block tensor interface
    typedef block_tensor_i_traits<T> bti_traits;

    //! Type of blocks
    template<size_t N>
    struct block_type {
        typedef dense_tensor<N, T, Alloc> type;
    };

    //! Type of block factory
    template<size_t N>
    struct block_factory_type {
        typedef block_factory<N, T, typename block_type<N>::type> type;
    };

};


} // namespace libtensor

#endif // LIBTENSOR_BLOCK_TENSOR_TRAITS_H
