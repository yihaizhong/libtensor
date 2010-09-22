#ifndef LIBTENSOR_TASK_BATCH_H
#define LIBTENSOR_TASK_BATCH_H

#include "task_i.h"
#include "task_dispatcher.h"

namespace libtensor {


/**	\brief Collects tasks for single- or multi-processor execution

	\ingroup libtensor_mp
 **/
class task_batch {
private:
	task_dispatcher::queue_id_t m_q; //!< Queue ID

public:
	task_batch() :
		m_q(task_dispatcher::get_instance().create_queue()) { }

	~task_batch() {
		task_dispatcher::get_instance().destroy_queue(m_q);
	}

	void push(task_i &task) {
		task_dispatcher::get_instance().push_task(m_q, task);
	}

	void wait() {
		task_dispatcher::get_instance().wait_on_queue(m_q);
	}
};


} // namespace libtensor

#endif // LIBTENSOR_TASK_BATCH_H