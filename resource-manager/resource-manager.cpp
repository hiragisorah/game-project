#include "resource-manager.h"

class ResourceManager::Impl
{
public:
	Impl(ResourceManager * graphics)
		: owner_(graphics)
	{
		if (this->singleton_)
		{
			throw std::exception("graphics is singleton.");
		}

		Impl::singleton_ = this;
	}

	virtual ~Impl(void)
	{
		Impl::singleton_ = nullptr;
	}

	ResourceManager * owner_;

	static Impl * singleton_;
};

ResourceManager::Impl * ResourceManager::Impl::singleton_ = nullptr;

ResourceManager::ResourceManager(void) noexcept(false)
	: impl_(std::make_unique<Impl>(this))
{

}

ResourceManager::ResourceManager(ResourceManager && move_from) noexcept
	: impl_(std::move(move_from.impl_))
{
	this->impl_->owner_ = this;
}

ResourceManager & ResourceManager::operator=(ResourceManager && move_from) noexcept
{
	this->impl_ = std::move(move_from.impl_);
	this->impl_->owner_ = this;
	return *this;
}

ResourceManager::~ResourceManager(void)
{
}

ResourceManager & ResourceManager::Get(void)
{
	if (!Impl::singleton_ || !Impl::singleton_->owner_)
		throw std::exception("resource-manager is a singleton.");

	return *Impl::singleton_->owner_;
}