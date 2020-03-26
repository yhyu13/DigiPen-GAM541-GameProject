#pragma once

#include "SupportSkill.h"

namespace gswy
{
	class Forking : public SupportSkill
	{
	public:

		Forking();
		~Forking();

		const int& GetCount();
		void SetCount(const int count);
		virtual void HandleSkill(BaseSkill* skill) override;
		virtual void RemoveSkill(std::shared_ptr<BaseSkill> skill) override;

	private:

		int m_count;
	};
}
