#include <libtensor/ctf_block_tensor/ctf_btod_diag.h>
#include <libtensor/expr/common/metaprog.h>
#include <libtensor/expr/dag/node_diag.h>
#include <libtensor/expr/eval/eval_exception.h>
#include "ctf_btensor_from_node.h"
#include "eval_ctf_btensor_double_diag.h"

namespace libtensor {
namespace expr {
namespace eval_ctf_btensor_double {

namespace {
using eval_btensor_double::dispatch_1;


template<size_t N>
class eval_diag_impl : public eval_ctf_btensor_evaluator_i<N, double> {
private:
    enum {
        Nmax = diag<N>::Nmax
    };

public:
    typedef typename eval_ctf_btensor_evaluator_i<N, double>::bti_traits
        bti_traits;

private:
    struct dispatch_diag {
        eval_diag_impl &eval;
        const tensor_transf<N, double> &trc;
        size_t na;

        dispatch_diag(
            eval_diag_impl &eval_,
            const tensor_transf<N, double> &trc_,
            size_t na_) :
            eval(eval_), trc(trc_), na(na_)
        { }

        template<size_t NA> void dispatch();
    };

private:
    const expr_tree &m_tree; //!< Expression tree
    expr_tree::node_id_t m_id; //!< ID of copy node
    additive_gen_bto<N, bti_traits> *m_op; //!< Block tensor operation

public:
    eval_diag_impl(const expr_tree &tr, expr_tree::node_id_t id,
        const tensor_transf<N, double> &trc);

    virtual ~eval_diag_impl();

    virtual additive_gen_bto<N, bti_traits> &get_bto() const {
        return *m_op;
    }

    template<size_t NA, size_t NB>
    void init(const tensor_transf<N, double> &trc);

};


template<size_t N>
eval_diag_impl<N>::eval_diag_impl(const expr_tree &tree,
    expr_tree::node_id_t id, const tensor_transf<N, double> &trc) :

    m_tree(tree), m_id(id), m_op(0) {

    const expr_tree::edge_list_t &e = m_tree.get_edges_out(m_id);
    const node_diag &nd =
    		m_tree.get_vertex(m_id).template recast_as<node_diag>();

    const node &arga = m_tree.get_vertex(e[0]);
    size_t na = arga.get_n();

    dispatch_diag disp(*this, trc, na);
    dispatch_1<N + 1, Nmax>::dispatch(disp, na);
}


template<size_t N>
eval_diag_impl<N>::~eval_diag_impl() {

    delete m_op;
}


template<size_t N> template<size_t NA, size_t M>
void eval_diag_impl<N>::init(const tensor_transf<N, double> &trc) {

    const expr_tree::edge_list_t &e = m_tree.get_edges_out(m_id);
    const node_diag &nd =
    		m_tree.get_vertex(m_id).template recast_as<node_diag>();

    ctf_btensor_from_node<NA, double> bta(m_tree, e[0]);

    sequence<NA, size_t> m(0);

    const std::vector<size_t> &idx = nd.get_idx();
    const std::vector<size_t> &didx = nd.get_didx();
    for(size_t i = 0; i < NA; i++) {
        if(idx[i] < didx.size()) m[i] = didx[idx[i]] + 1;
    }

    double d = bta.get_transf().get_scalar_tr().get_coeff() *
        trc.get_scalar_tr().get_coeff();
    m_op = new ctf_btod_diag<NA, NA - M + 1>(bta.get_btensor(), m,
        trc.get_perm(), d);
}


template<size_t N> template<size_t NA>
void eval_diag_impl<N>::dispatch_diag::dispatch() {

    eval.template init<NA, NA + 1 - N>(trc);
}


} // unnamed namespace


template<size_t N>
diag<N>::diag(const expr_tree &tree, node_id_t &id,
    const tensor_transf<N, double> &tr) :

    m_impl(new eval_diag_impl<N>(tree, id, tr)) {

}


template<size_t N>
diag<N>::~diag() {

    delete m_impl;
}


#if 0
//  The code here explicitly instantiates copy<N>
namespace aux {
template<size_t N>
struct aux_diag {
    const expr_tree *tree;
    expr_tree::node_id_t id;
    const tensor_transf<N, double> *tr;
    const node *t;
    diag<N> *e;
    aux_diag() {
#pragma noinline
        { e = new diag<N>(*tree, id, *tr); }
    }
};
} // namespace aux
template class instantiate_template_1<1, eval_btensor<double>::Nmax,
    aux::aux_diag>;
#endif
template class diag<1>;
template class diag<2>;
template class diag<3>;
template class diag<4>;
template class diag<5>;
template class diag<6>;
template class diag<7>;
template class diag<8>;


} // namespace eval_ctf_btensor_double
} // namespace expr
} // namespace libtensor
