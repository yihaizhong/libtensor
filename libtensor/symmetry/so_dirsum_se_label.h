#ifndef LIBTENSOR_SO_DIRSUM_SE_LABEL_H
#define LIBTENSOR_SO_DIRSUM_SE_LABEL_H

#include "se_label.h"
#include "so_dirsum.h"
#include "symmetry_element_set_adapter.h"
#include "symmetry_operation_impl_base.h"

namespace libtensor {

/** \brief Implementation of so_dirsum<N, M, T> for se_label<N, T>
    \tparam N Tensor order.
    \tparam T Tensor element type.

    \ingroup libtensor_symmetry
 **/
template<size_t N, size_t M, typename T>
class symmetry_operation_impl< so_dirsum<N, M, T>, se_label<N + M, T> > :
    public symmetry_operation_impl_base< so_dirsum<N, M, T>,
        se_label<N + M, T> > {
public:
    static const char *k_clazz; //!< Class name

public:
    typedef so_dirsum<N, M, T> operation_t;
    typedef se_label<N + M, T> element_t;
    typedef symmetry_operation_params<operation_t> symmetry_operation_params_t;

public:
    virtual ~symmetry_operation_impl() { }

protected:
    virtual void do_perform(symmetry_operation_params_t &params) const;
};

} // namespace libtensor

#endif // LIBTENSOR_SO_DIRSUM_SE_LABEL_H
