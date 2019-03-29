#include "stdafx.h"
#include "Lock.h"
#include "Thread.h"

#ifdef _DEBUG
#define STERN_MODE
#endif

const int INVALID_LINE = -1;

Lock::Lock(WCHAR *name)
{
	lockId_ = LockManager::getInstance().generalId();
	name_ = name;

	lockingFile_.clear();
	lockingLine_ = INVALID_LINE;
}

Lock::~Lock()
{
	name_.clear();
}

const WCHAR* Lock::name() 
{
	return name_.c_str();
}

size_t Lock::lockId()
{
	return lockId_;
}

lock_t& Lock::mutex()
{
	return mutex_;
}

void Lock::lock(LPCWSTR fileName, int lineNo)
{
	mutex_.lock();

	lockingFile_ = fileName;
	lockingLine_ = lineNo;
}

void Lock::unlock()
{
	mutex_.unlock();

	lockingFile_.clear();
	lockingLine_ = INVALID_LINE;
}

void Lock::setThreadId(size_t id)
{
	threadId_ = id;
}

size_t Lock::threadId()
{
	return threadId_;
}
//-------------------------------------------------------//
// �����ڿ��� ���� �ɰ�, ������ ���������� ��ü ������ ���� Ǭ��.
// std::lock_guard<lock_t> guard(lock) �� ����������, 
// ����� ������ ���� ���� �����Ѵ�.
LockSafeScope::LockSafeScope(Lock *lock, LPCWSTR fileName, int lineNo)
{
	if (lock == nullptr) {
		return;
	}
	if (_shutdown == true) {
		return;
	}

	lock_ = lock;
	Lock *deadLock = LockManager::getInstance().checkDeadLock(lock_);
	if (deadLock != nullptr) {
#ifdef STERN_MODE
		SErrLog(L"! [%s]lock and [%s]lock is dead detecting!", deadLock->name(), lock->name());
#else
		std::lock(lock_->mutex(), deadLock->mutex());
		SLog(L"!!! [%s]lock and [%s]lock is dead detecting!", deadLock->name(), lock->name());
#endif
		return;
	}

	lock_->lock(fileName, lineNo);
	lock->setThreadId(GET_CURRENT_THREAD_ID());
}

LockSafeScope::~LockSafeScope()
{
	if (!lock_) {
		return;
	}
	if (_shutdown == true) {
		return;
	}
	lock_->unlock();
	lock_->setThreadId(0);
}

//-------------------------------------------------------//
LockManager::LockManager()
{
	idSeed_ = 0;
}

/*
http://www.npteam.net/849
http://www.boost.org/doc/libs/1_46_1/libs/graph/doc/astar_visitor.html
http://includestdio.tistory.com/m/post/12

http://www.bogotobogo.com/cplusplus/multithreaded4_cplusplus11B.php#nested_lock
���� �ڽ��� �ɸ� threadId��,
Thread������ �ɰ��ִ� Lock ��ȣ�� ����.
�̵��� ���� ���ٰ� �ڽ��� �ɷ��� lock�� ������ lock ��ȯ
�� ����� ó��.
*/
Lock* LockManager::searchLockCycle(Lock *newLock)
{
	//list ���� lock �̸��� ���� ����.
	Thread *thread = ThreadManager::getInstance().at(GET_CURRENT_THREAD_ID());
	if (!thread) {
		return nullptr;
	}
	std::vector<Lock *> trace;		// ����� Ž����, �ɸ� �� stact ��ô
	trace.push_back(newLock);

	Lock *deadLock = nullptr;
	while (true) {
		Lock *threadLock = thread->lock();
		if (threadLock == nullptr) {
			break;
		}
		if (threadLock->lockId() == trace[0]->lockId()) {
			deadLock = threadLock;
			break;
		}
		trace.push_back(threadLock);
		thread = ThreadManager::getInstance().at(threadLock->threadId());
		if (!thread) {
			break;
		}
	}
	trace.empty();
	return deadLock;
}

Lock* LockManager::checkDeadLock(Lock *newLock)
{
	Lock *deadLock = this->searchLockCycle(newLock);
	if (deadLock) {
		return deadLock;
	}

	return nullptr;
}

size_t LockManager::generalId()
{
	size_t id = idSeed_++;
	return id;
}
