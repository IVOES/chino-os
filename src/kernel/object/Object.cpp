//
// Kernel Object
//
#include "Object.hpp"
#include "../kdebug.hpp"

using namespace Chino;

Object::Object()
	:refCount_(1)
{
}

Object::~Object()
{
}

void Object::AddRef() noexcept
{
	refCount_.fetch_add(1, std::memory_order_relaxed);
}

bool Object::Release() noexcept
{
	if (refCount_.fetch_sub(1, std::memory_order_relaxed) == 1)
	{
		delete this;
		return true;
	}

	return false;
}

ExclusiveObjectAccess::ExclusiveObjectAccess()
	:used_(ATOMIC_FLAG_INIT)
{

}

void ExclusiveObjectAccess::Open(ObjectAccessContext& context)
{
	kassert(used_.test_and_set(std::memory_order_relaxed));
	context.AccessToken = context.AccessAcquired;
	OnFirstOpen();
}

void ExclusiveObjectAccess::Close(ObjectAccessContext& context)
{
	OnLastClose();
	used_.clear(std::memory_order_relaxed);
	context.AccessToken = OA_None;
}

void ExclusiveObjectAccess::OnFirstOpen()
{

}

void ExclusiveObjectAccess::OnLastClose()
{

}

void FreeObjectAccess::Open(ObjectAccessContext& context)
{
	context.AccessToken = context.AccessAcquired;
	if (useCount_.fetch_add(1, std::memory_order_relaxed) == 0)
		OnFirstOpen();
}

void FreeObjectAccess::Close(ObjectAccessContext& context)
{
	context.AccessToken = OA_None;
	if (useCount_.fetch_sub(1, std::memory_order_relaxed) == 1)
		OnLastClose();
}

FreeObjectAccess::FreeObjectAccess()
	:useCount_(0)
{

}

void FreeObjectAccess::OnFirstOpen()
{

}

void FreeObjectAccess::OnLastClose()
{

}

void Chino::ValidateAccess(ObjectAccessContext& context, ObjectAccess accessRequried)
{
	kassert((context.AccessToken & accessRequried) == accessRequried);
}
