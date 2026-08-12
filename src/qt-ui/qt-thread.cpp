#include "thread.h"

#include <thread>
#include <mutex>
#include <condition_variable>
#include <chrono>

#ifdef _WIN32
#include <windows.h>
#else
#include <pthread.h>
#endif

/* thread_t : wraps std::thread */
struct thread_impl {
	std::thread t;
};

thread_t *thread_create(void (*thread_rout)(void *param), void *param) {
	auto *impl = new thread_impl;
	impl->t = std::thread(thread_rout, param);
	return (thread_t *)impl;
}

/* The emulator's worker threads (render/fifo/blit threads etc.) are infinite
   loops that block on events and never return on their own, so they cannot be
   stopped gracefully with join(). thread_kill() must therefore forcibly
   terminate the underlying native thread, matching the pre-refactor
   pthread_cancel()/TerminateThread() behavior. */
void thread_kill(thread_t *handle) {
	auto *impl = (thread_impl *)handle;
	if (impl->t.joinable()) {
#ifdef _WIN32
		TerminateThread(impl->t.native_handle(), 0);
#else
		pthread_cancel(impl->t.native_handle());
#endif
		impl->t.join();
	}
	delete impl;
}

/* event_t : wraps std::condition_variable + state flag */
struct event_impl {
	std::mutex m;
	std::condition_variable cv;
	bool state = false;
};

event_t *thread_create_event() {
	return (event_t *)new event_impl;
}

void thread_set_event(event_t *handle) {
	auto *e = (event_impl *)handle;
	{
		std::lock_guard<std::mutex> lk(e->m);
		e->state = true;
	}
	e->cv.notify_all();
}

void thread_reset_event(event_t *handle) {
	auto *e = (event_impl *)handle;
	std::lock_guard<std::mutex> lk(e->m);
	e->state = false;
}

int thread_wait_event(event_t *handle, int timeout) {
	auto *e = (event_impl *)handle;
	std::unique_lock<std::mutex> lk(e->m);
	if (timeout == -1) {
		e->cv.wait(lk, [e] { return e->state; });
	} else {
		if (!e->cv.wait_for(lk, std::chrono::milliseconds(timeout),
		                    [e] { return e->state; }))
			return 1; /* timeout */
	}
	return 0;
}

void thread_destroy_event(event_t *handle) {
	delete (event_impl *)handle;
}

/* mutex_t : wraps std::recursive_mutex */
struct mutex_impl {
	std::recursive_mutex m;
};

mutex_t *thread_create_mutex(void) {
	return (mutex_t *)new mutex_impl;
}

void thread_lock_mutex(mutex_t *handle) {
	((mutex_impl *)handle)->m.lock();
}

void thread_unlock_mutex(mutex_t *handle) {
	((mutex_impl *)handle)->m.unlock();
}

void thread_destroy_mutex(mutex_t *handle) {
	delete (mutex_impl *)handle;
}

void thread_sleep(int t) {
	std::this_thread::sleep_for(std::chrono::milliseconds(t));
}
