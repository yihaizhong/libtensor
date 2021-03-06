#ifndef LIBTENSOR_EXPR_EVAL_CTF_BTENSOR_DOUBLE_DOT_PRODUCT_H
#define LIBTENSOR_EXPR_EVAL_CTF_BTENSOR_DOUBLE_DOT_PRODUCT_H

#include "../eval_ctf_btensor.h"

namespace libtensor {
namespace expr {
namespace eval_ctf_btensor_double {


class dot_product {
public:
    enum {
        Nmax = eval_ctf_btensor<double>::Nmax
    };

    typedef expr_tree::node_id_t node_id_t; //!< Node ID type

private:
    const expr_tree &m_tree; //!< Expression tree
    node_id_t m_id; //!< ID of diag node

public:
    dot_product(const expr_tree &tr, node_id_t &id) :
        m_tree(tr), m_id(id)
    { }

    void evaluate(node_id_t lhs);

};


} // namespace eval_ctf_btensor_double
} // namespace expr
} // namespace libtensor

#endif // LIBTENSOR_EXPR_EVAL_CTF_BTENSOR_DOUBLE_DOT_PRODUCT_H
